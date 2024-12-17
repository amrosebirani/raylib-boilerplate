#include "audio_manager.hpp"
#include "game.h"
#include "raylib.h"
#include "utils.h"

AudioManager::AudioManager() {
    init();
}

void AudioManager::init() {
    sounds_to_load.emplace_back("combat1");
    sounds_to_load.emplace_back("combat2");
    sounds_to_load.emplace_back("combat3");
    sounds_to_load.emplace_back("combat4");
    sounds_to_load.emplace_back("combat5");
    sounds_to_load.emplace_back("troop_walking");
    sounds_to_load.emplace_back("building_destruction");
    sounds_to_load.emplace_back("coin_dropped");
    sounds_to_load.emplace_back("coin_collected");
    sounds_to_load.emplace_back("gem_collected");
    sounds_to_load.emplace_back("building_created");
    sounds_to_load.emplace_back("building_upgraded");
    sounds_to_load.emplace_back("summon_infantry");
    sounds_to_load.emplace_back("death_unit");
    sounds_to_load.emplace_back("building_hit1");
    sounds_to_load.emplace_back("building_hit2");
    sounds_to_load.emplace_back("archer_swish1");
    sounds_to_load.emplace_back("archer_swish2");
    sounds_to_load.emplace_back("archer_swish3");
    sounds_to_load.emplace_back("thunder");

    songs = {
        "normal", "under_attack_in_control", "under_attack_fucked", "victory",
        "sad",
    };

    initAudio();
}

void AudioManager::initAudio() {
    InitAudioDevice();
    SetMasterVolume(0.55f);
}

bool AudioManager::isSoundPlaying(std::string sound) {
    return IsSoundPlaying(sounds[sound]);
}

void AudioManager::playSound(std::string sound) {
    Sound ss = sounds[sound];
    players[sound]->play();
}

void AudioManager::play(std::string sound) {
    Sound ss = sounds[sound];
    PlaySound(ss);
}

void AudioManager::stopSound(std::string sound) {
    StopSound(sounds[sound]);
    players[sound]->stop();
}

float AudioManager::getSoundDuration(std::string sound) {
    Sound ss = sounds[sound];
    return ss.frameCount * 1.0f / ss.stream.sampleRate;
}

void AudioManager::update(float dt) {
    if (sounds_to_load.empty()) {
        soundsLoaded = true;
    } else {
        auto sound = sounds_to_load.back();
        sounds_to_load.pop_back();
        sounds[sound] =
            LoadSound(getAssetPath(("music/" + sound + ".mp3").c_str()));
        players[sound] = std::make_shared<SoundPlayer>(sound);
    }
    if (BGM.frameCount > 0) {
        UpdateMusicStream(BGM);
    }
    timer.update(dt);
    // call update on all sound players
    if (soundsLoaded) {
        if (!bgmInit) {
            startBGM();
            bgmInit = true;
            SetSoundVolume(sounds["troop_walking"], 1.0f);
            SetSoundVolume(sounds["coin_dropped"], 1.0f);
        }
        for (auto &sp : players) {
            sp.second->update(dt);
        }
    }
}

Sound AudioManager::getSound(std::string sound) {
    return sounds[sound];
}

void AudioManager::startBGM() {
    stopBGM();
    BGM = LoadMusicStream(getAssetPath(("music/normal.mp3")));
    // BGM = LoadMusicStream("sound/music.wav");
    BGM.looping = true;
    PlayMusicStream(BGM);
    currentBGM = "normal";
}

void AudioManager::stopBGM() {

    if (BGM.frameCount > 0) {
        StopMusicStream(BGM);
        UnloadMusicStream(BGM);
        BGM.frameCount = 0;
    }
}

bool AudioManager::areSoundsLoaded() {
    return soundsLoaded;
}

void AudioManager::playRandomCombatSound() {
    int random_ind = getRandomIntInRange(1, 5);
    std::string song_id = TextFormat("combat%d", random_ind);
    players[song_id]->play();
}

void AudioManager::playRandomSwishSound() {
    int random_ind = getRandomIntInRange(1, 3);
    std::string song_id = TextFormat("archer_swish%d", random_ind);
    players[song_id]->play();
}

void AudioManager::playBuildingHitSound() {
    int random_ind = getRandomIntInRange(1, 2);
    std::string song_id = TextFormat("building_hit%d", random_ind);
    players[song_id]->play();
}

void AudioManager::switchBGM(std::string next, float fadeTime) {
    if (currentBGM == next) {
        return;
    }
    this->timeTrack = 0;
    timer.during(
        fadeTime / 2,
        [this, fadeTime](float dt) {
            this->timeTrack += dt;
            float finalVolume = 1 - this->timeTrack / (fadeTime / 2);
            if (finalVolume < 0) {
                finalVolume = 0;
            }
            SetMusicVolume(this->BGM, finalVolume);
        },
        [this]() { this->timeTrack = 0; }, "");
    timer.after(
        fadeTime / 2 + .11,
        [this, fadeTime, next](float dt) {
            this->stopBGM();
            Music nextBGM = LoadMusicStream(
                getAssetPath(("music/" + next + ".mp3").c_str()));
            nextBGM.looping = true;
            SetMusicVolume(nextBGM, 0);
            PlayMusicStream(nextBGM);
            this->BGM = nextBGM;
            this->timeTrack = 0;
            this->timer.during(
                fadeTime / 2,
                [this, fadeTime](float dt) {
                    this->timeTrack += dt;
                    float finalVolume = this->timeTrack / (fadeTime / 2);
                    if (finalVolume > 1) {
                        finalVolume = 1;
                    }
                    SetMusicVolume(this->BGM, finalVolume);
                },
                [this]() { this->timeTrack = 0; }, "");
        },
        "");
    currentBGM = next;
}

void AudioManager::fadeIn(std::string name,
                          std::unordered_map<std::string, float> opts) {

    float *time = new float(0);
    BGM = LoadMusicStream(getAssetPath(("sound/" + name + ".ogg").c_str()));
    if (opts.find("looping") != opts.end()) {
        BGM.looping = true;
    }
    SetMusicVolume(BGM, 0);
    timer.during(
        opts["duration"],
        [this, &opts, &time](float dt) {
            *time += dt;
            SetMusicVolume(BGM, opts["volume"] * (*time) / opts["duration"]);
        },
        []() {}, "computer_background");
}

void AudioManager::fadeOut(Music source, float duration, float volume) {
    float *time = new float(0);
    timer.during(
        duration,
        [this, &source, &time, volume, duration](float dt) {
            *time += dt;
            SetMusicVolume(source, volume * (1 - *time / duration));
        },
        []() {}, "computer_background");
}
