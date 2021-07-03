#include "PTSDInteractionSubsystem.h"

#include "Subsystem/SubsystemActorManager.h"
#include "FGCharacterPlayer.h"
#include "PTSDInteraction/PTSDInteractionModule.h"

APTSDInteractionSubsystem::APTSDInteractionSubsystem()
{
	Input = CreateDefaultSubobject<UInputComponent>("Input");
	Input->BindAction("PrimaryFire", IE_Pressed, this, &APTSDInteractionSubsystem::OnPrimaryFirePressed).bConsumeInput = false;
	Input->BindAction("PrimaryFire", IE_Released, this, &APTSDInteractionSubsystem::OnPrimaryFireReleased).bConsumeInput = false;
	Input->BindAction("SecondaryFire", IE_Pressed, this, &APTSDInteractionSubsystem::OnSecondaryFirePressed).bConsumeInput = false;
	Input->BindAction("SecondaryFire", IE_Released, this, &APTSDInteractionSubsystem::OnSecondaryFireReleased).bConsumeInput = false;

	SetActorTickEnabled(true);
	PrimaryActorTick.SetTickFunctionEnable(true);
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	SetReplicates(true);
	bAlwaysRelevant = true;
}

void APTSDInteractionSubsystem::BeginPlay()
{
	Super::BeginPlay();

	TArray<AActor*> FoundCharacters;
	UGameplayStatics::GetAllActorsOfClass(this, AFGCharacterPlayer::StaticClass(), FoundCharacters);
	for (AActor* Character : FoundCharacters) AttachWidgetInteractionToPlayer(Cast<AFGCharacterPlayer>(Character));
}

void APTSDInteractionSubsystem::Tick(float dt)
{
	Super::Tick(dt);
	
	this->GetWorld()->GetFirstPlayerController()->PushInputComponent(Input);
}

void APTSDInteractionSubsystem::OnPrimaryFirePressed()
{
	AController* Controller = GetWorld()->GetFirstPlayerController();
	if (Controller)
	{
		AFGCharacterPlayer* Character = Cast<AFGCharacterPlayer>(Controller->GetCharacter());
		if (Character)
		{
			UWidgetInteractionComponent** ComponentRef = WidgetInteractionComponents.Find(Character);

			if (ComponentRef && IsValid(*ComponentRef))
			{
				(*ComponentRef)->PressPointerKey(EKeys::LeftMouseButton);

				if (TestForKBFocus)
				{
					for (auto FocusWidget : KBFocusTestWidgets)
					{
						if (IsValid(FocusWidget))
						{
							if (FocusWidget->IsHovered())
							{
								FocusWidget->SetKeyboardFocus();
							}
						}
					}
				}
			}
		}
	}
}

void APTSDInteractionSubsystem::OnPrimaryFireReleased()
{
	AController* Controller = GetWorld()->GetFirstPlayerController();
	if (Controller)
	{
		AFGCharacterPlayer* Character = Cast<AFGCharacterPlayer>(Controller->GetCharacter());
		if (Character)
		{
			UWidgetInteractionComponent** ComponentRef = WidgetInteractionComponents.Find(Character);
			if (ComponentRef && IsValid(*ComponentRef)) (*ComponentRef)->ReleasePointerKey(EKeys::LeftMouseButton);
		}
	}
}

void APTSDInteractionSubsystem::OnSecondaryFirePressed()
{
	AController* Controller = GetWorld()->GetFirstPlayerController();
	if (Controller)
	{
		AFGCharacterPlayer* Character = Cast<AFGCharacterPlayer>(Controller->GetCharacter());
		if (Character)
		{
			UWidgetInteractionComponent** ComponentRef = WidgetInteractionComponents.Find(Character);
			if (ComponentRef && IsValid(*ComponentRef)) (*ComponentRef)->PressPointerKey(EKeys::RightMouseButton);
		}
	}
}

void APTSDInteractionSubsystem::OnSecondaryFireReleased()
{
	AController* Controller = GetWorld()->GetFirstPlayerController();
	if (Controller)
	{
		AFGCharacterPlayer* Character = Cast<AFGCharacterPlayer>(Controller->GetCharacter());
		if (Character)
		{
			UWidgetInteractionComponent** ComponentRef = WidgetInteractionComponents.Find(Character);
			if (ComponentRef && IsValid(*ComponentRef)) (*ComponentRef)->ReleasePointerKey(EKeys::RightMouseButton);
		}
	}
}

APTSDInteractionSubsystem* APTSDInteractionSubsystem::GetInteractionSubsystem(UObject* WorldContext)
{
#if WITH_EDITOR
	return nullptr;
#endif
	UWorld* WorldObject = GEngine->GetWorldFromContextObjectChecked(WorldContext);
	USubsystemActorManager* SubsystemActorManager = WorldObject->GetSubsystem<USubsystemActorManager>();
	check(SubsystemActorManager);
	return SubsystemActorManager->GetSubsystemActor<APTSDInteractionSubsystem>();
}

void APTSDInteractionSubsystem::AttachWidgetInteractionToPlayer(AFGCharacterPlayer* Character)
{
	if (!IsValid(Character) || !Character->GetController() || !Character->GetController()->IsLocalPlayerController())
		return;
	DetachWidgetInteractionToPlayer(Character);
	UE_LOG(LogPTSDInteraction, Verbose, TEXT("Attaching WidgetInteraction"));
	UWidgetInteractionComponent* Component = NewObject<UWidgetInteractionComponent>(Character);
	Component->InteractionSource = EWidgetInteractionSource::World;
	UCameraComponent* Cam = Cast<UCameraComponent>(Character->GetComponentByClass(UCameraComponent::StaticClass()));
	Component->InteractionDistance = 10000.0;
	Component->VirtualUserIndex = VirtualUserNum++;
	Component->RegisterComponent();
	Component->AttachToComponent(Cam, FAttachmentTransformRules::KeepRelativeTransform);
	WidgetInteractionComponents.Add(Character, Component);
}

void APTSDInteractionSubsystem::DetachWidgetInteractionToPlayer(AFGCharacterPlayer* Character)
{
	if (!IsValid(Character)) return;
	UWidgetInteractionComponent** ComponentRef = WidgetInteractionComponents.Find(Character);
	if (ComponentRef)
	{
		UE_LOG(LogPTSDInteraction, Verbose, TEXT("Detaching WidgetInteraction"));
		(*ComponentRef)->UnregisterComponent();
		WidgetInteractionComponents.Remove(Character);
	}
}

void APTSDInteractionSubsystem::EnableTestForKBFocus()
{
	TestForKBFocus = true;
}

void APTSDInteractionSubsystem::RegisterDesiresKBFocusWidget(UWidget* Widget)
{
	KBFocusTestWidgets.Add(Widget);
}

void APTSDInteractionSubsystem::UnregisterDesiresKBFocusWidget(UWidget* Widget)
{
	KBFocusTestWidgets.Remove(Widget);
}
