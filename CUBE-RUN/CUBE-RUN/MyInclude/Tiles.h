#pragma once

#include "common.h"

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


