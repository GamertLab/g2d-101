#pragma once

#include <cstring>

#if defined(WIN32)
#	include <Windows.h>
#	define VK_USE_PLATFORM_WIN32_KHR
#	define __FILENAME__ (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)
#else
#	define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)
#endif

#include <stdexcept>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdint>
#include <set>
#include <algorithm>
#include <vector>
#include <string>
#include <map>
#include <array>
#include <cstdarg>
#include <unordered_map>
#include <unordered_set>
#include <cassert>
#include <functional>
#include <any>
#include <regex>
#include <filesystem>

// GRT Macros - Game-RT Macros
#if defined(DEBUG) || defined(_DEBUG)
#	define GRT_CHECK(x, msg) if(!(x)){ throw std::runtime_error(msg);}
#else
#	define GRT_CHECK(x, msg) void
#endif

#define GRT_IS_STRING_EQUAL(str1, str2)		\
			0 == strcmp(str1, str2)
#define GRT_IS_CLASS(obj, class_name)	\
			nullptr != dynamic_cast<class_name>(obj)
#define GRT_IS_CLASS_PTR(ptr, class_name)	\
			nullptr != dynamic_cast<class_name*>(ptr)

#define GRT_VULKAN_FACTOR_GETTER(factor_type, factor_name, private_var_name)	\
	factor_type get_vulkan_##factor_name() const	{return private_var_name;}

#define GRT_SAFE_DELETE(ptr)		if(ptr) { delete ptr; ptr = nullptr; }
#define GRT_SAFE_DELETE_ARRAY(ptr)	if(ptr) { delete[] ptr; ptr = nullptr; }

#if defined(DEBUG) || defined(_DEBUG)
#	define	GRT_LUA_STACKCHECK_BEGIN(lua_state)	\
				int _grt_lua_stackcheck_tops = lua_gettop(lua_state)
#	define	GRT_LUA_STACKCHECK_END(lua_state)	\
				GRT_CHECK(	\
					_grt_lua_stackcheck_tops == lua_gettop(lua_state),	\
					"lua stack is unbalanced")
#else
#	define	GRT_LUA_STACKCHECK_BEGIN(lua_state)	(void)
#	define	GRT_LUA_STACKCHECK_END(lua_state) (void)
#endif

#if defined(DEBUG) || defined(_DEBUG)
#	define GRT_LUA_CHECK(lua_state, expression)	\
			if((expression)) { \
				const char* msg = lua_tostring(lua_state, -1);	\
				throw std::runtime_error(msg); }
#else
#	define GRT_LUA_CHECK (void)
#endif



