#include "luart.hpp"
#include "resmgr-static.hpp"
#include "lexp.hpp"
#include "config-dynopt.hpp"


/* ************************************************************************
 * luart - lua runtime
 * ***********************************************************************/

lua_State* luart::global_state = nullptr;

void luart::init_runtime()
{
	global_state = luaL_newstate();
	luaL_openlibs(global_state);

	luart::_internal::config_lua_package();
	luart::_internal::boot();
	lexp::LuaExportRegistry::get_instance().register_entries(global_state);
	luart::_internal::load_requires();
}

void luart::uninit_runtime()
{
	if (global_state)
	{
		lua_close(global_state);
		global_state = nullptr;
	}
}


/* ************************************************************************
 * internal implementations
 * ***********************************************************************/

void luart::_internal::config_lua_package()
{
	// get "package"
	lua_getglobal(global_state, "package");

#if defined(WIN32)
	static const char* path_suffix = "\\scripts\\?.lua";
#else
	static const char* path_suffix = "/scripts/?.lua";
#endif

	std::string package_path =
		ResMgrStatic::get_instance()
			.get_resources_root_path() + path_suffix;

	lua_pushstring(
		global_state,
		package_path.c_str());

	lua_setfield(global_state, -2, "path");

	// pop "package"
	lua_pop(global_state, 1);
}

void luart::_internal::boot()
{
#define _LUART_REQUIRE(mod) "require(\"boot/" mod "\")\n"

	const std::string boot_list =
		_LUART_REQUIRE("class")
		_LUART_REQUIRE("game");
	int status = luaL_dostring(global_state, boot_list.c_str());
	if (status && lua_isstring(global_state, -1))
	{
		// error encountered
		const char* msg = lua_tostring(global_state, -1);
		throw std::runtime_error(msg);
	}
}

void luart::_internal::load_requires()
{
	for (const auto& req : ConfigDynopt::luart::require_list)
	{
		std::string relname = "scripts/" + req + ".lua";
		int status = luaL_dofile(
			global_state,
			ResMgrStatic::get_instance()
				.fullpath(relname)
					.c_str());

		if (status && lua_isstring(global_state, -1))
		{
			// error encountered
			const char* msg = lua_tostring(global_state, -1);
			throw std::runtime_error(msg);
		}
	}
}



/* ************************************************************************
 * game - a lua object
 * ***********************************************************************/

void luart::game::app_init()
{
	int tops = lua_gettop(global_state);

	lua_getglobal(global_state, "game");
	lua_getfield(global_state, -1, "on_app_init");
	lua_pushvalue(global_state, -2);

	GRT_LUA_CHECK(global_state, lua_pcall(global_state, 1, 0, 0));
	lua_pop(global_state, 1);

	GRT_CHECK(
		tops == lua_gettop(global_state),
		"stack is unbalanced.");
}


