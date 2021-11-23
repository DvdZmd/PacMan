#ifndef GHOST_H
#define GHOST_H

#include <list>

#include "MovableGameEntity.h"
#include "ChaseBehaviour.h"
#include "ScatterBehaviour.h"
#include "FrightenedBehaviour.h"

class World;
class PathmapTile;
class Avatar;
class IChaseBehaviour;
class IScatterBehaviour;
class IFrightenedBehaviour;

class Ghost : public MovableGameEntity
{
public:
	enum GhostBehavior
	{
		Scatter,
		Chase,
		Intercept,
		Fear,
		Dead,
		Respawning
	};

	Ghost(const Vector2f& aPosition,
		Sprite* entitySprite,
		GhostBehavior behavior,
		IChaseBehaviour* chase,
		IScatterBehaviour* wander,
		IFrightenedBehaviour* frightened,
		int dotRemainingToChase);
	~Ghost(void);

	bool myIsDeadFlag;

	int myDesiredMovementX;
	int myDesiredMovementY;

	GhostBehavior myBehavior;
	IChaseBehaviour* myChase;
	IScatterBehaviour* myScatter;
	IFrightenedBehaviour* myFrightened;

	std::list<PathmapTile*>* GetPath() { return &myPath; };
	void Update(float aTime, World* aWorld, Avatar* avatar);
	void SetBehaviour(GhostBehavior behave);
	void Die(World* aWorld);


protected:

	void RandomTileInsideRespawnZone(World* aWorld);

	std::list<PathmapTile*> myPath;
	int myDotsRemainingToChase;//In percentaje

};

#endif // GHOST_H