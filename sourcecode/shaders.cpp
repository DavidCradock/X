#include "PCH.h"
#include "shaders.h"
#include "openGLExtensions.h"
#include "log.h"

namespace X
{
    Shader::Shader()
    {
        ID = 0;
    }

	void Shader::setFilenames(const std::string& vertexShaderFilenameIn, const std::string& fragmentShaderFilenameIn)
	{
		vertexShaderFilename = vertexShaderFilenameIn;
		fragmentShaderFilename = fragmentShaderFilenameIn;
	}

    void Shader::load(void)
    {
        // Load in source code
        std::string vertexCode;
        std::string fragmentCode;
        std::ifstream vShaderFile;
        std::ifstream fShaderFile;
        // ensure ifstream objects can throw exceptions:
        vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        try
        {
            vShaderFile.open(vertexShaderFilename);
            fShaderFile.open(fragmentShaderFilename);
            std::stringstream vShaderStream, fShaderStream;
            // Read file's buffer contents into streams
            vShaderStream << vShaderFile.rdbuf();
            fShaderStream << fShaderFile.rdbuf();
            vShaderFile.close();
            fShaderFile.close();
            // convert stream into string
            vertexCode = vShaderStream.str();
            fragmentCode = fShaderStream.str();
        }
        catch (std::ifstream::failure e)
        {
			ThrowIfTrue(1, "Shader::compile() failed to read in program code for either " + vertexShaderFilename + " or " + fragmentShaderFilename);
        }
        const char* vShaderCode = vertexCode.c_str();
        const char* fShaderCode = fragmentCode.c_str();

        // Attempt to compile shaders
        unsigned int vertex, fragment;
        int success;
        char infoLog[512];

        // Vertex Shader
        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vShaderCode, NULL);
        glCompileShader(vertex);
        glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(vertex, 512, NULL, infoLog);
            std::string err("Shader::compile() failed. ");
            err.append(infoLog);
            ThrowIfTrue(1, err);
        };

        // Fragment Shader
        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fShaderCode, NULL);
        glCompileShader(fragment);
        glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(fragment, 512, NULL, infoLog);
            std::string err("Shader::compile() failed. ");
            err.append(infoLog);
			ThrowIfTrue(1, err);
        };

        // Shader Program
        ID = glCreateProgram();
        glAttachShader(ID, vertex);
        glAttachShader(ID, fragment);
        glLinkProgram(ID);

        // Show linking errors if any
        glGetProgramiv(ID, GL_LINK_STATUS, &success);
        if (!success)
        {
            std::string err("Shader::compile() failed. ");
            err.append(infoLog);
			ThrowIfTrue(1, err);
        }

        // Delete the shaders as they're linked into our program now and no longer necessary
        glDeleteShader(vertex);
        glDeleteShader(fragment);
    }

    void Shader::unload(void)
    {
        glDeleteProgram(ID);
        ID = 0;
    }

    void Shader::bind(void)
    {
        glUseProgram(ID);
    }

    void Shader::unbind(void)
    {
        glUseProgram(0);
    }

    void Shader::setBool(const std::string& name, bool value)
    {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
    }

    void Shader::setInt(const std::string& name, int value)
    {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
    }

    void Shader::setFloat(const std::string& name, float value)
    {
        glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
    }

    void Shader::setMat4(const std::string& name, const glm::mat4& matrix)
    {
        glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(matrix));
    }

	ShaderManager::ShaderManager()
	{
		addNewGroup("default");
	}

	unsigned int ShaderManager::getNumGroups(void)
	{
		return (unsigned int)mmapGroup.size();
	}

	unsigned int ShaderManager::getNumResInGroup(const std::string& strGroupName)
	{
		if (!groupExists(strGroupName))
		{
			std::string err("ShaderManager::getNumResInGroup(\"");
			err.append(strGroupName);
			err.append("\") failed. The group doesn't exist.");
			ThrowIfTrue(1, err);
		}
		std::map<std::string, Group*>::iterator itg = mmapGroup.find(strGroupName);
		return (unsigned int)itg->second->mmapResource.size();;
	}

	unsigned int ShaderManager::getNumResInGroupLoaded(const std::string& strGroupName)
	{
		if (!groupExists(strGroupName))
		{
			std::string err("ShaderManager::getNumResInGroupLoaded(\"");
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

	const std::string& ShaderManager::getGroupName(unsigned int iGroupIndex)
	{
		if (iGroupIndex >= mmapGroup.size())
		{
			std::string err("ShaderManager::getGroupName(");

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

	void ShaderManager::addNewGroup(const std::string& strNewGroupName)
	{
		if (groupExists(strNewGroupName))
		{
			return;
		}
		Group* pNewGroup = new Group;
		mmapGroup[strNewGroupName] = pNewGroup;
	}

	bool ShaderManager::groupExists(const std::string& strGroupName)
	{
		std::map<std::string, Group*>::iterator it = mmapGroup.find(strGroupName);
		if (it == mmapGroup.end())
			return false;
		return true;
	}

	void ShaderManager::loadGroup(const std::string& strGroupName)
	{
		// Group doesn't exist?
		if (!groupExists(strGroupName))
		{
			std::string err("ShaderManager::loadGroup(\"");
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

	void ShaderManager::loadGroupSingle(const std::string& strGroupName)
	{
		// Group doesn't exist?
		if (!groupExists(strGroupName))
		{
			std::string err("ShaderManager::loadGroupSingle(\"");
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

	void ShaderManager::unloadGroup(const std::string& strGroupName)
	{
		// Group doesn't exist?
		if (!groupExists(strGroupName))
		{
			std::string err("ShaderManager::unloadGroup(\"");
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

	Shader* ShaderManager::add(const std::string& strNewResourceName, const std::string& strGroupName)
	{
		// Group doesn't exist?
		if (!groupExists(strGroupName))
		{
			std::string err("ShaderManager::add(\"");
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
		pNewRes->pResource = new Shader;
		pNewRes->uiReferenceCount = 1;
		pNewRes->bLoaded = false;
		itg->second->mmapResource[strNewResourceName] = pNewRes;
		return pNewRes->pResource;
	}

	Shader* ShaderManager::get(const std::string& strResourceName, const std::string& strGroupName)
	{
		// Group doesn't exist?
		if (!groupExists(strGroupName))
		{
			std::string err("ShaderManager::get(\"");
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
			std::string err("ShaderManager::get(\"");
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

	bool ShaderManager::getExists(const std::string& strResourceName, const std::string& strGroupName)
	{
		std::map<std::string, Group*>::iterator itg = mmapGroup.find(strGroupName);
		if (itg == mmapGroup.end())
			return false;
		std::map<std::string, Resource*>::iterator it = itg->second->mmapResource.find(strResourceName);
		if (it == itg->second->mmapResource.end())
			return false;
		return true;
	}

	void ShaderManager::remove(const std::string& strResourceName, const std::string& strGroupName)
	{
		// Group doesn't exist?
		if (!groupExists(strGroupName))
		{
			std::string err("ShaderManager::remove(\"");
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
			std::string err("ShaderManager::remove(\"");
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