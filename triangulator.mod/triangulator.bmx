
Strict

Module Prime.Triangulator
ModuleInfo "Author: Kevin Primm"
ModuleInfo "License: MIT"
ModuleInfo "Credit: Adapated from John Ratcliff's triangulator code."
ModuleInfo "Credit: http://codesuppository.blogspot.com/2009/07/polygon-triangulator-ear-clipping.html"

Import Prime.Math3D

Type TTriangulator
	Field iput:TVector[]
	Field indices[], points:TVector[]
	Field bounds:TBoundingBox = New TBoundingBox
	
	Method AddPoint(x#,y#,z#)
		AddPoints([Vec3(x,y,z)])
	End Method
	
	Method AddPoints(vectors:TVector[])
		Local index = iput.length
		iput= iput[..iput.length+vectors.length]
		For Local vec:TVector = EachIn vectors
			iput[index] = vec
			index :+ 1
			bounds.Add vec
		Next
	End Method
	
	Method Run(epsilon# = EPSILON)
		indices = Null
		If iput.length < 3 Return False
		Local dx#,dy#,dz#
		bounds.GetSize dx,dy,dz
		Local i1,i2,i3
		If dx >= dy And dx >= dz
			i1 = 0
			If dy > dz i2 = 1;i3 = 2 Else i2 = 2;i3 = 1
		ElseIf dy >= dx And dy >= dz
			i1 = 1
			If dx >= dz i2 = 0;i3 = 2 Else i2 = 2;i3 = 0
		Else
			i1 = 2
			If dx >= dy i2 = 0;i3 = 1 Else i2 = 1;i3 = 0
		EndIf
		
		points = New TVector[iput.length]
		For Local i = 0 To points.length-1
			points[i] = Vec3(iput[i].Component(i1),iput[i].Component(i2),iput[i].Component(i3))
		Next
		
		Local vi[] = New Int[points.length], flipped = False
		If 0.0 < Area()
			For Local i = 0 To points.length-1
				vi[i] = i
			Next
		Else
			For Local i = 0 To points.length-1
				vi[i] = (points.length-1) - i
			Next
			flipped = True
		EndIf
		
		Local nv = points.length
		Local count = 2 * nv
		
		Local m = 0, v = nv - 1
		While nv > 2
			If count < 1
				count :- 1
				Return
			EndIf
			
			Local u = v
			If nv <= u u = 0
			v = u + 1
			If nv <= v v = 0
			Local w = v + 1
			If nv <= w w = 0
			
			If Snip(u,v,w,nv,vi)
				Local a = vi[u], b = vi[v], c = vi[w]
				If flipped
					AddTriangle a,b,c
				Else
					AddTriangle c,b,a
				EndIf
				m :+ 1
				Local s = v, t = v + 1
				While t < nv
					vi[s] = vi[t]
					s :+ 1;	t :+ 1
				Wend
				nv :- 1
				count = 2*nv
			EndIf
				
		Wend
		
		Return indices.length/3
	End Method
	
	Method Area#()
		Local a# = 0.0, p = points.length-1
		For Local q = 0 To points.length-1
			Local pv:TVector = points[p], qv:TVector = points[q]
			A :+ pv.x*qv.y - qv.x*pv.y
			p = q
		Next
		Return A*0.5
	End Method
	
	Method Snip(u, v, w, n, vi[])
		Local a:TVector = points[vi[u]], b:TVector = points[vi[v]], c:TVector = points[vi[w]]
		If EPSILON > (((b.x - a.x) * (c.y - a.y)) - ((b.y - a.y) * (c.x - a.x))) Return False
		For Local p = 0 To n-1
			If p = u Or p = v Or p = w Continue
			If InsideTriangle(a, b, c, points[vi[p]]) Return False
		Next
		Return True
	End Method
	
	Method InsideTriangle(a:TVector, b:TVector, c:TVector, p:TVector)
		Local ax#, ay#, bx#, by#, cx#, cy#, apx#, apy#, bpx#, bpy#, cpx#, cpy#
		Local cCROSSap, bCROSScp, aCROSSbp

    ax = C.x - B.x;  ay = C.y - B.y;
    bx = A.x - C.x;  by = A.y - C.y;
    cx = B.x - A.x;  cy = B.y - A.y;
    apx = P.x - A.x;  apy = P.y - A.y;
    bpx = P.x - B.x;  bpy = P.y - B.y;
    cpx = P.x - C.x;  cpy = P.y - C.y;

    aCROSSbp = ax * bpy - ay * bpx;
    cCROSSap = cx * apy - cy * apx;
    bCROSScp = bx * cpy - by * cpx;

    Return aCROSSbp >= 0.0 And bCROSScp >= 0.0 And cCROSSap >= 0.0
	End Method
	
	Method AddTriangle(v0, v1, v2)
		indices :+ [v0,v1,v2]
	End Method
	
	Method GetTriangle(index, v0 Var,v1 Var,v2 Var)
		v0 = indices[index*3+0]
		v1 = indices[index*3+1]
		v2 = indices[index*3+2]
	End Method
	Method GetVertex(index, x# Var,y# Var, z# Var)
		x = iput[index].x
		y = iput[index].y
		z = iput[index].z
	End Method
End Type
