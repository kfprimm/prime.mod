
Strict

Module Prime.Enumerator
ModuleInfo "Author: Kevin Primm"

Type TObjectArrayEnumerator
	Field _objects:Object[],_pos=-1
	
	Method Create:TObjectArrayEnumerator(objects:Object[])
		_objects=objects[..]
		Return Self
	End Method
	
	Method NextObject:Object()
		_pos:+1
		Return _objects[_pos]
	End Method
	
	Method HasNext()
		Return _pos<_objects.length-1
	End Method
End Type

