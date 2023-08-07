#pragma once
#include "PCH.h"
#include "resourceBase.h"
#include "matrix.h"
#include "vector2f.h"

namespace X
{
	class CResourceShader : public CResourceBase
	{
	public:
		CResourceShader(const std::string& strVertexProgramFilename, const std::string& strFragmentProgramFilename);
		~CResourceShader();

		// Create all OpenGL context dependent objects when an OpenGL context exists.
		void onGLContextCreated(void);

		// Free all OpenGL context dependent objects before the OpenGL context is destroyed.
		void onGLContextToBeDestroyed(void);

		// Bind the shader
		void bind(void) const;

		// Unbind the shader
		void unbind(void) const;

		// Set uniform (call after bind())
		void setBool(const std::string& name, bool value) const;

		// Set uniform (call after bind())
		// Usefull for telling OpenGL which sampler unit belongs to which uniform sampler2D name in the fragment program
		// Example...
		// Tell OpenGL, for each sampler, to which texture unit it belongs to
		// pShader->setInt("texture0", 0);
		// pShader->setInt("texture1", 1);
		void setInt(const std::string& name, int value) const;

		// Set uniform (call after bind())
		void setFloat(const std::string& name, float value) const;

		// Set named matrix (call after bind())
		void setMat4(const std::string& name, const glm::mat4& matrix) const;

		// Set named matrix (call after bind())
		void setMat4(const std::string& name, const CMatrix& matrix) const;

		// Set uniform vec2 (call after bind())
		void setVec2(const std::string& name, const glm::vec2& vec2) const;

		// Set uniform vec2 (call after bind())
		void setVec2(const std::string& name, const CVector2f& vec2) const;

		// Set uniform vec3 (call after bind())
		void setVec3(const std::string& name, const glm::vec3& vec3) const;

		// Set uniform vec3 (call after bind())
		void setVec3(const std::string& name, const CVector3f& vec3) const;

		// Set uniform vec3 from CColour, ignoring alpha (call after bind())
		void setVec3(const std::string& name, const CColour& colour) const;

		// Set uniform vec4 from CColour (call after bind())
		void setVec4(const std::string& name, const CColour& colour) const;

	private:
		std::string _mstrVertexShaderFilename;
		std::string _mstrFragmentShaderFilename;
		unsigned int _muiProgramID;
	};
}