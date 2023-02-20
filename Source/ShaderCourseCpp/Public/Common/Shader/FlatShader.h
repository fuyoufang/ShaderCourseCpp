#pragma once 

#include "CoreMinimal.h"
#include "Common/Shader/IShader.h"
#include "Common/MyModel.h"
#include "Common/Light/MyLightSourceBase.h"
#include "FlatShader.generated.h"

UCLASS()
class UFlatShader: public UIShader
{
	GENERATED_BODY()

public:
//	UFlatShader();

	EShaderType GetShaderType() override;

	/// <summary>
	/// 
	/// </summary>
	/// <param name="Model"></param>
	/// <param name="InFaceNum"></param>
	/// <param name="InVertexNum"></param>
	/// <param name="InHUD"></param>
	/// <returns></returns>
	virtual FVertexOutput VertexShader(UMyModel* Model, int32 InFaceNum, int32 InVertexNum, AHUD* InHUD) override;

	virtual FVector FragmentShader() override;


	/// <summary>
	/// 初始化各个参数
	/// </summary>
	/// <param name="InKa"></param>
	/// <param name="InKd"></param>
	/// <param name="InKs"></param>
	/// <param name="InShiness"></param>
	/// <param name="InCameraPos"></param>
	/// <param name="InAmbientColor"></param>
	void Init(FVector InKa, FVector InKd, FVector InKs, float InShiness, FVector InCameraPos, FVector InAmbientColor);

private:
	TMap<int32, FVector> WorldPosArray;

	float Shiness;

	/// <summary>
	/// 灯光方向
	/// </summary>
	FVector L;

	/// <summary>
	/// 三角面的法线
	/// </summary>
	FVector N;

	FVector V;

	void SetColor(TArray<AMyLightSourceBase*> Lights);

	FVector DiffuseColor;

	FVector SpecularColor;

	FVector Ka;

	FVector Kd;

	FVector Ks;

	FVector AmbientColor;

	FVector CameralPos;
	
	FVector ResultColor;
	/// <summary>
	/// 
	/// </summary>
	/// <param name="L"></param>
	/// <param name="N"></param>
	/// <param name="V"></param>
	/// <param name="OutDiffuse"></param>
	/// <param name="OutSpeculat">高光</param>
	void CalcuteLightIntensity(const FVector& InL, const FVector& InN, const FVector& InV,
		float& OutDiffuse, float& OutSpeculat);
};