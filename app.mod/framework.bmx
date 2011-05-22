
Strict

Import MaxGUI.Drivers
Import MaxGUI.ProxyGadgets
Import BRL.LinkedList
Import BRL.Reflection

Import "propertygrid.bmx"

Const CALLBACK_CANCELACTION	= 5

Global _globalapplication:TApplication

Type TAppObject
	Field _parent:TAppObject
	Field _children:TList,_link:TLink
	
	Field _currentevent:TEvent
	
	Method New()
		_children=CreateList()
		_currentevent=New TEvent
	End Method
	
	Method OnCreate();End Method
	Method OnFree();End Method
	
	Method Free() Abstract
	
	Method AddChild(child:TAppObject)
		child._link=_children.AddLast(child)
	End Method
	Method GetChildCount()
		Return _children.Count()
	End Method
	
	Method RegisterEvent(id,methd$,source:Object=Null)
		Local methdv:TMethod=TTypeId.ForObject(Self).FindMethod(methd)
		Assert methdv,"Method does not exist."
		_globalapplication.RegisterHandler id,Self,methdv,source,False
	End Method
	
	Method RegisterGlobalEvent(id,methd$,source:Object=Null)
		Local methdv:TMethod=TTypeId.ForObject(Self).FindMethod(methd)
		Assert methdv,"Method does not exist."
		_globalapplication.RegisterHandler id,Self,methdv,source,True
	End Method
		
	Method _SetEventInfo(evnt:TEvent)
		_currentevent.id=evnt.id;_currentevent.data=evnt.data
		_currentevent.mods=evnt.mods
		_currentevent.x=evnt.x;_currentevent.y=evnt.y
	End Method
	
	Method GetEventPos(x Var,y Var)
		x=_currentevent.x
		y=_currentevent.y
	End Method
	
	Method GetEventData()
		Return _currentevent.data
	End Method
End Type

Type TWidget Extends TAppObject
	Field _gadget:TGadget
	
	Method SetSize(width,height)
		CalcSize width,height
		SetGadgetShape _gadget,GadgetX(_gadget),GadgetY(_gadget),width,height
	End Method
	
	Method GetSize(width Var,height Var)
		width=GadgetWidth(_gadget)
		height=GadgetHeight(_gadget)
	End Method
	
	Method SetSensitivity(flags)
		SetGadgetSensitivity _gadget,flags
	End Method
	
	Method Center(x=True,y=True) Final
		Local parent:TGadget=GadgetGroup(_gadget)
		If parent=Null parent=Desktop()
		If x=False x=GadgetX(_gadget)	Else x=(ClientWidth(parent)-GadgetWidth(_gadget))/2
		If y=False y=GadgetY(_gadget) Else y=(ClientHeight(parent)-GadgetHeight(_gadget))/2
		SetGadgetShape _gadget,x,y,GadgetWidth(_gadget),GadgetHeight(_gadget)
	End Method
	
	Method SetVisibility(visible) Final
		If visible ShowGadget _gadget
		If Not visible HideGadget _gadget
	End Method
	
	Method SetLayout(lft,rght,tp,bttm)
		SetGadgetLayout _gadget,lft,rght,tp,bttm
	End Method
	
	Method Show() Final
		SetVisibility True
	End Method
	
	Method Hide() Final
		SetVisibility False
	End Method
	
	Method Disable() Final
		SetAbility False
	End Method
	
	Method Enable() Final
		SetAbility True
	End Method
	
	Method SetAbility(enab) Final
		If enab EnableGadget _gadget
		If Not enab DisableGadget _gadget
	End Method
	
	Method Activate()
		ActivateGadget _gadget
	End Method
	
	Method CalcSize(width Var,height Var,x=0,y=0,obj:TAppObject=Null)
		Local gadget:TWidget=TWidget(obj)
		If gadget=Null gadget=TWidget(_parent)
		If gadget
			Local pwidth,pheight
			gadget.GetClientSize pwidth,pheight
			DebugLog width+","+pwidth
			If width<1 If width<0 width=pwidth-Abs(width) Else width=pwidth-x
			If height<1 If height<0 height=pheight-Abs(height) Else height=pheight-y
		End If
	End Method
	
	Method GetClientSize(width Var,height Var)
		width=ClientWidth(_gadget)
		height=ClientHeight(_gadget)
	End Method
	
	Method Free() Final
		If OnFree()=CALLBACK_CANCELACTION Return
		FreeGadget _gadget
		If _link RemoveLink _link
	End Method
	
	Method _SetOwnership(parent:TAppObject) Final
		If parent parent.AddChild(Self)
		If _gadget _gadget.extra=Self
		_parent=parent
	End Method
	
	Method RegisterEvent(id,methd$,source:Object=Null)
		If source=Null source=_gadget
		Super.RegisterEvent id,methd,source
	End Method
	
	Method RegisterMenuAction(id,methd$)
		Local obj:TAppObject=Self,objmethd:TMethod
		Repeat
			If obj=Null Return False
			objmethd=TTypeId.ForObject(obj).FindMethod("_RegisterMenuAction")
			If objmethd<>Null Exit
			obj=obj._parent			
		Forever
		objmethd.Invoke obj,[Object(String(id)),Object(methd),Object(Self)]
		Return True
	End Method
	
	Method GetGadget:TGadget(param=True);End Method
End Type

Type TCollectionGadget Extends TWidget
	Field _item:TWidgetItem[]
	
	Method AddItem(text$,item:TWidgetItem)
		item._text=text
		_item=_item[.._item.length+1]
		_item[_item.length-1]=item
	
		AddGadgetItem _gadget,text,,,,item
		item._SetOwnership Self
		item._gadget=_gadget
		item._OnAddition;item.OnAddition
	End Method
End Type

Type TWidgetItem Extends TWidget
	Field _text$
	
	Method _OnAddition() Abstract
	Method OnAddition() Abstract
End Type

Const MENU_DISABLED	= 1
Const MENU_CHECKED	= 2
Type TMenu Extends TWidget Final
	Field _text$,_tag,_hotkey,_modifier,_style
	Field _ispopup,_window:TAppObject
	Field _istoggled
	
	Method New()
		_ispopup=-1
	End Method
	
	Method Create:TMenu(popup=False)
		_ispopup=popup
		Return Self
	End Method
	
	Method Append:TMenu(text$,tag,hotkey=0,modifier=0,style=0)
		Local menu:TMenu=New TMenu.Create()
		menu._text=text;menu._tag=tag;menu._hotkey=hotkey;menu._modifier=modifier;menu._style=style
		If _gadget
			menu._gadget=CreateMenu(text,tag,_gadget,hotkey,modifier)
			If _style&MENU_DISABLED DisableMenu menu._gadget
			If _style&MENU_CHECKED CheckMenu menu._gadget
			menu._gadget.extra=Self
		EndIf
		menu._SetOwnership(Self)
		Return menu
	End Method
	
	Method AppendToggle:TMenu(text$,tag,hotkey=0,modifier=0)
		Local menu:TMenu=Append(text,tag,hotkey,modifier)
		menu._istoggled=True
		Return menu
	End Method
	
	Method Checked()
		Return MenuChecked(_gadget)
	End Method
	
	Method SetCheck(checked)
		If checked CheckMenu(_gadget)
		If Not checked UncheckMenu(_gadget)
	End Method
	
	Method ToggleCheck()
		SetCheck Not Checked()
	End Method
	
	Method AppendSeparator()
		Append("",0)
	End Method
	
	Method _Create() 
		If _gadget=Null
			Local parent:TGadget=TWidget(_parent)._gadget
			_gadget=CreateMenu(_text,_tag,parent,_hotkey,_modifier)
			If _style&MENU_DISABLED DisableMenu _gadget
			If _style&MENU_CHECKED CheckMenu _gadget
			_gadget.extra=Self			
			_window=TMenu(_parent)._window
		EndIf
		For Local child:TMenu=EachIn _children
			child._Create()
		Next
	End Method
End Type

Type TMenuAction Final
	Field _tag,_methd:TMethod
	Field _obj:Object
	
	Method Create:TMenuAction(tag,methd:TMethod,obj:Object)
		_tag=tag;_methd=methd
		_obj=obj
		Return Self
	End Method
	
	Method SetMethod(methd:TMethod,obj:Object)
		_methd=methd
		_obj=obj
	End Method
End Type

Type TWindow Extends TWidget
	Field _action:TList
	
	Method New()
		_action=CreateList()
	End Method
	
	Method Create:TWindow(text$,x=-1,y=-1,width,height,parent:TAppObject=Null,style=WINDOW_DEFAULT) Final
		Local gparent:TGadget
		If TWidget(parent) gparent=TWidget(parent)._gadget
		_gadget=CreateWindow(text,x,y,width,height,gparent,style|WINDOW_HIDDEN)
		Center x<0,y<0	
		_SetOwnership parent
		If OnCreate()=CALLBACK_CANCELACTION Free();Return Null
		If style&WINDOW_HIDDEN=0 Show()
		Return Self
	End Method
	
	Method AppendMenu(menu:TMenu,text$) Final
		Assert menu._ispopup=0,"Menu is not appendable."
		menu._gadget=CreateMenu(text,0,WindowMenu(_gadget))
		menu._window=Self
		menu._SetOwnership Self
		menu._Create
		UpdateWindowMenu _gadget
	End Method
	
	Method _RegisterMenuAction(tag,methd$,obj:Object=Null) Final
		If obj=Null obj=Self
		Local methdv:TMethod=TTypeId.ForObject(obj).FindMethod(methd)
		Assert methdv,"Method does not exist."
		Local action:TMenuAction=FindMenuAction(tag)
		If action=Null action=New TMenuAction.Create(tag,methdv,obj);_action.AddLast(action)
		action.SetMethod(methdv,obj)
	End Method
	
	Method FindMenuAction:TMenuAction(tag) Final
		For Local action:TMenuAction=EachIn _action
			If action._tag=tag Return action
		Next
		Return Null
	End Method
	
	Method SetMinSize(width,height)
		SetMinWindowSize _gadget,width,height
	End Method
	
	Method ExecuteMenuAction(tag) Final
		Local action:TMenuAction=FindMenuAction(tag)
		If action<>Null
			Local methd:TMethod=action._methd
			Local args:Object[]=New Object[methd.ArgTypes().length]
			If args.length>0 args[0]=String(tag)
			action._methd.Invoke action._obj,args
			Return True
		EndIf
		Return False
	End Method
	
	Method Close() Final
		If Not OnCloseButton() Return
		Free()
	End Method
	
	Method OnCloseButton();End Method
	Method OnSize();End Method
End Type

Type TCanvas Extends TWidget
	Method Create:TCanvas(x=0,y=0,width=0,height=0,parent:TAppObject,style=0) Final
		CalcSize width,height,x,y,parent
		_gadget=CreateCanvas(x,y,width,height,TWidget(parent)._gadget,style)
		_SetOwnership(parent)
		If OnCreate()=CALLBACK_CANCELACTION Free();Return Null
		Return Self
	End Method
	
	Method GetGraphics:TGraphics() Final
		Return CanvasGraphics(_gadget)
	End Method
	
	Method Paint() Final
		RedrawGadget _gadget
	End Method
	
	Method OnPaint();End Method
End Type

Type TPanel Extends TWidget
	Method Create:TPanel(x=0,y=0,width=0,height=0,parent:TAppObject,style=0,title$="")
		CalcSize width,height,x,y,parent
		_gadget=CreatePanel(x,y,width,height,TWidget(parent)._gadget,style,title)
		DebugLog width
		_SetOwnership(parent)
		If OnCreate()=CALLBACK_CANCELACTION Free();Return Null
		Return Self
	End Method
End Type

Type TTabber Extends TCollectionGadget Final
	Method Create:TTabber(x=0,y=0,width=0,height=0,parent:TAppObject,style=0)
		CalcSize width,height,x,y,parent
		_gadget=CreateTabber(x,y,width,height,TWidget(parent)._gadget,style)
		If OnCreate()=CALLBACK_CANCELACTION Free();Return Null
		_SetOwnership(parent)
		Return Self
	End Method
End Type

Type TTabItem Extends TWidgetItem
	Field _panel:TPanel
	
	Method _OnAddition()
		_panel=New TPanel.Create(,,,,Self)
		_gadget=_panel._gadget
		SetLayout EDGE_ALIGNED,EDGE_ALIGNED,EDGE_ALIGNED,EDGE_ALIGNED
	End Method	
	Method OnAddition();End Method
End Type

Type TLabel Extends TWidget
	Method Create:TLabel(text$,x=0,y=0,width=0,height=0,parent:TAppObject,style=0)
		CalcSize width,height,x,y,parent
		_gadget=CreateLabel(text,x,y,width,height,TWidget(parent)._gadget,style)
		_SetOwnership(parent)
		If OnCreate()=CALLBACK_CANCELACTION Free();Return Null
		Return Self
	End Method
End Type

Type TComboBox Extends TWidget
	Method Create:TComboBox(x=0,y=0,width=0,height=0,parent:TAppObject,style=0)
		CalcSize width,height,x,y,parent
		_gadget=CreateComboBox(x,y,width,height,TWidget(parent)._gadget,style)
		_SetOwnership(parent)
		If OnCreate()=CALLBACK_CANCELACTION Free();Return Null
		Return Self
	End Method
End Type

Type TSplitter Extends TWidget
	Field _mainpanel:TPanel,_sidepanel:TPanel
	
	Method Create:TSplitter(x,y,width,height,parent:TAppObject,orientation=SPLIT_VERTICAL)
		Local gparent:TGadget
		If TWidget(parent) gparent=TWidget(parent)._gadget
		If width=-1 width=ClientWidth(gparent)-x
		If height=-1 height=ClientHeight(gparent)-y
		_gadget=CreateSplitter(x,y,width,height,gparent,orientation)
		_SetOwnership(parent)
		_mainpanel=New TPanel
		_mainpanel._gadget=SplitterPanel(TSplitter(_gadget),SPLITPANEL_MAIN)
		_mainpanel._SetOwnership Self
		_sidepanel=New TPanel
		_sidepanel._gadget=SplitterPanel(TSplitter(_gadget),SPLITPANEL_SIDEPANE)
		_sidepanel._SetOwnership Self
		If OnCreate()=CALLBACK_CANCELACTION Free();Return Null
		Return Self
	End Method
	
	Method SetPosition(position,save=True)
		SetSplitterPosition TSplitter(_gadget),position,save
	End Method
	
	Method SetBehavior(flags)
		SetSplitterBehavior TSplitter(_gadget),flags
	End Method
	
	Method SetOrientation(orientation)
		SetSplitterOrientation TSplitter(_gadget),orientation
	End Method
	
	Method GetMainPanel:TPanel()
		Return _mainpanel
	End Method
	
	Method GetSidePanel:TPanel()
		Return _sidepanel
	End Method
End Type

Type TPropertyGrid Extends TWidget
	Method Create:TPropertyGrid(x,y,width,height,parent:TAppObject)
		Local gparent:TGadget
		If TWidget(parent) gparent=TWidget(parent)._gadget
		If width=-1 width=ClientWidth(gparent)-x
		If height=-1 height=ClientHeight(gparent)-y
		_gadget=TPropertyGrid.Create(x,y,width,height,gparent)
		_SetOwnership(parent)
		If OnCreate()=CALLBACK_CANCELACTION Free();Return Null
		Return Self
	End Method
	
	Method AddGroup:TPropertyGroup(title$)
		Return New TPropertyGroup.Create(_gadget,title)
	End Method
End Type

Type TPropertyGroup
	Field _group:TPropertyGroup
	
	Method Create:TPropertyGroup(grid:TGadget,title$)
		_group=AddPropertyGridGroup(grid,title)
		Return Self
	End Method
	
	Method SetLabelWidth(width)
		_group.SetLabelWidth(width)
	End Method
	
	Method Expand(update=True)
		_group.Expand(update)
	End Method
	
	Method AdjustLayout()
		_group.AdjustLayout()
	End Method
	
	Method AddProperty(name$,value$="",style=0,tooltip$="",flags=0,label$="")
		_group.AddProperty(name,value,style,tooltip,flags,label) 
	End Method
End Type

Type TEventHandler
	Field _id,_source:Object
	Field _methd:TMethod,_obj:Object
	Field _isglobal
		
	Method Run(evnt:TEvent)
		Local arg:Object[]=New Object[_methd.ArgTypes().length]
		Local param$[]=_methd.MetaData("PFEventParams").Split(",")
		For Local i=0 To Min(param.length,arg.length)-1
			Select param[i].ToLower()
			Case "eventdata" arg[i]=String(evnt.data)
			Case "eventx" arg[i]=String(evnt.x)
			Case "eventy" arg[i]=String(evnt.y)
			End Select
		Next
		_methd.Invoke _obj,arg
		TAppObject(_obj)._SetEventInfo evnt
	End Method
End Type

Type TApplication Extends TAppObject
	Field _handlerlist:TMap
	
	Method New()
		_handlerlist=CreateMap()
	End Method
	
	Method Execute() Final
		_globalapplication=Self
		If OnCreate()=CALLBACK_CANCELACTION Free();Return
		AddHook EmitEventHook,EventHook,Self
		While GetChildCount()>0
			PollSystem
		Wend
	End Method
	
	Method Free() Final
		For Local child:TGadget=EachIn _children
			child.Free()
		Next
	End Method
	
	Method RegisterHandler:TEventHandler(id,obj:Object,methd:TMethod,source:Object=Null,glob)
		Local handlerlist:TList=FindHandlerList(id)
		If handlerlist=Null
			handlerlist=CreateList()
			MapInsert _handlerlist,String(id),handlerlist
		EndIf
		Local handler:TEventHandler=New TEventHandler
		handler._id=id;handler._source=source
		handler._methd=methd;handler._obj=obj
		handler._isglobal=glob
		handlerlist.AddLast handler
	End Method
	
	Method FindHandlerList:TList(id)
		Return TList(MapValueForKey(_handlerlist,String(id)))
	End Method
	
	Method HandleEvent(event:TEvent) Final
		Local source:TGadget=TGadget(event.source)
		Local handlerlist:TList=FindHandlerList(event.id)
		If handlerlist<>Null
			For Local handler:TEventHandler=EachIn handlerlist
				If (handler._isglobal=True Or handler._source=event.source) Or (handler._source=TAppObject(source.extra) And TAppObject(source.extra)<>Null) handler.Run(event)
			Next
		EndIf
		Select event.id
		Case EVENT_WINDOWCLOSE
			If TWindow(source.extra).OnCloseButton() TWindow(source.extra).Free()
		Case EVENT_WINDOWSIZE
			TWindow(source.extra).OnSize()
		Case EVENT_GADGETPAINT
			TCanvas(source.extra).OnPaint()
		Case EVENT_MENUACTION
			Local menu:TMenu=TMenu(source.extra)
			If menu._istoggled menu.ToggleCheck()
			Local window:TWindow=TWindow(menu._window)
			window.ExecuteMenuAction event.data
		End Select
	End Method
	
	Function EventHook:Object(id,data:Object,context:Object)
		Local event:TEvent=TEvent(data)
		If event TApplication(context).HandleEvent(event)
	End Function
End Type
