#include "Wall.h"

Wall::Wall(Ogre::SceneManager* sceneMgr){
	_sceneMgr = sceneMgr;
	_currentLevel = 1;

	for (unsigned int i=0; i<NUM_FILAS;i++){
		for (unsigned int j=0; j<NUM_COLUMNAS;j++){
			_bricks[i][j] = NULL;
		}
	}

}

Wall::~Wall(){

}

bool
Wall::loadLevel(int level){
	cout << "************************** Cargando NIVEL: " << level << endl;
	cleanCurrentLevel();
	_currentLevel = level;

	std::stringstream file;
	file << FILE_NAME << _currentLevel << ".txt";
	cout <<"************* Buscando fiechero  " << file.str() << endl;
	Properties props;
	// Si no se encuentra el fichero es porque ya no existe el nivel (fin de juego)
	if (!props.load(file.str())) {
		cout <<"************* No se ha encontrado fichero  " << file.str() << endl;
		return false;
	} else {
		cout <<"************* Se ha encontrado fichero  " << file.str() << endl;
		for (unsigned int i=0; i<NUM_FILAS;i++){

			std::stringstream propiedad;
			propiedad << "color_" << i;

			string lineaColor = props.getProperty(propiedad.str());

			propiedad.str("");
			propiedad.clear();
			propiedad << "resis_" << i;

			string lineaResistencia = props.getProperty(propiedad.str());

			propiedad.str("");
			propiedad.clear();
			propiedad << "item_" << i;

			string lineaItem = props.getProperty(propiedad.str());


			for (unsigned int j=0; j<NUM_COLUMNAS;j++){
				string caracterColor = lineaColor.substr(j,1);
				string caracterResis = lineaResistencia.substr(j,1);
				string caracterItem = lineaItem.substr(j,1);
				_bricks[i][j] = new Brick(_sceneMgr,atoi(caracterColor.c_str()),i,j);
				_bricks[i][j]->initSetUp();
				_bricks[i][j]->setPosition(Ogre::Vector3(9.3-(j*1.1),14-(i*0.6),0));
				_bricks[i][j]->setResistencia(atoi(caracterResis.c_str()));
				_bricks[i][j]->setItem(atoi(caracterItem.c_str()));

			}
		}
		return true;
	}


}

void
Wall::cleanCurrentLevel(){
	// Se elimina los ladrillos que pudiesen estar cargados
	for (unsigned int i=0; i<NUM_FILAS;i++){
		for (unsigned int j=0; j<NUM_COLUMNAS;j++){
			if (_bricks[i][j] != NULL){
				delete _bricks[i][j];
				_bricks[i][j] = NULL;
			}
		}
	}
}

Brick*
Wall::getBrickByName(const Ogre::String& name){

	Ogre::String filaColumna = name.substr(8);

	std::size_t found = filaColumna.find("_");
	if (found!=std::string::npos){
		// Se ha encontrado el _
		Ogre::String fila = filaColumna.substr(0,found);
		Ogre::String columna = filaColumna.substr(found+1);
		return _bricks[atoi(fila.c_str())][atoi(columna.c_str())];

	}
	return NULL;
}

bool
Wall::levelCompleted(){

	for (unsigned int i=0; i<NUM_FILAS;i++) {
		for (unsigned int j=0; j<NUM_COLUMNAS;j++) {
			Brick* brick = _bricks[i][j];
			if (brick->isVisible())
				return false;

		}
	}

	return true;
}

void
Wall::refresh(float timeSinceLastFrame) {
	// Se refrescan todos los ladrillos
	for (unsigned int i=0; i<NUM_FILAS;i++) {
		for (unsigned int j=0; j<NUM_COLUMNAS;j++) {
			Brick* brick = _bricks[i][j];
			brick->refresh(timeSinceLastFrame);
		}
	}

}

void
Wall::resetItems(){
	// Se recorren todos los ladrillos
	for (unsigned int i=0; i<NUM_FILAS;i++) {
		for (unsigned int j=0; j<NUM_COLUMNAS;j++) {
			Brick* brick = _bricks[i][j];
			Item* item = brick->getItem();
			if (item != NULL) {
				item->reset();
			}
		}
	}
}
