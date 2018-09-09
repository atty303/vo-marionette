#pragma once

#include <Unknwn.h>

class Factory : public IClassFactory
{
public:
  STDMETHODIMP QueryInterface(REFIID riid, void** ppvObject) override;
  STDMETHODIMP_(ULONG) AddRef() override;
  STDMETHODIMP_(ULONG) Release() override;
  
  STDMETHODIMP CreateInstance(IUnknown* pUnkOuter, REFIID riid, void** ppvObject) override;
  STDMETHODIMP LockServer(BOOL fLock) override;

  static bool DllCanUnloadNow();

private:
  void LockServerCount(BOOL bLock);

};
