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
//#include <x3daudio.h>
//#include <xapo.h>
//#include <xapobase.h>
//#include <xapofx.h>
#include <xaudio2.h>
#include <xaudio2fx.h>
//#include <hrtfapoapi.h>
//#include <mmdeviceapi.h>
