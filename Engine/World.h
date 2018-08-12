#pragma once
#include "Level.h"
#include "File.h"
#include <vector>

class World
{
public:
	World(Graphics& gfx,const int elevation);
	void Draw(const RectI& rectToDraw, Graphics & gfx, const File toDraw, const Vei2 playerLoc);
	void CalcPrime();
	Block& BlockAtGridPos(const Vei2 pos, const int evel);
	Vei2 GridToIso(const Vei2 gridpos, const Vei2 origin) const;
	Vei2 IsoToGrid(const Vei2 screenPos, const Vei2 origin) const;
	Vei2 GetOrigin() const;
	int GetWidth() const;
	int GetLength() const;
	int GetBrickWidth() const;
	int GetBrickHeight() const;
	int GetElevation() const;
private:
	void CheckNeighborsSetPrime(const Vei2 pos, const int evel);
private:
	//z height in the 3d 
	int elevation;
	//level you can currently effect
	int curEvel = 1;
	//ogres are like my game they have layers
	std::vector<Level> Layers;
	//origin right now
	Vei2 origin = { 400,300 };
	//dim of sprites
	int brickWidth = 64;
	int brickHeight = 64;
	//dim of layers
	int width = 10;
	int length = 10;
	Surface highlight = Surface("highLightComp.bmp");
};