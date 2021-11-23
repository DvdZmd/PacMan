#include "ChaseBehaviour.h"
#include <iostream>

/// <summary>
/// Chase implementation for Blinky, its follows avatar directly behind
/// </summary>
/// <param name="aWorld"></param>
/// <param name="avatar"></param>
/// <param name="ghost"></param>
void ChaseAggresive::chase(World* aWorld, Avatar* avatar, Ghost* ghost)
{
	ghost->GetPath()->clear();
	aWorld->GetPath(ghost, avatar->GetCurrentTileX(), avatar->GetCurrentTileY());
}

/// <summary>
/// Set the ghost's objective 4 tiles ahead the avatar to ambush him
/// </summary>
/// <param name="aWorld"></param>
/// <param name="avatar"></param>
/// <param name="ghost"></param>
void ChaseAmbush::chase(World* aWorld, Avatar* avatar, Ghost* ghost)
{
	//aWorld->ClearDots();

	ghost->GetPath()->clear();

	//calculate movement direction
	int ambushTileX{};
	int ambushTileY{};

	if (avatar->GetMovementDirection() == MovableGameEntity::MovementDirection::Up)
	{
		ambushTileY = avatar->GetCurrentTileY() - 4;
		ambushTileX = avatar->GetCurrentTileX();
	}
	else if (avatar->GetMovementDirection() == MovableGameEntity::MovementDirection::Down)
	{
		ambushTileY = avatar->GetCurrentTileY() + 4;
		ambushTileX = avatar->GetCurrentTileX();
	}
	else if (avatar->GetMovementDirection() == MovableGameEntity::MovementDirection::Left)
	{
		ambushTileY = avatar->GetCurrentTileY();
		ambushTileX = avatar->GetCurrentTileX() - 4;
	}
	else if (avatar->GetMovementDirection() == MovableGameEntity::MovementDirection::Right)
	{
		ambushTileY = avatar->GetCurrentTileY();
		ambushTileX = avatar->GetCurrentTileX() + 4;
	}

	//Set ghost path

	//If tile is outside the ranges then chase avatar directly
	if (aWorld->TileIsValid(ambushTileX, ambushTileY))
		aWorld->GetPath(ghost, ambushTileX, ambushTileY);
	else
		aWorld->GetPath(ghost, avatar->GetCurrentTileX(), avatar->GetCurrentTileY());


}

/// <summary>
/// This calculates tile destination duplicating the distance between Blinky and 4 tiles ahead avatars
/// </summary>
/// <param name="aWorld"></param>
/// <param name="avatar"></param>
/// <param name="ghost"></param>
void ChasePatrol::chase(World* aWorld, Avatar* avatar, Ghost* ghost)
{

	ghost->GetPath()->clear();

	//calculate movement direction
	int ambushTileX{};
	int ambushTileY{};

	if (avatar->GetMovementDirection() == MovableGameEntity::MovementDirection::Up)
	{
		ambushTileY = avatar->GetCurrentTileY() - 4;
		ambushTileX = avatar->GetCurrentTileX();
	}
	else if (avatar->GetMovementDirection() == MovableGameEntity::MovementDirection::Down)
	{
		ambushTileY = avatar->GetCurrentTileY() + 4;
		ambushTileX = avatar->GetCurrentTileX();
	}
	else if (avatar->GetMovementDirection() == MovableGameEntity::MovementDirection::Left)
	{
		ambushTileY = avatar->GetCurrentTileY();
		ambushTileX = avatar->GetCurrentTileX() - 4;
	}
	else if (avatar->GetMovementDirection() == MovableGameEntity::MovementDirection::Right)
	{
		ambushTileY = avatar->GetCurrentTileY();
		ambushTileX = avatar->GetCurrentTileX() + 4;
	}

	Vector2f ambushPosition(ambushTileX * 22, ambushTileY * 22);//First build a vector to the ambush tile
	Vector2f blinkysPosition = blinky->GetPosition();//then gets Blinky's vector position
	Vector2f destination = (ambushPosition - blinkysPosition) * 2;//Now build a new vector from Blinky's ambush tile to ambush tile and duplicate

	//If tile is outside the ranges then chase avatar directly
	if (aWorld->TileIsValid(destination.myX / 22, destination.myY / 22))
		aWorld->GetPath(ghost, destination.myX / 22, destination.myY / 22);
	else
		aWorld->GetPath(ghost, avatar->GetCurrentTileX(), avatar->GetCurrentTileY());

}

/// <summary>
/// Chase directly but when he too near, 8 tiles, he will scatter
/// </summary>
/// <param name="aWorld"></param>
/// <param name="avatar"></param>
/// <param name="ghost"></param>
void ChaseRandom::chase(World* aWorld, Avatar* avatar, Ghost* ghost)
{
	ghost->GetPath()->clear();

	Vector2f avatarPosition = avatar->GetPosition();
	Vector2f ghostPosition = ghost->GetPosition();

	Vector2f distance = avatarPosition - ghostPosition;


	if ((distance.Length() / 22) > 8)
		aWorld->GetPath(ghost, avatar->GetCurrentTileX(), avatar->GetCurrentTileY());
	else
		ghost->myScatter->Scatter(aWorld, avatar, ghost);
}