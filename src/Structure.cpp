#include "Structure.h"

Structure::Structure(Ogre::SceneManager* sceneMgr) {
	_sceneMgr = sceneMgr;

	_entLeft = _sceneMgr->createEntity("Pared1","Pared1.mesh");
	_entLeft->setCastShadows(true);

	_entRigt = _sceneMgr->createEntity("Pared2","Pared2.mesh");
	_entRigt->setCastShadows(true);

	_entTop = _sceneMgr->createEntity("Techo","Techo.mesh");
	_entTop->setCastShadows(true);

	_nodeStructure = _sceneMgr->createSceneNode("estructura");
	_nodeStructure->attachObject(_entLeft);
	_nodeStructure->attachObject(_entRigt);
	_nodeStructure->attachObject(_entTop);
}

Structure::~Structure() {
}

void
Structure::setNodeParent(Ogre::SceneNode* newParent) {

	Ogre::Vector3 posicion;
	if (_nodeStructure->getParentSceneNode() != NULL) {

		// Se extrae la posicion del nodo padre
		posicion = _nodeStructure->getParentSceneNode()->getPosition();

		// Se elimina del nodo padre actual
		_nodeStructure->getParentSceneNode()->removeChild(_nodeStructure);

		_nodeStructure->setPosition(posicion);
	}

	newParent->addChild(_nodeStructure);

}

void
Structure::initSetUp() {

}
