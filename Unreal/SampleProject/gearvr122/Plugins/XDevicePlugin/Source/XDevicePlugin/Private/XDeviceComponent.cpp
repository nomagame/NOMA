#include "XDevicePluginPrivatePCH.h"
#include "XDeviceComponent.h"
#include "XDeviceDataDelegate.h"
#include "Engine.h"
#include "CoreUObject.h"
#include "Kismet/HeadMountedDisplayFunctionLibrary.h"

template< class T >
T* NewGameObject(UWorld* World,const FName Name){
	FActorSpawnParameters SpawnParams;SpawnParams.Name=Name;
	T* GameObject=World->SpawnActor<T>(SpawnParams);
	/*USceneComponent* DefaultRootComponent=NewObject<USceneComponent>(GameObject,"root",RF_NoFlags,nullptr,false,nullptr);
	UChildActorComponent* ChildActorComponent = NewObject<UChildActorComponent>(GameObject, "name", RF_NoFlags, nullptr, false, nullptr);
	GameObject->AddInstanceComponent(DefaultRootComponent);
	GameObject->AddInstanceComponent(ChildActorComponent);*/
	return GameObject;
}

AActor* NewGameObject(UWorld* World,UClass* Class,const FName Name){
	if(Class==NULL){
		return NewGameObject<APawn>(World,Name);
	}
	FActorSpawnParameters SpawnParams;SpawnParams.Name=Name;
	AActor* GameObject=World->SpawnActor<AActor>(Class,SpawnParams);
	return GameObject;
}

UXDevicePluginComponent::UXDevicePluginComponent(const FObjectInitializer &init) : UActorComponent(init)
{
	bWantsInitializeComponent = true;
	bAutoActivate = true;
}

void UXDevicePluginComponent::InitializeComponent()
{
	Super::InitializeComponent();
}

void UXDevicePluginComponent::UninitializeComponent()
{
	//remove ourselves from the delegates
	//dataDelegate->RemoveEventDelegate(this);
	//dataDelegate = nullptr;

	Super::UninitializeComponent();
}

void UXDevicePluginComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction){
	Super::TickComponent(DeltaTime,TickType,ThisTickFunction);
	//
	if(Delegate!=NULL){
	if(PlayAreaRenderer!=NULL){
		float Value=Delegate->CheckBoundary(.5f);
		SetPlayAreaVisible(Value);
	}}
}
	
void UXDevicePluginComponent::Init(AActor* VRContext){
	UE_LOG(LogTemp,Log,TEXT("UXDevicePluginComponent::Init()"));
	if(VRContext==NULL){VRContext=GetOwner();}
	//
	UWorld* const World=GetWorld();
	USceneComponent* SceneComponent=NULL;
	AActor* TrackingSpace=NewGameObject(World,AnchorClass,"TrackingSpace");
	AActor* TrackerAnchor=NewGameObject(World,AnchorClass,"TrackerAnchor");
	/*AActor* LeftDisplayObject=NewGameObject(World,LeftHandClass,"DisplayObject (L)");
	AActor* RightDisplayObject=NewGameObject(World,RightHandClass,"DisplayObject (R)");
	SetVisibleObjectLinks(LeftDisplayObject,RightDisplayObject);*/
	TrackingSpace->AttachToActor(VRContext,FAttachmentTransformRules::KeepRelativeTransform);
		//
		TrackerAnchor->AttachToActor(TrackingSpace,FAttachmentTransformRules::KeepRelativeTransform);
		SceneComponent=TrackerAnchor->GetRootComponent();
		if(SceneComponent!=NULL){
			SceneComponent->SetRelativeRotation(FRotator(-15.0f,180.0f,0.0f));// 15 degree.
			SceneComponent->SetRelativeLocation(FVector(150.0f,0,// 1.5m
				TrackingOrigin==ETrackingOrigin::FloorLevel?167.5f:0));// TODO : Tracker Height
		}
		//
		/*LeftDisplayObject->AttachToActor(TrackingSpace,FAttachmentTransformRules::KeepRelativeTransform);
		RightDisplayObject->AttachToActor(TrackingSpace,FAttachmentTransformRules::KeepRelativeTransform);*/
	SetAnchor(EVRNode::TrackingSpace,TrackingSpace);
	SetAnchor(EVRNode::TrackerDefault,TrackerAnchor);
	//
	if(PlayAreaRenderer!=NULL){
		PlayAreaRenderer->SetRelativeLocation(FVector(0.0f,0.0f,TrackingOrigin==ETrackingOrigin::EyeLevel?-167.5f:0));// TODO : Tracker Height
		//PlayAreaRenderer->SetRelativeRotation(FRotator(0.0f,0.0f,0.0f));
	}
	SetPlayAreaVisible(0.25f);

	PrimaryComponentTick.bCanEverTick = true;
	Delegate = NULL;
	Controller = NULL;
	IXDevicePlugin::Get().DeferedAddDelegate(this);
}

AActor* UXDevicePluginComponent::GetAnchor(EVRNode Node){
	if(Controller!=NULL){
		return Controller->Anchors[(int)Node];
	}
	return NULL;
}

void UXDevicePluginComponent::SetAnchor(EVRNode Node,AActor* Anchor){
	if(Controller!=NULL){
		Controller->Anchors[(int)Node]=Anchor;
	}
}

void UXDevicePluginComponent::SetVisibleObjectLinks(AActor* PassedLeftObject, AActor* PassedRightObject){
	LeftVisibleObject = PassedLeftObject;
	RightVisibleObject = PassedRightObject;
	//
	//AActor* LeftHand=GetAnchor(EVRNode::LeftHand);
	//if(LeftHand!=NULL&&LeftVisibleObject!=NULL){
	//	LeftVisibleObject->AttachToActor(LeftHand,FAttachmentTransformRules::KeepRelativeTransform);
	//}
	////
	//AActor* RightHand=GetAnchor(EVRNode::RightHand);
	//if(RightHand!=NULL&&RightVisibleObject!=NULL){
	//	RightVisibleObject->AttachToActor(RightHand,FAttachmentTransformRules::KeepRelativeTransform);
	//}
}

float UXDevicePluginComponent::GetPlayAreaVisible(){
	return 0.0f;
}

void UXDevicePluginComponent::SetPlayAreaVisible(float Value){
	if(PlayAreaRenderer!=NULL){
		PlayAreaRenderer->SetScalarParameterValueOnMaterials("Alpha",Value);
		PlayAreaRenderer->SetVisibility(Value>0.0f);
	}
}

UStaticMeshComponent* UXDevicePluginComponent::GetPlayAreaRenderer(){
	return PlayAreaRenderer;
}

void UXDevicePluginComponent::SetPlayAreaRenderer(UStaticMeshComponent* Value){
	PlayAreaRenderer=Value;
}

void UXDevicePluginComponent::RecenterController(EControllerHand Hand){
	if(Delegate!=NULL){
		Delegate->RecenterDevice(Hand);
	}
}

void UXDevicePluginComponent::RecenterAllControllers(){
	if(Delegate!=NULL){
		for(int i=0,imax=3;i<imax;++i){
			Delegate->RecenterDevice((EControllerHand)i);
		}
	}
	// HMD
	/*if(UHeadMountedDisplayFunctionLibrary::IsHeadMountedDisplayEnabled()){
		UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition(0.0f,
			EOrientPositionSelector::Type::OrientationAndPosition
		);
	}else{
		UE_LOG(LogTemp,Log,TEXT("No HeadMountedDisplay can be recentered at UXDevicePluginComponent::RecenterAllControllers()."));
	}*/
}

void UXDevicePluginComponent::RecenterTracker(){
	RecenterAllControllers();
	//
	if(Delegate!=NULL){
		Delegate->RecenterDevice((EControllerHand)3);
	}
	if(PlayAreaRenderer!=NULL){
		AActor* TrackerAnchor=GetAnchor(EVRNode::TrackerDefault);
		if(TrackerAnchor!=NULL){
			USceneComponent* SceneComponent=TrackerAnchor->GetRootComponent();
			float Yaw=180.0f+SceneComponent->GetRelativeTransform().Rotator().Yaw;//New Forward.
			FVector NewPos=SceneComponent->GetRelativeTransform().GetLocation()-FRotator(0,Yaw,0).RotateVector(FVector(150.0f,0,0));// 1.5m
			NewPos.Z=PlayAreaRenderer->GetRelativeTransform().GetLocation().Z;
			PlayAreaRenderer->SetRelativeLocation(NewPos);
			PlayAreaRenderer->SetRelativeRotation(FRotator(0,Yaw,0));
			//UE_LOG(LogTemp,Log,TEXT("Update PlayAreaRenderer at RecenterTracker():%f"),Yaw);
		}
	}
}

void UXDevicePluginComponent::StartVibration(EControllerHand Hand,int Percent,int Frequency,int Timeout){
	if(Delegate!=NULL){
		Delegate->StartVibration(Hand,Percent,Frequency,Timeout);
	}
}

void UXDevicePluginComponent::StopVibration(EControllerHand Hand){
	if(Delegate!=NULL){
		Delegate->StopVibration(Hand);
	}
}