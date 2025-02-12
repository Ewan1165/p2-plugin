#pragma once

#include <format>
#include <string>

#include "Utils/Mem.hpp"
#include "sdk.hpp"

namespace engine {
	Module* engine;
	Interface* engineClient;
	Interface* engineTool;
	
	bool(__thiscall* zGetPlayerInfo)(void* thisptr, int idx, player_info_s* player_info);
	bool GetPlayerInfo(int idx, player_info_s* player_info) {
		return zGetPlayerInfo(engineClient->ThisPtr(), idx, player_info);
	}

	int(__thiscall * zClientCmd)(void* thisptr, const char* cmdString);
	int ExecuteCmd(const char* cmd) {
		return zClientCmd(engineClient->ThisPtr(), cmd);
	}

	const char* (__thiscall* zGetCurrentMap)(void* thisptr);
	const char* GetCurrentMap() {
		return zGetCurrentMap(engineTool->ThisPtr());
	}

	void(__thiscall* zChangeToMap)(void* thisptr, const char* mapname);
	void ChangeToMap(const char* mapname) {
		zChangeToMap(engineTool->ThisPtr(), mapname);
	}

	void(__thiscall* zSetTimescale)(void* thisptr, float scale);
	void SetTimescale(float scale) {
		zSetTimescale(engineTool->ThisPtr(), scale);
	}

	bool Init() {
		engine = new Module("engine.dll");
		if (engine->ptr == nullptr) return false;

		engineClient = engine->GetInterface("VEngineClient015");
		if (engineClient->ptr == nullptr) return false;
		engineTool = engine->GetInterface("VENGINETOOL003");
		if (engineTool->ptr == nullptr) return false;

		engineClient->GetFunc(8, &zGetPlayerInfo);
		engineClient->GetFunc(104, &zClientCmd);

		engineTool->GetFunc(25, &zGetCurrentMap);
		engineTool->GetFunc(26, &zChangeToMap);
		engineTool->GetFunc(35, &zSetTimescale);
		return true;
	}
}