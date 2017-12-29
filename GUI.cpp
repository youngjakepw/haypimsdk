#include "GUI.h"
#include "OptionsManager.h"
#include "SDK.h"
#include <ios>
#include <iosfwd>


CGUI::CGUI()
{

}

bool CGUI::GetKeyPress(unsigned int key)
{
	if (keys[key] == true && oldKeys[key] == false)
		return true;
	else
		return false;
}

bool CGUI::GetKeyState(unsigned int key)
{
	return keys[key];
}

bool CGUI::IsMouseInRegion(int y, int x2, int y2, int x)
{
	if (Mouse.x > x && Mouse.y > y && Mouse.x < x2 && Mouse.y < y2)
		return true;
	else
		return false;
}

bool CGUI::IsMouseInRegion(RECT region)
{
	return IsMouseInRegion(region.top, region.left + region.right, region.top + region.bottom, region.left);
}

POINT CGUI::GetMouse()
{
	return Mouse;
}

CGUI GUI;


namespace DirectX {
	EndScene_t g_fnOriginalEndScene;
	Reset_t g_fnOriginalReset;

	WNDPROC g_pOldWindowProc;
	HWND g_hWindow;

	bool g_IsInitialized = false;

	HRESULT __stdcall Hooked_EndScene(IDirect3DDevice9* pDevice) {
		if (!g_IsInitialized) {
			ImGui_ImplDX9_Init(g_hWindow, pDevice);

			ImGuiStyle& style = ImGui::GetStyle();

			ImColor mainColor = ImColor(54, 54, 54, 255);
			ImColor bodyColor = ImColor(24, 24, 24, 240);
			ImColor fontColor = ImColor(255, 255, 255, 255);

			ImVec4 mainColorHovered = ImVec4(mainColor.Value.x + 0.1f, mainColor.Value.y + 0.1f, mainColor.Value.z + 0.1f, mainColor.Value.w);
			ImVec4 mainColorActive = ImVec4(mainColor.Value.x + 0.2f, mainColor.Value.y + 0.2f, mainColor.Value.z + 0.2f, mainColor.Value.w);
			ImVec4 menubarColor = ImVec4(bodyColor.Value.x, bodyColor.Value.y, bodyColor.Value.z, bodyColor.Value.w - 0.8f);
			ImVec4 frameBgColor = ImVec4(bodyColor.Value.x, bodyColor.Value.y, bodyColor.Value.z, bodyColor.Value.w + .1f);
			ImVec4 tooltipBgColor = ImVec4(bodyColor.Value.x, bodyColor.Value.y, bodyColor.Value.z, bodyColor.Value.w + .05f);

			// Pandax - стандартные цвета

			style.Colors[ImGuiCol_Text] = ImVec4(0.86f, 0.93f, 0.89f, 0.78f);
			style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.92f, 0.18f, 0.29f, 0.78f);
			style.Colors[ImGuiCol_WindowBg] = ImVec4(0.13f, 0.14f, 0.17f, 1.00f);
			style.Colors[ImGuiCol_ChildWindowBg] = ImVec4(0.20f, 0.22f, 0.27f, 0.58f);
			style.Colors[ImGuiCol_PopupBg] = ImVec4(0.20f, 0.22f, 0.27f, 1.00f);
			style.Colors[ImGuiCol_Border] = ImVec4(0.00f, 0.00f, 0.00f, 0.35f);
			style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
			style.Colors[ImGuiCol_FrameBg] = ImVec4(0.20f, 0.22f, 0.27f, 1.00f);
			style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.92f, 0.18f, 0.29f, 0.78f);
			style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.92f, 0.18f, 0.29f, 1.00f);
			style.Colors[ImGuiCol_TitleBg] = ImVec4(0.20f, 0.22f, 0.27f, 1.00f);
			style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.20f, 0.22f, 0.27f, 0.75f);
			style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.92f, 0.18f, 0.29f, 1.00f);
			style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.20f, 0.22f, 0.27f, 0.47f);
			style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.20f, 0.22f, 0.27f, 1.00f);
			style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.09f, 0.15f, 0.16f, 1.00f);
			style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.92f, 0.18f, 0.29f, 0.78f);
			style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.92f, 0.18f, 0.29f, 1.00f);
			style.Colors[ImGuiCol_ComboBg] = ImVec4(0.20f, 0.22f, 0.27f, 1.00f);
			style.Colors[ImGuiCol_CheckMark] = ImVec4(0.71f, 0.18f, 0.29f, 1.00f);
			style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.92f, 0.18f, 0.29f, 0.37f);
			style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.92f, 0.18f, 0.29f, 1.00f);
			style.Colors[ImGuiCol_Button] = ImVec4(0.92f, 0.18f, 0.29f, 0.75f);
			style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.92f, 0.18f, 0.29f, 0.86f);
			style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.92f, 0.18f, 0.29f, 1.00f);
			style.Colors[ImGuiCol_Header] = ImVec4(0.92f, 0.18f, 0.29f, 0.76f);
			style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.92f, 0.18f, 0.29f, 0.86f);
			style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.92f, 0.18f, 0.29f, 1.00f);
			style.Colors[ImGuiCol_Column] = ImVec4(0.15f, 0.00f, 0.00f, 0.35f);
			style.Colors[ImGuiCol_ColumnHovered] = ImVec4(0.92f, 0.18f, 0.29f, 0.59f);
			style.Colors[ImGuiCol_ColumnActive] = ImVec4(0.92f, 0.18f, 0.29f, 1.00f);
			style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.92f, 0.18f, 0.29f, 0.63f);
			style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.92f, 0.18f, 0.29f, 0.78f);
			style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.92f, 0.18f, 0.29f, 1.00f);
			style.Colors[ImGuiCol_CloseButton] = ImVec4(1.00f, 1.00f, 1.00f, 0.51f);
			style.Colors[ImGuiCol_CloseButtonHovered] = ImVec4(1.00f, 1.00f, 1.00f, 0.67f);
			style.Colors[ImGuiCol_CloseButtonActive] = ImVec4(1.00f, 1.00f, 1.00f, 0.78f);
			style.Colors[ImGuiCol_PlotLines] = ImVec4(0.86f, 0.93f, 0.89f, 0.63f);
			style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.92f, 0.18f, 0.29f, 1.00f);
			style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.86f, 0.93f, 0.89f, 0.63f);
			style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.92f, 0.18f, 0.29f, 1.00f);
			style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.92f, 0.18f, 0.29f, 0.43f);
			style.Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(0.20f, 0.22f, 0.27f, 0.73f);

			style.Alpha = 1.0;
			style.ChildWindowRounding = 3;
			style.WindowRounding = 3;
			style.GrabRounding = 1;
			style.GrabMinSize = 20;
			style.FrameRounding = 0;
			style.WindowTitleAlign = ImVec2(0.5f, 0.5f);
			style.ItemInnerSpacing = ImVec2(8, 6);
			style.ItemSpacing = ImVec2(5, 8);




			g_IsInitialized = true;
		}
		else {
			ImGui::GetIO().MouseDrawCursor = OptionsManager.bShowMenu;

			//Тут что то связано с ImGui, так что лучше не трогайте это
			ImGui_ImplDX9_NewFrame();


			ImGui::SetNextWindowPos(ImVec2(100, 200), ImGuiSetCond_FirstUseEver);








			if (OptionsManager.bShowMenu) {
				//Основной код меню




				ImGui::Begin(XorStr("HAYPIM SDK (Pandax) / Small base by youngjake.pw"), &OptionsManager.bShowMenu, ImVec2(750, 500), OptionsManager.s_MenuTransparency, ImGuiWindowFlags_NoCollapse);
				{
					int tabWidth = ImGui::GetWindowWidth() / 5 - 10;
					if (ImGui::Button(XorStr("Rage"), ImVec2(tabWidth, 75))) {
						setlocale(LC_ALL, "Russian");
						OptionsManager.g_tRagebot = true;
						OptionsManager.g_tLegitbot = false;
						OptionsManager.g_tVisuals = false;
						OptionsManager.g_tMisc = false;
						OptionsManager.g_tSettings = false;
					}
					ImGui::SameLine();
					if (ImGui::Button(XorStr("Legit"), ImVec2(tabWidth, 75))) {
						OptionsManager.g_tRagebot = false;
						OptionsManager.g_tLegitbot = true;
						OptionsManager.g_tVisuals = false;
						OptionsManager.g_tMisc = false;
						OptionsManager.g_tSettings = false;
					}
					ImGui::SameLine();
					if (ImGui::Button(XorStr("Visuals"), ImVec2(tabWidth, 75))) {
						OptionsManager.g_tRagebot = false;
						OptionsManager.g_tLegitbot = false;
						OptionsManager.g_tVisuals = true;
						OptionsManager.g_tMisc = false;
						OptionsManager.g_tSettings = false;

					}
					ImGui::SameLine();
					if (ImGui::Button(XorStr("Misc"), ImVec2(tabWidth, 75))) {
						OptionsManager.g_tRagebot = false;
						OptionsManager.g_tLegitbot = false;
						OptionsManager.g_tVisuals = false;
						OptionsManager.g_tMisc = true;
						OptionsManager.g_tSettings = false;

					}
					ImGui::SameLine();
					if (ImGui::Button(XorStr("Settings"), ImVec2(tabWidth, 75))) {
						OptionsManager.g_tRagebot = false;
						OptionsManager.g_tLegitbot = false;
						OptionsManager.g_tVisuals = false;
						OptionsManager.g_tMisc = false;
						OptionsManager.g_tSettings = true;

					}

					ImGui::Separator();

					// Tabs
					if (OptionsManager.g_tRagebot) {
						ImGui::Text(XorStr("Rage:"));
						ImGui::Separator();


					}

					if (OptionsManager.g_tLegitbot) {
						ImGui::Text(XorStr("Legit:"));
						ImGui::Separator();

					}

					if (OptionsManager.g_tVisuals) {
						ImGui::Text(XorStr("Visuals:"));
						ImGui::Separator();

					}

					if (OptionsManager.g_tMisc) {
						ImGui::Text(XorStr("Misc:"));
						ImGui::Separator();

					}
					if (OptionsManager.g_tSettings) {
						ImGui::Text(XorStr("Settings:"));
						ImGui::Separator();

						if (ImGui::CollapsingHeader(XorStr("Menu"))) {
							ImGui::SliderFloat(XorStr("Menu Transparency"), &OptionsManager.s_MenuTransparency, .1, 1, "%0.1f");
						}
					}
				}

				ImGui::End();

				/*
				ImGui::SetNextWindowPos(ImVec2(550, 200), ImGuiSetCond_FirstUseEver);
				//Begin Target window code
				ImGui::Begin(XorStr("Narcissisti.cc v72 ALPHA - Target Selector"), &OptionsManager.bShowMenu, ImVec2(400, 500), 1.f, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
				{
				for (int i = 1; i < SSDK::I::Engine()->GetMaxClients(); ++i) {
				CBaseEntity *pClient = (CBaseEntity*)SSDK::I::EntityList()->GetClientEntity(i);
				if (pClient) {
				if (*pClient->GetTeamNum() != *CBaseEntity::GetLocalPlayer()->GetTeamNum()) {
				SSDK::PlayerInfo entInfo;
				SSDK::I::Engine()->GetPlayerInfo(i, &entInfo);
				if (ImGui::CollapsingHeader(entInfo.szName)) {
				ImGui::Checkbox(XorStr("Use custom settings"), &OptionsManager.aCacheAngle[i].bCustomSetts);
				ImGui::Checkbox(XorStr("Resolve angle"), &OptionsManager.aCacheAngle[i].bCorrect);
				static const char *szResolveType[] = { XorStr("Simple"), XorStr("Delta calc"), XorStr("Delta calc 2"), XorStr("Anti-Anti-Resolver") };
				ImGui::Combo(XorStr("Resolver type"), &OptionsManager.aCacheAngle[i].iResolverType, szResolveType, ARRAYSIZE(szResolveType));
				ImGui::Checkbox(XorStr("Awall Prioritize visible parts"), &OptionsManager.aCacheAngle[i].bPrioritizeVis);
				ImGui::Checkbox(XorStr("Prioritize selected hitbox"), &OptionsManager.aCacheAngle[i].bPrioritize);
				ImGui::SliderInt(XorStr("Select hitbox"), &OptionsManager.aCacheAngle[i].iHitbox, 0, 20);
				ImGui::SliderFloat(XorStr("Min dmg"), &OptionsManager.aCacheAngle[i].flMinDmg, 0, 100);
				ImGui::Checkbox(XorStr("Auto baim"), &OptionsManager.aCacheAngle[i].bAutoBaim);
				ImGui::SliderInt(XorStr("Baim after shot"), &OptionsManager.aCacheAngle[i].iAutoBaimAferShot, 0, 10);
				ImGui::SliderInt(XorStr("Select hitbox auto baim"), &OptionsManager.aCacheAngle[i].iHitboxAutoBaim, 0, 20);
				ImGui::Checkbox(XorStr("Disable interp/Lag compensate"), &OptionsManager.aCacheAngle[i].bInterpLagComp);
				}
				}
				}
				}
				}
				ImGui::End();
				*/
			}
			ImGui::Render();
		}

		return g_fnOriginalEndScene(pDevice);
	}

	HRESULT __stdcall Hooked_Reset(IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters) {
		if (!g_IsInitialized) return g_fnOriginalReset(pDevice, pPresentationParameters);
		ImGui_ImplDX9_InvalidateDeviceObjects();

		auto hr = g_fnOriginalReset(pDevice, pPresentationParameters);
		ImGui_ImplDX9_CreateDeviceObjects();
		return hr;
	}

	LRESULT __stdcall Hooked_WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
		switch (uMsg) {
		case WM_LBUTTONDOWN:
			OptionsManager.vecPressedKeys[VK_LBUTTON] = true;
			break;
		case WM_LBUTTONUP:
			OptionsManager.vecPressedKeys[VK_LBUTTON] = false;
			break;
		case WM_RBUTTONDOWN:
			OptionsManager.vecPressedKeys[VK_RBUTTON] = true;
			break;
		case WM_RBUTTONUP:
			OptionsManager.vecPressedKeys[VK_RBUTTON] = false;
			break;
		case WM_KEYDOWN:
			OptionsManager.vecPressedKeys[wParam] = true;
			break;
		case WM_KEYUP:
			OptionsManager.vecPressedKeys[wParam] = false;
			break;
		default: break;
		}

		static ConVar *conMouseEnable = Interfaces::CVar->FindVar(XorStr("cl_mouseenable"));
		static bool isDown = false;
		static bool isClicked = false;
		if (OptionsManager.vecPressedKeys[VK_INSERT]) {
			isClicked = false;
			isDown = true;
		}
		else if (!OptionsManager.vecPressedKeys[VK_INSERT] && isDown) {
			isClicked = true;
			isDown = false;
		}
		else {
			isClicked = false;
			isDown = false;
		}

		if (isClicked) {
			if (OptionsManager.bShowMenu) {
				//CONFIG SAVER - LOAD
				//std::ofstream ofs("hvhacademy.save", std::ios::binary);
				//ofs.write((char*)&OptionsManager, sizeof(OptionsManager));



			}
			OptionsManager.bShowMenu = !OptionsManager.bShowMenu;
			conMouseEnable->SetValue(!OptionsManager.bShowMenu);
		}


		if (g_IsInitialized && OptionsManager.bShowMenu && ImGui_ImplDX9_WndProcHandler(hWnd, uMsg, wParam, lParam))
			return true;

		return CallWindowProc(g_pOldWindowProc, hWnd, uMsg, wParam, lParam);
	}
}