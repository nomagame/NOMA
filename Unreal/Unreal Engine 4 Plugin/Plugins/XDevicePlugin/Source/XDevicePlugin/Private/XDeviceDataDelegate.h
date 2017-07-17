// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "XDeviceEnum.h"
#include "IXDevicePlugin.h"

class XDeviceDataDelegate{
public:
	IXDeviceController* Controller;
	int controllerHandles[3];
	ControllerState controllerStates[3];
	ControllerStateUE controllerPrevStateUEs[3];
	ControllerStateUE controllerStateUEs[3];
public:
	XDeviceDataDelegate();
	~XDeviceDataDelegate();
	void OnUpdateControllerState(int Index,ControllerState* pState);
	ControllerStateUE* GetControllerStateForControllerHand(const EControllerHand DeviceHand);
	int GetControllerHandleForControllerHand(const EControllerHand DeviceHand);
	int GetTrackedNodeForControllerHand(const EControllerHand DeviceHand);
	EControllerHand GetControllerWhichHand(int Index);
	
	void RecenterDevice(const EControllerHand DeviceHand);
	void StartVibration(EControllerHand Hand,int Percent,int Frequency,int Timeout);
	void StopVibration(EControllerHand Hand);
	float CheckBoundary(float MaxDistance); 
};