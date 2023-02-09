
#include "FlatShadingHUD.h"
#include "EngineUtils.h"
#include "Common/Light/MyLightSourceBase.h"
#include "Kismet/GameplayStatics.h"
//#include "Engine/World.h"


AFlatShadingHUD::AFlatShadingHUD()
{

}

void AFlatShadingHUD::BeginPlay()
{
	Super::BeginPlay();

	
	ResetLights();
	
}

void AFlatShadingHUD::ResetLights()
{
	Lights.Empty();
	// 方法一
	/*
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMyLightSourceBase::StaticClass(), FoundActors);
	*/

	// 方法二
	for (TActorIterator<AMyLightSourceBase> ItActor(GetWorld()); ItActor; ++ItActor)
	{
		Lights.Add(*ItActor);
	}
}

