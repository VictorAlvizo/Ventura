#pragma once
#include <iostream>
#include <irrKlang.h>
#include <unordered_map>

struct Sound {
	irrklang::ISoundSource * m_Sound;
	float m_Volume;
	bool m_Loop;
	unsigned int m_AudioLength;
	std::string m_IrrklangName; //Name given to the sound source by Irrklang
};

class Audio {
public:
	//Leave as is normally, but if you're going to add this as a component, set to false
	Audio(bool deathAllowed = true);
	~Audio();

	//Add a sound to the audio object
	void AddSound(const std::string& referenceName, const std::string& soundFileName, bool loop = false, float volumePercentage = 1.0f);
	//Play the sound by name
	void PlaySound(const std::string& soundName);
	//If soundName is left empty, all audio from this class will be stopped
	void Mute(const std::string& soundName = "");
	//Unmute any muted sounds 
	void Unmute();
	//Pause or unpause the current sound
	void PauseSound(bool pause);

	//Adjust the volume of the wanted sound
	void Volume(const std::string& soundName, float volume);
	//Position in miliseconds (1000 miliseconds = 1 second), set the position of the current playing sound
	void SetSoundPos(unsigned int newPos);

	//Get the sound object of the wanted sound
	Sound& getSound(const std::string& soundName);

	//Checks if the sound is playing
	bool isPlaying(const std::string& soundName);
	//Returns whether the current audio is playing. If there has yet to be any audio played it will return false
	bool isFinished();
	//Checks if the current sound is paused
	bool isPaused();
	//Returns the current play position in miliseconds (1000 miliseconds = 1 second). If there is no audio playing returns 0
	unsigned int getPlayPos();
	//Returns the audio length in miliseconds, 0 if there were any errors getting the data. (1000 miliseconds = 1 second)
	unsigned int getAudioLength(const std::string& soundName);

	//Returns the volume (range of 0.0 - 1.0f) of the specified sound
	float getVolume(const std::string& soundName);

	//Get the sound name of the current playing sound
	std::string currentSoundPlaying();
	//Check if the sounds are muted
	inline bool soundsMuted() const { return m_MuteActive; }

private:
	bool m_DeathEnabled; //Usage explained inside the deconstructer
	bool m_MuteActive;

	irrklang::ISoundEngine * m_SoundEngine = irrklang::createIrrKlangDevice();
	irrklang::ISound * m_CurrentSound;

	std::unordered_map<std::string, Sound> m_Sounds;

	friend class Component;
};