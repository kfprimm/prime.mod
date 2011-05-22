
Strict

Module Prime.DeltaTimer
ModuleInfo "Author: Kevin Primm"
ModuleInfo "License: MIT"

Import BRL.System

Type TDeltaTimer
	Field _targettps#
	Field _lastupdate
	Function Create:TDeltaTimer(tps)
		Local timer:TDeltaTimer=New TDeltaTimer
		timer._targettps=tps
		Return timer
	End Function
	Method Update#()
		Local delta#=Max(0,(MilliSecs()-_lastupdate)/(1000.0/_targettps))
		_lastupdate=MilliSecs()
		Return delta
	End Method
	Method GetFPS()
		Global _ticks,_fps,_lastup
		If _lastup+1000<MilliSecs()
			_lastup=MilliSecs()
			_fps=_ticks
			_ticks=0
			Return _fps
		EndIf	
		_ticks:+1
		Return _fps
	End Method
End Type