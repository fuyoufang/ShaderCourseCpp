#include "Common/Light/MyDirectionLight.h"
#include "Components/StaticMeshComponent.h"
#include "Components/ArrowComponent.h"

AMyDirectionLight::AMyDirectionLight()
{
	RootComponent = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	LightSourceColor = FVector(0, 0, 1);
}