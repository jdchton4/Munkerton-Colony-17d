#include "player.h"
#include "raylib.h"
#include "camera.h"
#include "timer.h"

#define BASE_SPEED float(5.0) // p.Speed base movement Speed
#define BASE_HEALTH float(100.0) // base health
#define DASH_TIME float(.2) // Time (sec) player dashes for
#define DASH_SPEED float(50.0) // Dash Speed constant
#define DASH_COST float(4.0) // Stamina cost of Dash
#define SPRINT_SPEED float(4.0) // Additional Speed when sprinting
#define STAMINA_BASE float(5.0) // base stamina
#define STAMINA_DRAIN float(2.0) // 5 Stamina drains per second
#define STAMINA_REGEN float(4.0) // 4 Stamina regenerates per second
#define STAM_CD_THRESHOLD float(.2) // Minimum Stamina to prevent stamcd from kicking in
#define MOVE_CONST float(40.0) // Coefficient of movement to make it standardized
#define POS_INIT_X 75.0f // starting position
#define POS_INIT_Y 270.0f // starting position
#define MIN_BOUND_X -50000 // world bounds
#define MAX_BOUND_X 50000 // world bounds
#define MIN_BOUND_Y -50000 // world bounds
#define MAX_BOUND_Y 50000 // world bounds


void PlayerInit(Player& p, Timer& t) // Initializes / Resets player
{

	Texture2D PlayerSpriteIdle = LoadTexture("resources/spritesheets/alienbaseidlespritesheet.png"); // starting player texture
	Texture2D PlayerSpriteMove = LoadTexture("resources/spritesheets/alienbasemovingspritesheet.png"); // player moving texture
	Texture2D PlayerSpriteMoveNE = LoadTexture("resources/spritesheets/alienbasemovingnespritesheet.png"); // player moving diag up texture
	Texture2D PlayerSpriteMoveSE = LoadTexture("resources/spritesheets/alienbasemovingsespritesheet.png"); // player moving diag up texture
	Texture2D PlayerSpriteDash = LoadTexture("resources/spritesheets/alienbasedashingspritesheet.png"); // player moving texture
	Texture2D PlayerSpriteStatic = LoadTexture("resources/spritesheets/alienbasestaticspritesheet.png"); // static placeholder texture
	Texture2D PlayerSpriteDead = LoadTexture("resources/spritesheets/alienbasedeadspritesheet.png"); // static placeholder texture
	Texture2D PlayerSpriteDashNE = LoadTexture("resources/spritesheets/alienbasedashingnespritesheet.png"); // player dashing ne
	Texture2D PlayerSpriteDashSE = LoadTexture("resources/spritesheets/alienbasedashingsespritesheet.png"); // player dashing ne

	Sound PlayerDashSfx = LoadSound("resources/sfx/dashsfx.wav");
	Sound PlayerSprintSfx = LoadSound("resources/sfx/sprintsfx.wav");
	Sound PlayerMoveSfx = LoadSound("resources/sfx/walksfx.wav");
	Sound PlayerDeathSfx = LoadSound("resources/sfx/deathsfx.wav");
	Sound PlayerDepleteStaminaSfx = LoadSound("resources/sfx/stambardeplete.wav");
	Sound PlayerSpawnSfx = LoadSound("resources/sfx/spawnsfx.wav");
	// Arbitrary volume sounds that keep levels in check 
	SetSoundVolume(PlayerDashSfx, 5.0);
	SetSoundVolume(PlayerMoveSfx, 3.5);
	SetSoundVolume(PlayerSprintSfx, 5.0);
	SetSoundVolume(PlayerDepleteStaminaSfx, 5.0);
	p.Pos.x = POS_INIT_X; // Starts player out at bottom left on the road
	p.Pos.y = POS_INIT_Y;
	p.NextPos.x = POS_INIT_X;
	p.NextPos.y = POS_INIT_Y;
	p.StaminaUpgrade = 0;
	p.Stamina = STAMINA_BASE + p.StaminaUpgrade; // Initial Stamina
	p.StaminaMax = STAMINA_BASE + p.StaminaUpgrade; // Starting amount of Stamina that doesn't change
	p.Health = BASE_HEALTH - 80; // Player Health
	p.HealthMax = BASE_HEALTH; // Max Health for reference
	p.IsAlive = true;
	p.IsDashing = false;
	p.IsSprinting = false;
	p.IsIdle = true;
	p.IsWalking = false;
	p.StamCD = false; // Triggers when you deplete all Stamina at once; Prevents you from using Stamina until true
	p.InventoryIsOpen = false;
	p.Facing = Player::NO_DIR; // Directions (see player.h for enum)
	p.State = Player::IDLE;
	p.WorldProgress = 1; // Relative game difficulty with progression ( 1 - 10 )
	p.TextureStatic = PlayerSpriteStatic; // Unmoving Texture sheet
	p.TextureIdle = PlayerSpriteIdle; // No input texture sheet
	p.TextureMoving = PlayerSpriteMove;
	p.TextureMovingNE = PlayerSpriteMoveNE; // Eye movement for diagonal direction
	p.TextureMovingSE = PlayerSpriteMoveSE; // Eye movement for diagonal direction
	p.TextureDashing = PlayerSpriteDash; // Mirage effect 
	p.TextureDashingNE = PlayerSpriteDashNE;
	p.TextureDashingSE = PlayerSpriteDashSE;
	p.TextureDead = PlayerSpriteDead; // Dead sprite
	p.CurrentTexture = PlayerSpriteIdle;
	p.DashSfx = PlayerDashSfx;
	p.MoveSfx = PlayerMoveSfx;
	p.SprintSfx = PlayerSprintSfx;
	p.SpawnSfx = PlayerSpawnSfx;
	p.StamDepleteSfx = PlayerDepleteStaminaSfx;
	p.DeathSfx = PlayerDeathSfx;
	t.DeathSoundCount = 0;

	p.HitboxH.width = p.CurrentTexture.width / 5; // Hitbox initialization for Head, Torso, Legs
	p.HitboxT.width = p.CurrentTexture.width / 5;
	p.HitboxL.width = p.CurrentTexture.width / 5;
	p.HitboxH.height = p.CurrentTexture.height;
	p.HitboxT.height = p.CurrentTexture.height;
	p.HitboxL.height = p.CurrentTexture.height;
	p.HitboxH.x = POS_INIT_X;
	p.HitboxT.x = POS_INIT_X;
	p.HitboxL.x = POS_INIT_X;
	p.HitboxH.y = POS_INIT_Y;
	p.HitboxT.y = POS_INIT_Y;
	p.HitboxL.y = POS_INIT_Y;




	PlaySound(p.SpawnSfx);
}

void PlayerStamina(Player& p, float dt, Timer& t) // Manages Stamina system ( Regen, Cooldown, Sound )
{
	// When Stamina falls below (.2), Stamina cooldown (No Stamina Actions allowed) & Play SFX
	if (p.Stamina <= STAM_CD_THRESHOLD) {
		p.StamCD = true;
		if (!IsSoundPlaying(p.StamDepleteSfx)) {
			PlaySound(p.StamDepleteSfx);
		}
	}
	if (p.Stamina < p.StaminaMax && !p.IsSprinting) { // if missing Stamina && not Sprinting, start ticking t.StamTick
		t.StamTick += dt;
	}

	// Stamina Regen
	if (!p.IsSprinting && !p.IsDashing && p.Stamina < p.StaminaMax && t.StamTick >= 2.0f) { // if not Sprinting or dashing, You are missing Stamina and have waited the delay amount
		p.Stamina += STAMINA_REGEN * dt; // regenerate 4 Stamina per second 
		if (p.Stamina > p.StaminaMax) p.Stamina = p.StaminaMax; // Stop Stamina overflow
	}
	if (p.Stamina == p.StaminaMax) p.StamCD = false; // After reaching max Stamina, cooldown is over and you can act again
}


void PlayerDash(Player& p, float dt, Timer& t, float& CurrentSpeed) // Handles player dash and dash sound
{
	if (IsKeyPressed(KEY_SPACE) && p.Stamina > DASH_COST && p.StamCD == false && p.IsWalking == true) { // Dash Conditions: Have sufficient Stamina, be in motion, not be in CD
		p.IsDashing = true;
		p.DashTimer = DASH_TIME; // Starts timer for dash duration
		p.Stamina -= DASH_COST; // Takes Stamina cost all at once
		t.StamTick = 0.0; // reset ticker
	}

	if (p.IsDashing == true) { // Decrement dash timer until 0
		p.DashTimer -= dt; 
		if (p.DashTimer <= 0) {
			p.IsDashing = false;
		}
		StopSound(p.MoveSfx);
		if (!IsSoundPlaying(p.DashSfx)) PlaySound(p.DashSfx); // Sfx
		p.DashBuff = DASH_SPEED; // Apply buff
		CurrentSpeed = p.Speed + p.DashBuff;
	}
	return;
}

void PlayerSprint(Player& p, float dt, Timer& t, float& CurrentSpeed) // Handles player sprinting & sprint sound
{
	// Conditions for sprinting
	if (p.IsDashing == false && IsKeyDown(KEY_LEFT_SHIFT) && p.Stamina > 0.0 && p.StamCD == false) p.IsSprinting = true;

	if (p.IsSprinting == true) {
		if (IsSoundPlaying(p.MoveSfx)) StopSound(p.MoveSfx); // Sfx
		if (!IsSoundPlaying(p.SprintSfx)) PlaySound(p.SprintSfx); // Sfx
		p.SprintBuff = SPRINT_SPEED; // Apply buff
		CurrentSpeed = p.Speed + p.SprintBuff; // Sprint buff is added onto Speed
		p.Stamina -= STAMINA_DRAIN * dt; // drain x Stamina every second
		if (p.Stamina < 0) p.Stamina = 0; // prevent Stamina underflow
		t.StamTick = 0; // reset regen counter while Sprinting 
	}
	if (p.IsSprinting == false) {
		if (IsSoundPlaying(p.SprintSfx)) StopSound(p.SprintSfx); // Sfx

		if (p.IsWalking && !IsSoundPlaying(p.MoveSfx)) {
			PlaySound(p.MoveSfx);
		}
		else if (!p.IsWalking || p.IsSprinting) {
			if (IsSoundPlaying(p.MoveSfx)) StopSound(p.MoveSfx); // Sfx
		}
	}
	return;
}

void GetPlayerInput(Player& p, float& MoveX, float& MoveY) // Handles player input & orientation
{
	// base movement
	if (IsKeyDown(KEY_D)) { // D -> RIGHT
		MoveX = 1;
		if (IsKeyDown(KEY_W)) {
			p.Facing = Player::NE;
		}
		else if (IsKeyDown(KEY_S)) {
			p.Facing = Player::SE;
		}
		else {
			p.Facing = Player::E;
		}
	}
	if (IsKeyDown(KEY_A)) { // A -> LEFT
		MoveX = -1;
		if (IsKeyDown(KEY_W)) {
			p.Facing = Player::NW;
		}
		else if (IsKeyDown(KEY_S)) {
			p.Facing = Player::SW;
		}
		else {
			p.Facing = Player::W;
		}
	}
	if (IsKeyDown(KEY_S)) { // S -> DOWN
		MoveY = 1;
	}
	if (IsKeyDown(KEY_W)) { // W -> UP
		MoveY = -1;
	}

	// Diagonal movement scaling
	if (MoveX != 0 && MoveY != 0) {
		MoveX = MoveX * .7071f;
		MoveY = MoveY * .7071f;

	}

	// move check
	if (MoveX != 0 || MoveY != 0) {
		p.IsIdle = false;
		p.IsWalking = true;
	}
	else {
		p.IsWalking = false;
		p.IsIdle = true;
	}
}

void PlayerDeath(Player& p, Timer& t) { // Triggers in case of player losing all Health; Manages sounds upon death
	if (p.Health <= 0.1) {
		p.IsAlive = false;
		StopSound(p.MoveSfx);
		StopSound(p.DashSfx);
		StopSound(p.SprintSfx);
		if (!IsSoundPlaying(p.DeathSfx) && t.DeathSoundCount < 50) PlaySound(p.DeathSfx);
		t.DeathSoundCount++;

	}
	if (IsKeyDown(KEY_FOUR)) p.Health = 0;
	if (IsKeyDown(KEY_FIVE)) {
		StopSound(p.DashSfx);
		StopSound(p.MoveSfx);
		StopSound(p.SprintSfx);
		StopSound(p.SpawnSfx);
		PlayerInit(p, t);
	}
}

void UpdatePlayer(Player& p, Level& l, Timer& t) { // Main function handling player movement, Stamina, dash, sprint, bounds, input, death check in one
	// Var Init
	float dt = GetFrameTime(); // Sec/Frame
	p.Speed = BASE_SPEED; // Minimum Speed
	float CurrentSpeed = p.Speed;
	float MoveX = 0;
	float MoveY = 0;
	p.IsSprinting = false;
	p.SprintBuff = 0;
	p.DashBuff = 0;
	Vector2 MovementChange = { 0, 0 };

	// Call Sub Functions
	PlayerDeath(p, t);
	if (p.IsAlive == true) {
		PlayerStamina(p, dt, t); // Stamina Depletion / Regen / SFX
		PlayerDash(p, dt, t, CurrentSpeed); // Dash Mechanics / SFX
		PlayerSprint(p, dt, t, CurrentSpeed); // Sprint Mechanics / SFX
		GetPlayerInput(p, MoveX, MoveY); // User Input / Diagonal Movement Scaling / Direction Facing
	}

	float MoveDistanceX = CurrentSpeed * MoveX * dt * MOVE_CONST; // MOVE_CONST multiplies movement factor to a playable amount
	float MoveDistanceY = CurrentSpeed * MoveY * dt * MOVE_CONST;

	p.Pos.x = p.Pos.x + MoveDistanceX;
	p.Pos.y = p.Pos.y + MoveDistanceY;

	// boundary holds
	if (p.Pos.x < MIN_BOUND_X)    p.Pos.x = MIN_BOUND_X;
	if (p.Pos.x > MAX_BOUND_X) p.Pos.x = MAX_BOUND_X;
	if (p.Pos.y < MIN_BOUND_X)    p.Pos.y = MIN_BOUND_X;
	if (p.Pos.y > MAX_BOUND_X) p.Pos.y = MAX_BOUND_X;
}

void PlayerHitbox(Player& p) {
	float frameWidth = ((float)p.CurrentTexture.width / 5.0f) ;
	float frameHeight = (float)p.CurrentTexture.height * 1.7 ;

	p.HitboxH.width = frameWidth;
	p.HitboxH.height = frameHeight;
	p.HitboxH.x = p.Pos.x - 50.0;
	p.HitboxH.y = p.Pos.y - 70.0;
}


void DrawPlayer(Player& p) { // Draws Player to screen; Handles player textures and animation
	p.CurrentTexture = p.TextureStatic;
	float FrameSpeed = 1.0f;
	bool StateChange = false; 
	p.scale = 5;
	float FrameWidth = (float)p.CurrentTexture.width / 5.00;
	float FrameHeight = (float)p.CurrentTexture.height;
	float ScaledWidth = FrameWidth * p.scale;
	float ScaledHeight = FrameHeight * p.scale;
	static int CurrentFrame = 0;
	static int FrameCount = 0;
	Vector2 origin = { ScaledWidth / 2.0f, ScaledHeight / 2.0f };
	FrameCount++;
	Rectangle FrameRect = { CurrentFrame * FrameWidth, 0.0f, FrameWidth, FrameHeight };
	Rectangle DestRect = { p.Pos.x, p.Pos.y, ScaledWidth, ScaledHeight };

	PlayerHitbox(p);


	if (p.IsAlive == false) { // If Dead PROPERTY is active, then if player state isnt dead, set it so and flag state change
		if (p.State != Player::DEAD) {
			p.State = Player::DEAD;
			StateChange = true;
		}
	}
	else if (p.IsAlive == true && p.State == Player::DEAD) p.State = Player::IDLE; // If Dead property is false but state is still Dead ( for respawn ), reset state to idle
	else if (p.State != Player::DEAD) { // when alive
		if (p.IsDashing == true) { // if dash property is active
			if (p.State != Player::DASHING) { // if state isnt already dashing, set it to dash and raise flag
				p.State = Player::DASHING;
				StateChange = true;
			}
		}
		else if (p.IsSprinting == true) { // if not dashing, then maybe sprinting?
			if (p.State != Player::SPRINTING) {
				p.State = Player::SPRINTING;
				StateChange = true;
			}
		}
		else if (p.IsWalking == true) { // If walking property is active and not dashing or sprinting
			if (p.State != Player::MOVING) { // if state isnt already walking, set it and raise flag
				p.State = Player::MOVING;
				StateChange = true;
			}
		}
		else if (p.IsWalking == false) { // if not walking OR dashing OR sprinting then player isnt moving
			if (p.State != Player::IDLE) { // set idle state and raise flag
				p.State = Player::IDLE;
				StateChange = true;
			}
		}
	}
		switch (p.State) {
		case Player::IDLE:
			FrameSpeed = 1.0; // slow frame speed
			p.CurrentTexture = p.TextureIdle;
			break;
		case Player::DASHING:
			FrameSpeed = 30.0; // frame speed covers entire animation and locks at last frame during draw phase
			if (p.Facing == Player::NE || p.Facing == Player::NW) {
				p.CurrentTexture = p.TextureDashingNE;
			}
			else if (p.Facing == Player::SE || p.Facing == Player::SW) {
				p.CurrentTexture = p.TextureDashingSE;
			}
			else {
				p.CurrentTexture = p.TextureDashing;
			}
			break;
		case Player::SPRINTING:
			FrameSpeed = 7.0;
			if (p.Facing == Player::NE || p.Facing == Player::NW) { // Texture gets flipped for NW so it works for that too
				p.CurrentTexture = p.TextureMovingNE;
			}
			else if (p.Facing == Player::SE || p.Facing == Player::SW) {
				p.CurrentTexture = p.TextureMovingSE;
			}
			else {
				p.CurrentTexture = p.TextureMoving;
			}
			break;
		case Player::MOVING:
			FrameSpeed = 5.0;
			if (p.Facing == Player::NE || p.Facing == Player::NW) { // Texture gets flipped for NW so it works for that too
				p.CurrentTexture = p.TextureMovingNE;
			}
			else if (p.Facing == Player::SE || p.Facing == Player::SW) {
				p.CurrentTexture = p.TextureMovingSE;
			}
			else {
				p.CurrentTexture = p.TextureMoving;
			}
			break;
		case Player::DEAD:
			FrameSpeed = .3;
			p.CurrentTexture = p.TextureDead;
			break;
		}

	if (StateChange) {
		CurrentFrame = 0;
		FrameCount = 0;
	}
	if (FrameCount >= (60 / FrameSpeed))
	{
		FrameCount = 0;
		CurrentFrame++;
		if (p.State == Player::DASHING) {
			if (CurrentFrame > 4) CurrentFrame = 4;
		}
		else if (p.State == Player::DEAD) {
			if (CurrentFrame > 4) CurrentFrame = 4;
		}
		else {
			if (CurrentFrame > 4) CurrentFrame = 0;
		}
	}

	if (p.Facing == Player::W || p.Facing == Player::NW || p.Facing == Player::SW) FrameRect.width = FrameWidth * -1;

	DrawTexturePro(p.CurrentTexture, FrameRect, DestRect, origin, 0.0f, WHITE);
	StateChange = false;
	DrawRectangleLinesEx(p.HitboxH, 2.0f, RED);
	return;
}

void SetPlayerPos(Player& p, Vector2 dest) { // Used for teleporting Player
	float saveposx = p.Pos.x;
	float saveposy = p.Pos.y;

	p.Pos.x = dest.x;
	p.Pos.y = dest.y;
	return;
}


Vector2 GetPlayerPos(Player& p) { // Returns player position
	return { p.Pos.x, p.Pos.y };
}

/* void SendPlayerDMG(int dmg, Player& p, Timer& t) {
* if iFrame timer reaches a threshold, player is valid to hit
* if player is valid to hit, 
	send p.DmgQueue the updated dmg value (p.DmgQueue += dmg)
	reset timer to 0 (timer should automatically tick from UpdatePlayer(); 
* 
*/

/* void ReceivePlayerDmg(Player&p, Timer& t)
* Check p.DmgQueue for value > 0
* if p.DmgQueue > 0 { 
	start ticking dmgbuffer timer
	if dmgbuffer > threshold ( a few seconds )
		p.health -= p.DmgQueue
		p.DmgTaken = p.DmqQueue
		p.DmgQueue = 0
		DmgMsg timer can start (in draw player, if t.DmgMsg timer is > 0, it means player has already taken damage, display a fading message above player that indicates damage taken
		, it will handle resetting DmgMsg timer
*
*/