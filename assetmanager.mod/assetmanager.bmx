
Strict

Module sys87.AssetManager
ModuleInfo "Author: Kevin Primm"

Import BRL.Map
Import BAH.libxml

Type TAssetManager
	Field _assets:TMap=CreateMap()
	
	Method Load(url$)
		Local doc:TxmlDoc=TxmlDoc.parseFile(url$)
		Local root:TxmlNode=doc.getRootElement()
		If root.getName()<>"assets" Return False
		For Local node:TxmlNode=EachIn root.getChildren()
			Local params:TMap=CreateMap()
			For Local attr$=EachIn node.getAttributeList()
				If attr<>"id" And attr<>"url" params.Insert(attr,node.getAttribute(attr))
			Next
			Local obj:Object=TAssetLoader.Load(node.getAttribute("url"),node.getName(),params)
			_assets.Insert(node.getAttribute("id"),obj)		
		Next
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
		Return Int(String(Param(key,String(def))))
	End Method
	
	Method ParamExists(key$)
		Return _params.ValueForKey(key)<>Null
	End Method
	
	Method Run:Object(url:Object) Abstract
	Method GetType$() Abstract
End Type