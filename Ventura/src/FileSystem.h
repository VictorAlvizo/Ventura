#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>
#include "Vendor/glm/glm.hpp"

//TODO: May update to a JSON file instead of a txt file later on
class FileSystem {
public:
	//WARNING: They look very alike but retriving data from the wrong type is most likely bad news!
	//Also, don't try storing objects unless they have an operator overloader for << and >>
	template<typename T>
	static void Store(std::string filepath, std::string dataName, T data, bool overwrite) {
		std::replace(dataName.begin(), dataName.end(), ' ', '_'); //Replace all spaces with _ for easier reading
		std::ofstream file;

		if (overwrite) {
			file.open(filepath);
		}
		else {
			file.open(filepath, std::ios::app);
		}

		file << dataName << " " << data << "\n";
		file.close();
	}

	template<>
	static void Store<std::string>(std::string filepath, std::string dataName, std::string data, bool overwrite) {
		std::replace(dataName.begin(), dataName.end(), ' ', '_');
		std::replace(data.begin(), data.end(), ' ', '_');

		std::ofstream file;

		if (overwrite) {
			file.open(filepath);
		}
		else {
			file.open(filepath, std::ios::app);
		}

		file << dataName << " " << data << "\n";
		file.close();
	}

	template<>
	static void Store<glm::vec2>(std::string filepath, std::string dataName, glm::vec2 data, bool overwrite) {
		std::replace(dataName.begin(), dataName.end(), ' ', '_'); //Replace all spaces with _ for easier reading
		std::ofstream file;

		if (overwrite) {
			file.open(filepath);
		}
		else {
			file.open(filepath, std::ios::app);
		}

		file << dataName << " " << data.x << " " << data.y << "\n";
		file.close();
	}

	template<>
	static void Store<glm::vec3>(std::string filepath, std::string dataName, glm::vec3 data, bool overwrite) {
		std::replace(dataName.begin(), dataName.end(), ' ', '_'); //Replace all spaces with _ for easier reading
		std::ofstream file;

		if (overwrite) {
			file.open(filepath);
		}
		else {
			file.open(filepath, std::ios::app);
		}

		file << dataName << " " << data.x << " " << data.y  << " " << data.z << "\n";
		file.close();
	}

	template<>
	static void Store<glm::vec4>(std::string filepath, std::string dataName, glm::vec4 data, bool overwrite) {
		std::replace(dataName.begin(), dataName.end(), ' ', '_'); //Replace all spaces with _ for easier reading
		std::ofstream file;

		if (overwrite) {
			file.open(filepath);
		}
		else {
			file.open(filepath, std::ios::app);
		}

		file << dataName << " " << data.x << " " << data.y << " " << data.z << " " << data.w << "\n";
		file.close();
	}

	template<typename T>
	static T Retrive(std::string filePath, std::string dataName) {
		if (!FileExist(filePath)) {
			std::cout << "Error: File " << filePath << " does not exist" << std::endl;
			return -1;
		}

		std::ifstream file(filePath);
		std::string line, currentDataName;
		T returnType;

		while (std::getline(file, line)) {
			std::stringstream ss(line);
			ss >> currentDataName;

			if (currentDataName == dataName) {
				ss >> returnType;
				file.close();
				return returnType;
			}

		}

		return -1;
		file.close();
	}

	template<>
	static std::string Retrive<std::string>(std::string filePath, std::string dataName) {
		if (!FileExist(filePath)) {
			std::cout << "Error: File " << filePath << " does not exist" << std::endl;
			return "";
		}

		std::ifstream file(filePath);
		std::string line, currentDataName;
		std::string returnType;

		while (std::getline(file, line)) {
			std::stringstream ss(line);
			ss >> currentDataName;

			if (currentDataName == dataName) {
				ss >> returnType;
				file.close();
				return returnType;
			}

		}

		return "";
		file.close();
	}

	template<>
	static char Retrive<char>(std::string filePath, std::string dataName) {
		if (!FileExist(filePath)) {
			std::cout << "Error: File " << filePath << " does not exist" << std::endl;
			return '\0';
		}

		std::ifstream file(filePath);
		std::string line, currentDataName;
		char returnType;

		while (std::getline(file, line)) {
			std::stringstream ss(line);
			ss >> currentDataName;

			if (currentDataName == dataName) {
				ss >> returnType;
				file.close();
				return returnType;
			}

		}

		return '\0';
		file.close();
	}

	template<>
	static glm::vec2 Retrive<glm::vec2>(std::string filePath, std::string dataName) {
		if (!FileExist(filePath)) {
			std::cout << "Error: File " << filePath << " does not exist" << std::endl;
			return glm::vec2(-1.0f);
		}

		std::ifstream file(filePath);
		std::string line, currentDataName;
		glm::vec2 returnType;

		while (std::getline(file, line)) {
			std::stringstream ss(line);
			ss >> currentDataName;

			if (currentDataName == dataName) {
				ss >> returnType.x;
				ss >> returnType.y;

				file.close();
				return returnType;
			}

		}

		return glm::vec2(-1.0f);
		file.close();
	}

	template<>
	static glm::vec3 Retrive<glm::vec3>(std::string filePath, std::string dataName) {
		if (!FileExist(filePath)) {
			std::cout << "Error: File " << filePath << " does not exist" << std::endl;
			return glm::vec3(-1.0f);
		}

		std::ifstream file(filePath);
		std::string line, currentDataName;
		glm::vec3 returnType;

		while (std::getline(file, line)) {
			std::stringstream ss(line);
			ss >> currentDataName;

			if (currentDataName == dataName) {
				ss >> returnType.x;
				ss >> returnType.y;
				ss >> returnType.z;

				file.close();
				return returnType;
			}

		}

		return glm::vec3(-1.0f);
		file.close();
	}

	template<>
	static glm::vec4 Retrive<glm::vec4>(std::string filePath, std::string dataName) {
		if (!FileExist(filePath)) {
			std::cout << "Error: File " << filePath << " does not exist" << std::endl;
			return glm::vec4(-1.0f);
		}

		std::ifstream file(filePath);
		std::string line, currentDataName;
		glm::vec4 returnType;

		while (std::getline(file, line)) {
			std::stringstream ss(line);
			ss >> currentDataName;

			if (currentDataName == dataName) {
				ss >> returnType.x;
				ss >> returnType.y;
				ss >> returnType.z;
				ss >> returnType.w;

				file.close();
				return returnType;
			}

		}

		return glm::vec4(-1.0f);
		file.close();
	}

	static bool FileExist(const std::string& filePath);
	static bool RemoveFile(const std::string& filePath);
	static bool RenameFile(const std::string& filePath, const std::string& newPath);

private:
	FileSystem();
};