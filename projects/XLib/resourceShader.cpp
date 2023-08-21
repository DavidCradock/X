#include "PCH.h"
#include "resourceShader.h"
#include "openGLExtensions.h"
#include "logging.h"

namespace X
{

	CResourceShader::CResourceShader(const std::string& strVertexProgramFilename, const std::string& strFragmentProgramFilename)
	{
        _mstrVertexShaderFilename = strVertexProgramFilename;
		_mstrFragmentShaderFilename = strFragmentProgramFilename;
        _muiProgramID = 0;

        // Create the program, compile shaders, link them to the program etc
        onGLContextCreated();
	}

	CResourceShader::~CResourceShader()
	{
        onGLContextToBeDestroyed();
	}

	void CResourceShader::onGLContextCreated(void)
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
            ThrowIfTrue(1, "CResourceShader::onGLContextCreated() failed to read in program code for either " + _mstrVertexShaderFilename + " or " + _mstrFragmentShaderFilename);
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
            std::string err("CResourceShader::onGLContextCreated() failed. ");
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
            std::string err("CResourceShader::onGLContextCreated() failed. ");
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
            std::string err("CResourceShader::onGLContextCreated() failed. ");
            err.append(infoLog);
            ThrowIfTrue(1, err);
        }

        // Delete the shaders as they're linked into our program now and no longer necessary
        glDeleteShader(vertex);
        glDeleteShader(fragment);
	}

    void CResourceShader::onGLContextToBeDestroyed(void)
    {
        glDeleteProgram(_muiProgramID);
        _muiProgramID = 0;
    }

    void CResourceShader::bind(void) const
    {
        glUseProgram(_muiProgramID);
    }

    void CResourceShader::unbind(void) const
    {
        glUseProgram(0);
    }

    void CResourceShader::setBool(const std::string& name, bool value) const
    {
        glUniform1i(glGetUniformLocation(_muiProgramID, name.c_str()), (int)value);
    }

    void CResourceShader::setInt(const std::string& name, int value) const
    {
        glUniform1i(glGetUniformLocation(_muiProgramID, name.c_str()), value);
    }

    void CResourceShader::setFloat(const std::string& name, float value) const
    {
        glUniform1f(glGetUniformLocation(_muiProgramID, name.c_str()), value);
    }

    void CResourceShader::setMat4(const std::string& name, const glm::mat4& matrix) const
    {
        glUniformMatrix4fv(glGetUniformLocation(_muiProgramID, name.c_str()), 1, GL_FALSE, glm::value_ptr(matrix));
    }

    void CResourceShader::setMat4(const std::string& name, const CMatrix& matrix) const
    {
        glUniformMatrix4fv(glGetUniformLocation(_muiProgramID, name.c_str()), 1, GL_FALSE, matrix.getFloat());
    }

    void CResourceShader::setVec2(const std::string& name, const glm::vec2& vec2) const
    {
        glUniform2fv(glGetUniformLocation(_muiProgramID, name.c_str()), 1, &vec2[0]);
    }

    void CResourceShader::setVec2(const std::string& name, const CVector2f& vec2) const
    {
        float array[2];
        vec2.getAsArray(array);
        glUniform2fv(glGetUniformLocation(_muiProgramID, name.c_str()), 1, &array[0]);
    }

    void CResourceShader::setVec3(const std::string& name, const glm::vec3& vec3) const
    {
        glUniform3fv(glGetUniformLocation(_muiProgramID, name.c_str()), 1, &vec3[0]);
    }

    void CResourceShader::setVec3(const std::string& name, const CVector3f& vec3) const
    {
        float array[3];
        vec3.getAsArray(array);
        glUniform3fv(glGetUniformLocation(_muiProgramID, name.c_str()), 1, &array[0]);
    }

    void CResourceShader::setVec3(const std::string& name, const CColour& colour) const
    {
        float array[3];
        array[0] = colour.red;
        array[1] = colour.green;
        array[2] = colour.blue;
        glUniform3fv(glGetUniformLocation(_muiProgramID, name.c_str()), 1, &array[0]);
    }

    void CResourceShader::setVec4(const std::string& name, const CColour& colour) const
    {
        float array[4];
        array[0] = colour.red;
        array[1] = colour.green;
        array[2] = colour.blue;
        array[3] = colour.alpha;
        glUniform4fv(glGetUniformLocation(_muiProgramID, name.c_str()), 1, &array[0]);
    }
}