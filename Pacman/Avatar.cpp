#include "Avatar.h"
#include <iostream>

Avatar::Avatar(const Vector2f& aPosition, Sprite* entitySprite)
	: MovableGameEntity(aPosition, entitySprite)
{
	SetSpeed(100.f);
	sprite->SetFrame("openLeft");
}

Avatar::~Avatar(void)
{
}

void Avatar::Update(float aTime)
{
	float distanceToMove = aTime * speed;
	int tileSize = 22;
	myBehaviourEmpleacedTime += aTime;

	Vector2f destination(myNextTileX * tileSize, myNextTileY * tileSize);
	Vector2f direction = destination - myPosition;


	if (distanceToMove > direction.Length() ||
		(myCurrentTileX == 0 && myNextTileX == 25) ||//This is for handle the circular way in the middle
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

		//This is used for animate the open/close avatar's movement
		if (myBehaviourEmpleacedTime > 0.1f)
		{
			SetMovementFrame(direction);
			myBehaviourEmpleacedTime = 0.f;
		}
	}
}


void Avatar::SetMovementFrame(Vector2f direction)
{
	if (direction.myX == 0 && direction.myY == 0)
	{//Avatars is not moving
	}
	else if (direction.myX == 0)
	{//Avatar is moveing on Y axis, the calculate if up or down
		if (direction.myY > 0)//going down
		{
			movementDirection = MovableGameEntity::MovementDirection::Down;
			if (sprite->getCurrentFrame() == "closedDown")
				sprite->SetFrame("openDown");
			else
				sprite->SetFrame("closedDown");
		}
		else if (direction.myY < 0)//going up
		{
			movementDirection = MovableGameEntity::MovementDirection::Up;
			if (sprite->getCurrentFrame() == "closedUp")
				sprite->SetFrame("openUp");
			else
				sprite->SetFrame("closedUp");
		}
	}
	else if (direction.myY == 0)
	{//Avatar is moveing on s axis, the calculate if left or right
		if (direction.myX > 0)
		{
			movementDirection = MovableGameEntity::MovementDirection::Right;
			if (sprite->getCurrentFrame() == "closedRight")
				sprite->SetFrame("openRight");
			else
				sprite->SetFrame("closedRight");
		}
		else if (direction.myX < 0)
		{
			movementDirection = MovableGameEntity::MovementDirection::Left;
			if (sprite->getCurrentFrame() == "closedLeft")
				sprite->SetFrame("openLeft");
			else
				sprite->SetFrame("closedLeft");
		}
	}
}
