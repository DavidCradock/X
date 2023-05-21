#include "PCH.h"
#include "window.h"
#include "log.h"

namespace X
{
	LRESULT CALLBACK MainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		return Window::getPointer()->MsgProc(hwnd, msg, wParam, lParam);
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
			pLog->add("Vulkan debug message: " + strError);
			ThrowIfTrue(1, strError);
		}
		return VK_FALSE;
	}

	Window::Window()
	{
		mhInstance = NULL;
		mhWindowHandle = NULL;
		miWindowWidth = 1024;
		miWindowHeight = 576;

		mvkPhysicalDevice = VK_NULL_HANDLE;
	}

	void Window::initialise(std::string strWindowTitle, int iWindowWidth, int iWindowHeight)
	{
		Log* pLog = Log::getPointer();
		pLog->add("Window::initialise() called.");

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

		ThrowIfFalse(RegisterClass(&mWindowClass), "Window::initialise() failed to register window class.");

		RECT R = { 0, 0, miWindowWidth, miWindowHeight };
		AdjustWindowRect(&R, WS_OVERLAPPEDWINDOW, false);
		int width = R.right - R.left;
		int height = R.bottom - R.top;

		mhWindowHandle = CreateWindow(L"XWindowClassName", StringToWString(mstrWindowTitle).c_str(), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, width, height, 0, 0, mhInstance, 0);
		ThrowIfFalse(mhWindowHandle, "Window::initialise() failed to create window.");

		// Center window
		RECT rectDesktop;
		GetClientRect(GetDesktopWindow(), &rectDesktop);
		int iPosLeft = (rectDesktop.right / 2) - (width / 2);
		int iPosTop = (rectDesktop.bottom / 2) - (height / 2);
		SetWindowPos(mhWindowHandle, HWND_TOP, iPosLeft, iPosTop, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
		ShowWindow(mhWindowHandle, SW_SHOW);
		UpdateWindow(mhWindowHandle);
		pLog->add("Window::initialise() has created the application window.");

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
		
		// Get supported extensions 
		uint32_t extensionCount = 0;
		vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
		std::vector<std::string> supportedInstanceExtensions;
		if (extensionCount > 0)
		{
			pLog->add("Window::initialise() has detected " + std::to_string(extensionCount) + " extensions.");
			std::vector<VkExtensionProperties> extensions(extensionCount);
			if (VK_SUCCESS == vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, &extensions.front()))
			{
				for (VkExtensionProperties extension : extensions)
				{
					supportedInstanceExtensions.push_back(extension.extensionName);
				}
			}
		}
		for (int i = 0; i < supportedInstanceExtensions.size(); ++i)
		{
			pLog->add("Window::initialise() extension name: " + supportedInstanceExtensions[i]);
		}

		// Add required extensions
		std::vector<const char*> instanceExtensions = { VK_KHR_SURFACE_EXTENSION_NAME };
		instanceExtensions.push_back(VK_KHR_WIN32_SURFACE_EXTENSION_NAME);

		// Add debug tools if in debug AND they are available (Due to the VulkanSDK being installed)
		if (bValidationLayersInUse)	// If we're in DEBUG build
		{
			// Find if they're available
			if (std::find(supportedInstanceExtensions.begin(), supportedInstanceExtensions.end(), VK_EXT_DEBUG_UTILS_EXTENSION_NAME) != supportedInstanceExtensions.end())
			{
				// Add the extension to the required extensions
				instanceExtensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
			}
		}

		// We can just attempt to create the device attempting to use the above requested extensions
		// and then if it fails, bail out, but it'd be nice to see which extension isn't supported by
		// comparing and attempting to find whether each extension in instanceExtensions is found in
		// supportedInstanceExtensions, so let's do that...
		for (int i = 0; i < instanceExtensions.size(); ++i)
		{
			// Attempt to find the extension in supportedInstanceExtensions
			if (std::find(supportedInstanceExtensions.begin(), supportedInstanceExtensions.end(), instanceExtensions[i]) == supportedInstanceExtensions.end())
			{
				std::string strError("Window::initialise() failed to find required extension: ");
				strError += instanceExtensions[i];
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
				pLog->add("Window::initialise() has enabled the debug layers.");
				vkInstanceCreateInfo.enabledLayerCount = 1;
				vkInstanceCreateInfo.ppEnabledLayerNames = &validationLayerName;
			}
		}

		vkInstanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		vkInstanceCreateInfo.pNext = nullptr;
		vkInstanceCreateInfo.flags = 0;
		vkInstanceCreateInfo.pApplicationInfo = &vkApplicationInfo;
		vkInstanceCreateInfo.enabledExtensionCount = (uint32_t)instanceExtensions.size();	// Number of global extensions to enable
		vkInstanceCreateInfo.ppEnabledExtensionNames = instanceExtensions.data();

		// Attempt to create the Vulkan instance
		pLog->add("Window::initialise() attempting to create main Vulkan instance.");
		VkResult result = vkCreateInstance(&vkInstanceCreateInfo, nullptr, &mvkInstance);
		ThrowIfFalse(bool(VK_SUCCESS == result), "Window::initialise() failed to create the main Vulkan instance.");
		pLog->add("Window::initialise() main Vulkan instance created.");

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
				ThrowIfTrue(VK_SUCCESS != func(mvkInstance, &createInfo, nullptr, &debugMessenger), "Window::initialise() failed whilst setting up debug function.");
			}
			else
			{
				ThrowIfTrue(1, "Window::initialise() unable to obtain vkCreateDebugUtilsMessengerExt function.");
			}
		}
		
		_initPhysicalDevice();

		// Get each Queue family of the physical device and make sure the VK_QUEUE_GRAPHICS_BIT is set for one of them.
		// Also, store the index to the graphics queue so we can use it below when parsing the VkDeviceQueueCreateInfo.queueFamilyIndex
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
		ThrowIfFalse(graphicsFamily.has_value(), "Window::initialise() was unable to find the VK_QUEUE_GRAPHICS_BIT for the chosen physical Vulkan device.");

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
		// Set debug layer info from the physical device's creation structure
		// Apparantly, these are ignored by more recent API versions, but we'll add it anyway, just for backwards compatibility.
		createInfo.enabledLayerCount = vkInstanceCreateInfo.enabledLayerCount;
		createInfo.ppEnabledLayerNames = vkInstanceCreateInfo.ppEnabledLayerNames;
		// Attempt to create the logical device
		ThrowIfTrue(VK_SUCCESS != vkCreateDevice(mvkPhysicalDevice, &createInfo, nullptr, &mvkLogicalDevice), "Window::initialise() was unable to create the Vulkan logical device.");
		// Obtain the graphics queue
		vkGetDeviceQueue(mvkLogicalDevice, graphicsFamily.value(), 0, &mvkGraphicsQueue);
		pLog->add("Window::initialise() has created the Vulkan logical device and obtained the graphics queue.");

		// Now create the window surface which Vulkan will render to and present in the window
		VkWin32SurfaceCreateInfoKHR createInfoWindowSurface{};
		createInfoWindowSurface.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
		createInfoWindowSurface.hwnd = mhWindowHandle;
		createInfoWindowSurface.hinstance = mhInstance;
		ThrowIfTrue(VK_SUCCESS != vkCreateWin32SurfaceKHR(mvkInstance, &createInfoWindowSurface, nullptr, &mvkWindowSurface), "Window::initialise() failed during call to vkCreateWin32SurfaceKHR()");
		pLog->add("Window::initialise() has created the window surface.");


	}

	void Window::_initPhysicalDevice(void)
	{
		Log* pLog = Log::getPointer();
		std::string strLogPrefix = "Window::_initPhysicalDevice() ";
		pLog->add(strLogPrefix + "called.");

		// Find how many physical devices there are, if none, throw stuff around have a little tantrum.
		uint32_t uiDeviceCount = 0;
		vkEnumeratePhysicalDevices(mvkInstance, &uiDeviceCount, nullptr);
		pLog->add("Window::_initPhysicalDevice() found " + std::to_string(uiDeviceCount) + " physical devices.");
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

	int Window::_initComputePhysicalDeviceScore(VkPhysicalDevice vkPhysicalDevice)
	{
		int iScore = 0;

		// Get physical device properties
		VkPhysicalDeviceProperties vkDeviceProperties;
		vkGetPhysicalDeviceProperties(vkPhysicalDevice, &vkDeviceProperties);

		// Log physical device properties
		Log* pLog = Log::getPointer();
		pLog->add("_initComputePhysicalDeviceScore() Vulkan Physical Device Name: " + std::string(vkDeviceProperties.deviceName));
		
		// Get physical device features
		VkPhysicalDeviceFeatures vkDeviceFeatures;
		vkGetPhysicalDeviceFeatures(vkPhysicalDevice, &vkDeviceFeatures);
		if (vkDeviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU)
		{
			pLog->add("_initComputePhysicalDeviceScore() Vulkan Physical Device Type: Virtual GPU");
			iScore += 1;
		}
		if (vkDeviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU)
		{
			pLog->add("_initComputePhysicalDeviceScore() Vulkan Physical Device Type: Integrated GPU");
			iScore += 1000;
		}
		if (vkDeviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
		{
			pLog->add("_initComputePhysicalDeviceScore() Vulkan Physical Device Type: Discrete GPU");
			iScore += 100000;
		}

		// Nice to have features
		// Nice to have larger texture dimensions
		iScore += vkDeviceProperties.limits.maxImageDimension2D;
		pLog->add("_initComputePhysicalDeviceScore() Vulkan Physical Device maxImageDimension2D: " + std::to_string(vkDeviceProperties.limits.maxImageDimension2D));

		// Check features we NEED
		if (!vkDeviceFeatures.geometryShader)
			return 0;

		return iScore;
	}

	bool Window::update(void)
	{
		return true;
	}

	void Window::shutdown(void)
	{
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
				func(mvkInstance, debugMessenger, nullptr);
			}
			else
			{
				ThrowIfTrue(1, "Window::shutdown() unable to obtain function pointer to vkDestroyDebugUtilsMessengerEXT function.");
			}
		}

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

	LRESULT Window::MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
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

	HWND Window::getWindowHandle(void)
	{
		return mhWindowHandle;
	}
}