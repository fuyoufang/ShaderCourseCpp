#pragma once 

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "FlatShadingHUD.generated.h"

/*
* 1. GENERATED_BODY 和 GENERATED_UCLASS_BODY 分别在什么时候用？
*/

UCLASS()
class SHADERCOURSECPP_API AFlatShadingHUD : public AHUD
{
	GENERATED_BODY()
	//GENERATED_UCLASS_BODY()

public:
	AFlatShadingHUD();
	
	void BeginPlay() override;

private:

	/// <summary>
	/// 场景中所有的灯光
	/// </summary>
	TArray<class AMyLightSourceBase *> Lights;

	/// <summary>
	/// 重设场景中的所有灯光
	/// </summary>
	void ResetLights();
};
