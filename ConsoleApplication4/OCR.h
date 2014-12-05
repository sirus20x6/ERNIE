#pragma once
#include <stdio.h>
#include "speech.h"
#include "api/baseapi.h"
#include "vs2010/include/leptonica/allheaders.h"

// For Leptonica / Tesseract
//	Tesseract is our Optical Character Recognition Library
//	Tesseract is based on Leptonica Libraries
PIX* pix;
PIX* croppedImage;
BOX* cropWindow;



//	Due to the nature of the Directx, Direct3d, hardware accellerated rendering engine, getting
//	a direct screen grab is difficult
std::string update_screenshot(const HWND appWindow) {
	std::string file;
	// This is where the game stores screenshots. Hardcoded for now
	boost::filesystem::path p = "C:\\Users\\sirus20x6\\Documents\\EVE\\capture\\Screenshots\\";

	// Lets make sure it's just the data we want
	speak("removing old screenshots");
	boost::filesystem::remove_all(p);
	Sleep(250); // wait to ensure finished

	// Make sure that directory is there
	boost::filesystem::create_directory(p);

	//	Hit the print screen button (virtually)
	speak("print screen");
	inputSim::press_key(VK_SNAPSHOT, 1, appWindow);
	speak("took snapshot");

	// Wait for screen shot to save
	Sleep(250);
	std::ostringstream buf;
	if (exists(p)) {
		if (is_directory(p)) {
			copy(boost::filesystem::directory_iterator(p), boost::filesystem::directory_iterator(), std::ostream_iterator<boost::filesystem::directory_entry>(buf, "\n"));
			file = buf.str();
		}
	}
	file.erase(remove(file.begin(), file.end(), '\"'), file.end());
	boost::algorithm::trim(file);
	std::cout << file;
	return file;
}

// Specify x,y start, an x,y end. Get text inside that rect
std::string get_text_at(int x1, int y1, int x2, int y2, HWND eveWindow, tesseract::TessBaseAPI tess) {
	std::string file = update_screenshot(eveWindow);
	std::string temp_str;

	//load screenshot from file. todo: capture image from screen
	pix = pixRead(file.c_str());

	//	Stealing a screen shot directly from Direct3d is tricky, but in the future I believe this is the leptonica call
	//pix = pixReadMem ( const l_uint8 *data, size_t size );

	// Create a box to crop
	cropWindow = boxCreate(x1, y1, x2, y2);

	// Apply the box crop to image and save it into sub image
	croppedImage = pixClipRectangle(pix, cropWindow, NULL);

	//	Scale by a factor of 10. tesseract likes high res files
	croppedImage = pixScale(croppedImage, 10.0f, 10.0f);

	// Feed tesseract OCR an image
	tess.SetImage(croppedImage);

	// Process image with tesseract
	tess.Recognize(0);

	// Give me a string
	temp_str = tess.GetUTF8Text();
	boost::algorithm::trim(temp_str);

	// Make everything same case to make evaluations eaiser
	transform(temp_str.begin(), temp_str.end(), temp_str.begin(), tolower);

	// Speak the Text
	speak(static_cast<std::string>(temp_str));

	return temp_str;
}




void update_locations(std::string* locations, const HWND appWindow, tesseract::TessBaseAPI tess) {
	std::string file = update_screenshot(appWindow);
	// Starting at y=160 and going down 17px (box size) + 2 (box border size each time)
	int rect_y = 160;

	// Load screenshot from file. todo: capture image from screen
	pix = pixRead(file.c_str());

	//	We're going to hint to the compiler that we want this on many cores
#pragma loop(hint_parallel(6))
	for (int x = 0; x < 47; x++) {

		// Create a box to crop
		cropWindow = boxCreate(1481, (rect_y + x * 19), 220, 17);

		// Apply the box crop to image and save it into sub image
		croppedImage = pixClipRectangle(pix, cropWindow, NULL);

		//scale by a factor of 10. tesseract likes high res files because it's a tard
		croppedImage = pixScale(croppedImage, 10.0f, 10.0f);

		// Feed tesseract OCR an image
		tess.SetImage(croppedImage);

		// Process image with tesseract
		tess.Recognize(0);

		// Give me a string
		locations[x] = tess.GetUTF8Text();

		// No leading / Trailing white space
		boost::algorithm::trim(locations[x]);

		// make everything same case to make evaluations eaiser
		std::transform(locations[x].begin(), locations[x].end(), locations[x].begin(), tolower);

		// Reset Tesseract
		tess.Clear();

		// Debug text
		std::cout << locations[x] << std::endl;
	}

}
