#ifndef Ball_H
#define Ball_H

#include <Ogre.h>
#include "PlayState.h"

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

private:
	Ogre::SceneManager* _sceneMgr;
	Ogre::SceneNode* _nodeBola;
	Ogre::Entity* _entBola;
	Ogre::RaySceneQuery* _raySceneQuery;
	PlayState* _playState;


	bool _enMovimiento;
	int _speed;
	Ogre:: Vector3 _direccion;

	bool isCollisionWihtStructure(const Ogre::Vector3& position, const Ogre::Vector3& direction);

	const int COLISION_PARED_IZQUIERDA = 0;
	const int COLISION_PARED_DERECHA = 1;
	const int COLISION_PARED_TECHO = 2;
	const int COLISION_PALA_I = 3;
	const int COLISION_PALA_D = 4;
	const int COLISION_PALA_C = 5;
	const int COLISION_SUELO = 6;
	bool _comprobarColision[7];

};

#endif
