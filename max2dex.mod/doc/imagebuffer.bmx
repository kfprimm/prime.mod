
Strict

Framework sys87.GLBufferedMax2D
'Import sys87.D3D9BufferedMax2D

Graphics 800,600

Const FRAMES = 7

Local image:TImage=CreateImage(75,50,FRAMES)
MidHandleImage image

SetClsColor 0,200,200
For Local i=0 To FRAMES-1
	SetBuffer ImageBuffer(image,i)
	Cls
	SetColor 255,255,0
	DrawRect 5,2,20,40
	SetColor 255,0,0
	
	Local str$="hi"
	For Local j=0 To i
		str:+"!"
	Next
	
	DrawText str,0,15
Next

SetBuffer BackBuffer()
SetClsColor 0,0,0
SetColor 255,255,255

Local frame#
While Not KeyDown(KEY_ESCAPE) Or AppTerminate()
	Cls
	DrawRect 10,5,200,200
	DrawImage image,MouseX(),MouseY(),frame
	frame = (frame+.02) Mod (FRAMES-1)
	Flip
Wend