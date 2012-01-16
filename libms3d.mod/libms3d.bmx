
Strict

Rem
	bbdoc: Milkshape 3D file format library.
End Rem
Module Prime.libMS3D
ModuleInfo "Author: Kevin Primm"
ModuleInfo "License: MIT"

Import BRL.Stream

Const MS3D_FILE_ID$ = "MS3D000000"

Type TMS3DFile
	Field id$
	Field version
	
	Field AnimationFPS#
	Field CurrentTime#
	Field TotalFrames
	
	Field Vertices:TMS3DVertex[]
	Field Triangles:TMS3DTriangle[]
	Field Groups:TMS3DGroup[]
	Field Materials:TMS3DMaterial[]
	Field Joints:TMS3DJoint[]
	
	Method Read:TMS3DFile(url:Object)
		Local stream:TStream=ReadStream(url)
		If stream=Null Return Null
		
		id=ReadString(stream,10)
		If id<>MS3D_FILE_ID
			CloseStream stream
			Return Null
		EndIf
		
		version=ReadInt(stream)
		If version<>4 
			CloseStream stream
			Return Null
		EndIf		
		
		Local vertex_count=ReadShort(stream)
		Vertices=New TMS3DVertex[vertex_count]
		For Local i=0 To vertex_count-1
			Vertices[i]=New TMS3DVertex
			Vertices[i].flags=ReadByte(stream)
			Vertices[i].vertex[0]=ReadFloat(stream)
			Vertices[i].vertex[1]=ReadFloat(stream)
			Vertices[i].vertex[2]=ReadFloat(stream)
			Vertices[i].boneID=ReadByte(stream)
			Vertices[i].referenceCount=ReadByte(stream)
		Next
		
		Local triangle_count=ReadShort(stream)
		Triangles=New TMS3DTriangle[triangle_count]
		For Local i=0 To triangle_count-1
			Triangles[i]=New TMS3DTriangle
			Triangles[i].flags=ReadShort(stream)
			Triangles[i].vertexIndices[0]=ReadShort(stream)
			Triangles[i].vertexIndices[1]=ReadShort(stream)
			Triangles[i].vertexIndices[2]=ReadShort(stream)
			For Local j=0 To 2
				Triangles[i].vertexNormals[j,0]=ReadFloat(stream)
				Triangles[i].vertexNormals[j,1]=ReadFloat(stream)
				Triangles[i].vertexNormals[j,2]=ReadFloat(stream)
			Next
			Triangles[i].s[0]=ReadFloat(stream)
			Triangles[i].s[1]=ReadFloat(stream)
			Triangles[i].s[2]=ReadFloat(stream)
			Triangles[i].t[0]=ReadFloat(stream)
			Triangles[i].t[1]=ReadFloat(stream)
			Triangles[i].t[2]=ReadFloat(stream)
			Triangles[i].smoothingGroup=ReadByte(stream)
			Triangles[i].groupIndex=ReadByte(stream)
		Next		
		
		Local group_count=ReadShort(stream)
		Groups=New TMS3DGroup[group_count]
		For Local i=0 To group_count-1
			Groups[i]=New TMS3DGroup
			Groups[i].flags=ReadByte(stream)
			Groups[i].name=ReadCString(stream,32)
			Groups[i].numtriangles=ReadShort(stream)
			Groups[i].triangleIndices=New Int[Groups[i].numtriangles]
			For Local j=0 To Groups[i].numtriangles-1
				Groups[i].triangleIndices[j]=ReadShort(stream)
			Next
			Groups[i].materialIndex=ReadByte(stream)
		Next
		
		Local material_count=ReadShort(stream)
		Materials=New TMS3DMaterial[material_count]
		For Local i=0 To material_count-1
			Materials[i]=New TMS3DMaterial
			Materials[i].name=ReadCString(stream,32)
			stream.ReadBytes Materials[i].ambient,4*4
			stream.ReadBytes Materials[i].ambient,4*4
			stream.ReadBytes Materials[i].ambient,4*4
			stream.ReadBytes Materials[i].ambient,4*4
			Materials[i].shininess=ReadFloat(stream)
			Materials[i].transparency=ReadFloat(stream)
			Materials[i].mode=ReadByte(stream)
			Materials[i].texture=ReadCString(stream,128)
			Materials[i].alphamap=ReadCString(stream,128)
		Next
		
		AnimationFPS=ReadFloat(stream)
		CurrentTime=ReadFloat(stream)
		TotalFrames=ReadInt(stream)
		
		Local joint_count=ReadShort(stream)
		Joints=New TMS3DJoint[joint_count]
		For Local i=0 To joint_count-1
			Joints[i]=New TMS3DJoint
			Joints[i].flags=ReadByte(stream)
			Joints[i].name=ReadCString(stream,32)
			Joints[i].parentName=ReadCString(stream,32)
			stream.ReadBytes Joints[i].rotation,4*3
			stream.ReadBytes Joints[i].position,4*3
			Joints[i].numKeyFramesRot=ReadShort(stream)
			Joints[i].numKeyFramesTrans=ReadShort(stream)
			Joints[i].keyFramesRot=New TMS3DKeyframe[Joints[i].numKeyFramesRot]
			For Local j=0 To Joints[i].numKeyFramesRot-1
				Joints[i].keyFramesRot[j]=New TMS3DKeyframe
				Joints[i].keyFramesRot[j].time=ReadFloat(stream)
				stream.ReadBytes Joints[i].keyFramesRot[j].xyz,4*3
			Next
			Joints[i].keyFramesTrans=New TMS3DKeyframe[Joints[i].numKeyFramesTrans]
			For Local j=0 To Joints[i].numKeyFramesTrans-1
				Joints[i].keyFramesTrans[j]=New TMS3DKeyframe
				Joints[i].keyFramesTrans[j].time=ReadFloat(stream)
				stream.ReadBytes Joints[i].keyFramesTrans[j].xyz,4*3
			Next
		Next
		
		CloseStream stream		
		Return Self
	End Method
	
	Method ReadCString$(stream:TStream,count)
		Local buf:Byte[count]
		stream.ReadBytes buf,count
		Return String.FromCString(buf)
	End Method
	
	Method ObjectEnumerator:Object()
		Return New TMS3DEnumerator.Create(Vertices+Triangles+Groups+Materials+Joints)
	End Method
End Type

Type TMS3DEnumerator
	Field _objs:Object[],_pos=-1
	
	Method Create:TMS3DEnumerator(objs:Object[])
		_objs=objs
		Return Self
	End Method
	
	Method HasNext()
		Return _pos>_objs.length-1
	End Method	
	Method NextObject:Object()
		_pos:+1
		Return _objs[_pos]
	End Method
End Type

Type TMS3DVertex
	Field flags
	Field vertex#[3]
	Field boneID
	Field referenceCount
End Type

Type TMS3DTriangle
	Field flags
	Field vertexIndices[3]
	Field vertexNormals#[3,3]
	Field s#[3],t#[3]
	Field smoothingGroup
	Field groupIndex
End Type

Type TMS3DEdge
	Field edgeIndices[2]
End Type

Type TMS3DGroup
	Field flags
	Field name$
	Field numtriangles
	Field triangleIndices[]
	Field materialIndex
End Type

Type TMS3DMaterial
	Field name$
	Field ambient#[4]
	Field diffuse#[4]
	Field specular#[4]
	Field emissive#[4]
	Field shininess#
	Field transparency#
	Field mode
	Field texture$
	Field alphamap$
End Type

Type TMS3DKeyframe
	Field time#
	Field xyz#[3]
End Type

Type TMS3DJoint
	Field flags
	Field name$
	Field parentName$
	Field rotation#[3]
	Field position#[3]
	
	Field numKeyFramesRot
	Field numKeyFramesTrans
	
	Field keyFramesRot:TMS3DKeyframe[]
	Field keyFramesTrans:TMS3DKeyframe[]
End Type

