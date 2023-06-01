#pragma once
// Precompiled header to help speed up compilation times

#include <algorithm>
#include <array>
#include <cassert>
#include <cstdint>
#include <chrono>
#include <codecvt>
#include <filesystem>
#include <fstream>
#include <locale>
#include <map>
#include <math.h>
#include <memory>
#include <optional>
#include <set>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>
#include <windows.h>

// OpenGL
#include <gl\gl.h>                                // Header File For The OpenGL32 Library
#include <gl\glu.h>                               // Header File For The GLu32 Library
#include "..\dependencies\OpenGL\glcorearb.h"
#include "..\dependencies\OpenGL\glext.h"
#include "..\dependencies\OpenGL\khrplatform.h"
#include "..\dependencies\OpenGL\wgl.h"
#include "..\dependencies\OpenGL\wglext.h"

// GLM
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

// XAudio2
#ifdef _XBOX //Big-Endian
#define fourccRIFF 'RIFF'
#define fourccDATA 'data'
#define fourccFMT 'fmt '
#define fourccWAVE 'WAVE'
#define fourccXWMA 'XWMA'
#define fourccDPDS 'dpds'
#endif
#ifndef _XBOX //Little-Endian
#define fourccRIFF 'FFIR'
#define fourccDATA 'atad'
#define fourccFMT ' tmf'
#define fourccWAVE 'EVAW'
#define fourccXWMA 'AMWX'
#define fourccDPDS 'sdpd'
#endif
//#include <x3daudio.h>
//#include <xapo.h>
//#include <xapobase.h>
//#include <xapofx.h>
#include <xaudio2.h>
#include <xaudio2fx.h>
//#include <hrtfapoapi.h>
//#include <mmdeviceapi.h>

// DirectX for direct input
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

//#define STB_IMAGE_IMPLEMENTATION
#include "../dependencies/stb_image.h"

//#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../dependencies/stb_image_write.h"