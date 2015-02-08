#ifndef Wall_H
#define Wall_H

#include "Brick.h"
#include "Ogre.h"

class Wall {

public:

	Wall(Ogre::SceneManager* sceneMgr);
	~Wall();

	void loadLevel();


private:
	Ogre::SceneManager* _sceneMgr;
	int _currentLevel;

	// Esto sera una matric de bricks
	Brick *_brick[38];
};

#endif
