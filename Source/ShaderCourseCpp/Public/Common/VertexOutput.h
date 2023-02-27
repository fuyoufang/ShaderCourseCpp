#pragma once

#include "CoreMinimal.h"
#include "VertexOutput.generated.h"

USTRUCT()
struct FVertexOutput
{
	GENERATED_BODY()

public:
	FVertexOutput() {};

	/// <summary>
	/// CVV 空间坐标
	/// </summary>
	FVector4 VertexPosCVV;

	/// <summary>
	/// 片源颜色
	/// </summary>
	FVector FragmentColor;

	/// <summary>
	/// 世界空间的法线
	/// WS: World Space
	/// </summary>
	FVector NormalWS;

	/// <summary>
	/// 世界空间的坐标
	/// WS: World Space
	/// </summary>
	FVector PosWS;
};