#include "PCH.h"
#include "log.h"

namespace X
{
	// VSync
	PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT;
	// Multi texturing
	PFNGLACTIVETEXTUREARBPROC glActiveTextureARB;
	PFNGLCLIENTACTIVETEXTUREARBPROC glClientActiveTexture;
	PFNGLMULTITEXCOORD1FARBPROC glMultiTexCoord1f;
	PFNGLMULTITEXCOORD2FARBPROC glMultiTexCoord2f;
	PFNGLMULTITEXCOORD3FARBPROC glMultiTexCoord3f;
	// Frame buffer
	PFNGLGENFRAMEBUFFERSEXTPROC glGenFramebuffersEXT;
	PFNGLBINDFRAMEBUFFEREXTPROC glBindFramebufferEXT;
	PFNGLDELETEFRAMEBUFFERSEXTPROC glDeleteFramebuffersEXT;
	// Render buffer
	PFNGLGENRENDERBUFFERSEXTPROC glGenRenderbuffersEXT;
	PFNGLBINDRENDERBUFFEREXTPROC glBindRenderbufferEXT;
	PFNGLRENDERBUFFERSTORAGEEXTPROC glRenderbufferStorageEXT;
	PFNGLFRAMEBUFFERRENDERBUFFEREXTPROC glFramebufferRenderbufferEXT;
	PFNGLFRAMEBUFFERTEXTURE2DEXTPROC glFramebufferTexture2DEXT;
	PFNGLCHECKFRAMEBUFFERSTATUSEXTPROC glCheckFramebufferStatusEXT;
	PFNGLDELETERENDERBUFFERSEXTPROC glDeleteRenderbuffersEXT;
	// Generating mip maps for textures bound to frame buffers
	PFNGLGENERATEMIPMAPEXTPROC glGenerateMipmap;
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
	PFNGLGENVERTEXARRAYSPROC glGenVertexArrays;

	void checkOpenGLExtension(void* extension, const std::string strExtensionName)
	{
		std::string strError = "Extension name: " + strExtensionName + " not found.";
		ThrowIfFalse(extension, strError);
		std::string strFound = "Extension name: " + strExtensionName + " found.";
		Log::getPointer()->add(strFound);
	}

	void setupOpenGLExtensions(void)
	{
		static bool bDoneThisBefore = false;
		if (bDoneThisBefore)
			return;
		bDoneThisBefore = true;

		Log* pLog = Log::getPointer();
		pLog->add("setupOpenGLExtensions() called...");

		// VSync
		wglSwapIntervalEXT = (PFNWGLSWAPINTERVALEXTPROC)wglGetProcAddress("wglSwapIntervalEXT");
		checkOpenGLExtension(wglSwapIntervalEXT, "wglSwapIntervalEXT");
		// Multi texturing
		glActiveTextureARB = (PFNGLACTIVETEXTUREARBPROC)wglGetProcAddress("glActiveTextureARB");
		checkOpenGLExtension(glActiveTextureARB, "glActiveTextureARB");
		glClientActiveTexture = (PFNGLCLIENTACTIVETEXTUREARBPROC)wglGetProcAddress("glClientActiveTexture");
		checkOpenGLExtension(glClientActiveTexture, "glClientActiveTexture");
		glMultiTexCoord1f = (PFNGLMULTITEXCOORD1FARBPROC)wglGetProcAddress("glMultiTexCoord1f");
		checkOpenGLExtension(glMultiTexCoord1f, "glMultiTexCoord1f");
		glMultiTexCoord2f = (PFNGLMULTITEXCOORD2FARBPROC)wglGetProcAddress("glMultiTexCoord2f");
		checkOpenGLExtension(glMultiTexCoord2f, "glMultiTexCoord2f");
		glMultiTexCoord3f = (PFNGLMULTITEXCOORD3FARBPROC)wglGetProcAddress("glMultiTexCoord3f");
		checkOpenGLExtension(glMultiTexCoord3f, "glMultiTexCoord3f");
		// Frame buffer
		glGenFramebuffersEXT = (PFNGLGENFRAMEBUFFERSEXTPROC)wglGetProcAddress("glGenFramebuffersEXT");
		checkOpenGLExtension(glGenFramebuffersEXT, "glGenFramebuffersEXT");
		glBindFramebufferEXT = (PFNGLBINDFRAMEBUFFEREXTPROC)wglGetProcAddress("glBindFramebufferEXT");
		checkOpenGLExtension(glBindFramebufferEXT, "glBindFramebufferEXT");
		glDeleteFramebuffersEXT = (PFNGLDELETEFRAMEBUFFERSEXTPROC)wglGetProcAddress("glDeleteFramebuffersEXT");
		checkOpenGLExtension(glDeleteFramebuffersEXT, "glDeleteFramebuffersEXT");
		// Render buffer
		glGenRenderbuffersEXT = (PFNGLGENRENDERBUFFERSEXTPROC)wglGetProcAddress("glGenRenderbuffersEXT");
		checkOpenGLExtension(glGenRenderbuffersEXT, "glGenRenderbuffersEXT");
		glBindRenderbufferEXT = (PFNGLBINDRENDERBUFFEREXTPROC)wglGetProcAddress("glBindRenderbufferEXT");
		checkOpenGLExtension(glBindRenderbufferEXT, "glBindRenderbufferEXT");
		glRenderbufferStorageEXT = (PFNGLRENDERBUFFERSTORAGEEXTPROC)wglGetProcAddress("glRenderbufferStorageEXT");
		checkOpenGLExtension(glRenderbufferStorageEXT, "glRenderbufferStorageEXT");
		glFramebufferRenderbufferEXT = (PFNGLFRAMEBUFFERRENDERBUFFEREXTPROC)wglGetProcAddress("glFramebufferRenderbufferEXT");
		checkOpenGLExtension(glFramebufferRenderbufferEXT, "glFramebufferRenderbufferEXT");
		glFramebufferTexture2DEXT = (PFNGLFRAMEBUFFERTEXTURE2DEXTPROC)wglGetProcAddress("glFramebufferTexture2DEXT");
		checkOpenGLExtension(glFramebufferTexture2DEXT, "glFramebufferTexture2DEXT");
		glCheckFramebufferStatusEXT = (PFNGLCHECKFRAMEBUFFERSTATUSEXTPROC)wglGetProcAddress("glCheckFramebufferStatusEXT");
		checkOpenGLExtension(glCheckFramebufferStatusEXT, "glCheckFramebufferStatusEXT");
		glDeleteRenderbuffersEXT = (PFNGLDELETERENDERBUFFERSEXTPROC)wglGetProcAddress("glDeleteRenderbuffersEXT");
		checkOpenGLExtension(glDeleteRenderbuffersEXT, "glDeleteRenderbuffersEXT");
		// Generating mip maps for textures bound to frame buffers
		glGenerateMipmap = (PFNGLGENERATEMIPMAPEXTPROC)wglGetProcAddress("glGenerateMipmapEXT");
		checkOpenGLExtension(glGenerateMipmap, "glGenerateMipmapEXT");
		// Programmable pipeline (Shaders)
		glCreateShader = (PFNGLCREATESHADERPROC)wglGetProcAddress("glCreateShader");
		checkOpenGLExtension(glCreateShader, "glCreateShader");
		glShaderSource = (PFNGLSHADERSOURCEPROC)wglGetProcAddress("glShaderSource");
		checkOpenGLExtension(glShaderSource, "glShaderSource");
		glCompileShader = (PFNGLCOMPILESHADERPROC)wglGetProcAddress("glCompileShader");
		checkOpenGLExtension(glCompileShader, "glCompileShader");
		// Programmable pipeline (Programs)
		glCreateProgram = (PFNGLCREATEPROGRAMPROC)wglGetProcAddress("glCreateProgram");
		checkOpenGLExtension(glCreateProgram, "glCreateProgram");
		glAttachShader = (PFNGLATTACHSHADERPROC)wglGetProcAddress("glAttachShader");
		checkOpenGLExtension(glAttachShader, "glAttachShader");
		glLinkProgram = (PFNGLLINKPROGRAMPROC)wglGetProcAddress("glLinkProgram");
		checkOpenGLExtension(glLinkProgram, "glLinkProgram");
		glUseProgram = (PFNGLUSEPROGRAMPROC)wglGetProcAddress("glUseProgram");
		checkOpenGLExtension(glUseProgram, "glUseProgram");
		// Programmable pipeline (Debugging)
		glGetShaderiv = (PFNGLGETSHADERIVPROC)wglGetProcAddress("glGetShaderiv");
		checkOpenGLExtension(glGetShaderiv, "glGetShaderiv");
		glGetProgramiv = (PFNGLGETPROGRAMIVPROC)wglGetProcAddress("glGetProgramiv");
		checkOpenGLExtension(glGetProgramiv, "glGetProgramiv");
		glGetShaderInfoLog = (PFNGLGETSHADERINFOLOGPROC)wglGetProcAddress("glGetShaderInfoLog");
		checkOpenGLExtension(glGetShaderInfoLog, "glGetShaderInfoLog");
		glGetProgramInfoLog = (PFNGLGETPROGRAMINFOLOGPROC)wglGetProcAddress("glGetProgramInfoLog");
		checkOpenGLExtension(glGetProgramInfoLog, "glGetProgramInfoLog");
		// Programmable pipeline (Cleanup)
		glDetachShader = (PFNGLDETACHSHADERPROC)wglGetProcAddress("glDetachShader");
		checkOpenGLExtension(glDetachShader, "glDetachShader");
		glDeleteShader = (PFNGLDELETESHADERPROC)wglGetProcAddress("glDeleteShader");
		checkOpenGLExtension(glDeleteShader, "glDeleteShader");
		glDeleteProgram = (PFNGLDELETEPROGRAMPROC)wglGetProcAddress("glDeleteProgram");
		checkOpenGLExtension(glDeleteProgram, "glDeleteProgram");
		// Programmable pipeline (Uniform variables)
		glGetUniformLocation = (PFNGLGETUNIFORMLOCATIONPROC)wglGetProcAddress("glGetUniformLocation");
		checkOpenGLExtension(glGetUniformLocation, "glGetUniformLocation");
		glUniform1f = (PFNGLUNIFORM1FPROC)wglGetProcAddress("glUniform1f");
		checkOpenGLExtension(glUniform1f, "glUniform1f");
		glUniform2f = (PFNGLUNIFORM2FPROC)wglGetProcAddress("glUniform2f");
		checkOpenGLExtension(glUniform2f, "glUniform2f");
		glUniform3f = (PFNGLUNIFORM3FPROC)wglGetProcAddress("glUniform3f");
		checkOpenGLExtension(glUniform3f, "glUniform3f");
		glUniform4f = (PFNGLUNIFORM4FPROC)wglGetProcAddress("glUniform4f");
		checkOpenGLExtension(glUniform4f, "glUniform4f");
		glUniform1fv = (PFNGLUNIFORM1FVPROC)wglGetProcAddress("glUniform1fv");
		checkOpenGLExtension(glUniform1fv, "glUniform1fv");
		glUniform2fv = (PFNGLUNIFORM2FVPROC)wglGetProcAddress("glUniform2fv");
		checkOpenGLExtension(glUniform2fv, "glUniform2fv");
		glUniform3fv = (PFNGLUNIFORM3FVPROC)wglGetProcAddress("glUniform3fv");
		checkOpenGLExtension(glUniform3fv, "glUniform3fv");
		glUniform4fv = (PFNGLUNIFORM4FVPROC)wglGetProcAddress("glUniform4fv");
		checkOpenGLExtension(glUniform4fv, "glUniform4fv");
		glUniform1i = (PFNGLUNIFORM1IPROC)wglGetProcAddress("glUniform1i");
		checkOpenGLExtension(glUniform1i, "glUniform1i");
		glUniform2i = (PFNGLUNIFORM2IPROC)wglGetProcAddress("glUniform2i");
		checkOpenGLExtension(glUniform2i, "glUniform2i");
		glUniform3i = (PFNGLUNIFORM3IPROC)wglGetProcAddress("glUniform3i");
		checkOpenGLExtension(glUniform3i, "glUniform3i");
		glUniform4i = (PFNGLUNIFORM4IPROC)wglGetProcAddress("glUniform4i");
		checkOpenGLExtension(glUniform4i, "glUniform4i");
		glUniform1iv = (PFNGLUNIFORM1IVPROC)wglGetProcAddress("glUniform1iv");
		checkOpenGLExtension(glUniform1iv, "glUniform1iv");
		glUniform2iv = (PFNGLUNIFORM2IVPROC)wglGetProcAddress("glUniform2iv");
		checkOpenGLExtension(glUniform2iv, "glUniform2iv");
		glUniform3iv = (PFNGLUNIFORM3IVPROC)wglGetProcAddress("glUniform3iv");
		checkOpenGLExtension(glUniform3iv, "glUniform3iv");
		glUniform4iv = (PFNGLUNIFORM4IVPROC)wglGetProcAddress("glUniform4iv");
		checkOpenGLExtension(glUniform4iv, "glUniform4iv");
		glUniformMatrix2fv = (PFNGLUNIFORMMATRIX2FVPROC)wglGetProcAddress("glUniformMatrix2fv");
		checkOpenGLExtension(glUniformMatrix2fv, "glUniformMatrix2fv");
		glUniformMatrix3fv = (PFNGLUNIFORMMATRIX3FVPROC)wglGetProcAddress("glUniformMatrix3fv");
		checkOpenGLExtension(glUniformMatrix3fv, "glUniformMatrix3fv");
		glUniformMatrix4fv = (PFNGLUNIFORMMATRIX4FVPROC)wglGetProcAddress("glUniformMatrix4fv");
		checkOpenGLExtension(glUniformMatrix4fv, "glUniformMatrix4fv");
		// Programmable pipeline (Attribute variables)
		glGetAttribLocation = (PFNGLGETATTRIBLOCATIONPROC)wglGetProcAddress("glGetAttribLocation");
		checkOpenGLExtension(glGetAttribLocation, "glGetAttribLocation");
		glVertexAttrib1f = (PFNGLVERTEXATTRIB1FPROC)wglGetProcAddress("glVertexAttrib1f");
		checkOpenGLExtension(glVertexAttrib1f, "glVertexAttrib1f");
		glVertexAttrib2f = (PFNGLVERTEXATTRIB2FPROC)wglGetProcAddress("glVertexAttrib2f");
		checkOpenGLExtension(glVertexAttrib2f, "glVertexAttrib2f");
		glVertexAttrib3f = (PFNGLVERTEXATTRIB3FPROC)wglGetProcAddress("glVertexAttrib3f");
		checkOpenGLExtension(glVertexAttrib3f, "glVertexAttrib3f");
		glVertexAttrib4f = (PFNGLVERTEXATTRIB4FPROC)wglGetProcAddress("glVertexAttrib4f");
		checkOpenGLExtension(glVertexAttrib4f, "glVertexAttrib4f");
		glVertexAttrib1fv = (PFNGLVERTEXATTRIB1FVPROC)wglGetProcAddress("glVertexAttrib1fv");
		checkOpenGLExtension(glVertexAttrib1fv, "glVertexAttrib1fv");
		glVertexAttrib2fv = (PFNGLVERTEXATTRIB2FVPROC)wglGetProcAddress("glVertexAttrib2fv");
		checkOpenGLExtension(glVertexAttrib2fv, "glVertexAttrib2fv");
		glVertexAttrib3fv = (PFNGLVERTEXATTRIB3FVPROC)wglGetProcAddress("glVertexAttrib3fv");
		checkOpenGLExtension(glVertexAttrib3fv, "glVertexAttrib3fv");
		glVertexAttrib4fv = (PFNGLVERTEXATTRIB4FVPROC)wglGetProcAddress("glVertexAttrib4fv");
		checkOpenGLExtension(glVertexAttrib4fv, "glVertexAttrib4fv");
		glEnableVertexAttribArray = (PFNGLENABLEVERTEXATTRIBARRAYPROC)wglGetProcAddress("glEnableVertexAttribArray");
		checkOpenGLExtension(glEnableVertexAttribArray, "glEnableVertexAttribArray");
		glVertexAttribPointer = (PFNGLVERTEXATTRIBPOINTERPROC)wglGetProcAddress("glVertexAttribPointer");
		checkOpenGLExtension(glVertexAttribPointer, "glVertexAttribPointer");
		// Vertex buffer objects
		glGenBuffers = (PFNGLGENBUFFERSPROC)wglGetProcAddress("glGenBuffers");
		checkOpenGLExtension(glGenBuffers, "glGenBuffers");
		glBindBuffer = (PFNGLBINDBUFFERPROC)wglGetProcAddress("glBindBuffer");
		checkOpenGLExtension(glBindBuffer, "glBindBuffer");
		glDeleteBuffers = (PFNGLDELETEBUFFERSPROC)wglGetProcAddress("glDeleteBuffers");
		checkOpenGLExtension(glDeleteBuffers, "glDeleteBuffers");
		glBufferData = (PFNGLBUFFERDATAARBPROC)wglGetProcAddress("glBufferData");
		checkOpenGLExtension(glBufferData, "glBufferData");
		glGenVertexArrays = (PFNGLGENVERTEXARRAYSPROC)wglGetProcAddress("glGenVertexArrays");;
		checkOpenGLExtension(glGenVertexArrays, "glGenVertexArrays");

		pLog->add("setupOpenGLExtensions() complete.");
	}
}