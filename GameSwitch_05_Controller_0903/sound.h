#pragma once

#include <nn/atk.h>
#include "SoundData.fsid"

void InitSound();
void UninitSound();
void UpdateSound();


void PlayBGM(nn::atk::SoundArchive::ItemId soundId);
void StopBGM();
void SetVolumeBGM(float volume, int delayFrame=0);

void PlaySE(nn::atk::SoundArchive::ItemId soundId);
