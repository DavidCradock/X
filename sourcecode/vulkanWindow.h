#pragma once
#include "PCH.h"
#include "singleton.h"
#include "log.h"

namespace X
{
	struct QueueFamilyIndices {
		std::optional<uint32_t> graphicsFamily;
		std::optional<uint32_t> presentFamily;

		bool isComplete() {
			return graphicsFamily.has_value() && presentFamily.has_value();
		}
	};

	struct SwapChainSupportDetails {
		VkSurfaceCapabilitiesKHR capabilities;
		std::vector<VkSurfaceFormatKHR> formats;
		std::vector<VkPresentModeKHR> presentModes;
	};

	class VulkanWindow : public Singleton<VulkanWindow>
	{
	public:
		// Creates the window and initialises Vulkan
		void initialise(std::string strWindowTitle, int iWindowWidth, int iWindowHeight, bool bFullscreen);

		// Updates the window and checks for window close messages, returning false if the window has been asked to close
		bool update(void);

		// Closes the window and destroys all the Vulkan objects
		void shutdown(void);

		// Returns the window's handle
		HWND getWindowHandle(void);

		// Returns the Vulkan logical device
		VkDevice getLogicalDevice(void) { return device; }

		// Returns the render pass (Used by graphics pipelines)
		VkRenderPass getRenderpass(void) { return renderPass; }

		unsigned int getWindowWidth(void);
		unsigned int getWindowHeight(void);
		bool getWindowFullscreen(void);

		void initWindow(std::string strWindowTitle, int iWindowWidth, int iWindowHeight, bool bFullscreen);
		void initVulkan(void);

		unsigned int muiWindowWidth;
		unsigned int muiWindowHeight;
		bool mbWindowFullscreen;
	private:
		GLFWwindow* window;

		VkInstance instance;
		VkDebugUtilsMessengerEXT debugMessenger;
		VkSurfaceKHR surface;

		VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
		VkDevice device;

		VkQueue graphicsQueue;
		VkQueue presentQueue;

		VkSwapchainKHR swapChain;
		std::vector<VkImage> swapChainImages;
		VkFormat swapChainImageFormat;
		VkExtent2D swapChainExtent;
		std::vector<VkImageView> swapChainImageViews;
		std::vector<VkFramebuffer> swapChainFramebuffers;

		VkRenderPass renderPass;
		VkPipelineLayout pipelineLayout;
		VkPipeline graphicsPipeline;

		VkCommandPool commandPool;
		std::vector<VkCommandBuffer> commandBuffers;

		std::vector<VkSemaphore> imageAvailableSemaphores;
		std::vector<VkSemaphore> renderFinishedSemaphores;
		std::vector<VkFence> inFlightFences;
		uint32_t currentFrame = 0;

		void createInstance(void);
		void cleanupSwapChain(void);
		void cleanup(void);
		void recreateSwapChain(void);
		void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
		void setupDebugMessenger(void);
		void createSurface(void);
		void pickPhysicalDevice(void);
		void createLogicalDevice(void);
		void createSwapChain(void);
		void createImageViews(void);
		void createRenderPass(void);
		void createGraphicsPipeline(void);
		void createFramebuffers(void);
		void createCommandPool(void);
		void createCommandBuffers(void);
		void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);
		void createSyncObjects(void);
		void drawFrame(void);
		VkShaderModule createShaderModule(const std::vector<char>& code);
		VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
		VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
		VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
		SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);
		bool isDeviceSuitable(VkPhysicalDevice device);
		bool checkDeviceExtensionSupport(VkPhysicalDevice device);
		QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
		std::vector<const char*> getRequiredExtensions(void);
		bool checkValidationLayerSupport(void);
		static std::vector<char> readFile(const std::string& filename)
		{
			std::ifstream file(filename, std::ios::ate | std::ios::binary);

			if (!file.is_open()) {
				throw std::runtime_error("failed to open file!");
			}

			size_t fileSize = (size_t)file.tellg();
			std::vector<char> buffer(fileSize);

			file.seekg(0);
			file.read(buffer.data(), fileSize);

			file.close();

			return buffer;
		}

		public:
			bool framebufferResized = false;
		private:
		
	};
}
