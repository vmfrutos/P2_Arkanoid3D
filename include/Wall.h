#ifndef Wall_H
#define Wall_H

#include "Brick.h"
#include "Ogre.h"
#include <string>
#include "Properties.h"

using namespace std;

const unsigned int NUM_FILAS = 10;
const unsigned int NUM_COLUMNAS = 18;

#ifdef WIN32
const string FILE_NAME = "levels\\level";
#else
const string FILE_NAME = "levels/level";
#endif


class Brick;

class Wall {

public:

	Wall(Ogre::SceneManager* sceneMgr);
	~Wall();

	bool loadLevel(int level);

	Brick* getBrickByName(const Ogre::String& name);
	bool levelCompleted();
	void refresh(float timeSinceLastFrame);
	void resetItems();

private:
	Ogre::SceneManager* _sceneMgr;
	int _currentLevel;

	// Esto sera una matric de bricks
	Brick* _bricks[NUM_FILAS][NUM_COLUMNAS];
	void cleanCurrentLevel();


};

#endif
