
#ifndef CREDITS_STATE_H
#define CREDITS_STATE_H

#include <Ogre.h>
#include <OIS/OIS.h>
#include <OgreSingleton.h>
#include <RendererModules/Ogre/CEGUIOgreRenderer.h>
#include <CEGUI.h>
#include <string>
#include "GameState.h"
#include "IntroState.h"

using namespace std;

class CreditsState : public Ogre::Singleton<CreditsState>, public GameState
{
 public:
	CreditsState();
	virtual ~CreditsState();


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
  static CreditsState& getSingleton ();
  static CreditsState* getSingletonPtr ();


 protected:
  Ogre::Root* _root;
  Ogre::RenderWindow* _win;
  Ogre::SceneManager* _sceneMgr;
  Ogre::Viewport* _viewport;
  Ogre::Camera* _camera;

  bool _exitState;

  CEGUI::Window* _sheet;
  CEGUI::Window* _menu;


 private:
 void  createScene();
 string ReadTextFromFile();
 bool mouseInButton(const CEGUI::EventArgs &e);
 bool mouseOutButton(const CEGUI::EventArgs &e);
 bool clickButton(const CEGUI::EventArgs &e);

 // Sonido
 TrackPtr _audioCredits;

 const string _CREDITS_FILE = "creditos.txt";
};

#endif
