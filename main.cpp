#include <windows.h>
#include <d3d9.h>
#include <mutex>
#include <toml++/toml.hpp>

#include "nya_dx9_hookbase.h"
#include "nya_commonhooklib.h"
#include "nfsmw.h"

#include "include/chloemenulib.h"

void DisableKeyboardInput(bool disable) {
	NyaHooks::bInputsBlocked = disable;
}

void UpdateD3DProperties() {
	g_pd3dDevice = *(IDirect3DDevice9**)0x982BDC;
	ghWnd = *(HWND*)0x982BF4;
	GetRacingResolution(&nResX, &nResY);
}

const char* versionString = "NFS Most Wanted Menu Lib 1.00";

#include "menulib.h"

BOOL WINAPI DllMain(HINSTANCE, DWORD fdwReason, LPVOID) {
	switch( fdwReason ) {
		case DLL_PROCESS_ATTACH: {
			if (NyaHookLib::GetEntryPoint() != 0x3C4040) {
				MessageBoxA(nullptr, "Unsupported game version! Make sure you're using v1.3 (.exe size of 6029312 bytes)", "nya?!~", MB_ICONERROR);
				return TRUE;
			}

			if (!InitAndLoadConfig("NFSMWMenuLib_gcp.toml")) return TRUE;
			
			NyaHooks::PlaceD3DHooks();
			NyaHooks::aEndSceneFuncs.push_back(D3DHookMain);
			NyaHooks::aD3DResetFuncs.push_back(OnD3DReset);
			NyaHooks::PlaceWndProcHook();
			NyaHooks::aWndProcFuncs.push_back(WndProcHook);
			NyaHooks::PlaceInputBlockerHook();
		} break;
		default:
			break;
	}
	return TRUE;
}