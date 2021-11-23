#include "BigDot.h"

BigDot::BigDot(Vector2f aPosition, Sprite* entitySprite)
: StaticGameEntity(aPosition, entitySprite)
{
	sprite->SetFrame(entitySprite->getCurrentFrame());
}

BigDot::~BigDot(void)
{
}
