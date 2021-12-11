#pragma once

#include "Mesh.h"
#include "Cube.h"
#include <list>

class TextureClass;

/// <summary>
/// 직선 혹은 턴 타입의 타일(큐브들의 집합)을 가지고 있는 class
/// 직선 = 6x3 행렬, 턴 = 3x3 행렬
/// </summary>

enum eTileType { eDirect = 0, eLeftTurn, eRightTurn };
enum eCreatePos {eUp = 0, eLeft, eRight, eDown};
enum eCubeType {eCubeNormal = 0, eCubeUp, eCubeDown, eCubeCoin};

class Tile
{
public:
	eTileType tileType;
	glm::vec3 startPos;			// startPos는 이전 타일 좌측 하단을 기준
	eCreatePos createDirection;
	std::vector<Cube*> cubes;
	std::vector<eCubeType> cubeTypes;
	std::vector<Cube*> coins;
public:
	void init(eTileType tileType, glm::vec3 startPos,  eCreatePos createPos);
	void update();
	void draw(unsigned int shaderNum, int bindTex);
	void release();
};

