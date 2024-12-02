
#include "sound_player.hpp"
#include "globals.h"

void SoundPlayer::update(float dt) {
    if (playTill > 0) {
        playTill -= dt;
    }
    if (playTill > 0) {
        std::shared_ptr<AudioManager> am = getAudioManager();
        if (am->isSoundPlaying(sound)) {
            return;
        }
        am->play(sound);
    }
}

void SoundPlayer::play() {
    if (playTill < soundDuration * .99) {
        playTill = soundDuration * .99;
    }
}

void SoundPlayer::stop() {
    playTill = 0;
}

SoundPlayer::SoundPlayer(std::string sound) : sound(sound) {
    soundDuration = getAudioManager()->getSoundDuration(sound);
}
