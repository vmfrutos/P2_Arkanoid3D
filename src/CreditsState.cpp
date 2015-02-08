#include "CreditsState.h"


template<> CreditsState* Ogre::Singleton<CreditsState>::msSingleton = 0;

using namespace std;

CreditsState::CreditsState() {
	_root = NULL;
	_win = NULL;
	_sceneMgr = NULL;
	_viewport = NULL;
	_camera = NULL;
	_exitState = false;
	_sheet = NULL;
	_menu = NULL;


}

CreditsState::~CreditsState() {
	cout << "~CreditsState" << endl;
}

void
CreditsState::enter ()
{
  _root = Ogre::Root::getSingletonPtr();
  _win = _root->getAutoCreatedWindow();
  _sceneMgr = _root->createSceneManager(Ogre::ST_GENERIC, "SceneManagerCredits");

  // Se define una camara
  _camera = _sceneMgr->createCamera("CreditsCamera");
  _camera->setPosition(Ogre::Vector3(0.03,0.96,2.35));
  _camera->lookAt(Ogre::Vector3(0,1,0));
  _camera->setFOVy (Ogre::Degree(50));
  _camera->setNearClipDistance(1);
  _camera->setFarClipDistance(100);
  _viewport = _win->addViewport(_camera);

  //Sheet
  _sheet = CEGUI::WindowManager::getSingleton().createWindow("DefaultWindow","Sheet");

  //Config Window
  _menu = CEGUI::WindowManager::getSingleton().loadWindowLayout("creditosArkanoid.layout");
  _menu->getChild("Credits/Volver")->subscribeEvent(CEGUI::PushButton::EventMouseEntersArea,CEGUI::Event::Subscriber(&CreditsState::mouseInButton,this));
  _menu->getChild("Credits/Volver")->subscribeEvent(CEGUI::PushButton::EventMouseLeavesArea,CEGUI::Event::Subscriber(&CreditsState::mouseOutButton,this));
  _menu->getChild("Credits/Volver")->subscribeEvent(CEGUI::PushButton::EventClicked,CEGUI::Event::Subscriber(&CreditsState::clickButton,this));

  string creditos = ReadTextFromFile();
  _menu->getChild("Credits/texto")->setText(creditos);

  _sheet->addChildWindow(_menu);

  CEGUI::System::getSingleton().setGUISheet(_sheet);

  // Se crea la escena
  createScene();
  _exitState = false;

  // Se establece el Track de sonido
  _audioCredits = GameManager::getTrackManager()->load("pianomoodmelancholy10.wav");

  // Se pone en modo loop
  _audioCredits->play(true);
}

void
CreditsState::exit()
{
	_audioCredits->stop();
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
CreditsState::pause ()
{
}

void
CreditsState::resume ()
{
}

bool
CreditsState::frameStarted
(const Ogre::FrameEvent& evt)
{
	return true;
}

bool
CreditsState::frameEnded
(const Ogre::FrameEvent& evt)
{
	// se se sale de este estado (intro) se sale del juego
	if (_exitState)
		changeState(IntroState::getSingletonPtr());
  return true;
}

void
CreditsState::keyPressed
(const OIS::KeyEvent &e)
{


}

void
CreditsState::keyReleased
(const OIS::KeyEvent &e )
{
	if (e.key == OIS::KC_ESCAPE) {
		_exitState = true;
	}
}

void
CreditsState::mouseMoved
(const OIS::MouseEvent &evt)
{
	CEGUI::System::getSingleton().injectMouseMove(evt.state.X.rel, evt.state.Y.rel);
}

void
CreditsState::mousePressed
(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
	CEGUI::System::getSingleton().injectMouseButtonDown(convertMouseButton(id));
}

void
CreditsState::mouseReleased
(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
	CEGUI::System::getSingleton().injectMouseButtonUp(convertMouseButton(id));
}

CreditsState*
CreditsState::getSingletonPtr ()
{
return msSingleton;
}

CreditsState&
CreditsState::getSingleton ()
{ 
  assert(msSingleton);
  return *msSingleton;
}

void
CreditsState::createScene() {

	Ogre::Light* light1 = _sceneMgr->createLight("Light");
		light1->setPosition(0,4,-2);
		light1->setDirection(Ogre::Vector3(0.07,-0.704,0.77));
		light1->setType(Ogre::Light::LT_DIRECTIONAL);
		light1->setPowerScale(1.7299998998641968);
		light1->setCastShadows(true);

	Ogre::Entity* entMuro = _sceneMgr->createEntity("Muro.mesh");
	entMuro->setCastShadows(false);


	Ogre::SceneNode* nodeMuro = _sceneMgr->createSceneNode("muro");


	nodeMuro->attachObject(entMuro);


	_sceneMgr->getRootSceneNode()->addChild(nodeMuro);

}

/**
 * Metodo que se ejecuta cuando el raton se pone encima de un botón
 */
bool
CreditsState::mouseInButton(const CEGUI::EventArgs &e){

	const CEGUI::MouseEventArgs& we = static_cast<const CEGUI::MouseEventArgs&>(e);
	CEGUI::String senderID = we.window->getName();

	_menu->getChild(senderID)->setAlpha(1.0);
	return true;
}

/**
 * Metodo que se ejecuta cuando el raton se quita de encima de un botón
 */
bool
CreditsState::mouseOutButton(const CEGUI::EventArgs &e){

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
CreditsState::clickButton(const CEGUI::EventArgs &e){

	const CEGUI::MouseEventArgs& we = static_cast<const CEGUI::MouseEventArgs&>(e);
	CEGUI::String senderID = we.window->getName();

	if (senderID == "Credits/Volver") {
		changeState(IntroState::getSingletonPtr());
	}
	return true;
}

string
CreditsState::ReadTextFromFile(){
	string line;
	string retorno;
	ifstream myfile (_CREDITS_FILE);

	if (myfile.is_open()){
		while ( getline (myfile,line) ){
			retorno.append(line);
			retorno.append("\n");
		}

		myfile.close();
	}

	return retorno;
}


