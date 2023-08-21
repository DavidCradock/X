#pragma once
// Precompiled header to help speed up compilation times

#include <algorithm>
#include <array>
#include <cassert>
#include <cstdint>
#include <chrono>
#include <codecvt>
#include <filesystem>
#include <format>		// For stuff like std::string strFPS = "FPS: "; strFPS += std::format("{:.2f}", timer.getFPSAveraged());
#include <fstream>
#include <iostream>
#include <locale>
#include <map>
#include <math.h>
#include <memory>
#include <optional>
#include <queue>
#include <set>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>
#include <windows.h>
#include <psapi.h>		// For GetProcessMemoryInfo() in utilities.h (Needs to be included after windows.h

// Modern  multithreading basics
#include <mutex>
#include <thread>
/* Example threading code
#include <mutex>
#include <thread>
unsigned int iNumberOfThreadsOnCPU = std::thread::hardware_concurrency();
std::mutex mtx;
void print_hello() {
  std::unique_lock<std::mutex> lock(mtx);
  std::cout << "Hello, world!" << std::endl;
  lock.unlock();
}
int main() {
  std::thread t1(print_hello);
  std::thread t2(print_hello);
  t1.join();
  t2.join();
  return 0;
}
*/

// OpenGL
#include <gl\gl.h>                                // Header File For The OpenGL32 Library
#include <gl\glu.h>                               // Header File For The GLu32 Library
#include "OpenGL\glcorearb.h"
#include "OpenGL\glext.h"
#include "OpenGL\khrplatform.h"
#include "OpenGL\wgl.h"
#include "OpenGL\wglext.h"

// GLM
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>		// glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtx/norm.hpp>

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
#include "stb_image.h"

//#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"