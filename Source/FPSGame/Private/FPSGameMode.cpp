// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "FPSGameMode.h"
#include "FPSHUD.h"
#include "FPSCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "FPSGameState.h"
AFPSGameMode::AFPSGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/Blueprints/BP_Player"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = AFPSHUD::StaticClass();

	GameStateClass = AFPSGameState::StaticClass();
}

void AFPSGameMode::CompleteMission(APawn* InstigatorPawn,bool MissionSuccess)
{
	if (InstigatorPawn)
	{

		if (SpectatingViewpointClass)
		{
			TArray<AActor*> ReturnedActors;
			UGameplayStatics::GetAllActorsOfClass(this, SpectatingViewpointClass, ReturnedActors);
			if (ReturnedActors.Num() > 0)
			{
				for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; It++)
				{
					APlayerController* PC = It->Get();
					if (PC)
					{
						//blend 人物 和 预定摄像机 视角
						PC->SetViewTargetWithBlend(ReturnedActors[0], 0.5f, EViewTargetBlendFunction::VTBlend_Cubic);
					}
				}
			}
		}
		else
		{
			UE_LOG(LogTemp,Warning,TEXT("SpectatingViewpointClass is nullptr!"))
		}

		AFPSGameState* GS = GetGameState<AFPSGameState>();
		if (GS)
		{
			GS->MulticastOnMissionComplete(InstigatorPawn, MissionSuccess);
		}
	}
	OnMissionComplete(InstigatorPawn, MissionSuccess);


}
