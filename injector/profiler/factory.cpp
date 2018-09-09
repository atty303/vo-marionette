#include "factory.h"
#include "VoMarionetteProfilerImpl.h"

STDMETHODIMP Factory::QueryInterface(REFIID riid, void **ppvObject) {
  *ppvObject = nullptr;

  if (IsEqualIID(riid, IID_IUnknown) || IsEqualIID(riid, IID_IClassFactory))
    *ppvObject = static_cast<IClassFactory *>(this);
  else
    return E_NOINTERFACE;

  AddRef();
  return S_OK;
}

STDMETHODIMP_(ULONG) Factory::AddRef() {
  LockServerCount(TRUE);
  return 2;
}

STDMETHODIMP_(ULONG) Factory::Release() {
  LockServerCount(FALSE);
  return 1;
}

STDMETHODIMP Factory::CreateInstance(IUnknown *pUnkOuter, REFIID riid, void **ppvObject) {
  *ppvObject = nullptr;
  if (pUnkOuter != nullptr)
    return CLASS_E_NOAGGREGATION;

  VoMarionetteProfiler* p = new VoMarionetteProfilerImpl();
  if (p == nullptr) return E_OUTOFMEMORY;

  auto hr = p->QueryInterface(riid, ppvObject);
  p->Release();

  return hr;
}

STDMETHODIMP Factory::LockServer(BOOL fLock) {
  LockServerCount(fLock);
  return S_OK;
}

static long g_lockServerCount = 0;

void Factory::LockServerCount(BOOL bLock) {
  if (bLock) {
    InterlockedIncrement(&g_lockServerCount);
  }
  else {
    InterlockedDecrement(&g_lockServerCount);
  }
}

bool Factory::DllCanUnloadNow() {
  return g_lockServerCount == 0 ? S_OK : S_FALSE;
}
