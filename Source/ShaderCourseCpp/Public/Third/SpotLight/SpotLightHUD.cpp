
#include "SpotLightHUD.h"

#include "EngineUtils.h"
#include "Common/Light/MyLightSourceBase.h"
#include "Kismet/GameplayStatics.h"
#include "Common/Shader/FlatShader.h"
#include "Common/Shader/GouraudShader.h"
#include "Common/Shader/PhongShader.h"
#include "KismetProceduralMeshLibrary.h"
#include <Kismet/KismetMathLibrary.h>

ASpotLightHUD::ASpotLightHUD()
{
	CameraTransform = FTransform(FRotator(), FVector());
	MyPerspectiveInfo = FPerspectiveInfo(90, 2, 10, 1250);
	ShaderType = EShaderType::EPhongShader;

}

void ASpotLightHUD::BeginPlay()
{
	Super::BeginPlay();

	ResetLights();
	// TTransform(const TRotator<T>& InRotation, const TVector<T>& InTranslation, const TVector<T>& InScale3D = TVector<T>(1.f, 1.f, 1.f))

	int32 StaticMeshIndex = 0;
	for (const auto& StaticMesh : StaticMeshs)
	{
		FTransform ModelMatrix;
		if (StaticMeshIndex < StaticMeshTransforms.Num())
		{
			ModelMatrix = StaticMeshTransforms[StaticMeshIndex];
		}
		else
		{
			FRotator Rotation;
			FVector Scale = FVector(1, 1, 1);
			ModelMatrix = FTransform(Rotation, FVector(600, 500, 0));
		}

		LoadModel(StaticMesh, ModelMatrix, Ka, Kd, Ks, Shiness);
		StaticMeshIndex += 1;
	}

	SetGMaxIndex();
	SetViewMatrix();
	SetPerspectiveMatrix();
	UpdateBuffer();
}

void ASpotLightHUD::DrawHUD()
{
	Super::DrawHUD();
	for (const auto& PixelColor : MyNewFrameBuffer.ColorBuffer)
	{
		DrawRect(PixelColor.Value.Color, PixelColor.Value.X, PixelColor.Value.Y, 1, 1);
	}
}

void ASpotLightHUD::ResetLights()
{
	Lights.Empty();
	// 方法一
	/*
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMyLightSourceBase::StaticClass(), FoundActors);
	*/

	// 方法二
	for (TActorIterator<AMyLightSourceBase> ItActor(GetWorld()); ItActor; ++ItActor)
	{
		Lights.Add(*ItActor);
	}
}

void ASpotLightHUD::LoadModel(UStaticMesh* InLoadMesh, FTransform InModelMatrix, FVector InKa, FVector InKd, FVector InKs, float InShiness)
{
	if (!IsValid(InLoadMesh))
	{
		return;
	}

	TArray<FVector> Vertices;
	// 三角形的顶点数组
	TArray<int32> TempTriangles;
	TArray<FVector> Normals;
	TArray<FVector2D> UVs;
	TArray<FProcMeshTangent> Tangents;

	// 使用时，需要：
	// 1. 添加头文件：#include "KismetProceduralMeshLibrary.h"
	// 2. 添加依赖 "ProceduralMeshComponent"
	UKismetProceduralMeshLibrary::GetSectionFromStaticMesh(InLoadMesh, 0, 0, Vertices, TempTriangles, Normals, UVs, Tangents);

	// 设置 model
	UMyModel* Model = NewObject<UMyModel>();
	auto ModelIndex = MyModelArray.Add(Model);
	Model->ModelMatrix = InModelMatrix.ToMatrixWithScale();
	Model->Vertex = Vertices;
	Model->Ka = InKa;
	Model->Kd = InKd;
	Model->Ks = InKs;
	Model->Shiness = InShiness;
	Model->Normals = Normals;

	// 将三角面的顶点转换为数组，添加到 faces 中
	int32 TrianglesCount = TempTriangles.Num() / 3;
	for (int32 i = 0; i < TrianglesCount; i++)
	{
		int32 FirstIndex = i * 3;
		TArray<int32> IndexArray;
		IndexArray.Add(TempTriangles[FirstIndex]);
		IndexArray.Add(TempTriangles[FirstIndex + 1]);
		IndexArray.Add(TempTriangles[FirstIndex + 2]);
		Model->Faces.Add(FTriangleFace(IndexArray));
	}

	Model->ModelShader = SpawnShader(ShaderType, Model);
}

UIShader* ASpotLightHUD::SpawnShader(EShaderType InShaderType, UMyModel* Model)
{
	UIShader* Shader = nullptr;
	switch (InShaderType)
	{
	case EShaderType::None:
		break;
	case EShaderType::EWireframeShader:
		break;
	case EShaderType::ESimpleShader:
		break;
	case EShaderType::EFlatShader:
	{
		UFlatShader* FlatShader = NewObject<UFlatShader>();
		FlatShader->Init(Model->Ka, Model->Kd, Model->Ks, Model->Shiness, CameraTransform.GetLocation(), AmbientColor);
		Shader = FlatShader;
	}
	break;
	case EShaderType::EGouraudShader:
	{
		UGouraudShader* GouraudShader = NewObject<UGouraudShader>();
		GouraudShader->Init(Model->Ka, Model->Kd, Model->Ks, Model->Shiness, CameraTransform.GetLocation(), AmbientColor);
		Shader = GouraudShader;
	}
	break;
	case EShaderType::EPhongShader:
	{
		UPhongShader* PhongShader = NewObject<UPhongShader>();
		PhongShader->Init(Model->Ka, Model->Kd, Model->Ks, Model->Shiness, CameraTransform.GetLocation(), AmbientColor, Lights);
		Shader = PhongShader;
	}
	break;
	default:
		break;
	}
	return Shader;
}

void ASpotLightHUD::SetGMaxIndex()
{
	GMaxWidthIndex = GWidth - 1;

	GMaxHeightIndex = GHeight - 1;
}

void ASpotLightHUD::SetViewMatrix()
{
	ViewMatrix = CameraTransform.Inverse().ToMatrixWithScale();
}

void ASpotLightHUD::SetPerspectiveMatrix()
{
	// 根据 MVP = ModelMatrix * ViewMatrix * PserspectiveMatrix ，可以计算出 MVP。

	float Top = 0, Bottom = 0, Left = 0, Right = 0;

	Top = UKismetMathLibrary::DegTan(MyPerspectiveInfo.FOV / 2) * MyPerspectiveInfo.N;
	Bottom = -Top;
	Right = MyPerspectiveInfo.Radio * Top;
	Left = -Right;

	FPlane Plan0 = FPlane(MyPerspectiveInfo.F / (MyPerspectiveInfo.F - MyPerspectiveInfo.N), 0, 0, 1);
	FPlane Plan1 = FPlane(0, MyPerspectiveInfo.N * 2 / (Right - Left), 0, 0);
	FPlane Plan2 = FPlane(0, 0, 2 * MyPerspectiveInfo.N / (Top - Bottom), 0);

	FPlane Plan3 = FPlane(-MyPerspectiveInfo.F * MyPerspectiveInfo.N / (MyPerspectiveInfo.F - MyPerspectiveInfo.N), 0, 0, 0);

	PerspectiveMatrix = FMatrix(Plan0, Plan1, Plan2, Plan3);
}

void ASpotLightHUD::UpdateBuffer()
{
	BufferReset();

	for (int32 i = 0; i < MyModelArray.Num(); i++)
	{
		auto Model = MyModelArray[i];

		for (int32 FaceNumber = 0; FaceNumber < Model->Faces.Num(); FaceNumber++)
		{
			DrawTriangle(Model, FaceNumber);
		}
	}
}

void ASpotLightHUD::BufferReset()
{
	//MyFrameBuffer.Reset();
	MyNewFrameBuffer.ZBuffer.Reset();
	MyNewFrameBuffer.ColorBuffer.Reset();
}

void ASpotLightHUD::DrawTriangle(UMyModel* InModel, int32 InFaceNumber)
{
	TArray<FVertexOutput> OutVertexArray;
	auto Shader = InModel->ModelShader;
	for (int32 i = 0; i < 3; i++)
	{
		FVertexOutput Output = Shader->VertexShader(InModel, InFaceNumber, i, this);
		OutVertexArray.Add(Output);
	}
	ClipPlanes(OutVertexArray);

	TArray<FVertexOutput> TempTriangle;
	// NDC 的坐标
	TArray<FVector> VertexPosArrayNDC;
	// 屏幕坐标
	TArray<FVector> ScreenPosArray;

	// 经过裁剪后，所有的顶点可以组成的三角形的个数
	int32 Num = OutVertexArray.Num() - 2;
	
	for (int32 i = 0; i < Num; i++)
	{
		int32 Index0 = 0;
		auto Index1 = i + 1;
		auto Index2 = Index1 + 1;
		// 图元装配
		TempTriangle.Add(OutVertexArray[Index0]);
		TempTriangle.Add(OutVertexArray[Index1]);
		TempTriangle.Add(OutVertexArray[Index2]);

		//透视除法
		for (const auto& VertexOutput : TempTriangle)
		{
			auto Vertex4PosNDC = VertexOutput.VertexPosCVV / FVector4(VertexOutput.VertexPosCVV.W, VertexOutput.VertexPosCVV.W, VertexOutput.VertexPosCVV.W, VertexOutput.VertexPosCVV.W);
			auto VertexPosNDC = FVector(Vertex4PosNDC.X, Vertex4PosNDC.Y, Vertex4PosNDC.Z);
			VertexPosArrayNDC.Add(VertexPosNDC);

			// 屏幕映射
			FVector ScreenPos;
			ScreenPos.X = (VertexPosNDC.Y + 1) * GWidth * 0.5;
			ScreenPos.Y = (1 - VertexPosNDC.Z) * GHeight * 0.5;
			ScreenPos.Z = VertexOutput.VertexPosCVV.W;
			ScreenPosArray.Add(ScreenPos);
		}

		// 背面剔除
		if (!IsBackface(VertexPosArrayNDC))
		{
			int32 XMin, YMin, XMax, YMax;
			TriangleSetup(ScreenPosArray, XMin, YMin, XMax, YMax);

			// 三角形遍历
			for (int32 X = XMin; X <= XMax; X++)
			{
				for (int32 Y = YMin; Y <= YMax; Y++)
				{
					float InX = X + 0.5;
					float InY = Y + 0.5;
					float Alpha, Beta, Gamma;
					ComputeBarycentric(InX, InY, ScreenPosArray, Alpha, Beta, Gamma);
					if (!IsInsideTriangle(Alpha, Beta, Gamma))
					{
						continue;
					}

					auto AlphaDivideX1 = Alpha / TempTriangle[0].VertexPosCVV.W;
					auto BetaDivideX2 = Beta / TempTriangle[1].VertexPosCVV.W;
					auto GammaDivideX3 = Gamma / TempTriangle[2].VertexPosCVV.W;
					auto Xt = 1 / (AlphaDivideX1 + BetaDivideX2 + GammaDivideX3);

					FVector PerspectiveCorrect = FVector(AlphaDivideX1 * Xt, BetaDivideX2 * Xt, GammaDivideX3 * Xt);
					// 计算插值 TempZ
					auto PixelIndex = GetPixelIndex(X, Y);
					auto ZValue = GetPixelDepth(PixelIndex);
					auto TempZ = Xt * (Alpha + Beta + Gamma);
					if (TempZ >= ZValue)
					{
						continue;
					}

					// 计算插值 颜色
					auto Color = (AlphaDivideX1 * TempTriangle[0].FragmentColor
						+ BetaDivideX2 * TempTriangle[1].FragmentColor
						+ GammaDivideX3 * TempTriangle[2].FragmentColor)
						* Xt;

					auto NormalWS = (AlphaDivideX1 * TempTriangle[0].NormalWS
						+ BetaDivideX2 * TempTriangle[1].NormalWS
						+ GammaDivideX3 * TempTriangle[2].NormalWS)
						* Xt;

					auto PosWS = (AlphaDivideX1 * TempTriangle[0].PosWS
						+ BetaDivideX2 * TempTriangle[1].PosWS
						+ GammaDivideX3 * TempTriangle[2].PosWS)
						* Xt;

					MyNewFrameBuffer.ZBuffer.Add(PixelIndex, TempZ);
					FVector Res = Shader->FragmentShader(Color, NormalWS, PosWS);
					FPixelColor PixelColor;
					PixelColor.X = X;
					PixelColor.Y = Y;
					PixelColor.Color = FLinearColor(Res);
					MyNewFrameBuffer.ColorBuffer.Add(PixelIndex, PixelColor);
				}
			}
		}
		TempTriangle.Reset();
		VertexPosArrayNDC.Reset();
		ScreenPosArray.Reset();
	}
}

void ASpotLightHUD::ClipPlanes(TArray<FVertexOutput>& InVertexArray)
{
	ClipWithPlan(EClipPlane::ClipWPlane, InVertexArray);
	ClipWithPlan(EClipPlane::ClipXNearPlane, InVertexArray);
	ClipWithPlan(EClipPlane::ClipXFarPlane, InVertexArray);
	ClipWithPlan(EClipPlane::ClipYRightPlane, InVertexArray);
	ClipWithPlan(EClipPlane::ClipYLeftPlane, InVertexArray);
	ClipWithPlan(EClipPlane::ClipZTopPlane, InVertexArray);
	ClipWithPlan(EClipPlane::ClipZBottomPlane, InVertexArray);
}

void ASpotLightHUD::ClipWithPlan(EClipPlane InClipPlane, TArray<FVertexOutput>& InVertexArray)
{
	TArray<FVertexOutput> TempArray;

	for (int32 i = 0; i < InVertexArray.Num(); i++)
	{
		// 当前点的 index
		int32 CurrentIndex = i;
		// 先前点的 index
		int32 PreIndex = (CurrentIndex - 1 + InVertexArray.Num()) % InVertexArray.Num();
		// 当前和先前点
		FVertexOutput PreVertex = InVertexArray[PreIndex];
		FVertexOutput CurrentVertex = InVertexArray[CurrentIndex];

		// 当前点、先前点是否在平面内
		bool IsCurInside = IsInsidePlane(InClipPlane, CurrentVertex.VertexPosCVV);
		bool IsPreInside = IsInsidePlane(InClipPlane, PreVertex.VertexPosCVV);

		// 判断先前点和当前点是否同时在平面内，如果不是则会产生交点
		if (IsCurInside != IsPreInside)
		{
			// 计算交点，并添加到 TempArray 中
			float Alpha = GetIntersecRatio(PreVertex.VertexPosCVV, CurrentVertex.VertexPosCVV, InClipPlane);
			auto VertexPosCVV = Vector4Lerp(PreVertex.VertexPosCVV, CurrentVertex.VertexPosCVV, Alpha);
			auto VertexOutput = FVertexOutput();
			VertexOutput.VertexPosCVV = VertexPosCVV;
			VertexOutput.FragmentColor = FMath::Lerp(PreVertex.FragmentColor, CurrentVertex.FragmentColor, Alpha);
			VertexOutput.NormalWS = FMath::Lerp(PreVertex.NormalWS, CurrentVertex.NormalWS, Alpha);
			VertexOutput.PosWS = FMath::Lerp(PreVertex.PosWS, CurrentVertex.PosWS, Alpha);

			TempArray.Add(VertexOutput);
		}
		// 如果当前点在平面内，则添加到 tempArray 中
		if (IsCurInside)
		{
			TempArray.Add(CurrentVertex);
		}
	}
	InVertexArray.Reset();
	InVertexArray.Append(TempArray);
}

bool ASpotLightHUD::IsInsidePlane(EClipPlane InClipPlane, FVector4 InVertexPosCVV)
{
	switch (InClipPlane)
	{
	case EClipPlane::ClipWPlane:
		return InVertexPosCVV.W >= LittleNum;
	case EClipPlane::ClipXNearPlane:
		return InVertexPosCVV.X >= 0;
	case EClipPlane::ClipXFarPlane:
		return InVertexPosCVV.X <= InVertexPosCVV.W;
	case EClipPlane::ClipYRightPlane:
		return InVertexPosCVV.Y <= InVertexPosCVV.W;
	case EClipPlane::ClipYLeftPlane:
		return InVertexPosCVV.Y >= -InVertexPosCVV.W;
	case EClipPlane::ClipZTopPlane:
		return InVertexPosCVV.Z <= InVertexPosCVV.W;
	case EClipPlane::ClipZBottomPlane:
		return InVertexPosCVV.Z >= -InVertexPosCVV.W;
	default:
		return false;
	}
}

float ASpotLightHUD::GetIntersecRatio(FVector4 InPreVertex, FVector4 InCurVertex, EClipPlane InClipPlane)
{
	switch (InClipPlane)
	{
	case EClipPlane::ClipWPlane:
		// Because:
		// LittleNum = Pre.w + t * (Cur.w - Pre.w)
		// So:
		// t = (Pre.w - LittleNum) / (Pre.w - Cur.w)
		return (InPreVertex.W - LittleNum) / (InPreVertex.W - InCurVertex.W);
	case EClipPlane::ClipXNearPlane:
		// Because:
		// x = x_near
		// x_near = 0
		// x = Pre.x + t * (Cur.x - Pre.x)
		// So:
		// t = Pre.x / (Pre.x - Cur.x)
		return InPreVertex.X / (InPreVertex.X - InCurVertex.X);
	case EClipPlane::ClipXFarPlane:
		return (InPreVertex.W - InPreVertex.X) / ((InPreVertex.W - InCurVertex.W) - (InPreVertex.X - InCurVertex.X));
	case EClipPlane::ClipYRightPlane:
		return (InPreVertex.W - InPreVertex.Y) / ((InPreVertex.W - InCurVertex.W) - (InPreVertex.Y - InCurVertex.Y));
	case EClipPlane::ClipYLeftPlane:
		return (InPreVertex.W + InPreVertex.Y) / ((InPreVertex.W - InCurVertex.W) - (InPreVertex.Y - InCurVertex.Y));
	case EClipPlane::ClipZTopPlane:
		return (InPreVertex.W - InPreVertex.Z) / ((InPreVertex.W - InCurVertex.W) - (InPreVertex.Z - InCurVertex.Z));
	case EClipPlane::ClipZBottomPlane:
		return (InPreVertex.W + InPreVertex.Z) / ((InPreVertex.W - InCurVertex.W) + (InPreVertex.Z - InCurVertex.Z));
	default:
		return 0.0f;
	}

}

FVector4 ASpotLightHUD::Vector4Lerp(FVector4 InPreVertex, FVector4 InCurVertex, float InAlpha)
{
	return InPreVertex + (InCurVertex - InPreVertex) * InAlpha;
}

bool ASpotLightHUD::IsBackface(TArray<FVector> InVertexPosNDC)
{
	// 1. 计算三角面的法向量
	// 2. 法向量和相机方向的点乘在 [-1, 0] 之间时为正面
	// 
	// 此处假设相机的方向为 (1, 0, 0)

	float Result = (InVertexPosNDC[0].Y * InVertexPosNDC[2].Z) - (InVertexPosNDC[0].Z * InVertexPosNDC[2].Y)
		+ (InVertexPosNDC[0].Z * InVertexPosNDC[1].Y) - (InVertexPosNDC[0].Y * InVertexPosNDC[1].Z)
		+ (InVertexPosNDC[1].Z * InVertexPosNDC[2].Y) - (InVertexPosNDC[1].Y * InVertexPosNDC[2].Z);
	return Result >= 0;
}

void ASpotLightHUD::TriangleSetup(const TArray<FVector>& InScreenPosArray, int32& OutXMin, int32& OutYMin, int32& OutXMax, int32& OutYMax)
{

	TArray<float> Xs;
	TArray<float> Ys;
	for (const auto& Pos : InScreenPosArray)
	{
		Xs.Add(Pos.X);
		Ys.Add(Pos.Y);
	}

	OutXMin = FMath::TruncToInt32(FMath::Max(0, FMath::Min(Xs)));
	OutXMax = FMath::Clamp(FMath::TruncToInt32(FMath::Max(Xs)), 0, GMaxWidthIndex);
	OutYMin = FMath::TruncToInt32(FMath::Max(0, FMath::Min(Ys)));
	OutYMax = FMath::Clamp(FMath::TruncToInt32(FMath::Max(Ys)), 0, GMaxHeightIndex);
}

void ASpotLightHUD::ComputeBarycentric(float InX, float InY, const TArray<FVector>& InScreenPosArray, float& OutAlpha, float& OutBeta, float& OutGamma)
{
	auto X1 = InScreenPosArray[0].X;
	auto Y1 = InScreenPosArray[0].Y;
	auto X2 = InScreenPosArray[1].X;
	auto Y2 = InScreenPosArray[1].Y;
	auto X3 = InScreenPosArray[2].X;
	auto Y3 = InScreenPosArray[2].Y;
	auto Y2SubY3 = Y2 - Y3;
	auto XSubX3 = InX - X3;
	auto YSubY3 = InY - Y3;
	auto X2SubX3 = X2 - X3;
	auto X1SubX3 = X1 - X3;
	auto Y1SubY3 = Y1 - Y3;
	auto A = (Y2SubY3 * X1SubX3) - (Y1SubY3 * X2SubX3);
	OutAlpha = ((Y2SubY3 * XSubX3) - (YSubY3 * X2SubX3)) / A;
	OutBeta = ((YSubY3 * X1SubX3) - (Y1SubY3 * XSubX3)) / A;
	OutGamma = 1 - OutAlpha - OutBeta;
}

bool ASpotLightHUD::IsInsideTriangle(float InAlpha, float InBeta, float InGamma)
{
	// float 类型的数据和 0 进行比较时，可能会产生错误的判断，所以需要和一个较小的数字进行比较
	float Temp = -0.001;
	return InAlpha > Temp && InBeta > Temp && InGamma > Temp;
}

int32 ASpotLightHUD::GetPixelIndex(int32 InPosX, int32 InPoxY)
{
	return InPoxY * FMath::TruncToInt32(GWidth) + InPosX;
}

float ASpotLightHUD::GetPixelDepth(int32 InPixelIndex)
{
	float* Result = MyNewFrameBuffer.ZBuffer.Find(InPixelIndex);
	if (Result == nullptr)
	{
		return 9999999.0;
	}
	else
	{
		return *Result;
	}
}