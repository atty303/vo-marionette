#include <windows.h>
#include "factory.h"

// Generated from MIDL
#include "VoMarionetteProfiler_h.h"

HMODULE g_hModule = nullptr;

BOOL APIENTRY DllMain(HANDLE hModule, DWORD dwReason, void* lpReserved) {
	if (dwReason == DLL_PROCESS_ATTACH) {
		g_hModule = static_cast<HMODULE>(hModule);
	}
	return TRUE;
}

STDAPI DllGetClassObject(const CLSID& clsid, const IID& iid, void** ppv) {
  static Factory factory;

  *ppv = nullptr;
  if (IsEqualCLSID(clsid, CLSID_VoMarionetteProfilerImpl)) {
    return factory.QueryInterface(iid, ppv);
  }

  return CLASS_E_CLASSNOTAVAILABLE;
}
