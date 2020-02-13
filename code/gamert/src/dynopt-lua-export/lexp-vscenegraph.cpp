#include "lexp-vscenegraph.hpp"
#include "vscenegraph.hpp"

EXPORT_LUABINDING(lexp::LExp_VSceneGraph, LExp_VSceneGraph);

void lexp::LExp_VSceneGraph::on_register(lua_State* L)
{
	GRT_LUA_STACKCHECK_BEGIN(L);

	lua_getglobal(L, "gamert");
	LuaClassesMgr::register_lua_class(
		typeid(LExp_VSceneGraph),
		LExp_VSceneGraph::_lexp_class_ctor,
		LExp_VSceneGraph::_lexp_class_gc,
		L);

	lua_setfield(L, -2, "VSceneGraph");			// set gamert["VSceneGraph"]
	lua_pop(L, 1);

	GRT_LUA_STACKCHECK_END(L);
}

void* lexp::LExp_VSceneGraph::_lexp_class_ctor(lua_State* L)
{
	return new VSceneGraph();
}

void lexp::LExp_VSceneGraph::_lexp_class_gc(lua_State* L, void* native_obj)
{
	delete (VSceneGraph*)native_obj;
}

