// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "CSVExportStyle.h"

class FCSVExportCommands : public TCommands<FCSVExportCommands>
{
public:

	FCSVExportCommands()
		: TCommands<FCSVExportCommands>(TEXT("CSVExport"), NSLOCTEXT("Contexts", "CSVExport", "CSVExport Plugin"), NAME_None, FCSVExportStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > PluginAction;
};
