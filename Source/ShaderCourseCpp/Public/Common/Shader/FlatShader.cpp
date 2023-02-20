#include "Common/Shader/FlatShader.h"
#include "Third/FlatShading/FlatShadingHUD.h"
#include "Common/MyModel.h"
#include "Common/Light/MyDirectionLight.h"

EShaderType UFlatShader::GetShaderType()
{
	return EShaderType::EFlatShader;
}

FVertexOutput UFlatShader::VertexShader(UMyModel* InModel, int32 InFaceNum, int32 InVertexNum, AHUD* InHUD)
{
	AFlatShadingHUD* HUD = Cast<AFlatShadingHUD>(InHUD);
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
	
	// 缓存模型顶点对应的世界坐标
	WorldPosArray.Add(InVertexNum, InModel->ModelMatrix.TransformVector(Vertex));
	
	if (InVertexNum == 2)
	{
		// 设置灯光信息
		Shiness = InModel->Shiness;
		auto Temp1 = WorldPosArray[1] - WorldPosArray[0];
		auto Temp2 = WorldPosArray[2] - WorldPosArray[0];
		N = Temp2.Cross(Temp1);
		N.Normalize();
		V = (HUD->CameraTransform.GetLocation() - WorldPosArray[2]);
		V.Normalize();
		
		SetColor(HUD->Lights);
	}

	// 计算 CVV 空间坐标。
	FMatrix Matrix = InModel->ModelMatrix* HUD->ViewMatrix* HUD->PerspectiveMatrix;
	auto Output = FVertexOutput();
	//----------
	// TODO 此处用错了，之前用的 Matrix.TransformVector
	//-----------
	Output.VertexPosCVV = Matrix.TransformPosition(Vertex);

	//if (GEngine) {
	//	// 在屏幕上打印 debug 信息
	//	auto T = Output.VertexPosCVV;
	//	GEngine->AddOnScreenDebugMessage(-1, 9.0f, FColor::Red, FString::Printf(TEXT("CVV：%f, %f, %f, %f"), T.X, T.Y, T.Z, T.W));
	//	GEngine->AddOnScreenDebugMessage(-1, 9.0f, FColor::Red, FString::Printf(TEXT("Vertex：%f, %f, %f"), Vertex.X, Vertex.Y, Vertex.Z));
	//}

	return Output;
}

void UFlatShader::SetColor(TArray<AMyLightSourceBase*> Lights)
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
	}
}

void UFlatShader::CalcuteLightIntensity(const FVector& InL, const FVector& InN, const FVector& InV,
	float& OutDiffuse, float& OutSpeculat)
{
	OutDiffuse = FMath::Max(0, InL.Dot(InN));

	auto Temp = (InL + InV); 
	Temp.Normalize();
	OutSpeculat = FMath::Pow(FMath::Max(0, Temp.Dot(InN)), Shiness);
}


FVector UFlatShader::FragmentShader(FVector InKa, FVector InKd, FVector InKs, FVector AmbientColor)
{
	return InKa.Cross(AmbientColor) + InKd.Cross(DiffuseColor) + InKs.Cross(SpecularColor);
}
