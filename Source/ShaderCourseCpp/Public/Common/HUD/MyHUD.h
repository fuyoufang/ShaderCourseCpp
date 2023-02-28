#pragma once 

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Common/MyModel.h"
#include "Common/Shader/ShaderType.h"
#include "Common/Shader/IShader.h"
#include "Common/PerspectiveInfo.h"
#include "Common/PixelColor.h"
#include "Common/MyFrameBuffer.h"
#include "Common/VertexOutput.h"
#include "Common/ClipPlane.h"
#include "MyHUD.generated.h"

/// <summary>
/// HUD 的基类
/// </summary>
UCLASS()
class SHADERCOURSECPP_API AMyHUD : public AHUD
{
	GENERATED_BODY()
		//GENERATED_UCLASS_BODY()

public:
	AMyHUD();


	// 场景信息
public:
	/// <summary>
	/// the Camera Transform
	/// </summary>
	FTransform CameraTransform;

	/// <summary>
	/// 透视信息
	/// </summary>
	FPerspectiveInfo MyPerspectiveInfo;

	/// <summary>
	/// 相机矩阵
	/// </summary>
	FMatrix ViewMatrix;

	/// <summary>
	/// 透视矩阵
	/// </summary>
	FMatrix PerspectiveMatrix;

	/// <summary>
	/// CVV 空间的 w 的最小值
	/// </summary>
	float LittleNum = 0.001;

	// 屏幕信息
public:
	/// <summary>
	/// 屏幕像素的大小
	/// </summary>
	float GWidth = 1280;

	//float GHeight = 720;
	float GHeight = 640;

	/// <summary>
	/// 屏幕像素的坐标的最大值
	/// </summary>
	int32 GMaxWidthIndex;

	int32 GMaxHeightIndex;

	// 灯光信息
public:
	/// <summary>
	/// 场景中所有的灯光
	/// </summary>
	TArray<class AMyLightSourceBase*> Lights;

	/// <summary>
	/// 环境光颜色
	/// </summary>
	FVector AmbientColor = FVector(0, 0.2, 0.3);

	// 模型信息
public:
	UPROPERTY(EditAnywhere, Category = "Mesh")
		TArray<UStaticMesh*> StaticMeshs;

	UPROPERTY(EditAnywhere, Category = "Mesh")
		TArray<FTransform> StaticMeshTransforms;

	TArray<UMyModel*> MyModelArray;

	FVector Ka = FVector(0.3, 0.3, 0.3);
	FVector Kd = FVector(1, 1, 1);
	FVector Ks = FVector(1, 1, 1);

	float Shiness = 500;

	//TArray<FPixelColor> MyFrameBuffer;

	FMyFrameBuffer MyNewFrameBuffer;


	// shader
public:
	EShaderType ShaderType = EShaderType::EFlatShader;

};
