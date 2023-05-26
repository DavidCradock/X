#include "PCH.h"
#include "graphicsPipelines.h"
#include "log.h"
#include "vulkanWindow.h"

namespace X
{
	GraphicsPipeline::GraphicsPipeline(void)
	{
	}

	GraphicsPipeline::~GraphicsPipeline(void)
	{
	}

	void GraphicsPipeline::load(void)
	{
		// Needed for logging...
		Log* pLog = Log::getPointer();

		// Make sure filenames exist
		ThrowIfFalse(bool(mstrVertexProgramFilename.size() > 0 && mstrFragmentProgramFilename.size() > 0), "GraphicsPipeline::load() failed as filenames of the GPU programs are not set.");

		// Get pointer to the Vulkan window as we need access to some of it's members
		VulkanWindow* pVulkanWindow = VulkanWindow::getPointer();
		
		// Get logical device as we'll use it below
		VkDevice vkLogicalDevice = pVulkanWindow->getLogicalDevice();

		// Well also need the render pass from the window...
		VkRenderPass vkRenderPass = pVulkanWindow->getRenderpass();

		// These will hold the shader modules for each of the GPU programs
		VkShaderModule vertexShaderModule;
		VkShaderModule fragmentShaderModule;

		// Attempt to create the shader module for the vertex program
		auto vertexCode = _readFile(mstrVertexProgramFilename);	// Read in shader code from file
		VkShaderModuleCreateInfo createInfoShaderModule{};
		createInfoShaderModule.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		createInfoShaderModule.codeSize = vertexCode.size();
		createInfoShaderModule.pCode = reinterpret_cast<const uint32_t*>(vertexCode.data());
		if (VK_SUCCESS != vkCreateShaderModule(vkLogicalDevice, &createInfoShaderModule, nullptr, &vertexShaderModule))
		{
			ThrowIfTrue(1, "GraphicsPipeline::load() failed to create shader module for " + mstrVertexProgramFilename);
		}

		// Attempt to create the shader module for the fragment program
		auto fragmentCode = _readFile(mstrFragmentProgramFilename);	// Read in shader code from file
		createInfoShaderModule.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		createInfoShaderModule.codeSize = fragmentCode.size();
		createInfoShaderModule.pCode = reinterpret_cast<const uint32_t*>(fragmentCode.data());
		if (VK_SUCCESS != vkCreateShaderModule(vkLogicalDevice, &createInfoShaderModule, nullptr, &fragmentShaderModule))
		{
			ThrowIfTrue(1, "GraphicsPipeline::load() failed to create shader module for " + mstrFragmentProgramFilename);
		}

		// Shaders need to be assigned to a pipeline stage, so create those
		VkPipelineShaderStageCreateInfo createInfoVertexPipelineShaderStage{};
		createInfoVertexPipelineShaderStage.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		createInfoVertexPipelineShaderStage.stage = VK_SHADER_STAGE_VERTEX_BIT;
		createInfoVertexPipelineShaderStage.module = vertexShaderModule;
		createInfoVertexPipelineShaderStage.pName = "main";

		VkPipelineShaderStageCreateInfo createInfoFragmentPipelineShaderStage{};
		createInfoFragmentPipelineShaderStage.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		createInfoFragmentPipelineShaderStage.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
		createInfoFragmentPipelineShaderStage.module = fragmentShaderModule;
		createInfoFragmentPipelineShaderStage.pName = "main";

		// Setup the fixed function pipeline

		// Setup how the vertex data will be passed to the vertex shader
		VkPipelineVertexInputStateCreateInfo createInfoPipelineVertexInputState{};
		createInfoPipelineVertexInputState.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
		createInfoPipelineVertexInputState.vertexBindingDescriptionCount = 0;
		createInfoPipelineVertexInputState.pVertexBindingDescriptions = nullptr; // Optional
		createInfoPipelineVertexInputState.vertexAttributeDescriptionCount = 0;
		createInfoPipelineVertexInputState.pVertexAttributeDescriptions = nullptr; // Optional

		// Input assembly
		// VK_PRIMITIVE_TOPOLOGY_POINT_LIST: points from vertices
		// VK_PRIMITIVE_TOPOLOGY_LINE_LIST: line from every 2 vertices without reuse
		// VK_PRIMITIVE_TOPOLOGY_LINE_STRIP : the end vertex of every line is used as start vertex for the next line
		// VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST : triangle from every 3 vertices without reuse
		// VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP : the second and third vertex of every triangle are used as first two vertices of the next triangle
		VkPipelineInputAssemblyStateCreateInfo createInfoPipelineInputAssemblyState{};
		createInfoPipelineInputAssemblyState.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
		createInfoPipelineInputAssemblyState.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
		createInfoPipelineInputAssemblyState.primitiveRestartEnable = VK_FALSE;

		// Viewport and scissor testing
		VkPipelineViewportStateCreateInfo createInfoPipelineViewportState{};
		createInfoPipelineViewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
		createInfoPipelineViewportState.viewportCount = 1;
		createInfoPipelineViewportState.scissorCount = 1;

		// Rasterizer performs converting tris into fragments, depth testing, face culling and scissor testing
		VkPipelineRasterizationStateCreateInfo createInfoRasterizationState{};
		createInfoRasterizationState.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
		createInfoRasterizationState.depthClampEnable = VK_FALSE;
		createInfoRasterizationState.rasterizerDiscardEnable = VK_FALSE;
		createInfoRasterizationState.polygonMode = VK_POLYGON_MODE_FILL;	// or VK_POLYGON_MODE_LINE or VK_POLYGON_MODE_POINT
		createInfoRasterizationState.lineWidth = 1.0f;
		createInfoRasterizationState.cullMode = VK_CULL_MODE_BACK_BIT;
		createInfoRasterizationState.frontFace = VK_FRONT_FACE_CLOCKWISE;
		createInfoRasterizationState.depthBiasEnable = VK_FALSE;

		// Cheaper than super-sampling (Rendering to a larger framebuffer and downsizing)
		// If enabled, we need to enable a GPU feature
		VkPipelineMultisampleStateCreateInfo createInfoMultisampleState{};
		createInfoMultisampleState.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
		createInfoMultisampleState.sampleShadingEnable = VK_FALSE;
		createInfoMultisampleState.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

		VkPipelineColorBlendAttachmentState pipelineColourBlendAttachmentState{};
		pipelineColourBlendAttachmentState.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
		pipelineColourBlendAttachmentState.blendEnable = VK_FALSE;

		// Blending
		VkPipelineColorBlendStateCreateInfo createInfoPipelineColourBlendState{};
		createInfoPipelineColourBlendState.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
		createInfoPipelineColourBlendState.logicOpEnable = VK_FALSE;
		createInfoPipelineColourBlendState.logicOp = VK_LOGIC_OP_COPY;
		createInfoPipelineColourBlendState.attachmentCount = 1;
		createInfoPipelineColourBlendState.pAttachments = &pipelineColourBlendAttachmentState;
		createInfoPipelineColourBlendState.blendConstants[0] = 0.0f;
		createInfoPipelineColourBlendState.blendConstants[1] = 0.0f;
		createInfoPipelineColourBlendState.blendConstants[2] = 0.0f;
		createInfoPipelineColourBlendState.blendConstants[3] = 0.0f;

		std::vector<VkDynamicState> dynamicStates = {
			VK_DYNAMIC_STATE_VIEWPORT,
			VK_DYNAMIC_STATE_SCISSOR
		};
		VkPipelineDynamicStateCreateInfo createInfoPipelineDynamicState{};
		createInfoPipelineDynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
		createInfoPipelineDynamicState.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
		createInfoPipelineDynamicState.pDynamicStates = dynamicStates.data();

		VkPipelineLayoutCreateInfo createInfoPipelineLayout{};
		createInfoPipelineLayout.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		createInfoPipelineLayout.setLayoutCount = 0;
		createInfoPipelineLayout.pushConstantRangeCount = 0;

		// Attempt to create the pipeline layout
		if (VK_SUCCESS != vkCreatePipelineLayout(vkLogicalDevice, &createInfoPipelineLayout, nullptr, &mvkPipelineLayout) != VK_SUCCESS)
			ThrowIfTrue(1, "GraphicsPipeline::load() failed to create pipeline layout for " + mstrVertexProgramFilename + " and " + mstrFragmentProgramFilename);

		// Create the graphics pipeline
		// First, place the vertex and fragment shader stages into an array...
		VkPipelineShaderStageCreateInfo shaderStages[] = { createInfoVertexPipelineShaderStage, createInfoFragmentPipelineShaderStage };
		VkGraphicsPipelineCreateInfo createInfoGraphicsPipeline{};
		createInfoGraphicsPipeline.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
		createInfoGraphicsPipeline.stageCount = 2;
		createInfoGraphicsPipeline.pStages = shaderStages;
		createInfoGraphicsPipeline.pVertexInputState = &createInfoPipelineVertexInputState;
		createInfoGraphicsPipeline.pInputAssemblyState = &createInfoPipelineInputAssemblyState;
		createInfoGraphicsPipeline.pViewportState = &createInfoPipelineViewportState;
		createInfoGraphicsPipeline.pRasterizationState = &createInfoRasterizationState;
		createInfoGraphicsPipeline.pMultisampleState = &createInfoMultisampleState;
		createInfoGraphicsPipeline.pDepthStencilState = nullptr; // Optional
		createInfoGraphicsPipeline.pColorBlendState = &createInfoPipelineColourBlendState;
		createInfoGraphicsPipeline.pDynamicState = &createInfoPipelineDynamicState;
		createInfoGraphicsPipeline.layout = mvkPipelineLayout;
		createInfoGraphicsPipeline.renderPass = vkRenderPass;
		createInfoGraphicsPipeline.subpass = 0;
		createInfoGraphicsPipeline.basePipelineHandle = VK_NULL_HANDLE; // Optional
		createInfoGraphicsPipeline.basePipelineIndex = -1; // Optional
		pLog->add("GraphicsPipeline::load() attempting to create graphics pipeline for " + mstrVertexProgramFilename + " and " + mstrFragmentProgramFilename + "...", false, true);
		ThrowIfTrue(VK_SUCCESS != vkCreateGraphicsPipelines(vkLogicalDevice, VK_NULL_HANDLE, 1, &createInfoGraphicsPipeline, nullptr, &mvkGraphicsPipeline), "GraphicsPipeline::load() failed to create graphics pipeline for " + mstrVertexProgramFilename + " and " + mstrFragmentProgramFilename);
		pLog->add(" done.", true, false);

		

		// Destroy the shader modules as they're no longer needed
		vkDestroyShaderModule(vkLogicalDevice, vertexShaderModule, nullptr);
		vkDestroyShaderModule(vkLogicalDevice, fragmentShaderModule, nullptr);
	}

	void GraphicsPipeline::unload(void)
	{
		VulkanWindow* pVulkanWindow = VulkanWindow::getPointer();
		VkDevice vkLogicalDevice = pVulkanWindow->getLogicalDevice();

		vkDestroyPipeline(vkLogicalDevice, mvkGraphicsPipeline, nullptr);
		vkDestroyPipelineLayout(vkLogicalDevice, mvkPipelineLayout, nullptr);
	}

	std::vector<char> GraphicsPipeline::_readFile(const std::string& filename)
	{
		// Open the file in binary mode and seek to the end of the file.
		std::ifstream file(filename, std::ios::ate | std::ios::binary);
		if (!file.is_open())
		{
			std::string strErr("GraphicsPipeline::_readFile(");
			strErr += filename + ") failed to open file.";
			ThrowIfTrue(1, strErr);
		}
		size_t fileSize = (size_t)file.tellg();
		std::vector<char> buffer(fileSize);
		file.seekg(0);
		file.read(buffer.data(), fileSize);
		file.close();
		return buffer;
	}

	void GraphicsPipeline::setFilenames(const std::string& strVertexProgram, const std::string& strFragmentProgram)
	{
		mstrVertexProgramFilename = strVertexProgram;
		mstrFragmentProgramFilename = strFragmentProgram;
	}

	GraphicsPipelineManager::GraphicsPipelineManager()
	{
		addNewGroup("default");
	}

	unsigned int GraphicsPipelineManager::getNumGroups(void)
	{
		return (unsigned int)mmapGroup.size();
	}

	unsigned int GraphicsPipelineManager::getNumResInGroup(const std::string& strGroupName)
	{
		if (!groupExists(strGroupName))
		{
			std::string err("GraphicsPipelineManager::getNumResInGroup(\"");
			err.append(strGroupName);
			err.append("\") failed. The group doesn't exist.");
			ThrowIfTrue(1, err);
		}
		std::map<std::string, Group*>::iterator itg = mmapGroup.find(strGroupName);
		return (unsigned int)itg->second->mmapResource.size();;
	}

	unsigned int GraphicsPipelineManager::getNumResInGroupLoaded(const std::string& strGroupName)
	{
		if (!groupExists(strGroupName))
		{
			std::string err("GraphicsPipelineManager::getNumResInGroupLoaded(\"");
			err.append(strGroupName);
			err.append("\") failed. The group doesn't exist.");
			ThrowIfTrue(1, err);
		}
		std::map<std::string, Group*>::iterator itg = mmapGroup.find(strGroupName);

		unsigned int iResLoadedTotal = 0;
		std::map<std::string, Resource*>::iterator it = itg->second->mmapResource.begin();
		while (it != itg->second->mmapResource.end())
		{
			if (it->second->bLoaded)
				++iResLoadedTotal;
			it++;
		}
		return iResLoadedTotal;
	}

	const std::string& GraphicsPipelineManager::getGroupName(unsigned int iGroupIndex)
	{
		if (iGroupIndex >= mmapGroup.size())
		{
			std::string err("GraphicsPipelineManager::getGroupName(");

			err.append(std::to_string(iGroupIndex));
			err.append(") failed. Invalid index given. Number of groups is ");
			err.append(std::to_string(getNumGroups()));
			ThrowIfTrue(1, err);
		}
		std::map<std::string, Group*>::iterator itg = mmapGroup.begin();
		unsigned int i = 0;
		while (i < iGroupIndex)
		{
			itg++;
			++i;
		}
		return itg->first;
	}

	void GraphicsPipelineManager::addNewGroup(const std::string& strNewGroupName)
	{
		if (groupExists(strNewGroupName))
		{
			return;
		}
		Group* pNewGroup = new Group;
		mmapGroup[strNewGroupName] = pNewGroup;
	}

	bool GraphicsPipelineManager::groupExists(const std::string& strGroupName)
	{
		std::map<std::string, Group*>::iterator it = mmapGroup.find(strGroupName);
		if (it == mmapGroup.end())
			return false;
		return true;
	}

	void GraphicsPipelineManager::loadGroup(const std::string& strGroupName)
	{
		// Group doesn't exist?
		if (!groupExists(strGroupName))
		{
			std::string err("GraphicsPipelineManager::loadGroup(\"");
			err.append(strGroupName);
			err.append("\") failed. As the given named group doesn't exist.");
			ThrowIfTrue(1, err);
		}

		// Load any unloaded resources within the group
		std::map<std::string, Group*>::iterator itg = mmapGroup.find(strGroupName);

		// For each resource in this group...
		std::map<std::string, Resource*>::iterator it = itg->second->mmapResource.begin();
		while (it != itg->second->mmapResource.end())
		{
			if (!it->second->bLoaded)
			{
				it->second->pResource->load();
				it->second->bLoaded = true;
			}
			it++;
		}
	}

	void GraphicsPipelineManager::loadGroupSingle(const std::string& strGroupName)
	{
		// Group doesn't exist?
		if (!groupExists(strGroupName))
		{
			std::string err("GraphicsPipelineManager::loadGroupSingle(\"");
			err.append(strGroupName);
			err.append("\") failed. As the given named group doesn't exist.");
			ThrowIfTrue(1, err);
		}

		// Load any unloaded resources within the group
		std::map<std::string, Group*>::iterator itg = mmapGroup.find(strGroupName);

		// For each resource in this group...
		std::map<std::string, Resource*>::iterator it = itg->second->mmapResource.begin();
		while (it != itg->second->mmapResource.end())
		{
			if (!it->second->bLoaded)
			{
				it->second->pResource->load();
				it->second->bLoaded = true;
				return;	// We've changed a resource from unloaded to loaded state, aka, we've loaded a resource, simply return.
			}
			it++;
		}
	}

	void GraphicsPipelineManager::unloadGroup(const std::string& strGroupName)
	{
		// Group doesn't exist?
		if (!groupExists(strGroupName))
		{
			std::string err("GraphicsPipelineManager::unloadGroup(\"");
			err.append(strGroupName);
			err.append("\") failed. As the given named group doesn't exist.");
			ThrowIfTrue(1, err);
		}

		// Unload any loaded resources within the group
		std::map<std::string, Group*>::iterator itg = mmapGroup.find(strGroupName);

		// For each resource in this group...
		std::map<std::string, Resource*>::iterator it = itg->second->mmapResource.begin();
		while (it != itg->second->mmapResource.end())
		{
			if (it->second->bLoaded)
			{
				it->second->pResource->unload();
				it->second->bLoaded = false;
			}
			it++;
		}
	}

	GraphicsPipeline* GraphicsPipelineManager::add(const std::string& strNewResourceName, const std::string& strGroupName)
	{
		// Group doesn't exist?
		if (!groupExists(strGroupName))
		{
			std::string err("GraphicsPipelineManager::add(\"");
			err.append(strNewResourceName);
			err.append("\", \"");
			err.append(strGroupName);
			err.append("\") failed. As the given named group of \"");
			err.append(strGroupName);
			err.append("\" which the new resource was to be placed into, doesn't exist.");
			ThrowIfTrue(1, err);
		}

		// Resource already exists in the group?
		std::map<std::string, Group*>::iterator itg = mmapGroup.find(strGroupName);								// Get iterator to the group (we know it exists)
		std::map<std::string, Resource*>::iterator it = itg->second->mmapResource.find(strNewResourceName);		// Try to find the named resource in the group
		if (itg->second->mmapResource.end() != it)
		{
			it->second->uiReferenceCount++;
			return it->second->pResource;
		}

		// If we get here, we have got to create, then add the resource to the existing named group
		Resource* pNewRes = new Resource;
		pNewRes->pResource = new GraphicsPipeline();
		pNewRes->uiReferenceCount = 1;
		pNewRes->bLoaded = false;
		itg->second->mmapResource[strNewResourceName] = pNewRes;
		return pNewRes->pResource;
	}

	GraphicsPipeline* GraphicsPipelineManager::get(const std::string& strResourceName, const std::string& strGroupName)
	{
		// Group doesn't exist?
		if (!groupExists(strGroupName))
		{
			std::string err("GraphicsPipelineManager::get(\"");
			err.append(strResourceName);
			err.append("\", \"");
			err.append(strGroupName);
			err.append("\") failed. As the given named group of \"");
			err.append(strGroupName);
			err.append("\" doesn't exist.");
			ThrowIfTrue(1, err);
		}

		// Resource doesn't exist in the group?
		std::map<std::string, Group*>::iterator itg = mmapGroup.find(strGroupName);							// Get iterator to the group (we know it exists)
		std::map<std::string, Resource*>::iterator it = itg->second->mmapResource.find(strResourceName);	// Try to find the named resource in the group
		if (itg->second->mmapResource.end() == it)
		{
			std::string err("GraphicsPipelineManager::get(\"");
			err.append(strResourceName);
			err.append("\", \"");
			err.append(strGroupName);
			err.append("\") failed. Although the given named group of \"");
			err.append(strGroupName);
			err.append("\" exists, the named resource couldn't be found.");
			ThrowIfTrue(1, err);
		}

		// Is the resource in an unloaded state?
		if (!it->second->bLoaded)
		{
			// Load it
			it->second->pResource->load();
			it->second->bLoaded = true;
		}
		// Return the resource pointer...
		return it->second->pResource;
	}

	bool GraphicsPipelineManager::getExists(const std::string& strResourceName, const std::string& strGroupName)
	{
		std::map<std::string, Group*>::iterator itg = mmapGroup.find(strGroupName);
		if (itg == mmapGroup.end())
			return false;
		std::map<std::string, Resource*>::iterator it = itg->second->mmapResource.find(strResourceName);
		if (it == itg->second->mmapResource.end())
			return false;
		return true;
	}

	void GraphicsPipelineManager::remove(const std::string& strResourceName, const std::string& strGroupName)
	{
		// Group doesn't exist?
		if (!groupExists(strGroupName))
		{
			std::string err("GraphicsPipelineManager::remove(\"");
			err.append(strResourceName);
			err.append("\", \"");
			err.append(strGroupName);
			err.append("\") failed. As the given named group of \"");
			err.append(strGroupName);
			err.append("\" which the resource is supposed to be in, doesn't exist.");
			ThrowIfTrue(1, err);
		}

		// Resource doesn't exist in the group?
		std::map<std::string, Group*>::iterator itg = mmapGroup.find(strGroupName);							// Get iterator to the group (we know it exists)
		std::map<std::string, Resource*>::iterator it = itg->second->mmapResource.find(strResourceName);	// Try to find the named resource in the group
		if (itg->second->mmapResource.end() == it)
		{
			std::string err("GraphicsPipelineManager::remove(\"");
			err.append(strResourceName);
			err.append("\", \"");
			err.append(strGroupName);
			err.append("\") failed. Although the given named group of \"");
			err.append(strGroupName);
			err.append("\" which the resource is supposed to be in, exists, the named resource couldn't be found.");
			ThrowIfTrue(1, err);
		}

		// If we get here, we've found the resource in the group
		// Decrement it's reference count 
		it->second->uiReferenceCount--;
		// If the reference count is now at zero
		if (it->second->uiReferenceCount <= 0)
		{
			// If it's in a loaded state, unload it
			if (it->second->bLoaded)
			{
				it->second->pResource->unload();
				it->second->bLoaded = false;
			}

			// Destroy the resource
			delete it->second->pResource;
			delete it->second;
			itg->second->mmapResource.erase(it);
		}
	}
}