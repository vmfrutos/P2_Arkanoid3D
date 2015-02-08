#ifndef Ground_H
#define Ground_H

#include <Ogre.h>

class Ground {
public:
	Ground(Ogre::SceneManager* sceneMgr);
	~Ground();
	void initSetUp();
	void setNodeParent(Ogre::SceneNode* scenNode);
	inline Ogre::SceneNode* getSceneNode() {return _nodeGround;}


private:
	Ogre::SceneManager* _sceneMgr;
	Ogre::SceneNode* _nodeGround;
	Ogre::Entity* _entGround;

};

#endif
