#pragma once

#include <format>
#include <string>
#include <fstream>

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
void logf(const std::format_string<Args...> fmt, Args&&... args) {
	tier0::Msg(std::vformat(fmt.get(), std::make_format_args(args...)).c_str());
}

void log(const char* str) {
	tier0::Msg(str);
}

void log(int number) {
	int len = ceil(log10(number+1));
	char* buff = reinterpret_cast<char*>(malloc(sizeof(char) * (len+1)));
	std::to_chars(buff, buff + len, number);
	buff[len] = 0;
	tier0::Msg(buff);
}

void log(size_t number) {
	log(static_cast<int>(number));
}