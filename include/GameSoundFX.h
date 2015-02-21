#ifndef GameSoundFX_H
#define GameSoundFX_H

#include "Ogre.h"
#include "SoundFXManager.h"
#include "GameManager.h"

using namespace std;

class GameSoundFX : public Ogre::Singleton<GameSoundFX> {
public:

	GameSoundFX();
	~GameSoundFX();

	// Heredados de Ogre::Singleton.
	static GameSoundFX& getSingleton ();
	static GameSoundFX* getSingletonPtr ();
	void setSoundFX(int fx);
	static const int FX_BOLA_STRUCTURA = 1;
	static const int FX_BOLA_LADRILLO = 2;
	static const int FX_BOLA_PALA = 3;
	static const int FX_BOLA_SUELO = 4;
	static const int FX_PALA_ITEM = 5;

private:




	SoundFXPtr _audioBallStructure;
	SoundFXPtr _audioBallBrick;
	SoundFXPtr _audioBallPaddle;
	SoundFXPtr _audioBallSuelo;
	SoundFXPtr _audioPaddleItem;


};
#endif
