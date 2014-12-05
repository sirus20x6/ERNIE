#include <string>
#include <iostream>
#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>
#include <boost/regex.hpp>
#include <boost/filesystem.hpp>
#include <time.h>
#include <stdio.h>
#include <windows.h>
#include <conio.h>
#include <map>
#include <iterator>
#include <algorithm>
#include <sstream>
#include <ppl.h>
#include "pixel.h"
#include "inputSimulator.h"
#include "eveInputWrapper.h"
#include "OCR.h"
#include "speech.h"


// Global Variables
/**********************************************************/
HWND eveWindow; // Our global handle to the eve window
std::string home_base;
std::string locations[48];

tesseract::TessBaseAPI tess;
/**********************************************************/

// Function Prototypes
bool eve_init();
void mine();
bool is_string_match(std::string, std::string);



// Our Entry point to the application
int main(int argc, char** argv) {
	speak("Bot loaded");
	// Get the last position of '/'
	std::string aux(argv[0]);

	// get '/' or '\\' depending on unix/mac or windows.
#if defined(_WIN32) || defined(WIN32)
	int pos = aux.rfind('\\');
#else
	int pos = aux.rfind('/');
#endif

	// Get the path and the name
	std::string path = aux.substr(0, pos + 1);
	std::string name = aux.substr(pos + 1);

	tess.Init(path.c_str(), "eng", tesseract::OEM_DEFAULT);

	


	//	PageSegMode is the layout of the page (as in pages in a physical book)
	//	Here we say auto because we arent scanning books or notes or anything like that
	tess.SetPageSegMode(tesseract::PSM_AUTO);

	// Wait for things to settle
	Sleep(5000);

	//	Initialize
	if (eve_init()) {
		// Let's mine some virtual asteroids for fake money
		mine();
	}
	return 0;
}


// Initialize our application
bool eve_init() {
	bool ready = true;

	// We randomize some input to make sure they dont detect we are using a bot

	POINT ptMouse; // This is our handle to our Mouse
	RECT windowPosition; // Used to offset mouse x,y
	eveWindow = FindWindow(NULL, TEXT("EVE - ERNIE20014 ERNIE")); //	Search for the Window title name

	if (eveWindow == NULL) {
		std::cout << "Could not find EVE window, closing. Make sure EVE is open before running! \n";
		system("pause");
		ready = false;
	}
	else {
		//	Hey We're in buisness!
		speak("eve window found");
	}

	//	Make sure our window has focus
	SetForegroundWindow(eveWindow);

	//Wait to settle
	Sleep(2000);

	// GET window Rect
	GetWindowRect(eveWindow, &windowPosition);
	GetCursorPos(&ptMouse);

	return ready;
}



// Is the ship docked?
bool is_docked() {

	// Check the screen for the yellow docked symbol
	if (pixel::is_yellow(1836, 175, eveWindow)){
		//&& pixel::is_yellow(1852, 175, eveWindow) && pixel::is_yellow(1870, 175, eveWindow)) {
		speak("ship is docked");
		return true;
	}
	speak("ship is not docked");
	return false;
}


// Undock the ship
void undock() {
	speak("undocking");
	inputSim::move_mouse(1853, 184, eveWindow);
	inputSim::click_mouse(eveWindow);
}


// This is a stub for future Implimentation
std::string where_am_i() {
	//	Here we would pull a std::string get_text_at(int x1, int y1, int x2, int y2, const HWND appWindow, tesseract::TessBaseAPI tess)
	//	At the location where it shows our current area. Not used yet.
	std::string location = "";
	return location;
}

// Stub for future implimentation
void empty_cargo_hold() {
}


//	Check to see if our health bar is full
bool is_full_health() {

	// If these pixels are red our heath is full
	if (pixel::is_red(1028, 981, eveWindow) && pixel::is_red(1021, 981, eveWindow) && pixel::is_red(1010, 981, eveWindow)) {
		speak("at full health");
		return true;
	}
	speak("health is not full");
	return false;
}


void warp_to_asteroids() {
	// Hint to the compiler that we want this split
	// Up between many cores
#pragma loop(hint_parallel(6))
	for (int x = 0; x < 40; x++) {

		size_t found = locations[x].find("asteroid");

		if (found != std::string::npos) {
			speak("warping to " + locations[x]);
			warp_location(x, eveWindow);
			break;
		}

	}
}


void goto_asteroid() {
	speak("finding most valuable asteroid");

	// In order from most valuable to least.
	std::string asteroids[48] = {
		"vitreous mercoxit",
		"magma mercoxit",
		"mercoxit",
		"glazed hedbergite",
		"radiant hemorphite",
		"vitric hedbergite",
		"vivid hemorphite",
		"pristine jaspet",
		"hedbergite",
		"hemorphite",
		"pure jaspet",
		"jaspet",
		"crystalline crokite",
		"massive scordite",
		"sharp crokite",
		"viscous pyroxeres",
		"condensed scordite",
		"prime arkonor",
		"obsidian ochre",
		"monoclinic bistot",
		"crokite",
		"solid pyroxeres",
		"scordite",
		"crimson arkonor",
		"onyx ochre",
		"triclinic bistot",
		"fiery kernite",
		"pyroxeres",
		"arkonor",
		"dark ochre",
		"bistot",
		"rich plagioclase",
		"luminous kernite",
		"azure plagioclase",
		"kernite",
		"plagioclase",
		"dense veldspar",
		"concentrated veldspar",
		"golden omber",
		"veldspar",
		"silvery omber",
		"omber",
		"prismatic gneiss",
		"iridescent gneiss",
		"gneiss",
		"gleaming spodumain",
		"bright spodumain",
		"spodumain"};
	int most_priority = 48;
	int most_priority_selection = 99;


	//	This takes a for loop as a Lambda and chops it up into bits
	//	Splitting the work up for many cpu cores
	//	OCR is very computationally intensive. It's also best suited for
	//	Vector processors not Out of order super scalers. Unfortunately
	//	Our graphics card is going to have it's hands full and then some
	//	Drawing the beuatiful EVE Universe. Thise forces us to spread the
	//	Work out as best we can in as many places as possible

	concurrency::parallel_for
		(size_t(0), static_cast<size_t>(48), [&](size_t x) {

			 for (int y = 47; y >= 0; y--) {

				 // If we have a match in our locations
				 if (is_string_match(locations[x], asteroids[y])) {

					 // If the priority is lower than the lowest we've seen (lower values = more profit)
					 if (y < most_priority) {

						 // Set most_priority to the index of the most valuable thing we've see (the lowest value)
						 most_priority = y;

						 // Set most_priority_selection to the index of location which we've seen the lowest value for most_priority
						 most_priority_selection = x;
					 }
				 }
			 }
		 });

	// Lets approach an asteroid
	speak("approching " + asteroids[most_priority]);
	goto_object(most_priority_selection, eveWindow);
}


// Are we being attacked? Check our health
bool is_being_attacked() {
	bool attacked;
	if (is_full_health()) {
		attacked = false;
	}
	else {
		attacked = true;
	}
	return attacked;
}


// Save the current location
void save_location() {
	speak("saving location. ");
	home_base = get_text_at(78, 100, 200, 15, eveWindow, tess );
	speak("home base is " + home_base);
}


// Does this string match?
bool is_string_match(std::string s1, std::string s2) {
	bool match = false;
	size_t found;

	// Lowercase String Comp
	transform(s1.begin(), s1.end(), s1.begin(), tolower);
	transform(s2.begin(), s2.end(), s2.begin(), tolower);

	// Trim Whitespace
	boost::algorithm::trim(s1);
	boost::algorithm::trim(s2);
	found = s1.find(s2);

	if (found != std::string::npos) {
		match = true;
	}
	return match;
}

// Are we in warp? If so send back true
bool is_in_warp() {
	bool warping = false;

	//	Check a part of the screen
	std::string warping_str = get_text_at(860, 812, 200, 78, eveWindow, tess);

	//	For text that indicates we are at warp
	//	Check for each word because the scene behind
	//	the text might make some words unreadable
	if (is_string_match(warping_str, "warp") ||
		is_string_match(warping_str, "drive") ||
		is_string_match(warping_str, "active")) {
		warping = true;
	}

	return warping;
}


//	You can't warp without being aligned to a known destination
bool align_home_base() {
	bool aligned_home_base = false;
	concurrency::parallel_for
		(size_t(0), (size_t)40, [&](size_t x) {

			 //	Check all text in navigation list for string matching our home base
			 if (is_string_match(locations[x], home_base)) {
				 right_click_location(x, 2, eveWindow);
				 aligned_home_base = true;

				 // Takes some time
				 Sleep(5000); //	In the future we should grab text at a screen rect. 
				 //	because different ships turn at different speeds, also alignment time
				 // naturally varies based on how far you have to turn to align to your destination

				 // Full stop scotty
				 full_stop(eveWindow);
			 }
			 else {
				 //	If we couldnt find the home base (oh crud that would be bad)
				 aligned_home_base = false;
			 }
		 });

	return aligned_home_base;
}

void mine() {
	while (eveWindow == GetForegroundWindow()) {

		if (is_docked()) {

			// Make room for money making ore!
			empty_cargo_hold();

			// Save our docked location
			save_location();

			// Let's set out!
			undock();

			//	Give the world a chance to load in
			speak("sleeping for 10 seconds");
			Sleep(10000);

			// Begin warp sequence captain
			click_warp(eveWindow);

			// Keep the user informed of what we're doing
			speak("updating locations");

			// Update or local locations lsit
			update_locations(locations, eveWindow, tess);

			//	Announce our destination
			speak("warping to asteroids");

			// let's get us some asteroids
			// .... Engage
			warp_to_asteroids();

			while (is_in_warp()) {
				// Let's wait until we arent in warp any more
				Sleep(100);
			}
			// Make sure our window has focus
			click_default(eveWindow);

			// What's around us?
			update_locations(locations, eveWindow, tess);

			// Let's goto an asteroid
			goto_asteroid();
		}
	}
}
