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
#include "Third/PhongShading/PhongShadingHUD.h"
#include "Common/HUD/MyHUD.h"
#include "PointLightHUD.generated.h"

/*
* 1. GENERATED_BODY 和 GENERATED_UCLASS_BODY 分别在什么时候用？
*/

UCLASS()
class SHADERCOURSECPP_API APointLightHUD : public AMyHUD
{
	GENERATED_BODY()
		//GENERATED_UCLASS_BODY()

public:
	APointLightHUD();

	void BeginPlay() override;

	void DrawHUD() override;

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

	/// <summary>EFlatShader
	/// 获取屏幕像素索引对应的深度值
	/// </summary>
	float GetPixelDepth(int32 InPixelIndex);
};