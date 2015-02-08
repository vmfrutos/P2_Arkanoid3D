#ifndef Paddle_H
#define Paddle_H

#include <Ogre.h>

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

private:
	Ogre::SceneManager* _sceneMgr;
	Ogre::SceneNode* _nodePaddle;
	Ogre::Entity* _entPaddleI;
	Ogre::Entity* _entPaddleC;
	Ogre::Entity* _entPaddleD;
	int _speed;
	Ogre::RaySceneQuery* _raySceneQuery;

	bool isCollisionWihtStructure(const Ogre::Vector3& position, const Ogre::Vector3& direction);
};

#endif
