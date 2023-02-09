
#include "FlatShadingHUD.h"
#include "EngineUtils.h"
#include "Common/Light/MyLightSourceBase.h"
#include "Kismet/GameplayStatics.h"
#include "Common/Shader/FlatShader.h"
#include "KismetProceduralMeshLibrary.h"

AFlatShadingHUD::AFlatShadingHUD()
{

}

void AFlatShadingHUD::BeginPlay()
{
	Super::BeginPlay();

	
	ResetLights();
	// TTransform(const TRotator<T>& InRotation, const TVector<T>& InTranslation, const TVector<T>& InScale3D = TVector<T>(1.f, 1.f, 1.f))
	FVector Translation;
	FRotator Rotation;
	FVector Scale;
	
	FTransform ModelMatrix = FTransform(Rotation, Translation, Scale);
		

	for (const auto &StaticMesh : StaticMeshs)
	{
		LoadModel(StaticMesh, ModelMatrix, Ka, Kd, Ks, Shiness);
	}
}

void AFlatShadingHUD::ResetLights()
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

void AFlatShadingHUD::LoadModel(UStaticMesh* InLoadMesh, FTransform InModelMatrix, FVector InKa, FVector InKd, FVector InKs, float InShiness)
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
	Model->Verts = Vertices;
	Model->Ka = InKa;
	Model->Kd = InKd;
	Model->Ks = InKs;
	Model->Shiness = InShiness;

	// 将三角面的顶点转换为数组，添加到 faces 中
	int32 TrianglesCount = TempTriangles.Num() / 3;
	for (int32 i = 0; i < TrianglesCount; i++)
	{
		int32 FirstIndex = i * 3;
		TArray<int32> IndexArray;
		IndexArray.Add(FirstIndex);
		IndexArray.Add(FirstIndex + 1);
		IndexArray.Add(FirstIndex + 2);
		Model->Faces.Add(FTriangleFace(IndexArray));
	}
}

UIShader* AFlatShadingHUD::SpawnShader(EShaderType InShaderType)
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
		//Shader = NewObject<UFlatShader>();
		break;
	default:
		break;
	}
	return Shader;
}

