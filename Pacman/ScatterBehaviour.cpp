#include "ScatterBehaviour.h"
#include <iostream>


void ScatterUpLeft::Scatter(World* aWorld, Avatar* avatar, Ghost* ghost)
{
	int gx = ghost->GetCurrentTileX();
	int gy = ghost->GetCurrentTileY();

	if (gx == 5 && gy == 4)
		aWorld->GetPath(ghost, 0, 0);
	else
		aWorld->GetPath(ghost, 5, 4);
}

void ScatterUpRight::Scatter(World* aWorld, Avatar* avatar, Ghost* ghost)
{
	int gx = ghost->GetCurrentTileX();
	int gy = ghost->GetCurrentTileY();

	if (gx == 20 && gy == 4)
		aWorld->GetPath(ghost, 25, 0);
	else
		aWorld->GetPath(ghost, 20, 4);
}

void ScatterDownLeft::Scatter(World* aWorld, Avatar* avatar, Ghost* ghost)
{
	int gx = ghost->GetCurrentTileX();
	int gy = ghost->GetCurrentTileY();

	if (gx == 5 && gy == 22)
		aWorld->GetPath(ghost, 5, 28);
	else
		aWorld->GetPath(ghost, 5, 22);
}


void ScatterDownRight::Scatter(World* aWorld, Avatar* avatar, Ghost* ghost)
{
	int gx = ghost->GetCurrentTileX();
	int gy = ghost->GetCurrentTileY();

	if (gx == 20 && gy == 22)
		aWorld->GetPath(ghost, 20, 28);
	else
		aWorld->GetPath(ghost, 20, 22);
}