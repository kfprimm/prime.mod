
Strict

Import Prime.BlockPacker

Graphics 800,600

Type TRect
	Field x,y,width,height,flipped
	Field r,g,b
End Type

Local rects:TRect[],sx,sy,drawing
While Not KeyDown(KEY_ESCAPE) And Not AppTerminate()
	If MouseDown(1)
		If Not drawing
			drawing = True
			sx = MouseX()
			sy = MouseY()
		EndIf
	Else
		If drawing
			Local x = sx,y = sy,width = MouseX()-sx, height = MouseY()-sy
			If width <> 0 And height <> 0
				If width < 0
					x = x + width
					width = Abs(width)
				EndIf
				If height < 0
					y = y + height
					height = Abs(height)
				EndIf
				
				Local rect:TRect = New TRect
				rect.x = x
				rect.y = y
				rect.width = width
				rect.height = height
				rect.r = Rand(255)
				rect.g = Rand(255)
				rect.b = Rand(255)
				rects :+ [rect]
			EndIf
			drawing = False
		EndIf
	EndIf
	
	If KeyHit(KEY_SPACE)
		Local packer:TBlockPacker = New TBlockPacker
		For Local i = 0 Until rects.length
			packer.Add(rects[i].width, rects[i].height)
		Next
		Local width, height
		packer.Run(width, height)
		For Local i = 0 Until rects.length
			rects[i].flipped = packer.Get(i,rects[i].x,rects[i].y,rects[i].width, rects[i].height)
		Next
	EndIf
	
	Cls
	For Local r:TRect = EachIn rects
		SetColor r.r,r.g,r.b
		DrawRectOutline r.x,r.y,r.width,r.height
		SetColor 255,255,255
		If Not r.flipped
			DrawLine r.x+r.width/2, r.y+2, r.x+r.width/2, r.y+r.height-2 
		Else
			DrawLine r.x+2, r.y+r.height/2, r.x+r.width-2, r.y+r.height/2 
		EndIf
	Next
	
	If drawing
		SetColor 255,255,255
		DrawRectOutline sx,sy,MouseX()-sx,MouseY()-sy
	EndIf
	
	Flip
Wend

Function DrawRectOutline(x#,y#,width#,height#)
	DrawLine x,y,x+width,y
	DrawLine x+width,y,x+width,y+height
	DrawLine x+width,y+height,x,y+height
	DrawLine x,y+height,x,y
End Function
