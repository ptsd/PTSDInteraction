// ReSharper disable CppCStyleCast

#include "PTSDInteractionModule.h"

#include "FGCharacterPlayer.h"
#include "PTSDInteractionSubsystem.h"
#include "Patching/NativeHookManager.h"

DEFINE_LOG_CATEGORY(LogPTSDInteraction);

void FPTSDInteractionModule::StartupModule()
{
	UE_LOG(LogPTSDInteraction, Verbose, TEXT("Starting PTSDInteraction Module"));
#if !WITH_EDITOR
		SUBSCRIBE_METHOD_VIRTUAL_AFTER(AFGCharacterPlayer::BeginPlay, (void*)GetDefault<AFGCharacterPlayer>(), [](AActor* Self) {
			AFGCharacterPlayer* Character = Cast<AFGCharacterPlayer>(Self);
			if (Character) {
				APTSDInteractionSubsystem* SubSys = APTSDInteractionSubsystem::GetInteractionSubsystem(Self->GetWorld());
				if (SubSys) SubSys->AttachWidgetInteractionToPlayer(Character);
			}
		})

		SUBSCRIBE_METHOD_VIRTUAL_AFTER(AFGCharacterPlayer::EndPlay, (void*)GetDefault<AFGCharacterPlayer>(), [](AActor* Self, EEndPlayReason::Type Reason) {
			AFGCharacterPlayer* Character = Cast<AFGCharacterPlayer>(Self);
			if (Character) {
				APTSDInteractionSubsystem* SubSys = APTSDInteractionSubsystem::GetInteractionSubsystem(Self->GetWorld());
				if (SubSys) SubSys->DetachWidgetInteractionToPlayer(Character);
			}
		})
#endif
}

IMPLEMENT_GAME_MODULE(FPTSDInteractionModule, PTSDInteraction);
