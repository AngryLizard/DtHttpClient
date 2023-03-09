

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

DECLARE_LOG_CATEGORY_EXTERN(DTHttpServer, Log, All);

class FDTHttpServerModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	FReply ServerButtonClicked();
	FReply SetupButtonClicked();

private:
	void RegisterMenus();

	const FSlateBrush* GetStatusImage() const;
	FText GetStatusText() const;
	bool IsButtonEnabled() const;
};
