// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "GameplayMessageRuntime/Public/GameFramework/GameplayMessageSubsystem.h"
#include "Engine/Classes/Engine/GameInstance.h"
#include "GameplayTags/Classes/GameplayTagContainer.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeGameplayMessageSubsystem() {}
// Cross Module References
	ENGINE_API UClass* Z_Construct_UClass_UGameInstanceSubsystem();
	GAMEPLAYMESSAGERUNTIME_API UClass* Z_Construct_UClass_UGameplayMessageSubsystem();
	GAMEPLAYMESSAGERUNTIME_API UClass* Z_Construct_UClass_UGameplayMessageSubsystem_NoRegister();
	GAMEPLAYMESSAGERUNTIME_API UScriptStruct* Z_Construct_UScriptStruct_FGameplayMessageListenerData();
	GAMEPLAYMESSAGERUNTIME_API UScriptStruct* Z_Construct_UScriptStruct_FGameplayMessageListenerHandle();
	GAMEPLAYTAGS_API UScriptStruct* Z_Construct_UScriptStruct_FGameplayTag();
	UPackage* Z_Construct_UPackage__Script_GameplayMessageRuntime();
// End Cross Module References
	static FStructRegistrationInfo Z_Registration_Info_UScriptStruct_GameplayMessageListenerHandle;
class UScriptStruct* FGameplayMessageListenerHandle::StaticStruct()
{
	if (!Z_Registration_Info_UScriptStruct_GameplayMessageListenerHandle.OuterSingleton)
	{
		Z_Registration_Info_UScriptStruct_GameplayMessageListenerHandle.OuterSingleton = GetStaticStruct(Z_Construct_UScriptStruct_FGameplayMessageListenerHandle, Z_Construct_UPackage__Script_GameplayMessageRuntime(), TEXT("GameplayMessageListenerHandle"));
	}
	return Z_Registration_Info_UScriptStruct_GameplayMessageListenerHandle.OuterSingleton;
}
template<> GAMEPLAYMESSAGERUNTIME_API UScriptStruct* StaticStruct<FGameplayMessageListenerHandle>()
{
	return FGameplayMessageListenerHandle::StaticStruct();
}
	struct Z_Construct_UScriptStruct_FGameplayMessageListenerHandle_Statics
	{
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Struct_MetaDataParams[];
#endif
		static void* NewStructOps();
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_Subsystem_MetaData[];
#endif
		static const UECodeGen_Private::FWeakObjectPropertyParams NewProp_Subsystem;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_Channel_MetaData[];
#endif
		static const UECodeGen_Private::FStructPropertyParams NewProp_Channel;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_ID_MetaData[];
#endif
		static const UECodeGen_Private::FIntPropertyParams NewProp_ID;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
		static const UECodeGen_Private::FStructParams ReturnStructParams;
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FGameplayMessageListenerHandle_Statics::Struct_MetaDataParams[] = {
		{ "BlueprintType", "true" },
		{ "Comment", "/**\n * An opaque handle that can be used to remove a previously registered message listener\n * @see UGameplayMessageSubsystem::RegisterListener and UGameplayMessageSubsystem::UnregisterListener\n */" },
		{ "ModuleRelativePath", "Public/GameFramework/GameplayMessageSubsystem.h" },
		{ "ToolTip", "An opaque handle that can be used to remove a previously registered message listener\n@see UGameplayMessageSubsystem::RegisterListener and UGameplayMessageSubsystem::UnregisterListener" },
	};
#endif
	void* Z_Construct_UScriptStruct_FGameplayMessageListenerHandle_Statics::NewStructOps()
	{
		return (UScriptStruct::ICppStructOps*)new UScriptStruct::TCppStructOps<FGameplayMessageListenerHandle>();
	}
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FGameplayMessageListenerHandle_Statics::NewProp_Subsystem_MetaData[] = {
		{ "ModuleRelativePath", "Public/GameFramework/GameplayMessageSubsystem.h" },
	};
#endif
	const UECodeGen_Private::FWeakObjectPropertyParams Z_Construct_UScriptStruct_FGameplayMessageListenerHandle_Statics::NewProp_Subsystem = { "Subsystem", nullptr, (EPropertyFlags)0x0044000000002000, UECodeGen_Private::EPropertyGenFlags::WeakObject, RF_Public|RF_Transient|RF_MarkAsNative, 1, nullptr, nullptr, STRUCT_OFFSET(FGameplayMessageListenerHandle, Subsystem), Z_Construct_UClass_UGameplayMessageSubsystem_NoRegister, METADATA_PARAMS(Z_Construct_UScriptStruct_FGameplayMessageListenerHandle_Statics::NewProp_Subsystem_MetaData, UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FGameplayMessageListenerHandle_Statics::NewProp_Subsystem_MetaData)) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FGameplayMessageListenerHandle_Statics::NewProp_Channel_MetaData[] = {
		{ "ModuleRelativePath", "Public/GameFramework/GameplayMessageSubsystem.h" },
	};
#endif
	const UECodeGen_Private::FStructPropertyParams Z_Construct_UScriptStruct_FGameplayMessageListenerHandle_Statics::NewProp_Channel = { "Channel", nullptr, (EPropertyFlags)0x0040000000002000, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, 1, nullptr, nullptr, STRUCT_OFFSET(FGameplayMessageListenerHandle, Channel), Z_Construct_UScriptStruct_FGameplayTag, METADATA_PARAMS(Z_Construct_UScriptStruct_FGameplayMessageListenerHandle_Statics::NewProp_Channel_MetaData, UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FGameplayMessageListenerHandle_Statics::NewProp_Channel_MetaData)) }; // 1225434376
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FGameplayMessageListenerHandle_Statics::NewProp_ID_MetaData[] = {
		{ "ModuleRelativePath", "Public/GameFramework/GameplayMessageSubsystem.h" },
	};
#endif
	const UECodeGen_Private::FIntPropertyParams Z_Construct_UScriptStruct_FGameplayMessageListenerHandle_Statics::NewProp_ID = { "ID", nullptr, (EPropertyFlags)0x0040000000002000, UECodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, 1, nullptr, nullptr, STRUCT_OFFSET(FGameplayMessageListenerHandle, ID), METADATA_PARAMS(Z_Construct_UScriptStruct_FGameplayMessageListenerHandle_Statics::NewProp_ID_MetaData, UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FGameplayMessageListenerHandle_Statics::NewProp_ID_MetaData)) };
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UScriptStruct_FGameplayMessageListenerHandle_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FGameplayMessageListenerHandle_Statics::NewProp_Subsystem,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FGameplayMessageListenerHandle_Statics::NewProp_Channel,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FGameplayMessageListenerHandle_Statics::NewProp_ID,
	};
	const UECodeGen_Private::FStructParams Z_Construct_UScriptStruct_FGameplayMessageListenerHandle_Statics::ReturnStructParams = {
		(UObject* (*)())Z_Construct_UPackage__Script_GameplayMessageRuntime,
		nullptr,
		&NewStructOps,
		"GameplayMessageListenerHandle",
		sizeof(FGameplayMessageListenerHandle),
		alignof(FGameplayMessageListenerHandle),
		Z_Construct_UScriptStruct_FGameplayMessageListenerHandle_Statics::PropPointers,
		UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FGameplayMessageListenerHandle_Statics::PropPointers),
		RF_Public|RF_Transient|RF_MarkAsNative,
		EStructFlags(0x00000201),
		METADATA_PARAMS(Z_Construct_UScriptStruct_FGameplayMessageListenerHandle_Statics::Struct_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FGameplayMessageListenerHandle_Statics::Struct_MetaDataParams))
	};
	UScriptStruct* Z_Construct_UScriptStruct_FGameplayMessageListenerHandle()
	{
		if (!Z_Registration_Info_UScriptStruct_GameplayMessageListenerHandle.InnerSingleton)
		{
			UECodeGen_Private::ConstructUScriptStruct(Z_Registration_Info_UScriptStruct_GameplayMessageListenerHandle.InnerSingleton, Z_Construct_UScriptStruct_FGameplayMessageListenerHandle_Statics::ReturnStructParams);
		}
		return Z_Registration_Info_UScriptStruct_GameplayMessageListenerHandle.InnerSingleton;
	}
	static FStructRegistrationInfo Z_Registration_Info_UScriptStruct_GameplayMessageListenerData;
class UScriptStruct* FGameplayMessageListenerData::StaticStruct()
{
	if (!Z_Registration_Info_UScriptStruct_GameplayMessageListenerData.OuterSingleton)
	{
		Z_Registration_Info_UScriptStruct_GameplayMessageListenerData.OuterSingleton = GetStaticStruct(Z_Construct_UScriptStruct_FGameplayMessageListenerData, Z_Construct_UPackage__Script_GameplayMessageRuntime(), TEXT("GameplayMessageListenerData"));
	}
	return Z_Registration_Info_UScriptStruct_GameplayMessageListenerData.OuterSingleton;
}
template<> GAMEPLAYMESSAGERUNTIME_API UScriptStruct* StaticStruct<FGameplayMessageListenerData>()
{
	return FGameplayMessageListenerData::StaticStruct();
}
	struct Z_Construct_UScriptStruct_FGameplayMessageListenerData_Statics
	{
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Struct_MetaDataParams[];
#endif
		static void* NewStructOps();
		static const UECodeGen_Private::FStructParams ReturnStructParams;
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FGameplayMessageListenerData_Statics::Struct_MetaDataParams[] = {
		{ "Comment", "/** \n * Entry information for a single registered listener\n */" },
		{ "ModuleRelativePath", "Public/GameFramework/GameplayMessageSubsystem.h" },
		{ "ToolTip", "Entry information for a single registered listener" },
	};
#endif
	void* Z_Construct_UScriptStruct_FGameplayMessageListenerData_Statics::NewStructOps()
	{
		return (UScriptStruct::ICppStructOps*)new UScriptStruct::TCppStructOps<FGameplayMessageListenerData>();
	}
	const UECodeGen_Private::FStructParams Z_Construct_UScriptStruct_FGameplayMessageListenerData_Statics::ReturnStructParams = {
		(UObject* (*)())Z_Construct_UPackage__Script_GameplayMessageRuntime,
		nullptr,
		&NewStructOps,
		"GameplayMessageListenerData",
		sizeof(FGameplayMessageListenerData),
		alignof(FGameplayMessageListenerData),
		nullptr,
		0,
		RF_Public|RF_Transient|RF_MarkAsNative,
		EStructFlags(0x00000001),
		METADATA_PARAMS(Z_Construct_UScriptStruct_FGameplayMessageListenerData_Statics::Struct_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FGameplayMessageListenerData_Statics::Struct_MetaDataParams))
	};
	UScriptStruct* Z_Construct_UScriptStruct_FGameplayMessageListenerData()
	{
		if (!Z_Registration_Info_UScriptStruct_GameplayMessageListenerData.InnerSingleton)
		{
			UECodeGen_Private::ConstructUScriptStruct(Z_Registration_Info_UScriptStruct_GameplayMessageListenerData.InnerSingleton, Z_Construct_UScriptStruct_FGameplayMessageListenerData_Statics::ReturnStructParams);
		}
		return Z_Registration_Info_UScriptStruct_GameplayMessageListenerData.InnerSingleton;
	}
	void UGameplayMessageSubsystem::StaticRegisterNativesUGameplayMessageSubsystem()
	{
		UClass* Class = UGameplayMessageSubsystem::StaticClass();
		static const FNameNativePtrPair Funcs[] = {
			{ "K2_BroadcastMessage", &UGameplayMessageSubsystem::execK2_BroadcastMessage },
		};
		FNativeFunctionRegistrar::RegisterFunctions(Class, Funcs, UE_ARRAY_COUNT(Funcs));
	}
	struct Z_Construct_UFunction_UGameplayMessageSubsystem_K2_BroadcastMessage_Statics
	{
		struct GameplayMessageSubsystem_eventK2_BroadcastMessage_Parms
		{
			FGameplayTag Channel;
			int32 Message;
		};
		static const UECodeGen_Private::FStructPropertyParams NewProp_Channel;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_Message_MetaData[];
#endif
		static const UECodeGen_Private::FIntPropertyParams NewProp_Message;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UECodeGen_Private::FFunctionParams FuncParams;
	};
	const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UGameplayMessageSubsystem_K2_BroadcastMessage_Statics::NewProp_Channel = { "Channel", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, 1, nullptr, nullptr, STRUCT_OFFSET(GameplayMessageSubsystem_eventK2_BroadcastMessage_Parms, Channel), Z_Construct_UScriptStruct_FGameplayTag, METADATA_PARAMS(nullptr, 0) }; // 1225434376
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UGameplayMessageSubsystem_K2_BroadcastMessage_Statics::NewProp_Message_MetaData[] = {
		{ "NativeConst", "" },
	};
#endif
	const UECodeGen_Private::FIntPropertyParams Z_Construct_UFunction_UGameplayMessageSubsystem_K2_BroadcastMessage_Statics::NewProp_Message = { "Message", nullptr, (EPropertyFlags)0x0010000008000182, UECodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, 1, nullptr, nullptr, STRUCT_OFFSET(GameplayMessageSubsystem_eventK2_BroadcastMessage_Parms, Message), METADATA_PARAMS(Z_Construct_UFunction_UGameplayMessageSubsystem_K2_BroadcastMessage_Statics::NewProp_Message_MetaData, UE_ARRAY_COUNT(Z_Construct_UFunction_UGameplayMessageSubsystem_K2_BroadcastMessage_Statics::NewProp_Message_MetaData)) };
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UGameplayMessageSubsystem_K2_BroadcastMessage_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UGameplayMessageSubsystem_K2_BroadcastMessage_Statics::NewProp_Channel,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UGameplayMessageSubsystem_K2_BroadcastMessage_Statics::NewProp_Message,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UGameplayMessageSubsystem_K2_BroadcastMessage_Statics::Function_MetaDataParams[] = {
		{ "AllowAbstract", "false" },
		{ "Category", "Messaging" },
		{ "Comment", "/**\n\x09 * Broadcast a message on the specified channel\n\x09 *\n\x09 * @param Channel\x09\x09\x09The message channel to broadcast on\n\x09 * @param Message\x09\x09\x09The message to send (must be the same type of UScriptStruct expected by the listeners for this channel, otherwise an error will be logged)\n\x09 */" },
		{ "CustomStructureParam", "Message" },
		{ "CustomThunk", "true" },
		{ "DisplayName", "Broadcast Message" },
		{ "ModuleRelativePath", "Public/GameFramework/GameplayMessageSubsystem.h" },
		{ "ToolTip", "Broadcast a message on the specified channel\n\n@param Channel                       The message channel to broadcast on\n@param Message                       The message to send (must be the same type of UScriptStruct expected by the listeners for this channel, otherwise an error will be logged)" },
	};
#endif
	const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UGameplayMessageSubsystem_K2_BroadcastMessage_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UGameplayMessageSubsystem, nullptr, "K2_BroadcastMessage", nullptr, nullptr, sizeof(Z_Construct_UFunction_UGameplayMessageSubsystem_K2_BroadcastMessage_Statics::GameplayMessageSubsystem_eventK2_BroadcastMessage_Parms), Z_Construct_UFunction_UGameplayMessageSubsystem_K2_BroadcastMessage_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UGameplayMessageSubsystem_K2_BroadcastMessage_Statics::PropPointers), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04480401, 0, 0, METADATA_PARAMS(Z_Construct_UFunction_UGameplayMessageSubsystem_K2_BroadcastMessage_Statics::Function_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UFunction_UGameplayMessageSubsystem_K2_BroadcastMessage_Statics::Function_MetaDataParams)) };
	UFunction* Z_Construct_UFunction_UGameplayMessageSubsystem_K2_BroadcastMessage()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UGameplayMessageSubsystem_K2_BroadcastMessage_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(UGameplayMessageSubsystem);
	UClass* Z_Construct_UClass_UGameplayMessageSubsystem_NoRegister()
	{
		return UGameplayMessageSubsystem::StaticClass();
	}
	struct Z_Construct_UClass_UGameplayMessageSubsystem_Statics
	{
		static UObject* (*const DependentSingletons[])();
		static const FClassFunctionLinkInfo FuncInfo[];
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UECodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_UGameplayMessageSubsystem_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_UGameInstanceSubsystem,
		(UObject* (*)())Z_Construct_UPackage__Script_GameplayMessageRuntime,
	};
	const FClassFunctionLinkInfo Z_Construct_UClass_UGameplayMessageSubsystem_Statics::FuncInfo[] = {
		{ &Z_Construct_UFunction_UGameplayMessageSubsystem_K2_BroadcastMessage, "K2_BroadcastMessage" }, // 2605432884
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UGameplayMessageSubsystem_Statics::Class_MetaDataParams[] = {
		{ "Comment", "/**\n * This system allows event raisers and listeners to register for messages without\n * having to know about each other directly, though they must agree on the format\n * of the message (as a USTRUCT() type).\n *\n *\n * You can get to the message router from the game instance:\n *    UGameInstance::GetSubsystem<UGameplayMessageSubsystem>(GameInstance)\n * or directly from anything that has a route to a world:\n *    UGameplayMessageSubsystem::Get(WorldContextObject)\n *\n * Note that call order when there are multiple listeners for the same channel is\n * not guaranteed and can change over time!\n */" },
		{ "IncludePath", "GameFramework/GameplayMessageSubsystem.h" },
		{ "ModuleRelativePath", "Public/GameFramework/GameplayMessageSubsystem.h" },
		{ "ToolTip", "This system allows event raisers and listeners to register for messages without\nhaving to know about each other directly, though they must agree on the format\nof the message (as a USTRUCT() type).\n\n\nYou can get to the message router from the game instance:\n   UGameInstance::GetSubsystem<UGameplayMessageSubsystem>(GameInstance)\nor directly from anything that has a route to a world:\n   UGameplayMessageSubsystem::Get(WorldContextObject)\n\nNote that call order when there are multiple listeners for the same channel is\nnot guaranteed and can change over time!" },
	};
#endif
	const FCppClassTypeInfoStatic Z_Construct_UClass_UGameplayMessageSubsystem_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UGameplayMessageSubsystem>::IsAbstract,
	};
	const UECodeGen_Private::FClassParams Z_Construct_UClass_UGameplayMessageSubsystem_Statics::ClassParams = {
		&UGameplayMessageSubsystem::StaticClass,
		nullptr,
		&StaticCppClassTypeInfo,
		DependentSingletons,
		FuncInfo,
		nullptr,
		nullptr,
		UE_ARRAY_COUNT(DependentSingletons),
		UE_ARRAY_COUNT(FuncInfo),
		0,
		0,
		0x001000A0u,
		METADATA_PARAMS(Z_Construct_UClass_UGameplayMessageSubsystem_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_UGameplayMessageSubsystem_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_UGameplayMessageSubsystem()
	{
		if (!Z_Registration_Info_UClass_UGameplayMessageSubsystem.OuterSingleton)
		{
			UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UGameplayMessageSubsystem.OuterSingleton, Z_Construct_UClass_UGameplayMessageSubsystem_Statics::ClassParams);
		}
		return Z_Registration_Info_UClass_UGameplayMessageSubsystem.OuterSingleton;
	}
	template<> GAMEPLAYMESSAGERUNTIME_API UClass* StaticClass<UGameplayMessageSubsystem>()
	{
		return UGameplayMessageSubsystem::StaticClass();
	}
	DEFINE_VTABLE_PTR_HELPER_CTOR(UGameplayMessageSubsystem);
	UGameplayMessageSubsystem::~UGameplayMessageSubsystem() {}
	struct Z_CompiledInDeferFile_FID_PORTFOLIO_PROJECT_HO_Portfolio01_Plugins_GameplayMessageRouter_Source_GameplayMessageRuntime_Public_GameFramework_GameplayMessageSubsystem_h_Statics
	{
		static const FStructRegisterCompiledInInfo ScriptStructInfo[];
		static const FClassRegisterCompiledInInfo ClassInfo[];
	};
	const FStructRegisterCompiledInInfo Z_CompiledInDeferFile_FID_PORTFOLIO_PROJECT_HO_Portfolio01_Plugins_GameplayMessageRouter_Source_GameplayMessageRuntime_Public_GameFramework_GameplayMessageSubsystem_h_Statics::ScriptStructInfo[] = {
		{ FGameplayMessageListenerHandle::StaticStruct, Z_Construct_UScriptStruct_FGameplayMessageListenerHandle_Statics::NewStructOps, TEXT("GameplayMessageListenerHandle"), &Z_Registration_Info_UScriptStruct_GameplayMessageListenerHandle, CONSTRUCT_RELOAD_VERSION_INFO(FStructReloadVersionInfo, sizeof(FGameplayMessageListenerHandle), 4176102888U) },
		{ FGameplayMessageListenerData::StaticStruct, Z_Construct_UScriptStruct_FGameplayMessageListenerData_Statics::NewStructOps, TEXT("GameplayMessageListenerData"), &Z_Registration_Info_UScriptStruct_GameplayMessageListenerData, CONSTRUCT_RELOAD_VERSION_INFO(FStructReloadVersionInfo, sizeof(FGameplayMessageListenerData), 2403622959U) },
	};
	const FClassRegisterCompiledInInfo Z_CompiledInDeferFile_FID_PORTFOLIO_PROJECT_HO_Portfolio01_Plugins_GameplayMessageRouter_Source_GameplayMessageRuntime_Public_GameFramework_GameplayMessageSubsystem_h_Statics::ClassInfo[] = {
		{ Z_Construct_UClass_UGameplayMessageSubsystem, UGameplayMessageSubsystem::StaticClass, TEXT("UGameplayMessageSubsystem"), &Z_Registration_Info_UClass_UGameplayMessageSubsystem, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UGameplayMessageSubsystem), 1394447791U) },
	};
	static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_PORTFOLIO_PROJECT_HO_Portfolio01_Plugins_GameplayMessageRouter_Source_GameplayMessageRuntime_Public_GameFramework_GameplayMessageSubsystem_h_3168583346(TEXT("/Script/GameplayMessageRuntime"),
		Z_CompiledInDeferFile_FID_PORTFOLIO_PROJECT_HO_Portfolio01_Plugins_GameplayMessageRouter_Source_GameplayMessageRuntime_Public_GameFramework_GameplayMessageSubsystem_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_PORTFOLIO_PROJECT_HO_Portfolio01_Plugins_GameplayMessageRouter_Source_GameplayMessageRuntime_Public_GameFramework_GameplayMessageSubsystem_h_Statics::ClassInfo),
		Z_CompiledInDeferFile_FID_PORTFOLIO_PROJECT_HO_Portfolio01_Plugins_GameplayMessageRouter_Source_GameplayMessageRuntime_Public_GameFramework_GameplayMessageSubsystem_h_Statics::ScriptStructInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_PORTFOLIO_PROJECT_HO_Portfolio01_Plugins_GameplayMessageRouter_Source_GameplayMessageRuntime_Public_GameFramework_GameplayMessageSubsystem_h_Statics::ScriptStructInfo),
		nullptr, 0);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
