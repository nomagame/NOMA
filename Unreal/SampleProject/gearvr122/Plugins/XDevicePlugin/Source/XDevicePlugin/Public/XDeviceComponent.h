#pragma once

#include "XDevicePluginPrivatePCH.h"
#include "IXDevicePlugin.h"
#include "XDeviceEnum.h"
#include "XDeviceComponent.generated.h"

class XDeviceDataDelegate;
class UMotionControllerComponent;

//These macros cannot be multi-line or it will not compile

UCLASS(ClassGroup="Input Controller", meta=(BlueprintSpawnableComponent))
class UXDevicePluginComponent : public UActorComponent
{
	GENERATED_UCLASS_BODY()
public:

	UPROPERTY(EditAnywhere, Category = "XDevice Properties")
	ETrackingOrigin TrackingOrigin;
	UPROPERTY(EditAnywhere, Category = "XDevice Properties")
	UClass* AnchorClass;
	/*UPROPERTY(EditAnywhere, Category = "XDevice Properties")
	UClass* LeftHandClass;
	UPROPERTY(EditAnywhere, Category = "XDevice Properties")
	UClass* RightHandClass;*/

	UFUNCTION(BlueprintCallable, Category = "XDevice Functions")
	void Init(AActor* VRContext);

	UFUNCTION(BlueprintCallable, Category = "XDevice Functions")
	AActor* GetAnchor(EVRNode Node);

	UFUNCTION(BlueprintCallable, Category = "XDevice Functions")
	void SetAnchor(EVRNode Node,AActor* Anchor);

	UFUNCTION(BlueprintCallable, Category = "XDevice Functions")
	void SetVisibleObjectLinks(AActor* LeftObject, AActor* RightObject);
	
	UFUNCTION(BlueprintCallable, Category = "XDevice Functions")
	UStaticMeshComponent* GetPlayAreaRenderer();
	
	UFUNCTION(BlueprintCallable, Category = "XDevice Functions")
	void SetPlayAreaRenderer(UStaticMeshComponent* Value);
	
	UFUNCTION(BlueprintCallable, Category = "XDevice Functions")
	float GetPlayAreaVisible();
	
	UFUNCTION(BlueprintCallable, Category = "XDevice Functions")
	void SetPlayAreaVisible(float Value);

	UFUNCTION(BlueprintCallable, Category = "XDevice Functions")
	void RecenterController(EControllerHand Hand);

	UFUNCTION(BlueprintCallable, Category = "XDevice Functions")
	void RecenterAllControllers();

	UFUNCTION(BlueprintCallable, Category = "XDevice Functions")
	void RecenterTracker();

	UFUNCTION(BlueprintCallable, Category = "XDevice Functions")
	void StartVibration(EControllerHand Hand,int Percent,int Frequency,int Timeout);

	UFUNCTION(BlueprintCallable, Category = "XDevice Functions")
	void StopVibration(EControllerHand Hand);

public:
	XDeviceDataDelegate* Delegate;
	IXDeviceController* Controller;

	AActor* LeftVisibleObject;
	AActor* RightVisibleObject;
	UStaticMeshComponent* PlayAreaRenderer;

protected:
	virtual void InitializeComponent() override;
	virtual void UninitializeComponent() override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
