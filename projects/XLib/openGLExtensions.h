#pragma once
#include "PCH.h"

// Call setupOpenGLExtensions() to setup OpenGL extensions AFTER the OpenGL window has been opened.


namespace X
{
	// Detects required extensions and retrieves pointers to the functions
	// Also writes out the extensions to the log file.
	// Must be called AFTER the OpenGL window is created.
	// Called from SCWindow::createWindow()
	// If it has been called before, it'll silently return.
	void setupOpenGLExtensions(bool bLogExtensions);

	// VSync
	extern PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT;
	// Multi texturing
	extern PFNGLACTIVETEXTUREPROC glActiveTexture;
//	extern PFNGLCLIENTACTIVETEXTUREARBPROC glClientActiveTexture;
//	extern PFNGLMULTITEXCOORD1FARBPROC glMultiTexCoord1f;
//	extern PFNGLMULTITEXCOORD2FARBPROC glMultiTexCoord2f;
//	extern PFNGLMULTITEXCOORD3FARBPROC glMultiTexCoord3f;
	// Frame buffer
	extern PFNGLGENFRAMEBUFFERSPROC glGenFramebuffers;
	extern PFNGLBINDFRAMEBUFFERPROC glBindFramebuffer;
	extern PFNGLDELETEFRAMEBUFFERSPROC glDeleteFramebuffers;
	extern PFNGLFRAMEBUFFERTEXTURE2DPROC glFramebufferTexture2D;
	// Render buffer
	extern PFNGLGENRENDERBUFFERSPROC glGenRenderbuffers;
	extern PFNGLBINDRENDERBUFFERPROC glBindRenderbuffer;
	extern PFNGLRENDERBUFFERSTORAGEPROC glRenderbufferStorage;
	extern PFNGLFRAMEBUFFERRENDERBUFFERPROC glFramebufferRenderbuffer;
	extern PFNGLCHECKFRAMEBUFFERSTATUSPROC glCheckFramebufferStatus;
	extern PFNGLDELETERENDERBUFFERSPROC glDeleteRenderbuffers;
	// Generating mip maps for textures bound to frame buffers
	extern PFNGLGENERATEMIPMAPPROC glGenerateMipmap;
	// Programmable pipeline (Shaders)
	extern PFNGLCREATESHADERPROC glCreateShader;
	extern PFNGLSHADERSOURCEPROC glShaderSource;
	extern PFNGLCOMPILESHADERPROC glCompileShader;
	// Programmable pipeline (Programs)
	extern PFNGLCREATEPROGRAMPROC glCreateProgram;
	extern PFNGLATTACHSHADERPROC glAttachShader;
	extern PFNGLLINKPROGRAMPROC glLinkProgram;
	extern PFNGLUSEPROGRAMPROC glUseProgram;
	// Programmable pipeline (Debugging)
	extern PFNGLGETSHADERIVPROC glGetShaderiv;
	extern PFNGLGETPROGRAMIVPROC glGetProgramiv;
	extern PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog;
	extern PFNGLGETPROGRAMINFOLOGPROC glGetProgramInfoLog;
	// Programmable pipeline (Cleanup)
	extern PFNGLDETACHSHADERPROC glDetachShader;
	extern PFNGLDELETESHADERPROC glDeleteShader;
	extern PFNGLDELETEPROGRAMPROC glDeleteProgram;
	// Programmable pipeline (Uniform variables)
	extern PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation;
	extern PFNGLUNIFORM1FPROC glUniform1f;
	extern PFNGLUNIFORM2FPROC glUniform2f;
	extern PFNGLUNIFORM3FPROC glUniform3f;
	extern PFNGLUNIFORM4FPROC glUniform4f;
	extern PFNGLUNIFORM1FVPROC glUniform1fv;
	extern PFNGLUNIFORM2FVPROC glUniform2fv;
	extern PFNGLUNIFORM3FVPROC glUniform3fv;
	extern PFNGLUNIFORM4FVPROC glUniform4fv;
	extern PFNGLUNIFORM1IPROC glUniform1i;
	extern PFNGLUNIFORM2IPROC glUniform2i;
	extern PFNGLUNIFORM3IPROC glUniform3i;
	extern PFNGLUNIFORM4IPROC glUniform4i;
	extern PFNGLUNIFORM1IVPROC glUniform1iv;
	extern PFNGLUNIFORM2IVPROC glUniform2iv;
	extern PFNGLUNIFORM3IVPROC glUniform3iv;
	extern PFNGLUNIFORM4IVPROC glUniform4iv;
	extern PFNGLUNIFORMMATRIX2FVPROC glUniformMatrix2fv;
	extern PFNGLUNIFORMMATRIX3FVPROC glUniformMatrix3fv;
	extern PFNGLUNIFORMMATRIX4FVPROC glUniformMatrix4fv;
	// Programmable pipeline (Attribute variables)
	extern PFNGLGETATTRIBLOCATIONPROC glGetAttribLocation;
	extern PFNGLVERTEXATTRIB1FPROC glVertexAttrib1f;
	extern PFNGLVERTEXATTRIB2FPROC glVertexAttrib2f;
	extern PFNGLVERTEXATTRIB3FPROC glVertexAttrib3f;
	extern PFNGLVERTEXATTRIB4FPROC glVertexAttrib4f;
	extern PFNGLVERTEXATTRIB1FVPROC glVertexAttrib1fv;
	extern PFNGLVERTEXATTRIB2FVPROC glVertexAttrib2fv;
	extern PFNGLVERTEXATTRIB3FVPROC glVertexAttrib3fv;
	extern PFNGLVERTEXATTRIB4FVPROC glVertexAttrib4fv;
	extern PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray;
	extern PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer;
	// Vertex buffer objects
	extern PFNGLGENBUFFERSPROC glGenBuffers;
	extern PFNGLBINDBUFFERPROC glBindBuffer;
	extern PFNGLDELETEBUFFERSPROC glDeleteBuffers;
	extern PFNGLBUFFERDATAARBPROC glBufferData;
	// Vertex array objects
	extern PFNGLGENVERTEXARRAYSPROC glGenVertexArrays;
	extern PFNGLBINDVERTEXARRAYPROC glBindVertexArray;
	extern PFNGLDELETEVERTEXARRAYSPROC glDeleteVertexArrays;
	// Instancing
	extern PFNGLDRAWELEMENTSINSTANCEDPROC glDrawElementsInstanced;
	extern PFNGLVERTEXATTRIBDIVISORPROC glVertexAttribDivisor;

}