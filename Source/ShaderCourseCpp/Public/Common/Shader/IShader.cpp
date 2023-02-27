#include "Common/Shader/IShader.h"


FVertexOutput UIShader::VertexShader(UMyModel* Model, int32 InFaceNum, int32 InVertexNum, AHUD* InHUD)
{
    return FVertexOutput();
}

FVector UIShader::FragmentShader(FVector InFragmentColor, FVector InNormalWS, FVector InPosWS)
{
    return FVector();
}

EShaderType UIShader::GetShaderType()
{
    return EShaderType::None;
}
