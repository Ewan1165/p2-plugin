#pragma once

typedef void* (*CreateInterfaceFn)(const char* pName, int* pReturnCode);
typedef void* (*InstantiateInterfaceFn)();

#define INTERFACEVERSION_ISERVERPLUGINCALLBACKS "ISERVERPLUGINCALLBACKS003"

class IServerPluginCallbacks {
public:
	virtual bool Load(CreateInterfaceFn interfaceFactory, CreateInterfaceFn gameServerFactory) = 0;
	virtual void Unload() = 0;
	virtual void Pause() = 0;
	virtual void UnPause() = 0;
	virtual const char* GetPluginDescription() = 0;
	virtual void LevelInit(char const* pMapName) = 0;
	virtual void ServerActivate(void* pEdictList, int edictCount, int clientMax) = 0;
	virtual void GameFrame(bool simulating) = 0;
	virtual void LevelShutdown() = 0;
	virtual void ClientFullyConnect(void* pEdict) = 0;
	virtual void ClientActive(void* pEntity) = 0;
	virtual void ClientDisconnect(void* pEntity) = 0;
	virtual void ClientPutInServer(void* pEntity, char const* playername) = 0;
	virtual void SetCommandClient(int index) = 0;
	virtual void ClientSettingsChanged(void* pEdict) = 0;
	virtual int ClientConnect(bool* bAllowConnect, void* pEntity, const char* pszName, const char* pszAddress, char* reject, int maxrejectlen) = 0;
	virtual int ClientCommand(void* pEntity, const void*& args) = 0;
	virtual int NetworkIDValidated(const char* pszUserName, const char* pszNetworkID) = 0;
	virtual void OnQueryCvarValueFinished(int iCookie, void* pPlayerEntity, int eStatus, const char* pCvarName, const char* pCvarValue) = 0;
	virtual void OnEdictAllocated(void* edict) = 0;
	virtual void OnEdictFreed(const void* edict) = 0;
};

typedef void BaseEntity;

struct QAngle {
	QAngle(float x, float y, float z) : x(x), y(y), z(z) {}
	float x, y, z;
};

struct Vector {
	Vector(float x, float y, float z) : x(x), y(y), z(z) {}
	float x, y, z;
};

typedef void (*FnChangeCallback_t)(void* var, const char* pOldValue, float flOldValue);

template <class T, class I = int>
struct CUtlMemory {
	T* m_pMemory;
	int m_nAllocationCount;
	int m_nGrowSize;
};

template <class T, class A = CUtlMemory<T>>
struct CUtlVector {
	A m_Memory;
	int m_Size;
	T* m_pElements;

	void Append(const T& val) {
		if (this->m_Size == this->m_Memory.m_nAllocationCount) {
			int grow = this->m_Memory.m_nGrowSize;
			if (grow == 0)
				grow = 1;
			this->m_Memory.m_nAllocationCount += grow;
			this->m_Memory.m_pMemory = static_cast<T*>(realloc(this->m_Memory.m_pMemory, sizeof(T) * this->m_Memory.m_nAllocationCount));
			this->m_pElements = this->m_Memory.m_pMemory;
		}
		this->m_Memory.m_pMemory[this->m_Size] = val;
		this->m_Size++;
	}

	void Clear() {
		if (this->m_Memory.m_pMemory) {
			free(this->m_Memory.m_pMemory);
			this->m_Memory.m_pMemory = 0;
		}
		this->m_Size = 0;
		this->m_Memory.m_nAllocationCount = 0;
	}
};

#define FCVAR_DEVELOPMENTONLY (1 << 1)
#define FCVAR_HIDDEN (1 << 4)
#define FCVAR_NEVER_AS_STRING (1 << 12)
#define FCVAR_CHEAT (1 << 14)
#define FCVAR_DONTRECORD (1 << 17)

#define COMMAND_COMPLETION_MAXITEMS 64
#define COMMAND_COMPLETION_ITEM_LENGTH 64

struct ConCommandBase {
	ConCommandBase(const char* name, int flags, const char* helpstr)
		: m_pNext(nullptr)
		, m_bRegistered(false)
		, m_pszName(name)
		, m_pszHelpString(helpstr)
		, m_nFlags(flags) {
	}

	// if we actually put a virtual destructor here, EVERYTHING BREAKS
	// so put dummy methods instead
	virtual void _dtor() {};
#ifndef _WIN32
	virtual void _dtor1() {};
#endif
	virtual bool IsCommand() const { return false; };  // will get overwritten anyway lol
	// Note: vtable incomplete, but sufficient

	ConCommandBase* m_pNext;      // 4
	bool m_bRegistered;           // 8
	const char* m_pszName;        // 12
	const char* m_pszHelpString;  // 16
	int m_nFlags;                 // 20
};

struct ConVar : ConCommandBase {
	void* ConVar_VTable;                                // 24
	ConVar* m_pParent;                                  // 28
	const char* m_pszDefaultValue;                      // 32
	char* m_pszString;                                  // 36
	int m_StringLength;                                 // 40
	float m_fValue;                                     // 44
	int m_nValue;                                       // 48
	bool m_bHasMin;                                     // 52
	float m_fMinVal;                                    // 56
	bool m_bHasMax;                                     // 60
	float m_fMaxVal;                                    // 64
	CUtlVector<FnChangeCallback_t> m_fnChangeCallback;  // 68

	ConVar(const char* name, const char* value, int flags, const char* helpstr, bool hasmin, float min, bool hasmax, float max)
		: ConCommandBase(name, flags, helpstr)
		, ConVar_VTable(nullptr)
		, m_pParent(nullptr)
		, m_pszDefaultValue(value)
		, m_pszString(nullptr)
		, m_StringLength(0)
		, m_fValue(0.0f)
		, m_nValue(0)
		, m_bHasMin(hasmin)
		, m_fMinVal(min)
		, m_bHasMax(hasmax)
		, m_fMaxVal(max)
		, m_fnChangeCallback() {
	}
};

struct CCommand {
	enum {
		COMMAND_MAX_ARGC = 64,
		COMMAND_MAX_LENGTH = 512
	};
	int m_nArgc;
	int m_nArgv0Size;
	char m_pArgSBuffer[COMMAND_MAX_LENGTH];
	char m_pArgvBuffer[COMMAND_MAX_LENGTH];
	const char* m_ppArgv[COMMAND_MAX_ARGC];

	int ArgC() const {
		return this->m_nArgc;
	}
	const char* Arg(int nIndex) const {
		return this->m_ppArgv[nIndex];
	}
	const char* operator[](int nIndex) const {
		return Arg(nIndex);
	}
};

using _CommandCallback = void (*)(const CCommand& args);
using _CommandCompletionCallback = int (*)(const char* partial, char commands[COMMAND_COMPLETION_MAXITEMS][COMMAND_COMPLETION_ITEM_LENGTH]);
struct ConCommand : ConCommandBase {
	union {
		void* m_fnCommandCallbackV1;
		_CommandCallback m_fnCommandCallback;
		void* m_pCommandCallback;
	};

	union {
		_CommandCompletionCallback m_fnCompletionCallback;
		void* m_pCommandCompletionCallback;
	};

	bool m_bHasCompletionCallback : 1;
	bool m_bUsingNewCommandCallback : 1;
	bool m_bUsingCommandCallbackInterface : 1;

	ConCommand(const char* pName, _CommandCallback callback, const char* pHelpString, int flags, _CommandCompletionCallback completionFunc)
		: ConCommandBase(pName, flags, pHelpString)
		, m_fnCommandCallback(callback)
		, m_fnCompletionCallback(completionFunc)
		, m_bHasCompletionCallback(completionFunc != nullptr)
		, m_bUsingNewCommandCallback(true)
		, m_bUsingCommandCallbackInterface(false) {
	}
};
