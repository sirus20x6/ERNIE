#pragma once
#include <Windows.h>


//	This file contains generic, reusable, input simulation to a windows application winow
//	You must pass a HWND (Handle to a window)

class inputSim {

public:

	//	Class is used just to organize functions

	// Double clicks the mouse
	static void double_click_mouse(HWND appWindow) {

		// Make sure window has focus
		SetForegroundWindow(appWindow);

		// We randomize the time to avoid bot detection
		// Don't want to be too perfect (avoid bot detection with rand)
		Sleep(200 + rand() % 250);

		mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
		mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);

		// We randomize the time to avoid bot detection
		// Don't want to be too perfect (avoid bot detection with rand)
		Sleep(100 + rand() % 100 + 0);
		mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
		mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
	}

	// Right clicks the mouse
	static void right_click_mouse(HWND appWindow) {
		// Make sure window has focus
		SetForegroundWindow(appWindow);

		// We randomize the time to avoid bot detection
		// Don't want to be too perfect (avoid bot detection with rand)
		Sleep(200 + rand() % 250);
		mouse_event(MOUSEEVENTF_RIGHTDOWN, 0, 0, 0, 0);

		// We randomize the time to avoid bot detection
		// Don't want to be too perfect
		Sleep(100);
		mouse_event(MOUSEEVENTF_RIGHTUP, 0, 0, 0, 0);
	}


	// Virtually presses two keys
	static void keys_down(WORD key1, WORD key2, HWND appWindow) {

		// Make sure window has focus
		SetForegroundWindow(appWindow);
		INPUT *key_click1, *key_click2;
		key_click1 = new INPUT;
		key_click1->ki.wVk = key1;
		key_click1->type = INPUT_KEYBOARD;
		key_click1->ki.dwFlags = 0;
		key_click1->ki.time = 0;
		key_click1->ki.wScan = 0;
		key_click1->ki.dwExtraInfo = 0;
		SendInput(1, key_click1, sizeof(INPUT));
		Sleep(10);

		key_click2 = new INPUT;
		key_click2->ki.wVk = key1;
		key_click2->type = INPUT_KEYBOARD;
		key_click2->ki.dwFlags = 0;
		key_click2->ki.time = 0;
		key_click2->ki.wScan = 0;
		key_click2->ki.dwExtraInfo = 0;
		SendInput(1, key_click2, sizeof(INPUT));
		Sleep(10);

		key_click1->ki.dwFlags = KEYEVENTF_KEYUP;
		SendInput(1, key_click1, sizeof(INPUT));
		key_click2->ki.dwFlags = KEYEVENTF_KEYUP;
		SendInput(1, key_click2, sizeof(INPUT));
	}


	// Virtually presses a key
	static void press_key(WORD key, int&& x, HWND appWindow) {
		int y = 0;
		while (y <= x) {
			// Make sure window has focus
			SetForegroundWindow(appWindow);

			// Random delays to avoid bot detection
			Sleep(200 + rand() % 250);
			INPUT* key_click1;
			key_click1 = new INPUT;
			key_click1->ki.wVk = key;
			key_click1->type = INPUT_KEYBOARD;
			key_click1->ki.dwFlags = 0;
			key_click1->ki.time = 0;
			key_click1->ki.wScan = 0;
			key_click1->ki.dwExtraInfo = 0;
			SendInput(1, key_click1, sizeof(INPUT));
			Sleep(100);
			key_click1->ki.dwFlags = KEYEVENTF_KEYUP;
			SendInput(1, key_click1, sizeof(INPUT));
			y++;
		}
	}


	// Clicks the mouse
	static void click_mouse(HWND eveWindow) {
		// Make sure window has focus
		SetForegroundWindow(eveWindow);

		// Randome delays to avoid bot detection
		Sleep(200 + rand() % 250);

		mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
		Sleep(100);
		mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
	}

	// "Unclicks" the mouse
	static void ClickUp(HWND appWindow) {
		// Make sure window has focus
		SetForegroundWindow(appWindow);

		// Randome delays to avoid bot detection
		Sleep(200 + rand() % 250);

		mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
		Sleep(100);
	}


	// Moves the mouse to location x,y
	static void move_mouse(int x, int y, HWND appWindow) {

		SetForegroundWindow(appWindow);

		// Random delays to avoid bot detection
		Sleep(200 + rand() % 25);
		POINT ptMouse;

		// Get Rekt son
		RECT windowPosition;
		GetWindowRect(appWindow, &windowPosition);

		// Little bit-O-Jiggle for anti-bot detection
		int finalX = (windowPosition.left + x) + rand() % 3;
		int finalY = (windowPosition.top + y) + rand() % 3;
		GetCursorPos(&ptMouse);
		int mX = ptMouse.x;
		int mY = ptMouse.y;

		while (mX != finalX || mY != finalY) {
			GetCursorPos(&ptMouse);
			mX = ptMouse.x;
			mY = ptMouse.y;
			int dy = 0;
			int dx = 0;

			if (finalX > mX) {
				if (mX != finalX)
					dx = 1 + rand() % 5; // Little bit-O-Jiggle for anti-bot detection
			}
			else {
				if (mX != finalX)
					dx = -1 - rand() % 5; // Little bit-O-Jiggle for anti-bot detection
			}

			if (finalY > mY) {
				if (mY != finalY)
					dy = 1 + rand() % 5; // Little bit-O-Jiggle for anti-bot detection
			}
			else {
				if (mY != finalY)
					dy = -1 - rand() % 5; // Little bit-O-Jiggle for anti-bot detection
			}

			SetCursorPos(mX + (dx), mY + (dy));
			SetForegroundWindow(appWindow);
			Sleep(10);
		}
	}

	// Nonblocking getch function
	static int getch_noblock() {
		if (_kbhit())
			return _getch();
		else
			return -1;
	}
};
