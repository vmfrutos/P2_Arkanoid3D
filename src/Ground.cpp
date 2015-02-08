#include "Ground.h"

Ground::Ground(Ogre::SceneManager* sceneMgr){
	_sceneMgr = sceneMgr;
	_entGround = _sceneMgr->createEntity("Suelo","SueloPlay.mesh");
	_entGround->setCastShadows(false);
	_nodeGround = _sceneMgr->createSceneNode("suelo");
	_nodeGround->attachObject(_entGround);
}

Ground::~Ground(){

}


void
Ground::initSetUp() {

}

void
Ground::setNodeParent(Ogre::SceneNode* newParent) {

	Ogre::Vector3 posicion;
	if (_nodeGround->getParentSceneNode() != NULL) {

		// Se extrae la posicion del nodo padre
		posicion = _nodeGround->getParentSceneNode()->getPosition();

		// Se elimina del nodo padre actual
		_nodeGround->getParentSceneNode()->removeChild(_nodeGround);

		_nodeGround->setPosition(posicion);
	}
	newParent->addChild(_nodeGround);
}

