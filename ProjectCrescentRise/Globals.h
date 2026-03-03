#pragma once

#define ONLINE_SESSION false // is the server available and connectable?
#define WIRE_FRAME false // enable wireframe for the planet
#define AUTO_GUI false // enable the editor GUI automatically on load
#define NOISE_RANDOMISATION true // randomise noise position on load, fully random planets every time

#define WINDOW_SIZE_X 1920 // size if not in full screen mode
#define WINDOW_SIZE_Y 1080 // size if not in full screen mode

#if defined(_DEBUG) || defined(DEBUG)
	#define DEBUG_MODE 1
	#define LOW_RES true // use low resolution texture (for the sky box)
	#define FULLSCREEN_MODE false
#else
	#define DEBUG_MODE 0
	#define LOW_RES false // use low resolution texture (for the sky box)
	#define FULLSCREEN_MODE true
#endif