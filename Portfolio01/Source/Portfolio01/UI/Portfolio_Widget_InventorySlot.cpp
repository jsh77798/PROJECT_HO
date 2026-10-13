// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Portfolio_Widget_InventorySlot.h"

void UPortfolio_Widget_InventorySlot::NativeConstruct()
{
	Super::NativeConstruct();

	ItemIconBack = Cast<UImage>(GetWidgetFromName(TEXT("ItemBack")));
	ItemIconImage = Cast<UImage>(GetWidgetFromName(TEXT("ItemIcon")));
	ItemCountBlock = Cast<UTextBlock>(GetWidgetFromName(TEXT("ItemCount")));

	// 인벤토리 아이템에 있는 UInvenItemData는 인벤토리라면 무조건 가지고 있어야 합니다.
	ItemData = NewObject<UInventoryItemData>();
}

void UPortfolio_Widget_InventorySlot::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

}

void UPortfolio_Widget_InventorySlot::SetItemData(UInventoryItemData* _ItemData)
{
	ItemData = _ItemData;
	SlotDataCheck();
}

void UPortfolio_Widget_InventorySlot::SlotDataCheck()
{
	if (nullptr == ItemData)
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> if (nullptr == ItemData)"), __FUNCTION__, __LINE__);
		// 치명적인 에러
		//ItemBackVisibility = ESlateVisibility::Visible;
		//ItemIconVisibility = ESlateVisibility::Hidden;
		//ItemCountVisibility = ESlateVisibility::Hidden;
		return;
	}

	if (nullptr == ItemData->Data)
	{
		ItemBackVisibility = ESlateVisibility::Visible;
		ItemIconVisibility = ESlateVisibility::Hidden;
		ItemCountVisibility = ESlateVisibility::Hidden;
		return;
	}

	if (nullptr != ItemData->Data)
	{
		ItemIconVisibility = ESlateVisibility::Visible;
		ItemIconImage->SetBrushFromTexture(Cast<UTexture2D>(ItemData->Data->Icon));
		ItemCountValue = ItemData->Count;

		if (1 < ItemData->Data->StackMax)
		{
			ItemCountVisibility = ESlateVisibility::Visible;
		}
		else
		{
			ItemCountVisibility = ESlateVisibility::Hidden;
		}
	}

	if (nullptr == ItemData->Data)
	{
		ItemCountValue = 0;

		ItemBackVisibility = ESlateVisibility::Visible;
		ItemIconVisibility = ESlateVisibility::Hidden;
		ItemCountVisibility = ESlateVisibility::Hidden;
		return;
	}
}

void UPortfolio_Widget_InventorySlot::DragSetting(UPortfolio_Widget_InventorySlot* _OtherDragSlot)
{
	ItemIconBack = Cast<UImage>(GetWidgetFromName(TEXT("ItemBack")));
	ItemIconImage = Cast<UImage>(GetWidgetFromName(TEXT("ItemIcon")));
	ItemCountBlock = Cast<UTextBlock>(GetWidgetFromName(TEXT("ItemCount")));


	// 이걸호출되는 당사자는
	// 드래그를 위해서 만들어진 아이콘입니다.
	ItemBackVisibility = ESlateVisibility::Hidden;

	// _OtherDragSlot은 드래그의 시작이된 슬롯이다.
	SetItemData(_OtherDragSlot->ItemData);

}

void UPortfolio_Widget_InventorySlot::MoveSetting(UPortfolio_Widget_InventorySlot* _OtherDragSlot)
{
	//if (_OtherDragSlot == this)
	//{
	//	return;
	//}
	//
	//if (ItemTypeValue != ItemType::NONE && _OtherDragSlot->ItemData->Data->Type != ItemTypeValue)
	//{
	//	return;
	//}
	//
	//UInventoryItemData* SwapItemData = _OtherDragSlot->ItemData;
	//_OtherDragSlot->ItemData = ItemData;
	//ItemData = SwapItemData;
	//
	//if (true == ItemChangeFunction.IsBound())
	//{
	//	ItemChangeFunction.Broadcast();
	//}
	//
	//SlotDataCheck();
	//_OtherDragSlot->SlotDataCheck();

	if (!IsValid(_OtherDragSlot) ||
		_OtherDragSlot == this ||
		!IsValid(ItemData) ||
		!IsValid(_OtherDragSlot->ItemData))
	{
		return;
	}

	// 목적지 슬롯의 아이템 종류 제한
	if (ItemTypeValue != ItemType::NONE &&
		_OtherDragSlot->ItemData->Data &&
		_OtherDragSlot->ItemData->Data->Type != ItemTypeValue)
	{
		return;
	}

	// 교환되어 돌아갈 아이템도 원래 슬롯에 들어갈 수 있어야 함
	if (_OtherDragSlot->ItemTypeValue != ItemType::NONE &&
		ItemData->Data &&
		ItemData->Data->Type != _OtherDragSlot->ItemTypeValue)
	{
		return;
	}

	Swap(ItemData->Data, _OtherDragSlot->ItemData->Data);
	Swap(ItemData->Count, _OtherDragSlot->ItemData->Count);

	SlotDataCheck();
	_OtherDragSlot->SlotDataCheck();

	ItemChangeFunction.Broadcast();
	_OtherDragSlot->ItemChangeFunction.Broadcast();
}


