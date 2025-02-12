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

	static Entity CreateByClassname(const char* classname, const char* targetname, Vector origin = Vector(0, 0, 0), const char* model = "") {
		Entity ent(server::CreateEntByName(classname));
		ent.SetKeyval("origin", origin);
		ent.SetKeyval("targetname", targetname);
		server::DispatchSpawn(ent.base);
		if (model != "") {
			ent.SetKeyval("model", model);
		}
		return ent;
	}

	static std::vector<Entity> GetAllByKeyValue(const char* key, const char* value) {
		std::vector<Entity> vec;
		Entity ent = Entity::First();
		char* currKeyVal = reinterpret_cast<char*>(malloc(sizeof(char)*128));
		while (ent.base) {
			server::GetKeyValue(ent.base, key, currKeyVal, 128);
			if (!strcmp(currKeyVal, value)) {
				vec.push_back(ent);
			}
			ent = Entity::NextEnt(ent);
		}
		return vec;
	}

	char* GetKeyVal(const char* key) {
		char* buff = reinterpret_cast<char*>(malloc(sizeof(char)*128));

		if (server::GetKeyValue(this->base, key, buff, 128)) {
			return buff;
		}
		return nullptr;
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