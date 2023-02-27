#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyLightSourceBase.generated.h"

/// <summary>
/// 灯光基类
/// </summary>
UCLASS()
class AMyLightSourceBase : public AActor
{
	GENERATED_BODY()

public:
	AMyLightSourceBase();

public:
	/// <summary>
	/// 灯光颜色
	/// </summary>
	UPROPERTY(EditAnywhere, Category = "Base")
	FVector LightSourceColor;

	/// <summary>
	/// 判断物体是否在灯光照射的范围内
	/// </summary>
	virtual bool IsInRange(FVector InWorldPos) { return false; }
	
	/// <summary>
	/// 获取灯光衰减系数
	/// </summary>
	virtual float GetDistanceAttenuationFactor() { return 0; }

	/// <summary>
	/// 获取灯光照射方向
	/// </summary>
	/// <returns></returns>
	virtual FVector GetLightDir() { return FVector(0, 0, 0); }
};