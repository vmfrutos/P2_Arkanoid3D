/*********************************************************************
 * Módulo 1. Curso de Experto en Desarrollo de Videojuegos
 * Autor: David Vallejo Fernández    David.Vallejo@uclm.es
 *
 * Código modificado a partir de Managing Game States with OGRE
 * http://www.ogre3d.org/tikiwiki/Managing+Game+States+with+OGRE
 * Inspirado en Managing Game States in C++
 * http://gamedevgeek.com/tutorials/managing-game-states-in-c/
 *
 * You can redistribute and/or modify this file under the terms of the
 * GNU General Public License ad published by the Free Software
 * Foundation, either version 3 of the License, or (at your option)
 * and later version. See <http://www.gnu.org/licenses/>.
 *
 * This file is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.  
 *********************************************************************/

#ifndef RECORDS_STATE_H
#define RECORDS_STATE_H

#include <Ogre.h>
#include <OIS/OIS.h>
#include <OgreSingleton.h>
#include <RendererModules/Ogre/CEGUIOgreRenderer.h>
#include <CEGUI.h>

#include "GameState.h"
#include "IntroState.h"
#include "RecordsManager.h"
#include "Record.h"

class RecordState : public Ogre::Singleton<RecordState>, public GameState
{
 public:
	RecordState();
	virtual ~RecordState();


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
  static RecordState& getSingleton ();
  static RecordState* getSingletonPtr ();


 protected:
  Ogre::Root* _root;
  Ogre::RenderWindow* _win;
  Ogre::SceneManager* _sceneMgr;
  Ogre::Viewport* _viewport;
  Ogre::Camera* _camera;

  bool _exitState;

  CEGUI::Window* _sheet;
  CEGUI::Window* _menu;

  Ogre::AnimationState *_animTextoJugador;
  Ogre::AnimationState *_animTextoNivel;
  Ogre::AnimationState *_animTextoTiempo;


 private:
 void  createScene();

 bool mouseInButton(const CEGUI::EventArgs &e);
 bool mouseOutButton(const CEGUI::EventArgs &e);
 bool clickButton(const CEGUI::EventArgs &e);

 // Sonido
 TrackPtr _audioRecords;
};

#endif
