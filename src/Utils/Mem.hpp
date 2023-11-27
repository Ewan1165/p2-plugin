#pragma once

#include <windows.h>
#include <psapi.h>
#include <filesystem>
#include <vector>
#include <map>
#include "sdk.hpp"

#define INRANGE(x, a, b) ((x) >= (a) && (x) <= (b))
#define getBits(x) (INRANGE(((x) & (~0x20)), 'A', 'F') ? (((x) & (~0x20)) - 'A' + 0xA) : (INRANGE(x, '0', '9') ? x - '0' : 0))
#define getByte(x) (getBits(x[0]) << 4 | getBits(x[1]))

class Interface {
public:
	uintptr_t** base;
	uintptr_t* ptr;

	Interface(void* base) {
		this->base = reinterpret_cast<uintptr_t**>(base);
		this->ptr = *this->base;
	}

	template <typename Ret, typename... Args>
	Ret CallFunc(int index, bool usecdecl = false, Args... args) {
		using cDecl = Ret(__cdecl*)(void*, Args...);
		using thisCall = Ret(__thiscall*)(void*, Args...);

		if (usecdecl) {
			return reinterpret_cast<cDecl>(ptr[index])(reinterpret_cast<void*>(base), args...);
		}
		return reinterpret_cast<thisCall>(ptr[index])(reinterpret_cast<void*>(base), args...);
	}
};

struct ModuleA {
	std::string name;
	std::string path;
	uintptr_t start;
	uintptr_t size;
};

std::vector<ModuleA> moduleList;

static void GetModules() {
	HMODULE hMods[1024];
	HANDLE pHandle = GetCurrentProcess();
	DWORD cbNeeded;
	if (EnumProcessModules(pHandle, hMods, sizeof(hMods), &cbNeeded)) {
		for (DWORD i = 0; i < (cbNeeded / sizeof(HMODULE)); i++) {
			TCHAR szModPath[MAX_PATH];
			TCHAR szModName[MAX_PATH];
			if (GetModuleBaseName(pHandle, hMods[i], szModName, sizeof(szModName) / sizeof(TCHAR)) && GetModuleFileNameEx(pHandle, hMods[i], szModPath, sizeof(szModPath) / sizeof(TCHAR))) {
				ModuleA curr;

				auto modInfo = MODULEINFO();
				curr.start = (uintptr_t)modInfo.lpBaseOfDll;
				curr.size = (uintptr_t)modInfo.SizeOfImage;

				std::wstring wName(szModName);
				curr.name = std::string(wName.begin(), wName.end());
				std::wstring wPath(szModPath);
				curr.path = std::string(wPath.begin(), wPath.end());

				moduleList.push_back(curr);
			}
		}
	}
}

class Module {
public:
	HMODULE ptr;
	ModuleA info;

	Module(const char* name) {
		for (ModuleA i : moduleList) {
			if (i.name == name) {
				info = i;
				ptr = GetModuleHandleA(i.path.c_str());
				return;
			}
		}
	}

	template <typename T = void*>
	T GetSymbol(const char* name) {
		return reinterpret_cast<T>(GetProcAddress(ptr, name));
	}

	template <typename T = uintptr_t>
	T Scan(const char* target, int offset = 0) {
		uintptr_t res;

		const char* pattern = target;
		uintptr_t result = 0;

		for (auto position = info.start; position < info.start+info.size; ++position) {
			if (!*pattern)
				res = result;

			if (*pattern == '?' || *(uint8_t*)position == getByte(pattern)) {
				if (!result)
					result = position;

				if (!pattern[1] || !pattern[2])
					res = result;

				pattern += (*pattern == '\?') ? 2 : 3;
			}
			else {
				pattern = target;
				result = 0;
			}
		}

		if (result) {
			result += offset;
		}

		return result;
	}

	Interface* GetInterface(const char* name) {
		void* CreateInterfacePtr = this->GetSymbol("CreateInterface");

		auto CreateInterface = reinterpret_cast<CreateInterfaceFn>(CreateInterfacePtr);

		int ret;
		void* InterfacePtr = CreateInterface(name, &ret);

		return new Interface(InterfacePtr);
	}
};