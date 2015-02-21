#include "Brick.h"

Brick::Brick(Ogre::SceneManager* sceneMgr, int color, int fila, int columna) {

	_visible = true;
	_item = NULL;
	_sceneMgr = sceneMgr;
	_color = color;
	_resistencia = 1;
	_tipoItem = 0;
	_fila = fila;
	_columna = columna;
	std::stringstream nombreLadrillo;
	nombreLadrillo << "Ladrillo" << _fila << "_" << _columna;
	_entBrick = _sceneMgr->createEntity(nombreLadrillo.str(),"Ladrillo.mesh");
	_entBrick->setCastShadows(true);


	_animLadrillo = _entBrick->getAnimationState("animLadrillo");
	_animLadrillo->setEnabled(true);
	_animLadrillo->setLoop(false);

	if (_color == COLOR_INVISIBLE) {
		hide();
	} else if (color == COLOR_ROJO) {
		setMaterial("mat_rojo");
	} else if (color == COLOR_VERDE) {
		setMaterial("mat_verde");
	} else if (color == COLOR_AMARILLO) {
		setMaterial("mat_amarillo");
	} else if (color == COLOR_AZUL) {
		setMaterial("mat_azul");
	}


	std::stringstream nombreNodo;
	nombreNodo << "Ladrillo" << _fila << "_" << _columna;
	_nodeBrick = sceneMgr->createSceneNode(nombreNodo.str());

	_nodeBrick->attachObject(_entBrick);

	_sceneMgr->getRootSceneNode()->addChild(_nodeBrick);



	_state = ANIM_NO_ACTIVE;

}

Brick::~Brick(){
	if (_item)
		delete _item;

	_sceneMgr->destroyEntity(_entBrick);
	_sceneMgr->destroySceneNode(_nodeBrick);
}

void
Brick::initSetUp(){

}

void
Brick::setPosition(Ogre::Vector3 position){
	_position = position;
	_nodeBrick->setPosition(_position);
}

void
Brick::hide(){
	_nodeBrick->detachObject(_entBrick);
	_visible = false;
}

void
Brick::romper(){
	_resistencia--;
	if (_resistencia <=0) {

		_state = ANIM_ACTIVE;
		if (_item != NULL) {
			_item->iniciar();
		}
	}
}



void
Brick::setResistencia(int valor) {
	_resistencia = valor;
}


int
Brick::getResistencia() {
	return _resistencia;
}


void
Brick::setItem(int tipoItem) {
	_tipoItem = tipoItem;
	if (_tipoItem != 0){
		std::stringstream nombreItem;
		nombreItem << "Item" << _fila << "_" << _columna;
		_item = new Item(_sceneMgr,tipoItem,_nodeBrick->getPosition(),nombreItem.str());
	}
}


Item*
Brick::getItem() {
	return _item;
}



bool
Brick::isVisible(){
	return _visible;
}

void
Brick::setMaterial(const Ogre::String& matName){

	_entBrick->setMaterialName(matName);

	/*
	std::stringstream nombreLadrillo;
	nombreLadrillo << "Ladrillo" << _fila << "_" << _columna;

	Ogre::Entity *ent = static_cast <Ogre::Entity *> (_nodeBrick->getAttachedObject(nombreLadrillo.str()));
	ent->setMaterialName(matName);
	*/
}

void
Brick::refresh(float timeSinceLastFrame){
	if (_item != NULL) {
		_item->refresh(timeSinceLastFrame);
	}

	if (_state == ANIM_ACTIVE){
		_animLadrillo->addTime(timeSinceLastFrame);
		if (_animLadrillo->hasEnded()){
			_state = ANIM_NO_ACTIVE;
			hide();
		}
	}
}
bool
Brick::estaRoto(){
	if (_state == ANIM_ACTIVE){
		return true;
	} else {
		return false;
	}

}
