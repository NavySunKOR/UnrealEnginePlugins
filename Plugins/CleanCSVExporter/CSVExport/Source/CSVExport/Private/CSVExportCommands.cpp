// Copyright Epic Games, Inc. All Rights Reserved.

#include "CSVExportCommands.h"

#define LOCTEXT_NAMESPACE "FCSVExportModule"

void FCSVExportCommands::RegisterCommands()
{
	UI_COMMAND(PluginAction, "CSVExport", "Execute CSVExport action", EUserInterfaceActionType::Button, FInputGesture());
}

#undef LOCTEXT_NAMESPACE
