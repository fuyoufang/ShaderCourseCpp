#pragma once 

#include "CoreMinimal.h"
#include "PerspectiveInfo.generated.h"

USTRUCT()
struct FPerspectiveInfo
{
	GENERATED_BODY()

public:
	FPerspectiveInfo() {}

	FPerspectiveInfo(float InFOV, float InRadio, float InN, float InF)
		: FOV(InFOV)
		,Radio(InRadio)
		,N(InN)
		,F(InF){ }

	/// <summary>
	/// 视域角
	/// </summary>
	float FOV;

	/// <summary>
	/// 宽高比
	/// </summary>
	float Radio;

	/// <summary>
	/// 近裁面
	/// </summary>
	float N;

	/// <summary>
	/// 远裁面
	/// </summary>
	float F;
};