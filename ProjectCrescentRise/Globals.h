#pragma once

#define ONLINE_SESSION false 
#define WIRE_FRAME false 
#define AUTO_GUI false 
#define NOISE_RANDOMISATION true 

#define WINDOW_SIZE_X 1920 
#define WINDOW_SIZE_Y 1080 

#define QUALITY 1

#if defined(_DEBUG) || defined(DEBUG)
	#define DEBUG_MODE 1
	#define LOW_RES true 
	#define FULLSCREEN_MODE false
#else
	#define DEBUG_MODE 0
	#define LOW_RES false 
	#define FULLSCREEN_MODE true
#endif