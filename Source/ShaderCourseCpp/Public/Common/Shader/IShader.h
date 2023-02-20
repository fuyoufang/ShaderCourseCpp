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

	virtual FVector FragmentShader(FVector InKa, FVector InKd, FVector InKs, FVector AmbientColor);

	virtual EShaderType GetShaderType();
};