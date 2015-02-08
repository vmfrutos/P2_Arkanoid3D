#ifndef Structure_H
#define Structure_H

#include <Ogre.h>

class Structure{

public:
	Structure(Ogre::SceneManager* sceneMgr);
	~Structure();
	void initSetUp();
	void setNodeParent(Ogre::SceneNode* scenNode);
	inline Ogre::SceneNode* getSceneNode() {return _nodeStructure;}

private:

	Ogre::SceneManager* _sceneMgr;
	Ogre::SceneNode* _nodeStructure;
	Ogre::Entity* _entLeft;
	Ogre::Entity* _entRigt;
	Ogre::Entity* _entTop;

};

#endif
