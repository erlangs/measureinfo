// Copyright Epic Games, Inc. All Rights Reserved.

#include "MeasureInfo.h"
#include "Editor/UnrealEdEngine.h"
#include "UnrealEdGlobals.h"
#include "Visualizers/ActorComponentVisualizer.h"

#define LOCTEXT_NAMESPACE "FMeasureInfoModule"

class FMeasureInfoModule : public IMeasureInfoModule
{
	virtual void StartupModule() override
	{
		// Register component visualizers
		RegisterComponentVisualizers();
	}

	virtual void ShutdownModule() override {
		UnregisterComponentVisualizers();

	}

private:
	void RegisterComponentVisualizers() {
		if (GUnrealEd) {
			StaticMeshComponentClassName = UStaticMeshComponent::StaticClass()->GetFName();
			SkeletalMeshComponentClassName = USkeletalMeshComponent::StaticClass()->GetFName();
			

			RegisterVisualizer(*GUnrealEd, StaticMeshComponentClassName, MakeShared<FActorComponentVisualizer>());
			RegisterVisualizer(*GUnrealEd, SkeletalMeshComponentClassName, MakeShared<FActorComponentVisualizer>());
		}
	}

	void RegisterVisualizer(UUnrealEdEngine& UnrealEdEngine, const FName& ComponentClassName, const TSharedRef<FComponentVisualizer>& Visualizer)
	{
		UnrealEdEngine.RegisterComponentVisualizer(ComponentClassName, Visualizer);
		Visualizer->OnRegister();
	}

	void UnregisterComponentVisualizers() {
		if (GUnrealEd) {
			GUnrealEd->UnregisterComponentVisualizer(StaticMeshComponentClassName);
			GUnrealEd->UnregisterComponentVisualizer(SkeletalMeshComponentClassName);
		}
	}

	FName StaticMeshComponentClassName;
	FName SkeletalMeshComponentClassName;
};

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FMeasureInfoModule, MeasureInfo)