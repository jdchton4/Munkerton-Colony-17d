// @author jdeepak
// @file timer.h

#pragma once
#include "raylib.h"


struct Timer {
	float EnemyGenTick;	// Timer that ticks up until a threshold to spawn an enemy
	float InvTick; 	// invtick is a buffer so that opening the inventory doesn't immediately close it from button bouncing
	float StamTick; // stam tick is the timer after using stamina that ticks until a threshold that then allows you to start regenerating stamina
	float CamShakeTick; // 
	int DeathSoundCount; // Death sound count is a timer that plays for the sounds duration that stops it from looping
	float InvMsg; // InvMsg is the timer that ticks for when an unusable item is clicked on; after the timer expires, the message disappears
	float MudEatCharge; 
	float MudEatCD;
	unsigned char TextOpacity; // Textopacity is a timer that ticks for the inventory; its value corresponds with the InvMsg text opacity
};

void TimerInit(Timer& t);