#pragma once
#pragma warning (disable : 4996)

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 800

// 
#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <gl/glm/gtc/type_ptr.hpp>
#include <gl/glm/glm.hpp>
#include <gl/glm/ext.hpp>


#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <list>
#include <random>
#include <gl/glm/gtc/matrix_transform.hpp>
#include <Mmsystem.h>
#include <cstring>
#include <tchar.h>
#include <windows.h> // Header File For Windows
#include <stdio.h>   // Header File For Standard Input/Output
#include <stdarg.h>  // Header File For Variable Argument Routines
#include <gl\gl.h>   // Header File For The OpenGL32 Library
#include <gl\glu.h>  // Header File For The GLu32 Library



#pragma comment (lib, "msimg32.lib")
#pragma comment (lib, "winmm.lib")

#pragma comment (lib, "glew32.lib")
#pragma comment (lib, "freeglut.lib")

#include "TextureClass.h"

#include "Singleton.h"

#include "CollisionManager.h"
#include "TextManager.h"
#include "ObjectManager.h"
#include "ParticleManager.h"
#include "SoundManager.h"

#include "System.h"

#include "Types.h"

#include "Object.h"
#include "Timer.h"
#include "Scene.h"
#include "ShaderProgram.h"
#include "DepthMap.h"

#include "Player.h"
#include "Tile.h"
#include "Tiles.h"
#include "Box.h"
#include "Particle.h"
#include "Screen.h"









