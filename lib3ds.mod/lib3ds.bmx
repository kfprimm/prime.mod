
Strict

Module Prime.lib3ds
ModuleInfo "Author: Kevin Primm"
ModuleInfo "License: MIT"
ModuleInfo "Credit: Based on Markus Rauch's BlitzBasic code archives entry."

Import BRL.Stream
Import BRL.Retro ' Just for testing.

Const A3DS_PRIMARY = $4D4D

Const A3DS_EDIT3DS           =$3D3D  ' Start of our actual objects
Const A3DS_KEYF3DS           =$B000  ' Start of the keyframe information

Const A3DS_VERSION           =$0002 'ok
Const A3DS_MESH_VERSION      =$3D3E
Const A3DS_KFVERSION         =$0005
Const A3DS_COLOR_F           =$0010
Const A3DS_COLOR_24          =$0011
Const A3DS_LIN_COLOR_24      =$0012
Const A3DS_LIN_COLOR_F       =$0013
Const A3DS_INT_PERCENTAGE    =$0030
Const A3DS_FLOAT_PERC        =$0031
Const A3DS_MASTER_SCALE      =$0100
Const A3DS_IMAGE_FILE        =$1100
Const A3DS_AMBIENT_LIGHT     =$2100

Const A3DS_NAMED_OBJECT      =$4000 'ok
Const A3DS_OBJ_MESH          =$4100 'ok
Const A3DS_MESH_VERTICES     =$4110 'ok
Const A3DS_VERTEX_FLAGS      =$4111
Const A3DS_MESH_FACES        =$4120 'ok
Const A3DS_MESH_MATER        =$4130 'ok
Const A3DS_MESH_TEX_VERT     =$4140 'ok
Const A3DS_MESH_XFMATRIX     =$4160 'ok Local coordinate system
Const A3DS_MESH_COLOR_IND    =$4165
Const A3DS_MESH_TEX_INFO     =$4170
Const A3DS_HEIRARCHY         =$4F00 '? skip

Const A3DS_MATERIAL          =$AFFF 'ok
Const A3DS_MAT_NAME          =$A000 'ok
Const A3DS_MAT_AMBIENT       =$A010 'ok
Const A3DS_MAT_DIFFUSE       =$A020 'ok
Const A3DS_MAT_SPECULAR      =$A030 'ok
Const A3DS_MAT_SHININESS     =$A040 '...
Const A3DS_MAT_FALLOFF       =$A052 'ok
Const A3DS_MAT_EMISSIVE      =$A080 'ok
Const A3DS_MAT_SHADER        =$A100
Const A3DS_MAT_TEXMAP        =$A200 'ok
Const A3DS_MAT_TEXFLNM       =$A300 'ok

Const A3DS_OBJ_LIGHT         =$4600
Const A3DS_OBJ_CAMERA        =$4700

Const A3DS_ANIM_HEADER       =$B00A 'skip
Const A3DS_ANIM_OBJ          =$B002 'ok
Const A3DS_ANIM_S_E_TIME     =$B008 'ok
Const A3DS_ANIM_NAME         =$B010 'ok
Const A3DS_ANIM_PIVOT        =$B013 'ok
Const A3DS_ANIM_POS          =$B020 'ok
Const A3DS_ANIM_ROT          =$B021 'ok 
Const A3DS_ANIM_SCALE        =$B022 'ok
Const A3DS_ANIM_HIERARCHYPOS =$B030 'ok

Type T3DSColor
	Field r#=1.0,g#=1.0,b#=1.0
End Type

Type T3DSMaterial
	Field Name$
	Field AmbientColor:T3DSColor=New T3DSColor
	Field DiffuseColor:T3DSColor=New T3DSColor
	Field SpecularColor:T3DSColor=New T3DSColor
	Field EmissiveColor:T3DSColor=New T3DSColor
	Field TextureFile$
End Type

Type T3DSVertex
	Field x#,y#,z#
End Type

Type T3DSFace
	Field a,b,c
	Field Material:T3DSMaterial
End Type

Type T3DSTex
	Field u#,v#
End Type

Type T3DSMesh
	Field Name$
	Field Vertices:T3DSVertex[]
	Field Faces:T3DSFace[]
	Field Texs:T3DSTex[]
	Field OriginX#,OriginY#,OriginZ#
	
  Field AxisXX#,AxisXY#,AxisXZ#
  Field AxisYX#,AxisYY#,AxisYZ#
  Field AxisZX#,AxisZY#,AxisZZ#
End Type

Type T3DSAnimPosition
	Field Frame
	Field x#,y#,z#
End Type

Type T3DSAnimScale
	Field Frame
	Field x#,y#,z#
End Type

Type T3DSAnimRotation
	Field Frame
	Field Rotation# ' In radians
	Field AxisX#
	Field AxisY#
	Field AxisZ#	
End Type

Type T3DSAnimation
	Field Parent,Pos
	Field Name$
	Field PosKeys:T3DSAnimPosition[]
	Field RotKeys:T3DSAnimRotation[]
	Field ScaleKeys:T3DSAnimScale[]
End Type

Type T3DSFile
	Field Version
	Field Materials:T3DSMaterial[]
	Field Meshes:T3DSMesh[]
	Field Animations:T3DSAnimation[]
	
	Function Read:T3DSFile(url:Object)
		Local stream:TStream=ReadStream(url)
		If stream=Null Return Null
		
		Local file:T3DSFile=New T3DSFile
		If file.ParseChunk(stream)<>A3DS_PRIMARY
			CloseStream stream
			Return Null
		EndIf
		CloseStream stream
		Return file
	End Function
	
	Method DumpInfo()
		DebugLog "Version: "+Version
		DebugLog "Material count: "+Materials.length
		DebugLog "Mesh count: "+Meshes.length
		DebugLog "Animation count: "+Animations.length
	End Method
	
	Method SkipChunk(stream:TStream,pos,length)
		SeekStream stream,(pos-6)+length
	End Method
	
	Method SkipAhead(stream:TStream,length)
		SeekStream stream,StreamPos(stream)+length
	End Method
	
	Method ParseChunk(stream:TStream)		
		Global _material:T3DSMaterial,_mesh:T3DSMesh,_animation:T3DSAnimation
		Global _color:T3DSColor
		
		If Eof(stream) Return				
		Local id=ReadShort(stream),length=ReadInt(stream),pos=StreamPos(stream)

		Select id
		Case A3DS_EDIT3DS
		Case A3DS_PRIMARY
		Case A3DS_VERSION
			Version=ReadInt(stream)
		'Case A3DS_MESH_VERSION
		'	DebugLog "MESH_VERSION"
		'	SkipChunk stream,pos,length
		'	ParseChunk stream
		Case A3DS_MATERIAL
			_material=New T3DSMaterial
			Materials:+[_material]
		Case A3DS_MAT_NAME
			_material.Name=ReadCString(stream)
		Case A3DS_MAT_AMBIENT
			_color=_material.AmbientColor
			ParseChunk stream
		Case A3DS_MAT_DIFFUSE
			_color=_material.DiffuseColor
			ParseChunk stream
		Case A3DS_MAT_SPECULAR
			_color=_material.SpecularColor
			ParseChunk stream
		'Case A3DS_MAT_SHININESS '? in %
			'GetShininessColour(tempChunk)		
		'Case A3DS_MAT_FALLOFF
			'GetFallOffColour(tempChunk)		
		Case A3DS_MAT_EMISSIVE
			_color=_material.EmissiveColor
			ParseChunk stream
		Case A3DS_MAT_TEXMAP
			DebugLog "MAT_TEXMAP"
		Case A3DS_MAT_TEXFLNM
			_material.TextureFile=ReadCString(stream)	
		Case A3DS_COLOR_F
			_color.r=ReadFloat(stream)
			_color.g=ReadFloat(stream)
			_color.b=ReadFloat(stream)
		Case A3DS_COLOR_24
			_color.r=ReadByte(stream)/255.0
			_color.g=ReadByte(stream)/255.0
			_color.b=ReadByte(stream)/255.0
		Case A3DS_NAMED_OBJECT
			_mesh=New T3DSMesh
			Meshes:+[_mesh]
			_mesh.Name=ReadCString(stream)
		Case A3DS_OBJ_MESH
		Case A3DS_MESH_VERTICES
			Local vertices:T3DSVertex[]=New T3DSVertex[ReadShort(stream)]
			For Local i=0 To vertices.length-1
				vertices[i]=New T3DSVertex
				vertices[i].x=ReadFloat(stream)
				vertices[i].y=ReadFloat(stream)
				vertices[i].z=ReadFloat(stream)
			Next
			_mesh.Vertices=vertices
			SkipChunk stream,pos,length
		Case A3DS_MESH_TEX_VERT '0x4140
			Local texs:T3DSTex[]=New T3DSTex[ReadShort(stream)]
			For Local i=0 To texs.length-1
				texs[i]=New T3DSTex
				texs[i].u=ReadFloat(stream)
				texs[i].v=ReadFloat(stream)
			Next	
			_mesh.Texs=texs
			SkipChunk stream,pos,length	
		Case A3DS_MESH_FACES '0x4120
			Local faces:T3DSFace[]=New T3DSFace[ReadShort(stream)]
			For Local i=0 To faces.length-1
				faces[i]=New T3DSFace
				faces[i].a=ReadShort(stream)
				faces[i].b=ReadShort(stream)
				faces[i].c=ReadShort(stream)
				Local flag=ReadShort(stream)
			Next
			_mesh.Faces=faces
		Case A3DS_MESH_MATER '0x4130
			Local name$=ReadCString(stream)	
			Local count=ReadShort(stream)
			For Local i=0 To count-1
				Local face=ReadShort(stream)
				For Local material:T3DSMaterial=EachIn Materials
					If material.Name=name _mesh.Faces[face].Material=material
				Next
			Next
		Case A3DS_MESH_XFMATRIX
			_mesh.AxisXX=ReadFloat(stream)
			_mesh.AxisXY=ReadFloat(stream)
			_mesh.AxisXZ=ReadFloat(stream)
			
			_mesh.AxisYX=ReadFloat(stream)
			_mesh.AxisYY=ReadFloat(stream)
			_mesh.AxisYZ=ReadFloat(stream)
			
			_mesh.AxisZX=ReadFloat(stream)
			_mesh.AxisZY=ReadFloat(stream)
			_mesh.AxisZZ=ReadFloat(stream)
			
			_mesh.OriginX=ReadFloat(stream)
			_mesh.OriginY=ReadFloat(stream)
			_mesh.OriginZ=ReadFloat(stream)				
		Case A3DS_HEIRARCHY
			SkipChunk stream,pos,length	
		Case A3DS_KEYF3DS
		Case A3DS_ANIM_HEADER
			SkipChunk stream,pos,length		
		Case A3DS_ANIM_S_E_TIME '0xB008
			'StartEndFrames(tempChunk)		
		Case A3DS_ANIM_OBJ '0xB002
			_animation=New T3DSAnimation
			Animations:+[_animation]
		Case A3DS_ANIM_NAME
			_animation.Name=ReadCString(stream)
			Local flag1=ReadShort(stream)
			Local flag2=ReadShort(stream)
			_animation.Parent=ReadShort(stream)
			SkipChunk stream,pos,length
		Case A3DS_ANIM_HIERARCHYPOS
			_animation.Pos=ReadShort(stream)
		Case A3DS_ANIM_PIVOT '0xB013
			Local x#=ReadFloat(stream)
			Local y#=ReadFloat(stream)
			Local z#=ReadFloat(stream)
		Case A3DS_ANIM_POS '0xB020
			DebugLog "ANIM_POS"
			'SkipAhead stream,10
			'Local keys=ReadShort(stream)
			'SkipAhead stream,2
			SkipChunk stream,pos,length
		Case A3DS_ANIM_SCALE '0xB022
			DebugLog "ANIM_SCALE"
			'ReadAnimScale(tempChunk)
			SkipChunk stream,pos,length
		Case A3DS_ANIM_ROT '0xB021
			DebugLog "ANIM_ROT"
			'ReadAnimRot(tempChunk)			
			SkipChunk stream,pos,length
		Default
			DebugLog "Skipped! ID: "+Hex(id)+" size="+length
			SkipAhead stream,length-6
		End Select
		ParseChunk stream
		Return id
	End Method	
		
	Function ReadCString$(stream:TStream)
		Local str$,c=ReadByte(stream)
		While c<>0
			str:+Chr(c)
			c=ReadByte(stream)
		Wend
		Return str.Trim()
	End Function
	
	Function WriteCString(stream:TStream,str$)
		For Local i=0 To str.length-1
			WriteByte stream,str[i]
		Next
		WriteByte stream,0
	End Function
End Type
