#include "Properties.h"

Properties::Properties() {

}
Properties::~Properties() {

}

/**
 * Returna true en caso de exito
 * y false si no encuentra el fichero
 */
bool
Properties::load(string file) {

	// Se limpia el hasmap
	_hashMap.clear();

	string line;
	ifstream myfile (file);

	if (myfile.is_open()){
		while ( getline (myfile,line) ){
			// Se mira si no es un comentario
			if (line.substr(0,2) != "//" && line.substr(0,1) != "#") {

				// Se busca el igual
				std::size_t found = line.find("=");
				if (found!=std::string::npos){
					// Se ha encontrado el =
					string clave = line.substr(0,found);
					string valor = line.substr(found+1);
					_hashMap[clave] = valor;
				}

			}

		}
		myfile.close();
	} else {
		return false;
	}

	return true;

}

vector<string>
Properties::getKeys(){
	vector<string> keys;
	keys.reserve(_hashMap.size());

	for (map<string, string>::iterator iter = _hashMap.begin();
	                                        iter != _hashMap.end();
	                                        ++iter)
	{
	    keys.push_back(iter->first);
	}
	return keys;
}
