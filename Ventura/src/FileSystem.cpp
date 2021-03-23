#include "FileSystem.h"

bool FileSystem::FileExist(const std::string& filePath) {
	std::fstream fileCheck;
	fileCheck.open(filePath, std::ios::out | std::ios::in);

	if (fileCheck.is_open()) {
		fileCheck.close();
		return true;
	}
	else {
		fileCheck.close();
		return false;
	}
}

bool FileSystem::DataExist(const std::string& filePath, const std::string& dataName) {
	if (!FileExist(filePath)) {
		std::cout << "Error: File " << filePath << " does not exist" << std::endl;
		return false;
	}

	std::ifstream file(filePath);

	std::string line, currentDataName;
	while (std::getline(file, line)) {
		std::stringstream ss(line);
		ss >> currentDataName;
		std::cout << "Current Data Name: " << currentDataName << std::endl;

		if (currentDataName == dataName) {
			file.close();
			return true;
		}
	}

	file.close();
	return false;
}

bool FileSystem::RemoveFile(const std::string& filePath) {
	if (!FileExist(filePath)) {
		std::cout << "Error: File " << filePath << " does not exist" << std::endl;
		return false;
	}

	if(std::remove(filePath.c_str()) == 0) {
		return true;
	}
	else {
		std::cout << "Error: " << " An error has occured while trying to remove the file " << filePath << std::endl;
		return false;
	}
}

bool FileSystem::RenameFile(const std::string& filePath, const std::string& newPath) {
	if (std::rename(filePath.c_str(), newPath.c_str()) == 0) {
		return true;
	}
	else {
		std::cout << "Error: Error while renaming file, permission denied" << std::endl;
		return false;
	}
}