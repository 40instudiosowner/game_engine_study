#include "ConfigReader.h"
#include <iostream>

ConfigReader::ConfigReader(std::string filepath) 
{
	std::ifstream configFile(filepath);
	if (!configFile.is_open()) {
		std::cerr << "Could not open config file: " << filepath << std::endl;
		exit(-1);
	}
	try {
		configFile >> _config;
	} catch (const json::parse_error& e) {
		std::cerr << "Error parsing config file: " << e.what() << std::endl;
		exit(-1);
	}
}