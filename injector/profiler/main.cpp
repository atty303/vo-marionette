#include <windows.h>

#include <initguid.h>
#include "VoMarionetteProfiler.c"

// Generated from MIDL
#include "VoMarionetteProfiler_h.h"

#include "factory.h"

static const int REG_STRING_MAX = 512;
static const WCHAR HakoniwaProfilerImplProgId[] = L"VoMarionetteProfilerLib.VoMarionetteProfilerImpl";

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

STDAPI DllCanUnloadNow() {
  return Factory::DllCanUnloadNow();
}

static BOOL HelperWriteKeyEx(
  HKEY roothk,
  const WCHAR *lpSubKey,
  LPCTSTR val_name,
  DWORD dwType,
  void *lpvData,
  DWORD dwDataSize);

static BOOL HelperWriteKey(
  HKEY roothk,
  const WCHAR* subKey,
  const WCHAR* keyName,
  const WCHAR* keyValue) {
  int dataSize = (wcslen(keyValue) + 1) * sizeof(WCHAR);
  return HelperWriteKeyEx(roothk, subKey, keyName, REG_SZ, (void*)keyValue, dataSize);
}


static BOOL HelperWriteKeyEx(
  HKEY roothk,
  const WCHAR *lpSubKey,
  LPCTSTR val_name,
  DWORD dwType,
  void *lpvData,
  DWORD dwDataSize) {
  HKEY hk;
  if (ERROR_SUCCESS != RegCreateKey(roothk, lpSubKey, &hk)) return FALSE;

  if (ERROR_SUCCESS != RegSetValueEx(hk, val_name, 0, dwType, (CONST BYTE *)lpvData, dwDataSize)) return FALSE;

  if (ERROR_SUCCESS != RegCloseKey(hk))   return FALSE;
  return TRUE;
}

STDAPI DllRegisterServer() {
  LPOLESTR lpwszClsid = nullptr;
  WCHAR szBuff[REG_STRING_MAX] = { 0 };
  WCHAR szClsid[REG_STRING_MAX] = { 0 };
  WCHAR szInproc[REG_STRING_MAX] = { 0 };
  WCHAR szProgId[REG_STRING_MAX] = { 0 };
  WCHAR szDescriptionVal[REG_STRING_MAX] = { 0 };

  HRESULT hr = StringFromCLSID(CLSID_VoMarionetteProfilerImpl, &lpwszClsid);
  if (hr != S_OK) return hr;
  wsprintf(szClsid, L"%s", lpwszClsid);
  wsprintf(szInproc, L"%s\\%s\\%s", L"clsid", szClsid, L"InprocServer32");
  wsprintf(szProgId, L"%s\\%s\\%s", L"clsid", szClsid, L"ProgId");
  wsprintf(szBuff, L"%s", L"Fast Profiler");
  wsprintf(szDescriptionVal, L"%s\\%s", L"clsid", szClsid);

  HelperWriteKey(
    HKEY_CLASSES_ROOT,
    szDescriptionVal,
    nullptr,
    szBuff
  );


  GetModuleFileName(
    g_hModule,
    szBuff,
    sizeof(szBuff));
  HelperWriteKey(
    HKEY_CLASSES_ROOT,
    szInproc,
    nullptr,//write to the "default" value
    szBuff
  );

  lstrcpy(szBuff, HakoniwaProfilerImplProgId);
  HelperWriteKey(
    HKEY_CLASSES_ROOT,
    szProgId,
    nullptr,
    szBuff
  );


  wsprintf(szBuff, L"%s", L"VoMarionette Profiler");
  HelperWriteKey(
    HKEY_CLASSES_ROOT,
    HakoniwaProfilerImplProgId,
    nullptr,
    szBuff
  );


  wsprintf(szProgId, L"%s\\%s", HakoniwaProfilerImplProgId, L"CLSID");
  HelperWriteKey(
    HKEY_CLASSES_ROOT,
    szProgId,
    nullptr,
    szClsid
  );

  CoTaskMemFree(lpwszClsid);

  return S_OK;
}

STDAPI DllUnregisterServer(void) {
  WCHAR szKeyName[REG_STRING_MAX] = { 0 };
  WCHAR szClsid[REG_STRING_MAX] = { 0 };
  LPOLESTR lpwszClsid = nullptr;

  wsprintf(szKeyName, L"%s\\%s", HakoniwaProfilerImplProgId, L"CLSID");
  RegDeleteKey(HKEY_CLASSES_ROOT, szKeyName);
  RegDeleteKey(HKEY_CLASSES_ROOT, HakoniwaProfilerImplProgId);

  StringFromCLSID(CLSID_VoMarionetteProfilerImpl, &lpwszClsid);
  wsprintf(szClsid, L"%s", lpwszClsid);
  wsprintf(szKeyName, L"%s\\%s\\%s", L"CLSID", szClsid, L"InprocServer32");
  RegDeleteKey(HKEY_CLASSES_ROOT, szKeyName);

  wsprintf(szKeyName, L"%s\\%s\\%s", L"CLSID", szClsid, L"ProgId");
  RegDeleteKey(HKEY_CLASSES_ROOT, szKeyName);

  wsprintf(szKeyName, L"%s\\%s", L"CLSID", szClsid);
  RegDeleteKey(HKEY_CLASSES_ROOT, szKeyName);

  CoTaskMemFree(lpwszClsid);

  return S_OK;
}
