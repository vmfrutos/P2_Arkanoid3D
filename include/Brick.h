#ifndef Brick_H
#define Brick_H

#include "Item.h"
#include "Ogre.h"
class Item;

class Brick {

public:

	Brick(Ogre::SceneManager* sceneMgr, int color, int fila, int columna);
	~Brick();
	void initSetUp();
	void hide();
	bool isVisible();


	void setPosition(Ogre::Vector3 position);

	void setResistencia(int valor);
	int getResistencia();
	void romper();
	void setItem(int item);
	Item* getItem();
	void refresh(float timeSinceLastFrame);
	bool estaRoto();

	const int COLOR_INVISIBLE = 0;
	const int COLOR_ROJO = 1;
	const int COLOR_VERDE = 2;
	const int COLOR_AMARILLO = 3;
	const int COLOR_AZUL = 4;

	const int ITEM_NO_ITEM = 0;
	const int ITEM_VIDA_EXTRA = 1;
	const int ITEM_AMPLIA_PALA_20 = 2;
	const int ITEM_AMPLIA_PALA_60 = 3;





private:
	Ogre::SceneManager* _sceneMgr;
	Ogre::Entity* _entBrick;

	Ogre::SceneNode* _nodeBrick;
	Ogre::AnimationState *_animLadrillo;

	int _resistencia;
	int _color;
	int _tipoItem;
	Item* _item;
	int _fila;
	int _columna;
	Ogre::Vector3 _position;
	bool _visible;
	int _state;

	const int ANIM_ACTIVE = 0;
	const int ANIM_NO_ACTIVE = 1;


	void setMaterial(const Ogre::String& matName);


};
#endif
