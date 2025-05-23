// Fill out your copyright notice in the Description page of Project Settings.


#include "SlateWidgets/SceneAssetViewer.h"
#include "EngineUtils.h"
#include "AssetRegistry/AssetRegistryModule.h"

void SSceneAssetViewerWidget::Construct(const FArguments& inArgs)
{
	PopulateAssetSet();
	ChildSlot
	[
		SNew(SVerticalBox)
		+SVerticalBox::Slot()
			[
				SAssignNew(ListViewWidget, SListView<TSharedPtr<ListAsset>>)
					.ListItemsSource(&AssetList)
					.OnGenerateRow(this, &SSceneAssetViewerWidget::OnGeneratedRow)
			]
	];
}

void SSceneAssetViewerWidget::PopulateAssetSet()
{
	AssetList.Empty();
	BlueprintSet.Empty();
	MeshesSet.Empty();
	MatsSet.Empty();

	if (UWorld* world = GEditor->GetEditorWorldContext().World())
	{
		for (TActorIterator<AActor> it(world); it; ++it)
		{
			AActor* act = *it;
			if (UBlueprint* bp = Cast<UBlueprint>(act->GetClass()->ClassGeneratedBy))
			{
				FString bpPathName = bp->GetPathName();
				if (!BlueprintSet.Contains(bpPathName))
				{
					BlueprintSet.Add(bp->GetPathName());
					AssetList.Add(MakeShared<ListAsset>(TEXT("Blueprint"), bpPathName));
				}
			}
			TArray<UActorComponent*> actorComponents;
			act->GetComponents(actorComponents);
			for (UActorComponent* comp : actorComponents)
			{
				if (UStaticMeshComponent* smc = Cast<UStaticMeshComponent>(comp))
				{
					if (UStaticMesh* sm = smc->GetStaticMesh()) 
					{
						FString smPathName = sm->GetPathName();
						if (!MeshesSet.Contains(smPathName))
						{
							MeshesSet.Add(smPathName);
							AssetList.Add(MakeShared<ListAsset>(TEXT("Static Mesh"), smPathName));
						}
					}
					for (int32 materialIndex = 0; materialIndex < smc->GetNumMaterials(); materialIndex++)
					{
						if (UMaterialInterface* materialInterface = smc->GetMaterial(materialIndex))
						{
							FString materialPathName = materialInterface->GetPathName();
							if (!MeshesSet.Contains(materialPathName))
							{
								MatsSet.Add(materialPathName);
								AssetList.Add(MakeShared<ListAsset>(TEXT("Material"), materialPathName));
							}
						}
					}
				}
			}
		}
	}
}

TSharedRef<ITableRow> SSceneAssetViewerWidget::OnGeneratedRow(TSharedPtr<ListAsset> item, const TSharedRef<STableViewBase>& OwnerTable)
{
	ListAsset object = *item;
	return SNew(STableRow<TSharedPtr<FString>>, OwnerTable)
		[
			SNew(SHorizontalBox)
				+ SHorizontalBox::Slot().FillWidth(0.3f)
				[
					SNew(STextBlock).Text(FText::FromString(object.Type)).Justification(ETextJustify::InvariantLeft)
				]
				+SHorizontalBox::Slot().FillWidth(0.3f)
				[
					SNew(STextBlock).Text(FText::FromString(object.Path)).Justification(ETextJustify::InvariantLeft)
				]
				+SHorizontalBox::Slot().FillWidth(0.3f)
				[
					SNew(SButton)
						.Text(FText::FromString("Show in Content Browser"))
						.OnClicked_Lambda([object]()->FReply {
							FAssetRegistryModule & AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
							IAssetRegistry& AssetRegistryInterface = AssetRegistryModule.Get();
							FAssetData AssetData = AssetRegistryInterface.GetAssetByObjectPath(object.Path);
							if(AssetData.IsValid())
							{ 
								GEditor->SyncBrowserToObject(AssetData);
							}

							return FReply::Handled();
						})
				]
		];
}
