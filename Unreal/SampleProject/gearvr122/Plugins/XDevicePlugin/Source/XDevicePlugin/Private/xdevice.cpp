#include "XDevicePluginPrivatePCH.h"

#include "IXDevicePlugin.h"
#include "xdevice.h"

#if !XDEVICE_SUPPORTED_ANDROID_PLATFORMS


void* s_XDeviceLib=NULL;
int (*XDeviceInit_)(void)=NULL;
int (*XDeviceExit_)(void)=NULL;

#define DLL_DEF(FuncName) delegate_##FuncName FuncName=NULL

	DLL_DEF(XDeviceGetInputDeviceHandle);
	DLL_DEF(XDeviceGetInputDeviceName);
	DLL_DEF(XDeviceGetInputDeviceCount);
	DLL_DEF(XDeviceGetInputDevices);
	DLL_DEF(XDeviceGetInputState);
	DLL_DEF(XDeviceSendMessage);
	DLL_DEF(XDeviceResetInputState);
	DLL_DEF(XDeviceUpdateInputState);
	DLL_DEF(XDeviceUpdateInputBuffer);
	DLL_DEF(XDeviceSerializeMessage);
	DLL_DEF(XDeviceSendBuffer);
	DLL_DEF(XDeviceSetEventListener);
	DLL_DEF(XDeviceDispatchEventListener);
	DLL_DEF(XDeviceGetBool);
	DLL_DEF(XDeviceSetBool);
	DLL_DEF(XDeviceGetInt);
	DLL_DEF(XDeviceSetInt);
	DLL_DEF(XDeviceGetFloat);
	DLL_DEF(XDeviceSetFloat);
	DLL_DEF(XDeviceGetObject);
	DLL_DEF(XDeviceSetObject);
	DLL_DEF(XDeviceGetNodePosition);
	DLL_DEF(XDeviceUpdateNodePose);
	DLL_DEF(XDeviceGetTrackerPose);
	DLL_DEF(XDeviceSetTrackerPose);
	DLL_DEF(XDeviceTestNode);
	DLL_DEF(XDeviceGetTickCount);

#undef DLL_DEF
	
	int XDeviceInit(void){
		if(s_XDeviceLib!=NULL){
			if(XDeviceInit_==NULL){
				XDeviceInit_=(int(*)())FPlatformProcess::GetDllExport(s_XDeviceLib,TEXT("XDeviceInit"));
				XDeviceExit_=(int(*)())FPlatformProcess::GetDllExport(s_XDeviceLib,TEXT("XDeviceExit"));
#define LoadProcAddress(FunName) FunName=(delegate_##FunName)FPlatformProcess::GetDllExport(s_XDeviceLib,TEXT(#FunName));
				LoadProcAddress(XDeviceGetInputDeviceHandle);
				LoadProcAddress(XDeviceGetInputDeviceName);
				LoadProcAddress(XDeviceGetInputDeviceCount);
				LoadProcAddress(XDeviceGetInputDevices);
				LoadProcAddress(XDeviceGetInputState);
				LoadProcAddress(XDeviceSendMessage);
				LoadProcAddress(XDeviceResetInputState);
				LoadProcAddress(XDeviceUpdateInputState);
				LoadProcAddress(XDeviceUpdateInputBuffer);
				LoadProcAddress(XDeviceSerializeMessage);
				LoadProcAddress(XDeviceSendBuffer);
				LoadProcAddress(XDeviceSetEventListener);
				LoadProcAddress(XDeviceDispatchEventListener);
				LoadProcAddress(XDeviceGetBool);
				LoadProcAddress(XDeviceSetBool);
				LoadProcAddress(XDeviceGetInt);
				LoadProcAddress(XDeviceSetInt);
				LoadProcAddress(XDeviceGetFloat);
				LoadProcAddress(XDeviceSetFloat);
				LoadProcAddress(XDeviceGetObject);
				LoadProcAddress(XDeviceSetObject);
				LoadProcAddress(XDeviceGetNodePosition);
				LoadProcAddress(XDeviceUpdateNodePose);
				LoadProcAddress(XDeviceGetTrackerPose);
				LoadProcAddress(XDeviceSetTrackerPose);
				LoadProcAddress(XDeviceTestNode);
				LoadProcAddress(XDeviceGetTickCount);

#undef LoadProcAddress
			}
			return XDeviceInit_();
		}
		return -1;
	}

	int XDeviceExit(void){
		if(s_XDeviceLib!=NULL){
			if(XDeviceInit_==NULL){
				int ret=XDeviceExit_();
				XDeviceInit_=NULL;
				XDeviceExit_=NULL;
				//
				FPlatformProcess::FreeDllHandle(s_XDeviceLib);
				s_XDeviceLib=NULL;
				//
				return ret;
			}
		}
		return -1;
	}

#endif