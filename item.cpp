#include "item.h"
#include "inventory.h" // necessary for cross inclusions
#include "player.h"
void StaminaVialInit(StaminaVial& sv)
{
	sv.Sprite = LoadTexture("resources/itemsprites/staminavialsprite.png");
	sv.Value = 3;
	sv.UseSfx = LoadSound("resources/sfx/UseVial.wav");
	SetSoundVolume(sv.UseSfx, 5.0);
}

void VitalityVialInit(VitalityVial& hv)
{
	hv.Sprite = LoadTexture("resources/itemsprites/vitalityvialsprite.png");
	hv.Value = 50;
	hv.UseSfx = LoadSound("resources/sfx/UseVial.wav");
	SetSoundVolume(hv.UseSfx, 5.0);
}

void UseStaminaVial(StaminaVial& sv, Player& p, Inventory& i)
{
	bool Usable = false;
	int Gain = 0;
	if (i.StaminaVialCount > 0 && p.IsAlive == true) Usable = true;
	if (Usable == true) {
		if (p.Stamina + sv.Value > p.StaminaMax) {
			p.Stamina = p.StaminaMax;
		}
		else {
			p.Stamina += sv.Value;
		}
		PlaySound(sv.UseSfx);
	}
	i.StaminaVialCount--;
	return;
}

void UseVitalityVial(VitalityVial& hv, Player& p, Inventory& i)
{
	bool Usable = false;
	int Gain = 0;
	if (i.VitalityVialCount > 0 && p.IsAlive == true) Usable = true;
	if (Usable == true) {
		if (p.Health + hv.Value > p.HealthMax) {
			p.Health = p.HealthMax;
		}
		else {
			p.Health += hv.Value;
		}
		PlaySound(hv.UseSfx);
	}
	i.VitalityVialCount--;
	return;
}
