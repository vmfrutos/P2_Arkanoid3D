#ifndef PlayState_H
#define PlayState_H

#include <Ogre.h>
#include <OIS/OIS.h>

#include "GameState.h"
#include "IntroState.h"
#include "Ball.h"
#include "Paddle.h"
#include "Structure.h"
#include "Ground.h"
#include "Wall.h"


class Ball;
class Wall;
class PlayState : public Ogre::Singleton<PlayState>, public GameState
{
 public:

	PlayState ();
	~PlayState ();
	void enter ();
	void exit ();
	void pause ();
	void resume ();

	void keyPressed (const OIS::KeyEvent &e);
	void keyReleased (const OIS::KeyEvent &e);

	void mouseMoved (const OIS::MouseEvent &e);
	void mousePressed (const OIS::MouseEvent &e, OIS::MouseButtonID id);
	void mouseReleased (const OIS::MouseEvent &e, OIS::MouseButtonID id);

	bool frameStarted (const Ogre::FrameEvent& evt);
	bool frameEnded (const Ogre::FrameEvent& evt);

	// Heredados de Ogre::Singleton.
	static PlayState& getSingleton ();
	static PlayState* getSingletonPtr ();

	void quitarVida();
	void reset();
	void mostrarNivel();
	void mostrarFinJuego();
	void incMarcador(long valor);
	void incVida();


	Wall* getMuro();
	Paddle* getPala();
	Ball* getBola();

 protected:
	Ogre::Root* _root;
	Ogre::RenderWindow* _win;
	Ogre::SceneManager* _sceneMgr;
	Ogre::Viewport* _viewport;
	Ogre::Camera* _camera;

	bool _exitGame;

 private:

	void createScene();
	bool clickFinishButton(const CEGUI::EventArgs &e);
	void gameOver();


	// Variables para CEGUI
	CEGUI::Window* _sheet;
	CEGUI::Window* _hud;
	CEGUI::Window* _endScreen;
	CEGUI::Window* _showMsg;

	CEGUI::Window* _timeWidget;
	CEGUI::Window* _fpsWidget;

	Contador _reloj;
	int _numVidas;

	Ball* _bola;
	Paddle* _pala;
	Structure* _structura;
	Ground* _suelo;
	Wall* _muro;
	int _state;
	int _level;
	long _score;


	bool _izdaPulsado;
	bool _dchaPulsado;
	bool _rotPulsado;


	Ogre::SceneNode* _nodeCamara;

	// ESTADOS DE LA PARTIDA
	const int STATE_PLAYING = 0;
	const int STATE_SHOW_LEVEL = 1;
	const int STATE_END = 2;
	const int STATE_PAUSE = 3;
	const int STATE_SET_RECORD = 4;


};
#endif
