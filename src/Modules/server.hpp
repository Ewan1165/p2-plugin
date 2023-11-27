#pragma once

#include "Utils/Mem.hpp"
#include "sdk.hpp"

namespace server {
	Module* server = nullptr;
	Interface* serverTools = nullptr;

	void* FirstEnt() {
		return serverTools->CallFunc<void*>(7, false);
	}

	void* NextEnt(void* prev) {
		return serverTools->CallFunc<void*, void*>(8, false, prev);
	}

	void* EntByHammerId(int id) {
		return serverTools->CallFunc<void*, int>(9, false, id);
	}

	void* CreateEntByName(const char* name) {
		return serverTools->CallFunc<void*, const char*>(14, false, name);
	}
	void DispatchSpawn(void* ent) {
		serverTools->CallFunc<void, void*>(15, false, ent);
	}

	bool SetKeyValueChar(void* ent, const char* key, const char* val) {
		return serverTools->CallFunc<bool, void*, const char*, const char*>(13, false, ent, key, val);
	}
	bool SetKeyValueFloat(void* ent, const char* key, float val) {
		return serverTools->CallFunc<bool, void*, const char*, float>(12, false, ent, key, val);
	}
	bool SetKeyValueVector(void* ent, const char* key, Vector val) {
		return serverTools->CallFunc<bool, void*, const char*, Vector&>(11, false, ent, key, val);
	}

	bool GetKeyValue(void* ent, const char* key, char* val, int maxLen) {
		return serverTools->CallFunc<bool, void*, const char*, char*, int>(10, false, ent, key, val, maxLen);
	}

	uintptr_t findByName;
	uintptr_t gEntList;

	bool Init() {
		server = new Module("server.dll");
		if (server->ptr == nullptr) return false;

		serverTools = server->GetInterface("VSERVERTOOLS001");
		if (serverTools->ptr == nullptr) return false;
		
		gEntList = ((uintptr_t)(*reinterpret_cast<void**>(serverTools->ptr[1] + 61))-4);
		findByName = gEntList + 100367904;

		return true;
	}
}