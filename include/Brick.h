#ifndef Brick_H
#define Brick_H

#include "Item.h"
#include "Ogre.h"

class Brick {

public:

	Brick(Ogre::SceneManager* sceneMgr, int color, int resistenia, Item* item, int num);
	~Brick();
	void initSetUp();
	inline int getColor() {return _color;}
	inline int getResistencia() {return _resistencia;}
	inline Item* getItem() {return _item;}

	void setPosition(Ogre::Vector3 position);

	const int COLOR_ROJO = 0;
	const int COLOR_VERDE = 1;
	const int COLOR_AMARILLO = 2;
	const int COLOR_AZUL = 3;




private:
	Ogre::SceneManager* _sceneMgr;
	Ogre::Entity* _entBrickTop;
	Ogre::Entity* _entBrickBottom;
	Ogre::Entity* _entBrickRigth;
	Ogre::Entity* _entBrickLeft;

	Ogre::SceneNode* _nodeBrick;

	int _resistencia;
	int _color;
	Item* _item;
	int _numero;
	Ogre::Vector3 _position;


};
#endif
