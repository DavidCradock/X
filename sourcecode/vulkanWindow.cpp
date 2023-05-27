#include "PCH.h"
#include "vulkanWindow.h"
#include "log.h"
#include "graphicsPipelines.h"

namespace X
{
	LRESULT CALLBACK MainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		return VulkanWindow::getPointer()->MsgProc(hwnd, msg, wParam, lParam);
	}

	// Debug messenger callback for Vulkan in debug builds
	VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
		VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
		VkDebugUtilsMessageTypeFlagsEXT messageType,
		const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
		void* pUserData)
	{
		// messageSeverity can be one of the following...
		// VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT : Diagnostic message
		// VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT : Informational message like the creation of a resource
		// VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT : Message which is probably something we've done wrong, but not an error
		// VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT : Message which is an error and may cause a crash.

		if (messageSeverity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT)
		{
			Log* pLog = Log::getPointer();
			std::string strError = pCallbackData->pMessage;
			pLog->add("\nVulkan debug message: " + strError, true, false);
			ThrowIfTrue(1, strError);
		}
		return VK_FALSE;
	}

	VulkanWindow::VulkanWindow()
	{
		mhInstance = GetModuleHandle(NULL);
		mhWindowHandle = NULL;
		miWindowWidth = 1024;
		miWindowHeight = 576;

		mvkPhysicalDevice = VK_NULL_HANDLE;
	}

	void VulkanWindow::initialise(std::string strWindowTitle, int iWindowWidth, int iWindowHeight, bool bVsyncEnabled, bool bLogLogicalDeviceExtensions)
	{
		Log* pLog = Log::getPointer();
		pLog->add("VulkanWindow::initialise() called.");

		mstrWindowTitle = strWindowTitle;

		// Create application window
		mWindowClass.cbClsExtra = 0;
		mWindowClass.cbWndExtra = 0;
		mWindowClass.hCursor = LoadCursor(0, IDC_ARROW);
		mWindowClass.hIcon = LoadIcon(0, IDI_APPLICATION);
		mWindowClass.hInstance = mhInstance;
		mWindowClass.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH);
		mWindowClass.lpfnWndProc = MainWndProc;	// Window procedure which passes everything onto this class's window procedure method
		mWindowClass.lpszClassName = L"XWindowClassName";
		mWindowClass.lpszMenuName = 0;
		mWindowClass.style = CS_HREDRAW | CS_VREDRAW;

		ThrowIfFalse(RegisterClass(&mWindowClass), "VulkanWindow::initialise() failed to register window class.");

		RECT R = { 0, 0, miWindowWidth, miWindowHeight };
		AdjustWindowRect(&R, WS_OVERLAPPEDWINDOW, false);
		int width = R.right - R.left;
		int height = R.bottom - R.top;

		mhWindowHandle = CreateWindow(L"XWindowClassName", StringToWString(mstrWindowTitle).c_str(), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, width, height, 0, 0, mhInstance, 0);
		ThrowIfFalse(mhWindowHandle, "VulkanWindow::initialise() failed to create window.");

		// Center window
		RECT rectDesktop;
		GetClientRect(GetDesktopWindow(), &rectDesktop);
		int iPosLeft = (rectDesktop.right / 2) - (width / 2);
		int iPosTop = (rectDesktop.bottom / 2) - (height / 2);
		SetWindowPos(mhWindowHandle, HWND_TOP, iPosLeft, iPosTop, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
		ShowWindow(mhWindowHandle, SW_SHOW);
		UpdateWindow(mhWindowHandle);
		pLog->add("VulkanWindow::initialise() has created the application window.");

		// Debug layers
		bool bValidationLayersInUse = false;
#ifdef _DEBUG
		bValidationLayersInUse = true;
#endif

		// Create Vulkan main instance
		// This next struct is optional, but as we're using Vulkan that's a pretty verbose API,
		// we might aswell use it.
		VkApplicationInfo vkApplicationInfo{};
		vkApplicationInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		vkApplicationInfo.pNext = nullptr;
		vkApplicationInfo.pApplicationName = "X";
		vkApplicationInfo.applicationVersion = VK_MAKE_API_VERSION(1, 1, 0, 0);
		vkApplicationInfo.pEngineName = "X";
		vkApplicationInfo.engineVersion = VK_MAKE_API_VERSION(1, 1, 0, 0);
		vkApplicationInfo.apiVersion = VK_API_VERSION_1_3;

		// Get supported instance extensions 
		uint32_t uiInstanceExtensionCount = 0;
		vkEnumerateInstanceExtensionProperties(nullptr, &uiInstanceExtensionCount, nullptr);
		std::vector<std::string> supportedInstanceExtensions;
		if (uiInstanceExtensionCount > 0)
		{
			pLog->add("VulkanWindow::initialise() has detected " + std::to_string(uiInstanceExtensionCount) + " instance extensions.");
			std::vector<VkExtensionProperties> extensions(uiInstanceExtensionCount);
			if (VK_SUCCESS == vkEnumerateInstanceExtensionProperties(nullptr, &uiInstanceExtensionCount, &extensions.front()))
			{
				for (VkExtensionProperties extension : extensions)
				{
					supportedInstanceExtensions.push_back(extension.extensionName);
				}
			}
		}
		// Log supported instance extensions
		for (int i = 0; i < supportedInstanceExtensions.size(); ++i)
		{
			pLog->add("VulkanWindow::initialise() instance extension name: " + supportedInstanceExtensions[i]);
		}

		// Add required instance extensions
		std::vector<const char*> requiredInstanceExtensions = { VK_KHR_SURFACE_EXTENSION_NAME };
		requiredInstanceExtensions.push_back(VK_KHR_WIN32_SURFACE_EXTENSION_NAME);

		// Add debug tools if in debug AND they are available (Due to the VulkanSDK being installed)
		if (bValidationLayersInUse)	// If we're in DEBUG build
		{
			// Find if they're available
			if (std::find(supportedInstanceExtensions.begin(), supportedInstanceExtensions.end(), VK_EXT_DEBUG_UTILS_EXTENSION_NAME) != supportedInstanceExtensions.end())
			{
				// Add the extension to the required extensions
				requiredInstanceExtensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
			}
		}

		// Log required instance extensions
		pLog->add("Required instance extensions...");
		for (int i = 0; i < requiredInstanceExtensions.size(); ++i)
		{
			pLog->add(requiredInstanceExtensions[i]);
		}

		// We can just attempt to create the device attempting to use the above requested extensions
		// and then if it fails, bail out, but it'd be nice to see which extension isn't supported by
		// comparing and attempting to find whether each extension in requiredInstanceExtensions is found in
		// supportedInstanceExtensions, so let's do that...
		for (int i = 0; i < requiredInstanceExtensions.size(); ++i)
		{
			// Attempt to find the extension in supportedInstanceExtensions
			if (std::find(supportedInstanceExtensions.begin(), supportedInstanceExtensions.end(), requiredInstanceExtensions[i]) == supportedInstanceExtensions.end())
			{
				std::string strError("VulkanWindow::initialise() failed to find required instance extension: ");
				strError += requiredInstanceExtensions[i];
				ThrowIfTrue(1, strError);
			}
		}

		// This next struct isn't optional.
		// It tells the Vulkan driver which extensions and validation layers which we wish to use.
		// It's setup below, but we need it here to set the debug validation layers part of it.
		VkInstanceCreateInfo vkInstanceCreateInfo{};

		// Now see if "VK_LAYER_KHRONOS_validation" is available for the debug validation layers
		if (bValidationLayersInUse)
		{
			const char* validationLayerName = "VK_LAYER_KHRONOS_validation";
			uint32_t instanceLayerCount;
			vkEnumerateInstanceLayerProperties(&instanceLayerCount, nullptr);
			std::vector<VkLayerProperties> instanceLayerProperties(instanceLayerCount);
			vkEnumerateInstanceLayerProperties(&instanceLayerCount, instanceLayerProperties.data());
			bool validationLayerPresent = false;
			for (VkLayerProperties layer : instanceLayerProperties)
			{
				if (strcmp(layer.layerName, validationLayerName) == 0)
				{
					validationLayerPresent = true;
					break;
				}
			}
			if (validationLayerPresent)
			{
				pLog->add("VulkanWindow::initialise() has enabled the debug layers.");
				vkInstanceCreateInfo.enabledLayerCount = 1;
				vkInstanceCreateInfo.ppEnabledLayerNames = &validationLayerName;
			}
		}

		vkInstanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		vkInstanceCreateInfo.pNext = nullptr;
		vkInstanceCreateInfo.flags = 0;
		vkInstanceCreateInfo.pApplicationInfo = &vkApplicationInfo;
		vkInstanceCreateInfo.enabledExtensionCount = (uint32_t)requiredInstanceExtensions.size();	// Number of global extensions to enable
		vkInstanceCreateInfo.ppEnabledExtensionNames = requiredInstanceExtensions.data();

		// Attempt to create the Vulkan instance
		pLog->add("VulkanWindow::initialise() attempting to create main Vulkan instance.");
		VkResult result = vkCreateInstance(&vkInstanceCreateInfo, nullptr, &mvkInstance);
		ThrowIfFalse(bool(VK_SUCCESS == result), "VulkanWindow::initialise() failed to create the main Vulkan instance.");
		pLog->add("VulkanWindow::initialise() main Vulkan instance created.");

		// Now we create debug callback (if we're in DEBUG build) so we can see all the errors which Vulkan generates
		if (bValidationLayersInUse)
		{
			VkDebugUtilsMessengerCreateInfoEXT createInfo{};
			createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
			createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
			createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
			createInfo.pfnUserCallback = debugCallback;
			createInfo.pUserData = nullptr;
			// Obtain the function for registering the callback
			auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(mvkInstance, "vkCreateDebugUtilsMessengerEXT");
			if (func != nullptr)
			{
				ThrowIfTrue(VK_SUCCESS != func(mvkInstance, &createInfo, nullptr, &mvkDebugMessenger), "VulkanWindow::initialise() failed whilst setting up debug function.");
			}
			else
			{
				ThrowIfTrue(1, "VulkanWindow::initialise() unable to obtain vkCreateDebugUtilsMessengerExt function.");
			}
		}

		_initPhysicalDevice();

		// Get each Queue family of the physical device and make sure the VK_QUEUE_GRAPHICS_BIT is set for one of them.
		// Also, store the index to the graphics queue so we can use it below
		uint32_t queueFamilyCount = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(mvkPhysicalDevice, &queueFamilyCount, nullptr);
		std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
		vkGetPhysicalDeviceQueueFamilyProperties(mvkPhysicalDevice, &queueFamilyCount, queueFamilies.data());
		int i = 0;
		std::optional<uint32_t> graphicsFamily;
		for (const auto& queueFamily : queueFamilies)
		{
			if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
			{
				graphicsFamily = i;
			}
			i++;
		}
		ThrowIfFalse(graphicsFamily.has_value(), "VulkanWindow::initialise() was unable to find the VK_QUEUE_GRAPHICS_BIT for the chosen physical Vulkan device.");

		// We have already checked for instance level extensions.
		// We need to check device level extensions for the VK_KHR_SWAPCHAIN_EXTENSION_NAME and enable it
		uint32_t uiDeviceExtensionCount = 0;
		vkEnumerateDeviceExtensionProperties(mvkPhysicalDevice, nullptr, &uiDeviceExtensionCount, nullptr);
		std::vector<std::string> supportedDeviceExtensions;
		if (uiDeviceExtensionCount > 0)
		{
			pLog->add("VulkanWindow::initialise() has detected " + std::to_string(uiDeviceExtensionCount) + " physical device extensions.");
			std::vector<VkExtensionProperties> extensions(uiDeviceExtensionCount);
			if (VK_SUCCESS == vkEnumerateDeviceExtensionProperties(mvkPhysicalDevice, nullptr, &uiDeviceExtensionCount, &extensions.front()))
			{
				for (VkExtensionProperties extension : extensions)
				{
					supportedDeviceExtensions.push_back(extension.extensionName);
				}
			}
		}
		// Log supported device extensions
		if (bLogLogicalDeviceExtensions)
		{
			for (int i = 0; i < supportedDeviceExtensions.size(); ++i)
			{
				pLog->add("VulkanWindow::initialise() device extension name: " + supportedDeviceExtensions[i]);
			}
		}

		// Add required device extensions
		std::vector<const char*> requiredDeviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
		//		requiredDeviceExtensions.push_back(MORE_EXTENSIONS);

				// Log required device extensions
		pLog->add("Required device extensions...");
		for (int i = 0; i < requiredDeviceExtensions.size(); ++i)
		{
			pLog->add(requiredDeviceExtensions[i]);
		}

		// We can just attempt to create the logical device attempting to use the above requested extensions
		// and then if it fails, bail out, but it'd be nice to see which extension isn't supported by
		// comparing and attempting to find whether each extension in requiredDeviceExtensions is found in
		// supportedDeviceExtensions, so let's do that...
		for (int i = 0; i < requiredDeviceExtensions.size(); ++i)
		{
			// Attempt to find the extension in supportedInstanceExtensions
			if (std::find(supportedDeviceExtensions.begin(), supportedDeviceExtensions.end(), requiredDeviceExtensions[i]) == supportedDeviceExtensions.end())
			{
				std::string strError("VulkanWindow::initialise() failed to find required device extension: ");
				strError += requiredDeviceExtensions[i];
				ThrowIfTrue(1, strError);
			}
		}

		// Create logical Vulkan device for the physical device
		VkDeviceQueueCreateInfo queueCreateInfo{};
		queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queueCreateInfo.queueFamilyIndex = graphicsFamily.value();
		queueCreateInfo.queueCount = 1;
		float queuePriority = 1.0f;
		queueCreateInfo.pQueuePriorities = &queuePriority;
		// Specify used device features
		VkPhysicalDeviceFeatures deviceFeatures{};
		deviceFeatures.geometryShader = true;
		VkDeviceCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		createInfo.pQueueCreateInfos = &queueCreateInfo;
		createInfo.queueCreateInfoCount = 1;
		createInfo.pEnabledFeatures = &deviceFeatures;
		// Device level extensions
		createInfo.ppEnabledExtensionNames = requiredDeviceExtensions.data();
		createInfo.enabledExtensionCount = (uint32_t)requiredDeviceExtensions.size();

		// Set debug layer info from the physical device's creation structure
		// Apparantly, these are ignored by more recent API versions, but we'll add it anyway, just for backwards compatibility.
		createInfo.enabledLayerCount = vkInstanceCreateInfo.enabledLayerCount;
		createInfo.ppEnabledLayerNames = vkInstanceCreateInfo.ppEnabledLayerNames;
		// Attempt to create the logical device
		ThrowIfTrue(VK_SUCCESS != vkCreateDevice(mvkPhysicalDevice, &createInfo, nullptr, &mvkLogicalDevice), "VulkanWindow::initialise() was unable to create the Vulkan logical device.");
		// Obtain the graphics queue
		vkGetDeviceQueue(mvkLogicalDevice, graphicsFamily.value(), 0, &mvkGraphicsQueue);
		pLog->add("VulkanWindow::initialise() has created the Vulkan logical device and obtained the graphics queue.");

		// Now create the window surface which Vulkan will render to and present in the window
		VkWin32SurfaceCreateInfoKHR createInfoWindowSurface{};
		createInfoWindowSurface.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
		createInfoWindowSurface.hwnd = mhWindowHandle;
		createInfoWindowSurface.hinstance = mhInstance;
		ThrowIfTrue(VK_SUCCESS != vkCreateWin32SurfaceKHR(mvkInstance, &createInfoWindowSurface, nullptr, &mvkWindowSurface), "VulkanWindow::initialise() failed during call to vkCreateWin32SurfaceKHR()");
		pLog->add("VulkanWindow::initialise() has created the window surface.");

		// Make sure that the surface has presentation support
		VkBool32 presentSupport = false;
		vkGetPhysicalDeviceSurfaceSupportKHR(mvkPhysicalDevice, graphicsFamily.value(), mvkWindowSurface, &presentSupport);
		ThrowIfFalse(presentSupport, "VulkanWindow::initialise() has detected that the created surface does not have presentation support.");
		pLog->add("VulkanWindow::initialise() has detected that the created surface does have presentation support.");

		// Create the presentation queue
		vkGetDeviceQueue(mvkLogicalDevice, graphicsFamily.value(), 0, &mvkPresentationQueue);
		ThrowIfFalse(mvkPresentationQueue, "VulkanWindow::initialise() was unable to create the presentation queue for the Vulkan logical device.");
		pLog->add("VulkanWindow::initialise() has created the presentation queue for the Vulkan logical device.");

		// Swapchain
		VkSurfaceCapabilitiesKHR capabilities;
		std::vector<VkSurfaceFormatKHR> formats;
		std::vector<VkPresentModeKHR> presentModes;
		// Get surface capabilies
		vkGetPhysicalDeviceSurfaceCapabilitiesKHR(mvkPhysicalDevice, mvkWindowSurface, &capabilities);
		// Get formats
		uint32_t formatCount;
		vkGetPhysicalDeviceSurfaceFormatsKHR(mvkPhysicalDevice, mvkWindowSurface, &formatCount, nullptr);
		if (formatCount != 0)
		{
			formats.resize(formatCount);
			vkGetPhysicalDeviceSurfaceFormatsKHR(mvkPhysicalDevice, mvkWindowSurface, &formatCount, formats.data());
		}
		else
		{
			ThrowIfTrue(1, "VulkanWindow::initialise() detected that the Vulkan physical device and it's created window surface have no formats.");
		}
		pLog->add("VulkanWindow::initialise() has detected " + std::to_string(formatCount) + " formats available.");

		// Go through all formats to see if the one we want is available
		VkSurfaceFormatKHR foundFormat;	// Will hold the requested format
		bool bFormatFound = false;
		for (const auto& format : formats)
		{
			if (format.format == VK_FORMAT_B8G8R8A8_SRGB && format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
			{
				bFormatFound = true;
				foundFormat = format;
				break;
			}
		}
		ThrowIfFalse(bFormatFound, "VulkanWindow::initialise() was unable to find the requested format of VK_FORMAT_B8G8R8A8_SRGB.");
		pLog->add("VulkanWindow::initialise() found the requested format of VK_FORMAT_B8G8R8A8_SRGB.");

		// Get presentation modes
		uint32_t presentModeCount;
		vkGetPhysicalDeviceSurfacePresentModesKHR(mvkPhysicalDevice, mvkWindowSurface, &presentModeCount, nullptr);

		if (presentModeCount != 0)
		{
			presentModes.resize(presentModeCount);
			vkGetPhysicalDeviceSurfacePresentModesKHR(mvkPhysicalDevice, mvkWindowSurface, &presentModeCount, presentModes.data());
		}
		else
		{
			ThrowIfTrue(1, "VulkanWindow::initialise() detected that the Vulkan physical device and it's created window surface have no presentation modes.");
		}
		pLog->add("VulkanWindow::initialise() has detected " + std::to_string(presentModeCount) + " presentation modes available.");

		// Go though all the presentation modes to see if the one we want is available
		VkPresentModeKHR requestedPresentMode = VK_PRESENT_MODE_FIFO_KHR;
		if (!bVsyncEnabled)
			requestedPresentMode = VK_PRESENT_MODE_IMMEDIATE_KHR;
		VkPresentModeKHR foundPresentMode;	// Will hold the requested presentation mode.
		bool bPresentationModeFound = false;
		for (const auto& presentMode : presentModes)
		{
			if (presentMode == requestedPresentMode)
			{
				// If we get here, we've found either immediate or vsync mode
				bPresentationModeFound = true;
				foundPresentMode = presentMode;
				break;
			}
		}
		ThrowIfFalse(bPresentationModeFound, "VulkanWindow::initialise() was unable to find the requested presentation mode.");
		pLog->add("VulkanWindow::initialise() was able to find the requested presentation mode.", false, true);
		if (bVsyncEnabled)
			pLog->add(" (VSync enabled)", true, false);
		else
			pLog->add(" (VSync disabled)", true, false);

		// If we get here, both foundFormat and foundPresentMode are correct
		// We'll use those in a bit to setup the swapchain.

		// Setup the swap chain (That was quick! :D)
		// Get swap chain image dimensions, aka, the extent.
		if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
		{
			mvkSwapchainExtent = capabilities.currentExtent;
		}
		else
		{
			VkExtent2D actualExtent = { static_cast<uint32_t>(iWindowWidth), static_cast<uint32_t>(iWindowHeight) };
			actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
			actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);
			mvkSwapchainExtent = actualExtent;
		}
		// How many images are needed in the swap chain?
		// Vulkan specifies the minimum amount needed, but we give it an extra one so that we
		// don't have to wait around for the driver to finish "internal stuff" before using the next one.
		uint32_t imageCount = capabilities.minImageCount + 1;
		// We should however, make sure that we do not exceed the maximum number of images allowed.
		// If maxImageCount is 0, that means that there is no limit.
		if (capabilities.maxImageCount > 0 && imageCount > capabilities.maxImageCount)
		{
			imageCount = capabilities.maxImageCount;
		}
		// Now set up a Vulkan struct
		VkSwapchainCreateInfoKHR vkSwapchainCreateInfo{};
		vkSwapchainCreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		vkSwapchainCreateInfo.surface = mvkWindowSurface;
		vkSwapchainCreateInfo.minImageCount = imageCount;
		vkSwapchainCreateInfo.imageFormat = foundFormat.format;
		vkSwapchainCreateInfo.imageColorSpace = foundFormat.colorSpace;
		vkSwapchainCreateInfo.imageExtent = mvkSwapchainExtent;
		vkSwapchainCreateInfo.imageArrayLayers = 1;
		// VK_IMAGE_USAGE_TRANSFER_DST_BIT  could be used here if we want to do post-processing, render to this and then copy it manually
		vkSwapchainCreateInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
		vkSwapchainCreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		vkSwapchainCreateInfo.queueFamilyIndexCount = 0; // Optional
		vkSwapchainCreateInfo.pQueueFamilyIndices = nullptr; // Optional
		vkSwapchainCreateInfo.preTransform = capabilities.currentTransform;
		vkSwapchainCreateInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
		vkSwapchainCreateInfo.presentMode = foundPresentMode;
		vkSwapchainCreateInfo.clipped = VK_TRUE;
		vkSwapchainCreateInfo.oldSwapchain = VK_NULL_HANDLE;

		// Create the swap chain
		pLog->add("VulkanWindow::initialise() attempting to create swapchain... ", false, true);
		ThrowIfTrue(VK_SUCCESS != vkCreateSwapchainKHR(mvkLogicalDevice, &vkSwapchainCreateInfo, nullptr, &mvkSwapchain), "VulkanWindow::intialise() failed to create Vulkan swapchain.");
		pLog->add("done.", true, false);

		// Get image handles
		// First, return the actual number of images created by Vulkan
		vkGetSwapchainImagesKHR(mvkLogicalDevice, mvkSwapchain, &imageCount, nullptr);
		// Resize to fit them all
		mvkSwapchainImages.resize(imageCount);
		// Get the swapchain images
		vkGetSwapchainImagesKHR(mvkLogicalDevice, mvkSwapchain, &imageCount, mvkSwapchainImages.data());
		// Finally, store the image format of the images used in the swap chain
		mvkSwapchainImageFormat = foundFormat.format;

		// Now we setup image views for the swapchain's images
		// Resize the image views vector to the number of images in the swapchain
		mvkSwapchainImageViews.resize(mvkSwapchainImages.size());
		// For each image in the swap chain, create an image view
		pLog->add("VulkanWindow::initialise() attempting to create image views for the swapchain images...", false, true);
		for (size_t i = 0; i < mvkSwapchainImages.size(); i++)
		{
			VkImageViewCreateInfo imageViewCreateInfo{};
			imageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
			imageViewCreateInfo.image = mvkSwapchainImages[i];
			imageViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
			imageViewCreateInfo.format = mvkSwapchainImageFormat;
			imageViewCreateInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
			imageViewCreateInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
			imageViewCreateInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
			imageViewCreateInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
			// No mipmaps or multiple layers...
			imageViewCreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			imageViewCreateInfo.subresourceRange.baseMipLevel = 0;
			imageViewCreateInfo.subresourceRange.levelCount = 1;
			imageViewCreateInfo.subresourceRange.baseArrayLayer = 0;
			imageViewCreateInfo.subresourceRange.layerCount = 1;

			if (vkCreateImageView(mvkLogicalDevice, &imageViewCreateInfo, nullptr, &mvkSwapchainImageViews[i]) != VK_SUCCESS)
			{
				ThrowIfTrue(1, "VulkanWindow::initialise() failed to create image views for the swapchain images");
			}
		}
		pLog->add(" done.", true, false);

		// Now setup the render pass
		VkAttachmentDescription colorAttachment{};
		colorAttachment.format = mvkSwapchainImageFormat;
		colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
		colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

		VkAttachmentReference colorAttachmentRef{};
		colorAttachmentRef.attachment = 0;
		colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

		VkSubpassDependency dependency{};
		dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
		dependency.dstSubpass = 0;
		dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		dependency.srcAccessMask = 0;
		dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

		VkSubpassDescription subpass{};
		subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
		subpass.colorAttachmentCount = 1;
		subpass.pColorAttachments = &colorAttachmentRef;
		VkRenderPassCreateInfo createInfoRenderPass{};
		createInfoRenderPass.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		createInfoRenderPass.attachmentCount = 1;
		createInfoRenderPass.pAttachments = &colorAttachment;
		createInfoRenderPass.subpassCount = 1;
		createInfoRenderPass.pSubpasses = &subpass;
		createInfoRenderPass.dependencyCount = 1;
		createInfoRenderPass.pDependencies = &dependency;

		pLog->add("VulkanWindow::initialise() attempting to create the render pass...", false, true);
		ThrowIfTrue(VK_SUCCESS != vkCreateRenderPass(mvkLogicalDevice, &createInfoRenderPass, nullptr, &mvkRenderPass), "VulkanWindow::initialise() failed to create render pass.");
		pLog->add(" done.", true, false);

		

		//  Swapchain framebuffers
		mvkSwapchainFramebuffers.resize(mvkSwapchainImageViews.size());
		for (size_t i = 0; i < mvkSwapchainImageViews.size(); i++)
		{
			VkImageView attachments[] = {
				mvkSwapchainImageViews[i]
			};

			VkFramebufferCreateInfo createInfoframebuffer{};
			createInfoframebuffer.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
			createInfoframebuffer.renderPass = mvkRenderPass;
			createInfoframebuffer.attachmentCount = 1;
			createInfoframebuffer.pAttachments = attachments;
			createInfoframebuffer.width = mvkSwapchainExtent.width;
			createInfoframebuffer.height = mvkSwapchainExtent.height;
			createInfoframebuffer.layers = 1;
			ThrowIfTrue(VK_SUCCESS != vkCreateFramebuffer(mvkLogicalDevice, &createInfoframebuffer, nullptr, &mvkSwapchainFramebuffers[i]), "VulkanWindow::initialise() failed to create framebuffer.");
		}

		// Command pool
		VkCommandPoolCreateInfo createInfoCommandPool{};
		createInfoCommandPool.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		createInfoCommandPool.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
		createInfoCommandPool.queueFamilyIndex = graphicsFamily.value();
		ThrowIfTrue(VK_SUCCESS != vkCreateCommandPool(mvkLogicalDevice, &createInfoCommandPool, nullptr, &mvkCommandPool), "VulkanWindow::initialise() failed to create command pool.");

		// Command buffer
		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.commandPool = mvkCommandPool;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandBufferCount = 1;
		ThrowIfTrue(VK_SUCCESS != vkAllocateCommandBuffers(mvkLogicalDevice, &allocInfo, &mvkCommandBuffer),  "VulkanWindow::initialise() failed to allocate command buffer.");
		
		// Create syncronization objects
		VkSemaphoreCreateInfo createInfoSemaphore{};
		createInfoSemaphore.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
		ThrowIfTrue(VK_SUCCESS != vkCreateSemaphore(mvkLogicalDevice, &createInfoSemaphore, nullptr, &mvkSemaphoreImageAvailable), "VulkanWindow::initialise() failed to create semaphore used to determine whether swapchain image is available.");
		ThrowIfTrue(VK_SUCCESS != vkCreateSemaphore(mvkLogicalDevice, &createInfoSemaphore, nullptr, &mvkSemaphoreRenderFinished), "VulkanWindow::initialise() failed to create semaphore used to determine whether rendering has finished.");
		VkFenceCreateInfo createInfoFence{};
		createInfoFence.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		// Set the fence to initially be signalled so that upon first call to the drawFrame() method, we won't wait forever for it to be signalled.
		createInfoFence.flags = VK_FENCE_CREATE_SIGNALED_BIT;
		ThrowIfTrue(VK_SUCCESS != vkCreateFence(mvkLogicalDevice, &createInfoFence, nullptr, &mvkFenceInFlight), "VulkanWindow::initialise() failed to create in flight fence.");
		
		// Create default graphics pipeline
		_createDefaultGraphicsPipeline();
	}

	void VulkanWindow::_createDefaultGraphicsPipeline(void)
	{
		GraphicsPipelineManager* pGraphicsPipelineManager = GraphicsPipelineManager::getPointer();
		GraphicsPipeline* pGraphicsPipeline = pGraphicsPipelineManager->add("default", "default");
		pGraphicsPipeline->setFilenames("GPUprograms/dev_v.spv", "GPUprograms/dev_f.spv");
		pGraphicsPipelineManager->loadGroup("default");
	}

	void VulkanWindow::_recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex, const std::string &strGraphicsPipelineName, const std::string &strGraphicsPipelineGroupName)
	{
		// Obtain graphics pipeline from manager
		GraphicsPipelineManager* pGraphicsPipelineManager = GraphicsPipelineManager::getPointer();
		GraphicsPipeline* pGraphicsPipeline = pGraphicsPipelineManager->get(strGraphicsPipelineName, strGraphicsPipelineGroupName);
		VkPipeline graphicsPipeline = pGraphicsPipeline->getGraphicsPipeline();

		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags = 0; // Optional
		beginInfo.pInheritanceInfo = nullptr; // Optional

		// Being recording to the command buffer
		ThrowIfTrue(VK_SUCCESS != vkBeginCommandBuffer(commandBuffer, &beginInfo), "VulkanWindow::_recordCommandBuffer() failed to begin recording to the command buffer.");
		VkRenderPassBeginInfo renderPassInfo{};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass = mvkRenderPass;
		renderPassInfo.framebuffer = mvkSwapchainFramebuffers[imageIndex];
		renderPassInfo.renderArea.offset = { 0, 0 };
		renderPassInfo.renderArea.extent = mvkSwapchainExtent;
		VkClearValue clearColour{};
		clearColour.color.float32[0] = mv4ClearColour.r;
		clearColour.color.float32[1] = mv4ClearColour.g;
		clearColour.color.float32[2] = mv4ClearColour.b;
		clearColour.color.float32[3] = mv4ClearColour.a;
		renderPassInfo.clearValueCount = 1;
		renderPassInfo.pClearValues = &clearColour;

		// Begin the render pass
		vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

		// Bind the named graphics pipeline
		vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline);

		// Viewport and scissor state for pipelines are dynamic, so we need to set them here
		VkViewport viewport{};
		viewport.x = 0.0f;
		viewport.y = 0.0f;
		viewport.width = static_cast<float>(mvkSwapchainExtent.width);
		viewport.height = static_cast<float>(mvkSwapchainExtent.height);
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;
		vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
		VkRect2D scissor{};
		scissor.offset = { 0, 0 };
		scissor.extent = mvkSwapchainExtent;
		vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

		// Draw a triangle
		vkCmdDraw(commandBuffer, 3, 1, 0, 0);

		// End the render pass
		vkCmdEndRenderPass(commandBuffer);

		// End recording to the command buffer
		ThrowIfTrue(VK_SUCCESS != vkEndCommandBuffer(commandBuffer), "VulkanWindow::_recordCommandBuffer() failed to end recording to the command buffer.");
	}

	void VulkanWindow::shutdown(void)
	{
		// Wait for things to be finished before continuing
		// Otherwise, we might be trying to destroy stuff while stuff is still going on which is a bad idea
		vkDeviceWaitIdle(mvkLogicalDevice);

		// Delete the debug messenger if in DEBUG
		bool bValidationLayersInUse = false;
#ifdef _DEBUG
		bValidationLayersInUse = true;
#endif
		if (bValidationLayersInUse)
		{
			// Obtain the function for destroying the callback
			auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(mvkInstance, "vkDestroyDebugUtilsMessengerEXT");
			if (func != nullptr)
			{
				func(mvkInstance, mvkDebugMessenger, nullptr);
			}
			else
			{
				ThrowIfTrue(1, "VulkanWindow::shutdown() unable to obtain function pointer to vkDestroyDebugUtilsMessengerEXT function.");
			}
		}

		// Destroy syncronization objects
		vkDestroySemaphore(mvkLogicalDevice, mvkSemaphoreImageAvailable, nullptr);
		vkDestroySemaphore(mvkLogicalDevice, mvkSemaphoreRenderFinished, nullptr);
		vkDestroyFence(mvkLogicalDevice, mvkFenceInFlight, nullptr);

		// Destroy command pool
		vkDestroyCommandPool(mvkLogicalDevice, mvkCommandPool, nullptr);

		// Destroy framebuffers
		for (auto framebuffer : mvkSwapchainFramebuffers)
		{
			vkDestroyFramebuffer(mvkLogicalDevice, framebuffer, nullptr);
		}

		// Destroy the render pass
		vkDestroyRenderPass(mvkLogicalDevice, mvkRenderPass, nullptr);

		// Destroy swapchain image views
		for (auto imageView : mvkSwapchainImageViews)
		{
			vkDestroyImageView(mvkLogicalDevice, imageView, nullptr);
		}

		// Destroy the swapchain
		vkDestroySwapchainKHR(mvkLogicalDevice, mvkSwapchain, nullptr);

		// Destroy window surface
		vkDestroySurfaceKHR(mvkInstance, mvkWindowSurface, nullptr);

		// Destroy the Vulkan logical device
		vkDestroyDevice(mvkLogicalDevice, nullptr);

		// Destroy main Vulkan instance
		vkDestroyInstance(mvkInstance, nullptr);

		// Close window
		if (mhWindowHandle)
		{
			CloseWindow(mhWindowHandle);
			mhWindowHandle = NULL;
		}
		// Unregister window class
		UnregisterClass(L"XWindowClassName", mhInstance);
	}

	void VulkanWindow::_initPhysicalDevice(void)
	{
		Log* pLog = Log::getPointer();
		std::string strLogPrefix = "VulkanWindow::_initPhysicalDevice() ";
		pLog->add(strLogPrefix + "called.");

		// Find how many physical devices there are, if none, throw stuff around have a little tantrum.
		uint32_t uiDeviceCount = 0;
		vkEnumeratePhysicalDevices(mvkInstance, &uiDeviceCount, nullptr);
		pLog->add("VulkanWindow::_initPhysicalDevice() found " + std::to_string(uiDeviceCount) + " physical devices.");
		ThrowIfTrue(0 == uiDeviceCount, strLogPrefix + "no physical devices were found.");

		// Populate a vector with information about each physical device.
		std::vector<VkPhysicalDevice> vkPhysicalDevices(uiDeviceCount);
		vkEnumeratePhysicalDevices(mvkInstance, &uiDeviceCount, vkPhysicalDevices.data());

		// For each physical device, compute their score and set mvkPhysicalDevice to the highest scoring device.
		int iHighestScore = 0;
		for (const auto& vkPhysicalDevice : vkPhysicalDevices)
		{
			int iScore = _initComputePhysicalDeviceScore(vkPhysicalDevice);
			if (iScore > iHighestScore)
			{
				iHighestScore = iScore;
				mvkPhysicalDevice = vkPhysicalDevice;
			}
		}
		ThrowIfTrue(0 == iHighestScore, strLogPrefix + "unable to find a physical device which has required features.");
		pLog->add(strLogPrefix + "Vulkan physical device has been created.");
	}

	int VulkanWindow::_initComputePhysicalDeviceScore(VkPhysicalDevice vkPhysicalDevice)
	{
		int iScore = 0;

		// Get physical device properties
		VkPhysicalDeviceProperties vkDeviceProperties;
		vkGetPhysicalDeviceProperties(vkPhysicalDevice, &vkDeviceProperties);

		// Log physical device properties
		Log* pLog = Log::getPointer();
		pLog->add("VulkanWindow::_initComputePhysicalDeviceScore() Vulkan Physical Device Name: " + std::string(vkDeviceProperties.deviceName));

		// Get physical device features
		VkPhysicalDeviceFeatures vkDeviceFeatures;
		vkGetPhysicalDeviceFeatures(vkPhysicalDevice, &vkDeviceFeatures);
		if (vkDeviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU)
		{
			pLog->add("VulkanWindow::_initComputePhysicalDeviceScore() Vulkan Physical Device Type: Virtual GPU");
			iScore += 1;
		}
		if (vkDeviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU)
		{
			pLog->add("VulkanWindow::_initComputePhysicalDeviceScore() Vulkan Physical Device Type: Integrated GPU");
			iScore += 1000;
		}
		if (vkDeviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
		{
			pLog->add("VulkanWindow::_initComputePhysicalDeviceScore() Vulkan Physical Device Type: Discrete GPU");
			iScore += 100000;
		}

		// Nice to have features
		// Nice to have larger texture dimensions
		iScore += vkDeviceProperties.limits.maxImageDimension2D;
		pLog->add("VulkanWindow::_initComputePhysicalDeviceScore() Vulkan Physical Device maxImageDimension2D: " + std::to_string(vkDeviceProperties.limits.maxImageDimension2D));

		// Check features we NEED
		if (!vkDeviceFeatures.geometryShader)
			return 0;

		return iScore;
	}

	bool VulkanWindow::update(void)
	{
		return true;
	}

	void VulkanWindow::drawFrame(void)
	{
		// Wait for frames to be rendered
		vkWaitForFences(mvkLogicalDevice, 1, &mvkFenceInFlight, VK_TRUE, UINT64_MAX);

		// Reset fence object to be unsignalled again...
		vkResetFences(mvkLogicalDevice, 1, &mvkFenceInFlight);

		// Get image from the swapchain
		uint32_t imageIndex;	// Index into mvkSwapchainImages which is now available
		vkAcquireNextImageKHR(mvkLogicalDevice, mvkSwapchain, UINT64_MAX, mvkSemaphoreImageAvailable, VK_NULL_HANDLE, &imageIndex);

		// Reset the command buffer so it can be recorded to
		vkResetCommandBuffer(mvkCommandBuffer, 0);

		// Record the command buffer
		_recordCommandBuffer(mvkCommandBuffer, imageIndex, "default", "default");

		// Submit the command buffer
		VkSubmitInfo submitInfo{};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		VkSemaphore waitSemaphores[] = { mvkSemaphoreImageAvailable };
		VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
		submitInfo.waitSemaphoreCount = 1;
		submitInfo.pWaitSemaphores = waitSemaphores;
		submitInfo.pWaitDstStageMask = waitStages;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &mvkCommandBuffer;
		VkSemaphore signalSemaphores[] = { mvkSemaphoreRenderFinished };
		submitInfo.signalSemaphoreCount = 1;
		submitInfo.pSignalSemaphores = signalSemaphores;
		ThrowIfTrue(VK_SUCCESS != vkQueueSubmit(mvkGraphicsQueue, 1, &submitInfo, mvkFenceInFlight), "VulkanWindow::drawFrame() failed to submit draw command buffer.");

		// Present the frame to the window
		VkPresentInfoKHR presentInfo{};
		presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
		presentInfo.waitSemaphoreCount = 1;
		presentInfo.pWaitSemaphores = signalSemaphores;
		VkSwapchainKHR swapChains[] = { mvkSwapchain };
		presentInfo.swapchainCount = 1;
		presentInfo.pSwapchains = swapChains;
		presentInfo.pImageIndices = &imageIndex;
		presentInfo.pResults = nullptr; // Optional
		vkQueuePresentKHR(mvkPresentationQueue, &presentInfo);
	}

	LRESULT VulkanWindow::MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		switch (msg)
		{
		case WM_ACTIVATE:	// WM_ACTIVATE is sent when the window is activated or deactivated.
			if (LOWORD(wParam) == WA_INACTIVE)
			{

			}
			else
			{

			}
			return 0;
		case WM_DESTROY:	// WM_DESTROY is sent when the window is being destroyed.
			PostQuitMessage(0);
			return 0;
		case WM_SIZE:		// WM_SIZE is sent when the user resizes the window.  
			//			miWindowWidth = LOWORD(lParam);
			//			miWindowHeight = HIWORD(lParam);
			return 0;
		case WM_ENTERSIZEMOVE:	// WM_EXITSIZEMOVE is sent when the user grabs the resize bars.
			return 0;
		case WM_EXITSIZEMOVE:	// WM_EXITSIZEMOVE is sent when the user releases the resize bars.
			return 0;

		case WM_GETMINMAXINFO:	// Catch this message so to prevent the window from becoming too small.
			((MINMAXINFO*)lParam)->ptMinTrackSize.x = 1024;
			((MINMAXINFO*)lParam)->ptMinTrackSize.y = 576;
			return 0;
		case WM_DISPLAYCHANGE:
			return 0;
		}


		return DefWindowProc(hwnd, msg, wParam, lParam);
	}

	HWND VulkanWindow::getWindowHandle(void)
	{
		return mhWindowHandle;
	}

	HINSTANCE VulkanWindow::getApplicationInstance(void)
	{
		return mhInstance;
	}

	void VulkanWindow::setClearColour(glm::vec4& clearColour)
	{
		mv4ClearColour = clearColour;
	}
}