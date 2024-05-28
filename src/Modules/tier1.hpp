#pragma once

#include "Utils/Mem.hpp"
#include "sdk.hpp"

namespace tier1 {
	Module* tier1;

	Interface* cvar;

	using Create_ = int(thiscall*)(ConVar*, const char*, const char*, int, const char*, bool, float, bool, float, FnChangeCallback_t);
	Create_ Create;

	void* ConVar_VTable = nullptr;
	void* ConVar_VTable2 = nullptr;

	ConCommandBase* FindCommandBase(const char* name) {
		return cvar->CallFunc<ConCommandBase*, const char*>(13, false, name);
	}

	void RegisterConCommand(ConCommandBase* ptr) {
		cvar->CallFunc<void, ConCommandBase*>(9, false, ptr);
	}

	void __fastcall RegisterConCommand_Hook(void* thisptr, ConCommandBase* ptr) {
		log("hi");
	}

	bool Init() {
		tier1 = new Module("vstdlib.dll");
		if (!tier1->ptr) return false;

		cvar = tier1->GetInterface("VEngineCvar007", true);
		if (!cvar->base) return false;

		ConVar* sv_lan = reinterpret_cast<ConVar*>(FindCommandBase("sv_lan"));
		ConVar_VTable = *(void**)sv_lan;
		ConVar_VTable2 = sv_lan->ConVar_VTable;

		Create = reinterpret_cast<Create_>((*((void***)&ConVar_VTable2))[27]);

		cvar->Hook(RegisterConCommand_Hook, 9);

		return true;
	}
}