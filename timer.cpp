// timer.cpp
#include "raylib.h"
#include "timer.h"



void TimerInit(Timer& t) { // Initializes global timers; These timers can be modified with the Timer struct by including Timer& t as a parameter
	t.EnemyGenTick = 0;
	t.InvTick = 0;
	t.StamTick = 0;
	t.CamShakeTick = 0;
	t.DeathSoundCount = 0;
	t.InvMsg = 0;
	t.MudEatCharge = 0;
	t.MudEatCD = 0;
	t.InvMsg = 0.0;
	t.TextOpacity = 255;
	return;
}