#ifndef CHASEBEHAVIOUR_H
#define CHASEBEHAVIOUR_H

#include "PathmapTile.h"
#include "Avatar.h"
#include "Ghost.h"

class Ghost;
class World;

/// <summary>
/// Interface for implement strategy pattern in the ghosts behaviour
/// </summary>
class IChaseBehaviour
{
public:
	virtual void chase(World* aWorld,
		Avatar* avatar,
		Ghost* ghost) = 0;

};

class ChaseAggresive : public IChaseBehaviour
{
public:
	void chase(World* aWorld,
		Avatar* avatar,
		Ghost* ghost);
};

class ChaseAmbush : public IChaseBehaviour
{
public:
	void chase(World* aWorld,
		Avatar* avatar,
		Ghost* ghost);
};

class ChasePatrol : public IChaseBehaviour
{
public:
	ChasePatrol(Ghost* ghost) { blinky = ghost; };
	~ChasePatrol();
	void chase(World* aWorld,
		Avatar* avatar,
		Ghost* ghost);

	void setBlinky(Ghost* ghost) {
		blinky = ghost;
	};
private:
	Ghost* blinky;
};

class ChaseRandom : public IChaseBehaviour
{
public:
	void chase(World* aWorld,
		Avatar* avatar,
		Ghost* ghost);
};


#endif 