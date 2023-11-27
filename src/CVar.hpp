#pragma once

#include <vector>

#include "Modules/tier1.hpp"
#include "sdk.hpp"

class CVar;

std::vector<CVar*> cvars;

class CVar {
public:
	ConVar* ptr;
	bool registered = false;

	CVar(ConVar* base) : ptr(base) {
		cvars.push_back(this);
	}

	CVar(const char* name) {
		this->ptr = reinterpret_cast<ConVar*>(tier1::FindCommandBase(name));
		this->registered = true;
		cvars.push_back(this);
	}

	CVar(const char* name, const char* val, const char* helpstr, int flags = FCVAR_NEVER_AS_STRING, bool hasMin = false, int min = 0, bool hasMax = false, int max = 0) {
		this->ptr = new ConVar(name, val, flags, helpstr, hasMin, min, hasMax, max);
		cvars.push_back(this);
	}

	const char* GetStr() {
		return ptr->m_pszString;
	}

	float GetFloat() {
		return ptr->m_fValue;
	}

	int GetInt() {
		return ptr->m_nValue;
	}

	bool GetBool() {
		return !!this->GetInt();
	}

	void Register() {
		*(void**)this->ptr = tier1::ConVar_VTable;
		this->ptr->ConVar_VTable = tier1::ConVar_VTable2;
		tier1::Create(this->ptr, this->ptr->m_pszName, this->ptr->m_pszDefaultValue, this->ptr->m_nFlags, this->ptr->m_pszHelpString, this->ptr->m_bHasMin, this->ptr->m_fMinVal, this->ptr->m_bHasMax, this->ptr->m_fMaxVal, nullptr);
		this->registered = true;
	}
};

void RegisterAllCVars() {
	for (CVar* cvar : cvars) {
		if (cvar->registered) continue;
		cvar->Register();
	}
}