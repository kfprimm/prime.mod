'MaXML 2.22
'Copyright (C) 2006  John Judnich

Strict

Rem
bbdoc: MaXML
endrem
Module Prime.MaXML

ModuleInfo "Description: MaXML allows you to load, manage, and save XML data quickly and easily."
ModuleInfo "Version: 2.22"
ModuleInfo "Author: John Judnich" 
ModuleInfo "License: Free for commercial and non-commercial use" 
ModuleInfo "Copyright: John Judnich" 

Import BRL.LinkedList
Import BRL.Stream
Import BRL.Bank
Import BRL.Retro

Const AS_CHILD = 1
Const AS_SIBLING = 2

Const FORMAT_XML = 1
Const FORMAT_BINARY = 2

Const SORTBY_NODE_NAME = 1
Const SORTBY_NODE_VALUE = 2
Const SORTBY_ATTR_NAME = 3
Const SORTBY_ATTR_VALUE = 4

Rem
bbdoc: The xmlDocument type represents a XML data structure.
about: An XML data structure is a "tree" of items (called "nodes"). Each node can contain data, such as a name,
value, And an unlimited number of attributes (see Type xmlNode for more info). A xmlDocument can be created
with xmlDocument.Create(), or simply with the "New" keyword. To delete an xmlDocument, simply ensure that no
variables are referencing it, and BlitzMax will take care of the rest.
endrem
Type xmlDocument
	Field _RootNode:xmlNode
	
	Method Delete()
		Clear()
	End Method
	
	Rem
	bbdoc: This function creates and returns a new, blank xmlDocument.
	about: If you want to initialize the xmlDocument with contents from a file, specify a file in "Url", and
	it will be loaded. Note: This is the exact equivelant of simply using BlitzMax's "New" keyword, along with
	the xmlDocument.Load() method if a file is specified.
	Example: Local doc:xmlDocument = xmlDocument.Create()
	endrem
	Function Create:xmlDocument(Url:Object = "")
		Local doc:xmlDocument = New xmlDocument
		If Url <> "" Then doc.Load(Url)
		Return doc
	End Function
	
	Rem
	bbdoc: Returns the xmlDocument's root node.
	about: If the xmlDocument does not contain a root node, a new one will be created (this would occur when
	an xmlDocument has just been created, or if you deleted the root node). The root node is what you will need
	to perform all operations on the xmlDocument's data (see Type xmlNode for more info on manipulating XML data).
	Example: doc.Root().Name = "root_node"
	endrem
	Method Root:xmlNode()
		If _RootNode = Null Then
			_RootNode = New xmlNode
			_RootNode.AttributeList = New TList
			_RootNode.ChildList = New TList
			_RootNode.Level = 0
			_RootNode.Parent = Null
			_RootNode.Name = "root"
			_RootNode.Value = ""
			_RootNode._Doc = Self
		End If
		
		Return _RootNode
	End Method
	
	Rem
	bbdoc: Reads the specified XML file into the xmlDocument.
	about: MaXML will automatically detect whether the specified file is in standard XML format, or MaXML Binary. Any
	nodes existing in the xmlDocument will be deleted before the file is loaded. Note: The file name may have any
	extension (not just ".xml").
	Example: doc.Load "myfile.xml"
	endrem
	Method Load(Url:Object)
		Local File:TStream
		Local Format:Int
		
		'Detect the file format
		Format = FORMAT_XML
		File = OpenStream(Url, True, False)
		If File = Null Then Throw "xmlDocument.Load(~q"+String(Url)+"~q): Cannot read from file."
		If File.ReadByte() = 216 Then
			If File.ReadByte() = 241 Then
				If File.ReadByte() = 188 Then
					Format = FORMAT_BINARY
				End If
			End If
		End If
		File.Close()
		
		Select Format
			Case FORMAT_XML
				'Setup the parser
				_InitTokenizer(Url)
			    
				'Empty the xmlDocument before loading
				Clear()
		
				'Read the XML file
				_Parse(Self, Null)		
			
			Case FORMAT_BINARY
				'Setup the parser
				_InitLoader_Binary(Url)
			    
				'Empty the xmlDocument before loading
				Clear()
				
				'Read the Binary file
				_Load_Binary(Self, Null)
			
		End Select
	End Method
	
	Rem
	bbdoc: Saves the xmlDocument's nodes to the specified filename.
	about: "Format" should be set to either FORMAT_XML or FORMAT_BINARY. Saving in XML format makes your files easily readible and
	editable from any text editor, while using the BINARY format loads and saves about 50% faster than plain XML.
	Note: The file name may have any extension (not just ".xml").
	endrem
	Method Save(Url:Object, Format = FORMAT_XML)
		Local File:TStream
		
		'Open the file for writing
		File = OpenStream(Url, False, True)
		If File = Null Then Throw "xmlDocument.Save(~q"+String(Url)+"~q): Cannot write to file."
		
		Select Format
			Case FORMAT_XML
				'Write the XML header
				File.WriteLine "<?xml version=~q1.0~q?>"
				
				'Write all the nodes
				If _RootNode <> Null Then
					_WriteNode(File, _RootNode)
				End If
				
			Case FORMAT_BINARY
				'Write the binary header
				File.WriteByte 216
				File.WriteByte 241
				File.WriteByte 188

				'Write all the nodes
				If _RootNode <> Null Then
					_WriteNode_Binary(File, _RootNode)
				End If
				
		End Select
				
		'Close the file
		File.Close()
	End Method
	
	Rem
	bbdoc: Returns the number of nodes in the xmlDocument
	about: Unlike using xmlDocument.Root().ChildCount(), this method counts ALL the nodes in the XML document.
	endrem
	Method NodeCount()
		Return _CountNodes( Root() )
	End Method
	
	Rem
	bbdoc: Not yet implimented.
	endrem
	Method SetEncryptionKey(Key:String)		
	End Method
	
	Rem
	bbdoc: Deletes all the xmlDocument's nodes.
	about: Calling this will make the xmlDocument completely blank.
	Example: doc.Clear
	endrem
	Method Clear()
		If _RootNode <> Null Then _RootNode.Free()
	End Method
	
End Type
 
Rem
bbdoc: The xmlNode Type is used To modify And access an node's data.
about: Each xmlNode may contain a name, a value, attributes, and sub-nodes. Each attribute has it's own name and value. The
xmlNode contains a wide variety of methods providing you with easy access to any of this data.
endrem
Type xmlNode
	Rem
	bbdoc: The name of the xmlNode.
	about: Use the Name field to modify or read the name of an xmlNode.
	Example: doc.Root().Name = "MapFile"
	endrem
	Field Name:String

	Rem
	bbdoc: The value of the xmlNode
	about: Use the Value field to modify or read the value of an xmlNode. Note: If you need multiple values, or values with names,
	try using attributes instead.
	Example: node.Value = "whatever"
	endrem
	Field Value:String
	
	Rem
	bbdoc: The node's parent.
	about: This field links the node to it's "parent". For example, if a node named "transistor" was a inside (a child of) a node
	named "computer", then the "computer" node would be the parent of the "transistor" node. Note: If this is the root node, it 
	has no parent, and therefore this variable will be Null. WARNING: DO NOT MODIFY THIS VARIABLE
	Example: node.Parent.Name = "test"
	endrem
	Field Parent:xmlNode 
	
	Rem
	bbdoc: The level of a node.
	about: This represents the node's "level", which indicates how many parents it has. For example, the root node has a level of 0.
	A child of the root is a level 1 node, and a child of that node has is a level 2 node, etc. etc. A child node's level will always
	be 1 greater than it's parent. Similarly, "sibling" nodes (nodes that share the same parent) will always share the same level.
	Example: If node.Level = 1 Then ...
	endrem
	Field Level:Int
	
	Field AttributeList:TList  
	Field ChildList:TList
	
	Field _Link:TLink
	
	Field _Doc:xmlDocument

	'----------- Node Control ----------------------------------------
	'Node control methods get/set node-related information
	
	Rem
	bbdoc: Adds a new node to the current node (as a child, or as a sibling), giving it the specified name.
	about: By default, this will add the new node as a child of the current node. Optionally, the "Position" option may be
	used to add the node as a child or as a sibling. Valid "Position" values are: AS_CHILD, and AS_SIBLING.
	Note: Any of the node's properties, including it's name, can be modified later.
	Example: doc.Root().AddNode("mynode", AS_CHILD)
	endrem
	Method AddNode:xmlNode(Name:String, Position:Int = AS_CHILD)
	    Local newnode:xmlNode
		
		newnode = New xmlNode
		newnode.AttributeList = New TList
		newnode.ChildList = New TList
		
		newnode._Doc = Self._Doc
		
		newnode.Name = Name
		newnode.Value = ""
		
		'Insert into location
		Select Position
			Case AS_CHILD
				newnode.Parent = Self
				newnode._Link = newnode.Parent.ChildList.AddLast(newnode)
				
			Case AS_SIBLING
				If Parent = Null Then Throw "xmlNode.AddNode(~q"+Name+"~q, AS_SIBLING): The root node may not have siblings."
				newnode.Parent = Self.Parent
				newnode._Link = newnode.Parent.ChildList.InsertAfterLink(newnode, Self._Link)
		End Select
		
		'Update level variables
		newnode._UpdateVars()
		 
		Return newnode
	End Method
	
	Rem
	bbdoc: Moves the current node (and all it's children) to the specified location (as a child, or as a sibling)
	about: By default, this will move the node into the specified destination "Dest", as a child. Optionally, you can
	use the "Position" variable to instruct MaXML to make it a sibling (AS_SIBLING) or a child (AS_CHILD).
	Example: configData.MoveTo settingsNode, AS_CHILD
	endrem
	Method MoveTo(Dest:xmlNode, Position:Int = AS_CHILD)
	    'Check for misuse
	    Local temp:xmlNode
	    If Self.Parent = Null Then
	    	If Position = AS_CHILD Then Throw "xmlNode.MoveTo(xmlNode, AS_CHILD): Cannot move root node." 
	    	If Position = AS_SIBLING Then Throw "xmlNode.MoveTo(xmlNode, AS_SIBLING): Cannot move root node."
	    End If
	    If Dest = Self Then Return
	    temp = Dest
	    Repeat
	    	temp = temp.Parent
	    	If temp = Self Then
	    		If Position = AS_CHILD Then Throw "xmlNode.MoveTo(xmlNode, AS_CHILD): Cannot move a node into one of it's children."
	    		If Position = AS_SIBLING Then Throw "xmlNode.MoveTo(xmlNode, AS_SIBLING): Cannot move a node to one of it's children."
	    	End If
	    Until temp = Null	
		
	    'Remove from current location
		Self._Link.Remove()
		
		'Insert into new location
		Select Position
			Case AS_CHILD
				Self.Parent = Dest
				Self._Link = Self.Parent.ChildList.AddLast(Self)
				
			Case AS_SIBLING
				If Dest.Parent = Null Then Throw "xmlNode.MoveTo(xmlNode, AS_SIBLING): The root node may not have siblings."
				Self.Parent = Dest.Parent
				Self._Link = Self.Parent.ChildList.InsertAfterLink(Self, Dest._Link)
		End Select
		
		'Update level variables
		_UpdateVars()
	End Method
	
	Rem
	bbdoc: Copies the current node (and all it's children) to the specified location (as a child, or as a sibling), returning
	the new copy.
	about: By default, this will copy the node into the specified destination "Dest", as a child. Optionally, you can
	use the "Position" variable to instruct MaXML to make it a sibling (AS_SIBLING) or a child (AS_CHILD). The copy of the
	node is returned.
	Example: node.CopyTo(node2, AS_CHILD)
	endrem
	Method CopyTo:xmlNode(Dest:xmlNode, Position:Int = AS_CHILD)
	    'Check for misuse
	    Local temp:xmlNode
	    If Self.Parent = Null Then
	    	If Position = AS_CHILD Then Throw "xmlNode.CopyTo(xmlNode, AS_CHILD): Cannot copy root node." 
	   		If Position = AS_SIBLING Then Throw "xmlNode.CopyTo(xmlNode, AS_SIBLING): Cannot copy root node."
	    End If
	    If Dest = Self Then 
	    	If Position = AS_CHILD Then Throw "xmlNode.CopyTo(CopyTo, AS_CHILD): Cannot move a copy into itself."
	    	If Position = AS_SIBLING Then Throw "xmlNode.CopyTo(CopyTo, AS_SIBLING): Cannot move a copy under itself."
	    End If
	    
	    'Make a copy of "Self"
	    Local this:xmlNode
	    this = Self.Copy()
		
	    'Remove from current location
		this._Link.Remove()
		
		'Insert into new location
		Select Position
			Case AS_CHILD
				this.Parent = Dest		
				this._Link = this.Parent.ChildList.AddLast(this)
				
			Case AS_SIBLING
				If Dest.Parent = Null Then Throw "xmlNode.CopyTo(xmlNode, AS_SIBLING): The root node may not have siblings."
				this.Parent = Dest.Parent
				this._Link = this.Parent.ChildList.InsertAfterLink(this, Dest._Link)
		End Select
		
		'Update level variables
		this._UpdateVars()
		
		Return this
	End Method
	
	Rem
	bbdoc: Returns an identicle copy of the current node (and all it's children).
	about: The new copy will be made as a sibling of the current node (as a child of the same parent). WARNING: Ignore the _Parent
	variable, since it should be used ONLY by MaXML when performing internal operations.
	Example: node.Copy().name = "Copy of "+node.Name	'Makes a copy of 'node', naming it "Copy of [node name]"
	endrem
	Method Copy:xmlNode(_Parent:xmlNode = Null)
		If Self.Parent = Null Then Throw "xmlNode.Copy(): Cannot copy root node."
	
		Local newnode:xmlNode, tempnode:xmlNode
		
		newnode = New xmlNode
		newnode.ChildList = New TList
		newnode.AttributeList = New TList
		
		'Copy data
		newnode.Name = Self.Name
		newnode.Value = Self.Value
		If _Parent = Null Then newnode.Parent = Self.Parent Else newnode.Parent = _Parent
		newnode.Level = Self.Level
		newnode._Link = newnode.Parent.ChildList.AddLast(newnode)
		
		'Copy attributes
		Local attr:xmlAttribute, newattr:xmlAttribute
		For attr = EachIn Self.AttributeList
			newattr = New xmlAttribute
			newattr.Name = attr.Name
			newattr.Value = attr.Value
			newattr.Node = attr.Node
			newattr._Link = newnode.AttributeList.AddLast(attr)	
		Next
		
		'Copy children
		For tempnode = EachIn Self.ChildList
			tempnode.Copy(newnode)
		Next
		
		Return newnode
	End Method
		
	Rem
	bbdoc: Swaps the current node with the specified node.
	about: Note: It is impossible to swap a node with one of it's children, or with one of it's parents.
	Example: node1.SwapWith node2 
	endrem
	Method SwapWith(Node:xmlNode) 
	    'Check for misuse
	    Local temp:xmlNode
	    'If Node.Parent = Null Or Self.Parent = Null Then Throw "xmlNode.SwapWith(xmlNode): Cannot swap root node."
	    If Node = Self Then 
	    	Return
	    	'If Position = AS_CHILD Then Throw "xmlNode.MoveTo(xmlNode, AS_CHILD): Cannot move a node into itself."
	    	'If Position = AS_SIBLING Then Throw "xmlNode.MoveTo(xmlNode, AS_SIBLING): Cannot move a node under itself."
	    End If
	    temp = Node
	    Repeat
	    	temp = temp.Parent
	    	If temp = Self Then Throw "xmlNode.SwapWith(xmlNode): Cannot swap a node with one of it's children."
	    Until temp = Null	
	    temp = Self
	    Repeat
	    	temp = temp.Parent
	    	If temp = Node Then Throw "xmlNode.SwapWith(xmlNode): Cannot swap a node with one of it's parents."
	    Until temp = Null

		Local BeforeNode:TLink = Node._Link.PrevLink()
		Local BeforeSelf:TLink = Self._Link.PrevLink()

	    'If both nodes are direct siblings, use another method
	    If Node.Parent = Self.Parent And Node.Parent <> Null Then
	    	If BeforeSelf = Node._Link Then
	    		Node._Link.Remove()
	    		Node._Link = Node.Parent.ChildList.InsertAfterLink(Node, Self._Link)
	    		Return
	    	End If
	    	If BeforeNode = Self._Link Then
	    		Self._Link.Remove()
	    		Self._Link = Self.Parent.ChildList.InsertAfterLink(Self, Node._Link)
	    		Return
	    	End If
	    End If
		
		'Remove from lists
		If Node.Parent <> Null Then Node._Link.Remove
		If Self.Parent <> Null Then Self._Link.Remove  
		
	    If Node.Parent <> Self.Parent Then
			Local tempLevel = Node.Level
			Local tempParent:xmlNode = Node.Parent
			'Swap Level and Parent fields
			Node.Level = Self.Level
			Node.Parent = Self.Parent
			Self.Level = tempLevel
			Self.Parent = tempParent
		End If
		
		'Re-insert into lists
		If Node.Parent <> Null Then
			If BeforeSelf = Null Then
				Node._Link = Node.Parent.ChildList.AddFirst(Node)
			Else
				Node._Link = Node.Parent.ChildList.InsertAfterLink(Node, BeforeSelf)
			End If
		End If
		If Self.Parent <> Null Then
			If BeforeNode = Null Then
				Self._Link = Self.Parent.ChildList.AddFirst(Self)
			Else
				Self._Link = Self.Parent.ChildList.InsertAfterLink(Self, BeforeNode)  
			End If
		End If
		
		Self._UpdateVars()
		Node._UpdateVars()
	End Method
	
	Rem
	bbdoc: Returns the node's index.
	about: A node's index represents the order it is listed. For example, if a node has an index of 1, it is the first child it's
	parent has. Likewise, if it's index is 7, it is the seventh child it's parent has.
	Example: node.SetIndex(node.GetIndex() + 1)	'Move the node one step down in the list
	endrem
	Method GetIndex:Int()
		Local Indx
		Local temp:TLink = _Link
		
		Repeat
			temp = temp.PrevLink()
			Indx = Indx + 1
		Until temp = Null
		
		Return Indx
	End Method
	
	Rem
	bbdoc: Sets the node's index.
	about: This method is very useful for arranging the order of nodes. Setting a node's index to 1 makes it the first child in it's
	parent has, setting it to 2 makes it the second child, and so on.
	Example: 
	endrem
	Method SetIndex(Index:Int)
		If Parent = Null Then Throw "xmlNode.SetIndex("+Index+"): Cannot set the index of the root node."
		If Index < 1 Then Throw "xmlNode.SetIndex("+Index+"): Index may not be less than 1."    
	
		Local i
		Local temp:TLink = Parent.ChildList.FirstLink()
		
		For i = 1 To Index-1
			temp = temp.NextLink()
			Assert (temp <> Null), "xmlNode.SetIndex("+Index+"): Index out of range."
		Next
		
		_Link.Remove()
		_Link = Parent.ChildList.InsertBeforeLink(Self, temp)
	End Method
	
	Rem
	bbdoc: Returns True if this node is the root node of an xmlDocument, False if not.
	about: Before attemping to access a node's parent, it is often useful to check whether or not it is the root node (since root
	nodes have no parent, and therefore a error would occur when attempting to access it)
	Example: If node.IsRoot() = False Then node.Parent.Name = "test"
	endrem
	Method IsRoot:Int()
		If Parent = Null Then Return True Else Return False
	End Method
	
	Rem
	bbdoc: Deletes the node from the xmlDocument.
	about: This function will allow the node to be deleted from memory, as soon as any references to it in your program
	are discarded. Warning: Do not try to manipulate or access a node after calling this function.
	Example: node.Free(); node = Null
	endrem
	Method Free()
		'Delete all child nodes	
		While ChildList.IsEmpty() = False
			xmlNode(ChildList.First()).Free()	
		Wend
		'Delete all attributes nodes	
		While AttributeList.IsEmpty() = False
			xmlAttribute(AttributeList.First()).Free()	
		Wend
		'Delete self
		If Self.Parent <> Null Then _Link.Remove()
		Parent = Null
		_Link = Null
	End Method

	          

	'----------- Child Node Control ----------------------------------------
	'Child node control methods allow you to easily access any child node you want, as well as perform
	'operations involving all of the node's children
	
	Rem
	bbdoc: Returns True is the node has children, False if not.
	about: The xmlNode.HasChildren() method simply returns whether or not the node has child nodes. Note: Although
	xmlNode.ChildCount() can be used for the same purpose, this is slightly faster.
	Example: If node.HasChildren() Then subnode = node.FirstChild()
	endrem
	Method HasChildren:Int()
		If ChildList.IsEmpty() Then Return False Else Return True 
	End Method

	Rem
	bbdoc: Returns the first child the node has, if any.
	about: This method simply returns the node's first child. If the node has no children, Null will be returned.
	Example: subnode = node.FirstChild()
	endrem
	Method FirstChild:xmlNode()
		If ChildList.IsEmpty() Then Return Null Else Return xmlNode(ChildList.First())
	End Method
	
	Rem
	bbdoc: Returns the last child the node has, if any.
	about: This method simply returns the node's last child. If the node has no children, Null will be returned.
	Example: subnode = node.LastChild()
	endrem
	Method LastChild:xmlNode()
		If ChildList.IsEmpty() Then Return Null Else Return xmlNode(ChildList.Last())
	End Method
	
	Rem
	bbdoc: Returns one of the node's children, specified by Index.
	about: If Index is 1, this will return the node's first child. If Index is 2, this will return the second child, etc. Note:
	Using xmlNode.FirstChild(), xmlnode.LastChild(), etc. is generally faster than using this to perform the same action.
	Example: subnode = node.GetChild(3)
	endrem
	Method GetChild:xmlNode(Index:Int)
		If Index < 1 Then Throw "xmlNode.SetIndex("+Index+"): Index may not be less than 1."    
		
		Local i
		Local temp:TLink = ChildList.FirstLink()

		For i = 1 To Index-1
			temp = temp.NextLink()
			Assert (temp <> Null), "xmlNode.GetChild("+Index+"): Index out of range."
		Next
		
		Return xmlNode(temp.Value())
	End Method
	           
	Rem
	bbdoc: Returns the number of children the node has.
	about: While this can be used to check if a node has children or not (such as "If node.ChildCount() = 0 Then ..."), it is
	slightly slower (and less readible) than simply using the HasChildren() method.
	Example: For i = 1 To node.ChildCount()
	endrem
	Method ChildCount:Int()
		Return ChildList.Count() 
	End Method
	
	Rem
	bbdoc: Searches the node's children for a node with the specified name.
	about: This will return Null if no match was found. Optionally, you can set Recurse To False, which will only allow the search
	to look through direct children of the node (indirect children would count the children's children, and their children, etc.)
	Also, you can set MatchCase to False to disable case-sensativity when searching (a search for "settings", for example, would
	return nodes named "Settings", "seTTingS", "SETTINGS", etc., not just "settings").
	Example: doc.Root().FindChild("settings", True, False)
	endrem
	Method FindChild:xmlNode(Name:String, Recurse = True, MatchCase = True)
		Local Node:xmlNode, temp:xmlNode
		
		If ChildList.IsEmpty() Then Return Null
		
		If MatchCase Then
			For Node = EachIn ChildList
				If node.Name = Name Then Return Node
			Next
		Else
			Name = Name.ToLower()
			For Node = EachIn ChildList
				If node.Name.ToLower() = Name Then Return Node
			Next
		End If
		
		If Recurse Then
			For Node = EachIn ChildList
				If node.ChildList.IsEmpty() = False Then
					temp = node.FindChild(Name, Recurse, MatchCase)
					If temp <> Null Then Return temp
				End If
			Next
		End If
		
		Return Null
	End Method
	
	Rem
	bbdoc: Searches the node's children for a node with the specified name and attribute value.
	about: Node.FindChildEx() is basically the same as Node.FindChild(), except two additional parameters are allowed: AttrName
	and AttrValue. Using these, you can find a specific node with an attribute of the specified name and value. See FindChild() for more
	information.
	Example: doc.Root().FindChildEx("entity", "type", "tree", True, False) 'This would find a node like this: (entity type="tree" size="10")
	EndRem
	Method FindChildEx:xmlNode(Name:String, AttrName$, AttrValue$, Recurse = True, MatchCase = True)
		Local Node:xmlNode, Attr:xmlAttribute, temp:xmlNode
		
		If ChildList.IsEmpty() Then Return Null
		
		If MatchCase Then
			For Node = EachIn ChildList
				If Node.Name = Name Then
					Attr = node.Attribute(AttrName, True)
					If Attr <> Null Then If Attr.Value = AttrValue Then Return Node
				End If
			Next
		Else
			Name = Name.ToLower()
			For Node = EachIn ChildList
				If Node.Name.ToLower() = Name Then
					Attr = node.Attribute(AttrName, False)
					If Attr <> Null Then If Attr.Value.ToLower() = AttrValue.ToLower() Then Return Node
				End If
			Next
		End If
		
		If Recurse Then
			For Node = EachIn ChildList
				If node.ChildList.IsEmpty() = False Then
					temp = node.FindChildEx(Name, AttrName, AttrValue, Recurse, MatchCase)
					If temp <> Null Then Return temp
				End If
			Next
		End If
		
		Return Null
	End Method

	Rem
	bbdoc: Alphabetically sorts all of the node's children, by node name, node value, attribute name, or attribute value.
	about: Set Mode to SORTBY_NODE_NAME, SORTBY_NODE_VALUE, SORTBY_ATTR_NAME, or SORTBY_ATTR_VAL depending on how you want the nodes to be sorted.
	Setting Recurse to True will allow indirect children to be sorted also. Ascending can be set to False if you want
	the nodes to be sorted in reverse-alphabetic order.
	Example: doc.Root().SortChildren(SORTBY_NODE_NAME, True, True)	'Sort entire XML file by node name
	EndRem
	Method SortChildren(Mode = SORTBY_NODE_NAME, Recurse = False, Ascending = True)
		Local term:TLink = ChildList._head
		Repeat
			Local link:TLink=ChildList._head._succ
			Local sorted=True
			Repeat
				Local succ:TLink=link._succ
				If succ=term Exit
				Local cc
				Local a$, b$
				Local attr:xmlAttribute, node:xmlNode
				Select Mode
					Case SORTBY_NODE_NAME
						a = (xmlNode(link._value).Name).ToLower()
						b = (xmlNode(succ._value).Name).ToLower()
					Case SORTBY_NODE_VALUE
						a = (xmlNode(link._value).Value).ToLower()
						b = (xmlNode(succ._value).Value).ToLower()
					Case SORTBY_ATTR_NAME
						a = ""
						node = xmlNode(link._value)
						For attr = EachIn node.AttributeList
							a = a + attr.Name
						Next
						a = a.ToLower()
						b = ""
						node = xmlNode(succ._value)
						For attr = EachIn node.AttributeList
							b = b + attr.Name
						Next
						b = b.ToLower()
					Case SORTBY_ATTR_VALUE
						a = ""
						node = xmlNode(link._value)
						For attr = EachIn node.AttributeList
							a = a + attr.Value
						Next
						a = a.ToLower()
						b = ""
						node = xmlNode(succ._value)
						For attr = EachIn node.AttributeList
							b = b + attr.Value
						Next
						b = b.ToLower()
				End Select
				If (a>b And Ascending) Or (a<b And Not Ascending)
					Local link_pred:TLink=link._pred
					Local succ_succ:TLink=succ._succ
					link_pred._succ=succ
					succ._succ=link
					succ._pred=link_pred
					link._succ=succ_succ
					link._pred=succ
					succ_succ._pred=link
					sorted=False
				Else
					link=succ
				EndIf
			Forever
			If sorted Return
			term=link
		Forever
        
		Local Node:xmlNode
		If Recurse Then
			For Node = EachIn ChildList
				node.SortChildren(Mode, Recurse, Ascending)
			Next
		End If
	End Method

	Rem
	bbdoc: Alphabetically sorts all of the node's children, by node name, node value, attribute name, and/or attribute value.
	about: SortChildrenEx() is basically the same as SortChildren(), except a secondary sort mode is accepted. The secondary sort
	mode is used when the position of two nodes is concidered equal by the primary sort mode (for example, when SORTBY_NODE_NAME
	comes across multiple nodes of the same name). In this case, the secondary sort method is used.
	Example: doc.Root().SortChildrenEx(SORTBY_NODE_NAME, SORTBY_ATTR_VALUE, True, True)	'Sort entire XML file by node name and value.
	Note: In this example, it would order (entity type="tree") below of (entity type="car"), since the secondary method specifies
	the attribute value to be used in this case (since SORTBY_NODE_NAME found multiple nodes of the same name).
	EndRem
	Method SortChildrenEx(PrimaryMode = SORTBY_NODE_NAME, SecondaryMode = SORTBY_NODE_VALUE, Recurse = False, Ascending = True)
		Local term:TLink = ChildList._head
		Repeat
			Local link:TLink=ChildList._head._succ
			Local sorted=True
			Repeat
				Local succ:TLink=link._succ
				If succ=term Exit
				Local cc
				Local a$, b$
				Local attr:xmlAttribute, node:xmlNode
				
				a = ""
				b = ""
				
				Select PrimaryMode
					Case SORTBY_NODE_NAME
						a = a + (xmlNode(link._value).Name).ToLower()
						b = b + (xmlNode(succ._value).Name).ToLower()
					Case SORTBY_NODE_VALUE
						a = a + (xmlNode(link._value).Value).ToLower()
						b = b + (xmlNode(succ._value).Value).ToLower()
					Case SORTBY_ATTR_NAME
						node = xmlNode(link._value)
						For attr = EachIn node.AttributeList
							a = a + attr.Name
						Next
						a = a.ToLower()
						node = xmlNode(succ._value)
						For attr = EachIn node.AttributeList
							b = b + attr.Name
						Next
						b = b.ToLower()
					Case SORTBY_ATTR_VALUE
						node = xmlNode(link._value)
						For attr = EachIn node.AttributeList
							a = a + attr.Value
						Next
						a = a.ToLower()
						node = xmlNode(succ._value)
						For attr = EachIn node.AttributeList
							b = b + attr.Value
						Next
						b = b.ToLower()
				End Select
				
				Select SecondaryMode
					Case SORTBY_NODE_NAME
						a = a + (xmlNode(link._value).Name).ToLower()
						b = b + (xmlNode(succ._value).Name).ToLower()
					Case SORTBY_NODE_VALUE
						a = a + (xmlNode(link._value).Value).ToLower()
						b = b + (xmlNode(succ._value).Value).ToLower()
					Case SORTBY_ATTR_NAME
						node = xmlNode(link._value)
						For attr = EachIn node.AttributeList
							a = a + attr.Name
						Next
						a = a.ToLower()
						node = xmlNode(succ._value)
						For attr = EachIn node.AttributeList
							b = b + attr.Name
						Next
						b = b.ToLower()
					Case SORTBY_ATTR_VALUE
						node = xmlNode(link._value)
						For attr = EachIn node.AttributeList
							a = a + attr.Value
						Next
						a = a.ToLower()
						node = xmlNode(succ._value)
						For attr = EachIn node.AttributeList
							b = b + attr.Value
						Next
						b = b.ToLower()
				End Select
				
				If (a>b And Ascending) Or (a<b And Not Ascending)
					Local link_pred:TLink=link._pred
					Local succ_succ:TLink=succ._succ
					link_pred._succ=succ
					succ._succ=link
					succ._pred=link_pred
					link._succ=succ_succ
					link._pred=succ
					succ_succ._pred=link
					sorted=False
				Else
					link=succ
				EndIf
			Forever
			If sorted Return
			term=link
		Forever
        
		Local Node:xmlNode
		If Recurse Then
			For Node = EachIn ChildList
				node.SortChildrenEx(PrimaryMode, SecondaryMode, Recurse, Ascending)
			Next
		End If
	End Method
		
	Rem
	bbdoc: Moves all of the node's children to another location,
	about: This method basically performs a xmlNode.MoveTo() operation on each child the current node has. Settings Position
	to AS_CHILD will move all it's children to the destination node, as children. Setting Position to AS_SIBLING will move all
	it's children to the destination node, as siblings.
	Example: node.MoveChildrenTo(doc.Root(), AS_CHILD)
	endrem
	Method MoveChildrenTo(Dest:xmlNode, Position:Int = AS_CHILD)
		Select Position
			Case AS_CHILD
				While ChildList.IsEmpty() = False
					xmlNode(ChildList.First()).MoveTo(Dest, AS_CHILD)
				Wend 
			Case AS_SIBLING
				While ChildList.IsEmpty() = False
					xmlNode(ChildList.Last()).MoveTo(Dest, AS_SIBLING)
				Wend 
		End Select
	End Method  
		
	Rem
	bbdoc: Copies all of the node's children to a specified location,
	about: This method basically performs a xmlNode.CopyTo() operation on each child the current node has. Settings Position
	to AS_CHILD will copy all it's children to the destination node, as children. Setting Position to AS_SIBLING will copy all
	it's children to the destination node, as siblings.
	Example: node.CopyChildrenTo(doc.Root().FirstChild(), AS_SIBLING)
	endrem
	Method CopyChildrenTo(Node:xmlNode, Position:Int = AS_CHILD)
		Select Position
			Case AS_CHILD
				While ChildList.IsEmpty() = False
					xmlNode(ChildList.First()).CopyTo(Node, AS_CHILD)
				Wend 
			Case AS_SIBLING
				While ChildList.IsEmpty() = False
					xmlNode(ChildList.Last()).CopyTo(Node, AS_SIBLING)
				Wend 
		End Select
	End Method  
	
	Rem
	bbdoc: Deletes all of the node's children from the xmlDocument.
	about: This function will allow all the node's children to be deleted from memory, as soon as any references
	to it in your program are discarded. See xmlNode.Free()
	Example: node.Free(); node = Null
	Example: 
	endrem
	Method FreeChildren()	
		While ChildList.IsEmpty() = False
			xmlNode(ChildList.First()).Free()	
		Wend
	End Method

	           

	'----------- Sibling Node Control ----------------------------------------
	'Sibling node control methods get/set the node's relationship to it's siblings. The sibling
	'methods are very similar to the child methods. They differ in that sibling methods affect
	'nodes on the current level. For example, "node.SiblingCount()" is exactly the same as
	'"node.Parent.ChildCount()"

	Rem
	bbdoc: Returns the node's next sibling.
	about: If there are no more siblings, Null will be returned. ("Siblings" is a term used to describe nodes
	which share the same parent node.)
	Example: node = node.NextSibling()
	endrem
	Method NextSibling:xmlNode()
		If Parent = Null Then Return Null 'If this is root, it has no siblings

		Local lnk:TLink		
		lnk = _Link.NextLink()
		If lnk = Null Then Return Null Else Return xmlNode(lnk.Value())
	End Method
	
	Rem
	bbdoc: Returns the node's next sibling.
	about: If there are no previous siblings, Null will be returned. ("Siblings" is a term used to describe nodes
	which share the same parent node.)
	Example: node = node.PrevSibling()
	endrem
	Method PrevSibling:xmlNode()
		If Parent = Null Then Return Null 'If this is root, it has no siblings

		Local lnk:TLink		
		lnk = _Link.PrevLink()
		If lnk = Null Then Return Null Else Return xmlNode(lnk.Value())
	End Method
	
	Rem
	bbdoc: Returns the node's first sibling.
	about: Technically, this returns the first child that it's parent has. So, if your node is already the first child
	it's parent has, this will return your node (not one of it's siblings).
	Example: node.FirstSibling().Value = "I'm the first child under my parent node!"
	endrem
	Method FirstSibling:xmlNode()
		If Parent = Null Then Return Self
		Return xmlNode(Parent.ChildList.First())
	End Method
	
	Rem
	bbdoc: Returns the node's last sibling.
	about: Technically, this returns the last child that it's parent has. So, if your node is already the last child
	it's parent has, this will return your node (not one of it's siblings).
	Example: node.LastSibling().Value = "I'm the last child under my parent node!"
	endrem
	Method LastSibling:xmlNode()
		If Parent = Null Then Return Self
		Return xmlNode(Parent.ChildList.Last())
	End Method
	
	Rem
	bbdoc: Returns one of the node's siblings, specified by Index.
	about: If Index is 1, this will return the first sibling. If Index is 2, this will return the second sibling, etc. Note:
	Using xmlNode.FirstSibling(), xmlnode.LastSibling(), etc. is generally faster than using this to perform the same action.
	Example: subnode = node.GetSibling(3)
	endrem
	Method GetSibling:xmlNode(Index:Int)
		If Parent = Null Then
			If Index = 1 Then Return Self
			Throw "xmlNode.GetSibling("+Index+"): The root node has no siblings"
		End If
		
		Local i
		Local temp:TLink = Parent.ChildList.FirstLink()
		
		For i = 1 To Index-1
			temp = temp.NextLink()
			Assert (temp <> Null), "xmlNode.GetSibling("+Index+"): Index out of range."
		Next
		
		Return xmlNode(temp.Value())
	End Method
	           
	Rem
	bbdoc: Returns the number of siblings this node has, including itself.
	about: Technically, this returns the number of children the node's parent has. This returns the number of nodes
	which are under the same parent as the current node. This is useful along with xmlNode.GetSibling() for loop through
	a node's siblings (although using xmlNode.FirstSibling(), xmlNode.NextSibling(), etc. is slightly faster)
	Example: For i = 1 To node.SiblingCount()
	endrem
	Method SiblingCount:Int()
		If Parent = Null Then Return 1
		Return Parent.ChildList.Count() 
	End Method
	
	Rem
	bbdoc: Searches the node's siblings for a node with the specified name.
	about: This will return Null if no match was found. Optionally, you can set MatchCase to False to disable
	case-sensativity when searching (a search for "settings", for example, could
	return nodes named "Settings", "seTTingS", "SETTINGS", etc., not just "settings").
	Example: settingsNode = gameplayNode.FindSibling("settings", False)
	endrem
	Method FindSibling:xmlNode(Name:String, MatchCase = True)
		If Parent = Null Then
			If MatchCase = True Then Throw "xmlNode.xmlNode(~q"+Name+"~q): The root node has no siblings."
			If MatchCase = False Then Throw "xmlNode.xmlNode(~q"+Name+"~q, False): The root node has no siblings."
		End If
		
		Return Parent.FindChild(Name, False, MatchCase)
	End Method

		

	'----------- Attribute Control ----------------------------------------
	'Attribute node control methods allows you to access the node's attributes easily

	Rem
	bbdoc: Returns True if the node has the specified attribute, false if not.
	about: Optionally, MatchCase can be set to False if you want it to consider nodes with different capitalization.
	Example: If Not node.HasAttribute("alpha") Then Throw "alpha attribute not specified!"
	endrem
	Method HasAttribute:Int(Name:String, MatchCase = True)
		Local attr:xmlAttribute
		
		If MatchCase Then
			For attr = EachIn AttributeList
				If attr.Name = Name Then Return True
			Next
		Else
			Name = Name.ToLower()
			For attr = EachIn AttributeList
				If attr.Name.ToLower() = Name Then Return True
			Next
		End If
	End Method
	
	Rem
	bbdoc: Returns True is the node has any attributes, False if not.
	about: The xmlNode.HasAttributes() method simply returns whether or not the node has attributes.
	Example: If node.HasAttributes() Then (...)
	endrem
	Method HasAttributes:Int()
		If AttributeList.IsEmpty() Then Return False Else Return True 
	End Method

	Rem
	bbdoc: Returns the first attribute the node has, if any.
	about: This method simply returns the node's first attribute. If the node has no attributes, Null will be returned.
	Example: subnode = node.FirstChild()
	endrem
	Method FirstAttribute:xmlNode()
		If AttributeList.IsEmpty() Then Return Null Else Return xmlNode(AttributeList.First())
	End Method
	
	Rem
	bbdoc: Returns the last attribute the node has, if any.
	about: This method simply returns the node's last attribute. If the node has no attributes, Null will be returned.
	Example: subnode = node.LastChild()
	endrem
	Method LastAttribute:xmlNode()
		If AttributeList.IsEmpty() Then Return Null Else Return xmlNode(AttributeList.Last())
	End Method
	
	Rem
	bbdoc: Returns an attribute of the specified name.
	about: This will return a xmlAttribute object with the specified name. If the attribute exists, this gives you access to
	it's data through the xmlAttribute Type. If no attribute with that name exists, a one will be created for your
	convenience. MatchCase can be set to False if you want this to ignore the capitalization when searching for the attribute.
	Example: SetAlpha node.Attribute("alpha").Value; node.Attribute("red").Value = 255
	endrem
	Method Attribute:xmlAttribute(Name:String, MatchCase = True)
		Local attr:xmlAttribute
		
		'Find the attribute
		If MatchCase Then
			For attr = EachIn AttributeList
				If attr.Name = Name Then Return attr
			Next
		Else
			Name = Name.ToLower()
			For attr = EachIn AttributeList
				If attr.Name.ToLower() = Name Then Return attr
			Next
		End If
		
		'If it does not exist, create one
		attr = New xmlAttribute
		attr.Name = Name
		attr.Value = ""
		attr.Node = Self
		attr._Link = AttributeList.AddLast(attr)
		Return attr
	End Method
	
	Rem
	bbdoc: Remove node attributes with no name or no value.
	about: This simply removes any attributes from the node which contain blank name or value data.
	Example: node.CleanAttributes()
	endrem
	Method CleanAttributes()
		Local attr:xmlAttribute
		
		For attr = EachIn AttributeList
			If attr.Name = "" Or attr.Value = "" Then attr.Free()
		Next
	End Method
	
	
	
	'*** INTERNAL METHODS- DO NOT USE ***
	Method _UpdateVars()
		If Parent <> Null Then Level = Parent.Level + 1
		
		Local Node:xmlNode
		For Node = EachIn ChildList
			node._UpdateVars()
		Next
	End Method
End Type

Rem
bbdoc: An xmlAttribute can be used to access and modify a node's attributes.
about: To add/access the attributes of an xmlNode, refer to the xmlNode documentation. xmlAttributes are created/accesed
with xmlNode.Attribute(). Once you have an attribute, you can use it's Fields/Methods to modify or read it's values.
endrem
Type xmlAttribute
	Rem
	bbdoc: The name of the xmlAttribute
	about: Use the Name field to modify or read the name of an attribute.
	Example: node.Attribute("R").Name = "ColorR"	'Rename attribute "R" to "ColorR"
	endrem
	Field Name:String
	
	Rem
	bbdoc: The value of the xmlAttribute
	about: Use the Value field to modify or read the name of an attribute.
	Example: node.Attribute("ColorR").Value = 255
	endrem
	Field Value:String	
	
	Rem
	bbdoc: The node the xmlAttribute belongs to
	about: While this will rarely be needed, you can always use this attribute to find the node which this attribute
	belongs to.
	Example: attr.Value = attr.Node.Name
	endrem
	Field Node:xmlNode
	
	Field _Link:TLink
	    
	Rem
	bbdoc: Returns the next attribute that the node has.
	about: If no more attributes exists after this one, Null is returned.
	Example: attr.NextAttr().Value = "123"
	endrem
	Method NextAttr:xmlAttribute()		
		Local lnk:TLink		
		lnk = _Link.NextLink()
		If lnk = Null Then Return Null Else Return xmlAttribute(lnk.Value())
	End Method
	
	Rem
	bbdoc: Returns the previous attribute that the node has.
	about: If no more attributes exists before this one, Null is returned.
	Example: attr.PrevAttr().Value = "123"
	endrem
	Method PrevAttr:xmlAttribute()
		Local lnk:TLink		
		lnk = _Link.PrevLink()
		If lnk = Null Then Return Null Else Return xmlAttribute(lnk.Value())
	End Method
		
	Rem
	bbdoc: Deletes the attribute from the node.
	about: The attribute will be deleted from memory after you discard all references to it (if any exist).
	Example: node.Attribute("test").Free()	'Delete the "test" attribute
	endrem
	Method Free()
		_Link.Remove()
	End Method
End Type
        



'*** INTERNAL FUNCTIONS - DO NOT USE ***

Private

Function _WriteNode(File:TStream, Node:xmlNode)
	Local NodeContents:String, Indent:String, Indent2:String
	Local TempAttr:xmlAttribute, TempNode:xmlNode
	    
	NodeContents = Node.Name
	For TempAttr = EachIn Node.AttributeList
		NodeContents = NodeContents + " " + TempAttr.Name + "=~q" + _AddEscapeCodes(TempAttr.Value) + "~q"
	Next
	
	Indent = Indent[ .. Node.Level*2]
	Indent2 = Indent2[ .. (Node.Level+1)*2]
	
	If Node.HasChildren() = False Then
		If Node.Value = "" Then
			File.WriteLine Indent + "<" + NodeContents + "/>" 
		Else
		    File.WriteLine Indent + "<" + NodeContents + ">" + _AddEscapeCodes(Node.Value) + "</" + Node.Name + ">"
		End If
	Else
		File.WriteLine Indent + "<" + NodeContents + ">"
		If Node.Value <> "" Then File.WriteLine Indent2 + _AddEscapeCodes(Node.Value)
		
		For TempNode = EachIn Node.ChildList
			_WriteNode(File, TempNode)
		Next
		
		File.WriteLine Indent + "</" + Node.Name + ">"
	End If
End Function

Function _WriteNode_Binary(File:TStream, Node:xmlNode)
	Local TempAttr:xmlAttribute, TempNode:xmlNode

	'Name and Value
	File.WriteInt Node.Name.Length; File.WriteString Node.Name
	File.WriteInt Node.Value.Length; File.WriteString Node.Value
	
	'Attributes
	File.WriteInt Node.AttributeList.Count()
	For TempAttr = EachIn Node.AttributeList
		File.WriteInt TempAttr.Name.Length; File.WriteString TempAttr.Name
		File.WriteInt TempAttr.Value.Length; File.WriteString TempAttr.Value
	Next
	
	'Children
	File.WriteInt Node.ChildList.Count()
	For TempNode = EachIn Node.ChildList
		_WriteNode_Binary(File, TempNode)
	Next
End Function



Global BinaryFile:TStream

Function _InitLoader_Binary(Url:Object)
	BinaryFile = OpenStream(Url, True, False) 
	
	If BinaryFile = Null Then Throw "xmlDocument.Load(~q"+String(Url)+"~q): Cannot read from file."
	If BinaryFile.ReadByte() = 216 Then
		If BinaryFile.ReadByte() = 241 Then
			If BinaryFile.ReadByte() = 188 Then
				Return
			End If
		End If
	End If
	Throw "xmlDocument.Load(~q"+String(Url)+"~q): Tried to load a non-binary file as a binary file."
End Function

Function _Load_Binary(doc:xmlDocument, parent:xmlNode)
	Local Node:xmlNode, i:Int, Length:Int
	
	'Load node
	Local Name:String, Value:String

	Length = BinaryFile.ReadInt(); Name = BinaryFile.ReadString(Length)
	Length = BinaryFile.ReadInt(); Value = BinaryFile.ReadString(Length)
	
	If parent <> Null Then
		Node = parent.AddNode(Name, AS_CHILD)
		Node.Value = Value
	Else
		Node = doc.Root()
		Node.Name = Name
		Node.Value = Value
	End If
	
	'Attributes
	Local AttributeCount, Attr:xmlAttribute
	
	AttributeCount = BinaryFile.ReadInt()
	For i = 1 To AttributeCount
		Attr = New xmlAttribute
		Attr.Node = Node
		Attr._Link = Node.AttributeList.AddLast(attr)           
		Length = BinaryFile.ReadInt(); Attr.Name = BinaryFile.ReadString(Length)  
		Length = BinaryFile.ReadInt(); Attr.Value = BinaryFile.ReadString(Length) 
	Next
	
	'Children
	Local ChildCount, SubNode:xmlNode
	
	ChildCount = BinaryFile.ReadInt()
	For i = 1 To ChildCount
		_Load_Binary(doc, Node)
	Next
	
End Function	               
	
	        
Global TokenValue:String

Global TokenBuffer:TBank
Global TokenBufferPos:Int
Global TokenBufferLen:Int

Const TOKEN_EOF = -1
Const TOKEN_NONE = 0
Const TOKEN_BEGIN_TAG = 1       '<
Const TOKEN_END_TAG = 2         '>
Const TOKEN_BEGIN_SLASHTAG = 3	'</
Const TOKEN_END_SLASHTAG = 4	'/>
Const TOKEN_EQUALS = 5			'=
Const TOKEN_TEXT = 6
Const TOKEN_QUOTEDTEXT = 7
Const TOKEN_SLASH = 8			'/


Function _InitTokenizer(Url:Object)
	TokenValue = ""
	
	TokenBuffer = TBank.Load(Url)
	If TokenBuffer = Null Then Throw "xmlDocument.Load(~q"+String(Url)+"~q): Cannot read from file."
	TokenBufferPos = -1
	TokenBufferLen = TokenBuffer.Size()
End Function

Function _Parse(doc:xmlDocument, parent:xmlNode)
	Local tok:Int, Node:xmlNode, attr:xmlAttribute
	Local TextSeperator:String
	
	Repeat
		tok = _NextToken()
		
		Select tok
			Case TOKEN_BEGIN_TAG
				'Load node
				If parent <> Null Then
					Node = parent.AddNode(TokenValue, AS_CHILD)
				Else
					Node = doc.Root()
					node.Name = TokenValue
				End If
				
				'Load attributes
				tok = _NextToken()
				While tok = TOKEN_TEXT	
					attr = New xmlAttribute
					attr.Node = Node
					attr._Link = node.AttributeList.AddLast(attr)
                    
					attr.Name = TokenValue  
					
					tok = _NextToken()
					If tok <> TOKEN_EQUALS Then Throw "XML Parse Error: Expecting equals (=) after attribute name."
				    
					tok = _NextToken()
					If tok <> TOKEN_QUOTEDTEXT Then Throw "XML Parse Error: Expecting attribute value."
					
					attr.Value = TokenValue'_RemoveEscapeCodes(TokenValue)
					
					tok = _NextToken()
				Wend
				
				'Load children (if any)
				If tok = TOKEN_END_TAG Then
					 _Parse(doc, Node)
				Else
					If tok <> TOKEN_END_SLASHTAG Then Throw "XML Parse Error: Unclosed tag (expecting ~q/>~q or ~q>~q)"
				End If
			
			Case TOKEN_BEGIN_SLASHTAG
				'Verify that the closing node matches the last opening node
				If parent = Null Then
					Throw "XML Parse Error: Misplaced closing tag (no tag has been opened)"
				Else
					If TokenValue <> parent.Name Then Throw "XML Parse Error: Closing tag does not match opening tag."
				End If
				
				'Make sure the tag is closed properly
				tok = _NextToken()
				If tok <> TOKEN_END_TAG Then Throw "XML Parse Error: Unclosed tag (expecting ~q>~q)"
				'Return, since reading this node is complete
				Return
				
			Case TOKEN_TEXT
				If parent = Null Then
					Throw "XML Parse Error: Misplaced text (possibly not an xml file)"
				Else
					If parent.Value = "" Then
						parent.Value = TokenValue
					Else
						parent.Value = parent.Value + TextSeperator + TokenValue
					End If
					TextSeperator = " "
				End If
				
			Case TOKEN_SLASH
				If TextSeperator = " " Then
					TextSeperator = "/"
				Else
					TextSeperator = TextSeperator + "/"
				End If
				
			Case TOKEN_EQUALS
				If TextSeperator = " " Then
					TextSeperator = "="
				Else
					TextSeperator = TextSeperator + "="
				End If
			
			Case TOKEN_END_TAG, TOKEN_END_SLASHTAG
				Throw "XML Parse Error: Misplaced tag end (expecting tag or tag data)." 
				
			Case TOKEN_EOF
				If parent = Null Then
					Return
				Else
					Throw "XML Parse Error: Unclosed node tag (reached end of file)"
				End If
		End Select
	Forever
End Function

Function _NextToken:Int()
	Local tok:Int
	Repeat
		tok = __NextToken()
	Until tok <> TOKEN_NONE
	Return tok
End Function

Function __NextToken:Int()
	Local ch:Int
	
	Local Value:String
	Local _TokenBufferLen = TokenBufferLen
	
	TokenBufferPos :+ 1
	If TokenBufferPos >= _TokenBufferLen Then Return TOKEN_EOF
	ch = TokenBuffer.PeekByte(TokenBufferPos)
	'ch = TokenBuffer._buf[TokenBufferPos]
	
	Select ch 
		Case 60 '<'
			'Tag beginning / closing node tag beginning / comment / header
			TokenBufferPos :+ 1
			ch = TokenBuffer.PeekByte(TokenBufferPos)
			'ch = TokenBuffer._buf[TokenBufferPos+1]
			Select ch
				Case 47 '/'
					'Closing node tag beginning "</"
					TokenBufferPos :+ 1
					ch = TokenBuffer.PeekByte(TokenBufferPos)
					'ch = TokenBuffer._buf[TokenBufferPos]
					Value = ""
					While ch <> 32 And ch <> 62 And ch <> 47
						Value = Value + Chr(ch)
						TokenBufferPos :+ 1
						If TokenBufferPos = TokenBufferLen Then Exit
						ch = TokenBuffer.PeekByte(TokenBufferPos)
						'ch = TokenBuffer._buf[TokenBufferPos]
					Wend
					If ch <> 32 Then TokenBufferPos :- 1
					TokenValue = Value
					
					Return TOKEN_BEGIN_SLASHTAG
					
				Case 45 '-'
					'Comment
					TokenBufferPos :+ 1
					ch = TokenBuffer.PeekByte(TokenBufferPos)
					While ch <> 62
						TokenBufferPos :+ 1
						If TokenBufferPos = TokenBufferLen Then Exit
						ch = TokenBuffer.PeekByte(TokenBufferPos)
					Wend
					Return TOKEN_NONE
					
				Case 63 '?'
					'Header
					Local temp:String
					temp = ""
					
					TokenBufferPos :+ 1
					ch = TokenBuffer.PeekByte(TokenBufferPos)
					While ch <> 62
						TokenBufferPos :+ 1
						If TokenBufferPos = TokenBufferLen Then Exit
						ch = TokenBuffer.PeekByte(TokenBufferPos)
						temp = temp + ch
					Wend					
					Return TOKEN_NONE
					
				Default
					'Tag beginning
					'ch = TokenBuffer._buf[TokenBufferPos]
					Value = ""
					While ch <> 32 And ch <> 62 And ch <> 47
						Value = Value + Chr(ch)
						TokenBufferPos :+ 1
						If TokenBufferPos = TokenBufferLen Then Exit
						ch = TokenBuffer.PeekByte(TokenBufferPos)
						'ch = TokenBuffer._buf[TokenBufferPos]
					Wend
					If ch <> 32 Then TokenBufferPos :- 1
					TokenValue = Value

					Return TOKEN_BEGIN_TAG
			End Select
			
		Case 47 '/'
			'Single-line tag end
			TokenBufferPos :+ 1
			ch = TokenBuffer.PeekByte(TokenBufferPos)
			'ch = TokenBuffer._buf[TokenBufferPos]
			If ch = 62 Then
				Return TOKEN_END_SLASHTAG
			End If
			TokenBufferPos :- 1
			Return TOKEN_SLASH
		
		Case 62 '>'
			'Tag end			
			Return TOKEN_END_TAG

		Case 61 '='
			'Equals symbol
			Return TOKEN_EQUALS
			   
		Case 34 '"'
			'Quoted text
			TokenBufferPos :+ 1
			ch = TokenBuffer.PeekByte(TokenBufferPos)
			'ch = TokenBuffer._buf[TokenBufferPos]
			Value = ""
			While ch <> 34
				'Process escape codes
				If ch = 38 Then '&'
					Select TokenBuffer.PeekByte(TokenBufferPos+1)
						Case 108 'l'
							If TokenBuffer.PeekByte(TokenBufferPos+2) = 116 Then
								If TokenBuffer.PeekByte(TokenBufferPos+3) = 59 Then
									ch = 60
									TokenBufferPos :+ 3
								End If
							End If
						Case 103 'g'
							If TokenBuffer.PeekByte(TokenBufferPos+2) = 116 Then
								If TokenBuffer.PeekByte(TokenBufferPos+3) = 59 Then
									ch = 62
									TokenBufferPos :+ 3
								End If
							End If
						Case 97 'a'
							If TokenBuffer.PeekByte(TokenBufferPos+2) = 112 Then
								If TokenBuffer.PeekByte(TokenBufferPos+3) = 111 Then
									If TokenBuffer.PeekByte(TokenBufferPos+4) = 115 Then
										If TokenBuffer.PeekByte(TokenBufferPos+5) = 59 Then
											ch = 39 '''
											TokenBufferPos :+ 5
										End If
									End If
								End If
							Else If TokenBuffer.PeekByte(TokenBufferPos+2) = 109 Then
								If TokenBuffer.PeekByte(TokenBufferPos+3) = 112 Then
									If TokenBuffer.PeekByte(TokenBufferPos+4) = 59 Then
										ch = 38 '&'
										TokenBufferPos :+ 4
									End If
								End If							
							End If
						Case 113 'q'
							If TokenBuffer.PeekByte(TokenBufferPos+2) = 117 Then
								If TokenBuffer.PeekByte(TokenBufferPos+3) = 111 Then
									If TokenBuffer.PeekByte(TokenBufferPos+4) = 116 Then
										If TokenBuffer.PeekByte(TokenBufferPos+5) = 59 Then
											ch = 34 '"'
											TokenBufferPos :+ 5
										End If
									End If
								End If
							End If
						Case 35 '#'
							Local tempCH:Int
							ch = 0
							TokenBufferPos :+ 2
							tempCH = TokenBuffer.PeekByte(TokenBufferPos)
							While tempCH <> 59 ';'
								ch :* 10
								ch :+ (tempCH & $000F)
								TokenBufferPos :+ 1
								tempCH = TokenBuffer.PeekByte(TokenBufferPos)
							Wend
					End Select
				End If
				
				Value = Value + Chr(ch)
				TokenBufferPos :+ 1
				If TokenBufferPos = TokenBufferLen Then Exit
				ch = TokenBuffer.PeekByte(TokenBufferPos)
				'ch = TokenBuffer._buf[TokenBufferPos]
			Wend
			TokenValue = Value
			Return TOKEN_QUOTEDTEXT
			
		Case 32, 13, 10 ' '
			'Blank spaces or empty lines
			Repeat
				TokenBufferPos :+ 1
				If TokenBufferPos = TokenBufferLen Then Exit
				ch = TokenBuffer.PeekByte(TokenBufferPos)
				'ch = TokenBuffer._buf[TokenBufferPos]
			Until ch <> 32 And ch <> 13 And ch <> 10
			TokenBufferPos :- 1
			Return TOKEN_NONE
	
		Default
			'Unquoted text
			Value = ""
			Repeat
				'Process escape codes
				If ch = 38 Then '&'
					Select TokenBuffer.PeekByte(TokenBufferPos+1)
						Case 108 'l'
							If TokenBuffer.PeekByte(TokenBufferPos+2) = 116 Then
								If TokenBuffer.PeekByte(TokenBufferPos+3) = 59 Then
									ch = 60
									TokenBufferPos :+ 3
								End If
							End If
						Case 103 'g'
							If TokenBuffer.PeekByte(TokenBufferPos+2) = 116 Then
								If TokenBuffer.PeekByte(TokenBufferPos+3) = 59 Then
									ch = 62
									TokenBufferPos :+ 3
								End If
							End If
						Case 97 'a'
							If TokenBuffer.PeekByte(TokenBufferPos+2) = 112 Then
								If TokenBuffer.PeekByte(TokenBufferPos+3) = 111 Then
									If TokenBuffer.PeekByte(TokenBufferPos+4) = 115 Then
										If TokenBuffer.PeekByte(TokenBufferPos+5) = 59 Then
											ch = 39 '''
											TokenBufferPos :+ 5
										End If
									End If
								End If
							Else If TokenBuffer.PeekByte(TokenBufferPos+2) = 109 Then
								If TokenBuffer.PeekByte(TokenBufferPos+3) = 112 Then
									If TokenBuffer.PeekByte(TokenBufferPos+4) = 59 Then
										ch = 38 '&'
										TokenBufferPos :+ 4
									End If
								End If							
							End If
						Case 113 'q'
							If TokenBuffer.PeekByte(TokenBufferPos+2) = 117 Then
								If TokenBuffer.PeekByte(TokenBufferPos+3) = 111 Then
									If TokenBuffer.PeekByte(TokenBufferPos+4) = 116 Then
										If TokenBuffer.PeekByte(TokenBufferPos+5) = 59 Then
											ch = 34 '"'
											TokenBufferPos :+ 5
										End If
									End If
								End If
							End If
						Case 35 '#'
							Local tempCH:Int
							ch = 0
							TokenBufferPos :+ 2
							tempCH = TokenBuffer.PeekByte(TokenBufferPos)
							While tempCH <> 59 ';'
								ch :* 10
								ch :+ (tempCH & $000F)
								TokenBufferPos :+ 1
								tempCH = TokenBuffer.PeekByte(TokenBufferPos)
							Wend
					End Select
				End If
				
				Value = Value + Chr(ch)
				TokenBufferPos :+ 1
				If TokenBufferPos >= _TokenBufferLen Then Exit
				ch = TokenBuffer.PeekByte(TokenBufferPos)
				'ch = TokenBuffer._buf[TokenBufferPos]
			Until ch = 60 Or ch = 47 Or ch = 61 Or ch = 62 Or ch = 13' Or ch = 32' Or TokenBufferPos >= (TokenBufferLen-1)
			TokenBufferPos :- 1
			TokenValue = Value
			Return TOKEN_TEXT

	End Select
	
End Function


Function _AddEscapeCodes:String(text:String)
	Local i:Int, ln:Int 
	Local rtext:String
	ln = Len(text)
	
	For i = 0 To ln-1
		Select text[i]
			Case 60 rtext = rtext + "&lt;"
			Case 62 rtext = rtext + "&gt;"
			Case 34 rtext = rtext + "&quot;"
			Case 39 rtext = rtext + "&apos;"
			Case 38 rtext = rtext + "&amp;"
			Default
				If (text[i] < 32 Or text[i] > 126) And text[i] <= 255 Then
					rtext = rtext + "&#" +String(text[i]) + ";"
				Else
					rtext = rtext + Chr(text[i])		
				End If
		End Select
	Next
	
	Return rtext
End Function


Function _CountNodes(node:xmlNode)
	Local subnode:xmlNode, count:Int
	
	'Count this node
	count = 1
	
	'Count children
	subnode = node.FirstChild()
	While subnode <> Null
		count :+ _CountNodes(subnode)
		subnode = subnode.NextSibling()
	Wend
	
	Return count
End Function

Public
