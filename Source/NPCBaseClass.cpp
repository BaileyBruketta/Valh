// Fill out your copyright notice in the Description page of Project Settings.


#include "NPCBaseClass.h"
//#include "MyNPCHandler.h"

void ANPCBaseClass::ChangeMesh(USkeletalMesh* MeshIn)
{
	MeshBody->SetSkeletalMesh(MeshIn);
}

void ANPCBaseClass::SABP(UAnimBlueprint* animbp)
{
	MeshBody->SetAnimClass(animbp->GetAnimBlueprintGeneratedClass());
}

//void ANPCBaseClass::OnSpawn2(AMyNPCHandler* theOne)
//{
//	theHandler2 = theOne;
//	AMyNPCHandler* kit = theOne;
//	theHandler2 = kit;
//
//}

void ANPCBaseClass::OnSpawn(AenemyHandler* theOne)
{
	theHandler = theOne;
	AenemyHandler* kit = theOne;
	theHandler = kit;

}