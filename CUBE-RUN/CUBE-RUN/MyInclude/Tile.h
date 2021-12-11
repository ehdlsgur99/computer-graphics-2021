#pragma once

#include "Mesh.h"
#include "Cube.h"
#include <list>

class TextureClass;

/// <summary>
/// ���� Ȥ�� �� Ÿ���� Ÿ��(ť����� ����)�� ������ �ִ� class
/// ���� = 6x3 ���, �� = 3x3 ���
/// </summary>

enum eTileType { eDirect = 0, eLeftTurn, eRightTurn };
enum eCreatePos {eUp = 0, eLeft, eRight, eDown};
enum eCubeType {eCubeUp = 0, eCubeDown, eCubeCoin};

class Tile
{
public:
	eTileType tileType;
	glm::vec3 startPos;			// startPos�� ���� Ÿ�� ���� �ϴ��� ����
	eCreatePos createDirection;
	std::vector<Cube*> cubes;
public:
	void init(eTileType tileType, glm::vec3 startPos,  eCreatePos createPos);
	void update();
	void draw(unsigned int shaderNum, int bindTex);
	void release();
};

