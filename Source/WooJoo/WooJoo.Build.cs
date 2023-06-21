// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;

public class WooJoo : ModuleRules
{
	public WooJoo(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "UMG", "EnhancedInput", "OnlineSubsystem", "OnlineSubsystemNull", "OnlineSubsystemSteam" });

		PrivateDependencyModuleNames.AddRange(new string[] { });

        // Woogle's debug macros
        PublicDefinitions.Add("WG_LOG(x)=UE_LOG(LogTemp, Warning, TEXT(x));");
        PublicDefinitions.Add("WG_TEXT(x)=if(GEngine){GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Magenta, TEXT(x));}");
        PublicDefinitions.Add("WG_STRING(x)=if(GEngine){GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, x);}");
        PublicDefinitions.Add("WG_BOOL(x)=if(GEngine){GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, (x ? FString(\"true\") : FString(\"false\")));}");
        PublicDefinitions.Add("WG_INT(x)=if(GEngine){GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString::FromInt(x));}");
        PublicDefinitions.Add("WG_FLOAT(x)=if(GEngine){GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString::SanitizeFloat(x));}");
        PublicDefinitions.Add("WG_VECTOR(x)=if(GEngine){GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, x.ToString());}");
        PublicDefinitions.Add("WG_ROTATOR(x)=if(GEngine){GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, x.ToString());}");
        PublicDefinitions.Add("WG_FUNC()=if(GEngine){GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, (FString(__FUNCTION__) + TEXT(\"(\") + FString::FromInt(__LINE__) + TEXT(\")\")));}");
    }
}
