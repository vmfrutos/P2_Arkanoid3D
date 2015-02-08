#include "Brick.h"

Brick::Brick(Ogre::SceneManager* sceneMgr, int color, int resistenia, Item* item, int num) {
	if (item != NULL)
		_item = item;
	_sceneMgr = sceneMgr;
	_color = color;
	_resistencia = resistenia;
	_numero = num;
	_entBrickTop = _sceneMgr->createEntity("LadrilloTop.mesh");
	_entBrickTop->setCastShadows(true);
	_entBrickBottom = _sceneMgr->createEntity("LadrilloBottom.mesh");
	_entBrickBottom->setCastShadows(true);
	_entBrickRigth = _sceneMgr->createEntity("LadrilloRight.mesh");
	_entBrickRigth->setCastShadows(true);
	_entBrickLeft = _sceneMgr->createEntity("LadrilloLeft.mesh");
	_entBrickLeft->setCastShadows(true);

	std::stringstream nombreNodo;
	nombreNodo << "Ladrillo" << _numero;
	_nodeBrick = sceneMgr->createSceneNode(nombreNodo.str());

	_nodeBrick->attachObject(_entBrickTop);
	_nodeBrick->attachObject(_entBrickBottom);
	_nodeBrick->attachObject(_entBrickRigth);
	_nodeBrick->attachObject(_entBrickLeft);

	_sceneMgr->getRootSceneNode()->addChild(_nodeBrick);


}

Brick::~Brick(){
	if (_item)
		delete _item;

}

void
Brick::initSetUp(){

}

void
Brick::setPosition(Ogre::Vector3 position){
	_position = position;
	_nodeBrick->setPosition(_position);
}
