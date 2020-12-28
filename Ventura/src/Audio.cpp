#include "Audio.h"

Audio::Audio() {
	//holder
}

Audio::~Audio() {
	irrklang::ISoundSource * sounbd = m_SoundEngine->addSoundSourceFromFile("dsa");

	m_SoundEngine->drop();
}

void Audio::AddSound(const std::string& referenceName, const std::string& soundFileName, bool loop, float volumePercentage) {
	Sound sound;
	sound.m_Sound = m_SoundEngine->addSoundSourceFromFile(soundFileName.c_str());
	sound.m_Loop = loop;
	sound.m_Volume = (volumePercentage > 1.0f || volumePercentage < 0.0f) ? 1.0f : volumePercentage;
	sound.m_IrrklangName = sound.m_Sound->getName();

	m_Sounds[referenceName] = sound;
}

void Audio::PlaySound(const std::string& soundName) {
	if (m_Sounds.find(soundName) != m_Sounds.end()) {
		if (isPlaying(soundName)) {
			m_SoundEngine->removeSoundSource(m_Sounds[soundName].m_Sound);
		}

		Sound& currentSound = m_Sounds[soundName];
		currentSound.m_Sound->setDefaultVolume(currentSound.m_Volume);
		m_SoundEngine->play2D(currentSound.m_Sound, currentSound.m_Loop, false, true);
	}
	else {
		std::cout << "Error: Sound " << soundName << " was not found" << std::endl;
	}
}

void Audio::Mute(const std::string& soundName) {
	if (soundName == "") {
		m_SoundEngine->stopAllSounds();
	}
	else {
		if (m_Sounds.find(soundName) != m_Sounds.end()) {
			m_SoundEngine->stopAllSoundsOfSoundSource(m_Sounds[soundName].m_Sound);
		}
		else {
			std::cout << "Error: Sound " << soundName << " was not found" << std::endl;
		}
	}
}

Sound& Audio::getSound(const std::string& soundName) {
	if (m_Sounds.find(soundName) != m_Sounds.end()) {
		return m_Sounds[soundName];
	}
	else {
		std::cout << "Error: Sound " << soundName << " was not found, returning an empty sound" << std::endl;
		Sound nullSound;
		return nullSound;
	}
}

bool Audio::isPlaying(const std::string& soundName) {
	if (m_Sounds.find(soundName) != m_Sounds.end()) {
		return m_SoundEngine->isCurrentlyPlaying(m_Sounds[soundName].m_Sound);
	}
	else {
		std::cout << "Error: Sound " << soundName << " was not found" << std::endl;
		return false;
	}
}