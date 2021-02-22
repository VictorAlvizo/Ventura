#include "Audio.h"

Audio::Audio(bool deathAllowed) 
	:m_DeathEnabled(deathAllowed), m_MuteActive(false)
{
	m_CurrentSound = nullptr;
}

Audio::~Audio() {
	/* When adding a local audio varible as a component, the deconstructer would be called 
	as it would go out of scope right after, causing it to drop the sound engine. So All
	access to audio is lost. Had to make this bool to for a hacked solution, so the drop
	part won't be called in those situations and only until the component sets it to true
	then it may call the deconstructer */
	if (m_DeathEnabled) {
		if (m_CurrentSound) {
			m_CurrentSound->drop();
		}

		m_SoundEngine->drop();
	}
}

void Audio::AddSound(const std::string& referenceName, const std::string& soundFileName, bool loop, float volumePercentage) {
	Sound sound;
	sound.m_Sound = m_SoundEngine->addSoundSourceFromFile(soundFileName.c_str());
	sound.m_Loop = loop;
	sound.m_Volume = (volumePercentage > 1.0f || volumePercentage < 0.0f) ? 1.0f : volumePercentage;
	sound.m_IrrklangName = sound.m_Sound->getName();
	sound.m_AudioLength = sound.m_Sound->getPlayLength();

	m_Sounds[referenceName] = sound;
}

void Audio::PlaySound(const std::string& soundName) {
	if (!m_MuteActive) {
		if (m_Sounds.find(soundName) != m_Sounds.end()) {
			if (isPlaying(soundName)) {
				m_SoundEngine->removeSoundSource(m_Sounds[soundName].m_Sound);
			}

			Sound& currentSound = m_Sounds[soundName];
			currentSound.m_Sound->setDefaultVolume(currentSound.m_Volume);
			m_CurrentSound = m_SoundEngine->play2D(currentSound.m_Sound, currentSound.m_Loop, false, true);
		}
		else {
			std::cout << "Error: Sound " << soundName << " was not found" << std::endl;
		}
	}
}

void Audio::Mute(const std::string& soundName) {
	if (soundName == "") {
		m_SoundEngine->stopAllSounds();
		m_MuteActive = true;
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

void Audio::Unmute() {
	m_MuteActive = false;
}

void Audio::PauseSound(bool pause) {
	if (m_CurrentSound) {
		m_CurrentSound->setIsPaused(pause);
	}
}

void Audio::Volume(const std::string& soundName, float volume) {
	if (m_Sounds.find(soundName) != m_Sounds.end()) {
		if (!(volume < 0.0f || volume > 1.0f)) {
			if (isPlaying(soundName)) {
				m_Sounds[soundName].m_Volume = volume;
				m_CurrentSound->setVolume(volume);
			}
			else {
				m_Sounds[soundName].m_Volume = volume;
			}
		}
		else {
			std::cout << "Error: volume must be within the range of 0.0 and 1.0" << std::endl;
		}
	}
	else {
		std::cout << "Error: Sound " << soundName << " was not found" << std::endl;
	}
}

void Audio::SetSoundPos(unsigned int newPos) {
	if (m_CurrentSound && !isFinished()) {
		if (newPos < getAudioLength(currentSoundPlaying())) {
			m_CurrentSound->setPlayPosition(newPos);
		}
		else {
			std::cout << "Error: position " << newPos << " exceeds the audio file length of " << getAudioLength(currentSoundPlaying()) << std::endl;
		}
	}
	else {
		std::cout << "Error: No audio is currently playing" << std::endl;
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

bool Audio::isFinished() {
	if (m_CurrentSound) {
		return m_CurrentSound->isFinished();
	}

	return false;
}

bool Audio::isPaused() {
	if (m_CurrentSound) {
		m_CurrentSound->getIsPaused();
	}

	return false;
}

unsigned int Audio::getPlayPos() {
	if (m_CurrentSound && !isFinished()) {
		return m_CurrentSound->getPlayPosition();
	}

	return 0;
}

unsigned int Audio::getAudioLength(const std::string& soundName) {
	if (m_Sounds.find(soundName) != m_Sounds.end()) {
		return m_Sounds[soundName].m_AudioLength;
	}

	std::cout << "Error: Sound " << soundName << " was not found, returning 0" << std::endl;
	return 0;
}

float Audio::getVolume(const std::string& soundName) {
	if (m_Sounds.find(soundName) != m_Sounds.end()) {
		return m_Sounds[soundName].m_Volume;
	}
	else {
		std::cout << "Error: Sound " << soundName << " was not found, returning 0" << std::endl;
	}
}

std::string Audio::currentSoundPlaying() {
	//If this is null that means that no sounds have been played yet
	if (!m_CurrentSound || m_CurrentSound->isFinished()) {
		return "";
	}

	irrklang::ISoundSource * currentSource = m_CurrentSound->getSoundSource();

	for (auto sound : m_Sounds) {
		if (currentSource == sound.second.m_Sound) {
			return sound.first;
		}
	}

	return "";
}