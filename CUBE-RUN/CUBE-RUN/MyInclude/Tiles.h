#pragma once
#include "Object.h"
#include "Box.h"
#include "Tile.h"

class TextureClass;

class Tiles
{
public:
	std::vector<Tile*> tiles;

public:
	void init();
	void update();
	void draw(unsigned int shaderNum, int bindTex);

	void createTile();


	void release();

};


