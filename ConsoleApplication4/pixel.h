#pragma once
#include <windows.h>
#include "speech.h"
#include "inputSimulator.h"

class pixel {
public:

	static bool is_yellow(int x, int y, HWND eveWindow) {
		// Make sure the window has focus
		SetForegroundWindow(eveWindow);
		RECT windowPosition;

		// Our Selection Rect is our window position
		GetWindowRect(eveWindow, &windowPosition);

		int finalX = (windowPosition.left + x);
		int finalY = (windowPosition.top + y);

		bool result;

		COLORREF color;
		HDC hDC;

		// Get the device context for the screen
		hDC = GetDC(NULL);
		if (hDC == NULL)
			return false;

		// Retrieve the color at that position
		color = GetPixel(hDC, finalX, finalY);
		if (color == CLR_INVALID)
			return false;

		// Release the device context again
		ReleaseDC(GetDesktopWindow(), hDC);

		//	Is the pixel kinda yellowish?
		inputSim::move_mouse(finalX, finalY, eveWindow);
		speak(std::to_string(finalX));
		speak(std::to_string(finalY));


		if ((GetRValue(color) > 150) && (GetGValue(color) > 150) && (GetBValue(color) < 20)) {
			result = true;
		}
		else {
			result = false;
		}
		Sleep(10);
		return result;
	}

	static bool is_red(int&& x, int&& y, const HWND eveWindow) {
		// Make sure the window has focus
		SetForegroundWindow(eveWindow);
		RECT windowPosition;

		// Our Selection Rect is our window position
		GetWindowRect(eveWindow, &windowPosition);

		int finalX = (windowPosition.left + x);
		int finalY = (windowPosition.top + y);

		bool result;


		COLORREF color;
		HDC hDC;

		// Get the device context for the screen
		hDC = GetDC(NULL);
		if (hDC == NULL)
			return false;

		// Retrieve the color at that position
		color = GetPixel(hDC, finalX, finalY);
		if (color == CLR_INVALID)
			return 1;

		// Release the device context again
		ReleaseDC(GetDesktopWindow(), hDC);

		//	Is the pixel kinda redish?
		if ((GetRValue(color) > 150) && (GetGValue(color) < 30) && (GetBValue(color) < 30)) {
			result = true;
		}
		else {
			result = false;
		}
		Sleep(10);
		return result;
	}

	static bool is_grey(int&& x, int&& y, const HWND eveWindow) {

		// Make sure the window has focus
		SetForegroundWindow(eveWindow);

		// Our Selection Rect is our window position
		RECT windowPosition;
		GetWindowRect(eveWindow, &windowPosition);

		// Add the window starting pos to our cursor offset
		int finalX = (windowPosition.left + x);
		int finalY = (windowPosition.top + y);

		COLORREF color;
		HDC hDC;

		// Get the device context for the screen
		hDC = GetDC(NULL);
		if (hDC == NULL)
			return false;

		// Retrieve the color at that position
		color = GetPixel(hDC, finalX, finalY);
		if (color == CLR_INVALID)
			return 1;

		// Release the device context again
		ReleaseDC(GetDesktopWindow(), hDC);

		//	255 shades of grey. Sounds like a good book title
		if ((GetRValue(color) == GetGValue(color)) && (GetGValue(color) == GetBValue(color))) {
			return true;
		}
		return false;
	}
};
