#pragma once
#include "exported.hpp"
#include <cstdint>
#include <string>

namespace memory
{
	static std::pair<std::uintptr_t, std::uint32_t> _memory_module{};

	bool initialize( const wchar_t* module_name );
	std::uintptr_t from_pattern( const char* sig, const char* mask );
}