#include "Ghost.h"
#include "World.h"
#include "PathmapTile.h"
#include "Drawer.h"
#include "Avatar.h"
#include <chrono>
#include <iostream>


Ghost::Ghost(const Vector2f& aPosition,
	Sprite* entitySprite,
	GhostBehavior behavior,
	IChaseBehaviour* chase,
	IScatterBehaviour* wander,
	IFrightenedBehaviour* frightened,
	int DotsRemainingToChase)
	: MovableGameEntity(aPosition, entitySprite),
	myBehavior(behavior),
	myChase(chase),
	myScatter(wander),
	myFrightened(frightened),
	myDotsRemainingToChase(DotsRemainingToChase)
{
	sprite->SetFrame("normal");
	myIsDeadFlag = false;

	myDesiredMovementX = 0;
	myDesiredMovementY = -1;

	speed = 110.f;
	myBehaviourEmpleacedTime = 0;
}


Ghost::~Ghost(void)
{
}

void Ghost::RandomTileInsideRespawnZone(World* aWorld)
{//11,12 16,12   11,14 16,14
	bool isDiferent = false;
	PathmapTile* tile = nullptr;

	int x = rand() % (15 - 10 + 1) + 10;
	int y = rand() % (14 - 12 + 1) + 12;

	for (int i = 10; i <= 15; i++)
		for (int j = 12; j <= 14; j++)
		{
			tile = aWorld->GetTile(i, j);
			tile->myIsVisitedFlag = false;
		}

	aWorld->Pathfind(aWorld->GetTile(myCurrentTileX, myCurrentTileY), aWorld->GetTile(x, y), *(GetPath()));

	return;
}


void Ghost::Die(World* aWorld)
{
	SetBehaviour(Dead);

	PathmapTile* tile = nullptr;
	for (int i = 0; i < 26; i++)
	{
		for (int j = 0; j < 29; j++)
		{
			tile = aWorld->GetTile(i, j);
			tile->myIsVisitedFlag = false;
		}
	}

	//Set a path from inside the respawn zone to ghost and the made the reverse way
	aWorld->Pathfind(aWorld->GetTile(12, 13), aWorld->GetTile(myCurrentTileX, myCurrentTileY), *(GetPath()));
}

/// <summary>
/// Sets the new behaviour and their variables
/// </summary>
/// <param name="behave"></param>
void Ghost::SetBehaviour(GhostBehavior behave)
{
	myPath.clear();

	myBehavior = behave;
	if (myBehavior == Fear)
	{
		sprite->SetFrame("vulnerable");
		speed = 50.f;
	}
	else if (myBehavior == Chase)
	{
		sprite->SetFrame("normal");
		speed = 110.f;
	}
	else if (myBehavior == Dead)
	{
		sprite->SetFrame("dead");
		myIsDeadFlag = true;
		speed = 120.f;
	}

	myBehaviourEmpleacedTime = 0.f;
}

void Ghost::Update(float aTime, World* aWorld, Avatar* avatar)
{
	try
	{
		myBehaviourEmpleacedTime += aTime;

		//Check remaining dots in percentage, each ghost leave the respawn zone with different quantities
		float dotsRemaining = (aWorld->GetDotCount() * 100) / aWorld->GetTotalDots();

		//Change the behaviour if needed
		if (myBehavior == Respawning && myDotsRemainingToChase >= dotsRemaining)
			SetBehaviour(Chase);
		if (myBehavior == Chase && myBehaviourEmpleacedTime >= 20.f)
			SetBehaviour(Scatter);
		else if (myBehavior == Scatter && myBehaviourEmpleacedTime >= 4.f)
			SetBehaviour(Chase);
		else if (myBehavior == Fear && myBehaviourEmpleacedTime >= 5)
			SetBehaviour(Chase);
		else if (myBehavior == Dead && myBehaviourEmpleacedTime >= 10)
		{
			myIsDeadFlag = false;
			SetBehaviour(Chase);
		}

		//Behave 
		if (IsAtDestination())
		{
			switch (myBehavior)
			{
			case Scatter:
				if (myPath.empty())
					myScatter->Scatter(aWorld, avatar, this);

				break;
			case Chase:
				if (aWorld->isDecisionTile(myCurrentTileX, myCurrentTileY) || myPath.empty())
					myChase->chase(aWorld, avatar, this);

				break;
			case Fear:
				myFrightened->Frightened(this, aWorld);

				break;
			case Respawning:
			case Dead:
				if (myPath.empty())
					RandomTileInsideRespawnZone(aWorld);

				break;
			default:
				myFrightened->Frightened(this, aWorld);

				break;
			}

			//This is to get at least 1 element in path
			if (myPath.empty())
				myFrightened->Frightened(this, aWorld);

			PathmapTile* nextTile = nullptr;
			if (myBehavior == Dead)
			{//If is dead the path is calculated from the respawn zone to the ghost and then make a revert way
				nextTile = myPath.back();
				myPath.pop_back();
			}
			else
			{
				nextTile = myPath.front();
				myPath.pop_front();
			}
			SetNextTile(nextTile->myX, nextTile->myY);
		}

		static int tileSize = 22;
		Vector2f destination(myNextTileX * tileSize, myNextTileY * tileSize);
		Vector2f direction = destination - myPosition;
		float distanceToMove = aTime * speed;

		if (distanceToMove > direction.Length() ||
			(myCurrentTileX == 0 && myNextTileX == 25) ||
			(myCurrentTileX == 25 && myNextTileX == 0))
		{
			myPosition = destination;

			myPreviousTileX = myCurrentTileX;
			myPreviousTileY = myCurrentTileY;

			myCurrentTileX = myNextTileX;
			myCurrentTileY = myNextTileY;
		}
		else
		{
			direction.Normalize();
			myPosition += direction * distanceToMove;
		}
	}
	catch (...)
	{
		const char* error = SDL_GetError();
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
	}
}
