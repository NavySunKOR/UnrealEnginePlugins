// Copyright Epic Games, Inc. All Rights Reserved.
/*
	author : 조호연(Ransom or NavySunKOR) 
	email : navysunkors2@gmail.com
	first created date : 2021-07-04 02:28
	object : CSV 파일을 쌍따옴표 없이 저장.
*/

#include "CSVExport.h"
#include "CSVExportStyle.h"
#include "CSVExportCommands.h"
#include "Misc/MessageDialog.h"
#include "ToolMenus.h"
#include "EditorFramework/AssetImportData.h"
#include "ContentBrowserModule.h"
#include <IContentBrowserSingleton.h>
#include <Runtime/Core/Public/Misc/FileHelper.h>

static const FName CSVExportTabName("CSVExport");

#define LOCTEXT_NAMESPACE "FCSVExportModule"

void FCSVExportModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
	FCSVExportStyle::Initialize();
	FCSVExportStyle::ReloadTextures();

	FCSVExportCommands::Register();
	
	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FCSVExportCommands::Get().PluginAction,
		FExecuteAction::CreateRaw(this, &FCSVExportModule::PluginButtonClicked),
		FCanExecuteAction());

	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FCSVExportModule::RegisterMenus));
}

void FCSVExportModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	UToolMenus::UnRegisterStartupCallback(this);

	UToolMenus::UnregisterOwner(this);

	FCSVExportStyle::Shutdown();

	FCSVExportCommands::Unregister();
}

void FCSVExportModule::PluginButtonClicked()
{
	// Put your "OnButtonClicked" stuff here

	/*
		1. 컨텐츠 브라우저에서 선택한 에셋들을 불러온다.
		2. 선택된 에셋을 UCurveTable로 변환.
		3. 이 테이블이 유효하면 파일 이름으로 CSV를 붙여서 변환.
	
	*/

	//다른 플러그인들을 참고하여 찾음.
	IContentBrowserSingleton& ContentBrowser = FModuleManager::LoadModuleChecked<FContentBrowserModule>("ContentBrowser").Get();
	static TArray<FAssetData> SelectedAssets;
	ContentBrowser.GetSelectedAssets(SelectedAssets);


	//DataTable 엔진코드를 찾아서 사용하고 있는 레퍼런스를 찾아 제작.
	for (int i = 0; i < SelectedAssets.Num(); i++)
	{
		UE_LOG(LogTemp,Warning,TEXT("ASSET NAME : %s"), *SelectedAssets[i].GetAsset()->GetName())
			UDataTable* tab = Cast<UDataTable>(SelectedAssets[i].GetAsset());
		if (tab)
		{
			const FString fileName = tab->AssetImportData->GetFirstFilename();
			const FString filePath = FPaths::ProjectDir() + FPaths::GetBaseFilename(fileName) + TEXT(".csv");
			FString fileContents = tab->GetTableAsCSV();
			fileContents = fileContents.Replace(TEXT(R"(")"), TEXT(""));
			FFileHelper::SaveStringToFile(fileContents, *filePath);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Plugin gone wrong!"))
		}
	}



	
	//
	//FText DialogText = FText::Format(
	//						LOCTEXT("PluginButtonDialogText", "Add code to {0} in {1} to override this button's actions"),
	//						FText::FromString(TEXT("FCSVExportModule::PluginButtonClicked()")),
	//						FText::FromString(TEXT("CSVExport.cpp"))
	//				   );
	//FMessageDialog::Open(EAppMsgType::Ok, DialogText);
}

void FCSVExportModule::RegisterMenus()
{
	// Owner will be used for cleanup in call to UToolMenus::UnregisterOwner
	FToolMenuOwnerScoped OwnerScoped(this);

	{
		UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Window");
		{
			FToolMenuSection& Section = Menu->FindOrAddSection("WindowLayout");
			Section.AddMenuEntryWithCommandList(FCSVExportCommands::Get().PluginAction, PluginCommands);
		}
	}

	{
		UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar");
		{
			FToolMenuSection& Section = ToolbarMenu->FindOrAddSection("Settings");
			{
				FToolMenuEntry& Entry = Section.AddEntry(FToolMenuEntry::InitToolBarButton(FCSVExportCommands::Get().PluginAction));
				Entry.SetCommandList(PluginCommands);
			}
		}
	}
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FCSVExportModule, CSVExport)