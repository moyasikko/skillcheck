#pragma once

void InitPlayer();
void UninitPlayer();
void UpdatePlayer();
void DrawPlayer();

int CollisionPlayer(Float2);
Float2 PositionPlayer();
Float2 PositionOldPlayer();