#ifndef __XIM_XDEVICE_H__
#define __XIM_XDEVICE_H__


#include "FieldIDs.h"
#include "MessageIDs.h"

#ifndef XIM_API
#if defined(_WIN32)||defined(_WIN64)
	#define XIM_API __declspec(dllimport)
#else
	#define XIM_API
#endif
#endif

// Device ID

#define ID_CONTEXT -1

#define XIM_DK3 0x3000
#define XIM_DK4 0x4000

//class DeviceContext;
//class InputDevice;

#if XDEVICE_SUPPORTED_ANDROID_PLATFORMS

#if defined(__LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
extern "C" {
#endif
	// Common
	/** Initialize the X-Device SDK library.*/
	XIM_API int XDeviceInit(void);
	/** Finalize the X-Device SDK library.*/
	XIM_API int XDeviceExit(void);

	// Device Management
	
	XIM_API int XDeviceGetInputDeviceHandle(char* name);
	/** Get the name of input device identified by handle.*/
	XIM_API char* XDeviceGetInputDeviceName(int which);

	/** Get count of input devices in X-Device SDK.*/
	XIM_API int XDeviceGetInputDeviceCount();
	/** Get handles of all input devices identified by type.*/
	XIM_API int XDeviceGetInputDevices(int type,int* whichBuffer,int whichBufferSize);

	// I/O
	
	/** Get the input state of input device identified by handle.*/
	XIM_API int XDeviceGetInputState(int which, void* state);
	/** Send a message to the input device identified by handle.*/
	XIM_API int XDeviceSendMessage(int which, int Msg, int wParam, int lParam);
	
	/** Reset the input state of input device identified by handle.*/
	XIM_API int XDeviceResetInputState(int which);
	/** Update the input device identified by handle.*/
	XIM_API int XDeviceUpdateInputState(int which);
	/** Update the input buffer of input device identified by handle.*/
	XIM_API int XDeviceUpdateInputBuffer(int which, unsigned char* buffer, int offset, int count);
	
	/** Serialize a message from the input device identified by handle.*/
	XIM_API int XDeviceSerializeMessage(unsigned char* buffer, int offset, int count,
		                                int which, int Msg, int wParam, int lParam);
	/** Send a buffer to the input device identified by handle.*/
	XIM_API int XDeviceSendBuffer(int which, unsigned char* buffer, int offset, int count);

	// Event

	/** Set an event listener on the event identified by eventID of the input device identified by handle.*/
	XIM_API int XDeviceSetEventListener(int which, int eventID, void* listener);
	/** Dispatch the event identified by eventID of the input device identified by handle.*/
	XIM_API int XDeviceDispatchEventListener(int which, int eventID);

	// Get/Set

	/** Get a bool value of input device identified by handle.*/
	XIM_API bool XDeviceGetBool(int which, int fieldID, bool defaultValue);
	/** Set a bool value of input device identified by handle.*/
	XIM_API void XDeviceSetBool(int which, int fieldID, bool value);
	/** Get an int value of input device identified by handle.*/
	XIM_API int XDeviceGetInt(int which, int fieldID, int defaultValue);
	/** Set an int value of input device identified by handle.*/
	XIM_API void XDeviceSetInt(int which, int fieldID, int value);
	/** Get a float value of input device identified by handle.*/
	XIM_API float XDeviceGetFloat(int which, int fieldID, float defaultValue);
	/** Set a float value of input device identified by handle.*/
	XIM_API void XDeviceSetFloat(int which, int fieldID, float value);
	/** Get a object value of input device identified by handle.*/
	XIM_API void XDeviceGetObject(int which, int fieldID, unsigned char* buffer, int offset, int count);
	/** Set a object value of input device identified by handle.*/
	XIM_API void XDeviceSetObject(int which, int fieldID, unsigned char* buffer, int offset, int count);

	  //Ext

	/** Get the node position of input device identified by handle.*/
	XIM_API int XDeviceGetNodePosition(int which, int history, int node, float* position);

	/** Update the node pose(position and rotation) of input device identified by handle.*/
	XIM_API int XDeviceUpdateNodePose(int which, int node, float* position, float* rotation);

	/** */
	XIM_API int XDeviceGetTrackerPose(int which,float* height,float* depth,float* pitch);

	/** */
	XIM_API int XDeviceSetTrackerPose(int which,float height,float depth,float pitch);

	/** */
	XIM_API int XDeviceTestNode(int which,int node,void* result);

	/** */
	XIM_API int XDeviceGetTickCount();

#if defined(__LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
}
#endif

#else

#define DLL_DEF(RetType,FuncName,...) \
typedef RetType (*delegate_##FuncName)(__VA_ARGS__); \
extern delegate_##FuncName FuncName;

	// Common
	/** Initialize the X-Device SDK library.*/
	int XDeviceInit(void);
	/** Finalize the X-Device SDK library.*/
	int XDeviceExit(void);

	// Device Management
	
	DLL_DEF(int,XDeviceGetInputDeviceHandle,char* name);
	/** Get the name of input device identified by handle.*/
	DLL_DEF(char*,XDeviceGetInputDeviceName,int which);

	/** Get count of input devices in X-Device SDK.*/
	DLL_DEF(int,XDeviceGetInputDeviceCount);
	/** Get handles of all input devices identified by type.*/
	DLL_DEF(int,XDeviceGetInputDevices,int type,int* whichBuffer,int whichBufferSize);

	// I/O
	
	/** Get the input state of input device identified by handle.*/
	DLL_DEF(int,XDeviceGetInputState,int which, void* state);
	/** Send a message to the input device identified by handle.*/
	DLL_DEF(int,XDeviceSendMessage,int which, int Msg, int wParam, int lParam);
	
	/** Reset the input state of input device identified by handle.*/
	DLL_DEF(int,XDeviceResetInputState,int which);
	/** Update the input device identified by handle.*/
	DLL_DEF(int,XDeviceUpdateInputState,int which);
	/** Update the input buffer of input device identified by handle.*/
	DLL_DEF(int,XDeviceUpdateInputBuffer,int which, unsigned char* buffer, int offset, int count);
	
	/** Serialize a message from the input device identified by handle.*/
	DLL_DEF(int,XDeviceSerializeMessage,unsigned char* buffer, int offset, int count,
		                                int which, int Msg, int wParam, int lParam);
	/** Send a buffer to the input device identified by handle.*/
	DLL_DEF(int,XDeviceSendBuffer,int which, unsigned char* buffer, int offset, int count);

	// Event

	/** Set an event listener on the event identified by eventID of the input device identified by handle.*/
	DLL_DEF(int,XDeviceSetEventListener,int which, int eventID, void* listener);
	/** Dispatch the event identified by eventID of the input device identified by handle.*/
	DLL_DEF(int,XDeviceDispatchEventListener,int which, int eventID);

	// Get/Set

	/** Get a bool value of input device identified by handle.*/
	DLL_DEF(bool,XDeviceGetBool,int which, int fieldID, bool defaultValue);
	/** Set a bool value of input device identified by handle.*/
	DLL_DEF(void,XDeviceSetBool,int which, int fieldID, bool value);
	/** Get an int value of input device identified by handle.*/
	DLL_DEF(int,XDeviceGetInt,int which, int fieldID, int defaultValue);
	/** Set an int value of input device identified by handle.*/
	DLL_DEF(void,XDeviceSetInt,int which, int fieldID, int value);
	/** Get a float value of input device identified by handle.*/
	DLL_DEF(float,XDeviceGetFloat,int which, int fieldID, float defaultValue);
	/** Set a float value of input device identified by handle.*/
	DLL_DEF(void,XDeviceSetFloat,int which, int fieldID, float value);
	/** Get a object value of input device identified by handle.*/
	DLL_DEF(void,XDeviceGetObject,int which, int fieldID, unsigned char* buffer, int offset, int count);
	/** Set a object value of input device identified by handle.*/
	DLL_DEF(void,XDeviceSetObject,int which, int fieldID, unsigned char* buffer, int offset, int count);

	  //Ext

	/** Get the node position of input device identified by handle.*/
	DLL_DEF(int,XDeviceGetNodePosition,int which, int history, int node, float* position);

	/** Update the node pose(position and rotation) of input device identified by handle.*/
	DLL_DEF(int,XDeviceUpdateNodePose,int which, int node, float* position, float* rotation);

	/** */
	DLL_DEF(int,XDeviceGetTrackerPose,int which,float* height,float* depth,float* pitch);

	/** */
	DLL_DEF(int,XDeviceSetTrackerPose,int which,float height,float depth,float pitch);

	/** */
	DLL_DEF(int,XDeviceTestNode,int which,int node,void* result);

	/** */
	DLL_DEF(int,XDeviceGetTickCount);
	
#undef DLL_DEF

#endif

#endif