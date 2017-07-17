// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	C++ class header boilerplate exported from UnrealHeaderTool.
	This is automatically generated by the tools.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "ObjectBase.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
class UStaticMeshComponent;
class AActor;
#ifdef XDEVICEPLUGIN_XDeviceComponent_generated_h
#error "XDeviceComponent.generated.h already included, missing '#pragma once' in XDeviceComponent.h"
#endif
#define XDEVICEPLUGIN_XDeviceComponent_generated_h

#define gearvr122_Plugins_XDevicePlugin_Source_XDevicePlugin_Public_XDeviceComponent_h_16_RPC_WRAPPERS \
 \
	DECLARE_FUNCTION(execStopVibration) \
	{ \
		P_GET_PROPERTY(UByteProperty,Z_Param_Hand); \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		this->StopVibration(EControllerHand(Z_Param_Hand)); \
		P_NATIVE_END; \
	} \
 \
	DECLARE_FUNCTION(execStartVibration) \
	{ \
		P_GET_PROPERTY(UByteProperty,Z_Param_Hand); \
		P_GET_PROPERTY(UIntProperty,Z_Param_Percent); \
		P_GET_PROPERTY(UIntProperty,Z_Param_Frequency); \
		P_GET_PROPERTY(UIntProperty,Z_Param_Timeout); \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		this->StartVibration(EControllerHand(Z_Param_Hand),Z_Param_Percent,Z_Param_Frequency,Z_Param_Timeout); \
		P_NATIVE_END; \
	} \
 \
	DECLARE_FUNCTION(execRecenterTracker) \
	{ \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		this->RecenterTracker(); \
		P_NATIVE_END; \
	} \
 \
	DECLARE_FUNCTION(execRecenterAllControllers) \
	{ \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		this->RecenterAllControllers(); \
		P_NATIVE_END; \
	} \
 \
	DECLARE_FUNCTION(execRecenterController) \
	{ \
		P_GET_PROPERTY(UByteProperty,Z_Param_Hand); \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		this->RecenterController(EControllerHand(Z_Param_Hand)); \
		P_NATIVE_END; \
	} \
 \
	DECLARE_FUNCTION(execSetPlayAreaVisible) \
	{ \
		P_GET_PROPERTY(UFloatProperty,Z_Param_Value); \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		this->SetPlayAreaVisible(Z_Param_Value); \
		P_NATIVE_END; \
	} \
 \
	DECLARE_FUNCTION(execGetPlayAreaVisible) \
	{ \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		*(float*)Z_Param__Result=this->GetPlayAreaVisible(); \
		P_NATIVE_END; \
	} \
 \
	DECLARE_FUNCTION(execSetPlayAreaRenderer) \
	{ \
		P_GET_OBJECT(UStaticMeshComponent,Z_Param_Value); \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		this->SetPlayAreaRenderer(Z_Param_Value); \
		P_NATIVE_END; \
	} \
 \
	DECLARE_FUNCTION(execGetPlayAreaRenderer) \
	{ \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		*(UStaticMeshComponent**)Z_Param__Result=this->GetPlayAreaRenderer(); \
		P_NATIVE_END; \
	} \
 \
	DECLARE_FUNCTION(execSetVisibleObjectLinks) \
	{ \
		P_GET_OBJECT(AActor,Z_Param_LeftObject); \
		P_GET_OBJECT(AActor,Z_Param_RightObject); \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		this->SetVisibleObjectLinks(Z_Param_LeftObject,Z_Param_RightObject); \
		P_NATIVE_END; \
	} \
 \
	DECLARE_FUNCTION(execSetAnchor) \
	{ \
		P_GET_PROPERTY(UByteProperty,Z_Param_Node); \
		P_GET_OBJECT(AActor,Z_Param_Anchor); \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		this->SetAnchor(EVRNode(Z_Param_Node),Z_Param_Anchor); \
		P_NATIVE_END; \
	} \
 \
	DECLARE_FUNCTION(execGetAnchor) \
	{ \
		P_GET_PROPERTY(UByteProperty,Z_Param_Node); \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		*(AActor**)Z_Param__Result=this->GetAnchor(EVRNode(Z_Param_Node)); \
		P_NATIVE_END; \
	} \
 \
	DECLARE_FUNCTION(execInit) \
	{ \
		P_GET_OBJECT(AActor,Z_Param_VRContext); \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		this->Init(Z_Param_VRContext); \
		P_NATIVE_END; \
	}


#define gearvr122_Plugins_XDevicePlugin_Source_XDevicePlugin_Public_XDeviceComponent_h_16_RPC_WRAPPERS_NO_PURE_DECLS \
 \
	DECLARE_FUNCTION(execStopVibration) \
	{ \
		P_GET_PROPERTY(UByteProperty,Z_Param_Hand); \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		this->StopVibration(EControllerHand(Z_Param_Hand)); \
		P_NATIVE_END; \
	} \
 \
	DECLARE_FUNCTION(execStartVibration) \
	{ \
		P_GET_PROPERTY(UByteProperty,Z_Param_Hand); \
		P_GET_PROPERTY(UIntProperty,Z_Param_Percent); \
		P_GET_PROPERTY(UIntProperty,Z_Param_Frequency); \
		P_GET_PROPERTY(UIntProperty,Z_Param_Timeout); \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		this->StartVibration(EControllerHand(Z_Param_Hand),Z_Param_Percent,Z_Param_Frequency,Z_Param_Timeout); \
		P_NATIVE_END; \
	} \
 \
	DECLARE_FUNCTION(execRecenterTracker) \
	{ \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		this->RecenterTracker(); \
		P_NATIVE_END; \
	} \
 \
	DECLARE_FUNCTION(execRecenterAllControllers) \
	{ \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		this->RecenterAllControllers(); \
		P_NATIVE_END; \
	} \
 \
	DECLARE_FUNCTION(execRecenterController) \
	{ \
		P_GET_PROPERTY(UByteProperty,Z_Param_Hand); \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		this->RecenterController(EControllerHand(Z_Param_Hand)); \
		P_NATIVE_END; \
	} \
 \
	DECLARE_FUNCTION(execSetPlayAreaVisible) \
	{ \
		P_GET_PROPERTY(UFloatProperty,Z_Param_Value); \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		this->SetPlayAreaVisible(Z_Param_Value); \
		P_NATIVE_END; \
	} \
 \
	DECLARE_FUNCTION(execGetPlayAreaVisible) \
	{ \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		*(float*)Z_Param__Result=this->GetPlayAreaVisible(); \
		P_NATIVE_END; \
	} \
 \
	DECLARE_FUNCTION(execSetPlayAreaRenderer) \
	{ \
		P_GET_OBJECT(UStaticMeshComponent,Z_Param_Value); \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		this->SetPlayAreaRenderer(Z_Param_Value); \
		P_NATIVE_END; \
	} \
 \
	DECLARE_FUNCTION(execGetPlayAreaRenderer) \
	{ \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		*(UStaticMeshComponent**)Z_Param__Result=this->GetPlayAreaRenderer(); \
		P_NATIVE_END; \
	} \
 \
	DECLARE_FUNCTION(execSetVisibleObjectLinks) \
	{ \
		P_GET_OBJECT(AActor,Z_Param_LeftObject); \
		P_GET_OBJECT(AActor,Z_Param_RightObject); \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		this->SetVisibleObjectLinks(Z_Param_LeftObject,Z_Param_RightObject); \
		P_NATIVE_END; \
	} \
 \
	DECLARE_FUNCTION(execSetAnchor) \
	{ \
		P_GET_PROPERTY(UByteProperty,Z_Param_Node); \
		P_GET_OBJECT(AActor,Z_Param_Anchor); \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		this->SetAnchor(EVRNode(Z_Param_Node),Z_Param_Anchor); \
		P_NATIVE_END; \
	} \
 \
	DECLARE_FUNCTION(execGetAnchor) \
	{ \
		P_GET_PROPERTY(UByteProperty,Z_Param_Node); \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		*(AActor**)Z_Param__Result=this->GetAnchor(EVRNode(Z_Param_Node)); \
		P_NATIVE_END; \
	} \
 \
	DECLARE_FUNCTION(execInit) \
	{ \
		P_GET_OBJECT(AActor,Z_Param_VRContext); \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		this->Init(Z_Param_VRContext); \
		P_NATIVE_END; \
	}


#define gearvr122_Plugins_XDevicePlugin_Source_XDevicePlugin_Public_XDeviceComponent_h_16_INCLASS_NO_PURE_DECLS \
	private: \
	static void StaticRegisterNativesUXDevicePluginComponent(); \
	friend XDEVICEPLUGIN_API class UClass* Z_Construct_UClass_UXDevicePluginComponent(); \
	public: \
	DECLARE_CLASS(UXDevicePluginComponent, UActorComponent, COMPILED_IN_FLAGS(0), 0, TEXT("/Script/XDevicePlugin"), NO_API) \
	DECLARE_SERIALIZER(UXDevicePluginComponent) \
	/** Indicates whether the class is compiled into the engine */ \
	enum {IsIntrinsic=COMPILED_IN_INTRINSIC};


#define gearvr122_Plugins_XDevicePlugin_Source_XDevicePlugin_Public_XDeviceComponent_h_16_INCLASS \
	private: \
	static void StaticRegisterNativesUXDevicePluginComponent(); \
	friend XDEVICEPLUGIN_API class UClass* Z_Construct_UClass_UXDevicePluginComponent(); \
	public: \
	DECLARE_CLASS(UXDevicePluginComponent, UActorComponent, COMPILED_IN_FLAGS(0), 0, TEXT("/Script/XDevicePlugin"), NO_API) \
	DECLARE_SERIALIZER(UXDevicePluginComponent) \
	/** Indicates whether the class is compiled into the engine */ \
	enum {IsIntrinsic=COMPILED_IN_INTRINSIC};


#define gearvr122_Plugins_XDevicePlugin_Source_XDevicePlugin_Public_XDeviceComponent_h_16_STANDARD_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	NO_API UXDevicePluginComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(UXDevicePluginComponent) \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, UXDevicePluginComponent); \
DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(UXDevicePluginComponent); \
private: \
	/** Private copy-constructor, should never be used */ \
	NO_API UXDevicePluginComponent(const UXDevicePluginComponent& InCopy); \
public:


#define gearvr122_Plugins_XDevicePlugin_Source_XDevicePlugin_Public_XDeviceComponent_h_16_ENHANCED_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	NO_API UXDevicePluginComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()) : Super(ObjectInitializer) { }; \
private: \
	/** Private copy-constructor, should never be used */ \
	NO_API UXDevicePluginComponent(const UXDevicePluginComponent& InCopy); \
public: \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, UXDevicePluginComponent); \
DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(UXDevicePluginComponent); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(UXDevicePluginComponent)


#define gearvr122_Plugins_XDevicePlugin_Source_XDevicePlugin_Public_XDeviceComponent_h_13_PROLOG
#define gearvr122_Plugins_XDevicePlugin_Source_XDevicePlugin_Public_XDeviceComponent_h_16_GENERATED_BODY_LEGACY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	gearvr122_Plugins_XDevicePlugin_Source_XDevicePlugin_Public_XDeviceComponent_h_16_RPC_WRAPPERS \
	gearvr122_Plugins_XDevicePlugin_Source_XDevicePlugin_Public_XDeviceComponent_h_16_INCLASS \
	gearvr122_Plugins_XDevicePlugin_Source_XDevicePlugin_Public_XDeviceComponent_h_16_STANDARD_CONSTRUCTORS \
public: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


#define gearvr122_Plugins_XDevicePlugin_Source_XDevicePlugin_Public_XDeviceComponent_h_16_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	gearvr122_Plugins_XDevicePlugin_Source_XDevicePlugin_Public_XDeviceComponent_h_16_RPC_WRAPPERS_NO_PURE_DECLS \
	gearvr122_Plugins_XDevicePlugin_Source_XDevicePlugin_Public_XDeviceComponent_h_16_INCLASS_NO_PURE_DECLS \
	gearvr122_Plugins_XDevicePlugin_Source_XDevicePlugin_Public_XDeviceComponent_h_16_ENHANCED_CONSTRUCTORS \
static_assert(false, "Unknown access specifier for GENERATED_BODY() macro in class XDevicePluginComponent."); \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID gearvr122_Plugins_XDevicePlugin_Source_XDevicePlugin_Public_XDeviceComponent_h


PRAGMA_ENABLE_DEPRECATION_WARNINGS
