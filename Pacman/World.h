#ifndef WORLD_H
#define WORLD_H

#include <list>
#include <map>
#include "Vector2f.h"
#include "Sprite.h"
#include "Ghost.h"

class Drawer;
class PathmapTile;
class Dot;
class BigDot;
class Cherry;
class Avatar;
class Ghost;

class World
{
public:

	World(Avatar* avatar, Drawer* gameDrawer);
	~World(void);
	void Init(Drawer* gameDrawer);

	void Draw(Drawer* aDrawer);
	PathmapTile* GetTile(int aFromX, int aFromY);
	void GetPath(Ghost* ghost, int aToX, int aToY);
	bool Pathfind(PathmapTile* aFromTile, PathmapTile* aToTile, std::list<PathmapTile*>& aList);
	bool isDecisionTile(int anX, int anY);
	bool TileIsValid(int anX, int anY);

	bool HasIntersectedDot(const Vector2f& aPosition);
	bool HasIntersectedBigDot(const Vector2f& aPosition);
	bool HasIntersectedCherry(const Vector2f& aPosition);

	int GetTotalDots() { return totalDots; };
	int GetDotCount() { return myDots.size() + myBigDots.size(); };

	//for debugging
	void World::GenerateDotInPath(std::list<PathmapTile*>& aList);
	void ClearDots();

private:

	bool ListDoesNotContain(PathmapTile* aFromTile, std::list<PathmapTile*>& aList);
	bool InitPathmapAndDots(Drawer* gameDrawer);

	PathmapTile* worldMap[26][29];
	std::list<Dot*> myDots;
	std::list<BigDot*> myBigDots;
	std::list<Cherry*> myCherry;

	Drawer* myDrawer;
	Sprite* boardBackground;
	Avatar* myAvatar;

	int totalDots;

};

#endif // WORLD_H