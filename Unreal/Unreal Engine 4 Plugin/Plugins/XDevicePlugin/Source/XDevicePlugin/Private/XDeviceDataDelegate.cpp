// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "XDevicePluginPrivatePCH.h"
#include "XDeviceDataDelegate.h"
#include "xdevice.h"


#undef XDEVICE_DECLARE_FKEY
#define XDEVICE_DECLARE_FKEY(Key) const FKey EKeysXDevice::Key(#Key);

//FKey declarations
//Define each FKey const in a .cpp so we can compile
XDEVICE_DECLARE_HAND_FKEYS(Left);
XDEVICE_DECLARE_HAND_FKEYS(Right);

XDeviceDataDelegate::XDeviceDataDelegate(){
	Controller=NULL;
	memset(this->controllerStates,0,sizeof(ControllerState)*MAX_CONTROLLERS_SUPPORTED);
	memset(this->controllerPrevStateUEs,0,sizeof(ControllerStateUE)*MAX_CONTROLLERS_SUPPORTED);
	memset(this->controllerStateUEs,0,sizeof(ControllerStateUE)*MAX_CONTROLLERS_SUPPORTED);
}

XDeviceDataDelegate::~XDeviceDataDelegate(){
}

void XDeviceDataDelegate::OnUpdateControllerState(int Index,ControllerState* pState){
	//
	memcpy(&this->controllerPrevStateUEs[Index],&this->controllerStateUEs[Index],sizeof(ControllerStateUE));
	//
	controllerStateUEs[Index].buttons=pState->buttons;
	int i=0;
	controllerStateUEs[Index].trigger=pState->axes[i++];
	//controllerStateUEs[index].trigger_pressed=
	//	controllerStateUEs[index].trigger>0.9f;
	++i;
	if((controllerStateUEs[Index].buttons&CONTROLLER_BUTTON_TOUCH)==0){
		controllerStateUEs[Index].joystick.Set(0.0f,0.0f);
	}else{
		controllerStateUEs[Index].joystick.Set(
			pState->axes[i+0],
			pState->axes[i+1]
		);
	}
	//Convert OpenGL Axis to Unreal: UnrealX = - OpenGLZ   UnrealY = OpenGLX   UnrealZ = OpenGLY
	controllerStateUEs[Index].position.Set(//converted to cm from m
		-pState->position[2]*100,
		 pState->position[0]*100,
		 pState->position[1]*100
	);
	controllerStateUEs[Index].rotation=FQuat(//converted & rotation values inverted
		 pState->rotation[2],
		-pState->rotation[0],
		-pState->rotation[1],
		 pState->rotation[3]
	);
}

// <!-- TODO

ControllerStateUE* XDeviceDataDelegate::GetControllerStateForControllerHand(const EControllerHand DeviceHand){
	switch(DeviceHand){
		case  EControllerHand::Left:
		return &this->controllerStateUEs[0];
		case  EControllerHand::Right:
		return &this->controllerStateUEs[1];
		case  EControllerHand::Pad:
		return &this->controllerStateUEs[2];
		default:
		return NULL;
	}
}

int XDeviceDataDelegate::GetControllerHandleForControllerHand(const EControllerHand DeviceHand){
	switch((uint8)DeviceHand){
		case (uint8)EControllerHand::Left:
		return this->controllerHandles[0];
		case (uint8)EControllerHand::Right:
		return this->controllerHandles[1];
		case (uint8)EControllerHand::Pad:
		return this->controllerHandles[2];
		case 3:
		return 2;
		default:
		return -1;
	}
}

int XDeviceDataDelegate::GetTrackedNodeForControllerHand(const EControllerHand DeviceHand){
	switch(DeviceHand){
		case EControllerHand::Left:
		return 0;
		case EControllerHand::Right:
		return 1;
		case EControllerHand::Pad:
		return 2;
		default:
		return -1;
	}
}

// TODO -->

EControllerHand XDeviceDataDelegate::GetControllerWhichHand(int Index){
	switch(Index){
		case 0:
		return EControllerHand::Left;
		case 1:
		return EControllerHand::Right;
		default:
		return EControllerHand::Pad;
	}
}

void XDeviceDataDelegate::RecenterDevice(const EControllerHand DeviceHand){
	//
	//UE_LOG(LogTemp,Log,TEXT("XDeviceDataDelegate::RecenterDevice()"));
	if(Controller!=NULL){
		Controller->RecenterAnchor((int)ControllerHandToVRNode(DeviceHand));
	}
}

void XDeviceDataDelegate::StartVibration(EControllerHand Hand,int Percent,int Frequency,int Timeout){
	int Handle=GetControllerHandleForControllerHand(Hand);
	if(Handle>=0){
		XDeviceSendMessage(Handle,kMessage_TriggerVibration,(Percent&0xFFFF)|((Frequency<<16)&0xFFFF0000),Timeout);
	}
}

void XDeviceDataDelegate::StopVibration(EControllerHand Hand){
	int Handle=GetControllerHandleForControllerHand(Hand);
	if(Handle>=0){
		XDeviceSendMessage(Handle,kMessage_TriggerVibration,-1,0);
	}
}

float XDeviceDataDelegate::CheckBoundary(float MaxDistance){
	//
	BoundaryTestResult Result;
	int Handle=GetControllerHandleForControllerHand((EControllerHand)3);
	float MinDistance=100.0f;
	int IntRet;
	for(int i=0;i<3;++i){
		IntRet=XDeviceTestNode(Handle,i,&Result);
		if(IntRet>=0){
			if(IntRet==0){// Outside
				if(Result.distance>0.0f){
					Result.distance*=-1;
				}
			}
			if(Result.distance<MinDistance){
				MinDistance=Result.distance;
			}
		}
	}
	return 1.0f-FMath::Clamp(MinDistance/MaxDistance,0.0f,1.0f);
}