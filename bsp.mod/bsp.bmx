
Strict

Rem
	bbdoc: Binary space partitioning tree.
End Rem
Module Prime.BSP
ModuleInfo "Author: Kevin Primm"
ModuleInfo "License: MIT"
ModuleInfo "Credit: Adapted from the famous BSP FAQ sample code."

Import Prime.Math3D
Import BRL.LinkedList

Const BSP_IN       = -1
Const BSP_ON       =  0
Const BSP_OUT      =  1
Const BSP_SPANNING =  2

Private
Function AppendListToList(l:TList,a:TList)
	For Local o:Object=EachIn a
		l.AddLast(o)
	Next
End Function

Public

Function BSPPolygon:TBSPPolygon(pts:TVector[])
	Return New TBSPPolygon.Create(pts)
End Function

Type TBSPPolygon
	Field Plane:TPlane=New TPlane
	Field Point:TVector[], Center:TVector
	
	Method Create:TBSPPolygon(pts:TVector[])
		Point = pts[..]
		Center = Vec4(0,0,0,0)
		For Local p:TVector = EachIn Point
			Center = Center.Add(p)
		Next
		Center = Center.Scale(1.0/Point.length)
		Plane.FromPoint(Normal(), Center)
		Return Self
	End Method
	
	Method Normal:TVector()
		Local sum:TVector=New TVector
		Local last=Count()-1
		For Local i=0 To Count()-1
			sum = sum.Add(Point[last].Cross(Point[i]))
			last=i
		Next
		Return sum.Normalize()
	End Method
	
	Method Contains(pt:TVector)
		Local x,y
		Select Plane.MajorAxis()
		Case AXIS_X x=AXIS_Y;y=AXIS_Z
		Case AXIS_Y x=AXIS_Z;y=AXIS_X
		Case AXIS_Z x=AXIS_X;y=AXIS_Y
		End Select
		
		Local tx#=pt.Component(x),ty#=pt.Component(y)
		Local p1:TVector=Point[Count()-1],p2:TVector=Point[0]
		Local yflag0=p1.Component(y)>=ty,inside=False
		For Local i=0 To Count()-1
			Local yflag1=p2.Component(y)>=ty
			If yflag0<>yflag1
				Local xflag0=p1.Component(x)>=tx
				If xflag0=(p2.Component(x)>tx)
					If xflag0 inside=Not inside
				Else
					If ((p2.Component(x) - (p2.Component(y) - ty) * (p1.Component(x) - p2.Component(x)) / (p1.Component(y) - p2.Component(y))) >= tx)
						inside=Not inside
					EndIf
				EndIf
			EndIf
			yflag0=yflag1
			p1=p2
			p2=Point[i]
		Next
		Return inside
	End Method
	
	Method Invert()
		Local pts:TVector[Count()]
		Local last=Count()-1
		For Local i=0 To last
			pts[i]=Point[last-i]
		Next
		Point=pts
		Plane.Invert
	End Method
	
	Method RayIntersection#(r:TRay)
		Return Plane.RayIntersection(r)
	End Method

	Method Count()
		Return Point.length
	End Method
End Type

Type TBSPNode
	Field On:TList=New TList,BoundingBox:TBoundingBox=New TBoundingBox
	Field In:TBSPTree=New TBSPTree
	Field Out:TBSPTree=New TBSPTree
	Field Plane:TPlane=New TPlane
	Field Data:Object,Dirty
	
	Method Create:TBSPNode(poly:TBSPPolygon)
		Plane.Equals(poly.Plane)
		AddPolygon(poly)
		Return Self
	End Method
	
	Method AddPolygon(poly:TBSPPolygon)
		On.AddLast(poly)
		Dirty = True
	End Method
	
	Method Polygons()
		Return On.Count() 
	End Method
	
	Method Insert(list:TList,keep)
		Local inside:TList=New TList,outside:TList=New TList
		Local inp:TBSPPolygon,outp:TBSPPolygon
		For Local poly:TBSPPolygon=EachIn list
			Local side=Split(poly,Plane,inp,outp)
			If side=BSP_ON
				AddPolygon(poly)
			Else
				If side=BSP_IN Or side=BSP_SPANNING inside.AddLast(inp)
				If side=BSP_OUT Or side=BSP_SPANNING outside.AddLast(outp)
			EndIf				
		Next
		If Not inside.IsEmpty() In.Insert(inside,keep,BSP_IN)
		If Not outside.IsEmpty() Out.Insert(outside,keep,BSP_OUT)
	End Method
	
	Method PushFace(poly:TBSPPolygon,result:TList,keep)
		Local inside:TList=New TList,outside:TList=New TList
		Local inp:TBSPPolygon,outp:TBSPPolygon
		Local side=Split(poly,Plane,inp,outp)
		If side=BSP_ON
			result.AddLast(poly)
		Else
			If side=BSP_IN Or side=BSP_SPANNING In.PushFace(inp,result,keep,BSP_IN)
			If side=BSP_OUT Or side=BSP_SPANNING Out.PushFace(outp,result,keep,BSP_OUT)
		EndIf
	End Method
	
	Method Push(list:TList,result:TList,keep)
		Local inside:TList=New TList,outside:TList=New TList
		Local inp:TBSPPolygon,outp:TBSPPolygon
		For Local poly:TBSPPolygon=EachIn list
			Local side=Split(poly,Plane,inp,outp)
			If side=BSP_ON
				result.AddLast(poly)
			Else
				If side=BSP_IN Or side=BSP_SPANNING inside.AddLast(inp)
				If side=BSP_OUT Or side=BSP_SPANNING outside.AddLast(outp)
			EndIf
		Next
		If Not inside.IsEmpty() In.Push(inside,result,keep,BSP_IN)
		If Not outside.IsEmpty() Out.Push(outside,result,keep,BSP_OUT)
	End Method
	
	Method Invert()
		For Local poly:TBSPPolygon=EachIn On
			poly.Invert
		Next
		Plane = TPlane(Plane.Scale(-1))
		In.Invert
		Out.Invert
		Local temp:TBSPTree = In
		In = Out
		Out = temp
	End Method
	
	Method Reduce()
		Local results:TList=New TList,boundary:TList=New TList
		For Local poly:TBSPPolygon=EachIn On
			If Abs(poly.Plane.w+Plane.w)>EPSILON
				In.PushFace(poly,results,BSP_IN,BSP_IN)
				Out.Push(results,boundary,BSP_OUT,BSP_OUT)
			Else
				Out.PushFace(poly,results,BSP_OUT,BSP_OUT)
				In.Push(results,boundary,BSP_IN,BSP_IN)
			EndIf
		Next
		
		Local link:TLink = boundary.FirstLink()
		While link<>Null
			Local comp:TLink = link.NextLink(), poly0:TBSPPolygon = TBSPPolygon(link.Value())
			While comp<>Null
				Local poly1:TBSPPolygon = TBSPPolygon(comp.Value())
				If poly0.Center.IsEqual(poly1.Center)
					Local oldcomp:TLink = comp
					comp = comp.NextLink()
					oldcomp.Remove()
				Else
					comp = comp.NextLink()
				EndIf
			Wend
			link = link.NextLink()
		Wend
		
		On=boundary
		In.Reduce
		Out.Reduce
	End Method
	
	
	Method RayIntersection(r:TRay Var,poly_hit:TBSPPolygon Var,ipt:TVector Var)
		Local dist#=r.Origin.Dot(plane),costheta#=r.Direction.Dot(plane)
		If dist>EPSILON
			If Out.RayIntersection(r,poly_hit,ipt) Return True
			If costheta<-EPSILON
				If PlaneSearch(r,poly_hit,ipt) Return True
				Return In.RayIntersection(r,poly_hit,ipt)
			EndIf
		ElseIf dist<-EPSILON
			If In.RayIntersection(r,poly_hit,ipt) Return True
			If costheta>EPSILON
				If PlaneSearch(r,poly_hit,ipt) Return True
				Return Out.RayIntersection(r,poly_hit,ipt)
			EndIf
		Else
			If costheta<-EPSILON
				Return In.RayIntersection(r,poly_hit,ipt)
			ElseIf costheta>EPSILON
				Return Out.RayIntersection(r,poly_hit,ipt)
			EndIf
		EndIf
		Return False
	End Method
	
	Method PlaneSearch(r:TRay Var,poly_hit:TBSPPolygon Var,ipt:TVector Var)
		Local dist#=Plane.RayIntersection(r)
		If dist>0.0
			ipt=r.IntersectionPoint(dist)
			If BoundingBox.Contains(ipt)
				For Local poly:TBSPPolygon=EachIn On
					If poly.Contains(ipt)
						poly_hit=poly
						Return True
					EndIf
				Next
			EndIf
		EndIf
		Return False
	End Method
	
	Function Split(poly:TBSPPolygon Var,plane:TPlane Var,in:TBSPPolygon Var,out:TBSPPolygon Var)
		Local outpts:TVector[64],inpts:TVector[64]
		Local out_c,in_c
		Local ptA:TVector,ptB:TVector
		Local sideA#,sideB#
		Local poly_class = BSP_ON
		ptA=poly.Point[poly.Count()-1]
		sideA=ptA.Dot(plane)		
		For Local i=0 To poly.Count()-1
			ptB=poly.Point[i]
			sideB=ptB.Dot(plane)
			If sideB>EPSILON
				If poly_class=BSP_ON
					poly_class=BSP_OUT
				ElseIf poly_class<>BSP_OUT
					poly_class=BSP_SPANNING
				EndIf
				If sideA<-EPSILON
					Local p:TVector=plane.LineIntersection(ptA,ptB)
					outpts[out_c]=p;inpts[in_c]=p
					out_c:+1;in_c:+1
					poly_class=BSP_SPANNING
				EndIf
				outpts[out_c]=ptB
				out_c:+1
			ElseIf sideB<-EPSILON
				If poly_class=BSP_ON
					poly_class=BSP_IN
				ElseIf poly_class<>BSP_IN
					poly_class=BSP_SPANNING
				EndIf
				If sideA>EPSILON
					Local p:TVector=plane.LineIntersection(ptA,ptB)
					outpts[out_c]=p;inpts[in_c]=p
					out_c:+1;in_c:+1
					poly_class=BSP_SPANNING
				EndIf
				inpts[in_c]=ptB
				in_c:+1
			Else
				outpts[out_c]=ptB;inpts[in_c]=ptB
				out_c:+1;in_c:+1
			EndIf
			ptA=ptB
			sideA=sideB
		Next	
		Select poly_class
		Case BSP_OUT
			out=poly
		Case BSP_IN
			in=poly
		Case BSP_SPANNING
			out=New TBSPPolygon.Create(outpts[..out_c])
			in=New TBSPPolygon.Create(inpts[..in_c])
		End Select
		Return poly_class
	End Function
End Type

Type TBSPTree
	Field Node:TBSPNode
	
	Method Union:TBSPTree(tree:TBSPTree)
	
	End Method
	
	Method Subtract:TBSPTree(tree:TBSPTree)
	
	End Method
	
	Method Intersect:TBSPTree(tree:TBSPTree)
	
	End Method
	
	Method Insert(list:TList,keep,cur)
		If list.IsEmpty() Return
		If Node
			Node.Insert(list,keep)
		Else
			If cur=keep Or keep=BSP_SPANNING
				Node=New TBSPNode.Create(TBSPPolygon(list.RemoveFirst()))
				If Not list.IsEmpty() Node.Insert(list,BSP_SPANNING)
			EndIf
		EndIf
	End Method
	
	Method PushFace(poly:TBSPPolygon,result:TList,keep,cur)
		If Node
			Node.PushFace(poly,result,keep)
		Else
			If cur=keep result.AddLast(poly)
		EndIf
	End Method
	
	Method Push(list:TList,result:TList,keep,cur)
		If list.IsEmpty() Return
		If Node
			Node.Push(list,result,keep)
		Else
			If cur=keep AppendListToList(result,list)
		EndIf
	End Method
	
	Method Invert()
		If Node Node.Invert()
	End Method
	
	Method Reduce()
		If Node Node.Reduce()
	End Method
	
	Method RayIntersection(r:TRay Var,poly_hit:TBSPPolygon Var,ipt:TVector Var)
		If Node Return Node.RayIntersection(r,poly_hit,ipt)
		Return False
	End Method
	
	Method CountNodes()
		If Node = Null Return 0
		Return Node.In.CountNodes() + Node.Out.CountNodes() + 1
	End Method
End Type

