// Fill out your copyright notice in the Description page of Project Settings.

#include "ChooseNextWaypoint.h"
#include "../Public/ChooseNextWaypoint.h"
#include "../Public/PatrollingGuard.h"
#include "Runtime/AIModule/Classes/BehaviorTree/BehaviorTreeComponent.h"
#include "Runtime/AIModule/Classes/AIController.h"
#include "Runtime/AIModule/Classes/BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UChooseNextWaypoint::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	// Get patrol points
	AAIController* AIController = OwnerComp.GetAIOwner();
	APawn* ControlledPawn = AIController->GetPawn();
	APatrollingGuard* Guard = Cast<APatrollingGuard>(ControlledPawn);
	TArray<AActor*> PatrolPoints = Guard->PatrolPointsCPP;

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
