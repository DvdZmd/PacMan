#include "World.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>

#include "PathmapTile.h"
#include "Dot.h"
#include "BigDot.h"
#include "Drawer.h"
#include "Avatar.h"
#include "Pacman.h"

World::World(Avatar* avatar, Drawer* gameDrawer)
{
	myDrawer = gameDrawer;
	myAvatar = avatar;
}

World::~World(void)
{
}

void World::Init(Drawer* gameDrawer)
{
	std::map<std::string, std::string> assetPaths;
	assetPaths["playfield"] = "playfield.png";
	boardBackground = Sprite::Create(assetPaths, gameDrawer, 1024, 768);
	boardBackground->SetFrame("playfield");
	InitPathmapAndDots(gameDrawer);
}

bool World::isDecisionTile(int x, int y)
{
	return GetTile(x, y)->isDecisionFlag;
}

/// <summary>
/// This reads the ascii file representing the map while create the 
/// several objects like walls, dots and decision points.
/// </summary>
/// <param name="gameDrawer"></param>
/// <returns></returns>
bool World::InitPathmapAndDots(Drawer* gameDrawer)
{
	Sprite* newSprite = NULL;
	std::map<std::string, std::string> assetPaths;

	std::string line;
	std::ifstream myfile("map.txt");
	try {
		if (myfile.is_open())
		{
			int lineIndex = 0;
			while (!myfile.eof())
			{
				std::getline(myfile, line);

				for (unsigned int i = 0; i < line.length(); i++)
				{
					PathmapTile* tile = new PathmapTile(i, lineIndex, (line[i] == 'x'));

					worldMap[tile->myX][tile->myY] = tile;

					if (line[i] == 'T')
						tile->isDecisionFlag = true;
					else if (line[i] == '.' || line[i] == 'D')
					{
						if (line[i] == 'D')
							tile->isDecisionFlag = true;

						assetPaths.clear();
						assetPaths["smallDot"] = "Small_Dot_32.png";
						newSprite = Sprite::Create(assetPaths, gameDrawer, 32, 32);
						newSprite->SetFrame("smallDot");
						Dot* dot = new Dot(Vector2f(i * 22, lineIndex * 22), newSprite);
						myDots.push_back(dot);
					}
					else if (line[i] == 'o')
					{
						assetPaths.clear();
						assetPaths["bigDot"] = "Big_Dot_32.png";
						newSprite = Sprite::Create(assetPaths, gameDrawer, 32, 32);
						newSprite->SetFrame("bigDot");
						BigDot* dot = new BigDot(Vector2f(i * 22, lineIndex * 22), newSprite);
						myBigDots.push_back(dot);
					}
				}

				lineIndex++;
			}
			totalDots = myDots.size() + myBigDots.size();
			myfile.close();
		}
	}
	catch (...) {
		cout << "Error.\n";
		return false;
	}

	return true;
}

void World::Draw(Drawer* aDrawer)
{
	boardBackground->Draw(aDrawer, 0, 0);

	for (std::list<Dot*>::iterator list_iter = myDots.begin(); list_iter != myDots.end(); list_iter++)
	{
		Dot* dot = *list_iter;
		dot->Draw(aDrawer);
	}

	for (std::list<BigDot*>::iterator list_iter = myBigDots.begin(); list_iter != myBigDots.end(); list_iter++)
	{
		BigDot* dot = *list_iter;
		dot->Draw(aDrawer);
	}
}

bool World::TileIsValid(int anX, int anY)
{
	if (anX < 0 || anX >= 26)
		return false;
	if (anY < 0 || anY >= 29)
		return false;


	return !worldMap[anX][anY]->myIsBlockingFlag;
}

bool World::HasIntersectedDot(const Vector2f& aPosition)
{
	for (std::list<Dot*>::iterator list_iter = myDots.begin(); list_iter != myDots.end(); list_iter++)
	{
		Dot* dot = *list_iter;
		if ((dot->GetPosition() - aPosition).Length() < 5.f)
		{
			myDots.remove(dot);
			delete dot;
			return true;
		}
	}

	return false;
}

bool World::HasIntersectedBigDot(const Vector2f& aPosition)
{
	for (std::list<BigDot*>::iterator list_iter = myBigDots.begin(); list_iter != myBigDots.end(); list_iter++)
	{
		BigDot* dot = *list_iter;
		if ((dot->GetPosition() - aPosition).Length() < 5.f)
		{
			myBigDots.remove(dot);
			delete dot;
			return true;
		}
	}

	return false;
}

bool World::HasIntersectedCherry(const Vector2f& aPosition)
{
	return true;
}

/// <summary>
/// First blank all myIsVisitedFlag from the myPathmapTiles list, also set as myIsBlockingFlag 
/// the previous ghost tile. Then call Pathfind for set the Ghost path, 
/// finish returing the previous ghost tile as a non bloking tile
/// </summary>
/// <param name="ghost"></param>
/// <param name="aToX"></param>
/// <param name="aToY"></param>
void World::GetPath(Ghost* ghost, int aToX, int aToY)
{
	PathmapTile* tile = nullptr;

	PathmapTile* fromTile = GetTile(ghost->GetCurrentTileX(), ghost->GetCurrentTileY());
	PathmapTile* toTile = GetTile(aToX, aToY);

	PathmapTile* previuosTile = nullptr;

	//Blank all tiles for build the new path
	for (int i = 0; i < 26; i++)
		for (int j = 0; j < 29; j++)
		{
			tile = worldMap[i][j];
			tile->myIsVisitedFlag = false;
		}

	//In chasing ghosts con not turn back
	if (!ghost->myIsDeadFlag)
	{
		tile = GetTile(ghost->GetPreviousTileX(), ghost->GetPreviousTileY());
		tile->myIsBlockingFlag = true;
		previuosTile = tile;
	}

	//Set the ghost path
	Pathfind(fromTile, toTile, *(ghost->GetPath()));

	//restore the previous tile if needed
	if (previuosTile != nullptr)
		previuosTile->myIsBlockingFlag = false;

}

void World::ClearDots()
{
	myDots.clear();
}



/// <summary>
/// Check if the values are between the valid range, if over, 
/// they will be settled by the minimal o maximus values.
/// </summary>
/// <param name="aFromX"></param>
/// <param name="aFromY"></param>
/// <returns></returns>
PathmapTile* World::GetTile(int aFromX, int aFromY)
{
	if (aFromX < 0)
		aFromX = 0;
	if (aFromX >= 26)
		aFromX = 25;

	if (aFromY < 0)
		aFromY = 0;
	if (aFromY >= 29)
		aFromY = 28;

	return worldMap[aFromX][aFromY];
}

bool World::ListDoesNotContain(PathmapTile* aFromTile, std::list<PathmapTile*>& aList)
{
	for (std::list<PathmapTile*>::iterator list_iter = aList.begin(); list_iter != aList.end(); list_iter++)
	{
		PathmapTile* tile = *list_iter;
		if (tile == aFromTile)
		{
			return false;
		}
	}

	return true;
}


/// <summary>
/// Sets the tiles path list from a tile to another. 
/// Is a recursive process that finish when find 
/// the objective tile or end the list to analice.
/// </summary>
/// <param name="aFromTile"></param>
/// <param name="aToTile"></param>
/// <param name="aList"></param>
/// <returns></returns>
bool World::Pathfind(PathmapTile* aFromTile, PathmapTile* aToTile, std::list<PathmapTile*>& aList)
{
	try
	{
		aFromTile->myIsVisitedFlag = true;

		if (aFromTile->myIsBlockingFlag)
			return false;

		if (aFromTile == aToTile)
			return true;

		std::list<PathmapTile*> neighborList;

		PathmapTile* up = GetTile(aFromTile->myX, aFromTile->myY - 1);
		if (up != nullptr)
			if (up && !up->myIsVisitedFlag && !up->myIsBlockingFlag && ListDoesNotContain(up, aList))
				neighborList.push_front(up);

		PathmapTile* down = GetTile(aFromTile->myX, aFromTile->myY + 1);
		if (down != nullptr)
			if (down && !down->myIsVisitedFlag && !down->myIsBlockingFlag && ListDoesNotContain(down, aList))
				neighborList.push_front(down);

		PathmapTile* right = GetTile(aFromTile->myX + 1, aFromTile->myY);
		if (right != nullptr)
			if (right && !right->myIsVisitedFlag && !right->myIsBlockingFlag && ListDoesNotContain(right, aList))
				neighborList.push_front(right);

		PathmapTile* left = GetTile(aFromTile->myX - 1, aFromTile->myY);
		if (left != nullptr)
			if (left && !left->myIsVisitedFlag && !left->myIsBlockingFlag && ListDoesNotContain(left, aList))
				neighborList.push_front(left);

		neighborList.sort([aToTile](const PathmapTile* a, const PathmapTile* b)
			{
				int la = abs(a->myX - aToTile->myX) + abs(a->myY - aToTile->myY);
				int lb = abs(b->myX - aToTile->myX) + abs(b->myY - aToTile->myY);

				return la < lb;
			});

		for (std::list<PathmapTile*>::iterator list_iter = neighborList.begin(); list_iter != neighborList.end(); list_iter++)
		{
			PathmapTile* tile = *list_iter;

			aList.push_back(tile);

			if (Pathfind(tile, aToTile, aList))
				return true;

			aList.pop_back();
		}

		return false;
	}
	catch (...)
	{
		const char* error = SDL_GetError();
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
	}
}


/// <summary>
/// This is only a debugin function that allow you to view the path calculated by a ghost
/// </summary>
/// <param name="aList"></param>
void World::GenerateDotInPath(std::list<PathmapTile*>& aList)
{
	static std::list<PathmapTile*> internalPath;

	for (std::list<PathmapTile*>::iterator list_iter = aList.begin(); list_iter != aList.end(); list_iter++)
	{
		PathmapTile* tile = *list_iter;

		Sprite* newSprite = NULL;
		std::map<std::string, std::string> assetPaths;

		assetPaths.clear();
		assetPaths["smallDot"] = "Small_Dot_32.png";
		newSprite = Sprite::Create(assetPaths, myDrawer, 32, 32);
		newSprite->SetFrame("smallDot");
		Dot* dot = new Dot(Vector2f(tile->myX * 22, tile->myY * 22), newSprite);
		myDots.push_back(dot);
	}
}