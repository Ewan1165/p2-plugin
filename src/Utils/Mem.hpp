#pragma once

#include <windows.h>
#include <psapi.h>
#include <filesystem>
#include <vector>
#include <map>
#include "sdk.hpp"
#include "Platform.hpp"

#define INRANGE(x, a, b) ((x) >= (a) && (x) <= (b))
#define getBits(x) (INRANGE(((x) & (~0x20)), 'A', 'F') ? (((x) & (~0x20)) - 'A' + 0xA) : (INRANGE(x, '0', '9') ? x - '0' : 0))
#define getByte(x) (getBits(x[0]) << 4 | getBits(x[1]))

class Interface {
public:
	uintptr_t** base;
	uintptr_t* ptr;
	std::unique_ptr<uintptr_t[]> copy;

	int size = 0;

	bool hook;

	Interface(void* base, bool hook = false) : hook(hook) {
		this->base = reinterpret_cast<uintptr_t**>(base);
		this->ptr = *this->base;

		while (this->ptr[this->size]) {
			++this->size;
		}

		if (hook) {
			copy = std::make_unique<uintptr_t[]>(this->size + 1);
			std::memcpy(this->copy.get(), this->ptr - 1, sizeof(uintptr_t) + this->size * sizeof(uintptr_t));

			*this->base = this->copy.get() + 1;
		}
	}

	template <typename Ret, typename... Args>
	Ret CallFunc(int index, bool usecdecl = false, Args... args) {
		using cDecl = Ret(cdecl*)(void*, Args...);
		using thisCall = Ret(thiscall*)(void*, Args...);

		if (usecdecl) {
			return reinterpret_cast<cDecl>(ptr[index])(reinterpret_cast<void*>(base), args...);
		}
		return reinterpret_cast<thisCall>(ptr[index])(reinterpret_cast<void*>(base), args...);
	}

	template <typename T = uintptr_t, typename U = void*>
		bool Hook(T detour, U &original, int index) {
		if (index >= 0 && index < this->size) {
			this->copy[index] = reinterpret_cast<uintptr_t>(detour);
			original = (U)this->ptr[index];
			return true;
		}
		return false;
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

	Interface* GetInterface(const char* name, bool hook = false) {
		void* CreateInterfacePtr = this->GetSymbol("CreateInterface");

		auto CreateInterface = reinterpret_cast<CreateInterfaceFn>(CreateInterfacePtr);

		int ret;
		void* InterfacePtr = CreateInterface(name, &ret);

		return new Interface(InterfacePtr, hook);
	}
};