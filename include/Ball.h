#ifndef Ball_H
#define Ball_H

#include <Ogre.h>
#include "PlayState.h"
#include "GameSoundFX.h"

class PlayState;
class Ball {
public:
	Ball(Ogre::SceneManager* sceneMgr,PlayState* playstate);
	~Ball();
	void initSetUp();
	inline bool isMove() {return _enMovimiento;}
	void setMove(bool _move);
	void move(float timeSinceLastFrame);
	void setNodeParent(Ogre::SceneNode* scenNode);
	inline Ogre::SceneNode* getSceneNode() {return _nodeBola;}
	void hide();
	void incSpeedBall();
	void decSpeedBall();
	bool slowBallTimer(int segundos, Ogre::String itemName);
	void reset();
	bool isBusy(Ogre::String itemName);


private:
	Ogre::SceneManager* _sceneMgr;
	Ogre::SceneNode* _nodeBola;
	Ogre::Entity* _entBola;
	Ogre::IntersectionSceneQuery* _intersectionSceneQuery;
	PlayState* _playState;

	bool _enMovimiento;
	int _speed;
	Ogre:: Vector3 _direccion;
	Ogre::String _ItemBlock;
	bool _timerAvtive;
	Contador _timer;


	bool isCollision(const Ogre::Vector3& position, const Ogre::Vector3& direction, Ogre::String& nombre);

	const int COLISION_PARED_IZQUIERDA = 0;
	const int COLISION_PARED_DERECHA = 1;
	const int COLISION_PARED_TECHO = 2;
	const int COLISION_PALA_I = 3;
	const int COLISION_PALA_D = 4;
	const int COLISION_PALA_C = 5;
	const int COLISION_PALA_EXT_I = 6;
	const int COLISION_PALA_EXT_D = 7;
	const int COLISION_SUELO = 8;
	const int COLISION_LADRILLO = 9;
	bool _comprobarColision[10];

	const int SPEED_BALL_NORMAL = 14;
	const int SPEED_BALL_SLOW = 8;



};

#endif
