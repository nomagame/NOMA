// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "XDevicePluginPrivatePCH.h"

#include "IXDevicePlugin.h"
#include "xdevice.h"
#include "XDeviceDataDelegate.h"
#include "XDeviceComponent.h"

#include "IMotionController.h"
#include "SlateBasics.h"
#include "IPluginManager.h"

#if XDEVICE_SUPPORTED_ANDROID_PLATFORMS

#include "Android/AndroidJNI.h"
#include "Android/AndroidApplication.h"

#elif PLATFORM_WINDOWS

extern void* s_XDeviceLib;

#endif

#define LOCTEXT_NAMESPACE "XDevicePlugin"
#define PLUGIN_VERSION "1.0.0"
DEFINE_LOG_CATEGORY_STATIC(XDevicePluginLog, Log, All);

class XDeviceUtilityTimer
{
	int64 TickTime = 0;
	int64 TockTime = 0;
public:
	XDeviceUtilityTimer()
	{
		tick();
	}

	double unixTimeNow()
	{
		FDateTime timeUtc = FDateTime::UtcNow();
		return timeUtc.ToUnixTimestamp() * 10000 + timeUtc.GetMillisecond();
	}

	void tick()
	{
		TickTime = unixTimeNow();
	}

	//return time elapsed in seconds
	float tock()
	{
		TockTime = unixTimeNow();
		return (TockTime - TickTime)/1000.f;
	}
};

//UE v4.6 IM event wrappers
bool EmitKeyUpEventForKey(FKey key, int32 user, bool repeat)
{
	FKeyEvent KeyEvent(key, FSlateApplication::Get().GetModifierKeys(), user, repeat, 0, 0);
	return FSlateApplication::Get().ProcessKeyUpEvent(KeyEvent);
}

bool EmitKeyDownEventForKey(FKey key, int32 user, bool repeat)
{
	FKeyEvent KeyEvent(key, FSlateApplication::Get().GetModifierKeys(), user, repeat, 0, 0);
	return FSlateApplication::Get().ProcessKeyDownEvent(KeyEvent);
}

bool EmitAnalogInputEventForKey(FKey key, float value, int32 user, bool repeat)
{
	FAnalogInputEvent AnalogInputEvent(key, FSlateApplication::Get().GetModifierKeys(), user, repeat, 0, 0, value);
	return FSlateApplication::Get().ProcessAnalogInputEvent(AnalogInputEvent);
}

FORCEINLINE FMatrix GetMatrix(AActor* From,AActor* To){
	if(From==NULL){
		FMatrix Matrix=FMatrix();
		Matrix.SetIdentity();
		return Matrix;
	}else{
		FMatrix Matrix=From->GetTransform().ToMatrixWithScale();
		if(To==NULL){
			return Matrix;
		}else{
			return Matrix*(To->GetTransform().ToMatrixWithScale().InverseFast());
		}
	}
}

class FXDeviceController :public IXDeviceController, public IInputDevice, public IMotionController {
	
public:
	void* DLLHandle;
	XDeviceDataDelegate* DataDelegate;
	XDeviceUtilityTimer UtilityTimer;
	UXDevicePluginComponent* Component;

	/** handler to send all messages to */
	TSharedRef<FGenericApplicationMessageHandler> MessageHandler;

public:
	//Init and Runtime
	FXDeviceController(const TSharedRef< FGenericApplicationMessageHandler >& InMessageHandler)
		: MessageHandler(InMessageHandler)
	{

#if PLATFORM_WINDOWS

		//This is a fixed relative path, meaning this file needs to exist for the plugin to work, even in shipping build!
		//Todo: fix path lookup when packaged
		//FString DllFilepath = FPaths::ConvertRelativePathToFull(FPaths::Combine(*FPaths::GameDir(),
		//	TEXT("Plugins"), TEXT("XDevicePlugin"), TEXT("Binaries/Win64")), TEXT("xdevice_x64.dll"));
/**/
		//Define Paths for direct dll bind
		FString BinariesRoot = FPaths::Combine(*FPaths::GameDir(), TEXT("Binaries"));
		FString PluginRoot = IPluginManager::Get().FindPlugin("XDevicePlugin")->GetBaseDir();
		FString PlatformString;
		FString XDeviceDLLString;

#if _WIN64
		//64bit
		XDeviceDLLString = FString(TEXT("xdevice.dll"));
		PlatformString = FString(TEXT("Win64"));
#else
		//32bit
		XDeviceDLLString = FString(TEXT("xdevice.dll"));
		PlatformString = FString(TEXT("Win32"));
#endif
		
		FString DllFilepath = FPaths::ConvertRelativePathToFull(FPaths::Combine(*PluginRoot, TEXT("Binaries"), *PlatformString, *XDeviceDLLString));

		UE_LOG(XDevicePluginLog, Log, TEXT("Fetching dll from %s"), *DllFilepath);

		//Check if the file exists, if not, give a detailed log entry why
		if (!FPaths::FileExists(DllFilepath)){
			UE_LOG(XDevicePluginLog, Error, TEXT("%s File is missing (Did you copy Binaries into project root?)! XDevice Unavailable."), *XDeviceDLLString);
			return;
		}

		DLLHandle = NULL;
		DLLHandle = FPlatformProcess::GetDllHandle(*DllFilepath);

		if (!DLLHandle){
			UE_LOG(XDevicePluginLog, Error, TEXT("GetDllHandle failed, XDevice Unavailable."));
			UE_LOG(XDevicePluginLog, Error, TEXT("Full path debug: %s."), *DllFilepath);
			return;
		}
		s_XDeviceLib=DLLHandle;
#elif XDEVICE_SUPPORTED_ANDROID_PLATFORMS
#else
		UE_LOG(LogClass, Error, TEXT("Unsupported Platform. XDevice Unavailable."));
#endif
		//
		int ret=-1;
#if XDEVICE_SUPPORTED_ANDROID_PLATFORMS
		//
		UE_LOG(XDevicePluginLog, Log, TEXT("Initialize XDevice-Api on UE4 Android."));
		// Have to get the application context for initializing the XDevice Api
		JNIEnv* jenv=FAndroidApplication::GetJavaEnv();
		static jmethodID Method=FJavaWrapper::FindMethod(jenv,FJavaWrapper::GameActivityClassID,"getApplicationContext","()Landroid/content/Context;",false);
		static jobject ApplicationContext=FJavaWrapper::CallObjectMethod(jenv,FJavaWrapper::GameActivityThis,Method);
		jclass MainClass=FAndroidApplication::FindJavaClass("com/ximmerse/sdk/XDeviceApi");
		if(MainClass!=NULL){
			jmethodID initMethod=jenv->GetStaticMethodID(MainClass,"init","(Landroid/content/Context;II)I");
			if(initMethod!=NULL){
				ret=jenv->CallStaticIntMethod(MainClass,initMethod,ApplicationContext,0x4000,0x3);
			}else{
				UE_LOG(XDevicePluginLog, Error , TEXT("initMethod==NULL"));
			}
		}else{
			UE_LOG(XDevicePluginLog, Error , TEXT("MainClass==NULL"));
		}
#else
		ret=XDeviceInit();
#endif
		if(ret==0){
			UE_LOG(XDevicePluginLog, Log, TEXT("XDevice Available."));
			// Init members
			DataDelegate = new XDeviceDataDelegate();
			DataDelegate->Controller=(IXDeviceController*)this;
			Component=NULL;
			memset(Anchors,0,sizeof(AActor*)*(int)EVRNode::Count);
			// X-Device TODO : 
#define XDEVICE_ADD_EKEY(Type,Key,Desc) EKeys::AddKey(FKeyDetails(EKeysXDevice::Key, LOCTEXT(#Key,#Desc), Type));
#define XDEVICE_ADD_HANDED_EKEY(Hand,Tag,Type,Key,Desc) XDEVICE_ADD_EKEY(Type,XDevice##Hand##Key,X-Device (Tag) Desc)
#define XDEVICE_ADD_HAND_EKEYS(Hand,Tag) \
	/* Touchpad */\
	XDEVICE_ADD_HANDED_EKEY(Hand,Tag,FKeyDetails::FloatAxis,JoystickX,Joystick X)\
	XDEVICE_ADD_HANDED_EKEY(Hand,Tag,FKeyDetails::FloatAxis,JoystickY,Joystick Y)\
	XDEVICE_ADD_HANDED_EKEY(Hand,Tag,FKeyDetails::GamepadKey,JoystickMoved,Joystick Moved)\
	/* Button */\
	XDEVICE_ADD_HANDED_EKEY(Hand,Tag,FKeyDetails::GamepadKey,ButtonClick,Joystick Click)\
	XDEVICE_ADD_HANDED_EKEY(Hand,Tag,FKeyDetails::GamepadKey,ButtonApp,Button App)\
	XDEVICE_ADD_HANDED_EKEY(Hand,Tag,FKeyDetails::GamepadKey,ButtonHome,Button Home)\
	XDEVICE_ADD_HANDED_EKEY(Hand,Tag,FKeyDetails::GamepadKey,ButtonGripL,Button Grip L)\
	XDEVICE_ADD_HANDED_EKEY(Hand,Tag,FKeyDetails::GamepadKey,ButtonGripR,Button Grip R)\
	XDEVICE_ADD_HANDED_EKEY(Hand,Tag,FKeyDetails::GamepadKey,ButtonGrip,Button Grip)\
	/* Trigger */\
	XDEVICE_ADD_HANDED_EKEY(Hand,Tag,FKeyDetails::FloatAxis,Trigger,Trigger)\
	XDEVICE_ADD_HANDED_EKEY(Hand,Tag,FKeyDetails::GamepadKey,TriggerClick,Trigger Click)

			//Attach all EKeys
			XDEVICE_ADD_HAND_EKEYS(Left  , L);
			XDEVICE_ADD_HAND_EKEYS(Right , R);
			// X-Device TODO : 
			DataDelegate->controllerHandles[0]=XDeviceGetInputDeviceHandle((char*)"XCobra-0");
			DataDelegate->controllerHandles[1]=XDeviceGetInputDeviceHandle((char*)"XCobra-1");
			DataDelegate->controllerHandles[2]=XDeviceGetInputDeviceHandle((char*)"VRDevice");
		}else{
			UE_LOG(XDevicePluginLog, Log, TEXT("XDevice Unavailable."));
		}

		//Required calls at init
		IModularFeatures::Get().RegisterModularFeature(GetModularFeatureName(), this);

	}

	virtual ~FXDeviceController()
	{
		int cleanshutdown = 0;
#if XDEVICE_SUPPORTED_ANDROID_PLATFORMS
		JNIEnv* jenv=FAndroidApplication::GetJavaEnv();
		jclass MainClass=FAndroidApplication::FindJavaClass("com/ximmerse/sdk/XDeviceApi");
		jmethodID exitMethod=jenv->GetMethodID(MainClass,"exit","()I");
		cleanshutdown=jenv->CallStaticIntMethod(MainClass,exitMethod);
#else
		if(XDeviceExit!=NULL){
			cleanshutdown=XDeviceExit();
		}
#endif

#if PLATFORM_WINDOWS
		FPlatformProcess::FreeDllHandle(DLLHandle);
#endif

		if (cleanshutdown == 0)
		{
			UE_LOG(XDevicePluginLog, Log, TEXT("XDevice Clean shutdown."));
		}
		
	}

	virtual void Tick(float DeltaTime) override
	{
		//Update Data History
	}

	virtual void SendControllerEvents() override
	{
		//Use late sampling attached to SendControllerEvents
		DelegateUpdateAllData(-1.f);	//-1 signifies we should use our internal utility timer for elapsed time
		DelegateEventTick();
	}


	virtual ETrackingStatus GetControllerTrackingStatus(const int32 ControllerIndex, const EControllerHand DeviceHand) const
	{
		return ETrackingStatus::Tracked;
	}
	
	//XDevice only supports one player so ControllerIndex is ignored.
	virtual bool GetControllerOrientationAndPosition(const int32 ControllerIndex, const EControllerHand DeviceHand, FRotator& OutOrientation, FVector& OutPosition) const
	{
		bool RetVal = false;
		
		ControllerStateUE* controller = DataDelegate->GetControllerStateForControllerHand(DeviceHand);

		if (true&&controller!=nullptr)
		{
			float position[3];
			if(XDeviceGetNodePosition(2,0,DataDelegate->GetTrackedNodeForControllerHand(DeviceHand),position)==0){
				RetVal = false;
			}else{
				OutOrientation = controller->rotation.Rotator();
				//if(Anchors[(int)EVRNode::TrackerDefault]!=NULL){
				//	FMatrix LocalToTrackingSpace=GetMatrix(Anchors[(int)EVRNode::TrackerDefault],Anchors[(int)EVRNode::TrackingSpace]);
				//	OutPosition = LocalToTrackingSpace.TransformPosition(GlVectorToFVector(position)/*controller->position*/);
				//}else{
				//	OutPosition = GlVectorToFVector(position)/*controller->position*/;
				//}
				OutPosition = controller->position;
				// TODO: HeadAnchor
				if(DeviceHand==EControllerHand::Pad){
				}
				RetVal = true;
			}
		}

		return RetVal;
	}


	virtual bool Exec(UWorld* InWorld, const TCHAR* Cmd, FOutputDevice& Ar) override
	{
		return false;
	}

	void SetChannelValue(int32 UnrealControllerId, FForceFeedbackChannelType ChannelType, float Value) override
	{
	}

	void SetChannelValues(int32 UnrealControllerId, const FForceFeedbackValues& Values) override
	{
	}

	virtual void SetMessageHandler(const TSharedRef< FGenericApplicationMessageHandler >& InMessageHandler) override
	{
		MessageHandler = InMessageHandler;
	}
	

	virtual void RecenterAnchor(int id) override {
		//UE_LOG(LogTemp,Log,TEXT("IXDeviceController::RecenterAnchor(%d)"),id);
#if PLATFORM_WINDOWS
		if(XDeviceSendMessage==NULL){
			UE_LOG(LogTemp,Log,TEXT("XDeviceSendMessage==NULL"));
			return;
		}
#endif
		switch((EVRNode)id){
			case EVRNode::LeftHand:
			case EVRNode::RightHand:
			case EVRNode::CenterEye:
				XDeviceSendMessage(
					DataDelegate->GetControllerHandleForControllerHand(VRNodeToControllerHand((EVRNode)id))
					,kMessage_RecenterSensor,0,0
				);
			break;
			case EVRNode::TrackerDefault:
				XDeviceSendMessage(DataDelegate->GetControllerHandleForControllerHand((EControllerHand)3),kMessage_RecenterSensor,1,0);
				//
				AActor* TrackerAnchor=Anchors[(int)EVRNode::TrackerDefault];
				if(TrackerAnchor!=NULL){
					USceneComponent* SceneComponent=TrackerAnchor->GetRootComponent();
					if(SceneComponent!=NULL){
						float trs[10];
						memset(trs,0,sizeof(float)*10);
						XDeviceGetObject(2,0,(unsigned char*)trs,0,sizeof(float)*10);
						SceneComponent->SetRelativeLocationAndRotation(
							GlVectorToFVector(&trs[0]),
							GlQuatToFQuat(&trs[3])
						);
					}
				}
			break;
		}
	}

private:
	//Delegate Private functions
	void DelegateUpdateAllData(float DeltaTime);
	void DelegateCheckEnabledCount(bool* plugNotChecked);
	void DelegateEventTick();
};


//Public API Implementation

/** Delegate Functions, called by plugin to keep data in sync and to emit the events.*/
void FXDeviceController::DelegateUpdateAllData(float DeltaTime){//Tick-tock the timer
	float timeElapsedSinceUpdate = UtilityTimer.tock();
	UtilityTimer.tick();

	if (DeltaTime < 0) 
	{
		DeltaTime = timeElapsedSinceUpdate;
	}
	float DeltaSquared = (DeltaTime*DeltaTime);
	// TODO : Update tracker here.
	XDeviceUpdateInputState(DataDelegate->GetControllerHandleForControllerHand((EControllerHand)3));
	
	ControllerStateUE* controller;
	ControllerStateUE* previous;

	for (int i = 0; i < MAX_CONTROLLERS_SUPPORTED; i++)
	{
		if(XDeviceGetInputState(DataDelegate->controllerHandles[i],&DataDelegate->controllerStates[i])!=0){
			continue;
		}
		DataDelegate->OnUpdateControllerState(i,&DataDelegate->controllerStates[i]);
		//
		controller = &DataDelegate->controllerStateUEs[i];
		previous = &DataDelegate->controllerPrevStateUEs[i];

		//Trigger 
		if (controller->trigger < 0.5)
		{
			controller->trigger_pressed = false;
		}
		else{
			controller->trigger_pressed = true;
		}

		// X-Device TODO : 
		//Calculate Velocity, Acceleration, and angular velocity
		controller->velocity = (controller->position - previous->position) / DeltaTime;
		controller->acceleration = (controller->velocity - previous->velocity) / DeltaTime;
		controller->angular_velocity = FRotator(controller->rotation - previous->rotation);	//unscaled by delta time	
		controller->angular_velocity = FRotator(controller->angular_velocity.Pitch / DeltaSquared,
			controller->angular_velocity.Yaw / DeltaSquared,
			controller->angular_velocity.Roll / DeltaSquared);	//has to be scaled here to avoid clamping in the quaternion initialization
	}
}

/** Internal Tick - Called by the Plugin */
void FXDeviceController::DelegateEventTick(){
	
	ControllerStateUE* controller;
	ControllerStateUE* previous;

	for (int i = 0; i < MAX_CONTROLLERS_SUPPORTED; i++)
	{
		//
		controller = &DataDelegate->controllerStateUEs[i];
		previous = &DataDelegate->controllerPrevStateUEs[i];

		if(true){

			//Determine Hand to support dynamic input mapping
			bool leftHand = DataDelegate->GetControllerWhichHand(i) == EControllerHand::Left;

#define XDEVICE_DISPATCH_AXIS_EVENT(Val,MyKey,McKey) \
	EmitAnalogInputEventForKey(EKeysXDevice::XDevice##MyKey, Val, 0, 0);\
	EmitAnalogInputEventForKey(FGamepadKeyNames::MotionController_##McKey, Val, 0, 0);

#define XDEVICE_DISPATCH_JOYSTICK_EVENT(Hand) \
	XDEVICE_DISPATCH_AXIS_EVENT(controller->joystick.X,Hand##JoystickX,Hand##_Thumbstick_X) \
	XDEVICE_DISPATCH_AXIS_EVENT(controller->joystick.Y,Hand##JoystickY,Hand##_Thumbstick_Y)

#define XDEVICE_DISPATCH_BUTTON_EVENT(Val,MyKey,McKey) \
	EmitKey##Val##EventForKey(EKeysXDevice::XDevice##MyKey, 0, 0);\
	EmitKey##Val##EventForKey(FGamepadKeyNames::MotionController_##McKey, 0, 0);

#define XDEVICE_DISPATCH_TRIGGER_EVENT(Hand) \
	XDEVICE_DISPATCH_AXIS_EVENT(controller->trigger,Hand##Trigger,Hand##_TriggerAxis)\
	if (controller->trigger_pressed != previous->trigger_pressed)\
	{\
		if (controller->trigger_pressed)\
		{\
			/*xdeviceDelegate->XDeviceButtonPressed(i, XDEVICE_BUTTON_TRIGGER);*/\
			/*xdeviceDelegate->XDeviceTriggerPressed(i);*/\
			/* InputMapping */\
			XDEVICE_DISPATCH_BUTTON_EVENT(Down,Hand##TriggerClick,Hand##_Trigger)\
		}\
		else{\
			/*xdeviceDelegate->XDeviceButtonReleased(i, XDEVICE_BUTTON_TRIGGER);*/\
			/*xdeviceDelegate->XDeviceTriggerReleased(i);*/\
			/* InputMapping */\
			XDEVICE_DISPATCH_BUTTON_EVENT(Up,Hand##TriggerClick,Hand##_Trigger)\
		}\
	}
#define XDEVICE_DISPATCH_BUTTON_EVENTS(Key,MyKey,McKey) \
	if ((controller->buttons & Key) != (previous->buttons & Key))\
	{\
		if ((controller->buttons & Key) != 0)\
		{\
			/*xdeviceDelegate->XDeviceButtonPressed(i, CONTROLLER_BUTTON_);*/\
			/*xdeviceDelegate->XDevice##MyKey##Pressed(i);*/\
			/* InputMapping */\
			if (leftHand)\
			{\
				EmitKeyDownEventForKey(EKeysXDevice::XDeviceLeft##MyKey, 0, 0);\
				EmitKeyDownEventForKey(FGamepadKeyNames::MotionController_Left_##McKey, 0, 0);\
			}\
			else\
			{\
				EmitKeyDownEventForKey(EKeysXDevice::XDeviceRight##MyKey, 0, 0);\
				EmitKeyDownEventForKey(FGamepadKeyNames::MotionController_Right_##McKey, 0, 0);\
			}\
		}\
		else{\
			/*xdeviceDelegate->XDeviceButtonReleased(i, CONTROLLER_BUTTON_);*/\
			/*xdeviceDelegate->XDevice##MyKey##Released(i);*/\
			/* InputMapping */\
			if (leftHand)\
			{\
				EmitKeyUpEventForKey(EKeysXDevice::XDeviceLeft##MyKey, 0, 0);\
				EmitKeyUpEventForKey(FGamepadKeyNames::MotionController_Left_##McKey, 0, 0);\
			}\
			else\
			{\
				EmitKeyUpEventForKey(EKeysXDevice::XDeviceRight##MyKey, 0, 0);\
				EmitKeyUpEventForKey(FGamepadKeyNames::MotionController_Right_##McKey, 0, 0);\
			}\
		}\
	}
			// X-Device TODO : 

			if (controller->trigger != previous->trigger){
				//xdeviceDelegate->XDeviceTriggerChanged(i, controller->trigger);
				//InputMapping
				if(leftHand){
					XDEVICE_DISPATCH_TRIGGER_EVENT(Left);
				}else{
					XDEVICE_DISPATCH_TRIGGER_EVENT(Right);
				}
			}
			//** Buttons */
			
			XDEVICE_DISPATCH_BUTTON_EVENTS(CONTROLLER_BUTTON_TOUCH      , JoystickMoved , FaceButton1);
			XDEVICE_DISPATCH_BUTTON_EVENTS(CONTROLLER_BUTTON_CLICK      , ButtonClick   , Thumbstick);
			XDEVICE_DISPATCH_BUTTON_EVENTS(CONTROLLER_BUTTON_HOME       , ButtonHome    , FaceButton5);
			XDEVICE_DISPATCH_BUTTON_EVENTS(CONTROLLER_BUTTON_APP        , ButtonApp     , FaceButton6);
			XDEVICE_DISPATCH_BUTTON_EVENTS(CONTROLLER_BUTTON_LEFT_GRIP  , ButtonGripL   , Grip1);
			XDEVICE_DISPATCH_BUTTON_EVENTS(CONTROLLER_BUTTON_RIGHT_GRIP , ButtonGripR   , Grip2);
			XDEVICE_DISPATCH_BUTTON_EVENTS(
			(CONTROLLER_BUTTON_LEFT_GRIP|CONTROLLER_BUTTON_RIGHT_GRIP)  , ButtonGrip    , Shoulder);
			
			/** Movement */
			
			//Joystick
			if (controller->joystick.X != previous->joystick.X ||
				controller->joystick.Y != previous->joystick.Y)
			{
				//xdeviceDelegate->XDeviceJoystickMoved(i, controller->joystick);
				
				//InputMapping
				if (leftHand)
				{
					XDEVICE_DISPATCH_JOYSTICK_EVENT(Left);
				}
				else
				{
					XDEVICE_DISPATCH_JOYSTICK_EVENT(Right);
				}
			}
		}

	}

}

//Implementing the module, required
class FXDevicePlugin : public IXDevicePlugin
{
	FXDeviceController* controllerReference = nullptr;
	bool inputDeviceCreated = false;
	TArray<UXDevicePluginComponent*> delegateComponents;

	virtual TSharedPtr< class IInputDevice > CreateInputDevice(const TSharedRef< FGenericApplicationMessageHandler >& InMessageHandler) override
	{
		controllerReference = new FXDeviceController(InMessageHandler);
		delegateComponents.Empty();
		inputDeviceCreated = true;
		return TSharedPtr< class IInputDevice >(controllerReference);
	}
	
	virtual XDeviceDataDelegate* DataDelegate() override
	{
		return controllerReference->DataDelegate;
	}

	virtual void DeferedAddDelegate(UXDevicePluginComponent* delegate) override
	{
		if (inputDeviceCreated){
			controllerReference->Component=delegate;
			delegate->Controller=controllerReference;
			delegate->Delegate=controllerReference->DataDelegate;
			for (int32 i = 0; i < delegateComponents.Num(); ++i)
			{
				delegateComponents[i]->Controller = controllerReference;
				delegateComponents[i]->Delegate = controllerReference->DataDelegate;
			}
		}
		else 
		{
			// defer until later
			delegateComponents.Add(delegate);
		}
	}

};

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FXDevicePlugin, XDevicePlugin)