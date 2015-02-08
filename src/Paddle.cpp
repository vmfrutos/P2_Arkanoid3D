#include "Paddle.h"

Paddle::Paddle(Ogre::SceneManager* sceneMgr) {
	_sceneMgr = sceneMgr;
	_entPaddleI = _sceneMgr->createEntity("PalaI","PalaI.mesh");
	_entPaddleD = _sceneMgr->createEntity("PalaD","PalaD.mesh");
	_entPaddleC = _sceneMgr->createEntity("PalaC","PalaC.mesh");

	_entPaddleI->setCastShadows(true);
	_entPaddleD->setCastShadows(true);
	_entPaddleC->setCastShadows(true);


	_nodePaddle = _sceneMgr->createSceneNode("pala");
	_nodePaddle->attachObject(_entPaddleI);
	_nodePaddle->attachObject(_entPaddleD);
	_nodePaddle->attachObject(_entPaddleC);
	_speed = 20;
	_raySceneQuery = _sceneMgr->createRayQuery(Ogre::Ray());
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
	Ogre::Vector3 dcha(-1.0,0.0,0.0);
	if (!isCollisionWihtStructure(_nodePaddle->getPosition(),dcha)) {
		_nodePaddle->translate(dcha * timeSinceLastFrame * _speed);
	}
}

void
Paddle::moveLeft(float timeSinceLastFrame){
	Ogre::Vector3 izda(1.0,0.0,0.0);
	if (!isCollisionWihtStructure(_nodePaddle->getPosition(),izda)) {
		_nodePaddle->translate(izda * timeSinceLastFrame * _speed);
	}
}

bool
Paddle::isCollisionWihtStructure(const Ogre::Vector3& position, const Ogre::Vector3& direction)
{
    Ogre::Ray ray(position, direction);
    _raySceneQuery->setRay(ray);

    Ogre::RaySceneQueryResult &result = _raySceneQuery->execute();

    Ogre::RaySceneQueryResult::iterator itr;

    for (itr = result.begin(); itr != result.end(); itr++) {
        if ((itr->movable->getName().compare("Pared1")==0
        		|| itr->movable->getName().compare("Pared2")==0)
        		&& (itr->distance<1.5)) {
            return true;
        }
    }
    return false;
}

void
Paddle::hide(){
	_nodePaddle->setVisible(false);
}





