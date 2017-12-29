#pragma once

class CCacheAngle {
public:
	//Menu
	bool bCustomSetts = false;
	bool bCorrect = true;
	int iResolverType = 0;
	bool bPrioritize = false;
	int iHitbox = 1;
	bool bAutoBaim = false;
	int iAutoBaimAferShot = 2;
	int iHitboxAutoBaim = 4;
	bool bPrioritizeVis = true;
	float flMinDmg = 5.f;
	bool bInterpLagComp = false;

	//GlobalPlayer
	bool bFlip = false;
};

class COptions {
public:
	//Global
	bool bAutoHop = false;

	//Menu related
	bool bShowMenu = false;
	bool vecPressedKeys[256] = {};

	//TargetList related
	CCacheAngle aCacheAngle[64];

	//VISUALS
	bool bEspEnabled = false;
	bool bGlow = false;
	bool bHealthBar = false;
	int bBoxESP = 0; //bool bBoxESP = false;
	bool bArmorBar = false;
	bool bPlayerName = false;
	bool bPlayerIndex = false;
	bool bBoneESP = false;
	bool bWeaponESP = false;
	bool bNoScope = false;
	bool bThirdPerson = false;
	bool bSnapLines = false;
	bool bAALines = false;
	bool bSpreadCross = false;
	bool bAmmoESP = false;


	//AIMBOT
	bool bAutoShoot = true;
	bool bAutoPistol = true;
	bool bBruteforceResolver = true;
	bool bAutoRevolver = true;
	bool bAutoScope = true;
	bool bSilentAim = true;
	int iHitchance = 0;
	int iHitChanceSeed = 256;

	bool bCorrect = true;
	bool bBackTack = false;
	int bBackTrackTicks = 12;
	int iResolverType = 0;
	bool aMultiHitboxes[15] = {
		true, true, true, true, true,
		true, true, true, true, true,
		true, true, true, true, true
	};
	bool bPrioritize = true;
	int iHitbox = 0;
	bool bAutoBaim = false;
	int iAutoBaimAferShot = 3;
	int iHitboxAutoBaim = 4;
	bool bPrioritizeVis = false;
	float flMinDmg = 5.f;
	bool bInterpLagComp = true;

	//ANTI-AIM
	bool bFakeLag = false;
	int iFakeLagAmount = 1;
	bool bAA = true;
	int iAAPitch = 0;
	int iAARealYaw = 0;
	int iAAFakeYaw = 0;
	bool bSlowMo = false;
	int iLBYReal = 0;
	int iLBYFake = 0;


	// Misc
	bool bNoVisRecoil = false;
	bool bNoFlash = false;
	bool bClantagchanger = false;
	bool bWaterMark = false;
	bool bIndicators = false;
	int iFov = 90;
	int iFov2 = 90;


	//SETTINGS
	float s_MenuTransparency = 1;
	float bESPAllyColor[4] = { 0.0f, 1.0f, 0.0f, 1.0f }; //RGBA color
	float bESPEnemyColor[4] = { 1.0f, 0.0f, 0.0f, 1.0f };

	// TAB
	bool g_tRagebot = true;
	bool g_tLegitbot = false;
	bool g_tVisuals = false;
	bool g_tMisc = false;
	bool g_tSettings = false;
	bool g_hitBoxList = true;
};

extern COptions OptionsManager;