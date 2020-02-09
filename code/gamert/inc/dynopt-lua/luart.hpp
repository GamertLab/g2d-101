#pragma once

#include "pre-req.hpp"
#include "singleton.hpp"
#include "lua.hpp"


// lua run-time
namespace luart
{

	/* ************************************************************************
	 * game - a lua object
	 * ***********************************************************************/
	namespace game
	{
		void app_init();
		void app_uninit();
	}

	/* ************************************************************************
	 * functions
	 * ***********************************************************************/

	void init_runtime();
	void uninit_runtime();

	/* ************************************************************************
	 * variables
	 * ***********************************************************************/

	extern lua_State*		global_state;


	/* ************************************************************************
	 * internal implementations
	 * ***********************************************************************/
	namespace _internal
	{
		void config_lua_package();
		void boot();
		void load_requires();
	}
}

