#pragma once

#include "CoreMinimal.h"
#include "PixelColor.generated.h"

USTRUCT()
struct FPixelColor 
{
	GENERATED_BODY()
public:

	FPixelColor() { }

	/// <summary>
	/// 坐标
	/// </summary>
	int32 X;
	int32 Y;

	/// <summary>
	/// 颜色
	/// </summary>
	FLinearColor Color;
};
