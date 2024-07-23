#pragma once

#include <format>
#include <string>

#include "Utils/Mem.hpp"
#include "sdk.hpp"

namespace engine {
	Module* engine;
	Interface* engineClient;
	
	int(__thiscall * zClientCmd)(void* thisptr, const char* cmdString);

	int ExecuteCmd(const char* cmd) {
		return zClientCmd(engineClient->ThisPtr(), cmd);
	}

	bool Init() {
		engine = new Module("engine.dll");
		if (engine->ptr == nullptr) return false;

		engineClient = engine->GetInterface("VEngineClient015");
		if (engineClient->ptr == nullptr) return false;

		engineClient->GetFunc(104, &zClientCmd);

		return true;
	}
}