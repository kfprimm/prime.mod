
Strict

Rem
	bbdoc: Block Packer
End Rem
Module Prime.BlockPacker
ModuleInfo "Author: Kevin Primm"
ModuleInfo "License: MIT"
ModuleInfo "Credit: Adapted from John Ratcliff's TexturePacker."
ModuleInfo "Credit: http://code.google.com/p/texture-atlas"

Private

Type TRect
	Field mX1, mY1, mX2, mY2

  Method Set(x1,y1,x2,y2)
    mX1 = x1
    mY1 = y1
    mX2 = x2
    mY2 = y2
  End Method

  Method Intersects(r:TRect)
    If mX2 < r.mX1 Or mX1 > r.mX2 Or mY2 < r.mY1 Or mY1 > r.mY2 Return False
    Return True
  End Method
End Type

Type TBlock
  Field mWidth, mHeight
  Field mX, mY
  Field mLongestEdge, mArea

  Field mFlipped, mPlaced

  Method Set(wid,hit)
    mWidth = wid
    mHeight = hit
    mArea = wid*hit
    mLongestEdge = Max(wid, hit)
  End Method

  Method IsPlaced()
  	Return mPlaced
  End Method

  Method Place(x,y,flipped)
    mX = x
    mY = y
    mFlipped = flipped
    mPlaced = true
  End Method
End Type

Type TNode
  Field mNext:TNode
  Field mX, mY, mWidth, mHeight
  
  Method Create:TNode(x,y,wid,hit)
    mX = x
    mY = y
    mWidth = wid
    mHeight = hit
    Return Self
  End Method

	Method getNext:TNode()
		Return mNext
	End Method

  Method Fits(wid,hit,edgeCount Var)
    edgeCount = 0

    If wid = mWidth Or hit = mHeight Or wid = mHeight Or hit = mWidth
      If wid = mWidth
        edgeCount :+ 1
        If hit = mHeight edgeCount :+ 1
      ElseIf wid = mHeight
        edgeCount :+ 1
        If hit = mWidth edgeCount :+ 1
      ElseIf hit = mWidth
        edgeCount :+ 1
      ElseIf hit = mHeight
        edgeCount :+ 1
			EndIf
    EndIf

    If wid <= mWidth And hit <= mHeight Return True
    If hit <= mWidth And wid <= mHeight Return True

    Return False
  End Method

  Method GetRect(r:TRect)
    r.set(mX,mY,mX+mWidth-1,mY+mHeight-1)
  End Method

  Method Validate(n:TNode)
    Local r1:TRect = New TRect, r2:TRect = New TRect
    getRect(r1)
    n.getRect(r2)
    Assert Not r1.Intersects(r2)
  End Method

  Method Merge(n:TNode)
    Local r1:TRect = New TRect, r2:TRect = New TRect

    getRect(r1)
    n.getRect(r2)
    
    r1.mX2 :+ 1
    r1.mY2 :+ 1
    r2.mX2 :+ 1
    r2.mY2 :+ 1

    If r1.mX1 = r2.mX1 And r1.mX2 = r2.mX2 And r1.mY1 = r2.mY2
      mY = n.mY
      mHeight :+ n.mHeight
      Return True
    ElseIf r1.mX1 = r2.mX1 And r1.mX2 = r2.mX2 And r1.mY2 = r2.mY1
      mHeight :+ n.mHeight
      Return True
    ElseIf r1.mY1 = r2.mY1 And r1.mY2 = r2.mY1 And r1.mX1 = r2.mX2
      mX = n.mX
      mWidth :+ n.mWidth
      Return True
    ElseIf r1.mY1 = r2.mY1 And r1.mY2 = r2.mY1 And r1.mX2 = r2.mX1
      mWidth :+ n.mWidth
      Return True
    EndIf

		Return False
  End Method
End Type

Public

Const PACKER_NONE   = 0
Const PACKER_POW2   = 1
Const PACKER_SQR    = 2
Const PACKER_BORDER = 4

Type TBlockPacker
  Field mDebugCount
  Field mFreeList:TNode
  Field mTextures:TBlock[]
  Field mLongestEdge, mTotalArea
  
  Method Reset()
  	mDebugCount = 0
  	mFreeList = Null
  	mTextures = Null
  	mLongestEdge = 0
  	mTotalArea = 0
  End Method
  
  Method Add(wid,hit)
  	Local texture:TBlock = New TBlock
  	texture.Set(wid,hit)

  	mLongestEdge = Max(mLongestEdge, texture.mLongestEdge)
    mTotalArea :+ texture.mArea
    
   	mTextures :+ [texture]
  	Return mTextures.length - 1
  End Method

  Method newFree(x,y,wid,hit)
    Local node:TNode = New TNode.Create(x,y,wid,hit)
    node.mNext = mFreeList
    mFreeList = node
  End Method

  Function NextPow2(v)
    Local p = 1
    While p < v
      p = p*2
		Wend
    Return p
  End Function

  Method Run(width Var,height Var,flags = 0,minsize = -1)
    width = 0
    height = 0
    
    If flags&PACKER_BORDER
      For Local t:TBlock = EachIn mTextures
        t.mWidth :+ 2
        t.mHeight :+ 2
			Next
      mLongestEdge :+ 2
    EndIf
		
    If minsize > -1 mLongestEdge = Max(mLongestEdge, minsize)

    If flags&PACKER_POW2 mLongestEdge = nextPow2(mLongestEdge)

    width  = mLongestEdge
    Local count = mTotalArea / (mLongestEdge*mLongestEdge)
    height = (count+2)*mLongestEdge
		
    mDebugCount = 0
    newFree(0,0,width,height)

    For Local i = 0 Until mTextures.length
      Local index, longestEdge, mostArea

      For Local j = 0 Until mTextures.length
        Local t:TBlock = mTextures[j]

        If Not t.IsPlaced()
          If t.mLongestEdge > longestEdge
            mostArea = t.mArea
            longestEdge = t.mLongestEdge
            index = j
          ElseIf t.mLongestEdge = longestEdge
          	If t.mArea > mostArea
            	mostArea = t.mArea
            	index = j
          	EndIf
          EndIf
        EndIf
      Next

      Local t:TBlock = mTextures[index]

      Local leastY = $7FFFFFFF
      Local leastX = $7FFFFFFF

      Local previousBestFit:TNode, bestFit:TNode, previous:TNode
      Local search:TNode = mFreeList
      Local edgeCount = 0

      While search
        Local ec
        If search.Fits(t.mWidth,t.mHeight,ec)
          If ec = 2
            previousBestFit = previous
            bestFit = search
            edgeCount = ec
            Exit
          EndIf
          If search.mY < leastY
            leastY = search.mY
            leastX = search.mX
            previousBestFit = previous
            bestFit = search
            edgeCount = ec
          ElseIf search.mY = leastY And search.mX < leastX
            leastX = search.mX
            previousBestFit = previous
            bestFit = search
            edgeCount = ec
          EndIf
        EndIf
        previous = search
        search = search.mNext
      Wend

      Assert bestFit

      If bestFit
        Validate

        Select edgeCount
        Case 0
		      If t.mLongestEdge <= bestFit.mWidth
		        Local flipped = False

		        Local wid = t.mWidth
		        Local hit = t.mHeight

		        If hit > wid
		          wid = t.mHeight
		          hit = t.mWidth
		          flipped = True
		        EndIf

		        t.Place(bestFit.mX,bestFit.mY,flipped)

		        NewFree(bestFit.mX,bestFit.mY+hit,bestFit.mWidth,bestFit.mHeight-hit)

		        bestFit.mX :+ wid
		        bestFit.mWidth :- wid
		        bestFit.mHeight = hit
		        Validate
		      Else
		        Assert t.mLongestEdge <= bestFit.mHeight

		        Local flipped = False

		        Local wid = t.mWidth
		        Local hit = t.mHeight

		        if hit < wid
		          wid = t.mHeight
		          hit = t.mWidth
		          flipped = True
		        EndIf

		        t.Place(bestFit.mX,bestFit.mY,flipped)

		        NewFree(bestFit.mX,bestFit.mY+hit,bestFit.mWidth,bestFit.mHeight-hit)

		        bestFit.mX :+ wid
		        bestFit.mWidth :- wid
		        bestFit.mHeight = hit
		        Validate
		      EndIf
        Case 1
		      If t.mWidth = bestFit.mWidth
						t.Place(bestFit.mX,bestFit.mY,False)
						bestFit.mY :+ t.mHeight
						bestFit.mHeight :- t.mHeight
						Validate
		      ElseIf t.mHeight = bestFit.mHeight
						t.Place(bestFit.mX,bestFit.mY,False)
						bestFit.mX :+ t.mWidth
						bestFit.mWidth :- t.mWidth
						Validate
		      ElseIf t.mWidth = bestFit.mHeight
		        t.place(bestFit.mX,bestFit.mY,True)
		        bestFit.mX :+ t.mHeight
		        bestFit.mWidth :- t.mHeight
		        Validate
		      ElseIf t.mHeight = bestFit.mWidth
		        t.Place(bestFit.mX,bestFit.mY,True)
		        bestFit.mY :+ t.mWidth
		        bestFit.mHeight :- t.mWidth
		        Validate
		      EndIf
        Case 2
          Local flipped = t.mWidth <> bestFit.mWidth Or t.mHeight <> bestFit.mHeight
          T.Place(bestFit.mX,bestFit.mY,flipped)
          If previousBestFit
            previousbestFit.mNext = bestFit.mNext
          Else
            mFreeList = bestFit.mNext
					EndIf
          Validate
        End Select
        While mergeNodes() Wend
      EndIf
    Next

    height = 0;
    For Local t:TBlock = EachIn mTextures
      If flags&PACKER_BORDER
        t.mWidth :- 2
        t.mHeight :- 2
        t.mX :+ 1
        t.mY :+ 1
      EndIf

      Local y
      If t.mFlipped
        y = t.mY + t.mWidth
      Else
        y = t.mY + t.mHeight
      EndIf

      height = Max(height, y)
    Next
    
    If flags&PACKER_POW2 height = nextPow2(height)

		If flags&PACKER_SQR
			width = Max(width, height)
			height = Max(width, height)
		EndIf
		
    Return (width*height)-mTotalArea
	End Method
  
  Method MergeNodes()
    Local f:TNode = mFreeList
    While f
    	Local prev:TNode, c:TNode = mFreeList
      While c
        If f <> c
          If f.Merge(c)
            Assert prev
            prev.mNext = c.mNext
            Return True
          EndIf
        EndIf
        prev = c
        c = c.mNext
      Wend
      f = f.mNext
    Wend
    Return False
  End Method

  Method Validate()
    ?Debug
    Local f:TNode = mFreeList
    While f
      Local c:TNode = mFreeList
      while c
        If f <> c f.Validate(c)
        c = c.mNext
      Wend
      f = f.mNext
    Wend
    ?
  End Method

  Method Get(index, x Var, y Var, wid Var, hit Var)
    Assert index < mTextures.length
    
    If index < mTextures.length
      Local t:TBlock = mTextures[index]
      x = t.mX
      y = t.mY
      If t.mFlipped
        wid = t.mHeight
        hit = t.mWidth
      Else
        wid = t.mWidth
        hit = t.mHeight
      EndIf
      Return t.mFlipped
    EndIf

    Return False
  End Method
End Type
