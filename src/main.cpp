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

#define UNUSED_VARIABLE(x) (void)x

#include "GameManager.h"
#include "IntroState.h"
#include "PlayState.h"
#include "PauseState.h"
#include "RecordsState.h"
#include "RecordsManager.h"

#include <iostream>

using namespace std;

int main () {


  GameManager* game = new GameManager();
  IntroState* introState = new IntroState();
  PlayState* playState = new PlayState();
  PauseState* pauseState = new PauseState();
  RecordState* recordState = new RecordState();
  CreditsState* creditsState = new CreditsState();
  RecordsManager* recordsManager = new RecordsManager();



  UNUSED_VARIABLE(introState);
  UNUSED_VARIABLE(playState);
  UNUSED_VARIABLE(pauseState);
  UNUSED_VARIABLE(recordState);
  UNUSED_VARIABLE(creditsState);
  UNUSED_VARIABLE(recordsManager);
    
  try
    {
      // Inicializa el juego y transición al primer estado.
      game->start(IntroState::getSingletonPtr());
    }
  catch (Ogre::Exception& e)
    {
      std::cerr << "Excepción detectada: " << e.getFullDescription();
    }

  cout << "MAIN_01" << endl;
  delete recordsManager;
  cout << "MAIN_02" << endl;
  delete recordState;
  cout << "MAIN_03" << endl;
  delete creditsState;
  cout << "MAIN_04" << endl;
  //delete pauseState;
  //delete playState;
  //delete introState;
  delete game;
  cout << "MAIN_05" << endl;


  return 0;
}
