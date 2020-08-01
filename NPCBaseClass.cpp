// Fill out your copyright notice in the Description page of Project Settings.


#include "NPCBaseClass.h"

void ANPCBaseClass::ChangeMesh(USkeletalMesh* MeshIn)
{
	MeshBody->SetSkeletalMesh(MeshIn);
}

void ANPCBaseClass::OnSpawn(AenemyHandler* theOne)
{
	theHandler = theOne;
	AenemyHandler* kit = theOne;
	theHandler = kit;

}