#include "RecordsManager.h"

template<> RecordsManager* Ogre::Singleton<RecordsManager>::msSingleton = 0;

RecordsManager::RecordsManager() {
	_recordsFile = "records.txt";
	loadFromFile();
}

RecordsManager::~RecordsManager() {
	saveToFile();
	cout << "~RecordsManager" << endl;
}

RecordsManager* RecordsManager::getSingletonPtr (){
	return msSingleton;
}

RecordsManager& RecordsManager::getSingleton (){
  assert(msSingleton);
  return *msSingleton;
}

void RecordsManager::insertarRecord(Record& record){

	_records.push_back(record);
	std::sort(_records.begin(),_records.end());
}

vector<Record> RecordsManager::getRecords() const{
	return _records;
}

void RecordsManager::loadFromFile(){
	string line;
	ifstream myfile (_recordsFile);

	if (myfile.is_open()){
		while ( getline (myfile,line) ){
			string nombre;
			string nivel;
			string tiempo;
			size_t pos = line.find(";",0);
			if (pos == string::npos)
				continue;
			nombre = line.substr(0,pos);

			size_t pos2 = line.find(";",pos+1);
			if (pos2 == string::npos)
				continue;
			nivel = line.substr(pos+1,pos2-pos-1);

			tiempo = line.substr(pos2+1);

			Record record;
			record.setNombre(nombre);
			record.setNivel(std::atoi(nivel.c_str()));
			record.setSegundosTranscurridos(Ogre::StringConverter::parseReal(tiempo));
			insertarRecord(record);
		}
		myfile.close();
	}
}

void RecordsManager::saveToFile(){
	ofstream myfile (_recordsFile,std::ofstream::trunc);

	if (myfile.is_open()){

		for (unsigned int i=0;i<_records.size();i++){
			myfile  << _records[i].getNombre() << ";" << _records[i].getNivel() << ";" << _records[i].getSegundosTranscurridos() << endl;
		}
		myfile.close();
	}

}

