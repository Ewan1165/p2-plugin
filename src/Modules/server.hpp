#pragma once

#include "Utils/Mem.hpp"
#include "sdk.hpp"

namespace server {
	Module* server = nullptr;
	Interface* serverTools = nullptr;

	void* (__thiscall* zFirstEnt)(void* thisPtr);
	void* FirstEnt() {//7
		return zFirstEnt(serverTools->ThisPtr());
	}

	void* (__thiscall* zNextEnt)(void* thisPtr, void* prev);
	void* NextEnt(void* prev) {//8
		return zNextEnt(serverTools->ThisPtr(), prev);
	}

	void* (__thiscall* zEntByHammerId)(void* thisPtr, int id);
	void* EntByHammerId(int id) {//9
		return zEntByHammerId(serverTools->ThisPtr(), id);
	}

	void* (__thiscall* zCreateEntByName)(void* thisPtr, const char* name);
	void* CreateEntByName(const char* name) {//14
		return zCreateEntByName(serverTools->ThisPtr(), name);
	}

	void(__thiscall* zDispatchSpawn)(void* thisPtr, void* ent);
	void DispatchSpawn(void* ent) {//15
		zDispatchSpawn(serverTools->ThisPtr(), ent);
	}

	bool(__thiscall* zSetKeyValueChar)(void* thisPtr, void* ent, const char* key, const char* val);
	bool SetKeyValueChar(void* ent, const char* key, const char* val) {//13
		return zSetKeyValueChar(serverTools->ThisPtr(), ent, key, val);
	}

	bool(__thiscall* zSetKeyValueFloat)(void* thisPtr, void* ent, const char* key, float val);
	bool SetKeyValueFloat(void* ent, const char* key, float val) {//12
		return zSetKeyValueFloat(serverTools->ThisPtr(), ent, key, val);
	}

	bool(__thiscall* zSetKeyValueVector)(void* thisPtr, void* ent, const char* key, Vector& val);
	bool SetKeyValueVector(void* ent, const char* key, Vector val) {//11
		return zSetKeyValueVector(serverTools->ThisPtr(), ent, key, val);
	}

	bool(__thiscall* zGetKeyValue)(void* thisPtr, void* ent, const char* key, char* val, int maxLen);
	bool GetKeyValue(void* ent, const char* key, char* val, int maxLen) {//10
		return zGetKeyValue(serverTools->ThisPtr(), ent, key, val, maxLen);
	}

	bool Init() {
		server = new Module("server.dll");
		if (server->ptr == nullptr) return false;

		serverTools = server->GetInterface("VSERVERTOOLS001");
		if (serverTools->ptr == nullptr) return false;

		serverTools->GetFunc(7, &zFirstEnt);
		serverTools->GetFunc(8, &zNextEnt);
		serverTools->GetFunc(9, &zEntByHammerId);
		serverTools->GetFunc(14, &zCreateEntByName);
		serverTools->GetFunc(15, &zDispatchSpawn);
		serverTools->GetFunc(13, &zSetKeyValueChar);
		serverTools->GetFunc(12, &zSetKeyValueFloat);
		serverTools->GetFunc(11, &zSetKeyValueVector);
		serverTools->GetFunc(10, &zGetKeyValue);

		return true;
	}
}