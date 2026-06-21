// @author jdeepak
#pragma once
#include "raylib.h"

struct Timer;
struct Player;

void CameraInit(Camera2D& c, Player& p);
void CameraScreenShake(Camera2D& c, Player& p, Timer& t, float Duration);