#pragma once

#include <format>
#include <string>

#include "Utils/Mem.hpp"
#include "sdk.hpp"

namespace engine {
	Module* engine;
	Interface* engineClient;
		
	int ExecuteCmd(const char* cmd) {
		return engineClient->CallFunc<int, const char*>(104, false, cmd);
	}

	void GetViewAngles(QAngle ang) {
		engineClient->CallFunc<int, QAngle&>(18, false, ang);
	}

	bool Init() {
		engine = new Module("engine.dll");
		if (engine->ptr == nullptr) return false;

		engineClient = engine->GetInterface("VEngineClient015");
		if (engineClient->ptr == nullptr) return false;

		return true;
	}
}