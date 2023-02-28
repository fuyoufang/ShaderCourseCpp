
#pragma once

#include "CoreMinimal.h"
#include "Common/Light/MyLightSourceBase.h"
#include "MySpotLight.generated.h"


UCLASS()
class AMySpotLight : public AMyLightSourceBase
{
	GENERATED_BODY()

public:
	AMySpotLight();

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
	virtual float GetDistanceAttenuationFactor() override;

	/// <summary>
	/// 获取灯光照射方向
	/// </summary>
	/// <returns></returns>
	virtual FVector GetLightDir() override { 
		return L;
	}

public:
	/// 衰减开始的距离
	UPROPERTY(EditAnywhere, Category = "SpotLight")
	float FalloffStart = 300;

	/// 衰减结束的距离
	UPROPERTY(EditAnywhere, Category = "SpotLight")
	float FalloffEnd = 600;

	/// <summary>
	/// 衰减的内锥角
	/// </summary>
	UPROPERTY(EditAnywhere, Category = "SpotLight")
	float InnerAngle = 30;

	/// <summary>
	/// 衰减的外锥角
	/// </summary>
	UPROPERTY(EditAnywhere, Category = "SpotLight")
	float OuterAngle = 60;

	// 用于计算的临时变量
private:
	/// <summary>
	/// FalloffEnd 和 FalloffStart 的差值
	/// </summary>
	float StartToEndLength;

	/// <summary>
	/// cos(InnerAngle / 2) - cos(OuterAngle / 2) 的值
	/// </summary>
	float CosHalfInnerAngleSubCosHalfOuterAngle;
	
	/// <summary>
	/// cos(OuterAngle / 2) 的值
	/// </summary>
	float CosHalfOuterAngle;
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

	/// <summary>
	/// 该 Actor 的方向就是聚光灯光源中心的朝向
	/// 
	/// Arrow 用于指示光源的朝向
	/// </summary>
	UPROPERTY(VisibleDefaultsOnly, Category = Extras)
		class UArrowComponent* Arrow;

};
