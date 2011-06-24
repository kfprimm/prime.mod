
Strict

Import "-lCgGL"

' CGGLenum
Const CG_GL_MATRIX_IDENTITY             = 0
Const CG_GL_MATRIX_TRANSPOSE            = 1
Const CG_GL_MATRIX_INVERSE              = 2
Const CG_GL_MATRIX_INVERSE_TRANSPOSE    = 3
Const CG_GL_MODELVIEW_MATRIX            = 4
Const CG_GL_PROJECTION_MATRIX           = 5
Const CG_GL_TEXTURE_MATRIX              = 6
Const CG_GL_MODELVIEW_PROJECTION_MATRIX = 7
Const CG_GL_VERTEX                      = 8
Const CG_GL_FRAGMENT                    = 9
Const CG_GL_GEOMETRY                    = 10
Const CG_GL_TESSELLATION_CONTROL        = 11
Const CG_GL_TESSELLATION_EVALUATION     = 12

Extern
	'Function cgGLIsProfileSupported(CGprofile profile)
	'Function cgGLEnableProfile(profile)
	'Function cgGLDisableProfile(profile)
	Function cgGLGetLatestProfile(profile_type)
	Function cgGLSetOptimalOptions(profile)
	'Function cgGLGetOptimalOptions:Byte ptr ptr(CGprofile profile)
	'Function cgGLLoadProgram(CGprogram program)
	'Function cgGLUnloadProgram(CGprogram program)
	'Function cgGLIsProgramLoaded(CGprogram program)
	'Function cgGLBindProgram(CGprogram program)
	'Function cgGLUnbindProgram(CGprofile profile)
	'Function cgGLGetProgramID(CGprogram program)
	'Function cgGLSetParameter1f(param, x#)
	'Function cgGLSetParameter2f(param, x#, y#)
	'Function cgGLSetParameter3f(param, x#, y#, z#)
	'Function cgGLSetParameter4f(param, x#, y#, z#, float w)
	'Function cgGLSetParameter1fv(param, const v:Float Ptr)
	'Function cgGLSetParameter2fv(param, const v:Float Ptr)
	'Function cgGLSetParameter3fv(param, const v:Float Ptr)
	'Function cgGLSetParameter4fv(param, const v:Float Ptr)
	'Function cgGLSetParameter1d(param, double x)
	'Function cgGLSetParameter2d(param, double x, double y)
	'Function cgGLSetParameter3d(param, double x, double y, double z)
	'Function cgGLSetParameter4d(param, double x, double y, double z, double w)
	'Function cgGLSetParameter1dv(param, const v:Double Ptr)
	'Function cgGLSetParameter2dv(param, const v:Double Ptr)
	'Function cgGLSetParameter3dv(param, const v:Double Ptr)
	'Function cgGLSetParameter4dv(param, const v:Double Ptr)
	'Function cgGLGetParameter1f(param, v:Float Ptr)
	'Function cgGLGetParameter2f(param, v:Float Ptr)
	'Function cgGLGetParameter3f(param, v:Float Ptr)
	'Function cgGLGetParameter4f(param, v:Float Ptr)
	'Function cgGLGetParameter1d(param, v:Double Ptr)
	'Function cgGLGetParameter2d(param, v:Double Ptr)
	'Function cgGLGetParameter3d(param, v:Double Ptr)
	'Function cgGLGetParameter4d(param, v:Double Ptr)
	'Function cgGLSetParameterArray1f(param, offset:Long, nelements:Long, const v:Float Ptr)
	'Function cgGLSetParameterArray2f(param, offset:Long, nelements:Long, const v:Float Ptr)
	'Function cgGLSetParameterArray3f(param, offset:Long, nelements:Long, const v:Float Ptr)
	'Function cgGLSetParameterArray4f(param, offset:Long, nelements:Long, const v:Float Ptr)
	'Function cgGLSetParameterArray1d(param, offset:Long, nelements:Long, const v:Double Ptr)
	'Function cgGLSetParameterArray2d(param, offset:Long, nelements:Long, const v:Double Ptr)
	'Function cgGLSetParameterArray3d(param, offset:Long, nelements:Long, const v:Double Ptr)
	'Function cgGLSetParameterArray4d(param, offset:Long, nelements:Long, const v:Double Ptr)
	'Function cgGLGetParameterArray1f(param, offset:Long, nelements:Long, v:Float Ptr)
	'Function cgGLGetParameterArray2f(param, offset:Long, nelements:Long, v:Float Ptr)
	'Function cgGLGetParameterArray3f(param, offset:Long, nelements:Long, v:Float Ptr)
	'Function cgGLGetParameterArray4f(param, offset:Long, nelements:Long, v:Float Ptr)
	'Function cgGLGetParameterArray1d(param, offset:Long, nelements:Long, v:Double Ptr)
	'Function cgGLGetParameterArray2d(param, offset:Long, nelements:Long, v:Double Ptr)
	'Function cgGLGetParameterArray3d(param, offset:Long, nelements:Long, v:Double Ptr)
	'Function cgGLGetParameterArray4d(param, offset:Long, nelements:Long, v:Double Ptr)
	'Function cgGLSetParameterPointer(param, GLint fsize, GLenum type, GLsizei stride, const GLvoid *pointer)
	'Function cgGLEnableClientState(param)
	'Function cgGLDisableClientState(param)
	'Function cgGLSetMatrixParameterdr(param, const double *matrix)
	'Function cgGLSetMatrixParameterfr(param, const float *matrix)
	'Function cgGLSetMatrixParameterdc(param, const double *matrix)
	'Function cgGLSetMatrixParameterfc(param, const float *matrix)
	'Function cgGLGetMatrixParameterdr(param, double *matrix)
	'Function cgGLGetMatrixParameterfr(param, float *matrix)
	'Function cgGLGetMatrixParameterdc(param, double *matrix)
	'Function cgGLGetMatrixParameterfc(param, float *matrix)
	'Function cgGLSetStateMatrixParameter(param, CGGLenum matrix, CGGLenum transform)
	'Function cgGLSetMatrixParameterArrayfc(param, offset:Long, nelements:Long, matrices:Float ptr)
	'Function cgGLSetMatrixParameterArrayfr(param, offset:Long, nelements:Long, matrices:Float ptr)
	'Function cgGLSetMatrixParameterArraydc(param, offset:Long, nelements:Long, matrices:Double ptr)
	'Function cgGLSetMatrixParameterArraydr(param, offset:Long, nelements:Long, matrices:Double ptr)
	'Function cgGLGetMatrixParameterArrayfc(param, offset:Long, nelements:Long, float *matrices)
	'Function cgGLGetMatrixParameterArrayfr(param, offset:Long, nelements:Long, float *matrices)
	'Function cgGLGetMatrixParameterArraydc(param, offset:Long, nelements:Long, double *matrices)
	'Function cgGLGetMatrixParameterArraydr(param, offset:Long, nelements:Long, double *matrices)
	'Function cgGLSetTextureParameter(param, GLuint texobj)
	'Function cgGLGetTextureParameter(param)
	'Function cgGLEnableTextureParameter(param)
	'Function cgGLDisableTextureParameter(param)
	'Function cgGLGetTextureEnum(param)
	'Function cgGLSetManageTextureParameters(CGcontext ctx, CGbool flag)
	'Function cgGLGetManageTextureParameters(CGcontext ctx)
	'Function cgGLSetupSampler(param, GLuint texobj)
	'Function cgGLRegisterStates(CGcontext ctx)
	'Function cgGLEnableProgramProfiles(CGprogram program)
	'Function cgGLDisableProgramProfiles(CGprogram program)
	'Function cgGLSetDebugMode(CGbool debug)
	'Function cgGLCreateBuffer(CGcontext context, int size, const void *data, GLenum bufferUsage)
	'Function cgGLGetBufferObject(CGbuffer buffer)
End Extern