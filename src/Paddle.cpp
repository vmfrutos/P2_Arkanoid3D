#include "Paddle.h"

Paddle::Paddle(Ogre::SceneManager* sceneMgr) {
	_sceneMgr = sceneMgr;
	_entPaddleI = _sceneMgr->createEntity("PalaI","PalaI.mesh");
	_entPaddleD = _sceneMgr->createEntity("PalaD","PalaD.mesh");
	_entPaddleC = _sceneMgr->createEntity("PalaC","PalaC.mesh");
	_entPaddleExtD = _sceneMgr->createEntity("PalaExtD","PalaExtD.mesh");
	_entPaddleExtI = _sceneMgr->createEntity("PalaExtI","PalaExtI.mesh");

	_entPaddleI->setCastShadows(true);
	_entPaddleD->setCastShadows(true);
	_entPaddleC->setCastShadows(true);
	_entPaddleExtD->setCastShadows(true);
	_entPaddleExtI->setCastShadows(true);


	_nodePaddle = _sceneMgr->createSceneNode("pala");
	_nodePaddle->attachObject(_entPaddleI);
	_nodePaddle->attachObject(_entPaddleD);
	_nodePaddle->attachObject(_entPaddleC);
	_speed = 20;
	_expanded = false;
	_intersectionSceneQuery = _sceneMgr->createIntersectionQuery();
	_timerAvtive = false;
	_timer.reset();
	_ItemBlock="";
}

Paddle::~Paddle() {

}

void
Paddle::initSetUp() {
	_nodePaddle->setPosition(0.0,0.0,0.0);
}

void
Paddle::setNodeParent(Ogre::SceneNode* newParent) {

	Ogre::Vector3 posicion;
	if (_nodePaddle->getParentSceneNode() != NULL) {

		// Se extrae la posicion del nodo padre
		posicion = _nodePaddle->getParentSceneNode()->getPosition();

		// Se elimina del nodo padre actual
		_nodePaddle->getParentSceneNode()->removeChild(_nodePaddle);

		_nodePaddle->setPosition(posicion);
	}

	newParent->addChild(_nodePaddle);

}

void
Paddle::moveRight(float timeSinceLastFrame){
	Ogre::Vector3 izda(1.0,0.0,0.0);
	_nodePaddle->translate(izda * timeSinceLastFrame * _speed);
	if (isCollisionWihtStructure("Pared1")) {
		_nodePaddle->translate((-izda) * timeSinceLastFrame * _speed);
	}
}

void
Paddle::moveLeft(float timeSinceLastFrame){
	Ogre::Vector3 dcha(-1.0,0.0,0.0);
	_nodePaddle->translate(dcha * timeSinceLastFrame * _speed);
	if (isCollisionWihtStructure("Pared2")) {
		_nodePaddle->translate((-dcha) * timeSinceLastFrame * _speed);
	}
}

bool
Paddle::isCollisionWihtStructure(const Ogre::String& name)
{
	Ogre::IntersectionSceneQueryResult &result =_intersectionSceneQuery->execute();

	    Ogre::SceneQueryMovableIntersectionList :: iterator iter;
	    for (iter=result.movables2movables.begin(); iter!=result.movables2movables.end(); ++iter) {

	    	Ogre::SceneQueryMovableObjectPair pObject = static_cast <Ogre::SceneQueryMovableObjectPair> (* iter);

	    	Ogre::String strFirst = pObject.first-> getName();
	    	Ogre::String strSecond = pObject.second-> getName();

	     	if (strFirst == name || strSecond == name) {
	     		if (strFirst == "PalaI" ||
	     			strSecond == "PalaI" ||
	     			strFirst == "PalaD" ||
	     			strSecond == "PalaD" ||
	     			strFirst == "PalaC" ||
	     			strSecond == "PalaC" ||
	     			strFirst == "PalaExtI" ||
	     			strSecond == "PalaExtI" ||
	     			strFirst == "PalaExtD" ||
	     			strSecond == "PalaExtD") {
	     			return true;
	     		}
	     	}


	    	/*
	    	if (strFirst == "PalaI" || strSecond == "PalaI") {

	    		if (strFirst == name || strSecond == name){
	    			return true;
	    		}
	    	} else if (strFirst == "PalaD" || strSecond == "PalaD") {

	    		if (strFirst == name || strSecond == name){
	    			return true;
	    		}
	    	} else if (strFirst == "PalaC" || strSecond == "PalaC") {

	    		if (strFirst == name || strSecond == name){
	    			return true;
	    		}
	    	} else if (strFirst == "PalaExtI" || strSecond == "PalaExtI") {

	    		if (strFirst == name || strSecond == name){
	    			return true;
	    		}
	    	} else if (strFirst == "PalaExtD" || strSecond == "PalaExtD") {

	    		if (strFirst == name || strSecond == name){
	    			return true;
	    		}
	    	}

	    	*/
	    }

    return false;
}

void
Paddle::hide(){
	_nodePaddle->setVisible(false);
}


bool
Paddle::expandPadle(int segundos, Ogre::String itemName){

	if (_ItemBlock == "" || _ItemBlock == itemName) {// Si no esta bloqueado o lo est'apor el llamante
		_ItemBlock = itemName;
		if (_timerAvtive == false ) {

			// Se activa el timer
			_timerAvtive = true;

			// Se amplia la pala
			_nodePaddle->attachObject(_entPaddleExtD);
			_nodePaddle->attachObject(_entPaddleExtI);
			_expanded = true;

		} else {
			if (_timer.getSegundosTranscurridos() > segundos) {

				// Se acobo el tiempo
				// Se contrae la pala
				contractPadle();
				_timerAvtive = false;
				_timer.reset();
				_ItemBlock = "";
				return true;
			}
		}
	}
	return false;
}

void
Paddle::contractPadle(){
	if (_expanded == true) {
		_nodePaddle->detachObject(_entPaddleExtD);
		_nodePaddle->detachObject(_entPaddleExtI);
		_expanded = false;
	}
}

void
Paddle::refresh(float timeSinceLastFrame){
	// Se incrementa el timer si está activo
	if (_timerAvtive) {
		_timer.incrementar(timeSinceLastFrame);
	}
}

void
Paddle::reset() {
	cout << "*************************Se llama a reset" << endl;
	contractPadle();
	_timerAvtive = false;
	_timer.reset();
	_ItemBlock = "";
}

bool
Paddle::isBusy(Ogre::String itemName){
	if (_ItemBlock == "" || _ItemBlock == itemName)
		return false;
	else
		return true;
}





