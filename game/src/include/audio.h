#pragma once

void InitAudio();
void ShutdownAudio();
void UpdateAudio();

int LoadSoundFile(const char *soundFile);

void StartBGM(const char *musicFile);
void StopBGM();

void PlaySound(int sound);
void StopSound(int sound);
