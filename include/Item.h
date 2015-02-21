#ifndef Item_H
#define Item_H

#include "Ogre.h"
#include "PlayState.h"

using namespace std;

class Item {

public:
	Item(Ogre::SceneManager* sceneMgr, int tipo, Ogre::Vector3 posicionInicial, const Ogre::String& nombre);
	~Item();

	void refresh(float timeSinceLastFrame);
	void iniciar();
	bool isCollisionWihtStructure(Ogre::String& name);
	void incrementarVida();
	void ampliarPala(int segundos);
	void reset();
	void redicirVelocidadBola(int segundos);
	void setMaterial(const Ogre::String& matName);


	const int ITEM_TIPO_VIDA_EXTRA = 1;
	const int ITEM_TIPO_PALA_20 = 2;
	const int ITEM_TIPO_PALA_60 = 3;
	const int ITEM_TIPO_BOLA_LENTA_15 = 4;

	const int ITEM_STATUS_DORMIDO = 0;
	const int ITEM_STATUS_CAYENDO = 1;
	const int ITEM_STATUS_ACTIVO = 2;
	const int ITEM_STATUS_FINALIZADO = 3;

private:
	Ogre::SceneManager* _sceneMgr;
	Ogre::Entity* _entItem;
	Ogre::SceneNode* _nodeItem;
	Ogre::IntersectionSceneQuery* _intersectionSceneQuery;
	Ogre::AnimationState *_animItem;

	int _tipo;
	int _status;
	int _speed;
	Ogre::String _name;


};
#endif
