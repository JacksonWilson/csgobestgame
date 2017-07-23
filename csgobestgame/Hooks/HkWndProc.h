#pragma once

bool pressedKeys[256] = {};
bool g_bWasInitialized = false;

WNDPROC g_pOldWindowProc = nullptr;

extern LRESULT ImGui_ImplDX9_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT __stdcall HkWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	//Captures the keys states
	switch (uMsg) {
	case WM_LBUTTONDOWN:
		pressedKeys[VK_LBUTTON] = true;
		break;
	case WM_LBUTTONUP:
		pressedKeys[VK_LBUTTON] = false;
		break;
	case WM_RBUTTONDOWN:
		pressedKeys[VK_RBUTTON] = true;
		break;
	case WM_RBUTTONUP:
		pressedKeys[VK_RBUTTON] = false;
		break;
	case WM_KEYDOWN:
		pressedKeys[wParam] = true;
		break;
	case WM_KEYUP:
		pressedKeys[wParam] = false;
		break;
	default: break;
	}

	//Insert toggles the menu
	{
		//Maybe there is a better way to do this? Not sure
		//We want to toggle when the key is pressed (i.e when it receives a down and then up signal)
		static bool isDown = false;
		static bool isClicked = false;
		if (pressedKeys[VK_INSERT]) {
			isClicked = false;
			isDown = true;
		}
		else if (!pressedKeys[VK_INSERT] && isDown) {
			isClicked = true;
			isDown = false;
		}
		else {
			isClicked = false;
			isDown = false;
		}

		if (isClicked) {
			Options::g_bMainWindowOpen = !Options::g_bMainWindowOpen;

			//Use cl_mouseenable convar to disable the mouse when the window is open 
			static auto cl_mouseenable = se::Interfaces::CVar()->FindVar(XorStr("cl_mouseenable"));
			cl_mouseenable->SetValue(!Options::g_bMainWindowOpen);
		}
	}

	//Processes the user input using ImGui_ImplDX9_WndProcHandler
	if (g_bWasInitialized && Options::g_bMainWindowOpen && ImGui_ImplDX9_WndProcHandler(hWnd, uMsg, wParam, lParam))
		return true; //Input was consumed, return

					 //Input was not consumed by the GUI, call original WindowProc to pass the input to the game
	return CallWindowProc(g_pOldWindowProc, hWnd, uMsg, wParam, lParam);
}