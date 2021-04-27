//$ Copyright 2015-21, Code Respawn Technologies Pvt Ltd - All Rights Reserved $//

#include "Visualizers/ActorComponentVisualizer.h"

#include "GameFramework/Actor.h"
#include "SceneManagement.h"

void FActorComponentVisualizer::DrawVisualization(const UActorComponent* Component, const FSceneView* View, FPrimitiveDrawInterface* PDI)
{
	if (!Component) return;
	
	AActor* Parent = Component->GetOwner();
	if (!Parent) return;

	FBox Bounds = GetBounds(Parent);
	Bounds = Bounds.ExpandBy(2);

	const float Thickness = 0;
	const FMatrix LocalToWorld = FMatrix::Identity;
	DrawWireBox(PDI, LocalToWorld, Bounds, FLinearColor::Red, SDPG_Foreground, Thickness);
}

void FActorComponentVisualizer::DrawVisualizationHUD(const UActorComponent* Component, const FViewport* Viewport, const FSceneView* View, FCanvas* Canvas) {

	FComponentVisualizer::DrawVisualizationHUD(Component, Viewport, View, Canvas);

	if (!Component) return;

	AActor* Parent = Component->GetOwner();
	if (!Parent) return;	

	UFont* RenderFont = GEngine->GetSmallFont();

	FBox Bounds = GetBounds(Parent);

	FVector drawPosX = Bounds.GetCenter() + FVector(0, -Bounds.GetExtent().Y, Bounds.GetExtent().Z);
	FVector2D PixelLocationX;
	View->ScreenToPixel(View->WorldToScreen(drawPosX), PixelLocationX);
	Canvas->DrawShadowedText(PixelLocationX.X, PixelLocationX.Y, FText::FromString("X: " + GetDistanceText(Bounds.GetSize().X, "")), RenderFont, FColor::Red);

	FVector drawPosY = Bounds.GetCenter() + FVector(-Bounds.GetExtent().X, 0, Bounds.GetExtent().Z);
	FVector2D PixelLocationY;
	View->ScreenToPixel(View->WorldToScreen(drawPosY), PixelLocationY);
	Canvas->DrawShadowedText(PixelLocationY.X, PixelLocationY.Y, FText::FromString("Y: " + GetDistanceText(Bounds.GetSize().Y, "")), RenderFont, FColor::Green);

	FVector drawPosZ = Bounds.GetCenter() + FVector(-Bounds.GetExtent().X, -Bounds.GetExtent().Y, 0);
	FVector2D PixelLocationZ;
	View->ScreenToPixel(View->WorldToScreen(drawPosZ), PixelLocationZ);
	Canvas->DrawShadowedText(PixelLocationZ.X, PixelLocationZ.Y, FText::FromString("Z: " + GetDistanceText(Bounds.GetSize().Z, "")), RenderFont, FColor::Blue);
}

FString FActorComponentVisualizer::GetDistanceText(float DistanceInCM, FName Name)
{
	FString res;
	float ResDistance = DistanceInCM;
	res = "m";
	ResDistance = DistanceInCM / 100.f;

	res = FString::Printf(TEXT("%.3f"), ResDistance) + res;
	if (Name != NAME_None)
		res += " [" + Name.ToString() + "]";

	return res;
}

namespace {

	void GetBoundsRecursive(AActor* InActor, FBox& OutBounds, bool bNonColliding, const TSet<UClass*>& IgnoreActorClasses) {
		if (InActor && InActor->IsLevelBoundsRelevant()) {
			const bool bIgnoreBounds = IgnoreActorClasses.Contains(InActor->GetClass());
			if (!bIgnoreBounds) {
				FBox ActorBounds(ForceInit);
				for (const UActorComponent* ActorComponent : InActor->GetComponents()) {
					if (const UPrimitiveComponent* InPrimComp = Cast<UPrimitiveComponent>(ActorComponent)) {
						if (!IgnoreActorClasses.Contains(InPrimComp->GetClass())) {
							if (InPrimComp->IsRegistered() && (bNonColliding || InPrimComp->IsCollisionEnabled())) {
								ActorBounds += InPrimComp->Bounds.GetBox();
							}
						}
					}
				}

				if (ActorBounds.GetExtent() == FVector::ZeroVector) {
					ActorBounds = FBox({ InActor->GetActorLocation() });
				}
				OutBounds += ActorBounds;
			}

			TArray<AActor*> AttachedActors;
			InActor->GetAttachedActors(AttachedActors);
			for (AActor* AttachedActor : AttachedActors) {
				GetBoundsRecursive(AttachedActor, OutBounds, bNonColliding, IgnoreActorClasses);
			}
		}
	}

	void DestroyActorTree(AActor* InActor) {
		if (!InActor) return;
		TArray<AActor*> Children;
		InActor->GetAttachedActors(Children);

		for (AActor* Child : Children) {
			DestroyActorTree(Child);
		}

		InActor->Destroy();
	}
}


FBox FActorComponentVisualizer::GetBounds(AActor* PrefabActor, bool bNonColliding)
{
	FBox Result(EForceInit::ForceInit);
	TSet<UClass*> IgnoreBoundingBoxForObjects;
	GetBoundsRecursive(PrefabActor, Result, bNonColliding, IgnoreBoundingBoxForObjects);
	return Result;
}