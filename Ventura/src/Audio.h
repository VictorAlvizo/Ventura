#pragma once
#include <iostream>
#include <irrKlang.h>
#include <unordered_map>

struct Sound {
	irrklang::ISoundSource * m_Sound;
	float m_Volume;
	bool m_Loop;
	std::string m_IrrklangName; //Name given to the sound source by Irrklang
};

class Audio {
public:
	Audio();
	~Audio();

	void AddSound(const std::string& referenceName, const std::string& soundFileName, bool loop = false, float volumePercentage = 1.0f);
	void PlaySound(const std::string& soundName);
	//If soundName is left empty, all audio from this class will be stopped
	void Mute(const std::string& soundName = "");

	Sound& getSound(const std::string& soundName);

	bool isPlaying(const std::string& soundName);

private:
	irrklang::ISoundEngine * m_SoundEngine = irrklang::createIrrKlangDevice();

	std::unordered_map<std::string, Sound> m_Sounds;
};

