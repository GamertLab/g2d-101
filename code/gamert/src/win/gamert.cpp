#include "osenv.hpp"

#include "vkapp.hpp"
#include "vkcontext.hpp"
#include "vkrenderer2d.hpp"
#include "vscenegraph2d.hpp"
#include "vnode-quad2d.hpp"
#include "vmatrix.hpp"

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

typedef void(*update_frame_fnp_t)(void);

VKRenderer2d	g_render;
VSceneGraph2d	g_scene;

VNode2d* vn_mid = nullptr;;
VNode2d* vn_left = nullptr;;
VNode2d* vn_right = nullptr;;

void dummy_update()
{}

void render_update()
{
	static float movex = -300.f;
	static float movey = 0.f;
	static bool dir_left = true;
	static bool dir_up = true;

	if (dir_left)
	{
		movex -= 2.f;
		if (movex < -450.f)
			dir_left = false;
	}
	else
	{
		movex += 2.f;
		if (movex > -150.f)
			dir_left = true;
	}

	if (dir_up)
	{
		movey += 2.f;
		if (movey > 150.f)
			dir_up = false;
	}
	else
	{
		movey -= 2.f;
		if (movey < -150.f)
			dir_up = true;
	}


	vn_left->set_poisition(VFVec2({movex, 0.f}));
	vn_mid->set_poisition(VFVec2({ 0.f, movey }));

	g_render.update(30.f);
}

update_frame_fnp_t g_update_function = dummy_update;

void init_gamert_app(HWND hwnd)
{
	VKContext::get_instance().init("gamert",
		"gamert engine",
		VK_MAKE_VERSION(1, 0, 0),
		VK_MAKE_VERSION(1, 0, 0),
		hwnd);

	VKContext::get_instance().register_renderer(&g_render);
	VKContext::get_instance().resize();

	VNode* root = new VNode();
	root->set_name("root");

	{ // middle
		VNodeQuad2d* quad = new VNodeQuad2d();
		quad->set_scale(VFVec2({ 150.f, 150.0f }));
		root->manage_child(quad);
		vn_mid = quad;
	}

	{ // left
		VNodeQuad2d* quad = new VNodeQuad2d();
		quad->set_poisition_fast(VFVec2({ -300.0f, 0.0f }));
		quad->set_scale_fast(VFVec2({ 200.f, 200.0f }));
		quad->calculate_world();
		root->manage_child(quad);
		vn_left = quad;
	}

	{ // right
		VNodeQuad2d* quad = new VNodeQuad2d();
		quad->set_poisition_fast(VFVec2({ 300.0f, 0.0f }));
		quad->set_scale_fast(VFVec2({ 100.f, 100.0f }));
		quad->calculate_world();
		root->manage_child(quad);
		vn_right = quad;
	}

	g_scene.init();
	g_scene.switch_root_node(root);
	g_render.bind_scene_graph(&g_scene);
	g_update_function = render_update;
}

void uninit_gamert_app()
{
	g_scene.uninit();

	VNode* root = g_scene.switch_root_node(nullptr);
	delete root;	// root will release all its children.

	g_render.unint();
	VKContext::get_instance().destroy();
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR pCmdLine, int nCmdShow)
{
	// Register the window class.
	const wchar_t CLASS_NAME[] = L"Gamert Window Class";
	const wchar_t MAINWIN_NAME[] = L"Gamert View";
	const int MAINWIN_WIDTH = 1000;
	const int MAINWIN_HEIGHT = 800;

	WNDCLASS wc = { };

	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.lpszClassName = CLASS_NAME;

	RegisterClass(&wc);

	// Create the window.

	HWND hwnd = CreateWindowEx(
		0,                              // Optional window styles.
		CLASS_NAME,                     // Window class
		MAINWIN_NAME,    // Window text
		WS_OVERLAPPEDWINDOW,            // Window style

		// Size and position
		CW_USEDEFAULT, CW_USEDEFAULT, MAINWIN_WIDTH, MAINWIN_HEIGHT,

		NULL,       // Parent window    
		NULL,       // Menu
		hInstance,  // Instance handle
		NULL        // Additional application data
	);

	if (hwnd == NULL)
	{
		return 0;
	}

	ShowWindow(hwnd, nCmdShow);
	
	init_gamert_app(hwnd);

	// Run the message loop.
	MSG msg = { };
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, NULL, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			g_update_function();
			Sleep(16);
		}
	}

	VKContext::get_instance().wait_device_idle();
	uninit_gamert_app();

	return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_SIZE:
		VKContext::get_instance().resize();
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

