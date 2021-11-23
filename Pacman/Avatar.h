#ifndef AVATAR_H
#define AVATAR_H

#include "MovableGameEntity.h"
#include "Vector2f.h"

class Avatar : public MovableGameEntity
{
public:
	Avatar(const Vector2f& aPosition, Sprite* entitySprite);
	~Avatar(void);
	void Update(float aTime);

	MovementDirection GetMovementDirection() { return movementDirection; };

private:

	void SetMovementFrame(Vector2f direction);
	MovementDirection movementDirection;

};

#endif //AVATAR_H