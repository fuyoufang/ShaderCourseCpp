#include "Common/Light/MySpotLight.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Components/ArrowComponent.h"
#include "Kismet/KismetMathLibrary.h"

AMySpotLight::AMySpotLight()
{
	// 如果不设置的话， RootComponent 则为 nillptr;
	RootComponent = CreateDefaultSubobject<USceneComponent>("Root Scene Component");

	Arrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	Arrow->SetupAttachment(RootComponent);

	Start = CreateDefaultSubobject<USphereComponent>(TEXT("Start"));
	// bUpdateOverlaps 设置为 false，因为此处不需要碰撞
	Start->SetSphereRadius(FalloffStart, false);
	Start->SetupAttachment(RootComponent);
	Start->ShapeColor = FColor(255, 0, 0, 255);

	End = CreateDefaultSubobject<USphereComponent>(TEXT("End"));
	End->SetSphereRadius(FalloffEnd, false);
	End->SetupAttachment(RootComponent);

	LightSourceColor = FVector(0, 0, 1);
}

void AMySpotLight::BeginPlay() 
{
	Super::BeginPlay();
	StartToEndLength = FalloffEnd - FalloffStart;
	
	CosHalfOuterAngle = UKismetMathLibrary::DegCos(OuterAngle / 2);
	CosHalfInnerAngleSubCosHalfOuterAngle = UKismetMathLibrary::DegCos(InnerAngle / 2) - CosHalfOuterAngle;

}

float AMySpotLight::GetDistanceAttenuationFactor() {
	// 计算距离上的衰减
	auto TempDistanceFactor = FMath::Clamp((FalloffEnd - LSize) / StartToEndLength, 0, 1);

	// 计算方向上的衰减
	// 转换为被照射的点到光源的方向
	FVector TempL = L * FVector(-1, -1, -1);

	
	float TempX = TempL.Dot(Arrow->GetForwardVector());
	auto TempForwardFactor = (TempX - CosHalfOuterAngle) / CosHalfInnerAngleSubCosHalfOuterAngle;
	
	TempForwardFactor = FMath::Clamp(TempForwardFactor, 0, 1);

	return TempDistanceFactor * TempForwardFactor;
}
