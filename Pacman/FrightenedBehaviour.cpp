#include "FrightenedBehaviour.h"
#include <iostream>


void IFrightenedBehaviour::PathToRespawn(World* aWorld, Ghost* ghost)
{
	ghost->GetPath()->clear();
	int x = rand() % (16 - 11 + 1) + 11;
	int y = rand() % (14 - 12 + 1) + 12;

	aWorld->GetPath(ghost, x, y);
}

//Random move when avatar eats a big dot
void FrigtenedWander::Frightened(Ghost* ghost, World* aWorld)
{
	ghost->GetPath()->clear();
	int cx = ghost->GetCurrentTileX();
	int cy = ghost->GetCurrentTileY();

	do
	{
		RandomMove(ghost);

	} while (!aWorld->TileIsValid(cx + ghost->myDesiredMovementX, cy + ghost->myDesiredMovementY));
	ghost->GetPath()->push_front(aWorld->GetTile(cx + ghost->myDesiredMovementX, cy + ghost->myDesiredMovementY));
}

void FrigtenedWander::RandomMove(Ghost* ghost)
{
	MovableGameEntity::MovementDirection nextDirection = (MovableGameEntity::MovementDirection)(rand() % MovableGameEntity::MovementDirection::DirectionCount);

	switch (nextDirection)
	{
	case MovableGameEntity::MovementDirection::Up:
		ghost->myDesiredMovementX = 0;
		ghost->myDesiredMovementY = 1;
		break;
	case MovableGameEntity::MovementDirection::Down:
		ghost->myDesiredMovementX = 0;
		ghost->myDesiredMovementY = -1;
		break;
	case MovableGameEntity::MovementDirection::Left:
		ghost->myDesiredMovementX = -1;
		ghost->myDesiredMovementY = 0;
		break;
	case MovableGameEntity::MovementDirection::Right:
		ghost->myDesiredMovementX = 1;
		ghost->myDesiredMovementY = 0;
		break;
	default:
		break;
	}
}