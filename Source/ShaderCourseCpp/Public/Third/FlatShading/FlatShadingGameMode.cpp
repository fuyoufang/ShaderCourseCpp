#include "FlatShadingGameMode.h"
#include "Third/FlatShading/FlatShadingHUD.h"
//#include "FlatShadingHUD.h"

AFlatShadingGameMode::AFlatShadingGameMode()
{
	HUDClass = AFlatShadingHUD::StaticClass();

}