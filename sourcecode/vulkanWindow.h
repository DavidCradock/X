#pragma once
#include "PCH.h"
#include "singleton.h"

namespace X
{
	class VulkanWindow : public Singleton<VulkanWindow>
	{
	public:
		VulkanWindow();

		// Creates the window and initialises Vulkan
		void initialise(std::string strWindowTitle, int iWindowWidth, int iWindowHeight, bool bVsyncEnabled = true, bool bLogLogicalDeviceExtensions = false);

		// Updates the window 
		bool update(void);

		// Frees all objects and closes the window.
		void shutdown(void);

		// Window message procedure
		LRESULT MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

		// Returns the window's handle
		HWND getWindowHandle(void);

		// Returns the application's HINSTANCE
		HINSTANCE getApplicationInstance(void);

		// Returns the Vulkan logical device
		VkDevice getLogicalDevice(void) {	return mvkLogicalDevice;	}

		// Returns the extent(dimensions) of the swap chain
		VkExtent2D getSwapchainExtent(void) {	return mvkSwapChainExtent;	}

		// Returns the render pass (Used by graphics pipelines)
		VkRenderPass getRenderpass(void) {	return mvkRenderPass;	}
	private:
		WNDCLASS mWindowClass;			// Window class used to create the window
		HINSTANCE mhInstance;			// Application instance handle
		HWND mhWindowHandle;			// Window handle
		std::string mstrWindowTitle;	// Window's title text
		int miWindowWidth;				// Width of window
		int miWindowHeight;				// Height of window

		VkInstance mvkInstance;								// Main Vulkan instance
		VkDebugUtilsMessengerEXT mvkDebugMessenger;			// Debug layers messenger
		VkPhysicalDevice mvkPhysicalDevice;					// Physical Vulkan device found and choosen in _initPhysicalDevice()
		VkDevice mvkLogicalDevice;							// Logical Vulkan device
		VkQueue mvkGraphicsQueue;							// Graphics queue for the logical device
		VkSurfaceKHR mvkWindowSurface;						// The window surface which Vulkan will render to
		VkQueue mvkPresentationQueue;						// Presentation queue for the window surface of the logical device.
		VkSwapchainKHR mvkSwapChain;						// Vulkan swap chain
		VkExtent2D mvkSwapChainExtent;						// The dimensions of the images in the swapchain
		VkFormat mvkSwapchainImageFormat;					// Image format of the swapchain images
		std::vector<VkImage> mvkSwapChainImages;			// Images for the swap chain
		std::vector<VkImageView> mvkSwapchainImageViews;	// Image views for the swap chain images
		VkRenderPass mvkRenderPass;							// Render pass

		// Find a physical device
		void _initPhysicalDevice(void);

		// Used by _initPhysicalDevice to compute a "score" for a physical device
		// _initPhysicalDevice then chooses the device with the highest score.
		// The more features, a higher score for a physical device.
		// Also logs device features to log file.
		int _initComputePhysicalDeviceScore(VkPhysicalDevice vkPhysicalDevice);
	};
}