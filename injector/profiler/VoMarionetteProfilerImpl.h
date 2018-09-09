#pragma once

#include <wrl/client.h>
#include <cstdio>

#include "VoMarionetteProfilerBase.h"

class VoMarionetteProfilerImpl :
	public HakoniwaProfilerBase
{
public:
	VoMarionetteProfilerImpl();
	~VoMarionetteProfilerImpl();

	STDMETHOD(QueryInterface)(REFIID riid, void **ppObj);
	ULONG STDMETHODCALLTYPE AddRef();
	ULONG STDMETHODCALLTYPE Release();
	STDMETHOD(Initialize)(IUnknown *pICorProfilerInfoUnk);


	STDMETHOD(JITCompilationStarted)(FunctionID functionID, BOOL fIsSafeToBlock);

private:
	HRESULT SetProfilerEventMask();

private:
	Microsoft::WRL::ComPtr<ICorProfilerInfo2> mCorProfilerInfo2;

	long mRefCount;
};
