#pragma once
#include <stdio.h>
#include <Windows.h>
#include <string>
#include <sapi.h>


// FullScreen games make reading debug text and program output in gerneral difficult
// We utilize Microsofts Text To Speech interface to keep us informed of our program state
void speak(std::string&& speak) {
	//	This is all pretty much boilerplate code for Mircosofts TTS
	std::wstring stemp = std::wstring(speak.begin(), speak.end());
	LPCWSTR sw = stemp.c_str();
	ISpVoice* pVoice = NULL;
	if (FAILED(::CoInitialize(NULL))) { // Oh something should be here eventually
	}
	HRESULT hr = CoCreateInstance(CLSID_SpVoice, NULL, CLSCTX_ALL, IID_ISpVoice, (void **)&pVoice);
	if (SUCCEEDED(hr)) {
		hr = pVoice->Speak(sw, 0, NULL);
		pVoice->Release();
		pVoice = NULL;
	}
	::CoUninitialize();
}
