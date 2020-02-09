#include "lexp-networksmgr.hpp"
#include "networksmgr.hpp"

EXPORT_LUABINDING(lexp::LExp_NetworksMgr, LExp_NetworksMgr);

void lexp::LExp_NetworksMgr::on_register(lua_State* L)
{
	GRT_LUA_STACKCHECK_BEGIN(L);

	lua_getglobal(L, "gamert");

	// networksmgr - create a lua object
	lua_newtable(L);

	// add a function - startup()
	lua_pushcfunction(L, LExp_NetworksMgr::_luafunc_startup);
	lua_setfield(L, -2, "startup");

	// add a function - shutdown()
	lua_pushcfunction(L, LExp_NetworksMgr::_luafunc_shutdown);
	lua_setfield(L, -2, "shutdown");
	
	// add a function - reconnect()
	lua_pushcfunction(L, LExp_NetworksMgr::_luafunc_reconnect);
	lua_setfield(L, -2, "reconnect");

	lua_setfield(L, -2, "networksmgr");

	lua_pop(L, -1);
	
	GRT_LUA_STACKCHECK_END(L);
}

int lexp::LExp_NetworksMgr::_luafunc_startup(lua_State* L)
{
	NetworksMgr::get_instance().startup();
	return 0;
}

int lexp::LExp_NetworksMgr::_luafunc_shutdown(lua_State* L)
{
	NetworksMgr::get_instance().shutdown();
	return 0;
}

int lexp::LExp_NetworksMgr::_luafunc_reconnect(lua_State* L)
{
	int argc = lua_gettop(L);

	bool param_force = false; // default

	if (argc >= 1)
	{
		if (lua_isboolean(L, 1))
		{
			param_force = lua_toboolean(L, 1);
		}
		else
		{
			LUA_REPORT_ERROR(L, "bad arguments.");
		}
	}

	NetworksMgr::get_instance().reconnect(param_force);

	return 0;
}

