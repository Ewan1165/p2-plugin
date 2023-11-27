#pragma once

#include <format>
#include <string>

#include "Utils/Mem.hpp"

namespace tier0 {
	using MsgType = void(*)(const char*);

	Module* lib;

	MsgType Msg;

	bool Init() {
		lib = new Module("tier0.dll");
		if (!lib->ptr) return false;

		Msg = lib->GetSymbol<MsgType>("Msg");
		if (!Msg) return false;

		return true;
	}
}

template<typename... Args>
void log(const std::format_string<Args...> fmt, Args&&... args) {
	tier0::Msg(std::vformat(fmt.get(), std::make_format_args(args...)).c_str());
	tier0::Msg("\n");
}