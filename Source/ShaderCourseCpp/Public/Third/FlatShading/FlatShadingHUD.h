#pragma once 

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Common/MyModel.h"
#include "Common/Shader/ShaderType.h"
#include "Common/Shader/IShader.h"
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


	TArray<UStaticMesh*> StaticMeshs;

	FVector Ka; 
	FVector Kd;
	FVector Ks;
	float Shiness;

private:

	/// <summary>
	/// 场景中所有的灯光
	/// </summary>
	TArray<class AMyLightSourceBase *> Lights;

	/// <summary>
	/// 重设场景中的所有灯光
	/// </summary>
	void ResetLights();

	/// <summary>
	/// 加载模型
	/// </summary>
	/// <param name="InStaticMesh"></param>
	/// <param name="InModelMatrix"></param>
	/// <param name="InKa"></param>
	/// <param name="InKd"></param>
	/// <param name="InKs"></param>
	/// <param name="InShiness"></param>
	void LoadModel(UStaticMesh* InStaticMesh, FTransform InModelMatrix, FVector InKa, FVector InKd, FVector InKs, float InShiness);

	UIShader* SpawnShader(EShaderType InShaderType);

	TArray<UMyModel*> MyModelArray;
};
