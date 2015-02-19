#ifndef Properties_H
#define Properties_H

#include <iostream>
#include <string>
#include <map>
#include <fstream>
#include <vector>

using namespace std;

class Properties {

public:
	Properties();
	~Properties();

	bool load(string file);
	string getProperty(string property) { return _hashMap[property];}
	vector<string> getKeys();

private:

	map<string, string> _hashMap;
};

#endif
