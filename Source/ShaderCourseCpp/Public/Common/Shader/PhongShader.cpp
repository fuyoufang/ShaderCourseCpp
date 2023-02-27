#include "Common/Shader/PhongShader.h"
#include "Third/PhongShading/PhongShadingHUD.h"
#include "Third/GouraudShading/GouraudShadingHUD.h"
#include "Third/PointLight/PointLightHUD.h"
#include "Common/MyModel.h"
#include "Common/Light/MyDirectionLight.h"

EShaderType UPhongShader::GetShaderType()
{
	return EShaderType::EGouraudShader;
}

FVertexOutput UPhongShader::VertexShader(UMyModel* InModel, int32 InFaceNum, int32 InVertexNum, AHUD* InHUD)
{
	//APhongShadingHUD* HUD = Cast<APhongShadingHUD>(InHUD);
	APointLightHUD* HUD = Cast<APointLightHUD>(InHUD);
	if (HUD == nullptr)
	{
		return FVertexOutput();
	}
	const auto& ViewMatrix = HUD->ViewMatrix;
	const auto& PerspectiveMatrix = HUD->PerspectiveMatrix;

	// 顶点索引
	int32 VertexIndex = InModel->Faces[InFaceNum].IndexArray[InVertexNum];
	// 顶点
	const auto& Vertex = InModel->Vertex[VertexIndex];
	
	auto Output = FVertexOutput();
	// 计算 CVV 空间坐标。
	FMatrix Matrix = InModel->ModelMatrix * HUD->ViewMatrix * HUD->PerspectiveMatrix;
	//----------
	// TODO 此处用错了，之前用的 Matrix.TransformVector
	// TransformPosition 针对坐标
	// TransformVector 针对向量
	//-----------
	Output.VertexPosCVV = Matrix.TransformPosition(Vertex);
	// 计算法线. GetTransposed 获得转置
	FVector N = InModel->ModelMatrix.GetTransposed().TransformVector(InModel->Normals[VertexIndex]);
	N.Normalize();
	Output.NormalWS = N;
	// 世界坐标
	Output.PosWS = InModel->ModelMatrix.TransformPosition(Vertex);
	
	return Output;
}

void UPhongShader::Init(FVector InKa, FVector InKd, FVector InKs, float InShiness, FVector InCameraPos, FVector InAmbientColor, TArray<AMyLightSourceBase*> InLightArray)
{
	Ka = InKa;
	Kd = InKd;
	Ks = InKs;
	Shiness = InShiness;
	CameralPos = InCameraPos;
	AmbientColor = InAmbientColor;
	LightArray = InLightArray;
}

void UPhongShader::CalcuteLightIntensity(const FVector& InL, const FVector& InN, const FVector& InV,
	float& OutDiffuse, float& OutSpeculat)
{
	OutDiffuse = FMath::Max(0, InL.Dot(InN));

	auto Temp = (InL + InV); 
	Temp.Normalize();
	OutSpeculat = FMath::Pow(FMath::Max(0, Temp.Dot(InN)), Shiness);
}

FVector UPhongShader::FragmentShader(FVector InFragmentColor, FVector InNormalWS, FVector InPosWS)
{
	//Output.FragmentColor = 
	FVector TempN = InNormalWS;
	TempN.Normalize();

	FVector TempV = (CameralPos - InPosWS);
	TempV.Normalize();

	return GetColor(TempN, TempV, InPosWS, LightArray);

	//return InFragmentColor;
}

FVector UPhongShader::GetColor(const FVector& InN, const FVector& InV, FVector InPosWS, TArray<AMyLightSourceBase*> Lights)
{
	FVector DiffuseColor;
	FVector SpecularColor;

	for (const auto& Light : Lights)
	{
		if (!Light->IsInRange(InPosWS))
		{
			continue;
		}
		FVector L = Light->GetLightDir();
		auto Factor = Light->GetDistanceAttenuationFactor();
		float Diffuse = 0, Speculat = 0;
		CalcuteLightIntensity(L, InN, InV, Diffuse, Speculat);

		auto FactorLightSourceColor = Light->LightSourceColor * Factor;
		DiffuseColor += FactorLightSourceColor * Diffuse;
		SpecularColor += FactorLightSourceColor * Speculat;
	}
	return Ka * AmbientColor + Kd * DiffuseColor + Ks * SpecularColor;
}