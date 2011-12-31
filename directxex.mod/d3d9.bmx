
Strict

' D3DFILLMODE
Const D3DFILL_POINT			= 1
Const D3DFILL_WIREFRAME	= 2
Const D3DFILL_SOLID			= 3

' D3DTEXTURETRANSFORMFLAGS 
Const D3DTTFF_DISABLE   = 0
Const D3DTTFF_COUNT1    = 1
Const D3DTTFF_COUNT2    = 2
Const D3DTTFF_COUNT3    = 3
Const D3DTTFF_COUNT4    = 4
Const D3DTTFF_PROJECTED = 256

Const D3DWRAPCOORD_0 = 1
Const D3DWRAPCOORD_1 = 2
Const D3DWRAPCOORD_2 = 4
Const D3DWRAPCOORD_3 = 8

Const D3DWRAP_U = D3DWRAPCOORD_0
Const D3DWRAP_V = D3DWRAPCOORD_1
Const D3DWRAP_W = D3DWRAPCOORD_2

Const D3DRENDERSTATE_WRAPBIAS = 128

' D3DTSS_TCI
Const D3DTSS_TCI_PASSTHRU                    = 0
Const D3DTSS_TCI_CAMERASPACENORMAL           = $00010000
Const D3DTSS_TCI_CAMERASPACEPOSITION         = $00020000
Const D3DTSS_TCI_CAMERASPACEREFLECTIONVECTOR = $00030000
Const D3DTSS_TCI_SPHEREMAP                   = $00040000

Function D3DCOLOR_ARGB(alpha,red,green,blue)
	Return ((alpha&$ff) Shl 24)|((red&$ff) Shl 16)|((green&$ff) Shl 8)|(blue&$ff)
End Function
Function D3DCOLOR_RGBA(red,green,blue,alpha)
	Return D3DCOLOR_ARGB(alpha,red,green,blue)
End Function
Function D3DCOLOR_XRGB(red,green,blue)
	Return D3DCOLOR_ARGB(255,red,green,blue)
End Function
Function D3DCOLOR_RGB(red,green,blue)
	Return D3DCOLOR_ARGB(255,red,green,blue)
End Function


