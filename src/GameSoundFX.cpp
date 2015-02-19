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

	if (FX_BOLA_STRUCTURA == fx) {
		_audioBallStructure->play(0);
	} else if (FX_BOLA_PALA == fx) {
		_audioBallPaddle->play(0);
	} else if (FX_BOLA_LADRILLO == fx) {
		_audioBallBrick->play(0);
	} else if (FX_BOLA_SUELO == fx) {
		int channel = _audioBallSuelo->play(0);
		while (_audioBallSuelo->isPlaying(channel) == true) {
			sleep(0.1);
		}
	} else if (FX_PALA_ITEM == fx) {
		_audioPaddleItem->play(0);
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
