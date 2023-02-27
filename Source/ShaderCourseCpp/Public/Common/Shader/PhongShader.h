#pragma once 

#include "CoreMinimal.h"
#include "Common/Shader/IShader.h"
#include "Common/MyModel.h"
#include "Common/Light/MyLightSourceBase.h"
#include "PhongShader.generated.h"

UCLASS()
class UPhongShader : public UIShader
{
	GENERATED_BODY()

public:
//	UFlatShader();

	EShaderType GetShaderType() override;

	/// <summary>
	/// 顶点着色器
	/// </summary>
	/// <param name="Model"></param>
	/// <param name="InFaceNum"></param>
	/// <param name="InVertexNum"></param>
	/// <param name="InHUD"></param>
	/// <returns></returns>
	virtual FVertexOutput VertexShader(UMyModel* Model, int32 InFaceNum, int32 InVertexNum, AHUD* InHUD) override;

	/// 片源着色器
	virtual FVector FragmentShader(FVector InFragmentColor, FVector InNormalWS, FVector InPosWS) override;


	/// <summary>
	/// 初始化各个参数
	/// </summary>
	/// <param name="InKa"></param>
	/// <param name="InKd"></param>
	/// <param name="InKs"></param>
	/// <param name="InShiness"></param>
	/// <param name="InCameraPos"></param>
	/// <param name="InAmbientColor"></param>
	void Init(FVector InKa, FVector InKd, FVector InKs, float InShiness, FVector InCameraPos, FVector InAmbientColor, TArray<AMyLightSourceBase*> InLightArray);

private:
	
	float Shiness;

	FVector GetColor(const FVector& InN, const FVector& InV, TArray<AMyLightSourceBase*> Lights);


	FVector Ka;

	FVector Kd;

	FVector Ks;

	FVector AmbientColor;

	FVector CameralPos;

	/// <summary>
	/// 场景中的灯光
	/// </summary>
	TArray<AMyLightSourceBase *> LightArray;

	/// <summary>
	/// 
	/// </summary>
	/// <param name="L"></param>
	/// <param name="N">法线</param>
	/// <param name="V"></param>
	/// <param name="OutDiffuse"></param>
	/// <param name="OutSpeculat">高光</param>
	void CalcuteLightIntensity(const FVector& InL, const FVector& InN, const FVector& InV,
		float& OutDiffuse, float& OutSpeculat);
};