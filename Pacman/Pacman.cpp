#include "Pacman.h"
#include "Drawer.h"
#include "SDL.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>

#include "Avatar.h"
#include "World.h"
#include "Ghost.h"
#include "SpriteFont.h"

Pacman* Pacman::Create(Drawer* aDrawer)
{
	//Creates and initialice characters properties and objects
	Pacman* pacman = new Pacman(aDrawer);

	//Here load the world map between image and logic
	if (!pacman->Init())
	{
		delete pacman;
		pacman = NULL;
	}

	return pacman;
}

Pacman::Pacman(Drawer* aDrawer)
	: myDrawer(aDrawer)
	, myTimeToNextUpdate(0.f)
	, myNextMovement(-1.f, 0.f)
	, myScore(0)
	, myFps(0)
	, myLives(3)
	, myGhostGhostCounter(0.f)
{
	//Used for initialice entities
	std::map<std::string, std::string> assetPaths;
	assetPaths.clear();

	//Avatar entity
	assetPaths["closedRight"] = "closed_right_32.png";
	assetPaths["openRight"] = "open_right_32.png";
	assetPaths["closedDown"] = "closed_down_32.png";
	assetPaths["openDown"] = "open_down_32.png";
	assetPaths["closedLeft"] = "closed_left_32.png";
	assetPaths["openLeft"] = "open_left_32.png";
	assetPaths["closedUp"] = "closed_up_32.png";
	assetPaths["openUp"] = "open_up_32.png";
	myAvatar = new Avatar(Vector2f(13 * 22, 22 * 22), Sprite::Create(assetPaths, myDrawer, 32, 32));
	assetPaths.clear();

	//Blinky
	assetPaths["vulnerable"] = "Ghost_Vulnerable_32.png";
	assetPaths["dead"] = "Ghost_Dead_32.png";
	assetPaths["normal"] = "ghost_32_red.png";

	Ghost* ghost = new Ghost(Vector2f(12 * 22, 10 * 22),
		Sprite::Create(assetPaths, myDrawer, 32, 32),
		Ghost::GhostBehavior::Chase,
		//new ChaseAmbush(),
		new ChaseAggresive(),
		new ScatterUpRight(),
		new FrigtenedWander(),
		100);
	ghosts.push_back(ghost);
	assetPaths.clear();

	//Pinky
	assetPaths.clear();
	assetPaths["vulnerable"] = "Ghost_Vulnerable_32.png";
	assetPaths["dead"] = "Ghost_Dead_32.png";
	assetPaths["normal"] = "ghost_32_pink.png";

	ghost = new Ghost(Vector2f(13 * 22, 13 * 22),
		Sprite::Create(assetPaths, myDrawer, 32, 32),
		Ghost::GhostBehavior::Respawning,
		new ChaseAmbush(),
		new ScatterUpLeft(),
		new FrigtenedWander(),
		97);
	ghosts.push_back(ghost);
	assetPaths.clear();

	//Inky
	assetPaths.clear();
	assetPaths["vulnerable"] = "Ghost_Vulnerable_32.png";
	assetPaths["dead"] = "Ghost_Dead_32.png";
	assetPaths["normal"] = "ghost_32_cyan.png";

	ghost = new Ghost(Vector2f(12 * 22, 14 * 22),
		Sprite::Create(assetPaths, myDrawer, 32, 32),
		Ghost::GhostBehavior::Respawning,
		new ChasePatrol(ghost),
		new ScatterDownRight(),
		new FrigtenedWander(),
		80);
	ghosts.push_back(ghost);
	assetPaths.clear();

	////Clyde
	assetPaths.clear();
	assetPaths["vulnerable"] = "Ghost_Vulnerable_32.png";
	assetPaths["dead"] = "Ghost_Dead_32.png";
	assetPaths["normal"] = "ghost_32_orange.png";

	ghost = new Ghost(Vector2f(14 * 22, 12 * 22),
		Sprite::Create(assetPaths, myDrawer, 32, 32),
		Ghost::GhostBehavior::Respawning,
		new ChaseRandom(),
		new ScatterDownLeft(),
		new FrigtenedWander(),
		70);
	ghosts.push_back(ghost);
	assetPaths.clear();

	myWorld = new World(myAvatar, myDrawer);

	//Sets display text
	gameplayMessage = SpriteFont::Create("freefont-ttf\\sfd\\FreeMono.ttf", "", { 255,255,255,255 }, 24, myDrawer);
	scoreDisplay = SpriteFont::Create("freefont-ttf\\sfd\\FreeMono.ttf", "", { 0,255,0,255 }, 24, myDrawer);
	livesDisplay = SpriteFont::Create("freefont-ttf\\sfd\\FreeMono.ttf", "", { 255,255,255,255 }, 24, myDrawer);
	fpsDisplay = SpriteFont::Create("freefont-ttf\\sfd\\FreeMono.ttf", "", { 255,255,255,255 }, 24, myDrawer);

	UpdateLives(myLives);
	UpdateScore(0);
}

Pacman::~Pacman(void)
{
}

bool Pacman::Init()
{
	myWorld->Init(myDrawer);
	return true;
}

bool Pacman::Update(float aTime)
{
	//Check keyboard input
	if (!UpdateInput())
		return false;


	//If has eaten all dots or have 0 lifes
	if (CheckEndGameCondition())
		return true;
	else if (myLives <= 0)
		return true;

	//Update Avatar status
	MoveAvatar();
	myAvatar->Update(aTime);

	//Update Ghosts status
	std::list<Ghost*>::iterator ghostIterator;
	for (ghostIterator = ghosts.begin(); ghostIterator != ghosts.end(); ghostIterator++)
		(*ghostIterator)->Update(aTime, myWorld, myAvatar);

	//Check avatar intersection between dots
	if (myWorld->HasIntersectedDot(myAvatar->GetPosition()))
	{
		UpdateScore(10);
		if (CheckEndGameCondition())
			gameplayMessage->SetText("You win!");
	}
	if (myWorld->HasIntersectedBigDot(myAvatar->GetPosition()))
	{
		UpdateScore(20);
		for (ghostIterator = ghosts.begin(); ghostIterator != ghosts.end(); ghostIterator++)
			(*ghostIterator)->SetBehaviour(Ghost::GhostBehavior::Fear);
	}

	//Check avatar intersection between ghost
	for (ghostIterator = ghosts.begin(); ghostIterator != ghosts.end(); ghostIterator++)
	{
		//If ghost is touching avatar
		if (((*ghostIterator)->GetPosition() - myAvatar->GetPosition()).Length() < 16.f)
		{
			if ((*ghostIterator)->myBehavior != Ghost::GhostBehavior::Fear && (*ghostIterator)->myBehavior != Ghost::GhostBehavior::Dead)
			{
				UpdateLives(myLives - 1);

				if (myLives > 0)
				{
					myAvatar->Respawn(Vector2f(13 * 22, 22 * 22));
					(*ghostIterator)->Respawn(Vector2f(13 * 22, 13 * 22));
					(*ghostIterator)->SetBehaviour(Ghost::GhostBehavior::Respawning);
					break;
				}
				else
				{
					gameplayMessage->SetText("You lose!");
					break;
				}
			}
			else if ((*ghostIterator)->myBehavior == Ghost::GhostBehavior::Fear && !(*ghostIterator)->myIsDeadFlag)
			{
				UpdateScore(50);
				(*ghostIterator)->Die(myWorld);
			}
		}
	}

	if (aTime > 0)
		SetFPS((int)(1 / aTime));

	return true;
}

void Pacman::UpdateScore(int scoreGain)
{
	myScore += scoreGain;
	std::stringstream stream;
	stream << "Score: ";
	stream << myScore;
	scoreDisplay->SetText(stream.str());
}

void Pacman::UpdateLives(int lives)
{
	myLives = lives;
	std::stringstream stream;
	stream << "Lives: ";
	stream << myLives;
	livesDisplay->SetText(stream.str());
}

void Pacman::SetFPS(int fps)
{
	myFps = fps;
	std::stringstream stream;
	stream << "FPS: ";
	stream << myFps;
	fpsDisplay->SetText(stream.str());
}

/// <summary>
/// Handles the user input key and made a vector for the movement
/// </summary>
/// <returns></returns>
bool Pacman::UpdateInput()
{
	const Uint8* keystate = SDL_GetKeyboardState(NULL);

	if (keystate[SDL_SCANCODE_UP])
	{
		myNextMovement = Vector2f(0.f, -1.f);
	}
	else if (keystate[SDL_SCANCODE_DOWN])
	{
		myNextMovement = Vector2f(0.f, 1.f);
	}
	else if (keystate[SDL_SCANCODE_RIGHT])
	{
		myNextMovement = Vector2f(1.f, 0.f);
	}
	else if (keystate[SDL_SCANCODE_LEFT])
	{
		myNextMovement = Vector2f(-1.f, 0.f);
	}

	if (keystate[SDL_SCANCODE_ESCAPE])
		return false;

	return true;
}

/// <summary>
/// Calculates the next x,y avatar position
/// </summary>
void Pacman::MoveAvatar()
{
	if (myAvatar->IsAtDestination())
	{
		int nextTileX = myAvatar->GetCurrentTileX() + myNextMovement.myX;
		int nextTileY = myAvatar->GetCurrentTileY() + myNextMovement.myY;

		//This is for handle the circular way in the middle
		if (nextTileY == 13 && nextTileX == -1)
			nextTileX = 25;
		else if (nextTileY == 13 && nextTileX == 26)
			nextTileX = 0;

		if (myWorld->TileIsValid(nextTileX, nextTileY))
			myAvatar->SetNextTile(nextTileX, nextTileY);
	}
}

bool Pacman::CheckEndGameCondition()
{
	return myWorld->GetDotCount() == 0;
}

bool Pacman::Draw()
{
	std::list<Ghost*>::iterator ghostIterator;
	myWorld->Draw(myDrawer);
	myAvatar->Draw(myDrawer);
	for (ghostIterator = ghosts.begin(); ghostIterator != ghosts.end(); ghostIterator++)
		(*ghostIterator)->Draw(myDrawer);

	scoreDisplay->Draw(myDrawer, 20, 50);
	livesDisplay->Draw(myDrawer, 20, 80);
	fpsDisplay->Draw(myDrawer, 880, 50);

	if (CheckEndGameCondition() || myLives <= 0)
		gameplayMessage->Draw(myDrawer, 500, 100);

	return true;
}
