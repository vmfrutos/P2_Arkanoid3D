#include "GameSoundFX.h"

template<> GameSoundFX* Ogre::Singleton<GameSoundFX>::msSingleton = 0;
GameSoundFX::GameSoundFX() {
	_audioBallStructure = GameManager::getSoundFXManager()->load("metal.wav");
	_audioBallBrick = GameManager::getSoundFXManager()->load("bolaLadrillo.wav");
	_audioBallPaddle = GameManager::getSoundFXManager()->load("bolaPala.wav");
	_audioBallSuelo = GameManager::getSoundFXManager()->load("gameOver.wav");
	_audioPaddleItem = GameManager::getSoundFXManager()->load("palaItem.wav");
}
GameSoundFX::~GameSoundFX(){

}

void
GameSoundFX::setSoundFX(int fx) {
	int channel = 0;
	if (FX_BOLA_STRUCTURA == fx) {
		 channel = _audioBallStructure->play(FX_BOLA_STRUCTURA,0);
		 cout << "Usando canal: "<< channel << endl;
	} else if (FX_BOLA_PALA == fx) {
		 channel = _audioBallPaddle->play(FX_BOLA_PALA,0);
		 cout << "Usando canal: "<< channel << endl;
	} else if (FX_BOLA_LADRILLO == fx) {
		 channel = _audioBallBrick->play(FX_BOLA_LADRILLO,0);
		 cout << "Usando canal: "<< channel << endl;
	} else if (FX_BOLA_SUELO == fx) {
		int channel = _audioBallSuelo->play(FX_BOLA_SUELO,0);
		 cout << "Usando canal: "<< channel << endl;
		while (_audioBallSuelo->isPlaying(channel) == true) {
			sleep(0.1);
		}
	} else if (FX_PALA_ITEM == fx) {
		 channel = _audioPaddleItem->play(FX_PALA_ITEM,0);
		 cout << "Usando canal: "<< channel << endl;
	}

}

GameSoundFX&
GameSoundFX::getSingleton (){
	assert(msSingleton);
	return *msSingleton;
}

GameSoundFX*
GameSoundFX::getSingletonPtr (){
	return msSingleton;
}
