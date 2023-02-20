#pragma once

#include "CoreMinimal.h"
#include "Common/TriangleFace.h"
#include "Common/Shader/IShader.h"
#include "MyModel.generated.h"

UCLASS()
class SHADERCOURSECPP_API UMyModel: public UObject {

	GENERATED_BODY()

public:
	UMyModel();

	TArray<FVector> Vertex;
	
	TArray<FTriangleFace> Faces;

	/// <summary>
	/// 顶点法线
	/// </summary>
	TArray<FVector> Normals;

	FMatrix ModelMatrix;

	UIShader* ModelShader;

	FVector Ka;

	FVector Kd;

	FVector Ks;

	float Shiness;
};