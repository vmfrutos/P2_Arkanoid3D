#include "IntroState.h"
#include "PlayState.h"

template<> IntroState* Ogre::Singleton<IntroState>::msSingleton = 0;

using namespace std;

void
IntroState::enter ()
{
  _root = Ogre::Root::getSingletonPtr();
  _win = _root->getAutoCreatedWindow();
  _sceneMgr = _root->createSceneManager(Ogre::ST_GENERIC, "SceneManager");
  // Se define una camara
  _camera = _sceneMgr->createCamera("MenuCamera");
  _camera->setPosition(Ogre::Vector3(0.0,2.5,8.5));
  _camera->lookAt(Ogre::Vector3(0,1,0));

  _camera->setFOVy (Ogre::Degree(50));
  _camera->setNearClipDistance(1);
  _camera->setFarClipDistance(100);

  _viewport = _win->addViewport(_camera);
  _viewport->setBackgroundColour(Ogre::ColourValue(0.016, 0.454, 0.467));

  // Se cre la escena
  createScene();
  _exitGame = false;
}

void
IntroState::exit()
{
  _sceneMgr->clearScene();
  _root->getAutoCreatedWindow()->removeAllViewports();
}

void
IntroState::pause ()
{
}

void
IntroState::resume ()
{
}

bool
IntroState::frameStarted
(const Ogre::FrameEvent& evt) 
{

	Ogre::Real deltaT = evt.timeSinceLastFrame;
	Ogre::Real grados = 4;
	Ogre::Real velocidad = 5;


	// Se hace rotar la camara
	Ogre::Quaternion incremento(Ogre::Degree(grados * deltaT * velocidad), Ogre::Vector3::UNIT_Y);
	_nodeCamara->rotate(incremento,Ogre::Node::TS_WORLD);
	_nodeFoco->rotate(incremento,Ogre::Node::TS_WORLD);

	// Se hace rotar el foco

	return true;
}

bool
IntroState::frameEnded
(const Ogre::FrameEvent& evt)
{
  if (_exitGame)
    return false;
  
  return true;
}

void
IntroState::keyPressed
(const OIS::KeyEvent &e)
{
  // Transición al siguiente estado.
  // Espacio --> PlayState
  if (e.key == OIS::KC_SPACE) {
    changeState(PlayState::getSingletonPtr());
  }

  /*

  if (e.key == OIS::KC_Q) {
    //changeState(PlayState::getSingletonPtr());
	Ogre::Vector3 pos(luz->getDirection());
	pos.x=pos.x+0.1;
	luz->setDirection(pos);
  }

  if (e.key == OIS::KC_W) {
    //changeState(PlayState::getSingletonPtr());
	Ogre::Vector3 pos(luz->getDirection());
	pos.y=pos.y+0.1;
	luz->setDirection(pos);
  }

  if (e.key == OIS::KC_E) {
    //changeState(PlayState::getSingletonPtr());
	Ogre::Vector3 pos(luz->getDirection());
	pos.z=pos.z+0.1;
	luz->setDirection(pos);
  }

  if (e.key == OIS::KC_A) {
    //changeState(PlayState::getSingletonPtr());
	Ogre::Vector3 pos(luz->getDirection());
	pos.x=pos.x-0.1;
	luz->setDirection(pos);
  }

  if (e.key == OIS::KC_S) {
    //changeState(PlayState::getSingletonPtr());
	Ogre::Vector3 pos(luz->getDirection());
	pos.y=pos.y-0.1;
	luz->setDirection(pos);
  }

  if (e.key == OIS::KC_D) {
    //changeState(PlayState::getSingletonPtr());
	Ogre::Vector3 pos(luz->getDirection());
	pos.z=pos.z-0.1;
	luz->setDirection(pos);
  }

  */
}

void
IntroState::keyReleased
(const OIS::KeyEvent &e )
{
  if (e.key == OIS::KC_ESCAPE) {
    _exitGame = true;
  }
}

void
IntroState::mouseMoved
(const OIS::MouseEvent &e)
{
}

void
IntroState::mousePressed
(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
}

void
IntroState::mouseReleased
(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
}

IntroState*
IntroState::getSingletonPtr ()
{
return msSingleton;
}

IntroState&
IntroState::getSingleton ()
{ 
  assert(msSingleton);
  return *msSingleton;
}

void
IntroState::createScene() {

	// Se establece la iluminacion
	_sceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_TEXTURE_MODULATIVE);
	_sceneMgr->setShadowColour(Ogre::ColourValue(0.5, 0.5, 0.5) );
	_sceneMgr->setAmbientLight(Ogre::ColourValue(0.9, 0.9, 0.9));

	//_sceneMgr->setShadowTextureCount(2);
	_sceneMgr->setShadowTextureSize(16384);



	Ogre::Light* light1 = _sceneMgr->createLight("Light2");
	light1->setPosition(0,4,-2);
	light1->setDirection(Ogre::Vector3(0.07,-0.704,0.77));
	light1->setType(Ogre::Light::LT_DIRECTIONAL);
	light1->setPowerScale(1.7299998998641968);
	light1->setCastShadows(true);


	Ogre::Entity* entMuro = _sceneMgr->createEntity("Muro.mesh");
	entMuro->setCastShadows(true);


	Ogre::Entity* entSuelo = _sceneMgr->createEntity("Suelo.mesh");
	entSuelo->setCastShadows(false);

	Ogre::Entity* entLetras = _sceneMgr->createEntity("Letras.mesh");
	entLetras->setCastShadows(true);

	// Arboles
	// Se crean las entidades arbol seco
	std::vector<Ogre::Entity*> entidadesArbolSeco;
	for (unsigned int j=0;j<_numArbolesSecos;j++){
		Ogre::Entity* entArbolSeco = _sceneMgr->createEntity("ArbolSeco.mesh");
		entidadesArbolSeco .push_back(entArbolSeco);
	}

	// Se crean las entidades arbol con hojas
	std::vector<Ogre::Entity*> entidadesArbol;
	for (unsigned int i=0;i<_numArboles;i++){
		Ogre::Entity* entArbol = _sceneMgr->createEntity("Arbol.mesh");
		entidadesArbol.push_back(entArbol);
	}


	Ogre::SceneNode* nodeMuro = _sceneMgr->createSceneNode("muro");
	Ogre::SceneNode* nodeSuelo = _sceneMgr->createSceneNode("suelo");
	Ogre::SceneNode* nodeTexto = _sceneMgr->createSceneNode("letras");
	_nodeCamara = _sceneMgr->createSceneNode("camara");
	_nodeFoco = _sceneMgr->createSceneNode("foco");

	// Se crean los nodos arbol seco
	std::vector<Ogre::SceneNode*> nodosArbolSeco;
	for (unsigned int i=0;i<_numArbolesSecos;i++){
		std::stringstream nombreNodo;
		nombreNodo << "arbolseco" << i;
		Ogre::SceneNode* nodeArbolSeco = _sceneMgr->createSceneNode(nombreNodo.str());
		nodosArbolSeco.push_back(nodeArbolSeco);
	}

	// Se crean los nodos arbol con hojas
	std::vector<Ogre::SceneNode*> nodosArbol;
	for (unsigned int i=0;i<_numArboles;i++){
		std::stringstream nombreNodo;
		nombreNodo << "arbol" << i;
		Ogre::SceneNode* nodeArbol = _sceneMgr->createSceneNode(nombreNodo.str());
		nodosArbol.push_back(nodeArbol);
	}


	nodeMuro->attachObject(entMuro);
	nodeSuelo->attachObject(entSuelo);
	nodeTexto->attachObject(entLetras);
	_nodeCamara->attachObject(_camera);
	_nodeFoco->attachObject(light1);

	// A cada nodo arbol seco se le asocia su entidad
	for (unsigned int i=0;i<nodosArbolSeco.size();i++){
		nodosArbolSeco[i]->attachObject(entidadesArbolSeco[i]);
	}

	// A cada nodo arbol se le asocia su entidad
	for (unsigned int i=0;i<nodosArbol.size();i++){
		nodosArbol[i]->attachObject(entidadesArbol[i]);
	}


	_sceneMgr->getRootSceneNode()->addChild(_nodeFoco);
	_sceneMgr->getRootSceneNode()->addChild(_nodeCamara);
	_sceneMgr->getRootSceneNode()->addChild(nodeSuelo);
	nodeSuelo->addChild(nodeMuro);
	nodeMuro->addChild(nodeTexto);

	// Se añaden los nodos arbol seco al nodo suelo y se colocan
	for (unsigned int i=0;i<nodosArbolSeco.size();i++){
		nodeSuelo->addChild(nodosArbolSeco[i]);

		// signo X
		int signoX = rand() % 2;
		// signo Y
		int signoZ = rand() % 2;

		float coordX = (rand() % 3000) + 300;
		Ogre::Real coordRealX = coordX/100.0;
		if (signoX == 1) {
			coordRealX = -coordRealX;
		}

		float coordZ = (rand() % 3000) + 300;
		Ogre::Real coordRealZ = coordZ/100.0;
		if (signoZ == 1) {
			coordRealZ = -coordRealZ;
		}
		nodosArbolSeco[i]->translate(coordRealX,0,coordRealZ);
	}

	// Se añaden los nodos arbol  al nodo suelo
	for (unsigned int i=0;i<nodosArbol.size();i++){
		nodeSuelo->addChild(nodosArbol[i]);

		// signo X
		int signoX = rand() % 2;
		// signo Y
		int signoZ = rand() % 2;

		float coordX = (rand() % 3000) + 300;
		Ogre::Real coordRealX = coordX/100.0;
		if (signoX == 1) {
			coordRealX = -coordRealX;
		}

		float coordZ = (rand() % 3000) + 300;
		Ogre::Real coordRealZ = coordZ/100.0;
		if (signoZ == 1) {
			coordRealZ = -coordRealZ;
		}
		nodosArbol[i]->translate(coordRealX,0,coordRealZ);

	}


}
