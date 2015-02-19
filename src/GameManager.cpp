#include <Ogre.h>

#include "GameManager.h"
#include "GameState.h"
#include <Ogre.h>
#include <OgreSingleton.h>
#include <RendererModules/Ogre/CEGUIOgreRenderer.h>
#include <OIS/OIS.h>
#include <CEGUI.h>


template<> GameManager* Ogre::Singleton<GameManager>::msSingleton = 0;

TrackManager*  GameManager::_pTrackManager = NULL;
SoundFXManager*  GameManager::_pSoundFXManager = NULL;


GameManager::GameManager ()
{
  _root = NULL;
  _renderWindow = NULL;
  _inputMgr = NULL;
}

GameManager::~GameManager ()
{
	cout << "~GameManager" << endl;
	if (GameManager::_pTrackManager) delete GameManager::_pTrackManager;
	if (GameManager::_pSoundFXManager) delete GameManager::_pSoundFXManager;

	while (!_states.empty()) {
		_states.top()->exit();
		_states.pop();
	}
  
	if (_root)
		delete _root;
}

void
GameManager::start
(GameState* state)
{
  // Creación del objeto Ogre::Root.
  _root = new Ogre::Root();
  
  loadResources();

  if (!configure())
    return;    
  	
  _inputMgr = new InputManager;
  _inputMgr->initialise(_renderWindow);

  // Registro como key y mouse listener...
  _inputMgr->addKeyListener(this, "GameManager");
  _inputMgr->addMouseListener(this, "GameManager");

  // Se inicializa la librería CEGUI
  initializeCEGUI();

  // Se inicializa la librería SDL
  initSDL();

  // El GameManager es un FrameListener.
  _root->addFrameListener(this);

  // Transición al estado inicial.
  changeState(state);

  // Bucle de rendering.
  _root->startRendering();
}

void
GameManager::changeState
(GameState* state)
{
  // Limpieza del estado actual.
  if (!_states.empty()) {
    // exit() sobre el último estado.
    _states.top()->exit();
    // Elimina el último estado.
    _states.pop();
  }

  // Transición al nuevo estado.
  _states.push(state);
  // enter() sobre el nuevo estado.
  _states.top()->enter();
}

void
GameManager::pushState
(GameState* state)
{
  // Pausa del estado actual.
  if (!_states.empty())
    _states.top()->pause();
  
  // Transición al nuevo estado.
  _states.push(state);
  // enter() sobre el nuevo estado.
  _states.top()->enter();
}

void
GameManager::popState ()
{
  // Limpieza del estado actual.
  if (!_states.empty()) {
    _states.top()->exit();
    _states.pop();
  }
  
  // Vuelta al estado anterior.
  if (!_states.empty())
    _states.top()->resume();
}

void
GameManager::loadResources ()
{
  Ogre::ConfigFile cf;
  cf.load("resources.cfg");
  
  Ogre::ConfigFile::SectionIterator sI = cf.getSectionIterator();
  Ogre::String sectionstr, typestr, datastr;
  while (sI.hasMoreElements()) {
    sectionstr = sI.peekNextKey();
    Ogre::ConfigFile::SettingsMultiMap *settings = sI.getNext();
    Ogre::ConfigFile::SettingsMultiMap::iterator i;
    for (i = settings->begin(); i != settings->end(); ++i) {
      typestr = i->first;    datastr = i->second;
      Ogre::ResourceGroupManager::getSingleton().addResourceLocation
            (datastr, typestr, sectionstr);	
    }
  }
}

bool
GameManager::configure ()
{
  if (!_root->restoreConfig()) {
    if (!_root->showConfigDialog()) {
      return false;
    }
  }
  
  _renderWindow = _root->initialise(true, "Game State Example");
  
  Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
  
  return true;
}

GameManager*
GameManager::getSingletonPtr ()
{
  return msSingleton;
}

GameManager&
GameManager::getSingleton ()
{  
  assert(msSingleton);
  return *msSingleton;
}

// Las siguientes funciones miembro delegan
// el evento en el estado actual.
bool
GameManager::frameStarted
(const Ogre::FrameEvent& evt)
{
  _inputMgr->capture();
  return _states.top()->frameStarted(evt);
}

bool
GameManager::frameEnded
(const Ogre::FrameEvent& evt)
{
  return _states.top()->frameEnded(evt);
}

bool
GameManager::keyPressed 
(const OIS::KeyEvent &e)
{
  _states.top()->keyPressed(e);
  return true;
}

bool
GameManager::keyReleased
(const OIS::KeyEvent &e)
{
  _states.top()->keyReleased(e);
  return true;
}

bool
GameManager::mouseMoved 
(const OIS::MouseEvent &e)
{
  _states.top()->mouseMoved(e);
  return true;
}

bool
GameManager::mousePressed 
(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
  _states.top()->mousePressed(e, id);
  return true;
}

bool
GameManager::mouseReleased
(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
  _states.top()->mouseReleased(e, id);
  return true;
}

void
GameManager::initializeCEGUI()
{
	  //CEGUI
	  CEGUI::OgreRenderer::bootstrapSystem();
	  CEGUI::Scheme::setDefaultResourceGroup("Schemes");
	  CEGUI::Imageset::setDefaultResourceGroup("Imagesets");
	  CEGUI::Font::setDefaultResourceGroup("Fonts");
	  CEGUI::WindowManager::setDefaultResourceGroup("Layouts");
	  CEGUI::WidgetLookManager::setDefaultResourceGroup("LookNFeel");

	  CEGUI::SchemeManager::getSingleton().create("TaharezLook.scheme");
	  CEGUI::System::getSingleton().setDefaultFont("DejaVuSans-10");
	  CEGUI::System::getSingleton().setDefaultMouseCursor("TaharezLook","MouseArrow");
}

bool
GameManager::initSDL() {
    // Inicializando SDL...
    if (SDL_Init(SDL_INIT_AUDIO) < 0)
        return false;
    // Llamar a  SDL_Quit al terminar.
    atexit(SDL_Quit);

    // Inicializando SDL mixer...
    if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT,MIX_DEFAULT_CHANNELS, 4096) < 0)
      return false;

    // Llamar a Mix_CloseAudio al terminar.
    atexit(Mix_CloseAudio);

    // Se inicializan los gestores de sonido (track y FX)
    GameManager::_pTrackManager = new TrackManager;
    GameManager::_pSoundFXManager = new SoundFXManager;
    new GameSoundFX;
    return true;
}

TrackManager*
GameManager::getTrackManager() {
	assert(GameManager::_pTrackManager);
	return GameManager::_pTrackManager;
}
SoundFXManager*
GameManager::getSoundFXManager(){
	assert(GameManager::_pSoundFXManager);
	return GameManager::_pSoundFXManager;
}
