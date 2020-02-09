#pragma once

#include "lexp.hpp"

namespace lexp
{
	class LExp_VSceneGraph : public ILuaExport
	{
	public:
		virtual void on_register(lua_State* L) override;

	public:
		static int _luafunc_ctor(lua_State* L);
		static int _luafunc_gc(lua_State* L);
	};
}


