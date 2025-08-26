// Copyright Epic Games, Inc. All Rights Reserved.

#include "PPContextEffectsLibraryFactory.h"

#include "FeedBack/ContextEffects/PPContextEffectsLibrary.h"
#include "Templates/SubclassOf.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(PPContextEffectsLibraryFactory)

class FFeedbackContext;
class UClass;
class UObject;

UPPContextEffectsLibraryFactory::UPPContextEffectsLibraryFactory(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	SupportedClass = UPPContextEffectsLibrary::StaticClass();

	bCreateNew = true;
	bEditorImport = false;
	bEditAfterNew = true;
}

UObject* UPPContextEffectsLibraryFactory::FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
	UPPContextEffectsLibrary* PPContextEffectsLibrary = NewObject<UPPContextEffectsLibrary>(InParent, Name, Flags);

	return PPContextEffectsLibrary;
}
