#include "IntroState.h"


template<> IntroState* Ogre::Singleton<IntroState>::msSingleton = 0;

using namespace std;

IntroState::IntroState() {
	_root = NULL;
	_win = NULL;
	_sceneMgr = NULL;
	_viewport = NULL;
	_camera = NULL;
	_nodeCamara = NULL;
	_nodeFoco = NULL;
	_exitState = false;
	_numArboles = 5;
	_numArbolesSecos = 5;
	_sheet = NULL;
	_menu = NULL;
	srand (time(NULL));
}

IntroState::~IntroState()
{
	cout << "~IntroState" << endl;
}

void
IntroState::enter ()
{
  _root = Ogre::Root::getSingletonPtr();
  _win = _root->getAutoCreatedWindow();
  _sceneMgr = _root->createSceneManager(Ogre::ST_GENERIC, "SceneManagerMenu");

  // Se define una camara
  _camera = _sceneMgr->createCamera("MenuCamera");
  _camera->setPosition(Ogre::Vector3(0.0,2.5,8.5));
  _camera->lookAt(Ogre::Vector3(0,1,0));
  _camera->setFOVy (Ogre::Degree(50));
  _camera->setNearClipDistance(1);
  _camera->setFarClipDistance(100);

  _viewport = _win->addViewport(_camera);
  _viewport->setBackgroundColour(Ogre::ColourValue(0.016, 0.454, 0.467));

  // Se inicializa el layout de CEGUI
  //Sheet
  _sheet = CEGUI::WindowManager::getSingleton().createWindow("DefaultWindow","Sheet");

    //Config Window
  _menu = CEGUI::WindowManager::getSingleton().loadWindowLayout("menuArkanoid.layout");

  _menu->getChild("Menu/Jugar")->subscribeEvent(CEGUI::PushButton::EventMouseEntersArea,CEGUI::Event::Subscriber(&IntroState::mouseInButton,this));
  _menu->getChild("Menu/Records")->subscribeEvent(CEGUI::PushButton::EventMouseEntersArea,CEGUI::Event::Subscriber(&IntroState::mouseInButton,this));
  _menu->getChild("Menu/Creditos")->subscribeEvent(CEGUI::PushButton::EventMouseEntersArea,CEGUI::Event::Subscriber(&IntroState::mouseInButton,this));
  _menu->getChild("Menu/Salir")->subscribeEvent(CEGUI::PushButton::EventMouseEntersArea,CEGUI::Event::Subscriber(&IntroState::mouseInButton,this));


  _menu->getChild("Menu/Jugar")->subscribeEvent(CEGUI::PushButton::EventMouseLeavesArea,CEGUI::Event::Subscriber(&IntroState::mouseOutButton,this));
  _menu->getChild("Menu/Records")->subscribeEvent(CEGUI::PushButton::EventMouseLeavesArea,CEGUI::Event::Subscriber(&IntroState::mouseOutButton,this));
  _menu->getChild("Menu/Creditos")->subscribeEvent(CEGUI::PushButton::EventMouseLeavesArea,CEGUI::Event::Subscriber(&IntroState::mouseOutButton,this));
  _menu->getChild("Menu/Salir")->subscribeEvent(CEGUI::PushButton::EventMouseLeavesArea,CEGUI::Event::Subscriber(&IntroState::mouseOutButton,this));

  _menu->getChild("Menu/Jugar")->subscribeEvent(CEGUI::PushButton::EventClicked,CEGUI::Event::Subscriber(&IntroState::clickButton,this));
  _menu->getChild("Menu/Records")->subscribeEvent(CEGUI::PushButton::EventClicked,CEGUI::Event::Subscriber(&IntroState::clickButton,this));
  _menu->getChild("Menu/Creditos")->subscribeEvent(CEGUI::PushButton::EventClicked,CEGUI::Event::Subscriber(&IntroState::clickButton,this));
  _menu->getChild("Menu/Salir")->subscribeEvent(CEGUI::PushButton::EventClicked,CEGUI::Event::Subscriber(&IntroState::clickButton,this));


  _sheet->addChildWindow(_menu);

  CEGUI::System::getSingleton().setGUISheet(_sheet);
  // Se cre la escena
  createScene();
  _exitState = false;

  // Se establece el Track de sonido
  _audioIntro = GameManager::getTrackManager()->load("snowfall-final.mp3");

  // Se pone en modo loop
  _audioIntro->play(true);

}

void
IntroState::exit()
{

	_audioIntro->stop();
	if (_menu) _menu->destroy();
	if (_sheet) _sheet->destroy();
	if (_sceneMgr) _sceneMgr->clearScene();
	if (_root) {
		if (_sceneMgr) {
		_root->destroySceneManager(_sceneMgr);
		}
		_root->getAutoCreatedWindow()->removeAllViewports();
	}
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


	// Se hace rotar la camara y el foco
	Ogre::Quaternion incremento(Ogre::Degree(grados * deltaT * velocidad), Ogre::Vector3::UNIT_Y);

	_nodeCamara->rotate(incremento,Ogre::Node::TS_WORLD);
	//_nodeFoco->rotate(incremento,Ogre::Node::TS_WORLD);


	return true;
}

bool
IntroState::frameEnded
(const Ogre::FrameEvent& evt)
{
	// se se sale de este estado (intro) se sale del juego
	if (_exitState) {
		return false;
	}

  
  return true;
}

void
IntroState::keyPressed
(const OIS::KeyEvent &e)
{
	CEGUI::System::getSingleton().injectKeyDown(e.key);
	CEGUI::System::getSingleton().injectChar(e.text);

}

void
IntroState::keyReleased
(const OIS::KeyEvent &e )
{
	CEGUI::System::getSingleton().injectKeyUp(e.key);
	if (e.key == OIS::KC_ESCAPE) {
		_exitState = true;
	}

}

void
IntroState::mouseMoved
(const OIS::MouseEvent &evt)
{
	CEGUI::System::getSingleton().injectMouseMove(evt.state.X.rel, evt.state.Y.rel);
}

void
IntroState::mousePressed
(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
	CEGUI::System::getSingleton().injectMouseButtonDown(convertMouseButton(id));
}

void
IntroState::mouseReleased
(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
	CEGUI::System::getSingleton().injectMouseButtonUp(convertMouseButton(id));
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
	//_sceneMgr->setShadowTextureSize(16384);
	_sceneMgr->setShadowTextureSize(512);



	Ogre::Light* light1 = _sceneMgr->createLight("Light1");
	light1->setDirection(Ogre::Vector3(0.07,-0.704,0.77));

	//light1->setPosition(0,4,-2);
	//light1->setType(Ogre::Light::LT_DIRECTIONAL);



	light1->setPosition(0,4.63,-2.93);
	light1->setType(Ogre::Light::LT_SPOTLIGHT);
	light1->setSpotlightInnerAngle(Ogre::Degree(25));
	light1->setSpotlightOuterAngle(Ogre::Degree(70));
	light1->setSpotlightFalloff(0);


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

/**
 * Metodo que se ejecuta cuando el raton se pone encima de un botón
 */
bool
IntroState::mouseInButton(const CEGUI::EventArgs &e){

	const CEGUI::MouseEventArgs& we = static_cast<const CEGUI::MouseEventArgs&>(e);
	CEGUI::String senderID = we.window->getName();

	_menu->getChild(senderID)->setAlpha(1.0);
	return true;
}

/**
 * Metodo que se ejecuta cuando el raton se quita de encima de un botón
 */
bool
IntroState::mouseOutButton(const CEGUI::EventArgs &e){

	const CEGUI::MouseEventArgs& we = static_cast<const CEGUI::MouseEventArgs&>(e);
	CEGUI::String senderID = we.window->getName();

	_menu->getChild(senderID)->setAlpha(0.3);
	return true;
}

/**
 * Metodo que se ejecuta cuando se pulsa un botom
 * Se compruba cual es el botón pulsado y se realiza la accion correspondiente
 */
bool
IntroState::clickButton(const CEGUI::EventArgs &e){

	const CEGUI::MouseEventArgs& we = static_cast<const CEGUI::MouseEventArgs&>(e);
	CEGUI::String senderID = we.window->getName();

	if (senderID == "Menu/Jugar") {
		changeState(PlayState::getSingletonPtr());
	} else if  (senderID == "Menu/Records") {
		changeState(RecordState::getSingletonPtr());
	} else if  (senderID == "Menu/Creditos") {
		changeState(CreditsState::getSingletonPtr());
	} else if  (senderID == "Menu/Salir") {
		_exitState = true;
	}
	return true;
}

