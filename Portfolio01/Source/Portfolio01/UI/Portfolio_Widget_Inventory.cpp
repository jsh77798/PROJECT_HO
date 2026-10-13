// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Portfolio_Widget_Inventory.h"
#include "Components/TileView.h"
#include <Global/Portfolio_GameInstance.h>
#include "UI/Portfolio_Widget_InventorySlot.h"
#include "UI/InventoryItemData.h"


// 굉장히 위험도가 높다.
UPortfolio_Widget_Inventory* UPortfolio_Widget_Inventory::InvenWidget = nullptr;

// GameMode아니면 HUD가 알게하는게 가장 좋습니다.

int32 UPortfolio_Widget_Inventory::GetAmmoCount() const
{
	//if (!IsValid(InvenList) ||
	//	!AmmoItemRow.DataTable ||
	//	AmmoItemRow.RowName.IsNone())
	//{
	//	return 0;
	//}
	//
	//const FItemData* AmmoData =
	//	AmmoItemRow.GetRow<FItemData>(TEXT("GetAmmoCount"));
	//
	//if (!AmmoData)
	//{
	//	return 0;
	//}
	//
	//int32 Total = 0;
	//
	//for (UObject* Object : InvenList->GetListItems())
	//{
	//	const UInventoryItemData* Item =
	//		Cast<UInventoryItemData>(Object);
	//
	//	if (IsValid(Item) && Item->Data == AmmoData)
	//	{
	//		Total += FMath::Max(0, Item->Count);
	//	}
	//}
	//
	//return Total;

	return GetAmmoCountForRow(AmmoItemRow);
}

bool UPortfolio_Widget_Inventory::TryConsumeAmmo(int32 Amount)
{
	//if (Amount <= 0 || GetAmmoCount() < Amount)
	//{
	//	return false;
	//}
	//
	//const FItemData* AmmoData =
	//	AmmoItemRow.GetRow<FItemData>(TEXT("TryConsumeAmmo"));
	//
	//int32 Remaining = Amount;
	//
	//for (UObject* Object : InvenList->GetListItems())
	//{
	//	UInventoryItemData* Item =
	//		Cast<UInventoryItemData>(Object);
	//
	//	if (!IsValid(Item) ||
	//		Item->Data != AmmoData ||
	//		Item->Count <= 0)
	//	{
	//		continue;
	//	}
	//
	//	const int32 ConsumeCount =
	//		FMath::Min(Item->Count, Remaining);
	//
	//	Item->Count -= ConsumeCount;
	//	Remaining -= ConsumeCount;
	//
	//	if (Item->Count == 0)
	//	{
	//		// 슬롯은 유지하고 내용만 비움
	//		Item->Data = nullptr;
	//	}
	//
	//	if (Remaining == 0)
	//	{
	//		break;
	//	}
	//}
	//
	//CheckBulletNum = GetAmmoCount();
	//
	//// 현재 화면에 표시된 슬롯 갱신
	//for (UUserWidget* Entry : InvenList->GetDisplayedEntryWidgets())
	//{
	//	auto* InventorySlotWidget =
	//		Cast<UPortfolio_Widget_InventorySlot>(Entry);
	//
	//	if (IsValid(InventorySlotWidget))
	//	{
	//		InventorySlotWidget->SlotDataCheck();
	//	}
	//}
	//
	//return true;

	return TryConsumeAmmoForRow(AmmoItemRow, Amount);
}

int32 UPortfolio_Widget_Inventory::GetAmmoCountForRow(const FDataTableRowHandle& AmmoRow) const
{
	if (!IsValid(InvenList) ||
		!AmmoRow.DataTable ||
		AmmoRow.RowName.IsNone())
	{
		return 0;
	}

	const FItemData* AmmoData =
		AmmoRow.GetRow<FItemData>(TEXT("GetAmmoCountForRow"));

	if (!AmmoData)
	{
		return 0;
	}

	int32 Total = 0;

	for (UObject* Object : InvenList->GetListItems())
	{
		const UInventoryItemData* Item =
			Cast<UInventoryItemData>(Object);

		if (IsValid(Item) && Item->Data == AmmoData)
		{
			Total += FMath::Max(0, Item->Count);
		}
	}

	return Total;
}

bool UPortfolio_Widget_Inventory::TryConsumeAmmoForRow(const FDataTableRowHandle& AmmoRow, int32 Amount)
{
	if (Amount <= 0 || GetAmmoCountForRow(AmmoRow) < Amount)
	{
		return false;
	}

	const FItemData* AmmoData =
		AmmoRow.GetRow<FItemData>(TEXT("TryConsumeAmmoForRow"));

	int32 Remaining = Amount;

	for (UObject* Object : InvenList->GetListItems())
	{
		UInventoryItemData* Item =
			Cast<UInventoryItemData>(Object);

		if (!IsValid(Item) ||
			Item->Data != AmmoData ||
			Item->Count <= 0)
		{
			continue;
		}

		const int32 ConsumeCount =
			FMath::Min(Item->Count, Remaining);

		Item->Count -= ConsumeCount;
		Remaining -= ConsumeCount;

		if (Item->Count == 0)
		{
			Item->Data = nullptr;
		}

		if (Remaining == 0)
		{
			break;
		}
	}

	CheckBulletNum = GetAmmoCount();

	for (UUserWidget* Entry : InvenList->GetDisplayedEntryWidgets())
	{
		auto* InventorySlotWidget =
			Cast<UPortfolio_Widget_InventorySlot>(Entry);

		if (IsValid(InventorySlotWidget))
		{
			InventorySlotWidget->SlotDataCheck();
		}
	}

	return true;
}

// 데이터와 랜더링은 분리되어야 한다.
void UPortfolio_Widget_Inventory::NativeConstruct()
{
	Super::NativeConstruct();

	InvenWidget = this;

	InvenList = Cast<UTileView>(GetWidgetFromName(TEXT("InvenTileView")));


	//for (size_t i = 0; i < 4; i++)
	//{
	//	UInventoryItemData* ItemObject = NewObject<UInventoryItemData>();
	//	ItemObject->Data = nullptr;
	//	InvenList->AddItem(ItemObject);
	//}

	if (IsValid(InvenList) && InvenList->GetNumItems() == 0)
	{
		// 빈 슬롯 4개 생성
		for (int32 i = 0; i < 4; ++i)
		{
			UInventoryItemData* ItemObject =
				NewObject<UInventoryItemData>(this);

			ItemObject->Data = nullptr;
			ItemObject->Count = 0;

			InvenList->AddItem(ItemObject);
		}

		// 지정한 탄약 아이템 5개 지급
		if (AmmoItemRow.DataTable && !AmmoItemRow.RowName.IsNone())
		{
			const FItemData* AmmoData =
				AmmoItemRow.GetRow<FItemData>(TEXT("StartingAmmo"));

			if (AmmoData)
			{
				for (int32 i = 0; i < 5; ++i)
				{
					AddGameItem(AmmoData);
				}
			}
		}

		if (PistolAmmoItemRow.DataTable &&
			!PistolAmmoItemRow.RowName.IsNone())
		{
			const FItemData* PistolAmmoData =
				PistolAmmoItemRow.GetRow<FItemData>(TEXT("StartingPistolAmmo"));

			if (PistolAmmoData)
			{
				for (int32 i = 0; i < 12; ++i)
				{
					AddGameItem(PistolAmmoData);
				}
			}
		}

		CheckBulletNum = GetAmmoCount();
	}

	UPortfolio_GameInstance* GameInstance = GetWorld()->GetGameInstance<UPortfolio_GameInstance>();

	/*
	const TArray<UObject*>& Items = InvenList->GetListItems();
	for (size_t i = 0; i < 2; i++)
	{
		UInventoryItemData* DataObject = Cast<UInventoryItemData>(Items[i]);
		DataObject->Data = GameInstance->GetRandomItemData();
	}
	*/
	MyVariable = 6;
}

void UPortfolio_Widget_Inventory::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	const TArray<UObject*>& Items = InvenList->GetListItems();
	for (size_t i = 0; i < Items.Num(); i++)
	{
		UInventoryItemData* DataObject = Cast<UInventoryItemData>(Items[i]);

		if (nullptr == DataObject->Widget)
		{
			// 세팅될때까지 기다리면 된다.
			continue;
		}

		UPortfolio_Widget_InventorySlot* SlotWidget = DataObject->Widget;
		SlotWidget->SlotDataCheck();
	}

	//CheckBulletNum = InvenList->GetNumItems(); 
	//CheckBulletNum += 1;
	//
	//for (size_t i = 0; i < 2; i++)
	//{
	//	CheckBulletNum = InvenList->GetNumItems();
	//}

	CheckBulletNum = GetAmmoCount();
}

void UPortfolio_Widget_Inventory::AddInvenItem(UObject* _Data, UUserWidget* _Widget)
{
	UPortfolio_Widget_InventorySlot* ItemSlotWidget = Cast<UPortfolio_Widget_InventorySlot>(_Widget);

	UInventoryItemData* InvenSlotData = Cast<UInventoryItemData>(_Data);

	if (nullptr == ItemSlotWidget)
	{
		return;
	}

	InvenSlotData->Widget = ItemSlotWidget;
	ItemSlotWidget->SetItemData(InvenSlotData);

	int a = 0;
}

bool UPortfolio_Widget_Inventory::AddGameItem(const FItemData* Data)
{
	if (!Data || !IsValid(InvenList))
	{
		return false;
	}

	const TArray<UObject*>& Items = InvenList->GetListItems();
	const int32 StackLimit = FMath::Max(1, Data->StackMax);

	// 같은 아이템이 있는 슬롯부터 채움
	for (UObject* Object : Items)
	{
		UInventoryItemData* Item =
			Cast<UInventoryItemData>(Object);

		if (!IsValid(Item))
		{
			continue;
		}

		if (Item->Data == Data && Item->Count < StackLimit)
		{
			++Item->Count;
			++MyVariable;
			return true;
		}
	}

	// 합칠 수 없다면 빈 슬롯 사용
	for (UObject* Object : Items)
	{
		UInventoryItemData* Item =
			Cast<UInventoryItemData>(Object);

		if (!IsValid(Item))
		{
			continue;
		}

		if (Item->Data == nullptr)
		{
			Item->Data = Data;
			Item->Count = 1;
			return true;
		}
	}

	// 들어갈 공간이 없음
	return false;
}


void UPortfolio_Widget_Inventory::NewWidget(UUserWidget* _Widget)
{
	int a = 0;
}