#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EShaderType : uint8
{
	None,
	EWireframeShader,
	ESimpleShader,
	EFlatShader,
	EGouraudShader
};