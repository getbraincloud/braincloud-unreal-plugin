// Copyright 2026 bitHeads, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EditorUtilityWidget.h"
#include "BCPortalTypes.h"
#include "BCSecureStore.h"
#include "Components/SlateWrapperTypes.h"
#include "BCChildAppRowWidget.generated.h"

class UButton;
class UComboBoxString;
class UBCUtilityWidgetBase;

/**
 * A child app row in the brainCloud editor panel.
 * Create a Blueprint of this class with a ChildAppSelectBox, and optionally a RefreshButton and RemoveButton.
 */
UCLASS(Abstract, Blueprintable)
class UBCChildAppRowWidget : public UEditorUtilityWidget
{
    GENERATED_BODY()

public:
    /**
     * The list of apps to choose the child app from.
     */
    UPROPERTY(BlueprintReadOnly, Category = "BrainCloud", meta = (BindWidget))
    UComboBoxString* ChildAppSelectBox = nullptr;

    /**
     * Refreshes the list of apps.
     */
    UPROPERTY(BlueprintReadOnly, Category = "BrainCloud", meta = (BindWidgetOptional))
    UButton* RefreshButton = nullptr;

    /**
     * Removes this child app.
     */
    UPROPERTY(BlueprintReadOnly, Category = "BrainCloud", meta = (BindWidgetOptional))
    UButton* RemoveButton = nullptr;

    void Setup(UBCUtilityWidgetBase* InOwner, const FBCStoredChildApp& InChild);

    void SetOptions(const TArray<FBCPortalApp>& Apps, bool bAppsLoaded);
    void SetRemovable(bool bRemovable);
    void Clear();

    const FBCStoredChildApp& GetChild() const { return Child; }
    void SetSecret(const FString& AppId, const FString& AppSecret);

private:
    UFUNCTION() void HandleSelectionChanged(FString SelectedItem, ESelectInfo::Type SelectionType);
    UFUNCTION() void HandleRefreshClicked();
    UFUNCTION() void HandleRemoveClicked();

    TWeakObjectPtr<UBCUtilityWidgetBase> Owner;
    FBCStoredChildApp Child;
    TArray<FBCPortalApp> Options;
};
