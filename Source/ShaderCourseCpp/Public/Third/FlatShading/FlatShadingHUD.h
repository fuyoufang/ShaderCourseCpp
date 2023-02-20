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
#include "FlatShadingHUD.generated.h"

/*
* 1. GENERATED_BODY 和 GENERATED_UCLASS_BODY 分别在什么时候用？
*/

UCLASS()
class SHADERCOURSECPP_API AFlatShadingHUD : public AHUD
{
	GENERATED_BODY()
	//GENERATED_UCLASS_BODY()

public:
	AFlatShadingHUD();
	
	void BeginPlay() override;

	void DrawHUD() override;
	

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

private:

	/// <summary>
	/// 重设场景中的所有灯光
	/// </summary>
	void ResetLights();

	/// <summary>
	/// 加载模型
	/// </summary>
	/// <param name="InStaticMesh"></param>
	/// <param name="InModelMatrix"></param>
	/// <param name="InKa"></param>
	/// <param name="InKd"></param>
	/// <param name="InKs"></param>
	/// <param name="InShiness"></param>
	void LoadModel(UStaticMesh* InStaticMesh, FTransform InModelMatrix, FVector InKa, FVector InKd, FVector InKs, float InShiness);

	UIShader* SpawnShader(EShaderType InShaderType, UMyModel* Model);

	void SetGMaxIndex();

	void SetViewMatrix();

	void SetPerspectiveMatrix();

	void UpdateBuffer();

	void BufferReset();

	void DrawTriangle(UMyModel* InModel, int32 InFaceNumber);

	/// <summary>
	/// 平面裁剪
	/// </summary>
	void ClipPlanes(TArray<FVertexOutput>& InVertexArray);

	void ClipWithPlan(EClipPlane InClipPlane, TArray<FVertexOutput>& InVertexArray);

	bool IsInsidePlane(EClipPlane InClipPlane, FVector4 VertexPosCVV);

	/// <summary>
	/// 获取两个点在指定平面上的交点的 Ratio
	/// </summary>
	float GetIntersecRatio(FVector4 InPreVertex, FVector4 InCurVertex, EClipPlane InClipPlane);

	FVector4 Vector4Lerp(FVector4 InPreVertex, FVector4 InCurVertex, float InAlpha);

	/// <summary>
	/// 是否为背面
	/// </summary>
	/// <param name="InVertexPosNDC"></param>
	/// <returns></returns>
	bool IsBackface(TArray<FVector> InVertexPosNDC);

	void TriangleSetup(const TArray<FVector>& InScreenPosArray, int32& OutXMin, int32& OutYMin, int32& OutXMax, int32& OutYMax);

	/// <summary>
	/// 计算重心坐标
	/// </summary>
	void ComputeBarycentric(float InX, float InY, const TArray<FVector>& InScreenPosArray, float& OutAlpha, float& OutBeta, float& OutGamma);

	/// <summary>
	/// 判断是否在三角形内
	/// </summary>
	bool IsInsideTriangle(float InAlpha, float InBeta, float InGamma);

	/// <summary>
	/// 根据屏幕坐标，计算像素索引
	/// </summary>
	/// <param name="InPosX"></param>
	/// <param name="InPoxY"></param>
	/// <returns></returns>
	int32 GetPixelIndex(int32 InPosX, int32 InPoxY);

	/// <summary>
	/// 获取屏幕像素索引对应的深度值
	/// </summary>
	float GetPixelDepth(int32 InPixelIndex);
};
