// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "ModuleManager.h"
#include "IInputDeviceModule.h"

class XDeviceDataDelegate;
class UXDevicePluginComponent;

#define XDEVICE_SUPPORTED_ANDROID_PLATFORMS (PLATFORM_ANDROID)

class IXDevicePlugin : public IInputDeviceModule
{

public:

	/**
	 * Singleton-like access to this module's interface.  This is just for convenience!
	 * Beware of calling this during the shutdown phase, though.  Your module might have been unloaded already.
	 *
	 * @return Returns singleton instance, loading the module on demand if needed
	 */
	static inline IXDevicePlugin& Get()
	{
		return FModuleManager::LoadModuleChecked< IXDevicePlugin >( "XDevicePlugin" );
	}

	/**
	 * Checks to see if this module is loaded and ready.  It is only valid to call Get() if IsAvailable() returns true.
	 *
	 * @return True if the module is loaded and ready to use
	 */
	static inline bool IsAvailable()
	{
		return FModuleManager::Get().IsModuleLoaded( "XDevicePlugin" );
	}

	virtual XDeviceDataDelegate* DataDelegate(){ return nullptr; };
	virtual void DeferedAddDelegate(UXDevicePluginComponent* delegate) {};

};

class AActor;
class IXDeviceController {
public:
	class AActor* Anchors[9];
	virtual void RecenterAnchor(int id){}
};