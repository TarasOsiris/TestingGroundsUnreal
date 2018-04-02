// Fill out your copyright notice in the Description page of Project Settings.

#include "ChooseNextWaypoint.h"
#include "../Public/ChooseNextWaypoint.h"
#include "PatrolRouteComponent.h"
#include "Runtime/AIModule/Classes/BehaviorTree/BehaviorTreeComponent.h"
#include "Runtime/AIModule/Classes/AIController.h"
#include "Runtime/AIModule/Classes/BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UChooseNextWaypoint::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	// Get patrol route
	AAIController* AIController = OwnerComp.GetAIOwner();
	APawn* ControlledPawn = AIController->GetPawn();
	UPatrolRouteComponent *PatrolRouteComponent = ControlledPawn->FindComponentByClass<UPatrolRouteComponent>();

	if (!ensure(PatrolRouteComponent))
	{
		return EBTNodeResult::Failed;
	}

	TArray<AActor*> PatrolPoints = PatrolRouteComponent->GetPatrolPoints();
	if (PatrolPoints.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Patrol Points array is empty"));
		return EBTNodeResult::Failed;
	}

	// Set next waypoint
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	uint32 Index = BlackboardComp->GetValueAsInt(IndexKey.SelectedKeyName);
	AActor *NextWaypoint = PatrolPoints[Index];
	BlackboardComp->SetValueAsObject(WaypointKey.SelectedKeyName, NextWaypoint);

	// Cycle waypoints
	uint32 NextWaypointIndex = ++Index % PatrolPoints.Num();
	BlackboardComp->SetValueAsInt(IndexKey.SelectedKeyName, NextWaypointIndex);

	return EBTNodeResult::Succeeded;
}
