#ifndef SCATTERBEHAVIOUR_H
#define SCATTERBEHAVIOUR_H

#include "World.h"
#include "PathmapTile.h"
#include "Avatar.h"
#include "Ghost.h"
#include "World.h"

class Ghost;
class World;

/// <summary>
/// Interface implemented by Ghost for apply Strategy Pattern on in their behaviours
/// </summary>
class IScatterBehaviour
{
public:
	virtual void Scatter(World* aWorld, Avatar* avatar, Ghost* ghost) = 0;
};

/// <summary>
/// Used for Blinky
/// </summary>
class ScatterUpRight : public IScatterBehaviour
{
public:
	void Scatter(World* aWorld, Avatar* avatar, Ghost* ghost);
};

/// <summary>
/// Used for Pinky
/// </summary>
class ScatterUpLeft : public IScatterBehaviour
{
public:
	void Scatter(World* aWorld, Avatar* avatar, Ghost* ghost);
};


/// <summary>
/// Used for Clyde
/// </summary>
class ScatterDownLeft : public IScatterBehaviour
{
public:
	void Scatter(World* aWorld, Avatar* avatar, Ghost* ghost);
};

/// <summary>
/// Used for Inky
/// </summary>
class ScatterDownRight : public IScatterBehaviour
{
public:
	void Scatter(World* aWorld, Avatar* avatar, Ghost* ghost);
};

#endif //SCATTER_H