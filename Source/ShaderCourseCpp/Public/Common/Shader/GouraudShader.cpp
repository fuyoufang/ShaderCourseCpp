#include "Common/Shader/GouraudShader.h"
#include "Third/GouraudShading/GouraudShadingHUD.h"
#include "Common/MyModel.h"
#include "Common/Light/MyDirectionLight.h"

EShaderType UGouraudShader::GetShaderType()
{
	return EShaderType::EGouraudShader;
}

FVertexOutput UGouraudShader::VertexShader(UMyModel* InModel, int32 InFaceNum, int32 InVertexNum, AHUD* InHUD)
{
	AGouraudShadingHUD* HUD = Cast<AGouraudShadingHUD>(InHUD);
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
	
	WorldPos = InModel->ModelMatrix.TransformVector(Vertex);
	
	// 计算法线
	// GetTransposed 获得转置
	N = InModel->ModelMatrix.GetTransposed().TransformVector(InModel->Normals[VertexIndex]);
	N.Normalize();

	
	V = (CameralPos - WorldPos);
	V.Normalize();
		
	SetColor(HUD->Lights);
	

	// 计算 CVV 空间坐标。
	FMatrix Matrix = InModel->ModelMatrix* HUD->ViewMatrix* HUD->PerspectiveMatrix;
	auto Output = FVertexOutput();
	Output.FragmentColor = ResultColor;
	//----------
	// TODO 此处用错了，之前用的 Matrix.TransformVector
	// TransformPosition 针对坐标
	// TransformVector 针对向量
	//-----------
	Output.VertexPosCVV = Matrix.TransformPosition(Vertex);
	if (GEngine) {

		//GEngine->AddOnScreenDebugMessage(-1, 9.0f, FColor::Red, FString::Printf(TEXT("ResultColor：%f：%f：%f"), ResultColor.X, ResultColor.Y, ResultColor.Z));
		//GEngine->AddOnScreenDebugMessage(-1, 9.0f, FColor::Red, FString::Printf(TEXT("ResultColor：%f：%f：%f"), InModel->Normals[VertexIndex].X, InModel->Normals[VertexIndex].Y, InModel->Normals[VertexIndex].Z));
	}
	
	return Output;
}

void UGouraudShader::SetColor(TArray<AMyLightSourceBase*> Lights)
{
	// 首先清空数据
	DiffuseColor = FVector(0, 0, 0);
	SpecularColor = FVector(0, 0, 0);

	for (const auto& Light : Lights)
	{
		if (!Light->IsInRange())
		{
			continue;
		}
		L = Light->GetLightDir();
		auto Factor = Light->GetDistanceAttenuationFactor();
		float Diffuse = 0, Speculat = 0;
		CalcuteLightIntensity(L, N, V, Diffuse, Speculat);

		auto FactorLightSourceColor = Light->LightSourceColor * Factor;
		DiffuseColor += FactorLightSourceColor * Diffuse;
		SpecularColor += FactorLightSourceColor * Speculat;

		if (Diffuse < 0 || Speculat < 0)
		{
			GEngine->AddOnScreenDebugMessage(-1, 9.0f, FColor::Red, FString::Printf(TEXT("less zero")));

		}
	}
	
	ResultColor = Ka.Cross(AmbientColor) + Kd.Cross(DiffuseColor) + Ks.Cross(SpecularColor); 
}

void UGouraudShader::Init(FVector InKa, FVector InKd, FVector InKs, float InShiness, FVector InCameraPos, FVector InAmbientColor)
{
	Ka = InKa;
	Kd = InKd;
	Ks = InKs;
	Shiness = InShiness;
	CameralPos = InCameraPos;
	AmbientColor = InAmbientColor;
}

void UGouraudShader::CalcuteLightIntensity(const FVector& InL, const FVector& InN, const FVector& InV,
	float& OutDiffuse, float& OutSpeculat)
{
	OutDiffuse = FMath::Max(0, InL.Dot(InN));

	auto Temp = (InL + InV); 
	Temp.Normalize();
	OutSpeculat = FMath::Pow(FMath::Max(0, Temp.Dot(InN)), Shiness);

	/*if (GEngine) {

		GEngine->AddOnScreenDebugMessage(-1, 9.0f, FColor::Red, FString::Printf(TEXT("OutDiffuse：%f"), OutDiffuse));
		GEngine->AddOnScreenDebugMessage(-1, 9.0f, FColor::Red, FString::Printf(TEXT("OutSpeculat：%f"), OutSpeculat));
	}*/

}

FVector UGouraudShader::FragmentShader(FVector InFragmentShader)
{
	return ResultColor;
}
