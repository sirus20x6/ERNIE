#pragma once
#include "inputSimulator.h"

//	These are the application specific wrappers to the inputsimulator functions
//	For future apps these would be replaced with calls to inputsimulator functions
//	relative to that application

// Moves mouse to the warp icon and clicks it
inline void click_warp(HWND appWindow) {
	inputSim::move_mouse(1614, 129, appWindow);
	inputSim::click_mouse(appWindow);
}

// Moves mouse to the default location and clicks it
inline void click_default(HWND appWindow) {
	inputSim::move_mouse(1451, 129, appWindow);
	inputSim::click_mouse(appWindow);
}

// Moves mouse to the pvp hostiles icon and clicks it
inline void click_pvp_hostiles(HWND appWindow) {
	inputSim::move_mouse(1566, 129, appWindow);
	inputSim::click_mouse(appWindow);
}

// Moves mouse to the drone icont and clicks it
inline void click_drone(HWND appWindow) {
	inputSim::move_mouse(1512, 129, appWindow);
	inputSim::click_mouse(appWindow);
}

// Moves mouse to the pvp logi icon and clicks it
inline void click_pvp_logi(HWND appWindow) {
	inputSim::move_mouse(1672, 129, appWindow);
	inputSim::click_mouse(appWindow);
}

// Moves mouse to location and right clicks it
inline void right_click_location(int list, int context_menu_selection, HWND appWindow) {
	int y = 160 + list * 19;
	y += 8;
	inputSim::move_mouse(1691, y, appWindow);
	inputSim::right_click_mouse(appWindow);
	y += 7 + context_menu_selection * 16;
	inputSim::move_mouse(1741, y, appWindow);
	inputSim::click_mouse(appWindow);
}

// Preform a full stop captain
inline void full_stop(HWND appWindow) {
	inputSim::keys_down(VK_CONTROL, VK_SPACE, appWindow);
}

// Warps to location in list
inline void warp_location(int& list, HWND appWindow) {
	right_click_location(list, 0, appWindow);
}

// Goes to location in list
inline void goto_object(int& list, HWND appWindow) {
	right_click_location(list, 0, appWindow);
}
