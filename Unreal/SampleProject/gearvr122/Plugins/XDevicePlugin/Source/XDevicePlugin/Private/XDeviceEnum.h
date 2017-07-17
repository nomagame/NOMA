#pragma once

#define MAX_CONTROLLERS_SUPPORTED 3
#define CONTROLLERS_PER_PLAYER	2

// Reference : https://msdn.microsoft.com/en-us/library/windows/apps/microsoft.directx_sdk.reference.xinput_gamepad
enum ControllerButton {
	CONTROLLER_BUTTON_HOME           = 0x0010,
	CONTROLLER_BUTTON_APP            = 0x0020,
	CONTROLLER_BUTTON_TOUCH          = 0x10000,
	CONTROLLER_BUTTON_LEFT_GRIP      = 0x0100,
	CONTROLLER_BUTTON_RIGHT_GRIP     = 0x0200,
	CONTROLLER_BUTTON_CLICK          = 0x0040,
	// Emulation
	CONTROLLER_BUTTON_TRIGGER        = 0x0400,
	//
	CONTROLLER_BUTTON_NONE           = 0x0,
	CONTROLLER_BUTTON_ANY            = ~CONTROLLER_BUTTON_NONE,
};

// X-Device TODO : 
#define XDEVICE_DECLARE_FKEY(Key) static const FKey Key;
#define XDEVICE_DECLARE_HANDED_FKEY(Hand,Key) XDEVICE_DECLARE_FKEY(XDevice##Hand##Key)
#define XDEVICE_DECLARE_HAND_FKEYS(Hand) \
	/* Touchpad */\
	XDEVICE_DECLARE_HANDED_FKEY(Hand,JoystickX)\
	XDEVICE_DECLARE_HANDED_FKEY(Hand,JoystickY)\
	XDEVICE_DECLARE_HANDED_FKEY(Hand,JoystickMoved)\
	/* Button */\
	XDEVICE_DECLARE_HANDED_FKEY(Hand,ButtonClick)\
	XDEVICE_DECLARE_HANDED_FKEY(Hand,ButtonApp)\
	XDEVICE_DECLARE_HANDED_FKEY(Hand,ButtonHome)\
	XDEVICE_DECLARE_HANDED_FKEY(Hand,ButtonGripL)\
	XDEVICE_DECLARE_HANDED_FKEY(Hand,ButtonGripR)\
	XDEVICE_DECLARE_HANDED_FKEY(Hand,ButtonGrip)\
	/* Trigger */\
	XDEVICE_DECLARE_HANDED_FKEY(Hand,Trigger)\
	XDEVICE_DECLARE_HANDED_FKEY(Hand,TriggerClick)

//Input Mapping Keys
struct EKeysXDevice{
	XDEVICE_DECLARE_HAND_FKEYS(Left);
	XDEVICE_DECLARE_HAND_FKEYS(Right);
};

UENUM(BlueprintType)
enum class ETrackingOrigin : uint8
{
	EyeLevel,
	FloorLevel,
};

UENUM(BlueprintType)
enum class EVRNode : uint8
{
	None,
	Head,
	LeftEye,
	RightEye,
	CenterEye,
	LeftHand,
	RightHand,
	TrackerDefault,
	TrackingSpace,
	Count,
};

// <!-- TODO

inline EVRNode ControllerHandToVRNode(EControllerHand Hand){
	switch((uint8)Hand){
		case (uint8)EControllerHand::Left:return EVRNode::LeftHand;
		case (uint8)EControllerHand::Right:return EVRNode::RightHand;
		case (uint8)EControllerHand::Pad:return EVRNode::CenterEye;
		case 3:return EVRNode::TrackerDefault;
	}
	return EVRNode::None;
}

inline EControllerHand VRNodeToControllerHand(EVRNode Node){
	switch(Node){
		case EVRNode::LeftHand:return EControllerHand::Left;
		case EVRNode::RightHand:return EControllerHand::Right;
		case EVRNode::CenterEye:return EControllerHand::Pad;
		case EVRNode::TrackerDefault:return (EControllerHand)3;
	}
	return (EControllerHand)0xff;
}

inline FVector GlVectorToFVector(float* Vector){
	//Convert OpenGL Axis to Unreal: UnrealX = - OpenGLZ   UnrealY = OpenGLX   UnrealZ = OpenGLY
	return FVector(//converted to cm from m
		-Vector[2]*100,
		 Vector[0]*100,
		 Vector[1]*100
	);
}

inline FQuat GlQuatToFQuat(float* Quat){
	return FQuat(//converted & rotation values inverted
		 Quat[2],
		-Quat[0],
		-Quat[1],
		 Quat[3]
	);
}

// TODO -->

typedef struct tagControllerState{
	// Common
	int handle;
	int timestamp;
	int state_mask;
	// Gamepad
	float axes[6];
	unsigned int buttons;
	// Motion
	float position[3];
	float rotation[4];
	float acceleration[3];
	float gyroscope[3];
}ControllerState;

typedef struct tagControllerStateUE{
	// Common
	int handle;
	int timestamp;
	int state_mask;
	// Gamepad
	FVector2D joystick;
	float trigger;
	bool trigger_pressed;
	unsigned int buttons;
	// Motion
	FVector position;
	FQuat rotation;

	//added values
	FVector velocity;				//cm/s
	FVector acceleration;			//cm/s^2
	FRotator angular_velocity;		//deg/s

}ControllerStateUE;

typedef struct tagBoundaryTestResult{
	// Returns true if the queried test would violate and/or trigger the tested boundary types.
	bool isTriggering;
	//
	int cornerIds[2];
	// Returns the distance between the queried test object and the closest tested boundary type.
	float distance;
	// Returns the closest point to the queried test object.
	float point[3];
	// Returns the normal of the closest point to the queried test object.
	float normal[3];
}BoundaryTestResult;