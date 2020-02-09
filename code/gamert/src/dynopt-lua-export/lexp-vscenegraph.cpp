#include "lexp-vscenegraph.hpp"
#include "vscenegraph.hpp"

EXPORT_LUABINDING(lexp::LExp_VSceneGraph, LExp_VSceneGraph);

void lexp::LExp_VSceneGraph::on_register(lua_State* L)
{
	GRT_LUA_STACKCHECK_BEGIN(L);

	lua_getglobal(L, "gamert");
	lua_getglobal(L, "BtClass");				// get the func BtClass() to call

	lua_pushcfunction(L, LExp_VSceneGraph::_luafunc_ctor);	// param: ctor
	GRT_LUA_CHECK(L, lua_pcall(L, 1, 1, 0));	// call BtClass(ctor)

	lua_setfield(L, -2, "VSceneGraph");			// set gamert["VSceneGraph"]

	lua_pop(L, 1);		// gamert

	GRT_LUA_STACKCHECK_END(L);
}

int lexp::LExp_VSceneGraph::_luafunc_ctor(lua_State* L)
{
	VSceneGraph* nobj = new VSceneGraph();

	try
	{
		int c = lua_gettop(L);

		// obj["_LEXP_FIELD_NATIVE_OBJ"] = nobj
		lua_pushinteger(L, (lua_Integer)nobj);
		lua_setfield(L, -2, lexp::_LEXP_FIELD_NATIVE_OBJ);	


	}
	catch (const std::exception & ex)
	{
		delete nobj;
	}

	
	return 0;
}


