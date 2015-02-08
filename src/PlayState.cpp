#include "PlayState.h"


template<> PlayState* Ogre::Singleton<PlayState>::msSingleton = 0;

PlayState::PlayState ()
{
	_root = NULL;
	_win = NULL;
	_sceneMgr = NULL;
	_viewport = NULL;
	_camera = NULL;
	_exitGame = NULL;
	_nodeCamara = NULL;

	_sheet = NULL;
	_hud = NULL;
	_endScreen = NULL;

	_timeWidget = NULL;
	_fpsWidget = NULL;

	_izdaPulsado = false;
	_dchaPulsado = false;
	_rotPulsado = false;

	_bola = NULL;
	_pala = NULL;
	_structura = NULL;
	_suelo = NULL;
	_muro = NULL;

	_numVidas = 3;

	_state = STATE_PLAYING;
}

PlayState::~PlayState ()
{
	cout << "~PlayState" << endl;
}

void
PlayState::enter ()
{
	_state = STATE_PLAYING;
	_numVidas = 3;

	_root = Ogre::Root::getSingletonPtr();
	_win = _root->getAutoCreatedWindow();
	_sceneMgr = _root->createSceneManager(Ogre::ST_GENERIC, "SceneManagerPlay");

	// Se define una camara
	_camera = _sceneMgr->createCamera("PlayCamera");
	_camera->setPosition(Ogre::Vector3(28.37,12.38,-25.50));
	_camera->lookAt(Ogre::Vector3(0,5.85,0));
	_camera->setFOVy (Ogre::Degree(50));
	_camera->setNearClipDistance(1);
	_camera->setFarClipDistance(300);

	_viewport = _win->addViewport(_camera);
	_viewport->setBackgroundColour(Ogre::ColourValue(0.016, 0.454, 0.467));


	// Se inicializa el layout de CEGUI
	//Sheet
	_sheet = CEGUI::WindowManager::getSingleton().createWindow("DefaultWindow","Sheet");

	//Config Window
	_endScreen = CEGUI::WindowManager::getSingleton().loadWindowLayout("endScreenArkanoid.layout");
	_hud  = CEGUI::WindowManager::getSingleton().loadWindowLayout("hudArkanoid.layout");

	// Se crean los ImageSet
	CEGUI::ImagesetManager::getSingleton().createFromImageFile("ImageBackGroud", "fondoHud.png");
	CEGUI::ImagesetManager::getSingleton().createFromImageFile("ImageLive", "acid.png");
	CEGUI::ImagesetManager::getSingleton().createFromImageFile("ImageScore", "score.png");
	CEGUI::ImagesetManager::getSingleton().createFromImageFile("Imagelevel", "level.png");
	CEGUI::ImagesetManager::getSingleton().createFromImageFile("ImageTime", "time.png");
	CEGUI::ImagesetManager::getSingleton().createFromImageFile("ImageFps", "fps.png");
	CEGUI::ImagesetManager::getSingleton().createFromImageFile("ImageEndScreen", "endScreen.png");


	// Se inilizan los widgets del Hud
	CEGUI::Window* fondoHud = _hud->getChild("Hud/Fondo");
	CEGUI::Window* vida1 = fondoHud->getChild("Fondo/Vida1");
	CEGUI::Window* vida2 = fondoHud->getChild("Fondo/Vida2");
	CEGUI::Window* vida3 = fondoHud->getChild("Fondo/Vida3");
	CEGUI::Window* scoreBox = fondoHud->getChild("Fondo/ScoreBox");
	CEGUI::Window* levelBox = fondoHud->getChild("Fondo/LevelBox");
	CEGUI::Window* timeBox = fondoHud->getChild("Fondo/TimeBox");
	CEGUI::Window* fpsBox = fondoHud->getChild("Fondo/FpsBox");

	fondoHud->setProperty("Image", "set:ImageBackGroud image:full_image");
	vida1->setProperty("Image", "set:ImageLive image:full_image");
	vida2->setProperty("Image", "set:ImageLive image:full_image");
	vida3->setProperty("Image", "set:ImageLive image:full_image");
	scoreBox->setProperty("Image", "set:ImageScore image:full_image");
	levelBox->setProperty("Image", "set:Imagelevel image:full_image");
	timeBox->setProperty("Image", "set:ImageTime image:full_image");
	fpsBox->setProperty("Image", "set:ImageFps image:full_image");


	_timeWidget = timeBox->getChild("TimeBox/TimeText");
	_fpsWidget = fpsBox->getChild("FpsBox/FpsText");

	// Se inicializan los widgets dela ventana endScreen
	CEGUI::Window* fondoEndScreen = _endScreen->getChild("EndScreen/Fondo");
	fondoEndScreen->setProperty("Image", "set:ImageEndScreen image:full_image");
	_endScreen->getChild("EndScreen/Fondo")->getChild("EndScreen/Salir")->subscribeEvent(CEGUI::PushButton::EventClicked,CEGUI::Event::Subscriber(&PlayState::clickFinishButton,this));


	_sheet->addChildWindow(_hud);
	_sheet->addChildWindow(_endScreen);
	CEGUI::System::getSingleton().setGUISheet(_sheet);

	_exitGame = false;

	_reloj.reset();

	createScene();

	CEGUI::MouseCursor::getSingleton().hide();
	_endScreen->hide();

}

void
PlayState::exit ()
{
	CEGUI::MouseCursor::getSingleton().show();
	if (_bola) {
		delete _bola;
		_bola = NULL;
	}
	if (_pala) {
		delete _pala;
		_pala = NULL;
	}
	if (_structura)  {
		delete _structura;
		_structura = NULL;
	}
	if (_suelo) {
		delete _suelo;
		_suelo = NULL;
	}


	if (_hud) _hud->destroy();
	if (_endScreen) _endScreen->destroy();
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
PlayState::pause()
{
}

void
PlayState::resume()
{

}

bool
PlayState::frameStarted
(const Ogre::FrameEvent& evt)
{
	Ogre::Vector3 vt(0,0,0);
	float delta = evt.timeSinceLastFrame;
	int fps = 0.0;

	if (delta == 0.0) {
		fps = 1000; // esto es por evitar división por 0 en equipos muy rápidos
	} else {
		fps = 1.0f / delta;
	}

	// Se actualizan los fps del hud
	_fpsWidget->setText(Ogre::StringConverter::toString(fps));

	if (_state == STATE_PLAYING) {

		// Se actualiza el reloj del hud
		_reloj.incrementar(delta);
		_timeWidget->setText(_reloj.getContadorStr());


		// Se comprueba si hay que rotar el suelo
		if (_rotPulsado) {
			Ogre::Quaternion incremento(Ogre::Degree(90 * delta), Ogre::Vector3::UNIT_Y);
			_nodeCamara->rotate(incremento,Ogre::Node::TS_WORLD);
		}

		if (_dchaPulsado) {
			_pala->moveRight(delta);
		}

		if (_izdaPulsado) {
			_pala->moveLeft(delta);
		}

		// Se mueve la bola esta en estado de movimiento
		if (_bola->isMove()) {
			_bola->move(delta);
		}
	} else if (_state == STATE_END) {
		// Se muestra la ventana de fin de partida
	}  else if (_state == STATE_PAUSE) {
		// Se muestra la ventana de pausa
	}


  return true;
}

bool
PlayState::frameEnded
(const Ogre::FrameEvent& evt)
{
  if (_exitGame)
    changeState(IntroState::getSingletonPtr());
  return true;
}

void
PlayState::keyPressed
(const OIS::KeyEvent &e)
{
	CEGUI::System::getSingleton().injectKeyDown(e.key);
	CEGUI::System::getSingleton().injectChar(e.text);

  // Tecla p --> PauseState.
  if (e.key == OIS::KC_P) {
    if (_state == STATE_PAUSE) {
    	_state = STATE_PLAYING;
    } else if (_state == STATE_PLAYING) {
    	_state = STATE_PAUSE;
    }
  }

  if (e.key == OIS::KC_LEFT) {
    _izdaPulsado = true;
  }

  if (e.key == OIS::KC_RIGHT) {
    _dchaPulsado = true;
  }

  if (e.key == OIS::KC_R) {
    _rotPulsado = true;
  }

  if (e.key == OIS::KC_SPACE) {
	//Si no está en movimiento se activa el flag de movimiento de la bola
	// y se cambia el nodo padre
	if (!_bola->isMove()) {
		_bola->setMove(true);
		_bola->setNodeParent(_suelo->getSceneNode());
	}
  }


}

void
PlayState::keyReleased
(const OIS::KeyEvent &e)
{
	CEGUI::System::getSingleton().injectKeyUp(e.key);

  if (e.key == OIS::KC_ESCAPE) {
    _exitGame = true;
  }

  if (e.key == OIS::KC_LEFT) {
    _izdaPulsado = false;
  }

  if (e.key == OIS::KC_RIGHT) {
    _dchaPulsado = false;
  }

  if (e.key == OIS::KC_R) {
    _rotPulsado = false;
  }

}

void
PlayState::mouseMoved
(const OIS::MouseEvent &evt)
{
	CEGUI::System::getSingleton().injectMouseMove(evt.state.X.rel, evt.state.Y.rel);
}

void
PlayState::mousePressed
(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
	CEGUI::System::getSingleton().injectMouseButtonDown(convertMouseButton(id));
}

void
PlayState::mouseReleased
(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
	CEGUI::System::getSingleton().injectMouseButtonUp(convertMouseButton(id));
}

PlayState*
PlayState::getSingletonPtr ()
{
return msSingleton;
}

PlayState&
PlayState::getSingleton ()
{ 
  assert(msSingleton);
  return *msSingleton;
}

void
PlayState::createScene() {

	// Se establece la iluminacion
	_sceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_TEXTURE_MODULATIVE);
	_sceneMgr->setShadowColour(Ogre::ColourValue(0.5, 0.5, 0.5) );
	_sceneMgr->setAmbientLight(Ogre::ColourValue(0.9, 0.9, 0.9));
	_sceneMgr->setShadowTextureSize(16384);

	Ogre::Light* light1 = _sceneMgr->createLight("Light1");
	light1->setPosition(0,4,-2);
	light1->setDirection(Ogre::Vector3(-1.0,-1.40,-1.0));
	light1->setType(Ogre::Light::LT_DIRECTIONAL);
	light1->setPowerScale(1.7299998998641968);
	light1->setCastShadows(true);

	_nodeCamara = _sceneMgr->createSceneNode("camara");
	_nodeCamara->attachObject(_camera);
	_sceneMgr->getRootSceneNode()->addChild(_nodeCamara);


	// Se inicializa el suelo
	_suelo = new Ground(_sceneMgr);
	_suelo->initSetUp();
	_suelo->setNodeParent(_sceneMgr->getRootSceneNode());

	// Se inicializa la estructura
	_structura = new Structure(_sceneMgr);
	_structura->initSetUp();
	_structura->setNodeParent(_suelo->getSceneNode());

	// Se inicializa la pala
	_pala = new Paddle(_sceneMgr);
	_pala->initSetUp();
	_pala->setNodeParent(_suelo->getSceneNode());

	// Se inicializa la bola
	_bola = new Ball(_sceneMgr,this);
	_bola->initSetUp();
	_bola->setNodeParent(_pala->getSceneNode());

	// Se inicializa el muro
	_muro = new Wall(_sceneMgr);
	_muro->loadLevel();

}

void
PlayState::quitarVida() {
	_numVidas--;
	if (_numVidas == 0) {
		gameOver();
	} else {
		reset();

		if (_numVidas == 2)
			_hud->getChild("Hud/Fondo")->getChild("Fondo/Vida1")->hide();
		else if (_numVidas == 1)
			_hud->getChild("Hud/Fondo")->getChild("Fondo/Vida2")->hide();
	}
}

void
PlayState::reset() {
		_pala->setNodeParent(_suelo->getSceneNode());
		_pala->initSetUp();

		_bola->setNodeParent(_pala->getSceneNode());
		_bola->initSetUp();
}

void
PlayState::gameOver() {

	_hud->getChild("Hud/Fondo")->getChild("Fondo/Vida3")->hide();
	// Fin de partida
	_state = STATE_END;
	_bola->hide();
	_pala->hide();

	CEGUI::String score = _hud->getChild("Hud/Fondo")->getChild("Fondo/ScoreBox")->getChild("ScoreBox/ScoreText")->getText();
	CEGUI::String level = _hud->getChild("Hud/Fondo")->getChild("Fondo/LevelBox")->getChild("LevelBox/LevelText")->getText();
	CEGUI::String time = _reloj.getContadorStr();

	_endScreen->getChild("EndScreen/Fondo")->getChild("EndScreen/ScoreText")->setText(score);
	_endScreen->getChild("EndScreen/Fondo")->getChild("EndScreen/LevelText")->setText(level);
	_endScreen->getChild("EndScreen/Fondo")->getChild("EndScreen/TimeText")->setText(time);

	CEGUI::MouseCursor::getSingleton().show();
	_endScreen->show();

}

bool
PlayState::clickFinishButton(const CEGUI::EventArgs &e){

	string level(_endScreen->getChild("EndScreen/Fondo")->getChild("EndScreen/LevelText")->getText().c_str());
	string time(_endScreen->getChild("EndScreen/Fondo")->getChild("EndScreen/TimeText")->getText().c_str());
	string nickName(_endScreen->getChild("EndScreen/Fondo")->getChild("EndScreen/NickNameEdit")->getText().c_str());

	Record record;
	record.setNombre(nickName);
	record.setNivel(std::atoi(level.c_str()));
	record.setSegundosTranscurridos(_reloj.getSegundosTranscurridos());
	RecordsManager::getSingletonPtr()->insertarRecord(record);

	changeState(IntroState::getSingletonPtr());

	return true;
}

