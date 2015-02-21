#include "Item.h"

Item::Item(Ogre::SceneManager* sceneMgr, int tipo, Ogre::Vector3 posicionInicial, const Ogre::String& nombre) {
	_tipo = tipo;
	_status = ITEM_STATUS_DORMIDO;
	_speed = 10;
	_sceneMgr = sceneMgr;
	_name = nombre;


	_entItem = _sceneMgr->createEntity(_name,"Item.mesh");
	_entItem->setCastShadows(true);
	_nodeItem = sceneMgr->createSceneNode(_name);
	_nodeItem->attachObject(_entItem);

	_sceneMgr->getRootSceneNode()->addChild(_nodeItem);
	_entItem->setVisible(false);
	_nodeItem->setPosition(posicionInicial);
	_intersectionSceneQuery = _sceneMgr->createIntersectionQuery();

	_animItem = _entItem->getAnimationState("animItem");
	_animItem->setEnabled(true);
	_animItem->setLoop(true);

	if (tipo == ITEM_TIPO_VIDA_EXTRA) {
		setMaterial("mat_rojo");
	} else if (tipo == ITEM_TIPO_PALA_20) {
		setMaterial("mat_verde");
	} else if (tipo == ITEM_TIPO_PALA_60) {
		setMaterial("mat_azul");
	} else if (tipo == ITEM_TIPO_BOLA_LENTA_15) {
		setMaterial("mat_amarillo");
	}
}
Item::~Item() {
	_sceneMgr->destroyEntity(_entItem);
	_sceneMgr->destroySceneNode(_nodeItem);
}

void
Item::refresh(float timeSinceLastFrame) {

	if (_status == ITEM_STATUS_CAYENDO) {
		Ogre::Vector3 vt(0,-1,0);
		_nodeItem->translate(vt * _speed * timeSinceLastFrame);
		_animItem->addTime(timeSinceLastFrame);
		Ogre::String elemntoColisionado;
		if (isCollisionWihtStructure(elemntoColisionado)) {
			if (elemntoColisionado == "Suelo") {
				_status = ITEM_STATUS_FINALIZADO;

			} else {
				_status = ITEM_STATUS_ACTIVO;
				GameSoundFX::getSingletonPtr()->setSoundFX(GameSoundFX::FX_PALA_ITEM);
			}
			_entItem->setVisible(false);

		}
	} else if (_status == ITEM_STATUS_ACTIVO) {
		if (_tipo == ITEM_TIPO_VIDA_EXTRA) {
			incrementarVida();
		} else if (_tipo == ITEM_TIPO_PALA_20) {
			ampliarPala(20);
		} else if (_tipo == ITEM_TIPO_PALA_60) {
			ampliarPala(60);
		}else if (_tipo == ITEM_TIPO_BOLA_LENTA_15) {
			redicirVelocidadBola(15);
		}
	}
}

void
Item::iniciar() {
	// Solo se incia si el estado está dormido
	if (_status == ITEM_STATUS_DORMIDO) {
		_status = ITEM_STATUS_CAYENDO;
		_entItem->setVisible(true);
	}
}

bool
Item::isCollisionWihtStructure(Ogre::String& name)
{
	Ogre::IntersectionSceneQueryResult &result =_intersectionSceneQuery->execute();

	    Ogre::SceneQueryMovableIntersectionList :: iterator iter;
	    for (iter=result.movables2movables.begin(); iter!=result.movables2movables.end(); ++iter) {

	    	Ogre::SceneQueryMovableObjectPair pObject = static_cast <Ogre::SceneQueryMovableObjectPair> (* iter);

	    	Ogre::String strFirst = pObject.first-> getName ();
	    	Ogre::String strSecond = pObject.second-> getName ();
	    	if (strFirst == _name || strSecond == _name) {

	    		if (strFirst == _name)
	    			name = strSecond;
	    		else
	    			name = strFirst;

	    		if (strFirst == "PalaI" ||
	    			strSecond == "PalaI" ||
	    			strFirst == "PalaD" ||
	    			strSecond == "PalaD" ||
	    			strFirst == "PalaC" ||
	    			strSecond == "PalaC" ||
	    			strFirst == "PalaExtD" ||
	    			strSecond == "PalaExtD" ||
	    			strFirst == "PalaExtI" ||
	    			strSecond == "PalaExtI" ||
	    			strFirst == "Suelo" ||
	    			strSecond == "Suelo"){
	    			return true;
	    		}
	    	}
	    }

    return false;
}

void Item::incrementarVida(){
	PlayState::getSingletonPtr()->incVida();
	_status = ITEM_STATUS_FINALIZADO;
}

void
Item::ampliarPala(int segundos){
	if (PlayState::getSingletonPtr()->getPala()->isBusy(_name)) {
		// Si ya esta exandida se ignora el item
		_status = ITEM_STATUS_FINALIZADO;
	} else {
		if (PlayState::getSingletonPtr()->getPala()->expandPadle(segundos,_name)){
			_status = ITEM_STATUS_FINALIZADO;
		}
	}

}

void
Item::redicirVelocidadBola(int segundos){
	if (PlayState::getSingletonPtr()->getBola()->isBusy(_name)) {
		// Si ya esta ralentizada se ignora el item
		_status = ITEM_STATUS_FINALIZADO;
	} else {
		if (PlayState::getSingletonPtr()->getBola()->slowBallTimer(segundos,_name)){
			_status = ITEM_STATUS_FINALIZADO;
		}
	}

}

void
Item::reset(){
	_status = ITEM_STATUS_DORMIDO;
	_entItem->setVisible(false);
}

void
Item::setMaterial(const Ogre::String& matName){

	_entItem->setMaterialName(matName);

}
