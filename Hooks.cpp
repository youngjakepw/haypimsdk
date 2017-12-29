#include "Hooks.h"
#include "Hacks.h"
#include <intrin.h>
#include "DLLMain.h"
#include "Interfaces.h"
#include "RenderManager.h"
#include "MiscHacks.h"
#include "CRC32.h"
#include <map>
#include <iostream>
#include <cstdio>
#include <ctime>
#include "Interfaces.h"
#include "MiscDef.h"
#include "Entities.h"
#include "Hooks.h"
#include "GameEventManager.h"
#include <deque>
#include <algorithm>
#include "XorStr.h"
#include "GUI.h"

namespace G
{
	CInput::CUserCmd *UserCmd;
}



typedef bool(__stdcall *create_move)(float, CInput::CUserCmd*);
typedef void(__thiscall* PaintTraverse_)(PVOID, unsigned int, bool, bool);
typedef void(__stdcall *FrameStageNotifyFn)(ClientFrameStage_t);

create_move oCreateMove;
PaintTraverse_ oPaintTraverse;
FrameStageNotifyFn oFrameStageNotify;



bool __stdcall CreateMoveClient_Hooked(/*void* self, int edx,*/ float frametime, CInput::CUserCmd* pCmd);
void __fastcall PaintTraverse_Hooked(PVOID pPanels, int edx, unsigned int vguiPanel, bool forceRepaint, bool allowForce);
void  __stdcall Hooked_FrameStageNotify(ClientFrameStage_t curStage);

namespace Hooks
{
	// VMT Managers
	Utilities::Memory::VMTManager VMTPanel; // Hooking drawing functions
	Utilities::Memory::VMTManager VMTClient; // Maybe CreateMove
	Utilities::Memory::VMTManager VMTClientMode; // CreateMove for functionality
	Utilities::Memory::VMTManager VMTClientMode2; // CreateMove2 for functionality
	Utilities::Memory::VMTManager VMTModelRender; // DrawModelEx for chams
	Utilities::Memory::VMTManager VMTPrediction; // InPrediction for no vis recoil
	Utilities::Memory::VMTManager VMTPlaySound; // Autoaccept 
	Utilities::Memory::VMTManager VMTRenderView;
	Utilities::Memory::VMTManager VMTEventManager; // EventManager
	Utilities::Memory::VMTManager g_pD3DDevice9Hook; // EventManager
	Utilities::Memory::VMTManager VMTRunCommand; // EventManager
};

void Hooks::UndoHooks()
{
	VMTPanel.RestoreOriginal();
	VMTPrediction.RestoreOriginal();
	VMTModelRender.RestoreOriginal();
	VMTClientMode.RestoreOriginal();
	VMTClientMode2.RestoreOriginal();
	g_pD3DDevice9Hook.RestoreOriginal();
}


void Hooks::Initialise()
{
	VMTPanel.Initialise((DWORD*)Interfaces::Panels);
	oPaintTraverse = (PaintTraverse_)VMTPanel.HookMethod((DWORD)&PaintTraverse_Hooked, Offsets::VMT::Panel_PaintTraverse);

	VMTPrediction.Initialise((DWORD*)Interfaces::Prediction);
	//Hooked_InPrediction(Ex for no recoil)

	VMTModelRender.Initialise((DWORD*)Interfaces::ModelRender);
	//DME Hook

	VMTClientMode.Initialise((DWORD*)Interfaces::ClientMode);
	VMTClientMode.HookMethod((DWORD)CreateMoveClient_Hooked, 24);

	VMTClient.Initialise((DWORD*)Interfaces::Client);
	oFrameStageNotify = (FrameStageNotifyFn)VMTClient.HookMethod((DWORD)&Hooked_FrameStageNotify, 36);

	VMTEventManager.Initialise((DWORD*)Interfaces::GameEventManager);

	auto dwDevice = **(uint32_t**)(Utilities::Memory::FindPatternV2("shaderapidx9.dll", "A1 ? ? ? ? 50 8B 08 FF 51 0C") + 1);
	while (!(DirectX::g_hWindow = FindWindowA(XorStr("Valve001"), NULL))) Sleep(200);

	if (DirectX::g_hWindow)
		DirectX::g_pOldWindowProc = (WNDPROC)SetWindowLongPtr(DirectX::g_hWindow, GWLP_WNDPROC, (LONG_PTR)DirectX::Hooked_WndProc);

	g_pD3DDevice9Hook.Initialise((DWORD*)dwDevice);
	g_pD3DDevice9Hook.HookMethod((DWORD)DirectX::Hooked_Reset, 16);
	g_pD3DDevice9Hook.HookMethod((DWORD)DirectX::Hooked_EndScene, 42);
	DirectX::g_fnOriginalReset = (DirectX::Reset_t)g_pD3DDevice9Hook.HookMethod((DWORD)DirectX::Hooked_Reset, 16);
	DirectX::g_fnOriginalEndScene = (DirectX::EndScene_t)g_pD3DDevice9Hook.HookMethod((DWORD)DirectX::Hooked_EndScene, 42);
}

class CFixMove {
public:
	void Start(CInput::CUserCmd*);
	void Stop(CInput::CUserCmd*);
	QAngle GetOldAngle();
private:
	QAngle old;
	float oldFowardMove;
	float oldSideMove;

	void CorrectMovement(QAngle vOldAngles, CInput::CUserCmd* pCmd, float fOldForward, float fOldSidemove);
};

void CFixMove::CorrectMovement(QAngle vOldAngles, CInput::CUserCmd* pCmd, float fOldForward, float fOldSidemove)
{
	//side/forward move correction
	float deltaView = pCmd->viewangles.y - vOldAngles.y;
	float f1;
	float f2;

	if (vOldAngles.y < 0.f)
		f1 = 360.0f + vOldAngles.y;
	else
		f1 = vOldAngles.y;

	if (pCmd->viewangles.y < 0.0f)
		f2 = 360.0f + pCmd->viewangles.y;
	else
		f2 = pCmd->viewangles.y;

	if (f2 < f1)
		deltaView = abs(f2 - f1);
	else
		deltaView = 360.0f - abs(f1 - f2);
	deltaView = 360.0f - deltaView;

	pCmd->forwardmove = cos(DEG2RAD(deltaView)) * fOldForward + cos(DEG2RAD(deltaView + 90.f)) * fOldSidemove;
	pCmd->sidemove = sin(DEG2RAD(deltaView)) * fOldForward + sin(DEG2RAD(deltaView + 90.f)) * fOldSidemove;
}

void CFixMove::Start(CInput::CUserCmd* pCmd) {
	old = pCmd->viewangles;
	oldFowardMove = pCmd->forwardmove;
	oldSideMove = pCmd->sidemove;
}

void CFixMove::Stop(CInput::CUserCmd* pCmd) {
	CorrectMovement(old, pCmd, oldFowardMove, oldSideMove);
}

QAngle CFixMove::GetOldAngle() {
	return old;
}


bool __stdcall CreateMoveClient_Hooked(/*void* self, int edx,*/ float frametime, CInput::CUserCmd* pCmd)
{
	if (!pCmd->command_number)
		return true;

	int sequence_number = 0;
	CInput::CVerifiedUserCmd *verifiedCommands = *(CInput::CVerifiedUserCmd **)(reinterpret_cast< uint32_t >(Interfaces::pInput) + 0xF0);
	CInput::CVerifiedUserCmd *verified = &verifiedCommands[sequence_number % 150];
	CInput::CUserCmd *cmd = Interfaces::pInput->GetUserCmd2(0, sequence_number);
	static CFixMove *FixMoveManager = new CFixMove();


	if (Interfaces::Engine->IsConnected() && Interfaces::Engine->IsInGame())
	{
		PVOID pebp;
		__asm mov pebp, ebp;
		bool* pbSendPacket = (bool*)(*(DWORD*)pebp - 0x1C);
		bool& bSendPacket = *pbSendPacket;

		IClientEntity *pEnt;
		IClientEntity *pLocal = Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());

		if (!pLocal)
			return;

		// Backup for safety
		Vector origView = pCmd->viewangles;
		Vector viewforward, viewright, viewup, aimforward, aimright, aimup;
		Vector qAimAngles;
		qAimAngles.Init(0.0f, pCmd->viewangles.y, 0.0f);
		AngleVectors(qAimAngles, &viewforward, &viewright, &viewup);


		if (Interfaces::Engine->IsConnected() && Interfaces::Engine->IsInGame() && pLocal && pLocal->IsAlive()) {
			Hacks::MoveHacks(pCmd, bSendPacket);
		}


		FixMoveManager->Start(pCmd);

		FixMoveManager->Stop(pCmd);

	}

	return false;
	oCreateMove(frametime, pCmd);
	verified->m_crc = pCmd->GetChecksum();
}



void __fastcall PaintTraverse_Hooked(PVOID pPanels, int edx, unsigned int vguiPanel, bool forceRepaint, bool allowForce)
{
	IClientEntity* localplayer = (IClientEntity*)Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());

	static unsigned int FocusOverlayPanel = 0;
	static bool FoundPanel = false;

	if (!FoundPanel)
	{
		PCHAR szPanelName = (PCHAR)Interfaces::Panels->GetName(vguiPanel);
		if (strstr(szPanelName, "MatSystemTopPanel"))
		{
			FocusOverlayPanel = vguiPanel;
			FoundPanel = true;
		}
	}
	if (FocusOverlayPanel == vguiPanel)
	{
		Render::Text(5, 5, Color(255, 255, 255, 255), Render::Fonts::MenuText, "HAYPIM SDK / by youngjake");
		Render::Text(5, 10, Color(255, 0, 255, 255), Render::Fonts::MenuText, "HAYPIM.NET");
	}
	oPaintTraverse(pPanels, vguiPanel, forceRepaint, allowForce);
}


void  __stdcall Hooked_FrameStageNotify(ClientFrameStage_t curStage)
{
	IClientEntity *pLocal = Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());

	if (curStage == FRAME_NET_UPDATE_POSTDATAUPDATE_START)
	{

	}

	if (Interfaces::Engine->IsConnected() && Interfaces::Engine->IsInGame() && curStage == FRAME_RENDER_START) {

	}
	oFrameStageNotify(curStage);
}


