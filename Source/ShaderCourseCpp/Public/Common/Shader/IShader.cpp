#include "Common/Shader/IShader.h"


FVertexOutput UIShader::VertexShader(UMyModel* Model, int32 InFaceNum, int32 InVertexNum, AHUD* InHUD)
{
    return FVertexOutput();
}

FVector UIShader::FragmentShader(FVector InKa, FVector InKd, FVector InKs, FVector AmbientColor)
{
    return FVector();
}

EShaderType UIShader::GetShaderType()
{
    return EShaderType::None;
}
