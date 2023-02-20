#pragma once

#include "CoreMinimal.h"

/// <summary>
/// 需要裁剪的平面
/// </summary>
UENUM(BlueprintType)
enum class EClipPlane : uint8
{
	ClipWPlane,
	ClipXNearPlane,
	ClipXFarPlane,
	ClipYRightPlane,
	ClipYLeftPlane,
	ClipZTopPlane,
	ClipZBottomPlane
};