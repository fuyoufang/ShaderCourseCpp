#pragma once

#include "CoreMinimal.h"
#include "VertexOutput.generated.h"

USTRUCT()
struct FVertexOutput
{
	GENERATED_BODY()

public:
	FVertexOutput() {};

	FVector4 VertexPosCVV;

	/// <summary>
	/// 片源颜色
	/// </summary>
	FVector FragmentColor;
};