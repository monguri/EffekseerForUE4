
#pragma once
#include "CoreMinimal.h"
#include "UnrealEd.h"
#include "AssetTypeActions_Base.h"

class FAssetTypeActions_EffekseerMaterial : public FAssetTypeActions_Base
{
public:
	virtual FText GetName() const override
	{
		return NSLOCTEXT("AssetTypeActions", "AssetTypeActions_EffekseerMaterial", "EffekseerMaterial");
	}
	virtual FColor GetTypeColor() const override { return FColor::White; }
	virtual uint32 GetCategories() override { return EAssetTypeCategories::Misc; }
	virtual UClass* GetSupportedClass() const override;
	virtual bool IsImportedAsset() const override { return true; }

	virtual bool HasActions(const TArray<UObject*>& InObjects) const override { return true; }
	virtual void GetActions(const TArray<UObject*>& InObjects, FMenuBuilder& MenuBuilder) override;
	void ExecuteGenerateMaterialElements(TArray<TWeakObjectPtr<class UEffekseerMaterial>> Objects);
};