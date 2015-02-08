#include "Wall.h"

Wall::Wall(Ogre::SceneManager* sceneMgr){
	_sceneMgr = sceneMgr;
	_currentLevel = 0;

}

Wall::~Wall(){

}

void
Wall::loadLevel(){

	for (unsigned int i=0; i<38;i++){
		_brick[i] = new Brick(_sceneMgr,0,1,NULL,i);
		_brick[i]->initSetUp();
		_brick[i]->setPosition(Ogre::Vector3(9.5-(i/2.0),7,0));
		std::cout << "X: " << Ogre::Vector3(9.5-(i/2.0),7,0).x << std::endl;
	}


}
