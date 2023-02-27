
#pragma once

#include "CoreMinimal.h"
#include "Common/Light/MyLightSourceBase.h"
#include "MyPointLight.generated.h"


UCLASS()
class AMyPointLight : public AMyLightSourceBase
{
	GENERATED_BODY()

public:
	AMyPointLight();

	virtual void BeginPlay() override;

	/// <summary>
	/// 判断物体是否在灯光照射的范围内
	/// </summary>
	virtual bool IsInRange(FVector InWorldPos) override { 
		L = GetActorLocation() - InWorldPos;
		LSize = L.Size();
		L.Normalize();

		if (LSize >= FalloffEnd)
		{
			return false;
		}
		else 
		{
			return true;
		}
	}

	/// <summary>
	/// 获取灯光衰减系数
	/// </summary>
	virtual float GetDistanceAttenuationFactor() override { 
		return FMath::Clamp((FalloffEnd - LSize) / StartToEndLength, 0, 1);
	}

	/// <summary>
	/// 获取灯光照射方向
	/// </summary>
	/// <returns></returns>
	virtual FVector GetLightDir() override { 
		return L;
	}

public:

	/// 衰减开始的距离
	float FalloffStart = 300;

	/// 衰减结束的距离
	float FalloffEnd = 600;

	/// 
	float StartToEndLength;
public:
	/// 开始衰减
	UPROPERTY()
	class USphereComponent* Start;

	/// 衰减结束
	UPROPERTY()
	class USphereComponent* End;

private:
	/// <summary>
	/// 点和光源的距离
	/// </summary>
	float LSize;

	/// <summary>
	/// 光源到指定点之间的方向
	/// </summary>
	FVector L;
};
