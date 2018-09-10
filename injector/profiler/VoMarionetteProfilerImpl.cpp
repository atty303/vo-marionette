#include "VoMarionetteProfilerImpl.h"
#include "Util.h"
#include "FunctionInfo.h"
#include "Trampoline.h"

#include <string>
#include <wchar.h>
#include <corhlpr.h>
#include <memory>
#include <iostream>
#include <fstream>
#include "VoMarionetteProfilerImpl.h"

#pragma comment (lib, "corguids.lib")



VoMarionetteProfilerImpl::VoMarionetteProfilerImpl() {
	mRefCount = 1;
}

VoMarionetteProfilerImpl::~VoMarionetteProfilerImpl() {
}

HRESULT VoMarionetteProfilerImpl::SetProfilerEventMask() {
	DWORD eventMask = 0;
	eventMask |= COR_PRF_MONITOR_JIT_COMPILATION;
	eventMask |= COR_PRF_DISABLE_OPTIMIZATIONS;
	eventMask |= COR_PRF_USE_PROFILE_IMAGES;

	return mCorProfilerInfo2->SetEventMask(eventMask);
}

STDMETHODIMP VoMarionetteProfilerImpl::QueryInterface(REFIID riid, void **ppObj) {
	*ppObj = nullptr;

	if (riid == IID_IUnknown) {
		*ppObj = static_cast<IUnknown *>(static_cast<VoMarionetteProfiler *>(this));
		AddRef();
		return S_OK;
	}

	if (riid == IID_VoMarionetteProfiler) {
		*ppObj = static_cast<VoMarionetteProfiler *>(this);
		AddRef();
		return S_OK;
	}

	if (riid == IID_ICorProfilerCallback) {
		*ppObj = static_cast<ICorProfilerCallback*>(this);
		AddRef();
		return S_OK;
	}

	if (riid == IID_ICorProfilerCallback2) {
		*ppObj = static_cast<ICorProfilerCallback2*>(this);
		AddRef();
		return S_OK;
	}

	if (riid == IID_ICorProfilerCallback3) {
		*ppObj = dynamic_cast<ICorProfilerCallback3*>(this);
		AddRef();
		return S_OK;
	}

	if (riid == IID_ICorProfilerInfo || riid == IID_ICorProfilerInfo2) {
		mCorProfilerInfo2.CopyTo(riid, ppObj);
		return S_OK;
	}

	return E_NOINTERFACE;
}

ULONG STDMETHODCALLTYPE VoMarionetteProfilerImpl::AddRef() {
	return InterlockedIncrement(&mRefCount);
}

ULONG STDMETHODCALLTYPE VoMarionetteProfilerImpl::Release() {
	long nRefCount = InterlockedDecrement(&mRefCount);
	if (nRefCount == 0) {
		delete this;
	}
	return nRefCount;
}

STDMETHODIMP VoMarionetteProfilerImpl::Initialize(IUnknown *pICorProfilerInfoUnk) {

	ICorProfilerInfo2* iInfo2;
	HRESULT hr = pICorProfilerInfoUnk->QueryInterface(__uuidof(iInfo2), (LPVOID*)&iInfo2);
	if (FAILED(hr)) {
		DebugPrintf(L"Error: Failed to get ICorProfiler2\n");
		exit(-1);
	}
	mCorProfilerInfo2.Attach(iInfo2);

	hr = SetProfilerEventMask();
	if (FAILED(hr)) {
		DebugPrintf(L"Error: Failed in SetProfilerEventMask\n");
		exit(-1);
	}
	DebugPrintf(L"Successfully initialized profiling\n");

	return S_OK;
}

#include <memory>

#include <stdio.h>
#include <string>

STDMETHODIMP VoMarionetteProfilerImpl::JITCompilationStarted(FunctionID functionID, BOOL fIsSafeToBlock) {
	std::shared_ptr<FunctionInfo> fi(FunctionInfo::CreateFunctionInfo(mCorProfilerInfo2.Get(), functionID));

	if (fi->get_ClassName() == L"System.Windows.Forms.CommonDialog" && fi->get_FunctionName() == L"ShowDialog") {
		std::wofstream os;
		os.open("d:\\hook2.txt", std::ios_base::out | std::ios_base::app);
		os.imbue(std::locale("japanese"));
		os << std::wstring(fi->get_ClassName()) << L" :: " << fi->get_FunctionName() << L"(" << fi->get_ArgumentCount() << std::endl;
		os.close();
		//FILE *fp;// = fopen("d:\\hook3.txt", "w+");
		//fopen_s(&fp, "d:\\hook2", "w+");
		//fprintf(fp, "%s\n", fi->get_ClassName().c_str());
		//fclose(fp);
	}
	if (fi->get_ClassName() == L"System.Windows.Forms.CommonDialog" && fi->get_FunctionName() == L"ShowDialog") {
		if (fi->get_ArgumentCount() == 1) {
			DebugPrintf(L"%s", fi->get_SignatureText().c_str());
			Tranpoline tranpoline(mCorProfilerInfo2, fi);
			tranpoline.Update(L"HakoniwaProfiler.MethodHook.MethodHook", L"ShowDialog");
		}
	}

	/*

	//クラス名，メソッド名が一致する物を置き換える
	if (fi->get_ClassName() == L"System.DateTime" && fi->get_FunctionName() == L"get_Now") {
		DebugPrintf(L"%s", fi->get_SignatureText().c_str());
		Tranpoline tranpoline(mCorProfilerInfo2, fi);
		tranpoline.Update(L"HakoniwaProfiler.MethodHook.MethodHook", L"get_Now");
	}

	if (fi->get_ClassName() == L"System.Text.RegularExpressions.Regex" && fi->get_FunctionName() == L"Replace") {
		//overloadされているメソッドは，引数を確認して置き換え対象か判断する
		if (fi->get_ArgumentCount() == 3 && fi->get_Arguments()[0] == L"string" && fi->get_Arguments()[1] == L"string" && fi->get_Arguments()[2] == L"string") {
			// staticなメソッドの置き換え
			DebugPrintf(L"%s", fi->get_SignatureText().c_str());
	pp		Tranpoline tranpoline(mCorProfilerInfo2, fi);
	p		tranpoline.Update(L"HakoniwaProfiler.MethodHook.MethodHook", L"Replace");
		} else if (fi->get_ArgumentCount() == 2 && fi->get_Arguments()[0] == L"string" && fi->get_Arguments()[1] == L"string") {
	p		// 非staticなメソッドの置き換え
	p		DebugPrintf(L"%s", fi->get_SignatureText().c_str());
			Tranpoline tranpoline(mCorProfilerInfo2, fi);
			tranpoline.Update(L"HakoniwaProfiler.MethodHook.MethodHook", L"Replace");
		}
	}

	if (fi->get_ClassName() == L"ConsoleAppTest.Program" && fi->get_FunctionName() == L"getStr1") {
		//テストプログラムのメソッドを置き換える
		DebugPrintf(L"%s", fi->get_SignatureText().c_str());
		Tranpoline tranpoline(mCorProfilerInfo2, fi);
		tranpoline.Update(L"HakoniwaProfiler.MethodHook.MethodHook", L"getStr1");
	}

	if (fi->get_ClassName() == L"ConsoleAppTest.Program" && fi->get_FunctionName() == L"haveArguments") {
		DebugPrintf(L"%s", fi->get_SignatureText().c_str());
		Tranpoline tranpoline(mCorProfilerInfo2, fi);
		tranpoline.Update(L"HakoniwaProfiler.MethodHook.MethodHook", L"haveArguments");
	}

	if(fi->get_ClassName() == L"ConsoleAppTest.Program" && fi->get_FunctionName() == L"haveManyArguments") {
		DebugPrintf(L"%s", fi->get_SignatureText().c_str());
		Tranpoline tranpoline(mCorProfilerInfo2, fi);
		tranpoline.Update(L"HakoniwaProfiler.MethodHook.MethodHook", L"haveManyArguments");
	}

	if (fi->get_ClassName() == L"ConsoleAppTest.TestClass" && fi->get_FunctionName() == L"test1") {
		// staticなメソッドの置き換え
		DebugPrintf(L"%s", fi->get_SignatureText().c_str());
		Tranpoline tranpoline(mCorProfilerInfo2, fi);
		tranpoline.Update(L"HakoniwaProfiler.MethodHook.MethodHook", L"test1");
	}

	if (fi->get_ClassName() == L"ConsoleAppTest.TestClass" && fi->get_FunctionName() == L"test2") {
		// 非staticなメソッドの置き換え
		DebugPrintf(L"%s", fi->get_SignatureText().c_str());
		Tranpoline tranpoline(mCorProfilerInfo2, fi);
		tranpoline.Update(L"HakoniwaProfiler.MethodHook.MethodHook", L"test2");
	}

	if (fi->get_ClassName() == L"ConsoleAppTest.ConcreteClass" && fi->get_FunctionName() == L"Show") {
		// 非staticなメソッドの置き換え
		DebugPrintf(L"%s", fi->get_SignatureText().c_str());
		Tranpoline tranpoline(mCorProfilerInfo2, fi);
		tranpoline.Update(L"HakoniwaProfiler.MethodHook.MethodHook", L"Show");
	}
	*/

	return S_OK;
}

