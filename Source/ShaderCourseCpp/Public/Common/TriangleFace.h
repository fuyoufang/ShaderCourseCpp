#include "CoreMinimal.h"
#include "TriangleFace.generated.h"

USTRUCT(BlueprintType)
struct FTriangleFace
{

	GENERATED_BODY()
public:
	FTriangleFace() {}

	FTriangleFace(TArray<int32> InIndexArray): IndexArray(InIndexArray) {}

	TArray<int32> IndexArray;
};