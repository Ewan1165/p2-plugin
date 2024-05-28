#include "Utils/Mem.hpp"
#include "Modules/tier0.hpp"

namespace vgui {
	enum PaintMode_t {
		PAINT_UIPANELS = (1 << 0),
		PAINT_INGAMEPANELS = (1 << 1),
	};

	Module* engine;
	Module* vguimatsurface;
	Module* vguischeme;

	Interface* engineVgui;
	Interface* vguiSurface;
	Interface* vguiScheme;

	using Paint_ = int(thiscall*)(void*, PaintMode_t);
	Paint_ Paint;
	int __fastcall Paint_Hook(void* thisptr, PaintMode_t mode) {
		log("ass");
		return Paint(thisptr, mode);
	}

	bool Init() {
		engine = new Module("engine.dll");
		vguimatsurface = new Module("vguimatsurface.dll");
		vguischeme = new Module("vgui2.dll");

		engineVgui = engine->GetInterface("VEngineVGui001", true);
		vguiSurface = vguimatsurface->GetInterface("VGUI_Surface031");
		vguiScheme = vguischeme->GetInterface("VGUI_Scheme010");

		//engineVgui->Hook<int(__fastcall*)(void*, PaintMode_t), Paint_>(Paint_Hook, Paint, 14);

		return true;
	}
}