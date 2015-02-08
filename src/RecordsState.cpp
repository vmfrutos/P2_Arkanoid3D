#include "RecordsState.h"


template<> RecordState* Ogre::Singleton<RecordState>::msSingleton = 0;

using namespace std;

RecordState::RecordState() {
	_root = NULL;
	_win = NULL;
	_sceneMgr = NULL;
	_viewport = NULL;
	_camera = NULL;
	_exitState = false;
	_sheet = NULL;
	_menu = NULL;
	_animTextoJugador = NULL;
	_animTextoNivel = NULL;
	_animTextoTiempo = NULL;

}

RecordState::~RecordState() {
	cout << "~RecordState" << endl;
}

void
RecordState::enter ()
{
  _root = Ogre::Root::getSingletonPtr();
  _win = _root->getAutoCreatedWindow();
  _sceneMgr = _root->createSceneManager(Ogre::ST_GENERIC, "SceneManagerRecords");

  // Se define una camara
  _camera = _sceneMgr->createCamera("RecordsCamera");
  _camera->setPosition(Ogre::Vector3(0.03,0.96,2.35));
  _camera->lookAt(Ogre::Vector3(0,1,0));
  _camera->setFOVy (Ogre::Degree(50));
  _camera->setNearClipDistance(1);
  _camera->setFarClipDistance(100);

  _viewport = _win->addViewport(_camera);

  //Sheet
  _sheet = CEGUI::WindowManager::getSingleton().createWindow("DefaultWindow","Sheet");

  //Config Window
  _menu = CEGUI::WindowManager::getSingleton().loadWindowLayout("recordsArkanoid.layout");

  _menu->getChild("Records/Volver")->subscribeEvent(CEGUI::PushButton::EventMouseEntersArea,CEGUI::Event::Subscriber(&RecordState::mouseInButton,this));
  _menu->getChild("Records/Volver")->subscribeEvent(CEGUI::PushButton::EventMouseLeavesArea,CEGUI::Event::Subscriber(&RecordState::mouseOutButton,this));
  _menu->getChild("Records/Volver")->subscribeEvent(CEGUI::PushButton::EventClicked,CEGUI::Event::Subscriber(&RecordState::clickButton,this));

  _sheet->addChildWindow(_menu);

  CEGUI::System::getSingleton().setGUISheet(_sheet);

  // Se recuperan los records
  vector<Record> records = RecordsManager::getSingletonPtr()->getRecords();

  // Se muestran solo los 5 primeros registros
  unsigned int numRegistros = 5;
  if (records.size() < 5) {
	  numRegistros = records.size();
  }

  for (unsigned int i=1; i<=numRegistros;i++) {
	  std::stringstream nombreCampo;

	  nombreCampo << "Records/Nombre" << i;
	  _menu->getChild(nombreCampo.str())->setText(records[i-1].getNombre());

	  nombreCampo.str("");
	  nombreCampo << "Records/Nivel" << i;
	  _menu->getChild(nombreCampo.str())->setText(records[i-1].getNivelStr());

	  nombreCampo.str("");
	  nombreCampo << "Records/Tiempo" << i;
	  _menu->getChild(nombreCampo.str())->setText(records[i-1].getTiempoTranscurridoStr());
  }

  // Se cre la escena
  createScene();
  _exitState = false;

  // Se establece el Track de sonido
  _audioRecords = GameManager::getTrackManager()->load("forest-ruins.wav");

  // Se pone en modo loop
  _audioRecords->play(true);
}

void
RecordState::exit()
{
	_audioRecords->stop();
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
RecordState::pause ()
{
}

void
RecordState::resume ()
{
}

bool
RecordState::frameStarted
(const Ogre::FrameEvent& evt)
{
	Ogre::Real deltaT = evt.timeSinceLastFrame;

	_animTextoJugador->addTime(deltaT);
	_animTextoNivel->addTime(deltaT);
	_animTextoTiempo->addTime(deltaT);
	return true;
}

bool
RecordState::frameEnded
(const Ogre::FrameEvent& evt)
{
	// se se sale de este estado (intro) se sale del juego
	if (_exitState)
		changeState(IntroState::getSingletonPtr());
  
  return true;
}

void
RecordState::keyPressed
(const OIS::KeyEvent &e)
{


}

void
RecordState::keyReleased
(const OIS::KeyEvent &e )
{
	if (e.key == OIS::KC_ESCAPE) {
		_exitState = true;
	}
}

void
RecordState::mouseMoved
(const OIS::MouseEvent &evt)
{
	CEGUI::System::getSingleton().injectMouseMove(evt.state.X.rel, evt.state.Y.rel);
}

void
RecordState::mousePressed
(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
	CEGUI::System::getSingleton().injectMouseButtonDown(convertMouseButton(id));
}

void
RecordState::mouseReleased
(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
	CEGUI::System::getSingleton().injectMouseButtonUp(convertMouseButton(id));
}

RecordState*
RecordState::getSingletonPtr ()
{
return msSingleton;
}

RecordState&
RecordState::getSingleton ()
{ 
  assert(msSingleton);
  return *msSingleton;
}

void
RecordState::createScene() {

	Ogre::Light* light1 = _sceneMgr->createLight("Light");
		light1->setPosition(0,4,-2);
		light1->setDirection(Ogre::Vector3(0.07,-0.704,0.77));
		light1->setType(Ogre::Light::LT_DIRECTIONAL);
		light1->setPowerScale(1.7299998998641968);
		light1->setCastShadows(true);

	Ogre::Entity* entMuro = _sceneMgr->createEntity("Muro.mesh");
	entMuro->setCastShadows(false);

	Ogre::Entity* entTextoJugador = _sceneMgr->createEntity("Textjugador.mesh");
	entTextoJugador->setCastShadows(false);

	Ogre::Entity* entTextoNivel = _sceneMgr->createEntity("TextNivel.mesh");
	entTextoNivel->setCastShadows(false);

	Ogre::Entity* entTextoTiempo = _sceneMgr->createEntity("TextTiempo.mesh");
	entTextoTiempo->setCastShadows(false);


	Ogre::SceneNode* nodeMuro = _sceneMgr->createSceneNode("muro");
	Ogre::SceneNode* nodeTextoJugador = _sceneMgr->createSceneNode("TextoJugador");
	Ogre::SceneNode* nodeTextoNivel = _sceneMgr->createSceneNode("TextoNivel");
	Ogre::SceneNode* nodeTextoTiempo = _sceneMgr->createSceneNode("TextoTiempo");

	nodeTextoJugador->translate(0.0,0.5,0.0);
	nodeTextoNivel->translate(0.0,0.5,0.0);
	nodeTextoTiempo->translate(0.0,0.5,0.0);

	nodeTextoJugador->scale(0.7,0.7,1.0);
	nodeTextoNivel->scale(0.7,0.7,1.0);
	nodeTextoTiempo->scale(0.7,0.7,1.0);

	nodeMuro->attachObject(entMuro);
	nodeTextoJugador->attachObject(entTextoJugador);
	nodeTextoNivel->attachObject(entTextoNivel);
	nodeTextoTiempo->attachObject(entTextoTiempo);

	_sceneMgr->getRootSceneNode()->addChild(nodeMuro);
	_sceneMgr->getRootSceneNode()->addChild(nodeTextoJugador);
	_sceneMgr->getRootSceneNode()->addChild(nodeTextoNivel);
	_sceneMgr->getRootSceneNode()->addChild(nodeTextoTiempo);

	// Se inicializan las animaciones de los textos
	_animTextoJugador = entTextoJugador->getAnimationState("AnimationTextoJugador");
	_animTextoNivel = entTextoNivel->getAnimationState("AnimationTextoNivel");
	_animTextoTiempo = entTextoTiempo->getAnimationState("AnimationTextoTiempo");

	_animTextoJugador->setEnabled(true);
	_animTextoJugador->setLoop(true);

	_animTextoNivel->setEnabled(true);
	_animTextoJugador->setLoop(true);

	_animTextoTiempo->setEnabled(true);
	_animTextoTiempo->setLoop(true);


}

/**
 * Metodo que se ejecuta cuando el raton se pone encima de un botón
 */
bool
RecordState::mouseInButton(const CEGUI::EventArgs &e){

	const CEGUI::MouseEventArgs& we = static_cast<const CEGUI::MouseEventArgs&>(e);
	CEGUI::String senderID = we.window->getName();

	_menu->getChild(senderID)->setAlpha(1.0);
	return true;
}

/**
 * Metodo que se ejecuta cuando el raton se quita de encima de un botón
 */
bool
RecordState::mouseOutButton(const CEGUI::EventArgs &e){

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
RecordState::clickButton(const CEGUI::EventArgs &e){

	const CEGUI::MouseEventArgs& we = static_cast<const CEGUI::MouseEventArgs&>(e);
	CEGUI::String senderID = we.window->getName();

	if (senderID == "Records/Volver") {
		changeState(IntroState::getSingletonPtr());
	}
	return true;
}


