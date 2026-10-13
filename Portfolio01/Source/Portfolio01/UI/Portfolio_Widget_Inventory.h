// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TileView.h"
#include "Engine/DataTable.h"
#include "Portfolio_Widget_Inventory.generated.h"

/**
 * 
 */
UCLASS()
class PORTFOLIO01_API UPortfolio_Widget_Inventory : public UUserWidget
{
	GENERATED_BODY()

public:
	// 메모리적으로 엔진전
	static UPortfolio_Widget_Inventory* InvenWidget;

	UFUNCTION(BlueprintCallable)
		void AddInvenItem(UObject* _Data, UUserWidget* _Widget);

	UFUNCTION(BlueprintCallable)
		void NewWidget(UUserWidget* _Widget);


	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		int32 MyVariable=0; //캐릭터에서 해당값이용

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		int32 CheckBulletNum = 0;

	bool AddGameItem(const struct FItemData* Data);

	struct FPlayerData* CurPlayerData;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ammo")
	FDataTableRowHandle AmmoItemRow;

	UFUNCTION(BlueprintPure, Category = "Ammo")
	int32 GetAmmoCount() const;

	bool TryConsumeAmmo(int32 Amount);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ammo")
	FDataTableRowHandle PistolAmmoItemRow;

	int32 GetAmmoCountForRow(const FDataTableRowHandle& AmmoRow) const;

	bool TryConsumeAmmoForRow(
		const FDataTableRowHandle& AmmoRow,
		int32 Amount
	);

protected:
	void NativeConstruct() override;
	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

private:
	UPROPERTY(Category = "Effect", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class UTileView* InvenList = nullptr;
};
