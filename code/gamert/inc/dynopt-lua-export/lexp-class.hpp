#pragma once

#include "lexp.hpp"

namespace lexp
{
	namespace LuaClassesMgr
	{
		typedef void* (*class_ctor_fn)(lua_State* L);
		typedef void  (*class_gc_fn)(lua_State* L, void* native_obj);

		/*
		 * @func: register_lua_class
		 * @remarks: this function will leave a lua table on the top of the stack L
		 */
		void register_lua_class(
			const std::type_info& lexp_class,
			class_ctor_fn ctor,
			class_gc_fn gc,
			lua_State* L);

		namespace _internal
		{
			typedef struct _lua_class_info
			{
				class_ctor_fn	class_ctor;
				class_gc_fn		class_gc;
			} lua_class_info;

			extern std::unordered_map<std::string, lua_class_info> classes;
			
			int luafunc_ctor(lua_State* L);
			int luafunc_gc(lua_State* L);

		}
	}
}


