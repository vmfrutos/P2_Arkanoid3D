#ifndef RECORD_MANAGER_H
#define RECORD_MANAGER_H

#include <Ogre.h>
#include <vector>
#include <string>
#include <algorithm>
#include <functional>
#include <fstream>
#include "Record.h"
#include "PlayState.h"
#include <string>
#include <iostream>
#include "RecordsManager.h"

using namespace std;

class RecordsManager : public Ogre::Singleton<RecordsManager>
{
public:
	RecordsManager();
	~RecordsManager();
	static RecordsManager& getSingleton ();
	static RecordsManager* getSingletonPtr();

	void insertarRecord(Record& record);
	vector<Record> getRecords() const;

private:

	vector<Record> _records;
	string _recordsFile;


	void loadFromFile();
	void saveToFile();



};
#endif
