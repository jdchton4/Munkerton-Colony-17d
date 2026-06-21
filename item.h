// @author jdeepak
// @file item.h
#pragma once

#include "raylib.h"


struct Inventory;
struct Player;

struct StaminaVial {
	int Value; 
	Texture2D Sprite;
	Sound UseSfx;
};

struct VitalityVial {
	int Value;
	Texture2D Sprite;
	Sound UseSfx;
};

void StaminaVialInit(StaminaVial& sv);
void VitalityVialInit(VitalityVial& hv);
void UseStaminaVial(StaminaVial& sv, Player& p, Inventory& i);
void UseVitalityVial(VitalityVial& hv, Player& p, Inventory& i);
