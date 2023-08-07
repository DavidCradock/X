#include "PCH.h"
#include "log.h"

namespace X
{
	// VSync
	PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT;
	// Multi texturing
	PFNGLACTIVETEXTUREPROC glActiveTexture;
//	PFNGLCLIENTACTIVETEXTUREARBPROC glClientActiveTexture;
//	PFNGLMULTITEXCOORD1FARBPROC glMultiTexCoord1f;
//	PFNGLMULTITEXCOORD2FARBPROC glMultiTexCoord2f;
//	PFNGLMULTITEXCOORD3FARBPROC glMultiTexCoord3f;
	// Frame buffer
	PFNGLGENFRAMEBUFFERSPROC glGenFramebuffers;
	PFNGLBINDFRAMEBUFFERPROC glBindFramebuffer;
	PFNGLDELETEFRAMEBUFFERSPROC glDeleteFramebuffers;
	PFNGLFRAMEBUFFERTEXTURE2DPROC glFramebufferTexture2D;
	// Render buffer
	PFNGLGENRENDERBUFFERSPROC glGenRenderbuffers;
	PFNGLBINDRENDERBUFFERPROC glBindRenderbuffer;
	PFNGLRENDERBUFFERSTORAGEPROC glRenderbufferStorage;
	PFNGLFRAMEBUFFERRENDERBUFFERPROC glFramebufferRenderbuffer;
	PFNGLCHECKFRAMEBUFFERSTATUSPROC glCheckFramebufferStatus;
	PFNGLDELETERENDERBUFFERSPROC glDeleteRenderbuffers;
	// Generating mip maps for textures bound to frame buffers
	PFNGLGENERATEMIPMAPPROC glGenerateMipmap;
	// Programmable pipeline (Shaders)
	PFNGLCREATESHADERPROC glCreateShader;
	PFNGLSHADERSOURCEPROC glShaderSource;
	PFNGLCOMPILESHADERPROC glCompileShader;
	// Programmable pipeline (Programs)
	PFNGLCREATEPROGRAMPROC glCreateProgram;
	PFNGLATTACHSHADERPROC glAttachShader;
	PFNGLLINKPROGRAMPROC glLinkProgram;
	PFNGLUSEPROGRAMPROC glUseProgram;
	// Programmable pipeline (Debugging)
	PFNGLGETSHADERIVPROC glGetShaderiv;
	PFNGLGETPROGRAMIVPROC glGetProgramiv;
	PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog;
	PFNGLGETPROGRAMINFOLOGPROC glGetProgramInfoLog;
	// Programmable pipeline (Cleanup)
	PFNGLDETACHSHADERPROC glDetachShader;
	PFNGLDELETESHADERPROC glDeleteShader;
	PFNGLDELETEPROGRAMPROC glDeleteProgram;
	// Programmable pipeline (Uniform variables)
	PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation;
	PFNGLUNIFORM1FPROC glUniform1f;
	PFNGLUNIFORM2FPROC glUniform2f;
	PFNGLUNIFORM3FPROC glUniform3f;
	PFNGLUNIFORM4FPROC glUniform4f;
	PFNGLUNIFORM1FVPROC glUniform1fv;
	PFNGLUNIFORM2FVPROC glUniform2fv;
	PFNGLUNIFORM3FVPROC glUniform3fv;
	PFNGLUNIFORM4FVPROC glUniform4fv;
	PFNGLUNIFORM1IPROC glUniform1i;
	PFNGLUNIFORM2IPROC glUniform2i;
	PFNGLUNIFORM3IPROC glUniform3i;
	PFNGLUNIFORM4IPROC glUniform4i;
	PFNGLUNIFORM1IVPROC glUniform1iv;
	PFNGLUNIFORM2IVPROC glUniform2iv;
	PFNGLUNIFORM3IVPROC glUniform3iv;
	PFNGLUNIFORM4IVPROC glUniform4iv;
	PFNGLUNIFORMMATRIX2FVPROC glUniformMatrix2fv;
	PFNGLUNIFORMMATRIX3FVPROC glUniformMatrix3fv;
	PFNGLUNIFORMMATRIX4FVPROC glUniformMatrix4fv;
	// Programmable pipeline (Attribute variables)
	PFNGLGETATTRIBLOCATIONPROC glGetAttribLocation;
	PFNGLVERTEXATTRIB1FPROC glVertexAttrib1f;
	PFNGLVERTEXATTRIB2FPROC glVertexAttrib2f;
	PFNGLVERTEXATTRIB3FPROC glVertexAttrib3f;
	PFNGLVERTEXATTRIB4FPROC glVertexAttrib4f;
	PFNGLVERTEXATTRIB1FVPROC glVertexAttrib1fv;
	PFNGLVERTEXATTRIB2FVPROC glVertexAttrib2fv;
	PFNGLVERTEXATTRIB3FVPROC glVertexAttrib3fv;
	PFNGLVERTEXATTRIB4FVPROC glVertexAttrib4fv;
	PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray;
	PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer;
	// Vertex buffer objects
	PFNGLGENBUFFERSPROC glGenBuffers;
	PFNGLBINDBUFFERPROC glBindBuffer;
	PFNGLDELETEBUFFERSPROC glDeleteBuffers;
	PFNGLBUFFERDATAARBPROC glBufferData;
	// Vertex arrays
	PFNGLGENVERTEXARRAYSPROC glGenVertexArrays;
	PFNGLBINDVERTEXARRAYPROC glBindVertexArray;
	PFNGLDELETEVERTEXARRAYSPROC glDeleteVertexArrays;
	// Instancing
	PFNGLDRAWELEMENTSINSTANCEDPROC glDrawElementsInstanced;
	PFNGLVERTEXATTRIBDIVISORPROC glVertexAttribDivisor;

	void checkOpenGLExtension(void* extension, const std::string strExtensionName, bool bLogExtensions)
	{
		std::string strError = "Extension name: " + strExtensionName + " not found.";
		ThrowIfFalse(extension, strError);

		if (bLogExtensions)
		{
			std::string strFound = "Extension name: " + strExtensionName + " found.";
			CLog::getPointer()->add(strFound);
		}
	}

	void setupOpenGLExtensions(bool bLogExtensions)
	{
		static bool bDoneThisBefore = false;
		if (bDoneThisBefore)
			return;
		bDoneThisBefore = true;

		CLog* pLog = CLog::getPointer();
		pLog->add("setupOpenGLExtensions() called...");

		// VSync
		wglSwapIntervalEXT = (PFNWGLSWAPINTERVALEXTPROC)wglGetProcAddress("wglSwapIntervalEXT");	checkOpenGLExtension(wglSwapIntervalEXT, "wglSwapIntervalEXT", bLogExtensions);
		// Multi texturing
		glActiveTexture = (PFNGLACTIVETEXTUREPROC)wglGetProcAddress("glActiveTexture");	checkOpenGLExtension(glActiveTexture, "glActiveTexture", bLogExtensions);
//		glClientActiveTexture = (PFNGLCLIENTACTIVETEXTUREARBPROC)wglGetProcAddress("glClientActiveTexture");	checkOpenGLExtension(glClientActiveTexture, "glClientActiveTexture", bLogExtensions);
//		glMultiTexCoord1f = (PFNGLMULTITEXCOORD1FARBPROC)wglGetProcAddress("glMultiTexCoord1f");	checkOpenGLExtension(glMultiTexCoord1f, "glMultiTexCoord1f", bLogExtensions);
//		glMultiTexCoord2f = (PFNGLMULTITEXCOORD2FARBPROC)wglGetProcAddress("glMultiTexCoord2f");	checkOpenGLExtension(glMultiTexCoord2f, "glMultiTexCoord2f", bLogExtensions);
//		glMultiTexCoord3f = (PFNGLMULTITEXCOORD3FARBPROC)wglGetProcAddress("glMultiTexCoord3f");	checkOpenGLExtension(glMultiTexCoord3f, "glMultiTexCoord3f", bLogExtensions);
		// Frame buffer
		glGenFramebuffers = (PFNGLGENFRAMEBUFFERSPROC)wglGetProcAddress("glGenFramebuffers");	checkOpenGLExtension(glGenFramebuffers, "glGenFramebuffers", bLogExtensions);
		glBindFramebuffer = (PFNGLBINDFRAMEBUFFERPROC)wglGetProcAddress("glBindFramebuffer");	checkOpenGLExtension(glBindFramebuffer, "glBindFramebuffer", bLogExtensions);
		glDeleteFramebuffers = (PFNGLDELETEFRAMEBUFFERSPROC)wglGetProcAddress("glDeleteFramebuffers");	checkOpenGLExtension(glDeleteFramebuffers, "glDeleteFramebuffers", bLogExtensions);
		glFramebufferTexture2D = (PFNGLFRAMEBUFFERTEXTURE2DPROC)wglGetProcAddress("glFramebufferTexture2D");	checkOpenGLExtension(glFramebufferTexture2D, "glFramebufferTexture2D", bLogExtensions);
		// Render buffer
		glGenRenderbuffers = (PFNGLGENRENDERBUFFERSEXTPROC)wglGetProcAddress("glGenRenderbuffers");	checkOpenGLExtension(glGenRenderbuffers, "glGenRenderbuffers", bLogExtensions);
		glBindRenderbuffer = (PFNGLBINDRENDERBUFFERPROC)wglGetProcAddress("glBindRenderbuffer");	checkOpenGLExtension(glBindRenderbuffer, "glBindRenderbuffer", bLogExtensions);
		glRenderbufferStorage = (PFNGLRENDERBUFFERSTORAGEPROC)wglGetProcAddress("glRenderbufferStorage");	checkOpenGLExtension(glRenderbufferStorage, "glRenderbufferStorage", bLogExtensions);
		glFramebufferRenderbuffer = (PFNGLFRAMEBUFFERRENDERBUFFERPROC)wglGetProcAddress("glFramebufferRenderbuffer");	checkOpenGLExtension(glFramebufferRenderbuffer, "glFramebufferRenderbuffer", bLogExtensions);
		glCheckFramebufferStatus = (PFNGLCHECKFRAMEBUFFERSTATUSPROC)wglGetProcAddress("glCheckFramebufferStatus");	checkOpenGLExtension(glCheckFramebufferStatus, "glCheckFramebufferStatus", bLogExtensions);
		glDeleteRenderbuffers = (PFNGLDELETERENDERBUFFERSPROC)wglGetProcAddress("glDeleteRenderbuffers");	checkOpenGLExtension(glDeleteRenderbuffers, "glDeleteRenderbuffers", bLogExtensions);		
		// Generating mip maps for textures bound to frame buffers
		glGenerateMipmap = (PFNGLGENERATEMIPMAPPROC)wglGetProcAddress("glGenerateMipmap");	checkOpenGLExtension(glGenerateMipmap, "glGenerateMipmap", bLogExtensions);
		// Programmable pipeline (Shaders)
		glCreateShader = (PFNGLCREATESHADERPROC)wglGetProcAddress("glCreateShader");	checkOpenGLExtension(glCreateShader, "glCreateShader", bLogExtensions);
		glShaderSource = (PFNGLSHADERSOURCEPROC)wglGetProcAddress("glShaderSource");	checkOpenGLExtension(glShaderSource, "glShaderSource", bLogExtensions);
		glCompileShader = (PFNGLCOMPILESHADERPROC)wglGetProcAddress("glCompileShader");	checkOpenGLExtension(glCompileShader, "glCompileShader", bLogExtensions);
		// Programmable pipeline (Programs)
		glCreateProgram = (PFNGLCREATEPROGRAMPROC)wglGetProcAddress("glCreateProgram");	checkOpenGLExtension(glCreateProgram, "glCreateProgram", bLogExtensions);
		glAttachShader = (PFNGLATTACHSHADERPROC)wglGetProcAddress("glAttachShader");	checkOpenGLExtension(glAttachShader, "glAttachShader", bLogExtensions);
		glLinkProgram = (PFNGLLINKPROGRAMPROC)wglGetProcAddress("glLinkProgram");	checkOpenGLExtension(glLinkProgram, "glLinkProgram", bLogExtensions);
		glUseProgram = (PFNGLUSEPROGRAMPROC)wglGetProcAddress("glUseProgram");	checkOpenGLExtension(glUseProgram, "glUseProgram", bLogExtensions);
		// Programmable pipeline (Debugging)
		glGetShaderiv = (PFNGLGETSHADERIVPROC)wglGetProcAddress("glGetShaderiv");	checkOpenGLExtension(glGetShaderiv, "glGetShaderiv", bLogExtensions);
		glGetProgramiv = (PFNGLGETPROGRAMIVPROC)wglGetProcAddress("glGetProgramiv");	checkOpenGLExtension(glGetProgramiv, "glGetProgramiv", bLogExtensions);
		glGetShaderInfoLog = (PFNGLGETSHADERINFOLOGPROC)wglGetProcAddress("glGetShaderInfoLog");	checkOpenGLExtension(glGetShaderInfoLog, "glGetShaderInfoLog", bLogExtensions);
		glGetProgramInfoLog = (PFNGLGETPROGRAMINFOLOGPROC)wglGetProcAddress("glGetProgramInfoLog");	checkOpenGLExtension(glGetProgramInfoLog, "glGetProgramInfoLog", bLogExtensions);
		// Programmable pipeline (Cleanup)
		glDetachShader = (PFNGLDETACHSHADERPROC)wglGetProcAddress("glDetachShader");	checkOpenGLExtension(glDetachShader, "glDetachShader", bLogExtensions);
		glDeleteShader = (PFNGLDELETESHADERPROC)wglGetProcAddress("glDeleteShader");	checkOpenGLExtension(glDeleteShader, "glDeleteShader", bLogExtensions);
		glDeleteProgram = (PFNGLDELETEPROGRAMPROC)wglGetProcAddress("glDeleteProgram");	checkOpenGLExtension(glDeleteProgram, "glDeleteProgram", bLogExtensions);
		// Programmable pipeline (Uniform variables)
		glGetUniformLocation = (PFNGLGETUNIFORMLOCATIONPROC)wglGetProcAddress("glGetUniformLocation");	checkOpenGLExtension(glGetUniformLocation, "glGetUniformLocation", bLogExtensions);
		glUniform1f = (PFNGLUNIFORM1FPROC)wglGetProcAddress("glUniform1f");	checkOpenGLExtension(glUniform1f, "glUniform1f", bLogExtensions);
		glUniform2f = (PFNGLUNIFORM2FPROC)wglGetProcAddress("glUniform2f");	checkOpenGLExtension(glUniform2f, "glUniform2f", bLogExtensions);
		glUniform3f = (PFNGLUNIFORM3FPROC)wglGetProcAddress("glUniform3f");	checkOpenGLExtension(glUniform3f, "glUniform3f", bLogExtensions);
		glUniform4f = (PFNGLUNIFORM4FPROC)wglGetProcAddress("glUniform4f");	checkOpenGLExtension(glUniform4f, "glUniform4f", bLogExtensions);
		glUniform1fv = (PFNGLUNIFORM1FVPROC)wglGetProcAddress("glUniform1fv");	checkOpenGLExtension(glUniform1fv, "glUniform1fv", bLogExtensions);
		glUniform2fv = (PFNGLUNIFORM2FVPROC)wglGetProcAddress("glUniform2fv");	checkOpenGLExtension(glUniform2fv, "glUniform2fv", bLogExtensions);
		glUniform3fv = (PFNGLUNIFORM3FVPROC)wglGetProcAddress("glUniform3fv");	checkOpenGLExtension(glUniform3fv, "glUniform3fv", bLogExtensions);
		glUniform4fv = (PFNGLUNIFORM4FVPROC)wglGetProcAddress("glUniform4fv");	checkOpenGLExtension(glUniform4fv, "glUniform4fv", bLogExtensions);
		glUniform1i = (PFNGLUNIFORM1IPROC)wglGetProcAddress("glUniform1i");	checkOpenGLExtension(glUniform1i, "glUniform1i", bLogExtensions);
		glUniform2i = (PFNGLUNIFORM2IPROC)wglGetProcAddress("glUniform2i");	checkOpenGLExtension(glUniform2i, "glUniform2i", bLogExtensions);
		glUniform3i = (PFNGLUNIFORM3IPROC)wglGetProcAddress("glUniform3i");	checkOpenGLExtension(glUniform3i, "glUniform3i", bLogExtensions);
		glUniform4i = (PFNGLUNIFORM4IPROC)wglGetProcAddress("glUniform4i");	checkOpenGLExtension(glUniform4i, "glUniform4i", bLogExtensions);
		glUniform1iv = (PFNGLUNIFORM1IVPROC)wglGetProcAddress("glUniform1iv");	checkOpenGLExtension(glUniform1iv, "glUniform1iv", bLogExtensions);
		glUniform2iv = (PFNGLUNIFORM2IVPROC)wglGetProcAddress("glUniform2iv");	checkOpenGLExtension(glUniform2iv, "glUniform2iv", bLogExtensions);
		glUniform3iv = (PFNGLUNIFORM3IVPROC)wglGetProcAddress("glUniform3iv");	checkOpenGLExtension(glUniform3iv, "glUniform3iv", bLogExtensions);
		glUniform4iv = (PFNGLUNIFORM4IVPROC)wglGetProcAddress("glUniform4iv");	checkOpenGLExtension(glUniform4iv, "glUniform4iv", bLogExtensions);
		glUniformMatrix2fv = (PFNGLUNIFORMMATRIX2FVPROC)wglGetProcAddress("glUniformMatrix2fv");	checkOpenGLExtension(glUniformMatrix2fv, "glUniformMatrix2fv", bLogExtensions);
		glUniformMatrix3fv = (PFNGLUNIFORMMATRIX3FVPROC)wglGetProcAddress("glUniformMatrix3fv");	checkOpenGLExtension(glUniformMatrix3fv, "glUniformMatrix3fv", bLogExtensions);
		glUniformMatrix4fv = (PFNGLUNIFORMMATRIX4FVPROC)wglGetProcAddress("glUniformMatrix4fv");	checkOpenGLExtension(glUniformMatrix4fv, "glUniformMatrix4fv", bLogExtensions);
		// Programmable pipeline (Attribute variables)
		glGetAttribLocation = (PFNGLGETATTRIBLOCATIONPROC)wglGetProcAddress("glGetAttribLocation");	checkOpenGLExtension(glGetAttribLocation, "glGetAttribLocation", bLogExtensions);
		glVertexAttrib1f = (PFNGLVERTEXATTRIB1FPROC)wglGetProcAddress("glVertexAttrib1f");	checkOpenGLExtension(glVertexAttrib1f, "glVertexAttrib1f", bLogExtensions);
		glVertexAttrib2f = (PFNGLVERTEXATTRIB2FPROC)wglGetProcAddress("glVertexAttrib2f");	checkOpenGLExtension(glVertexAttrib2f, "glVertexAttrib2f", bLogExtensions);
		glVertexAttrib3f = (PFNGLVERTEXATTRIB3FPROC)wglGetProcAddress("glVertexAttrib3f");	checkOpenGLExtension(glVertexAttrib3f, "glVertexAttrib3f", bLogExtensions);
		glVertexAttrib4f = (PFNGLVERTEXATTRIB4FPROC)wglGetProcAddress("glVertexAttrib4f");	checkOpenGLExtension(glVertexAttrib4f, "glVertexAttrib4f", bLogExtensions);
		glVertexAttrib1fv = (PFNGLVERTEXATTRIB1FVPROC)wglGetProcAddress("glVertexAttrib1fv");	checkOpenGLExtension(glVertexAttrib1fv, "glVertexAttrib1fv", bLogExtensions);
		glVertexAttrib2fv = (PFNGLVERTEXATTRIB2FVPROC)wglGetProcAddress("glVertexAttrib2fv");	checkOpenGLExtension(glVertexAttrib2fv, "glVertexAttrib2fv", bLogExtensions);
		glVertexAttrib3fv = (PFNGLVERTEXATTRIB3FVPROC)wglGetProcAddress("glVertexAttrib3fv");	checkOpenGLExtension(glVertexAttrib3fv, "glVertexAttrib3fv", bLogExtensions);
		glVertexAttrib4fv = (PFNGLVERTEXATTRIB4FVPROC)wglGetProcAddress("glVertexAttrib4fv");	checkOpenGLExtension(glVertexAttrib4fv, "glVertexAttrib4fv", bLogExtensions);
		glEnableVertexAttribArray = (PFNGLENABLEVERTEXATTRIBARRAYPROC)wglGetProcAddress("glEnableVertexAttribArray");	checkOpenGLExtension(glEnableVertexAttribArray, "glEnableVertexAttribArray", bLogExtensions);
		glVertexAttribPointer = (PFNGLVERTEXATTRIBPOINTERPROC)wglGetProcAddress("glVertexAttribPointer");	checkOpenGLExtension(glVertexAttribPointer, "glVertexAttribPointer", bLogExtensions);
		// Vertex buffer objects
		glGenBuffers = (PFNGLGENBUFFERSPROC)wglGetProcAddress("glGenBuffers");	checkOpenGLExtension(glGenBuffers, "glGenBuffers", bLogExtensions);
		glBindBuffer = (PFNGLBINDBUFFERPROC)wglGetProcAddress("glBindBuffer");	checkOpenGLExtension(glBindBuffer, "glBindBuffer", bLogExtensions);
		glDeleteBuffers = (PFNGLDELETEBUFFERSPROC)wglGetProcAddress("glDeleteBuffers");	checkOpenGLExtension(glDeleteBuffers, "glDeleteBuffers", bLogExtensions);
		glBufferData = (PFNGLBUFFERDATAARBPROC)wglGetProcAddress("glBufferData");	checkOpenGLExtension(glBufferData, "glBufferData", bLogExtensions);
		// Vertex arrays
		glGenVertexArrays = (PFNGLGENVERTEXARRAYSPROC)wglGetProcAddress("glGenVertexArrays");	checkOpenGLExtension(glGenVertexArrays, "glGenVertexArrays", bLogExtensions);
		glBindVertexArray = (PFNGLBINDVERTEXARRAYPROC)wglGetProcAddress("glBindVertexArray");	checkOpenGLExtension(glBindVertexArray, "glBindVertexArray", bLogExtensions);
		glDeleteVertexArrays = (PFNGLDELETEVERTEXARRAYSPROC)wglGetProcAddress("glDeleteVertexArrays");	checkOpenGLExtension(glDeleteVertexArrays, "glDeleteVertexArrays", bLogExtensions);
		// Instancing
		glDrawElementsInstanced = (PFNGLDRAWELEMENTSINSTANCEDPROC)wglGetProcAddress("glDrawElementsInstanced");	checkOpenGLExtension(glDrawElementsInstanced, "glDrawElementsInstanced", bLogExtensions);
		glVertexAttribDivisor = (PFNGLVERTEXATTRIBDIVISORPROC)wglGetProcAddress("glVertexAttribDivisor");	checkOpenGLExtension(glVertexAttribDivisor, "glVertexAttribDivisor", bLogExtensions);
		pLog->add("setupOpenGLExtensions() complete.");
	}
}