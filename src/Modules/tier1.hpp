#pragma once

#include "Utils/Mem.hpp"
#include "sdk.hpp"

namespace tier1 {
	Module* tier1;

	Interface* cvar;

	using Create_ = int(__thiscall*)(ConVar*, const char*, const char*, int, const char*, bool, float, bool, float, FnChangeCallback_t);
	Create_ Create;

	void* ConVar_VTable = nullptr;
	void* ConVar_VTable2 = nullptr;

	ConCommandBase* (__thiscall* zFindCommandBase)(void* thisptr, const char* name);
	ConCommandBase* FindCommandBase(const char* name) {//13
		return zFindCommandBase(cvar->ThisPtr(), name);
	}

	void(__thiscall* zRegisterConCommand)(void* thisptr, ConCommandBase* ptr);
	void RegisterConCommand(ConCommandBase* ptr) {//9
		zRegisterConCommand(cvar->ThisPtr(), ptr);
	}

	bool Init() {
		tier1 = new Module("vstdlib.dll");
		if (!tier1->ptr) return false;

		cvar = tier1->GetInterface("VEngineCvar007", true);
		if (!cvar->base) return false;

		cvar->GetFunc(13, &zFindCommandBase);
		cvar->GetFunc(9, &zRegisterConCommand);

		ConVar* sv_lan = reinterpret_cast<ConVar*>(FindCommandBase("sv_lan"));
		ConVar_VTable = *(void**)sv_lan;
		ConVar_VTable2 = sv_lan->ConVar_VTable;

		Create = reinterpret_cast<Create_>((*((void***)&ConVar_VTable2))[27]);

		return true;
	}
}