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
};