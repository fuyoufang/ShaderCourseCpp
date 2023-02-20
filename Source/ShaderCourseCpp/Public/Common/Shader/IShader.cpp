#include "Common/Shader/IShader.h"


FVertexOutput UIShader::VertexShader(UMyModel* Model, int32 InFaceNum, int32 InVertexNum, AHUD* InHUD)
{
    return FVertexOutput();
}

FVector UIShader::FragmentShader(FVector InFragmentShader)
{
    return FVector();
}

EShaderType UIShader::GetShaderType()
{
    return EShaderType::None;
}
