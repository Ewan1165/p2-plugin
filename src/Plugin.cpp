#include "Plugin.hpp"

Plugin plugin;

CVar testvar("testvar", "", "test var");

bool Plugin::Load(CreateInterfaceFn interfaceFactory, CreateInterfaceFn gameServerFactory) {
	GetModules();
	if (!tier0::Init()) return false;
	if (!tier1::Init()) {
		log("vstdlib.dll Couldnt Load");
		return false;
	}
	if (!engine::Init()) {
		log("Engine.dll Couldnt Load");
		return false;
	}
	if (!server::Init()) {
		log("Server.dll couldnt load");
		return false;
	}
	if (!vgui::Init()) {
		log("vgui couldnt load");
		return false;
	}
	RegisterAllCVars();

	log("Plugin Loaded");

	return true;
}

void Plugin::ClientPutInServer(void* pEntity, char const* playername) {
	engine::ExecuteCmd("sv_cheats 1");

	Entity first = Entity::First();
	first.SetKeyval("targetname", "testingvro");
}

void Plugin::Unload() { }


const char* Plugin::GetPluginDescription() { return "Plugin"; }

void Plugin::LevelInit(char const* pMapName) { }
void Plugin::GameFrame(bool simulating) { }
void Plugin::Pause() { }
void Plugin::ServerActivate(void* pEdictList, int edictCount, int clientMax) { }
void Plugin::UnPause() { }
void Plugin::LevelShutdown() { }
void Plugin::ClientActive(void* pEntity) { }
void Plugin::ClientDisconnect(void* pEntity) { }
void Plugin::ClientFullyConnect(void* pEdict) { }
void Plugin::SetCommandClient(int index) { }
void Plugin::ClientSettingsChanged(void* pEdict) { }
int Plugin::ClientConnect(bool* bAllowConnect, void* pEntity, const char* pszName, const char* pszAddress, char* reject, int maxrejectlen) { return 0; }
int Plugin::ClientCommand(void* pEntity, const void*& args) { return 0; }
int Plugin::NetworkIDValidated(const char* pszUserName, const char* pszNetworkID) { return 0; }
void Plugin::OnQueryCvarValueFinished(int iCookie, void* pPlayerEntity, int eStatus, const char* pCvarName, const char* pCvarValue) { }
void Plugin::OnEdictAllocated(void* edict) { }
void Plugin::OnEdictFreed(const void* edict) { }





extern "C" __declspec(dllexport) void* CreateInterface(const char* name, int* ret) {
	if (!strcmp(INTERFACEVERSION_ISERVERPLUGINCALLBACKS, name)) {
		if (ret) *ret = 0;
		return &plugin;
	}
	if (ret) *ret = 1;
	return nullptr;
}