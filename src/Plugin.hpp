#pragma once

#include "sdk.hpp"
#include "Utils/Mem.hpp"
#include "Modules/tier0.hpp"
#include "Modules/engine.hpp"
#include "Modules/server.hpp"
#include "Modules/tier1.hpp"
#include "Entity.hpp"
#include "CVar.hpp"

#include <string.h>
#include <Windows.h>

class Plugin : public IServerPluginCallbacks {
public:
	virtual bool Load(CreateInterfaceFn interfaceFactory, CreateInterfaceFn gameServerFactory);
	virtual void Unload();
	virtual void Pause();
	virtual void UnPause();
	virtual const char* GetPluginDescription();
	virtual void LevelInit(char const* pMapName);
	virtual void ServerActivate(void* pEdictList, int edictCount, int clientMax);
	virtual void GameFrame(bool simulating);
	virtual void LevelShutdown();
	virtual void ClientFullyConnect(void* pEdict);
	virtual void ClientActive(void* pEntity);
	virtual void ClientDisconnect(void* pEntity);
	virtual void ClientPutInServer(void* pEntity, char const* playername);
	virtual void SetCommandClient(int index);
	virtual void ClientSettingsChanged(void* pEdict);
	virtual int ClientConnect(bool* bAllowConnect, void* pEntity, const char* pszName, const char* pszAddress, char* reject, int maxrejectlen);
	virtual int ClientCommand(void* pEntity, const void*& args);
	virtual int NetworkIDValidated(const char* pszUserName, const char* pszNetworkID);
	virtual void OnQueryCvarValueFinished(int iCookie, void* pPlayerEntity, int eStatus, const char* pCvarName, const char* pCvarValue);
	virtual void OnEdictAllocated(void* edict);
	virtual void OnEdictFreed(const void* edict);
};