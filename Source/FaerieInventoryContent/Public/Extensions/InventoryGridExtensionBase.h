﻿// Copyright Guy (Drakynfly) Lundvall. All Rights Reserved.

#pragma once

#include "FaerieGridEnums.h"
#include "FaerieGridStructs.h"
#include "ItemContainerExtensionBase.h"
#include "InventoryGridExtensionBase.generated.h"

using FFaerieGridSizeChangedNative = TMulticastDelegate<void(FIntPoint)>;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FFaerieGridSizeChanged, FIntPoint, NewGridSize);

using FFaerieGridStackChangedNative = TMulticastDelegate<void(const FInventoryKey&, EFaerieGridEventType)>;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FSpatialStackChanged, FInventoryKey, Key, EFaerieGridEventType, EventType);

/**
 * Base class for extensions that map inventory keys to positions on a 2D grid.
 */
UCLASS(Abstract)
class FAERIEINVENTORYCONTENT_API UInventoryGridExtensionBase : public UItemContainerExtensionBase
{
	GENERATED_BODY()

	friend FFaerieGridContent;

public:
	//~ UObject
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void PostInitProperties() override;
	//~ UObject

protected:
	//~ UItemContainerExtensionBase
	virtual void DeinitializeExtension(const UFaerieItemContainerBase* Container) override;
	//~ UItemContainerExtensionBase

	virtual void PreStackRemove(const FFaerieGridKeyedStack& Stack) {}
	virtual void PostStackAdd(const FFaerieGridKeyedStack& Stack) {}
	virtual void PostStackChange(const FFaerieGridKeyedStack& Stack) {}

	// Convert a point into a grid index
	int32 Ravel(const FIntPoint& Point) const;

	// Convert a grid index to a point
	FIntPoint Unravel(int32 Index) const;

	UFUNCTION(/* Replication */)
	virtual void OnRep_GridSize();

public:
	UFUNCTION(BlueprintCallable, Category = "Faerie|Grid")
	FFaerieGridPlacement GetStackPlacementData(const FInventoryKey& Key) const;

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Faerie|Grid")
	void SetGridSize(FIntPoint NewGridSize);

	FFaerieGridStackChangedNative::RegistrationType& GetOnSpatialStackChanged() { return SpatialStackChangedNative; }
	FFaerieGridSizeChangedNative::RegistrationType& GetOnGridSizeChanged() { return GridSizeChangedNative; }

protected:
	UPROPERTY(EditAnywhere, Replicated, Category = "Data")
	FFaerieGridContent GridContent;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FSpatialStackChanged SpatialStackChangedDelegate;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FFaerieGridSizeChanged GridSizeChangedDelegate;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, ReplicatedUsing = "OnRep_GridSize", Category = "Config")
	FIntPoint GridSize = FIntPoint(10, 10);

	TBitArray<> OccupiedCells;

	FFaerieGridStackChangedNative SpatialStackChangedNative;
	FFaerieGridSizeChangedNative GridSizeChangedNative;

	/*
	 * @todo we do not support multiple containers. FFaerieGridContent would need to be refactored to allow that.
	 * (Or use UInventoryReplicatedDataExtensionBase). Until then, we can safely assume we only worry about one container.
	 * @todo2, additionally, this class is tied to UFaerieItemStorage. Initializing with other types isn't supported. Safeguard this?
	 */
	UPROPERTY(Replicated)
	TObjectPtr<UFaerieItemContainerBase> InitializedContainer;
};