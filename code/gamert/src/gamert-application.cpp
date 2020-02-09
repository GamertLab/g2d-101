#include "gamert-application.hpp"
#include "configmgr.hpp"
#include "networksmgr.hpp"
#include "luart.hpp"
#include "joystick.hpp"
#include "vkcontext.hpp"
#include "resmgr-runtime.hpp"

GamertApplication::GamertApplication()
{}

GamertApplication::~GamertApplication()
{}

void GamertApplication::init()
{
	// load configuration
	ConfigMgr::get_instance().load_config();


	// init vk layer
	this->on_init_vklayer();
	
	// init joystick. TODO: let luart do this job.
	JoyStick::get_instance().check_controllers();



	this->on_init_renderers();




	// start lua runtime
	luart::init_runtime();

	luart::game::app_init();
}

void GamertApplication::uninit()
{
	VKContext::get_instance().wait_device_idle();
	
	NetworksMgr::get_instance().shutdown();

	ResMgrRuntime::get_instance().purge_visual_scenes();

	luart::uninit_runtime();

	this->on_uninit_renderers();
	this->on_uninit_vklayer();
}

void GamertApplication::do_min_gameframe()
{
	this->on_min_gameframe();
}

void GamertApplication::sized()
{
	this->on_sized();
}

float GamertApplication::min_gameframe_interval() const
{
	return ConfigMaster::min_gameframe_interval;
}

