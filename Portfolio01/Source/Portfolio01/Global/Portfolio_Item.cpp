// Fill out your copyright notice in the Description page of Project Settings.


#include "Global/Portfolio_Item.h"
#include "Global/Portfolio_GameInstance.h"
#include "Kismet/GameplayStatics.h"
#include <UI/Portfolio_Widget_Inventory.h>
#include <UI/Portfolio_HUD.h>
#include "Sound/SoundBase.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/DataTable.h"

// Sets default values
APortfolio_Item::APortfolio_Item()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APortfolio_Item::BeginPlay()
{
	// 반드시 한 번만 호출
	Super::BeginPlay();

	Data = nullptr;

	if (!ItemRow.DataTable || ItemRow.RowName.IsNone())
	{
		UE_LOG(
			LogTemp, Warning,
			TEXT("%s: ItemRow is not assigned."),
			*GetName()
		);
		return;
	}

	Data = ItemRow.GetRow<FItemData>(
		TEXT("Item BeginPlay")
	);
}

// Called every frame
void APortfolio_Item::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APortfolio_Item::Take(AActor* _Actor)
{
	if (!Data)
	{
		return;
	}

	APlayerController* Con = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	APortfolio_HUD* HUD = Con->GetHUD<APortfolio_HUD>();
	if (nullptr == HUD)
	{
		// 인벤토리가 없을 것이다.
		return;
	}

	UPortfolio_Widget_Main* WidGet = HUD->GetMainWidget();
	if (nullptr == WidGet)
	{
		return;
	}

	UWidget* InvenWidGet = WidGet->GetWidgetFromName(TEXT("WBP_UI_Inventory"));
	UPortfolio_Widget_Inventory* Inventory = Cast<UPortfolio_Widget_Inventory>(InvenWidGet);

	if (nullptr == Inventory)
	{
		return;
	}

	int32 AddedCount = 0;

	for (int32 i = 0; i < PickupAmount; ++i)
	{
		if (!Inventory->AddGameItem(Data))
		{
			break;
		}

		++AddedCount;
	}

	// 실제로 하나 이상 획득했을 때만 한 번 재생
	if (AddedCount > 0 && IsValid(Data->PickupSound))
	{
		UGameplayStatics::PlaySound2D(
			this,
			Data->PickupSound
		);
	}

	// Inventory->AddInvenItem()

	int a = 0;

}