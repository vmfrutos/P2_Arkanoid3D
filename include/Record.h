#ifndef _RECORD_H
#define _RECORD_H

#include <string>
#include "Ogre.h"
#include "Contador.h"
using namespace std;


class Record {


public:

	Record();
	Record(const Record& r);
	~Record();

	void setNombre(string nombre);
	void setSegundosTranscurridos(Ogre::Real  segundosTranscurridos);
	void setNivel(int nivel);

	string getNombre()  const;
	Ogre::Real  getSegundosTranscurridos()  const;
	string  getTiempoTranscurridoStr()  const;
	int getNivel() const;
	string getNivelStr() const;

	bool operator<(const Record& r) const;
	Record& operator=(const Record &r);

private:
	string _nombre;
	int _nivel;
	Ogre::Real _segundosTranscurridos;
};
#endif
