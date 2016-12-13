
#include "EffekseerEdPrivatePCH.h"
#include "EffekseerModelFactory.h"
#include "EffekseerModel.h"

UEffekseerModelFactory::UEffekseerModelFactory(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	SupportedClass = UEffekseerModel::StaticClass();
	bCreateNew = false;
	bText = false;
	bEditorImport = true;
	Formats.Add(TEXT("efkmodel;EffekseerModel"));
}
bool UEffekseerModelFactory::DoesSupportClass(UClass* Class)
{
	return (Class == UEffekseerModel::StaticClass());
}
UClass* UEffekseerModelFactory::ResolveSupportedClass()
{
	return UEffekseerModel::StaticClass();
}

UObject* UEffekseerModelFactory::FactoryCreateBinary(
	UClass* InClass,
	UObject* InParent,
	FName InName,
	EObjectFlags Flags,
	UObject* Context,
	const TCHAR* Type,
	const uint8*& Buffer,
	const uint8* BufferEnd,
	FFeedbackContext* Warn)
{
	//TArray<UObject*> retAssets;


	UEffekseerModel* asset = CastChecked<UEffekseerModel>(StaticConstructObject(InClass, InParent, InName, Flags));
	
	if (asset)
	{
		auto path = asset->GetPathName();
		//asset->Load(Buffer, (int32_t)(BufferEnd - Buffer), *path);

		if (!asset->AssetImportData)
		{
			//asset->AssetImportData = NewObject<UAssetImportData>(this, TEXT("AssetImportData"));
			asset->AssetImportData = ConstructObject<UAssetImportData>(UAssetImportData::StaticClass(), asset);
		}

		asset->AssetImportData->Update(CurrentFilename);
	}
	
	return asset;
}

bool UEffekseerModelFactory::CanReimport(UObject* Obj, TArray<FString>& OutFilenames)
{
	UEffekseerModel* asset = Cast<UEffekseerModel>(Obj);
	if (asset && asset->AssetImportData)
	{
		asset->AssetImportData->ExtractFilenames(OutFilenames);
		return true;
	}
	return false;
}

void UEffekseerModelFactory::SetReimportPaths(UObject* Obj, const TArray<FString>& NewReimportPaths)
{
	UEffekseerModel* asset = Cast<UEffekseerModel>(Obj);
	if (asset && ensure(NewReimportPaths.Num() == 1))
	{
		asset->AssetImportData->UpdateFilenameOnly(NewReimportPaths[0]);
	}
}

EReimportResult::Type UEffekseerModelFactory::Reimport(UObject* Obj)
{
	UEffekseerModel* asset = Cast<UEffekseerModel>(Obj);
	if (!asset)
	{
		return EReimportResult::Failed;
	}

	const FString Filename = asset->AssetImportData->GetFirstFilename();

	if (!Filename.Len() || IFileManager::Get().FileSize(*Filename) == INDEX_NONE)
	{
		return EReimportResult::Failed;
	}

	EReimportResult::Type Result = EReimportResult::Failed;

	if (UFactory::StaticImportObject(
		asset->GetClass(), 
		asset->GetOuter(),
		*asset->GetName(), 
		RF_Public | RF_Standalone, 
		*Filename, 
		NULL, 
		this))
	{
		if (asset->GetOuter())
		{
			asset->GetOuter()->MarkPackageDirty();
		}
		else
		{
			asset->MarkPackageDirty();
		}
		return EReimportResult::Succeeded;
	}

	return EReimportResult::Failed;
}