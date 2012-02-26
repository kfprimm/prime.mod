
Strict

Import Prime.Triangulator

Graphics 600,600
SetBlend ALPHABLEND

Local triangulator:TTriangulator = New TTriangulator

Local processed, xy#[], triangles
While Not KeyDown(KEY_ESCAPE) And Not AppTerminate()
	Cls
	
	If MouseHit(1)
		Local x# = MouseX(), y# = MouseY()
		xy :+ [x, y]
		triangulator.AddPoint x, y, 0
	End If
	
	If MouseHit(2)
		triangulator.Run
		processed = True
	EndIf
	
	If Not processed
		SetAlpha .5
		DrawPoly xy
		SetAlpha 1
		Local x#, y#
		If xy.length > 1
			x = xy[xy.length-2]
			y = xy[xy.length-1]
		EndIf
		For Local i = 0 To xy.length-1 Step 2
			DrawRect xy[i+0]-1.5,xy[i+1]-1.5,3,3
			DrawLine xy[i+0],xy[i+1],x,y
			x = xy[i+0]
			y = xy[i+1]
		Next
	Else		
		For Local i = 0 To (triangulator.indices.length/3)-1
			Local v0,v1,v2
			triangulator.GetTriangle i,v0,v1,v2
			Local x0#,y0#,z0#,x1#,y1#,z1#,x2#,y2#,z2#
			triangulator.GetVertex v0,x0,y0,z0
			triangulator.GetVertex v1,x1,y1,z1
			triangulator.GetVertex v2,x2,y2,z2
			
			SetColor 128,128,128
			DrawPoly ([x0, y0, x1, y1, x2, y2])
			SetColor 255,255,255
			DrawLine x0, y0, x1, y1
			DrawLine x1, y1, x2, y2
			DrawLine x2, y2, x0, y0
		Next
	EndIf
	
	Flip
Wend
