#include "Record.h"


Record::Record(){
	_nombre = "";
	_segundosTranscurridos = 0.0;
	_nivel = 0;
}

Record::Record(const Record& r){
	_nombre = r.getNombre();
	_segundosTranscurridos = r.getSegundosTranscurridos();
	_nivel = r.getNivel();

}

Record::~Record(){

}

void Record::setNombre(string nombre){
	_nombre = nombre;
}

void Record::setSegundosTranscurridos(Ogre::Real segundosTranscurridos){
	_segundosTranscurridos = segundosTranscurridos;
}

void Record::setNivel(int nivel){
	_nivel = nivel;
}


string Record::getNombre() const{
	return _nombre;
}

Ogre::Real Record::getSegundosTranscurridos() const{
	return _segundosTranscurridos;
}

string Record::getTiempoTranscurridoStr()  const {
	Contador cont;
	cont.incrementar(getSegundosTranscurridos());
	return cont.getContadorStr();
}

int Record::getNivel() const{
	return _nivel;
}

string Record::getNivelStr() const {
	std::ostringstream nivelStr;
	nivelStr << getNivel();
	return nivelStr.str();
}

bool Record::operator<(const Record& r) const{
	//cout << "Se va a ordenar ";
	if (this->getNivel() == r.getNivel()) {
		//cout << "nivel igual " << endl;
		return this->getSegundosTranscurridos() < r.getSegundosTranscurridos();
	} else {
		//cout << "nivel igual " << endl;
		return this->getNivel() >  r.getNivel();
	}

}

Record &Record::operator = (const Record &r){
	_nivel = r.getNivel();
	_nombre = r.getNombre();
	_segundosTranscurridos = r.getSegundosTranscurridos();
	return(*this);
}





