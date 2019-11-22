#pragma once

#if defined(WIN32)
#	include <Windows.h>
#	define VK_USE_PLATFORM_WIN32_KHR
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
#include <cassert>
#include <functional>

// GRT Macros - Game-RT Macros
#define GRT_CHECK(x, msg) if(!(x)){ throw std::runtime_error(msg);}
#define GRT_VULKAN_FACTOR_GETTER(factor_type, factor_name, private_var_name)	\
	factor_type get_vulkan_##factor_name() const	{return private_var_name;}

#define GRT_SAFE_DELETE(ptr)		if(ptr) { delete ptr; ptr = nullptr; }
#define GRT_SAFE_DELETE_ARRAY(ptr)	if(ptr) { delete[] ptr; ptr = nullptr; }


