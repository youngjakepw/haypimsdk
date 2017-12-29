

#define _CRT_SECURE_NO_WARNINGS

#include "MiscHacks.h"
#include "Interfaces.h"
#include "RenderManager.h"
#include "CRC32.h"
#include <algorithm>
#include <time.h>
#include "GUI.h"

template<class T, class U>
inline T clamp(T in, U low, U high)
{
	if (in <= low)
		return low;
	else if (in >= high)
		return high;
	else
		return in;
}

inline float bitsToFloat(unsigned long i)
{
	return *reinterpret_cast<float*>(&i);
}

inline float FloatNegate(float f)
{
	return bitsToFloat(FloatBits(f) ^ 0x80000000);
}

Vector AutoStrafeView;

void CMiscHacks::Init()
{
	// Any init
}

void CMiscHacks::Draw()
{

}

int CircleFactor = 0;
void CMiscHacks::RotateMovement(CInput::CUserCmd* pCmd, float rotation)
{
	rotation = DEG2RAD(rotation);

	float cos_rot = cos(rotation);
	float sin_rot = sin(rotation);

	float new_forwardmove = (cos_rot * pCmd->forwardmove) - (sin_rot * pCmd->sidemove);
	float new_sidemove = (sin_rot * pCmd->forwardmove) + (cos_rot * pCmd->sidemove);

	pCmd->forwardmove = new_forwardmove;
	pCmd->sidemove = -new_sidemove;

}


bool CMiscHacks::doCircularStrafe(CInput::CUserCmd* pCmd, Vector& OriginalView) {
	IClientEntity *pLocalEntity = Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());

	if (!pLocalEntity)
		return false;

	if (!pLocalEntity->IsAlive())
		return false;

	CircleFactor++;
	if (CircleFactor > 361)
		CircleFactor = 0;

	int GetItDoubled = 3.0 * CircleFactor - Interfaces::Globals->interval_per_tick;

	Vector StoredViewAngles = pCmd->viewangles;

	if ((pCmd->buttons & IN_JUMP) || !(pLocalEntity->GetFlags() & FL_ONGROUND))
	{
		pCmd->viewangles = OriginalView;
		pCmd->forwardmove = 450.f;
		RotateMovement(pCmd, GetItDoubled);
	}

	return true;
}


void CMiscHacks::Move(CInput::CUserCmd *pCmd, bool &bSendPacket)
{
	// Any Move Stuff
	Vector vecOriginalView;
	IClientEntity *pLocal = Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());


	// AutoStrafe
	Interfaces::Engine->GetViewAngles(AutoStrafeView);
}



static __declspec(naked) void __cdecl Invoke_NET_SetConVar(void* pfn, const char* cvar, const char* value)
{
	__asm
	{
		push    ebp
		mov     ebp, esp
		and     esp, 0FFFFFFF8h
		sub     esp, 44h
		push    ebx
		push    esi
		push    edi
		mov     edi, cvar
		mov     esi, value
		jmp     pfn
	}
}
void DECLSPEC_NOINLINE NET_SetConVar(const char* value, const char* cvar)
{
	static DWORD setaddr = Utilities::Memory::FindPattern("engine.dll", (PBYTE)"\x8D\x4C\x24\x1C\xE8\x00\x00\x00\x00\x56", "xxxxx????x");
	if (setaddr != 0)
	{
		void* pvSetConVar = (char*)setaddr;
		Invoke_NET_SetConVar(pvSetConVar, cvar, value);
	}
}



void CMiscHacks::AutoJump(CInput::CUserCmd *pCmd)
{
	if (pCmd->buttons & IN_JUMP && GetAsyncKeyState(VK_SPACE))
	{
		int iFlags = hackManager.pLocal()->GetFlags();
		if (!(iFlags & FL_ONGROUND))
			pCmd->buttons &= ~IN_JUMP;

		if (hackManager.pLocal()->GetVelocity().Length() <= 50)
		{
			pCmd->forwardmove = 450.f;
		}
	}
}

void CMiscHacks::LegitStrafe(CInput::CUserCmd *pCmd)
{
	IClientEntity* pLocal = hackManager.pLocal();
	if (!(pLocal->GetFlags() & FL_ONGROUND))
	{
		pCmd->forwardmove = 0.0f;

		if (pCmd->mousedx < 0)
		{
			pCmd->sidemove = -450.0f;
		}
		else if (pCmd->mousedx > 0)
		{
			pCmd->sidemove = 450.0f;
		}
	}
}

void CMiscHacks::RageStrafe(CInput::CUserCmd *pCmd)
{
		IClientEntity* pLocal = hackManager.pLocal();

		bool bKeysPressed = true;
		if (GUI.GetKeyState(0x41) || GUI.GetKeyState(0x57) || GUI.GetKeyState(0x53) || GUI.GetKeyState(0x44)) bKeysPressed = false;

		if ((GetAsyncKeyState(VK_SPACE) && !(pLocal->GetFlags() & FL_ONGROUND)) && bKeysPressed)
		{
			if (pCmd->mousedx > 1 || pCmd->mousedx < -1) {
				pCmd->sidemove = pCmd->mousedx < 0.f ? -450.f : 450.f;
			}
			else {
				pCmd->forwardmove = (1800.f * 4.f) / pLocal->GetVelocity().Length2D();
				pCmd->sidemove = (pCmd->command_number % 2) == 0 ? -450.f : 450.f;
				if (pCmd->forwardmove > 450.f)
					pCmd->forwardmove = 450.f;
			}
		}

		/*	IClientEntity* pLocal = hackManager.pLocal();
		static bool bDirection = true;


		static float move = 450; //400.f; // move = max(move, (abs(cmd->move.x) + abs(cmd->move.y)) * 0.5f);
		float s_move = move * 0.5065f;


		if ((pCmd->buttons & IN_JUMP) || !(pLocal->GetFlags() & FL_ONGROUND))
		{
		pCmd->forwardmove = move * 0.015f;
		pCmd->sidemove += (float)(((pCmd->tick_count % 2) * 2) - 1) * s_move;

		if (pCmd->mousedx)
		pCmd->sidemove = (float)clamp(pCmd->mousedx, -1, 1) * s_move;

		static float strafe = pCmd->viewangles.y;

		float rt = pCmd->viewangles.y, rotation;
		rotation = strafe - rt;

		if (rotation < FloatNegate(Interfaces::Globals->interval_per_tick))
		pCmd->sidemove = -s_move;

		if (rotation > Interfaces::Globals->interval_per_tick)
		pCmd->sidemove = s_move;

		strafe = rt;
		} */
}

Vector GetAutostrafeView()
{
	return AutoStrafeView;
}


void CMiscHacks::Crutcke(CInput::CUserCmd *pCmd)
{
		IClientEntity* pLocal = hackManager.pLocal();
		static bool bDirection = true;

		bool bKeysPressed = true;

		if (GUI.GetKeyState(0x41) || GUI.GetKeyState(0x57) || GUI.GetKeyState(0x53) || GUI.GetKeyState(0x44))
			bKeysPressed = false;
		//if (pCmd->buttons & IN_ATTACK)
		//	bKeysPressed = false;

		float flYawBhop = 0.f;
		if (pLocal->GetVelocity().Length() > 45.f)
		{
			float x = 30.f, y = pLocal->GetVelocity().Length(), z = 0.f, a = 0.f;

			z = x / y;
			z = fabsf(z);

			a = x * z;

			flYawBhop = a;
		}

		if ((GetAsyncKeyState(VK_SPACE) && !(pLocal->GetFlags() & FL_ONGROUND)) && bKeysPressed)
		{

			if (bDirection)
			{
				AutoStrafeView -= flYawBhop;
				GameUtils::NormaliseViewAngle(AutoStrafeView);
				pCmd->sidemove = -450;
				bDirection = false;
			}
			else
			{
				AutoStrafeView += flYawBhop;
				GameUtils::NormaliseViewAngle(AutoStrafeView);
				pCmd->sidemove = 430;
				bDirection = true;
			}

			if (pCmd->mousedx < 0)
			{
				if (pCmd->viewangles.x > 178 || pCmd->viewangles.x < -178)
				{
					//pCmd->forwardmove = -22;
				}
				else
				{
					//pCmd->forwardmove = 22;
				}
				pCmd->sidemove = -450;
			}

			if (pCmd->mousedx > 0)
			{
				if (pCmd->viewangles.x > 178 || pCmd->viewangles.x < -178)
				{
					//pCmd->forwardmove = -22;
				}
				else
				{
					//pCmd->forwardmove = +22;
				}
				pCmd->sidemove = 450;
			}
	}
}












int LagCompBreak() {
	IClientEntity *pLocalPlayer = Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());
	Vector velocity = pLocalPlayer->GetVelocity();
	velocity.z = 0;
	float speed = velocity.Length();
	if (speed > 0.f) {
		auto distance_per_tick = speed *
			Interfaces::Globals->interval_per_tick;
		int choked_ticks = std::ceilf(65.f / distance_per_tick);
		return std::min<int>(choked_ticks, 14);
	}
	return 1;
}


