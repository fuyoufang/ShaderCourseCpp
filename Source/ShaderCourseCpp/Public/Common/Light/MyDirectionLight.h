
#pragma once

#include "CoreMinimal.h"
#include "Common/Light/MyLightSourceBase.h"
#include "MyDirectionLight.generated.h"


UCLASS()
class AMyDirectionLight : public AMyLightSourceBase
{
	GENERATED_BODY()

public:
	AMyDirectionLight();

	/// <summary>
	/// 判断物体是否在灯光照射的范围内
	/// </summary>
	virtual bool IsInRange() override { return true; }

	/// <summary>
	/// 获取灯光衰减系数
	/// </summary>
	virtual float GetDistanceAttenuationFactor() override { return 1; }

	/// <summary>
	/// 获取灯光照射方向
	/// </summary>
	/// <returns></returns>
	virtual FVector GetLightDir() override { 
		// 平行光的光照方向是 actor 的相反的方向
		return GetActorForwardVector() * FVector(-1, -1, -1);
	}

public:
	UPROPERTY(VisibleDefaultsOnly, Category = Extras)
	class UArrowComponent* Arrow;
};
