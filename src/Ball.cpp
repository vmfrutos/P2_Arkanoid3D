#include "Ball.h"


Ball::Ball(Ogre::SceneManager* sceneMgr,PlayState* playstate){
	_playState = playstate;
	_sceneMgr = sceneMgr;
	_entBola = _sceneMgr->createEntity("Bola", "Bola.mesh");
	_entBola->setCastShadows(true);
	_nodeBola = _sceneMgr->createSceneNode("bola");
	_nodeBola->attachObject(_entBola);
	_enMovimiento = false;
	_speed = SPEED_BALL_NORMAL;
	//_raySceneQuery = _sceneMgr->createRayQuery(Ogre::Ray());
	_intersectionSceneQuery =  _sceneMgr->createIntersectionQuery();
    //_raySceneQuery->setSortByDistance(true);
	srand (time(NULL));
	_ItemBlock = "";
	_timerAvtive = false;
	_timer.reset();

}

Ball::~Ball() {

}

void
Ball::initSetUp() {
	_enMovimiento = false;
	_comprobarColision[COLISION_PARED_IZQUIERDA] = true;
	_comprobarColision[COLISION_PARED_DERECHA] = true;
	_comprobarColision[COLISION_PARED_TECHO] = true;
	_comprobarColision[COLISION_PALA_I] = false;
	_comprobarColision[COLISION_PALA_D] = false;
	_comprobarColision[COLISION_PALA_C] = false;
	_comprobarColision[COLISION_PALA_EXT_I] = false;
	_comprobarColision[COLISION_PALA_EXT_D] = false;
	_comprobarColision[COLISION_SUELO] = false;
	_comprobarColision[COLISION_LADRILLO] = true;
	_nodeBola->setPosition(0.0,0.6,0.0);
	_direccion = Ogre::Vector3(0,1,0);
}


void
Ball::setMove(bool move) {
	_enMovimiento = move;
}

void
Ball::move(float timeSinceLastFrame) {

	// Se incrementa el timer si está activo
	if (_timerAvtive) {
		_timer.incrementar(timeSinceLastFrame);
	}

	Ogre::String nombre;
	// Si hay colision con la estructura cambia la direccion
	if (isCollision(_nodeBola->getPosition(),_direccion, nombre)) {

		// si ha colisionado con pared izquierda
		if (!_comprobarColision[COLISION_PARED_IZQUIERDA]) {
			_direccion = Ogre::Vector3(-_direccion.x,_direccion.y,0.0);
			GameSoundFX::getSingletonPtr()->setSoundFX(GameSoundFX::FX_BOLA_STRUCTURA);
		}
		// si ha colisionado con pared derecha
		else if (!_comprobarColision[COLISION_PARED_DERECHA]) {
			_direccion = Ogre::Vector3(-_direccion.x,_direccion.y,0.0);
			GameSoundFX::getSingletonPtr()->setSoundFX(GameSoundFX::FX_BOLA_STRUCTURA);
		}

		// si ha colisionado con techo
		else if (!_comprobarColision[COLISION_PARED_TECHO]) {
			_direccion = Ogre::Vector3(_direccion.x,-_direccion.y,0.0);
			GameSoundFX::getSingletonPtr()->setSoundFX(GameSoundFX::FX_BOLA_STRUCTURA);
		}

		// si ha colisionado con la pala izquierda
		else if (!_comprobarColision[COLISION_PALA_I] || !_comprobarColision[COLISION_PALA_EXT_I]) {
			Ogre::Vector3 _direccionrefleja;
			float desvio = 0.0;
			if (_direccion.x >=0 && _direccion.y <=0) {
				_direccionrefleja = Ogre::Vector3(-_direccion.x,-_direccion.y,0.0);

			} else {
				_direccionrefleja = Ogre::Vector3(_direccion.x,-_direccion.y,0.0);
				desvio = ((rand() % 5))/100.0;
			}
			_direccion = _direccionrefleja - Ogre::Vector3(desvio,0,0);
			GameSoundFX::getSingletonPtr()->setSoundFX(GameSoundFX::FX_BOLA_PALA);
		}

		// si ha colisionado con la pala derecha
		else if (!_comprobarColision[COLISION_PALA_D] || !_comprobarColision[COLISION_PALA_EXT_D]) {
			Ogre::Vector3 _direccionrefleja;
			float desvio = 0.0;
			if (_direccion.x <=0 && _direccion.y <=0) {
				_direccionrefleja = Ogre::Vector3(-_direccion.x,-_direccion.y,0.0);

			} else {
				_direccionrefleja = Ogre::Vector3(_direccion.x,-_direccion.y,0.0);
				desvio = ((rand() % 4))/100.0;
			}
			_direccion = _direccionrefleja + Ogre::Vector3(desvio,0,0);
			GameSoundFX::getSingletonPtr()->setSoundFX(GameSoundFX::FX_BOLA_PALA);
		}

		// si ha colisionado con la pala centro
		else if (!_comprobarColision[COLISION_PALA_C]) {
			Ogre::Vector3 _direccionrefleja = Ogre::Vector3(_direccion.x,-_direccion.y,0.0);
			float desvio = ((rand() % 6)+10)/100.0;

			int signo = (rand() % 2);
			if (signo == 0) {
				desvio = -desvio;
			}

			if (_direccion.x <=0 && _direccion.y <=0) {
				_direccion = _direccionrefleja - Ogre::Vector3(desvio,0,0);
			} else {
				_direccion = _direccionrefleja + Ogre::Vector3(desvio,0,0);
			}
			GameSoundFX::getSingletonPtr()->setSoundFX(GameSoundFX::FX_BOLA_PALA);
		}

		// si ha colisionado con el suelo
		else if (!_comprobarColision[COLISION_SUELO]) {
			_playState->quitarVida();
			GameSoundFX::getSingletonPtr()->setSoundFX(GameSoundFX::FX_BOLA_SUELO);
		}

		// si ha colisionado con un ladrillo
		if (nombre.substr(0,8) == "Ladrillo") {
			Brick* brick =_playState->getMuro()->getBrickByName(nombre);
			if (brick!= NULL) {
				// En funcion de la direccion actual de la bola
				// se calcula la nueva trayectoria
				if (_direccion.x <= 0 && _direccion.y >= 0){
					_direccion = Ogre::Vector3(_direccion.x,-_direccion.y,0.0);
				} else if (_direccion.x >= 0 && _direccion.y >= 0){
					_direccion = Ogre::Vector3(_direccion.x,-_direccion.y,0.0);
				} else if (_direccion.x <= 0 && _direccion.y <= 0){
					_direccion = Ogre::Vector3(_direccion.x,-_direccion.y,0.0);
				} else if (_direccion.x >= 0 && _direccion.y <= 0){
					_direccion = Ogre::Vector3(_direccion.x,-_direccion.y,0.0);
				}

				if (!brick->estaRoto()) {
					brick->romper();
					_playState->incMarcador(20);
					GameSoundFX::getSingletonPtr()->setSoundFX(GameSoundFX::FX_BOLA_LADRILLO);
				}
			}

		}

	}
	Ogre::Vector3 vx(1,1,0);
	_nodeBola->translate(vx * _direccion.normalisedCopy() * timeSinceLastFrame * _speed);

}

void Ball::setNodeParent(Ogre::SceneNode* newParent) {

	Ogre::Vector3 posicion;
	if (_nodeBola->getParentSceneNode() != NULL) {

		// Se extrae la posicion del nodo padre
		posicion = _nodeBola->getParentSceneNode()->getPosition();

		// Se elimina del nodo padre actual
		_nodeBola->getParentSceneNode()->removeChild(_nodeBola);

		_nodeBola->setPosition(posicion);
	}

	newParent->addChild(_nodeBola);

}

bool
Ball::isCollision(const Ogre::Vector3& position, const Ogre::Vector3& direction, Ogre::String& nombre)
{
    Ogre::IntersectionSceneQueryResult &result =_intersectionSceneQuery->execute();

    Ogre::SceneQueryMovableIntersectionList :: iterator iter;
    for (iter=result.movables2movables.begin(); iter!=result.movables2movables.end(); ++iter) {

    	Ogre::SceneQueryMovableObjectPair pObject = static_cast <Ogre::SceneQueryMovableObjectPair> (* iter);

    	Ogre::String strFirst = pObject.first-> getName ();
    	Ogre::String strSecond = pObject.second-> getName ();
    	if (strFirst == "Bola" || strSecond == "Bola") {

    		if (strFirst == "Bola") {
    			nombre = strSecond;
    		} else {
    			nombre = strFirst;
    		}

    		if ( (strFirst == "Pared1" || strSecond == "Pared1") && _comprobarColision[COLISION_PARED_IZQUIERDA]) {
    			_comprobarColision[COLISION_PARED_IZQUIERDA] = false;
    			_comprobarColision[COLISION_PARED_DERECHA] = true;
    			_comprobarColision[COLISION_PARED_TECHO] = true;
    			_comprobarColision[COLISION_PALA_I] = true;
    			_comprobarColision[COLISION_PALA_D] = true;
    			_comprobarColision[COLISION_PALA_C] = true;
    			_comprobarColision[COLISION_PALA_EXT_I] = true;
    			_comprobarColision[COLISION_PALA_EXT_D] = true;
    			_comprobarColision[COLISION_SUELO] = true;
    			_comprobarColision[COLISION_LADRILLO] = true;
    			//cout << "*****************Colision:  " << pObject.first-> getName () << " y " << pObject.second-> getName () << endl;
    			return true;
    		}

    		if ( (strFirst == "Pared2" || strSecond == "Pared2") && _comprobarColision[COLISION_PARED_DERECHA]) {
    			_comprobarColision[COLISION_PARED_IZQUIERDA] = true;
    			_comprobarColision[COLISION_PARED_DERECHA] = false;
    			_comprobarColision[COLISION_PARED_TECHO] = true;
    			_comprobarColision[COLISION_PALA_I] = true;
    			_comprobarColision[COLISION_PALA_D] = true;
    			_comprobarColision[COLISION_PALA_C] = true;
    			_comprobarColision[COLISION_PALA_EXT_I] = true;
    			_comprobarColision[COLISION_PALA_EXT_D] = true;
    			_comprobarColision[COLISION_SUELO] = true;
    			_comprobarColision[COLISION_LADRILLO] = true;
    			//cout << "*****************Colision:  " << pObject.first-> getName () << " y " << pObject.second-> getName () << endl;
    			return true;
    		}
    		if ( (strFirst == "Techo" || strSecond == "Techo") && _comprobarColision[COLISION_PARED_TECHO]) {
    			_comprobarColision[COLISION_PARED_IZQUIERDA] = true;
    			_comprobarColision[COLISION_PARED_DERECHA] = true;
    			_comprobarColision[COLISION_PARED_TECHO] = false;
    			_comprobarColision[COLISION_PALA_I] = true;
    			_comprobarColision[COLISION_PALA_D] = true;
    			_comprobarColision[COLISION_PALA_C] = true;
    			_comprobarColision[COLISION_PALA_EXT_I] = true;
    			_comprobarColision[COLISION_PALA_EXT_D] = true;
    			_comprobarColision[COLISION_SUELO] = true;
    			_comprobarColision[COLISION_LADRILLO] = true;
    			//cout << "*****************Colision:  " << pObject.first-> getName () << " y " << pObject.second-> getName () << endl;
    			return true;
    		}

    		if ( (strFirst == "PalaI" || strSecond == "PalaI") && _comprobarColision[COLISION_PALA_I]) {
        		_comprobarColision[COLISION_PARED_IZQUIERDA] = true;
        		_comprobarColision[COLISION_PARED_DERECHA] = true;
        		_comprobarColision[COLISION_PARED_TECHO] = true;
        		_comprobarColision[COLISION_PALA_I] = false;
        		_comprobarColision[COLISION_PALA_D] = true;
        		_comprobarColision[COLISION_PALA_C] = true;
    			_comprobarColision[COLISION_PALA_EXT_I] = true;
    			_comprobarColision[COLISION_PALA_EXT_D] = true;
        		_comprobarColision[COLISION_SUELO] = true;
        		_comprobarColision[COLISION_LADRILLO] = true;
        		//cout << "*****************Colision:  " << pObject.first-> getName () << " y " << pObject.second-> getName () << endl;
        		return true;
    		}

    		if ( (strFirst == "PalaExtI" || strSecond == "PalaExtI") && _comprobarColision[COLISION_PALA_EXT_I]) {
        		_comprobarColision[COLISION_PARED_IZQUIERDA] = true;
        		_comprobarColision[COLISION_PARED_DERECHA] = true;
        		_comprobarColision[COLISION_PARED_TECHO] = true;
        		_comprobarColision[COLISION_PALA_I] = true;
        		_comprobarColision[COLISION_PALA_D] = true;
        		_comprobarColision[COLISION_PALA_C] = true;
    			_comprobarColision[COLISION_PALA_EXT_I] = false;
    			_comprobarColision[COLISION_PALA_EXT_D] = true;
        		_comprobarColision[COLISION_SUELO] = true;
        		_comprobarColision[COLISION_LADRILLO] = true;
        		//cout << "*****************Colision:  " << pObject.first-> getName () << " y " << pObject.second-> getName () << endl;
        		return true;
    		}

    		if ( (strFirst == "PalaExtD" || strSecond == "PalaExtD") && _comprobarColision[COLISION_PALA_EXT_D]) {
        		_comprobarColision[COLISION_PARED_IZQUIERDA] = true;
        		_comprobarColision[COLISION_PARED_DERECHA] = true;
        		_comprobarColision[COLISION_PARED_TECHO] = true;
        		_comprobarColision[COLISION_PALA_I] = true;
        		_comprobarColision[COLISION_PALA_D] = true;
        		_comprobarColision[COLISION_PALA_C] = true;
    			_comprobarColision[COLISION_PALA_EXT_I] = true;
    			_comprobarColision[COLISION_PALA_EXT_D] = false;
        		_comprobarColision[COLISION_SUELO] = true;
        		_comprobarColision[COLISION_LADRILLO] = true;
        		//cout << "*****************Colision:  " << pObject.first-> getName () << " y " << pObject.second-> getName () << endl;
        		return true;
    		}

    		if ( (strFirst == "PalaD" || strSecond == "PalaD") && _comprobarColision[COLISION_PALA_D]) {
        		_comprobarColision[COLISION_PARED_IZQUIERDA] = true;
        		_comprobarColision[COLISION_PARED_DERECHA] = true;
        		_comprobarColision[COLISION_PARED_TECHO] = true;
        		_comprobarColision[COLISION_PALA_I] = true;
        		_comprobarColision[COLISION_PALA_D] = false;
        		_comprobarColision[COLISION_PALA_C] = true;
    			_comprobarColision[COLISION_PALA_EXT_I] = true;
    			_comprobarColision[COLISION_PALA_EXT_D] = true;
        		_comprobarColision[COLISION_SUELO] = true;
        		_comprobarColision[COLISION_LADRILLO] = true;
        		//cout << "*****************Colision:  " << pObject.first-> getName () << " y " << pObject.second-> getName () << endl;
        		return true;
    		}

    		if ( (strFirst == "PalaC" || strSecond == "PalaC") && _comprobarColision[COLISION_PALA_C]) {
        		_comprobarColision[COLISION_PARED_IZQUIERDA] = true;
        		_comprobarColision[COLISION_PARED_DERECHA] = true;
        		_comprobarColision[COLISION_PARED_TECHO] = true;
        		_comprobarColision[COLISION_PALA_I] = true;
        		_comprobarColision[COLISION_PALA_D] = true;
        		_comprobarColision[COLISION_PALA_C] = false;
    			_comprobarColision[COLISION_PALA_EXT_I] = true;
    			_comprobarColision[COLISION_PALA_EXT_D] = true;
        		_comprobarColision[COLISION_SUELO] = true;
        		_comprobarColision[COLISION_LADRILLO] = true;
        		//cout << "*****************Colision:  " << pObject.first-> getName () << " y " << pObject.second-> getName () << endl;
        		return true;
    		}

    		if ( (strFirst == "Suelo" || strSecond == "Suelo") && _comprobarColision[COLISION_SUELO]) {
        		_comprobarColision[COLISION_PARED_IZQUIERDA] = true;
        		_comprobarColision[COLISION_PARED_DERECHA] = true;
        		_comprobarColision[COLISION_PARED_TECHO] = true;
        		_comprobarColision[COLISION_PALA_I] = true;
        		_comprobarColision[COLISION_PALA_D] = true;
        		_comprobarColision[COLISION_PALA_C] = true;
    			_comprobarColision[COLISION_PALA_EXT_I] = true;
    			_comprobarColision[COLISION_PALA_EXT_D] = true;
        		_comprobarColision[COLISION_SUELO] = false;
        		_comprobarColision[COLISION_LADRILLO] = true;
        		//cout << "*****************Colision:  " << pObject.first-> getName () << " y " << pObject.second-> getName () << endl;
        		return true;
    		}

        	if ( (strFirst.substr(0,8) == "Ladrillo" || strSecond.substr(0,8) == "Ladrillo" ) && _comprobarColision[COLISION_LADRILLO]){
        		_comprobarColision[COLISION_PARED_IZQUIERDA] = true;
        		_comprobarColision[COLISION_PARED_DERECHA] = true;
        		_comprobarColision[COLISION_PARED_TECHO] = true;
        		_comprobarColision[COLISION_PALA_I] = true;
        		_comprobarColision[COLISION_PALA_D] = true;
        		_comprobarColision[COLISION_PALA_C] = true;
    			_comprobarColision[COLISION_PALA_EXT_I] = true;
    			_comprobarColision[COLISION_PALA_EXT_D] = true;
        		_comprobarColision[COLISION_SUELO] = true;
        		_comprobarColision[COLISION_LADRILLO] = false;
        		//cout << "*****************Colision:  " << pObject.first-> getName () << " y " << pObject.second-> getName () << endl;
        		return true;
        	}


    	}
    }

	return false;


}

void
Ball::hide(){
	_nodeBola->setVisible(false);
}

void
Ball::incSpeedBall(){
	_speed = SPEED_BALL_NORMAL;
}

void
Ball::decSpeedBall(){
	_speed = SPEED_BALL_SLOW;
}

bool
Ball::slowBallTimer(int segundos, Ogre::String itemName){

	if (_ItemBlock == "" || _ItemBlock == itemName) {// Si no esta bloqueado o lo est'apor el llamante
		_ItemBlock = itemName;
		if (_timerAvtive == false ) {

			// Se activa el timer
			_timerAvtive = true;

			// Se ralentiza la bola
			decSpeedBall();

		} else {
			if (_timer.getSegundosTranscurridos() > segundos) {

				// Se acobo el tiempo
				// Se contrae la pala
				incSpeedBall();
				_timerAvtive = false;
				_timer.reset();
				_ItemBlock = "";
				return true;
			}
		}
	}
	return false;
}

void
Ball::reset() {
	incSpeedBall();
	_timerAvtive = false;
	_timer.reset();
	_ItemBlock = "";
}

bool
Ball::isBusy(Ogre::String itemName){
	if (_ItemBlock == "" || _ItemBlock == itemName)
		return false;
	else
		return true;
}



