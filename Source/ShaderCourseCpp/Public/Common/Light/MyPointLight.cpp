#include "Common/Light/MyPointLight.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"

AMyPointLight::AMyPointLight()
{
	// 如果不设置的话， RootComponent 则为 nillptr;
	RootComponent = CreateDefaultSubobject<USceneComponent>("Root Scene Component");

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

void AMyPointLight::BeginPlay() 
{
	Super::BeginPlay();
	StartToEndLength = FalloffEnd - FalloffStart;
}