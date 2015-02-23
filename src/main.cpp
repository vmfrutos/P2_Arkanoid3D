#define UNUSED_VARIABLE(x) (void)x

#include "GameManager.h"
#include "IntroState.h"
#include "PlayState.h"
#include "RecordsState.h"
#include "RecordsManager.h"

#include <iostream>
#include "Properties.h"


using namespace std;

int main (int argc, char**argv) {

  GameManager* game = new GameManager();
  IntroState* introState = new IntroState();
  PlayState* playState = new PlayState();
  RecordState* recordState = new RecordState();
  CreditsState* creditsState = new CreditsState();
  RecordsManager* recordsManager = new RecordsManager();



  UNUSED_VARIABLE(introState);
  UNUSED_VARIABLE(playState);
  UNUSED_VARIABLE(recordState);
  UNUSED_VARIABLE(creditsState);
  UNUSED_VARIABLE(recordsManager);
    
  try
    {
      // Inicializa el juego y transición al primer estado.
      game->start(IntroState::getSingletonPtr());;
    }
  catch (Ogre::Exception& e)
    {
      std::cerr << "Excepción detectada: " << e.getFullDescription();
    }


  if (recordsManager) delete recordsManager;
  if (recordState) delete recordState;
  if (creditsState) delete creditsState;
  if (playState)  delete playState;
  if (introState) delete introState;



  return 0;
}
