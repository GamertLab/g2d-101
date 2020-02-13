#include "lexp-class.hpp"


std::unordered_map<std::string, lexp::LuaClassesMgr::_internal::lua_class_info> lexp::LuaClassesMgr::_internal::classes;

void lexp::LuaClassesMgr::register_lua_class(
			const std::type_info& lexp_class,
			class_ctor_fn ctor,
			class_gc_fn gc,
			lua_State* L)
{
	const auto& entry = _internal::classes.find(lexp_class.name());
	if (entry == _internal::classes.cend())
	{
		GRT_LUA_STACKCHECK_BEGIN(L);

		lua_getglobal(L, "BtClass");				// get the func BtClass() to call
		lua_pushcfunction(L, lexp::LuaClassesMgr::_internal::luafunc_ctor);	// param: ctor
		GRT_LUA_CHECK(L, lua_pcall(L, 1, 1, 0));	// call BtClass(ctor)

		// prototype["_LEXP_FIELD_NATIVE_LEXP_CLASS_NAME"] = lexp_class.name()
		lua_pushstring(L, lexp_class.name());
		lua_setfield(L, -2, lexp::_LEXP_FIELD_NATIVE_LEXP_CLASS_NAME);

		// remain the created class obj in the top of stack
		GRT_LUA_STACKCHECK_END_OFFSET(L, 1);

		// store the ctor in the registry
		_internal::lua_class_info info;
		info.class_ctor = ctor;
		info.class_gc = gc;
		_internal::classes[lexp_class.name()] = info;
	}
	else
	{
		throw std::runtime_error("the lua class has been already registered.");
	}
}

int lexp::LuaClassesMgr::_internal::luafunc_ctor(lua_State* L)
{
	// get lexp class name
	lua_getfield(L, -1, lexp::_LEXP_FIELD_NATIVE_LEXP_CLASS_NAME);
	const char* lexpname = lua_tostring(L, -1);
	lua_pop(L, 1);

	auto& entry = _internal::classes.find(lexpname);
	if (entry != _internal::classes.end())
	{
		GRT_LUA_STACKCHECK_BEGIN(L);

		void* native_obj = entry->second.class_ctor(L);

		// obj["_LEXP_FIELD_NATIVE_OBJ"] = nobj
		lua_pushinteger(L, (lua_Integer)native_obj);
		lua_setfield(L, -2, lexp::_LEXP_FIELD_NATIVE_OBJ);	

		// prepare metatable
		lua_newtable(L);
		lua_pushcfunction(L, lexp::LuaClassesMgr::_internal::luafunc_gc);
		lua_setfield(L, -2, "__gc");		// {__gc = native::ongc}

		// set metatable
		lua_setmetatable(L, -2);

		GRT_LUA_STACKCHECK_END(L);
	}
	else
	{
		throw std::runtime_error("the lexp class name not found.");
	}


	return 0;
}

int lexp::LuaClassesMgr::_internal::luafunc_gc(lua_State* L)
{
	// get lexp class name
	lua_getfield(L, -1, lexp::_LEXP_FIELD_NATIVE_LEXP_CLASS_NAME);
	const char* lexpname = lua_tostring(L, -1);
	lua_pop(L, 1);

	auto& entry = _internal::classes.find(lexpname);
	if (entry != _internal::classes.end())
	{
		GRT_LUA_STACKCHECK_BEGIN(L);

		lua_getfield(L, -1, lexp::_LEXP_FIELD_NATIVE_OBJ);
		int64_t native_obj_addr = lua_tointeger(L, -1);
		lua_pop(L, 1);

		if (entry->second.class_gc)
		{
			entry->second.class_gc(L, (void*)native_obj_addr);
		}

		GRT_LUA_STACKCHECK_END(L);
	}
	else
	{
		throw std::runtime_error("the lexp class name not found.");
	}

	return 0;
}










