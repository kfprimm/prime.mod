
Strict

Import Prime.GLMax2DEx
Import Prime.D3D9Max2DEx

Graphics 800,600

Local radius# = 0
While Not KeyDown(KEY_ESCAPE) And Not AppTerminate()
	radius:+(KeyDown(KEY_UP)-KeyDown(KEY_DOWN))*.5
	
	Cls
	DrawRoundRect 100,100,600,400,radius

	DrawText "Press up to increase radius, down to decrease.",0,0
	DrawText "Rectangle radius: "+radius,0,15
	Flip
Wend
