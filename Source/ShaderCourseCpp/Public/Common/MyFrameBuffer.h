#pragma once

#include "CoreMinimal.h"
#include "Common/PixelColor.h"

USTRUCT()
struct FMyFrameBuffer
{
	GENERATED_BODY()

public:
	TMap<int32, float> ZBuffer;

	TMap<int32, FPixelColor> ColorBuffer;
};