#pragma once

#include "sdk.hpp"
#include "Modules/server.hpp"

class Entity {
public:
	void* base;

	Entity(void* base) : base(base) {}

	static Entity First() {
		return Entity(server::FirstEnt());
	}
	static Entity NextEnt(Entity prev) {
		return Entity(server::NextEnt(prev.base));
	}
	static Entity ByKeyval(const char* key, const char* val) {
		char buffer[sizeof(val)/sizeof(char)];
		void* curr = server::FirstEnt();
		for (;;) {
			server::GetKeyValue(curr, key, buffer, sizeof(buffer));
			if (buffer == val) {
				return Entity(curr);
			}
			curr = server::NextEnt(curr);
			if (curr == nullptr) return nullptr;
		}
	}
	static Entity ByHammerId(int id) {
		return Entity(server::EntByHammerId(id));
	}

	bool SetKeyval(const char* key, const char* val) {
		return server::SetKeyValueChar(base, key, val);
	}
	bool SetKeyval(const char* key, float val) {
		return server::SetKeyValueFloat(base, key, val);
	}
	bool SetKeyval(const char* key, Vector val) {
		return server::SetKeyValueVector(base, key, val);
	}
};