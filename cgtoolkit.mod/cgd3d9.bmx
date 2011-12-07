
Strict

Import PUB.DirectX
Import "lib/libcgD3D9.a"

' cgD3D9Errors
Const cgD3D9Failed     = 1000
Const cgD3D9DebugTrace = 1001

Const CGD3D9ERR_NOTLOADED       = (Long(1) Shl 31) | (Long($877) Shl 16) | Long(1)
Const CGD3D9ERR_NODEVICE        = (Long(1) Shl 31) | (Long($877) Shl 16) | Long(1)
Const CGD3D9ERR_NOTSAMPLER      = (Long(1) Shl 31) | (Long($877) Shl 16) | Long(1)
Const CGD3D9ERR_INVALIDPROFILE  = (Long(1) Shl 31) | (Long($877) Shl 16) | Long(1)
Const CGD3D9ERR_NULLVALUE       = (Long(1) Shl 31) | (Long($877) Shl 16) | Long(1)
Const CGD3D9ERR_OUTOFRANGE      = (Long(1) Shl 31) | (Long($877) Shl 16) | Long(1)
Const CGD3D9ERR_NOTUNIFORM      = (Long(1) Shl 31) | (Long($877) Shl 16) | Long(1)
Const CGD3D9ERR_NOTMATRIX       = (Long(1) Shl 31) | (Long($877) Shl 16) | Long(1)
Const CGD3D9ERR_INVALIDPARAM    = (Long(1) Shl 31) | (Long($877) Shl 16) | Long(1)

Const CGD3D9_INVALID_USAGE = $FF

Extern
	Function cgD3D9TypeToSize(type_)
	'Function cgD3D9ResourceToDeclUsage:byte(CGresource resource)
	'Function cgD3D9GetVertexDeclaration(prog, D3DVERTEXELEMENT9 decl[MAXD3DDECLLENGTH])
	'Function cgD3D9ValidateVertexDeclaration(prog, const D3DVERTEXELEMENT9 *decl)
	Function cgD3D9GetDevice:IDirect3DDevice9()
	Function cgD3D9SetDevice(pDevice:IDirect3DDevice9)
	'Function cgD3D9LoadProgram(prog, paramShadowing, assemFlags)
	'Function cgD3D9UnloadProgram(prog)
	'Function cgD3D9IsProgramLoaded(prog)
	'Function cgD3D9BindProgram(prog)
	'Function cgD3D9SetUniform(CGparameter param, const void *floats)
	'Function cgD3D9SetUniformArray(CGparameter param, DWORD offset, DWORD numItems, const void *values)
	'Function cgD3D9SetUniformMatrix(CGparameter param, const D3DMATRIX *matrix)
	'Function cgD3D9SetUniformMatrixArray(CGparameter param, DWORD offset, DWORD numItems, const D3DMATRIX *matrices)
	'Function cgD3D9SetTexture(CGparameter param, IDirect3DBaseTexture9 *tex)
	'Function cgD3D9SetSamplerState(CGparameter param, D3DSAMPLERSTATETYPE type, DWORD value)
	'Function cgD3D9SetTextureWrapMode(CGparameter param, DWORD value)
	'Function cgD3D9EnableParameterShadowing(prog, CGbool enable)
	'Function cgD3D9IsParameterShadowingEnabled(prog)
	Function cgD3D9GetLatestVertexProfile()
	Function cgD3D9GetLatestPixelProfile()
	'Function cgD3D9GetOptimalOptions:Byte ptr ptr(CGprofile profile)
	'Function cgD3D9IsProfileSupported(CGprofile profile)
	'Function cgD3D9GetLastError(void)
	'Function cgD3D9TranslateCGerror:byte ptr(CGerror error)
	'Function cgD3D9TranslateHRESULT:byte ptr(HRESULT hr)
	'Function cgD3D9EnableDebugTracing(CGbool enable)
	'Function cgD3D9RegisterStates(CGcontext ctx)
	'Function cgD3D9SetManageTextureParameters(CGcontext ctx, CGbool flag)
	'Function cgD3D9GetManageTextureParameters(CGcontext ctx)
	'Function cgD3D9GetTextureParameter:IDirect3DBaseTexture9(CGparameter param)
	'Function cgD3D9SetTextureParameter(param, tex:IDirect3DBaseTexture9 )
	Function cgD3D9UnloadAllPrograms()
	Function cgD3D9UnbindProgram(prog)
End Extern