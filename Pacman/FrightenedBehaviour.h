#ifndef FRIGHTENEDBEHAVIOUR_H
#define FRIGHTENEDBEHAVIOUR_H

#include "World.h"
#include "PathmapTile.h"
#include "Avatar.h"
#include "Ghost.h"
#include "World.h"


class Ghost;
class World;

/// <summary>
/// Interface for implement strategy pattern in the ghosts behaviour
/// </summary>
class IFrightenedBehaviour
{
public:
	virtual void Frightened(Ghost* ghost, World* aWorld) = 0;
	void PathToRespawn(World* aWorld, Ghost* ghost);
};

class FrigtenedWander : public IFrightenedBehaviour
{
public:
	void Frightened(Ghost* ghost, World* aWorld);
	void RandomMove(Ghost* ghost);
};

#endif //SCATTER_H