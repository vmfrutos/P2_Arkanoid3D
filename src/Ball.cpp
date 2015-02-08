#include "Ball.h"


Ball::Ball(Ogre::SceneManager* sceneMgr,PlayState* playstate){
	_playState = playstate;
	_sceneMgr = sceneMgr;
	_entBola = _sceneMgr->createEntity("Bola.mesh");
	_entBola->setCastShadows(true);
	_nodeBola = _sceneMgr->createSceneNode("bola");
	_nodeBola->attachObject(_entBola);
	_enMovimiento = false;
	_speed = 10;
	_raySceneQuery = _sceneMgr->createRayQuery(Ogre::Ray());

	srand (time(NULL));
}

Ball::~Ball() {

}

void
Ball::initSetUp() {
	_enMovimiento = false;
	_comprobarColision[0] = true;
	_comprobarColision[1] = true;
	_comprobarColision[2] = true;
	_comprobarColision[3] = false;
	_comprobarColision[4] = false;
	_comprobarColision[5] = false;
	_comprobarColision[6] = false;
	_nodeBola->setPosition(0.0,0.6,0.0);
	_direccion = Ogre::Vector3(0,1,0);
}


void
Ball::setMove(bool move) {
	_enMovimiento = move;
}

void
Ball::move(float timeSinceLastFrame) {

	// Si hay colision con la estructura cambia la direccion
	if (isCollisionWihtStructure(_nodeBola->getPosition(),_direccion)) {

		// si ha colisionado con pared izquierda
		if (!_comprobarColision[0])
			_direccion = Ogre::Vector3(-_direccion.x,_direccion.y,0.0);

		// si ha colisionado con pared derecha
		else if (!_comprobarColision[1]) {
			_direccion = Ogre::Vector3(-_direccion.x,_direccion.y,0.0);
		}

		// si ha colisionado con techo
		else if (!_comprobarColision[2]) {
			_direccion = Ogre::Vector3(_direccion.x,-_direccion.y,0.0);
		}

		// si ha colisionado con la pala izquierda
		else if (!_comprobarColision[3]) {
			_direccion = Ogre::Vector3(_direccion.x-0.3,-_direccion.y,0.0);
		}

		// si ha colisionado con la pala derecha
		else if (!_comprobarColision[4]) {
			_direccion = Ogre::Vector3(_direccion.x+0.3,-_direccion.y,0.0);
		}

		// si ha colisionado con la pala centro
		else if (!_comprobarColision[5]) {
			int signo = rand() % 2;
			float desvio = (rand() % 3)/10.0;
			if (signo == 1) desvio = -desvio;
			std::cout << "desvio: " << desvio << std::endl;
			_direccion = Ogre::Vector3(_direccion.x+desvio,-_direccion.y,0.0);
		}

		// si ha colisionado con el suelo
		else if (!_comprobarColision[6]) {
			_playState->quitarVida();
		}

	}
	_nodeBola->translate(_direccion * timeSinceLastFrame * _speed);

}

void Ball::setNodeParent(Ogre::SceneNode* newParent) {

	Ogre::Vector3 posicion;
	if (_nodeBola->getParentSceneNode() != NULL) {

		// Se extrae la posicion del nodo padre
		posicion = _nodeBola->getParentSceneNode()->getPosition();

		// Se elimina del nodo padre actual
		_nodeBola->getParentSceneNode()->removeChild(_nodeBola);

		_nodeBola->setPosition(posicion);
	}

	newParent->addChild(_nodeBola);

}

bool
Ball::isCollisionWihtStructure(const Ogre::Vector3& position, const Ogre::Vector3& direction)
{
    Ogre::Ray ray(position, direction);
    _raySceneQuery->setRay(ray);

    Ogre::RaySceneQueryResult &result = _raySceneQuery->execute();

    Ogre::RaySceneQueryResult::iterator itr;


    for (itr = result.begin(); itr != result.end(); itr++) {
    	if (itr->movable->getName().compare("Pared1")==0 && itr->distance<0.100 && _comprobarColision[0]) {
    		_comprobarColision[0] = false;
    		_comprobarColision[1] = true;
    		_comprobarColision[2] = true;
    		_comprobarColision[3] = true;
    		_comprobarColision[4] = true;
    		_comprobarColision[5] = true;
    		_comprobarColision[6] = true;
    		std::cout << "Hay colision con Pared1" << std::endl;
    		return true;
    	}
    	if (itr->movable->getName().compare("Pared2")==0 && itr->distance<0.100 && _comprobarColision[1]) {
    		_comprobarColision[0] = true;
    		_comprobarColision[1] = false;
    		_comprobarColision[2] = true;
    		_comprobarColision[3] = true;
    		_comprobarColision[4] = true;
    		_comprobarColision[5] = true;
    		_comprobarColision[6] = true;
    		std::cout << "Hay colision con Pared2" << std::endl;
    		return true;
    	}
    	if (itr->movable->getName().compare("Techo")==0 && itr->distance<0.100 && _comprobarColision[2]) {
    		_comprobarColision[0] = true;
    		_comprobarColision[1] = true;
    		_comprobarColision[2] = false;
    		_comprobarColision[3] = true;
    		_comprobarColision[4] = true;
    		_comprobarColision[5] = true;
    		_comprobarColision[6] = true;
    		std::cout << "Hay colision con techo" << std::endl;
    		return true;
    	}

    	if (itr->movable->getName().compare("PalaI")==0 && itr->distance<0.100 && _comprobarColision[3]) {
    		_comprobarColision[0] = true;
    		_comprobarColision[1] = true;
    		_comprobarColision[2] = true;
    		_comprobarColision[3] = false;
    		_comprobarColision[4] = true;
    		_comprobarColision[5] = true;
    		_comprobarColision[6] = true;
    		std::cout << "Hay colision con Pala Izquierda" << std::endl;
    		return true;
    	}

    	if (itr->movable->getName().compare("PalaD")==0 && itr->distance<0.100 && _comprobarColision[4]) {
    		_comprobarColision[0] = true;
    		_comprobarColision[1] = true;
    		_comprobarColision[2] = true;
    		_comprobarColision[3] = true;
    		_comprobarColision[4] = false;
    		_comprobarColision[5] = true;
    		_comprobarColision[6] = true;
    		std::cout << "Hay colision con Pala Derecha" << std::endl;
    		return true;
    	}

    	if (itr->movable->getName().compare("PalaC")==0 && itr->distance<0.100 && _comprobarColision[5]) {
    		_comprobarColision[0] = true;
    		_comprobarColision[1] = true;
    		_comprobarColision[2] = true;
    		_comprobarColision[3] = true;
    		_comprobarColision[4] = true;
    		_comprobarColision[5] = false;
    		_comprobarColision[6] = true;
    		std::cout << "Hay colision con Pala Centro" << std::endl;
    		return true;
    	}

    	if (itr->movable->getName().compare("Suelo")==0 && itr->distance<0.100 && _comprobarColision[6]) {
    		_comprobarColision[0] = true;
    		_comprobarColision[1] = true;
    		_comprobarColision[2] = true;
    		_comprobarColision[3] = true;
    		_comprobarColision[4] = true;
    		_comprobarColision[5] = true;
    		_comprobarColision[6] = false;
    		std::cout << "Hay colision con Suelo" << std::endl;
    		return true;
    	}



    }
    return false;
}

void
Ball::hide(){
	_nodeBola->setVisible(false);
}
