#ifndef Paddle_H
#define Paddle_H

#include <Ogre.h>
#include <Contador.h>

class Paddle {
public:
	Paddle(Ogre::SceneManager* sceneMgr);
	~Paddle();
	void initSetUp();
	void setNodeParent(Ogre::SceneNode* scenNode);
	inline Ogre::SceneNode* getSceneNode() {return _nodePaddle;}
	void moveRight(float timeSinceLastFrame);
	void moveLeft(float timeSinceLastFrame);
	void hide();
	bool expandPadle(int segundos, Ogre::String itemName);
	void contractPadle();
	bool isBusy(Ogre::String itemName);
	void refresh(float timeSinceLastFrame);
	void reset();

private:
	Ogre::SceneManager* _sceneMgr;
	Ogre::SceneNode* _nodePaddle;
	Ogre::Entity* _entPaddleI;
	Ogre::Entity* _entPaddleC;
	Ogre::Entity* _entPaddleD;
	Ogre::Entity* _entPaddleExtD;
	Ogre::Entity* _entPaddleExtI;
	int _speed;
	bool _expanded;
	bool _timerAvtive;
	Contador _timer;
	Ogre::IntersectionSceneQuery* _intersectionSceneQuery;
	bool isCollisionWihtStructure(const Ogre::String& name);
	Ogre::String _ItemBlock;

};

#endif
