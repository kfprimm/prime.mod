
Strict

Module sys87.AssetManager
ModuleInfo "Author: Kevin Primm"
ModuleInfo "License: MIT"

Import BRL.FileSystem
Import BRL.Map
Import BRL.TextStream
Import PUB.MaXML

Type TAssetManager
	Field _assets:TMap=CreateMap()
	
	Method Load(url:Object)
		Local doc:xmlDocument=New xmlDocument
		doc.Load url
		Local root:xmlNode=doc.Root()
		If root.Name<>"assets" Return False
		For Local node:xmlNode=EachIn root.ChildList
			ParseNode(node,"",root.Attribute("dir").Value+"/")
		Next
	End Method
	
	Method ParseNode(node:xmlNode,id$,dir$)
		If node.Name<>"group"
			Local params:TMap=CreateMap()
			For Local attr:xmlAttribute=EachIn node.AttributeList
				If attr.Name<>"id" And attr.Name<>"url" params.Insert(attr.Name,attr.Value)
			Next
			Local obj:Object=TAssetLoader.Load(dir+node.Attribute("url").Value,node.Name,params)
			If obj<>Null
				Local obj_id$
				If Not node.HasAttribute("id")
					obj_id=StripAll(node.Attribute("url").Value)
				Else
					obj_id=node.Attribute("id").Value
				EndIf
				_assets.Insert(id+obj_id,obj)
			Else
				DebugLog "Failed to load ~q"+dir+node.Attribute("url").Value+"~q"
			EndIf
		Else			
			If node.HasAttribute("dir")
				dir:+node.Attribute("dir").Value
				If node.HasAttribute("id") id:+node.Attribute("id").Value+"_"
			Else
				If node.HasAttribute("id")
					dir:+node.Attribute("id").Value+"/"
					id:+node.Attribute("id").Value+"_"
				EndIf
			EndIf
			For Local child:xmlNode=EachIn node.ChildList
				ParseNode(child,id,dir)
			Next
		EndIf
	End Method
	
	Method List$[]()
		Local l$[]
		For Local key:Object=EachIn MapKeys(_assets)
			l=l[..l.length+1]
			l[l.length-1]=String(key)
		Next
		Return l
	End Method
	
	Method Get:Object(id$)
		Return _assets.ValueForKey(id)
	End Method
End Type

Type TAssetLoader
	Global _first:TAssetLoader
	Field _next:TAssetLoader,_params:TMap
	
	Method New()
		If _first=Null
			_first=Self
		Else
			Local loader:TAssetLoader=_first
			While loader._next
				loader=loader._next
			Wend
			loader._next=Self
		EndIf
	End Method
	
	Function Load:Object(url:Object,typ$="",params:TMap=Null)
		Local loader:TAssetLoader=_first
		While loader
			If (loader.GetType()=typ And typ<>"") Or typ=""
				loader._params=params
				Local obj:Object=loader.Run(url)
				loader._params=Null
				If obj Return obj
			EndIf
			loader=loader._next
		Wend
	End Function
	
	Method GetStream:TStream(url:Object)
		Local stream:TStream=TStream(url)
		If stream=Null stream=ReadStream(url)
		If stream=Null Return Null
	End Method
	
	Method Param:Object(key$,def:Object=Null)
		If Not ParamExists(key) Return def
		Return _params.ValueForKey(key)
	End Method
	
	Method IParam(key$,def=0)
		Return Int(SParam(key,def))
	End Method
	
	Method SParam$(key$,def$="")
		Return String(Param(key,String(def)))
	End Method
	
	Method SAParam$[](key$,delim$,def$="")
		Return SParam(key,def).Split(delim)
	End Method
	
	Method ParamExists(key$)
		Return _params.ValueForKey(key)<>Null
	End Method
	
	Method Run:Object(url:Object) Abstract
	Method GetType$() Abstract
End Type