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
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>
#include <windows.h>

// Vulkan
#define VK_USE_PLATFORM_WIN32_KHR
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_win32.h>

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
