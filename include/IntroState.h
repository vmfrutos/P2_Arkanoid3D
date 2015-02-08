#ifndef IntroState_H
#define IntroState_H

#include <Ogre.h>
#include <OIS/OIS.h>
#include <OgreSingleton.h>
#include <RendererModules/Ogre/CEGUIOgreRenderer.h>
#include <CEGUI.h>

#include "GameState.h"
#include "RecordsState.h"
#include "CreditsState.h"
#include "SoundFXManager.h"
#include "TrackManager.h"

class IntroState : public Ogre::Singleton<IntroState>, public GameState
{
public:
	IntroState();
	virtual ~IntroState();

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
	static IntroState& getSingleton ();
	static IntroState* getSingletonPtr ();

	// Eventos CEGUI
	bool mouseInButton(const CEGUI::EventArgs &e);
	bool mouseOutButton(const CEGUI::EventArgs &e);
	bool clickButton(const CEGUI::EventArgs &e);

protected:
	Ogre::Root* _root;
	Ogre::RenderWindow* _win;
	Ogre::SceneManager* _sceneMgr;
	Ogre::Viewport* _viewport;
	Ogre::Camera* _camera;
	Ogre::SceneNode* _nodeCamara;
	Ogre::SceneNode* _nodeFoco;

	// Sonido
	TrackPtr _audioIntro;

	bool _exitState;

	unsigned int _numArboles;
	unsigned int _numArbolesSecos;


	CEGUI::Window* _sheet;
	CEGUI::Window* _menu;

 private:
	void  createScene();
};

#endif
