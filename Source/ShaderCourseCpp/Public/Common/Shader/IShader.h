#pragma once

#include "CoreMinimal.h"
#include "Common/VertexOutput.h"
#include "Common/Shader/ShaderType.h"
// #include "Common/MyModel.h"
#include "IShader.generated.h"

class UMyModel;

UCLASS()
class UIShader: public UObject
{
	GENERATED_BODY()

public:
	UIShader() {};

	virtual FVertexOutput VertexShader(UMyModel* Model, int32 InFaceNum, int32 InVertexNum, AHUD* InHUD);

	/// <summary>
	/// 环境光 + 高光 + 慢反射光
	/// 
	/// 可以对计算的颜色进行修改、映射
	/// </summary>
	/// <param name="InFragmentColor"></param>
	/// <param name="InNormalWS">世界空间的法线</param>
	/// <param name="InPosWS">世界空间的坐标</param>
	/// <returns></returns>
	virtual FVector FragmentShader(FVector InFragmentColor, FVector InNormalWS = FVector::Zero(), FVector InPosWS = FVector::Zero());

	virtual EShaderType GetShaderType();
};