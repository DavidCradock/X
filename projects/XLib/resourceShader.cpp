#include "PCH.h"
#include "resourceShader.h"
#include "openGLExtensions.h"
#include "log.h"

namespace X
{

	ResourceShader::ResourceShader(const std::string& strVertexProgramFilename, const std::string& strFragmentProgramFilename)
	{
        _mstrVertexShaderFilename = strVertexProgramFilename;
		_mstrFragmentShaderFilename = strFragmentProgramFilename;
        _muiProgramID = 0;

        // Create the program, compile shaders, link them to the program etc
        onGLContextCreated();
	}

	ResourceShader::~ResourceShader()
	{
        onGLContextToBeDestroyed();
	}

	void ResourceShader::onGLContextCreated(void)
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
            vShaderFile.open(_mstrVertexShaderFilename);
            fShaderFile.open(_mstrFragmentShaderFilename);
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
            ThrowIfTrue(1, "ResourceShader::onGLContextCreated() failed to read in program code for either " + _mstrVertexShaderFilename + " or " + _mstrFragmentShaderFilename);
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
            std::string err("ResourceShader::onGLContextCreated() failed. ");
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
            std::string err("ResourceShader::onGLContextCreated() failed. ");
            err.append(infoLog);
            ThrowIfTrue(1, err);
        };

        // Shader Program
        _muiProgramID = glCreateProgram();
        glAttachShader(_muiProgramID, vertex);
        glAttachShader(_muiProgramID, fragment);
        glLinkProgram(_muiProgramID);

        // Show linking errors if any
        glGetProgramiv(_muiProgramID, GL_LINK_STATUS, &success);
        if (!success)
        {
            std::string err("ResourceShader::onGLContextCreated() failed. ");
            err.append(infoLog);
            ThrowIfTrue(1, err);
        }

        // Delete the shaders as they're linked into our program now and no longer necessary
        glDeleteShader(vertex);
        glDeleteShader(fragment);
	}

    void ResourceShader::onGLContextToBeDestroyed(void)
    {
        glDeleteProgram(_muiProgramID);
        _muiProgramID = 0;
    }

    void ResourceShader::bind(void)
    {
        glUseProgram(_muiProgramID);
    }

    void ResourceShader::unbind(void)
    {
        glUseProgram(0);
    }

    void ResourceShader::setBool(const std::string& name, bool value)
    {
        glUniform1i(glGetUniformLocation(_muiProgramID, name.c_str()), (int)value);
    }

    void ResourceShader::setInt(const std::string& name, int value)
    {
        glUniform1i(glGetUniformLocation(_muiProgramID, name.c_str()), value);
    }

    void ResourceShader::setFloat(const std::string& name, float value)
    {
        glUniform1f(glGetUniformLocation(_muiProgramID, name.c_str()), value);
    }

    void ResourceShader::setMat4(const std::string& name, const glm::mat4& matrix)
    {
        glUniformMatrix4fv(glGetUniformLocation(_muiProgramID, name.c_str()), 1, GL_FALSE, glm::value_ptr(matrix));
    }

    void ResourceShader::setVec3(const std::string& name, const glm::vec3& vec3)
    {
        glUniform3fv(glGetUniformLocation(_muiProgramID, name.c_str()), 1, &vec3[0]);
    }
}