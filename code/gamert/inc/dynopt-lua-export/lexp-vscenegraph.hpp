#pragma once

#include "lexp.hpp"
#include "lexp-class.hpp"

namespace lexp
{
	class LExp_VSceneGraph : public ILuaExport
	{
	public:
		virtual void on_register(lua_State* L) override;

	public:
		static void* _lexp_class_ctor(lua_State* L);
		static void  _lexp_class_gc(lua_State* L, void* native_obj);
	};
}


