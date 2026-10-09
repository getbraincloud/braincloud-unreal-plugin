// Copyright 2026 bitHeads, Inc. All Rights Reserved.

#include "BCChildAppRowWidget.h"
#include "BCUtilityWidgetBase.h"
#include "Components/Button.h"
#include "Components/ComboBoxString.h"

void UBCChildAppRowWidget::Setup(UBCUtilityWidgetBase* InOwner, const FBCStoredChildApp& InChild)
{
    Owner = InOwner;
    Child = InChild;

    if (ChildAppSelectBox)
    {
        ChildAppSelectBox->OnSelectionChanged.RemoveAll(this);
        ChildAppSelectBox->OnSelectionChanged.AddDynamic(this, &UBCChildAppRowWidget::HandleSelectionChanged);
    }
    if (RefreshButton)
    {
        RefreshButton->OnClicked.RemoveAll(this);
        RefreshButton->OnClicked.AddDynamic(this, &UBCChildAppRowWidget::HandleRefreshClicked);
    }
    if (RemoveButton)
    {
        RemoveButton->OnClicked.RemoveAll(this);
        RemoveButton->OnClicked.AddDynamic(this, &UBCChildAppRowWidget::HandleRemoveClicked);
    }
}

void UBCChildAppRowWidget::SetOptions(const TArray<FBCPortalApp>& Apps, bool bAppsLoaded)
{
    Options = Apps;
    const bool bHasSelection = !Child.AppId.IsEmpty();
    const bool bSelectionListed = Options.ContainsByPredicate(
        [this](const FBCPortalApp& App) { return App.AppId == Child.AppId; });

    if (bHasSelection && !bSelectionListed)
    {
        if (bAppsLoaded)
        {
            Child = FBCStoredChildApp();
        }
        else
        {
            FBCPortalApp Stored;
            Stored.AppId = Child.AppId;
            Stored.AppName = Child.AppName;
            Options.Insert(Stored, 0);
        }
    }

    if (!ChildAppSelectBox)
    {
        return;
    }
    ChildAppSelectBox->ClearOptions();
    ChildAppSelectBox->ClearSelection();
    int32 SelectIndex = INDEX_NONE;
    for (int32 i = 0; i < Options.Num(); ++i)
    {
        const FString Label = Options[i].AppName.IsEmpty() ? Options[i].AppId
            : FString::Printf(TEXT("%s (%s)"), *Options[i].AppName, *Options[i].AppId);
        ChildAppSelectBox->AddOption(Label);
        if (!Child.AppId.IsEmpty() && Options[i].AppId == Child.AppId)
        {
            SelectIndex = i;
        }
    }
    if (SelectIndex != INDEX_NONE)
    {
        ChildAppSelectBox->SetSelectedIndex(SelectIndex);
    }
}

void UBCChildAppRowWidget::SetRemovable(bool bRemovable)
{
    if (RemoveButton)
    {
        RemoveButton->SetVisibility(bRemovable ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
    }
}

void UBCChildAppRowWidget::Clear()
{
    Child = FBCStoredChildApp();
    if (ChildAppSelectBox)
    {
        ChildAppSelectBox->ClearSelection();
    }
}

void UBCChildAppRowWidget::SetSecret(const FString& AppId, const FString& AppSecret)
{
    if (Child.AppId == AppId)
    {
        Child.AppSecret = AppSecret;
    }
}

void UBCChildAppRowWidget::HandleSelectionChanged(FString SelectedItem, ESelectInfo::Type SelectionType)
{
    if (SelectionType == ESelectInfo::Direct || !ChildAppSelectBox)
    {
        return;
    }
    const int32 Index = ChildAppSelectBox->GetSelectedIndex();
    if (!Options.IsValidIndex(Index))
    {
        return;
    }
    Child.AppId = Options[Index].AppId;
    Child.AppName = Options[Index].AppName;
    Child.AppSecret.Empty();
    if (Owner.IsValid())
    {
        Owner->OnChildRowSelected(this);
    }
}

void UBCChildAppRowWidget::HandleRefreshClicked()
{
    if (Owner.IsValid())
    {
        Owner->OnChildRowRefresh(this);
    }
}

void UBCChildAppRowWidget::HandleRemoveClicked()
{
    if (Owner.IsValid())
    {
        Owner->OnChildRowRemove(this);
    }
}
