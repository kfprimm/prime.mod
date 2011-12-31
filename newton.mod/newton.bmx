
Strict

Rem
	bbdoc: Netwon Module for BlitzMax
End Rem
Module Prime.Newton
ModuleInfo "Author: Kevin Primm"
ModuleInfo "License: MIT"
ModuleInfo "Version: 0.01"
ModuleInfo "Newton Version: 2.33"

?Linux
ModuleInfo "CC_OPTS: -Wall -Wno-strict-aliasing -D_LINUX_VER -O2 -g -fpic -msse -msse2 -ffloat-store -ffast-math -freciprocal-math -funsafe-math-optimizations -fsingle-precision-constant"

?Win32
Import "newton.a"
Import "dJointLibrary.a"

?Not Win32
Import "source/core/*.h"

Import "source/core/dg.cpp"
Import "source/core/dgRef.cpp"
Import "source/core/dgCRC.cpp"
Import "source/core/dgTree.cpp"
Import "source/core/dgNode.cpp"
Import "source/core/dgTypes.cpp"
Import "source/core/dgMatrix.cpp"
Import "source/core/dgGoogol.cpp"
Import "source/core/dgMemory.cpp"
Import "source/core/dgSphere.cpp"
Import "source/core/dgRandom.cpp"
Import "source/core/dgThreads.cpp"
Import "source/core/dgPolyhedra.cpp"
Import "source/core/dgQuaternion.cpp"
Import "source/core/dgConvexHull3d.cpp"
Import "source/core/dgConvexHull4d.cpp"
Import "source/core/dgIntersections.cpp"
Import "source/core/dgAABBPolygonSoup.cpp"
Import "source/core/dgSmallDeterminant.cpp"
Import "source/core/dgPolygonSoupBuilder.cpp"
Import "source/core/dgPolyhedraMassProperties.cpp"
Import "source/core/dgDelaunayTetrahedralization.cpp"

Import "source/physics/*.h"

Import "source/physics/dgWorld.cpp"
Import "source/physics/dgBody.cpp"
Import "source/physics/dgMeshEffect.cpp"
Import "source/physics/dgMeshEffect2.cpp"
Import "source/physics/dgMeshEffectSolidTree.cpp"
Import "source/physics/dgCollisionConvex.cpp"
Import "source/physics/dgBallConstraint.cpp"
Import "source/physics/dgBilateralConstraint.cpp"
Import "source/physics/dgCorkscrewConstraint.cpp"
Import "source/physics/dgHingeConstraint.cpp"
Import "source/physics/dgNarrowPhaseCollision.cpp"
Import "source/physics/dgBroadPhaseCollision.cpp"
Import "source/physics/dgMinkowskiConv.cpp"
Import "source/physics/dgCollisionBox.cpp"
Import "source/physics/dgCollisionCapsule.cpp"
Import "source/physics/dgCollisionBVH.cpp"
Import "source/physics/dgCollisionMesh.cpp"
Import "source/physics/dgCollisionUserMesh.cpp"
Import "source/physics/dgCollisionChamferCylinder.cpp"
Import "source/physics/dgPointToCurveConstraint.cpp"
Import "source/physics/dgCollisionCone.cpp"
Import "source/physics/dgCollisionConvexHull.cpp"
Import "source/physics/dgSlidingConstraint.cpp"
Import "source/physics/dgCollisionConvexModifier.cpp"
Import "source/physics/dgUniversalConstraint.cpp"
Import "source/physics/dgCollision.cpp"
Import "source/physics/dgUpVectorConstraint.cpp"
Import "source/physics/dgCollisionCylinder.cpp"
Import "source/physics/dgUserConstraint.cpp"
Import "source/physics/dgCollisionEllipse.cpp"
Import "source/physics/dgCollisionNull.cpp"
Import "source/physics/dgCollisionScene.cpp"
Import "source/physics/dgCollisionCompoundBreakable.cpp"
Import "source/physics/dgCollisionHeightField.cpp"
Import "source/physics/dgWorldDynamicUpdate.cpp"
Import "source/physics/dgCollisionSphere.cpp"
Import "source/physics/dgCollisionCompound.cpp"
Import "source/physics/dgConnectorConstraint.cpp"
Import "source/physics/dgConstraint.cpp"
Import "source/physics/dgBodyMasterList.cpp"
Import "source/physics/dgContact.cpp"

Import "source/newton/Newton.cpp"
Import "source/newton/NewtonClass.cpp"
?

Const NEWTON_MAJOR_VERSION = 2
Const NEWTON_MINOR_VERSION = 31

Const NEWTON_PROFILER_WORLD_UPDATE						= 0

Const NEWTON_PROFILER_COLLISION_UPDATE					= 1
Const NEWTON_PROFILER_FORCE_CALLBACK_UPDATE				= 2
Const NEWTON_PROFILER_COLLISION_UPDATE_BROAD_PHASE		= 3
Const NEWTON_PROFILER_COLLISION_UPDATE_NARROW_PHASE		= 4

Const NEWTON_PROFILER_DYNAMICS_UPDATE					= 5
Const NEWTON_PROFILER_DYNAMICS_CONSTRAINT_GRAPH			= 6
Const NEWTON_PROFILER_DYNAMICS_SOLVE_CONSTRAINT_GRAPH	= 7 

Const SERIALIZE_ID_BOX					= 0	
Const SERIALIZE_ID_CONE					= 1
Const SERIALIZE_ID_SPHERE				= 2
Const SERIALIZE_ID_CAPSULE				= 3
Const SERIALIZE_ID_CYLINDER				= 4
Const SERIALIZE_ID_COMPOUND				= 5
Const SERIALIZE_ID_CONVEXHULL			= 6
Const SERIALIZE_ID_CONVEXMODIFIER		= 7
Const SERIALIZE_ID_CHAMFERCYLINDER		= 8
Const SERIALIZE_ID_TREE					= 9
Const SERIALIZE_ID_NULL					= 10
Const SERIALIZE_ID_HEIGHTFIELD			= 11
Const SERIALIZE_ID_USERMESH				= 12
Const SERIALIZE_ID_SCENE				= 13
Const SERIALIZE_ID_COMPOUND_BREAKABLE	= 14

Rem
	struct NewtonCollisionInfoRecord
	{
		dFloat m_offsetMatrix[4][4];
		int m_collisionType;				' tag id to identify the collision primitive
		int m_referenceCount;				' the current reference count for this collision		
		int m_collisionUserID;				

		struct NewtonBoxParam
		{
			dFloat m_x;
			dFloat m_y;
			dFloat m_z;
		};

		struct NewtonSphereParam
		{
			dFloat m_r0;
			dFloat m_r1;
			dFloat m_r2;
		};

		struct NewtonCylinderParam
		{
			dFloat m_r0;
			dFloat m_r1;
			dFloat m_height;
		};

		struct NewtonCapsuleParam
		{
			dFloat m_r0;
			dFloat m_r1;
			dFloat m_height;
		};

		struct NewtonConeParam
		{
			dFloat m_r;
			dFloat m_height;
		};

		struct NewtonChamferCylinderParam
		{
			dFloat m_r;
			dFloat m_height;
		};

		struct NewtonConvexHullParam
		{
			int m_vertexCount;
			int m_vertexStrideInBytes;
			int m_faceCount;
			dFloat* m_vertex;
		};

		struct NewtonConvexHullModifierParam
		{
			NewtonCollision* m_chidren;
		};


		struct NewtonCompoundCollisionParam
		{
			int m_chidrenCount;
			NewtonCollision** m_chidren;
		};

		struct NewtonCollisionTreeParam
		{
			int m_vertexCount;
			int m_indexCount;
		};

		struct NewtonHeightFieldCollisionParam
		{
			int m_width;
			int m_height;
			int m_gridsDiagonals;
			dFloat m_horizonalScale;
			dFloat m_verticalScale;
			unsigned short *m_elevation;
			char *m_atributes;
		};

		struct NewtonSceneCollisionParam
		{
			int m_childrenProxyCount;
		};

		union {
			NewtonBoxParam m_box;									
			NewtonConeParam m_cone;
			NewtonSphereParam m_sphere;
			NewtonCapsuleParam m_capsule;
			NewtonCylinderParam m_cylinder;
			NewtonChamferCylinderParam m_chamferCylinder;
			NewtonConvexHullParam m_convexHull;
			NewtonCompoundCollisionParam m_compoundCollision;
			NewtonConvexHullModifierParam m_convexHullModifier;
			NewtonCollisionTreeParam m_collisionTree;
			NewtonHeightFieldCollisionParam m_heightField;
			NewtonSceneCollisionParam m_sceneCollision;
			dFloat m_paramArray[64];		  ' user define collision can use this to store information
		};
	};

	struct NewtonJointRecord
	{
		dFloat m_attachmenMatrix_0[4][4];
		dFloat m_attachmenMatrix_1[4][4];
		dFloat m_minLinearDof[3];
		dFloat m_maxLinearDof[3];
		dFloat m_minAngularDof[3];
		dFloat m_maxAngularDof[3];
		const NewtonBody* m_attachBody_0;
		const NewtonBody* m_attachBody_1;
		dFloat m_extraParameters[16];
		int	m_bodiesCollisionOn;
		char m_descriptionType[32];
	} ;


	
	struct NewtonUserMeshCollisionCollideDesc
	{
		dFloat m_boxP0[4];						' lower bounding box of intersection query in local space
		dFloat m_boxP1[4];						' upper bounding box of intersection query in local space
		int m_threadNumber;						' current thread executing this query
		int	m_faceCount;                        ' the application should set here how many polygons intersect the query box
		int m_vertexStrideInBytes;              ' the application should set here the size of each vertex
		void* m_userData;                       ' user data passed to the collision geometry at creation time
		dFloat* m_vertex;                       ' the application should the pointer to the vertex array. 
	 	int* m_userAttribute;                   ' the application should set here the pointer to the user data, one for each face
		int* m_faceIndexCount;                  ' the application should set here the pointer to the vertex count of each face.
		int* m_faceVertexIndex;                 ' the application should set here the pointer index array for each vertex on a face.
		NewtonBody* m_objBody;                  ' pointer to the colliding body
		NewtonBody* m_polySoupBody;             ' pointer to the rigid body owner of this collision tree 
	};

	struct NewtonWorldConvexCastReturnInfo
	{
		dFloat m_point[4];						' collision point in global space
		dFloat m_normal[4];						' surface normal at collision point in global space
		dFloat m_normalOnHitPoint[4];      ' surface normal at the surface of the hit body, 
												' is the same as the normal calculated by a ray cast hitting the body at the hit point
		dFloat m_penetration;          ' contact penetration at collision point
		int  m_contactID;	          ' collision ID at contact point
		const NewtonBody* m_hitBody;			' body hit at contact point
	};
	
	struct NewtonUserMeshCollisionRayHitDesc
	{
		dFloat m_p0[4];							' ray origin in collision local space
		dFloat m_p1[4];             ' ray destination in collision local space  
		dFloat m_normalOut[4];					' copy here the normal at the ray intersection
		int m_userIdOut;            ' copy here a user defined id for further feedback 
		void* m_userData;            ' user data passed to the collision geometry at creation time
	};

	struct NewtonHingeSliderUpdateDesc
	{
		dFloat m_accel;
		dFloat m_minFriction;
		dFloat m_maxFriction;
		dFloat m_timestep;
	} ;
End Rem

Rem
	' Newton callback functions
	typedef void* (*NewtonAllocMemory) (int sizeInBytes)
	Function (*NewtonFreeMemory) (void* const ptr, int sizeInBytes)
	Function (*NewtonDestroyWorld) (newtonWorld:Byte Ptr)

	typedef unsigned (*NewtonGetTicksCountCallback) ()

	Function NewtonSerialize) (void* const serializeHandle, const void* buffer, int size)
	Function NewtonDeserialize) (void* const serializeHandle, void* buffer, int size)
	
	' user collision callbacks	
	Function (*NewtonUserMeshCollisionDestroyCallback) (void* const userData)
	Function (*NewtonUserMeshCollisionCollideCallback) (NewtonUserMeshCollisionCollideDesc* const collideDescData)
	typedef dFloat (*NewtonUserMeshCollisionRayHitCallback) (NewtonUserMeshCollisionRayHitDesc* const lineDescData)
	Function (*NewtonUserMeshCollisionGetCollisionInfo) (void* const userData, NewtonCollisionInfoRecord* const infoRecord)
	Function (*NewtonUserMeshCollisionGetFacesInAABB) (void* const userData, const dFloat* const p0, const dFloat* const p1,
														  const dFloat** const vertexArray, int* const vertexCount, int* const vertexStrideInBytes, 
		                          const int* const indexList, int maxIndexCount, const int* const userDataList)

	typedef dFloat (*NewtonCollisionTreeRayCastCallback) (body:Byte Ptr, const NewtonCollision* const treeCollision, dFloat interception, dFloat* normal, int faceId, void* usedData)
	typedef dFloat (*NewtonHeightFieldRayCastCallback) (body:Byte Ptr, const NewtonCollision* const heightFieldCollision, dFloat interception, int row, int col, dFloat* normal, int faceId, void* usedData)

	
	' collision tree call back (obsoleted no recommended)
	Function (*NewtonTreeCollisionCallback) (body:Byte PtrWithTreeCollision, body:Byte Ptr, int faceID, 
												 int vertexCount, const dFloat* const vertex, int vertexStrideInBytes) 

	Function NewtonBodyDestructor (body:Byte Ptr)
	Function NewtonApplyForceAndTorque (body:Byte Ptr, dFloat timestep, int threadIndex)
	Function NewtonSetTransform (body:Byte Ptr, const matrix:Float Ptr, int threadIndex)

	Function (*NewtonIslandUpdate) (newtonWorld:Byte Ptr, const void* islandHandle, int bodyCount)
	Function (*NewtonBodyLeaveWorld) (body:Byte Ptr, int threadIndex)
	Function (*NewtonDestroyBodyByExeciveForce) (body:Byte Ptr, const NewtonJoint* const contact)
	Function (*NewtonCollisionDestructor) (newtonWorld:Byte Ptr, collision:Byte Ptr)

	Function (*NewtonCollisionCompoundBreakableCallback) (NewtonMesh* const mesh, void* const userData, dFloat* const planeMatrixOut)

	Function (*NewtonGetBuoyancyPlane) (const int collisionID, void* const context, const dFloat* const globalSpaceMatrix, dFloat* const globalSpacePlane)
	typedef unsigned (*NewtonWorldRayPrefilterCallback)(body:Byte Ptr, collision:Byte Ptr, void* const userData)
	Function NewtonWorldRayFilterCallback#(body:Byte Ptr, const dFloat* const hitNormal, int collisionID, void* const userData, dFloat intersectParam)
	

	Function  (*NewtonOnAABBOverlap) (const NewtonMaterial* const material, body:Byte Ptr0, body:Byte Ptr1, int threadIndex)
	Function (*NewtonContactsProcess) (const NewtonJoint* const contact, dFloat timestep, int threadIndex)

	Function (*NewtonBodyIterator) (body:Byte Ptr, void* const userData)
	Function (*NewtonJointIterator) (joint:Byte Ptr, void* const userData)
	Function (*NewtonCollisionIterator) (void* const userData, int vertexCount, const dFloat* const faceArray, int faceId)

	Function (*NewtonBallCallBack) (ball:Byte Ptr, dFloat timestep)
	typedef unsigned (*NewtonHingeCallBack) (hinge:Byte Ptr, NewtonHingeSliderUpdateDesc* const desc)
	typedef unsigned (*NewtonSliderCallBack) (const NewtonJoint* const slider, NewtonHingeSliderUpdateDesc* const desc)
	typedef unsigned (*NewtonUniversalCallBack) (universal:Byte Ptr, NewtonHingeSliderUpdateDesc* const desc)
	typedef unsigned (*NewtonCorkscrewCallBack) (corkscrew:Byte Ptr, NewtonHingeSliderUpdateDesc* const desc)

	Function NewtonUserBilateralCallBack) (const NewtonJoint* const userJoint, dFloat timestep, int threadIndex)
	Function NewtonUserBilateralGetInfoCallBack) (const NewtonJoint* const userJoint, NewtonJointRecord* const info)

	Function NewtonConstraintDestructor) (const NewtonJoint*const  me)

'	typedef void (*NewtonSetRagDollTransform) (const NewtonRagDollBone* const bone)
'	typedef void (*NewtonBodyActivationState) (body:Byte Ptr, unsigned state)
'	typedef void (*NewtonVehicleTireUpdate) (const NewtonJoint* const vehicle, dFloat timestep)
End Rem
Extern
	Function NewtonWorldGetVersion()
	Function NewtonWorldFloatSize()

	Function NewtonGetMemoryUsed()
	Function NewtonSetMemorySystem(malloc:Byte Ptr, mfree:Byte Ptr)

	Function NewtonCreate:Byte Ptr()
	Function NewtonDestroy(newtonWorld:Byte Ptr)
	Function NewtonDestroyAllBodies(newtonWorld:Byte Ptr)

	Function NewtonUpdate(newtonWorld:Byte Ptr, timestep#)
	Function NewtonInvalidateCache(newtonWorld:Byte Ptr)
	Function NewtonCollisionUpdate(newtonWorld:Byte Ptr)

	Function NewtonSetSolverModel (newtonWorld:Byte Ptr, model)
	Function NewtonSetPlatformArchitecture (newtonWorld:Byte Ptr, mode)
	Function NewtonGetPlatformArchitecture(newtonWorld:Byte Ptr, description:Byte Ptr)
	Function NewtonSetMultiThreadSolverOnSingleIsland (newtonWorld:Byte Ptr, mode)
	Function NewtonGetMultiThreadSolverOnSingleIsland (newtonWorld:Byte Ptr)

	Function NewtonSetPerformanceClock (newtonWorld:Byte Ptr, callback:Byte Ptr)
	Function NewtonReadPerformanceTicks (newtonWorld:Byte Ptr, performanceEntry)
	Function NewtonReadThreadPerformanceTicks (newtonWorld:Byte Ptr, threadIndex)
	
	Function NewtonWorldCriticalSectionLock (newtonWorld:Byte Ptr)
	Function NewtonWorldCriticalSectionUnlock (newtonWorld:Byte Ptr)
	Function NewtonSetThreadsCount (newtonWorld:Byte Ptr, threads)
	Function NewtonGetThreadsCount(newtonWorld:Byte Ptr)
	Function NewtonGetMaxThreadsCount(newtonWorld:Byte Ptr)


	Function NewtonSetFrictionModel(newtonWorld:Byte Ptr, model)
	Function NewtonSetMinimumFrameRate(newtonWorld:Byte Ptr, frameRate#)
	Function NewtonSetBodyLeaveWorldEvent(newtonWorld:Byte Ptr, callback:Byte Ptr)
	Function NewtonSetWorldSize(newtonWorld:Byte Ptr, minPoint:Float Ptr, maxPoint:Float Ptr)
	Function NewtonSetIslandUpdateEvent(newtonWorld:Byte Ptr, islandUpdate:Byte Ptr)
	Function NewtonSetCollisionDestructor(newtonWorld:Byte Ptr, callback:Byte Ptr)
	Function NewtonSetDestroyBodyByExeciveForce(newtonWorld:Byte Ptr, callback:Byte Ptr)

Rem

'	Function NewtonWorldForEachBodyDo (newtonWorld:Byte Ptr, callback:Byte Ptr)
	Function NewtonWorldForEachJointDo (newtonWorld:Byte Ptr, NewtonJointIterator callback, void* const userData)
	Function NewtonWorldForEachBodyInAABBDo (newtonWorld:Byte Ptr, const dFloat* const p0, const dFloat* const p1, NewtonBodyIterator callback, void* const userData)

	
	Function NewtonWorldSetUserData (newtonWorld:Byte Ptr, void* const userData)
	Function* NewtonWorldGetUserData (newtonWorld:Byte Ptr)
	Function NewtonWorldSetDestructorCallBack (newtonWorld:Byte Ptr, NewtonDestroyWorld destructor)
	NEWTON_API NewtonDestroyWorld NewtonWorldGetDestructorCallBack (newtonWorld:Byte Ptr)

	Function NewtonWorldRayCast (newtonWorld:Byte Ptr, const dFloat* const p0, const dFloat* const p1, NewtonWorldRayFilterCallback filter, void* const userData, 
										NewtonWorldRayPrefilterCallback prefilter)
	Function NewtonWorldConvexCast (newtonWorld:Byte Ptr, matrix:Float Ptr, const dFloat* const target, const NewtonCollision* shape, dFloat* const hitParam, void* const userData,  
										  NewtonWorldRayPrefilterCallback prefilter, NewtonWorldConvexCastReturnInfo* info, int maxContactsCount, int threadIndex)
									   
	' world utility functions
	Function NewtonWorldGetBodyCount(newtonWorld:Byte Ptr)
	Function NewtonWorldGetConstraintCount(newtonWorld:Byte Ptr)

	' Function NewtonGetActiveBodiesCount()
	' Function NewtonGetActiveConstraintsCount()
	' NEWTON_API dFloat NewtonGetGlobalScale (newtonWorld:Byte Ptr)



	' **********************************************************************************************
	'
	' Simulation islands 
	'
	' **********************************************************************************************
	NEWTON_API NewtonBody* NewtonIslandGetBody:Byte Ptr (const void* const island:Byte Ptr, bodyIndex)
	Function NewtonIslandGetBodyAABB (island:Byte Ptr, bodyIndex, p0:Float Ptr, p1:Float Ptr)

	' **********************************************************************************************
	'
	' Physics Material Section
	'
	' **********************************************************************************************
	Function NewtonMaterialCreateGroupID(newtonWorld:Byte Ptr)
	Function NewtonMaterialGetDefaultGroupID(newtonWorld:Byte Ptr)
	Function NewtonMaterialDestroyAllGroupID(newtonWorld:Byte Ptr)

	' material definitions that can not be overwritten in function callback
	Function* NewtonMaterialGetUserData (newtonWorld:Byte Ptr, int id0, int id1)
	Function NewtonMaterialSetSurfaceThickness (newtonWorld:Byte Ptr, int id0, int id1, dFloat thickness)
	Function NewtonMaterialSetContinuousCollisionMode (newtonWorld:Byte Ptr, int id0, int id1, int state)
	Function NewtonMaterialSetCollisionCallback (newtonWorld:Byte Ptr, id0, id1, void* const userData:Byte Ptr,
														NewtonOnAABBOverlap aabbOverlap, NewtonContactsProcess process)

	Function NewtonMaterialSetDefaultSoftness (newtonWorld:Byte Ptr, int id0, int id1, dFloat value)
	Function NewtonMaterialSetDefaultElasticity (newtonWorld:Byte Ptr, int id0, int id1, dFloat elasticCoef)
	Function NewtonMaterialSetDefaultCollidable (newtonWorld:Byte Ptr, int id0, int id1, int state)
	Function NewtonMaterialSetDefaultFriction (newtonWorld:Byte Ptr, int id0, int id1, dFloat staticFriction, dFloat kineticFriction)

	NEWTON_API NewtonMaterial* NewtonWorldGetFirstMaterial (newtonWorld:Byte Ptr)
	NEWTON_API NewtonMaterial* NewtonWorldGetNextMaterial (newtonWorld:Byte Ptr, const NewtonMaterial* const material)

	Function NewtonWorldGetFirstBody:Byte Ptr(newtonWorld:Byte Ptr)
	Function NewtonWorldGetNextBody:Byte Ptr(newtonWorld:Byte Ptr, curBody:Byte Ptr)


	' **********************************************************************************************
	'
	' Physics Contact control functions
	'
	' **********************************************************************************************
	Function *NewtonMaterialGetMaterialPairUserData (const NewtonMaterial* const material)
	NEWTON_API unsigned NewtonMaterialGetContactFaceAttribute (const NewtonMaterial* const material)
	NEWTON_API unsigned NewtonMaterialGetBodyCollisionID (const NewtonMaterial* const material, body:Byte Ptr)
	NEWTON_API dFloat NewtonMaterialGetContactNormalSpeed (const NewtonMaterial* const material)
	Function NewtonMaterialGetContactForce (const NewtonMaterial* const material, NewtonBody* const body, dFloat* const force)
	Function NewtonMaterialGetContactPositionAndNormal (const NewtonMaterial* const material, NewtonBody* const body, dFloat* const posit, dFloat* const normal)
	Function NewtonMaterialGetContactTangentDirections (const NewtonMaterial* const material, NewtonBody* const body, dFloat* const dir0, dFloat* const dir1)
	NEWTON_API dFloat NewtonMaterialGetContactTangentSpeed (const NewtonMaterial* const material, int index)
	
	Function NewtonMaterialSetContactSoftness (const NewtonMaterial* const material, dFloat softness)
	Function NewtonMaterialSetContactElasticity (const NewtonMaterial* const material, dFloat restitution)
	Function NewtonMaterialSetContactFrictionState (const NewtonMaterial* const material, int state, int index)
	Function NewtonMaterialSetContactFrictionCoef (const NewtonMaterial* const material, dFloat staticFrictionCoef, dFloat kineticFrictionCoef, int index)
	
	Function NewtonMaterialSetContactNormalAcceleration (const NewtonMaterial* const material, dFloat accel)
	Function NewtonMaterialSetContactNormalDirection (const NewtonMaterial* const material, const dFloat* const directionVector)

	Function NewtonMaterialSetContactTangentAcceleration (const NewtonMaterial* const material, dFloat accel, int index)
	Function NewtonMaterialContactRotateTangentDirections (const NewtonMaterial* const material, const dFloat* const directionVector)

End Rem
	

	Function NewtonCreateNull:Byte Ptr (newtonWorld:Byte Ptr)
	Function NewtonCreateSphere:Byte Ptr (newtonWorld:Byte Ptr, radiusX#, radiusY#, radiusZ#, shapeID, offsetMatrix:Float Ptr)
	Function NewtonCreateBox:Byte Ptr(newtonWorld:Byte Ptr, dx#, dy#, dz#, shapeID, offsetMatrix:Float Ptr)
	Function NewtonCreateCone:Byte Ptr (newtonWorld:Byte Ptr, radius#, height#, shapeID, offsetMatrix:Float Ptr)
	Function NewtonCreateCapsule:Byte Ptr (newtonWorld:Byte Ptr, radius#, height#, shapeID, offsetMatrix:Float Ptr)
	Function NewtonCreateCylinder:Byte Ptr (newtonWorld:Byte Ptr, radius#, height#, shapeID, offsetMatrix:Float Ptr)
	Function NewtonCreateChamferCylinder:Byte Ptr (newtonWorld:Byte Ptr, radius#, height#, shapeID, offsetMatrix:Float Ptr)
	Function NewtonCreateConvexHul:Byte Ptr (newtonWorld:Byte Ptr, count, vertexCloud:Float Ptr, strideInBytes, tolerance#, shapeID, offsetMatrix:Float Ptr)
	Function NewtonCreateConvexHullFromMesh:Byte Ptr (newtonWorld:Byte Ptr, mesh:Byte Ptr, tolerance#, shapeID)

	Function NewtonCreateConvexHullModifier:Byte Ptr (newtonWorld:Byte Ptr, convexHullCollision:Byte Ptr, shapeID)
	Function NewtonConvexHullModifierGetMatrix (convexHullCollision:Byte Ptr, matrix:Float Ptr)
	Function NewtonConvexHullModifierSetMatrix (convexHullCollision:Byte Ptr, matrix:Float Ptr)
	
	Function NewtonCollisionIsTriggerVolume(convexCollision:Byte Ptr)
	Function NewtonCollisionSetAsTriggerVolume(convexCollision:Byte Ptr, trigger)
	
	Function NewtonCollisionSetMaxBreakImpactImpulse(convexHullCollision:Byte Ptr, maxImpactImpulse#)
	Function NewtonCollisionGetMaxBreakImpactImpulse#(convexHullCollision:Byte Ptr)
	
	Function NewtonCollisionSetUserID (convexCollision:Byte Ptr, id)
	Function NewtonCollisionGetUserID (convexCollision:Byte Ptr)
	
	Function NewtonConvexHullGetFaceIndices (convexHullCollision:Byte Ptr, face, faceIndices:Int Ptr)
	Function NewtonConvexCollisionCalculateVolume# (convexCollision:Byte Ptr)
	Function NewtonConvexCollisionCalculateInertialMatrix (convexCollision:Byte Ptr, inertia:Float Ptr, origin:Float Ptr)	
	
	
	Function NewtonCollisionMakeUnique (newtonWorld:Byte Ptr, collision:Byte Ptr)
	Function NewtonReleaseCollision (newtonWorld:Byte Ptr, collision:Byte Ptr)
	Function NewtonAddCollisionReference (collision:Byte Ptr)

Rem	
	' **********************************************************************************************
	'
	' mass/spring/damper collision shape
	'
	' **********************************************************************************************
'	NEWTON_API NewtonCollision* NewtonCreateSoftShape (newtonWorld:Byte Ptr)
'	Function NewtonSoftBodySetMassCount (const NewtonCollision* convexCollision, int count)
'	Function NewtonSoftBodySetSpringCount (const NewtonCollision* convexCollision, int count)

'	Function NewtonSoftBodySetMass (const NewtonCollision* convexCollision, int index, dFloat mass, dFloat* position)
'	Function NewtonSoftBodySetSpring (const NewtonCollision* convexCollision, int index, int mass0, int mass1, dFloat stiffness, dFloat damper)
'	Function NewtonSoftBodyGetMassArray (const NewtonCollision* convexCollision, dFloat* masses, dFloat** positions)	


	' **********************************************************************************************
	'
	' complex collision primitives creation functions
	'
	' **********************************************************************************************
	NEWTON_API NewtonCollision* NewtonCreateCompoundCollision (newtonWorld:Byte Ptr, int count, NewtonCollision* const collisionPrimitiveArray[], int shapeID)
	'NEWTON_API NewtonCollision* NewtonCreateCompoundCollisionFromMesh (newtonWorld:Byte Ptr, mesh:Byte ptr, dFloat concavity, int shapeID, int subShapeID)
	'NEWTON_API NewtonCollision* NewtonCreateCompoundCollisionFromMesh(newtonWorld:Byte Ptr, mesh:Byte ptr, int maxSubShapesCount, int shapeID, int subShapeID)

	' **********************************************************************************************
	'
	' complex breakable collision primitives interface
	'
	' **********************************************************************************************
'	NEWTON_API NewtonCollision* NewtonCreateCompoundBreakable (newtonWorld:Byte Ptr, int meshCount, 
'															   NewtonMesh* const solids[], NewtonMesh* const splitePlanes[], 
'															   dFloat* const matrixPallete, int* const shapeIDArray, dFloat* const densities,
'															   int shapeID, int debriID, NewtonCollisionCompoundBreakableCallback callback, void* buildUsedData)

	NEWTON_API NewtonCollision* NewtonCreateCompoundBreakable (newtonWorld:Byte Ptr, int meshCount, 
															   const NewtonMesh** const solids, const int* const shapeIDArray, 
															   const dFloat* const densities, const int* const internalFaceMaterial, 
															   int shapeID, int debriID, dFloat debriSeparationGap)


	Function NewtonCompoundBreakableResetAnchoredPieces (const NewtonCollision* const compoundBreakable)
	Function NewtonCompoundBreakableSetAnchoredPieces (const NewtonCollision* const compoundBreakable, int fixShapesCount, dFloat* const matrixPallete, NewtonCollision** const fixedShapesArray)

	Function NewtonCompoundBreakableGetVertexCount (const NewtonCollision* const compoundBreakable) 
	Function NewtonCompoundBreakableGetVertexStreams (const NewtonCollision* const compoundBreakable, int vertexStrideInByte, dFloat* const vertex,
															int normalStrideInByte, dFloat* const normal,	int uvStrideInByte, dFloat* const uv)

	
	NEWTON_API NewtonbreakableComponentMesh* NewtonBreakableGetMainMesh (const NewtonCollision* const compoundBreakable)
	NEWTON_API NewtonbreakableComponentMesh* NewtonBreakableGetFirstComponent (const NewtonCollision* const compoundBreakable)
	NEWTON_API NewtonbreakableComponentMesh* NewtonBreakableGetNextComponent (const NewtonbreakableComponentMesh* const component)

	Function NewtonBreakableBeginDelete (const NewtonCollision* const compoundBreakable)
	NEWTON_API NewtonBody* NewtonBreakableCreateDebrieBody (const NewtonCollision* const compoundBreakable, const NewtonbreakableComponentMesh* const component)
	Function NewtonBreakableDeleteComponent (const NewtonCollision* const compoundBreakable, const NewtonbreakableComponentMesh* const component)
	Function NewtonBreakableEndDelete (const NewtonCollision* const compoundBreakable)


	Function NewtonBreakableGetComponentsInRadius (const NewtonCollision* const compoundBreakable, const dFloat* position, dFloat radius, NewtonbreakableComponentMesh** const segments, int maxCount)

	Function* NewtonBreakableGetFirstSegment (const NewtonbreakableComponentMesh* const breakableComponent) 
	Function* NewtonBreakableGetNextSegment (const void* const segment) 

	Function NewtonBreakableSegmentGetMaterial (const void* const segment) 
	Function NewtonBreakableSegmentGetIndexCount (const void* const segment) 
	Function NewtonBreakableSegmentGetIndexStream (const NewtonCollision* const compoundBreakable, const NewtonbreakableComponentMesh* const meshOwner, const void* const segment, int* const index) 
	Function NewtonBreakableSegmentGetIndexStreamShort (const NewtonCollision* const compoundBreakable, const NewtonbreakableComponentMesh* const meshOwner, const void* const segment, short int* const index) 


	NEWTON_API NewtonCollision* NewtonCreateUserMeshCollision (newtonWorld:Byte Ptr, const dFloat* const minBox, 
		const dFloat* const maxBox, void* const userData, NewtonUserMeshCollisionCollideCallback collideCallback, 
		NewtonUserMeshCollisionRayHitCallback rayHitCallback, NewtonUserMeshCollisionDestroyCallback destroyCallback,
		NewtonUserMeshCollisionGetCollisionInfo getInfoCallback, NewtonUserMeshCollisionGetFacesInAABB facesInAABBCallback, int shapeID)


	NEWTON_API NewtonCollision* NewtonCreateSceneCollision (newtonWorld:Byte Ptr, int shapeID)
	NEWTON_API NewtonSceneProxy* NewtonSceneCollisionCreateProxy (NewtonCollision* const scene, NewtonCollision* const collision, matrix:Float Ptr)

	Function NewtonSceneCollisionDestroyProxy (NewtonCollision* const scene, NewtonSceneProxy* Proxy)
	Function NewtonSceneProxySetMatrix (NewtonSceneProxy* const proxy, const dFloat* matrix)
	Function NewtonSceneProxyGetMatrix (NewtonSceneProxy* const proxy, dFloat* matrix)
	Function NewtonSceneSetProxyUserData (NewtonSceneProxy* const proxy, void* userData)
	Function* NewtonSceneGetProxyUserData (NewtonSceneProxy* const proxy)

	NEWTON_API NewtonSceneProxy* NewtonSceneGetFirstProxy (NewtonCollision* const scene)
	NEWTON_API NewtonSceneProxy* NewtonSceneGetNextProxy (NewtonCollision* const scene, NewtonSceneProxy* const proxy)
	
	Function NewtonSceneCollisionOptimize (NewtonCollision* scene)

	' ***********************************************************************************************************
	'
	'	Collision serialization functions
	'
	' ***********************************************************************************************************
	NEWTON_API NewtonCollision* NewtonCreateCollisionFromSerialization (newtonWorld:Byte Ptr, NewtonDeserialize deserializeFunction, void* const serializeHandle)
	Function NewtonCollisionSerialize (newtonWorld:Byte Ptr, collision:Byte Ptr, NewtonSerialize serializeFunction, void* const serializeHandle)
	Function NewtonCollisionGetInfo (collision:Byte Ptr, NewtonCollisionInfoRecord* const collisionInfo)

	' **********************************************************************************************
	'
	' Static collision shapes functions
	'
	' **********************************************************************************************
	NEWTON_API NewtonCollision* NewtonCreateHeightFieldCollision (newtonWorld:Byte Ptr, int width, int height, int gridsDiagonals,
																  const unsigned short* const elevationMap, const char* const attributeMap,
																  dFloat horizontalScale, dFloat verticalScale, int shapeID)
	Function NewtonHeightFieldSetUserRayCastCallback (const NewtonCollision* const treeCollision, NewtonHeightFieldRayCastCallback rayHitCallback)

	
	NEWTON_API NewtonCollision* NewtonCreateTreeCollision (newtonWorld:Byte Ptr, int shapeID)
	Function NewtonTreeCollisionSetUserRayCastCallback (const NewtonCollision* const treeCollision, NewtonCollisionTreeRayCastCallback rayHitCallback)

	Function NewtonTreeCollisionBeginBuild (const NewtonCollision* treeCollision)
	Function NewtonTreeCollisionAddFace (const NewtonCollision* const treeCollision, int vertexCount, const dFloat* const vertexPtr, int strideInBytes, int faceAttribute)
	Function NewtonTreeCollisionEndBuild (const NewtonCollision* const treeCollision, int optimize)

	Function NewtonTreeCollisionGetFaceAtribute (const NewtonCollision* const treeCollision, const int* const faceIndexArray) 
	Function NewtonTreeCollisionSetFaceAtribute (const NewtonCollision* const treeCollision, const int* const faceIndexArray, int attribute) 
	Function NewtonTreeCollisionGetVertexListIndexListInAABB (const NewtonCollision* const treeCollision, const dFloat* const p0, const dFloat* const p1, const dFloat** const vertexArray, int* const vertexCount, int* const vertexStrideInBytes, const int* const indexList, int maxIndexCount, const int* const faceAttribute) 


	Function NewtonStaticCollisionSetDebugCallback (const NewtonCollision* const staticCollision, NewtonTreeCollisionCallback userCallback)

	' **********************************************************************************************
	'
	' General purpose collision library functions
	'
	' **********************************************************************************************
	Function NewtonCollisionPointDistance (newtonWorld:Byte Ptr, const dFloat* const point,
		collision:Byte Ptr, const matrix:Float Ptr, dFloat* const contact, dFloat* const normal, int threadIndex)

	Function NewtonCollisionClosestPoint (newtonWorld:Byte Ptr, 
		collision:Byte PtrA, const matrix:Float PtrA, collision:Byte PtrB, const matrix:Float PtrB,
		dFloat* const contactA, dFloat* const contactB, dFloat* const normalAB, int threadIndex)
	
	Function NewtonCollisionCollide (newtonWorld:Byte Ptr, int maxSize,
		collision:Byte PtrA, const matrix:Float PtrA, 
		collision:Byte PtrB, const matrix:Float PtrB,
		dFloat* const contacts, dFloat* const normals, dFloat* const penetration, int threadIndex)

	Function NewtonCollisionCollideContinue (newtonWorld:Byte Ptr, int maxSize, const dFloat timestep, 
		collision:Byte PtrA, const matrix:Float PtrA, const dFloat* const velocA, const dFloat* omegaA, 
		collision:Byte PtrB, const matrix:Float PtrB, const dFloat* const velocB, const dFloat* const omegaB, 
		dFloat* const timeOfImpact, dFloat* const contacts, dFloat* const normals, dFloat* const penetration, int threadIndex)

	Function NewtonCollisionSupportVertex (collision:Byte Ptr, const dFloat* const dir, dFloat* const vertex)
	NEWTON_API dFloat NewtonCollisionRayCast (collision:Byte Ptr, const dFloat* const p0, const dFloat* const p1, dFloat* const normal, int* const attribute)
	Function NewtonCollisionCalculateAABB (collision:Byte Ptr, matrix:Float Ptr, dFloat* const p0, dFloat* const p1)
	Function NewtonCollisionForEachPolygonDo (collision:Byte Ptr, matrix:Float Ptr, NewtonCollisionIterator callback, void* const userData)
	
	
	' **********************************************************************************************
	'
	' transforms utility functions
	'
	' **********************************************************************************************
	Function NewtonGetEulerAngle (matrix:Float Ptr, dFloat* const eulersAngles)
	Function NewtonSetEulerAngle (const dFloat* const eulersAngles, dFloat* const matrix)
	NEWTON_API dFloat NewtonCalculateSpringDamperAcceleration (dFloat dt, dFloat ks, dFloat x, dFloat kd, dFloat s)
End Rem

	' **********************************************************************************************
	'
	' body manipulation functions
	'
	' **********************************************************************************************
	Function NewtonCreateBody:Byte Ptr (newtonWorld:Byte Ptr, collision:Byte Ptr, matrix:Float Ptr)
	Function NewtonDestroyBody(newtonWorld:Byte Ptr, body:Byte Ptr)

	Function NewtonBodyAddForce (body:Byte Ptr, force:Float Ptr)
	Function NewtonBodyAddTorque (body:Byte Ptr, torque:Float Ptr)
	Function NewtonBodyCalculateInverseDynamicsForce (body:Byte Ptr, timestep#, desiredVeloc:Float Ptr, forceOut:Float Ptr)

	Function NewtonBodySetMatrix (body:Byte Ptr, matrix:Float Ptr)
	Function NewtonBodySetMatrixRecursive (body:Byte Ptr, matrix:Float Ptr)
	Function NewtonBodySetMassMatrix (body:Byte Ptr, mass#, Ixx#, Iyy#, Izz#)
	Function NewtonBodySetMaterialGroupID (body:Byte Ptr, id)
	Function NewtonBodySetContinuousCollisionMode (body:Byte Ptr, state)
	Function NewtonBodySetJointRecursiveCollision (body:Byte Ptr, state)
	Function NewtonBodySetOmega (body:Byte Ptr, omega:Float Ptr) 
	Function NewtonBodySetVelocity (body:Byte Ptr, velocity:Float Ptr)
	Function NewtonBodySetForce (body:Byte Ptr, force:Float Ptr)
	Function NewtonBodySetTorque (body:Byte Ptr, torque:Float Ptr)
	
	Function NewtonBodySetCentreOfMass (body:Byte Ptr, com:Float Ptr)
	Function NewtonBodySetLinearDamping (body:Byte Ptr, linearDamp#)
	Function NewtonBodySetAngularDamping (body:Byte Ptr, angularDamp:Float Ptr)
	Function NewtonBodySetUserData (body:Byte Ptr, userData:Byte Ptr)
	Function NewtonBodySetCollision (body:Byte Ptr, collision:Byte Ptr)

	Function NewtonBodyGetSleepState (body:Byte Ptr)
	Function NewtonBodyGetAutoSleep (body:Byte Ptr)
	Function NewtonBodySetAutoSleep (body:Byte Ptr, state)
	
	Function NewtonBodyGetFreezeState(body:Byte Ptr)
	Function NewtonBodySetFreezeState (body:Byte Ptr, state)


'	Function NewtonBodySetAutoFreeze(body:Byte Ptr, state)
'	Function NewtonBodyCoriolisForcesMode (body:Byte Ptr, mode)
'	Function NewtonBodySetGyroscopicForcesMode (body:Byte Ptr, mode)
'	Function NewtonBodyGetGyroscopicForcesMode (body:Byte Ptr)
'	Function NewtonBodyGetFreezeState (body:Byte Ptr)
'	Function NewtonBodySetFreezeState  (body:Byte Ptr, state)
'	Function NewtonBodyGetFreezeTreshold (body:Byte Ptr, freezeSpeed2:Float Ptr, freezeOmega2:Float Ptr)
'	Function NewtonBodySetFreezeTreshold (body:Byte Ptr, freezeSpeed2#, freezeOmega2#, framesCount)
'	Function NewtonBodySetAutoactiveCallback (body:Byte Ptr, NewtonBodyActivationState callback)
	

	Function NewtonBodySetDestructorCallback (body:Byte Ptr, callback:Byte Ptr)

	Function NewtonBodySetTransformCallback (body:Byte Ptr, callback:Byte Ptr)
	Function NewtonBodyGetTransformCallback:Byte Ptr (body:Byte Ptr)

	Function NewtonBodySetForceAndTorqueCallback (body:Byte Ptr, callback:Byte Ptr)
	Function NewtonBodyGetForceAndTorqueCallback:Byte Ptr (body:Byte Ptr)
	Function NewtonBodyGetUserData:Byte Ptr (body:Byte Ptr)
	Function NewtonBodyGetWorld:Byte Ptr(body:Byte Ptr)
	Function NewtonBodyGetCollision:Byte Ptr(body:Byte Ptr)
	Function NewtonBodyGetMaterialGroupID (body:Byte Ptr)

	Function NewtonBodyGetContinuousCollisionMode (body:Byte Ptr)
	Function NewtonBodyGetJointRecursiveCollision (body:Byte Ptr)

	Function NewtonBodyGetMatrix(body:Byte Ptr, matrix:Float Ptr)
	Function NewtonBodyGetRotation(body:Byte Ptr, rotation:Float Ptr)
	Function NewtonBodyGetMassMatrix (body:Byte Ptr, mass:Float Ptr, Ixx:Float Ptr, Iyy:Float Ptr, Izz:Float Ptr)
	Function NewtonBodyGetInvMass(body:Byte Ptr, invMass:Float Ptr, invIxx:Float Ptr, invIyy:Float Ptr, invIzz:Float Ptr)
	Function NewtonBodyGetOmega(body:Byte Ptr, vector:Float Ptr)
	Function NewtonBodyGetVelocity(body:Byte Ptr, vector:Float Ptr)
	Function NewtonBodyGetForce(body:Byte Ptr, vector:Float Ptr)
	Function NewtonBodyGetTorque(body:Byte Ptr, vector:Float Ptr)
	Function NewtonBodyGetForceAcc(body:Byte Ptr, vector:Float Ptr)
	Function NewtonBodyGetTorqueAcc(body:Byte Ptr, vector:Float Ptr)
	Function NewtonBodyGetCentreOfMass (body:Byte Ptr, com:Float Ptr)


	Function NewtonBodyGetLinearDamping# (body:Byte Ptr)
	Function NewtonBodyGetAngularDamping (body:Byte Ptr, vector:Float Ptr)
	Function NewtonBodyGetAABB (body:Byte Ptr, p0:Float Ptr, p1:Float Ptr)
	Function NewtonBodyGetFirstJoint:Byte Ptr (body:Byte Ptr)
	Function NewtonBodyGetNextJoint:Byte Ptr  (body:Byte Ptr, joint:Byte Ptr)
	Function NewtonBodyGetFirstContactJoint:Byte Ptr  (body:Byte Ptr)
	Function NewtonBodyGetNextContactJoint:Byte Ptr  (body:Byte Ptr, contactJoint:Byte Ptr)

	Function NewtonContactJointGetFirstContact:Byte Ptr (contactJoint:Byte Ptr)
	Function NewtonContactJointGetNextContact:Byte Ptr (contactJoint:Byte Ptr, contact:Byte Ptr)

	Function NewtonContactJointGetContactCount(contactJoint:Byte Ptr)
	Function NewtonContactJointRemoveContact(contactJoint:Byte Ptr, contact:Byte Ptr) 

	Function NewtonContactGetMaterial:Byte Ptr (contact:Byte Ptr)

	
	Function NewtonBodyAddBuoyancyForce (body:Byte Ptr, fluidDensity#, fluidLinearViscosity#, fluidAngularViscosity#, gravityVector:Float Ptr, buoyancyPlane:Byte Ptr, context:Byte Ptr)

'	Function NewtonBodyForEachPolygonDo (body:Byte Ptr, NewtonCollisionIterator callback)
	Function NewtonBodyAddImpulse (body:Byte Ptr, pointDeltaVeloc:Float Ptr, pointPosit:Float Ptr)


Rem
	' **********************************************************************************************
	'
	' Common joint functions
	'
	' **********************************************************************************************
	Function* NewtonJointGetUserData (joint:Byte Ptr)
	Function NewtonJointSetUserData (joint:Byte Ptr, void* const userData)

	NEWTON_API NewtonBody* NewtonJointGetBody0 (joint:Byte Ptr)
	NEWTON_API NewtonBody* NewtonJointGetBody1 (joint:Byte Ptr)

	Function NewtonJointGetInfo  (joint:Byte Ptr, NewtonJointRecord* const info)
	Function NewtonJointGetCollisionState (joint:Byte Ptr)
	Function NewtonJointSetCollisionState (joint:Byte Ptr, int state)

	NEWTON_API dFloat NewtonJointGetStiffness (joint:Byte Ptr)
	Function NewtonJointSetStiffness (joint:Byte Ptr, dFloat state)

	Function NewtonDestroyJoint(newtonWorld:Byte Ptr, joint:Byte Ptr)
	Function NewtonJointSetDestructor (joint:Byte Ptr, NewtonConstraintDestructor destructor)

	' **********************************************************************************************
	'
	' Ball and Socket joint functions
	'
	' **********************************************************************************************
	NEWTON_API NewtonJoint* NewtonConstraintCreateBall (newtonWorld:Byte Ptr, const dFloat* pivotPoint, 
														const NewtonBody* const childBody, const NewtonBody* const parentBody)
	Function NewtonBallSetUserCallback (ball:Byte Ptr, NewtonBallCallBack callback)
	Function NewtonBallGetJointAngle (ball:Byte Ptr, dFloat* angle)
	Function NewtonBallGetJointOmega (ball:Byte Ptr, dFloat* omega)
	Function NewtonBallGetJointForce (ball:Byte Ptr, dFloat* const force)
	Function NewtonBallSetConeLimits (ball:Byte Ptr, const dFloat* pin, dFloat maxConeAngle, dFloat maxTwistAngle)

	' **********************************************************************************************
	'
	' Hinge joint functions
	'
	' **********************************************************************************************
	NEWTON_API NewtonJoint* NewtonConstraintCreateHinge (newtonWorld:Byte Ptr, 
		const dFloat* pivotPoint, const dFloat* pinDir, 
		const NewtonBody* const childBody, const NewtonBody* const parentBody)

	Function NewtonHingeSetUserCallback (hinge:Byte Ptr, NewtonHingeCallBack callback)
	NEWTON_API dFloat NewtonHingeGetJointAngle (hinge:Byte Ptr)
	NEWTON_API dFloat NewtonHingeGetJointOmega (hinge:Byte Ptr)
	Function NewtonHingeGetJointForce (hinge:Byte Ptr, dFloat* const force)
	NEWTON_API dFloat NewtonHingeCalculateStopAlpha (hinge:Byte Ptr, const NewtonHingeSliderUpdateDesc* const desc, dFloat angle)

	' **********************************************************************************************
	'
	' Slider joint functions
	'
	' **********************************************************************************************
	NEWTON_API NewtonJoint* NewtonConstraintCreateSlider (newtonWorld:Byte Ptr, 
		const dFloat* pivotPoint, const dFloat* pinDir, 
		const NewtonBody* const childBody, const NewtonBody* const parentBody)
	Function NewtonSliderSetUserCallback (const NewtonJoint* const slider, NewtonSliderCallBack callback)
	NEWTON_API dFloat NewtonSliderGetJointPosit (slider:Byte Ptr)
	NEWTON_API dFloat NewtonSliderGetJointVeloc (slider:Byte Ptr)
	Function NewtonSliderGetJointForce (const NewtonJoint* const slider, dFloat* const force)
	NEWTON_API dFloat NewtonSliderCalculateStopAccel (const NewtonJoint* const slider, const NewtonHingeSliderUpdateDesc* const desc, dFloat position)


	' **********************************************************************************************
	'
	' Corkscrew joint functions
	'
	' **********************************************************************************************
	NEWTON_API NewtonJoint* NewtonConstraintCreateCorkscrew (newtonWorld:Byte Ptr, 
		const dFloat* pivotPoint, const dFloat* pinDir, 
		const NewtonBody* const childBody, const NewtonBody* const parentBody)
	Function NewtonCorkscrewSetUserCallback (corkscrew:Byte Ptr, NewtonCorkscrewCallBack callback)
	NEWTON_API dFloat NewtonCorkscrewGetJointPosit (corkscrew:Byte Ptr)
	NEWTON_API dFloat NewtonCorkscrewGetJointAngle (corkscrew:Byte Ptr)
	NEWTON_API dFloat NewtonCorkscrewGetJointVeloc (corkscrew:Byte Ptr)
	NEWTON_API dFloat NewtonCorkscrewGetJointOmega (corkscrew:Byte Ptr)
	Function NewtonCorkscrewGetJointForce (corkscrew:Byte Ptr, dFloat* const force)
	NEWTON_API dFloat NewtonCorkscrewCalculateStopAlpha (corkscrew:Byte Ptr, const NewtonHingeSliderUpdateDesc* const desc, dFloat angle)
	NEWTON_API dFloat NewtonCorkscrewCalculateStopAccel (corkscrew:Byte Ptr, const NewtonHingeSliderUpdateDesc* const desc, dFloat position)


	' **********************************************************************************************
	'
	' Universal joint functions
	'
	' **********************************************************************************************
	NEWTON_API NewtonJoint* NewtonConstraintCreateUniversal (newtonWorld:Byte Ptr, 
		const dFloat* pivotPoint, const dFloat* pinDir0, const dFloat* pinDir1, 
		const NewtonBody* const childBody, const NewtonBody* const parentBody)
	Function NewtonUniversalSetUserCallback (universal:Byte Ptr, NewtonUniversalCallBack callback)
	NEWTON_API dFloat NewtonUniversalGetJointAngle0 (universal:Byte Ptr)
	NEWTON_API dFloat NewtonUniversalGetJointAngle1 (universal:Byte Ptr)
	NEWTON_API dFloat NewtonUniversalGetJointOmega0 (universal:Byte Ptr)
	NEWTON_API dFloat NewtonUniversalGetJointOmega1 (universal:Byte Ptr)
	Function NewtonUniversalGetJointForce (universal:Byte Ptr, dFloat* const force)
	NEWTON_API dFloat NewtonUniversalCalculateStopAlpha0 (universal:Byte Ptr, const NewtonHingeSliderUpdateDesc* const desc, dFloat angle)
	NEWTON_API dFloat NewtonUniversalCalculateStopAlpha1 (universal:Byte Ptr, const NewtonHingeSliderUpdateDesc* const desc, dFloat angle)


	' **********************************************************************************************
	'
	' Up vector joint functions
	'
	' **********************************************************************************************
	NEWTON_API NewtonJoint* NewtonConstraintCreateUpVector (newtonWorld:Byte Ptr, const dFloat* pinDir, body:Byte Ptr) 
	Function NewtonUpVectorGetPin (const NewtonJoint* const upVector, dFloat *pin)
	Function NewtonUpVectorSetPin (const NewtonJoint* const upVector, const dFloat *pin)


	' **********************************************************************************************
	'
	' User defined bilateral Joint
	'
	' **********************************************************************************************
	NEWTON_API NewtonJoint* NewtonConstraintCreateUserJoint (newtonWorld:Byte Ptr, int maxDOF, 
															 NewtonUserBilateralCallBack callback,
															 NewtonUserBilateralGetInfoCallBack getInfo,
															 const NewtonBody* const childBody, const NewtonBody* const parentBody) ; 
	Function NewtonUserJointSetFeedbackCollectorCallback (joint:Byte Ptr, NewtonUserBilateralCallBack getFeedback)
	Function NewtonUserJointAddLinearRow (joint:Byte Ptr, const dFloat* const pivot0, const dFloat* const pivot1, const dFloat* const dir)
	Function NewtonUserJointAddAngularRow (joint:Byte Ptr, dFloat relativeAngle, const dFloat* const dir)
	Function NewtonUserJointAddGeneralRow (joint:Byte Ptr, const dFloat* const jacobian0, const dFloat* const jacobian1)
	Function NewtonUserJointSetRowMinimumFriction (joint:Byte Ptr, dFloat friction)
	Function NewtonUserJointSetRowMaximumFriction (joint:Byte Ptr, dFloat friction)
	Function NewtonUserJointSetRowAcceleration (joint:Byte Ptr, dFloat acceleration)
	Function NewtonUserJointSetRowSpringDamperAcceleration (joint:Byte Ptr, dFloat springK, dFloat springD)
	Function NewtonUserJointSetRowStiffness (joint:Byte Ptr, dFloat stiffness)
	NEWTON_API dFloat NewtonUserJointGetRowForce (joint:Byte Ptr, int row)


	' **********************************************************************************************
	'
	' Mesh joint functions
	'
	' **********************************************************************************************
	NEWTON_API NewtonMesh* NewtonMeshCreate(newtonWorld:Byte Ptr)
	NEWTON_API NewtonMesh* NewtonMeshCreateFromMesh(mesh:Byte ptr)
	NEWTON_API NewtonMesh* NewtonMeshCreateFromCollision(collision:Byte Ptr)
	NEWTON_API NewtonMesh* NewtonMeshConvexHull (newtonWorld:Byte Ptr, int count, const dFloat* const vertexCloud, int strideInBytes, dFloat tolerance)
	NEWTON_API NewtonMesh* NewtonMeshCreatePlane (newtonWorld:Byte Ptr, const dFloat* const locationMatrix, dFloat witdth, dFloat breadth, int material, const dFloat* const textureMatrix0, const dFloat* const textureMatrix1)
	
	Function NewtonMeshDestroy(mesh:Byte ptr)

	Function NewtonMeshCalculateOOBB(mesh:Byte ptr, matrix:Float Ptr, dFloat* const x, dFloat* const y, dFloat* const z)

	Function NewtonMeshCalculateVertexNormals(mesh:Byte ptr, dFloat angleInRadians)
	Function NewtonMeshApplySphericalMapping(mesh:Byte ptr, int material)
	Function NewtonMeshApplyBoxMapping(mesh:Byte ptr, int front, int side, int top)
	Function NewtonMeshApplyCylindricalMapping(mesh:Byte ptr, int cylinderMaterial, int capMaterial)
	
	Function NewtonMeshIsOpenMesh (mesh:Byte ptr)
	Function NewtonMeshFixTJoints (mesh:Byte ptr)


	Function NewtonMeshPolygonize (mesh:Byte ptr)
	Function NewtonMeshTriangulate (mesh:Byte ptr)
	NEWTON_API NewtonMesh* NewtonMeshUnion (mesh:Byte ptr, const NewtonMesh* const clipper, const dFloat* const clipperMatrix)
	NEWTON_API NewtonMesh* NewtonMeshDifference (mesh:Byte ptr, const NewtonMesh* const clipper, const dFloat* const clipperMatrix)
	NEWTON_API NewtonMesh* NewtonMeshIntersection (mesh:Byte ptr, const NewtonMesh* const clipper, const dFloat* const clipperMatrix)
	Function NewtonMeshClip (mesh:Byte ptr, const NewtonMesh* const clipper, const dFloat* const clipperMatrix, NewtonMesh** const topMesh, NewtonMesh** const bottomMesh)

	NEWTON_API NewtonMesh* NewtonMeshConvexDecomposition (mesh:Byte ptr, int maxCount)
	NEWTON_API NewtonMesh* NewtonMeshVoronoiDecomposition (mesh:Byte ptr, int pointCount, int pointStrideInBytes, const dFloat* const pointCloud, int internalMaterial, const dFloat* const textureMatrix)


	Function NewtonRemoveUnusedVertices(mesh:Byte ptr, int* const vertexRemapTable)

	Function NewtonMeshBeginFace(mesh:Byte ptr)
	Function NewtonMeshAddFace(mesh:Byte ptr, int vertexCount, const dFloat* const vertex, int strideInBytes, int materialIndex)
	Function NewtonMeshEndFace(mesh:Byte ptr)

	Function NewtonMeshBuildFromVertexListIndexList(mesh:Byte ptr,
		int faceCount, const int* const faceIndexCount, const int* const faceMaterialIndex, 
		const dFloat* const vertex, int vertexStrideInBytes, const int* const vertexIndex,
		const dFloat* const normal, int normalStrideInBytes, const int* const normalIndex,
		const dFloat* const uv0, int uv0StrideInBytes, const int* const uv0Index,
		const dFloat* const uv1, int uv1StrideInBytes, const int* const uv1Index)



	Function NewtonMeshGetVertexStreams (mesh:Byte ptr, 
												int vertexStrideInByte, dFloat* const vertex,
												int normalStrideInByte, dFloat* const normal,
												int uvStrideInByte0, dFloat* const uv0,
												int uvStrideInByte1, dFloat* const uv1)
	Function NewtonMeshGetIndirectVertexStreams(mesh:Byte ptr, 
													  int vertexStrideInByte, dFloat* const vertex, int* const vertexIndices, int* const vertexCount,
													  int normalStrideInByte, dFloat* const normal, int* const normalIndices, int* const normalCount,
													  int uvStrideInByte0, dFloat* const uv0, int* const uvIndices0, int* const uvCount0,
													  int uvStrideInByte1, dFloat* const uv1, int* const uvIndices1, int* const uvCount1)
	Function* NewtonMeshBeginHandle (mesh:Byte ptr) 
	Function NewtonMeshEndHandle (mesh:Byte ptr, void* const handle) 
	Function NewtonMeshFirstMaterial (mesh:Byte ptr, void* const handle) 
	Function NewtonMeshNextMaterial (mesh:Byte ptr, void* const handle, int materialId) 
	Function NewtonMeshMaterialGetMaterial (mesh:Byte ptr, void* const handle, int materialId) 
	Function NewtonMeshMaterialGetIndexCount (mesh:Byte ptr, void* const handle, int materialId) 
	Function NewtonMeshMaterialGetIndexStream (mesh:Byte ptr, void* const handle, int materialId, int* const index) 
	Function NewtonMeshMaterialGetIndexStreamShort (mesh:Byte ptr, void* const handle, int materialId, short int* const index) 

	NEWTON_API NewtonMesh* NewtonMeshCreateFirstSingleSegment (mesh:Byte ptr) 
	NEWTON_API NewtonMesh* NewtonMeshCreateNextSingleSegment (mesh:Byte ptr, const NewtonMesh* const segment) 

	NEWTON_API NewtonMesh* NewtonMeshCreateFirstLayer (mesh:Byte ptr) 
	NEWTON_API NewtonMesh* NewtonMeshCreateNextLayer (mesh:Byte ptr, const NewtonMesh* const segment) 


	Function NewtonMeshGetTotalFaceCount (mesh:Byte ptr) 
	Function NewtonMeshGetTotalIndexCount (mesh:Byte ptr) 
	Function NewtonMeshGetFaces (mesh:Byte ptr, int* const faceIndexCount, int* const faceMaterial, void** const faceIndices) 


	Function NewtonMeshGetPointCount (mesh:Byte ptr) 
	Function NewtonMeshGetPointStrideInByte (mesh:Byte ptr) 
	NEWTON_API dFloat64* NewtonMeshGetPointArray (mesh:Byte ptr) 
	NEWTON_API dFloat64* NewtonMeshGetNormalArray (mesh:Byte ptr) 
	NEWTON_API dFloat64* NewtonMeshGetUV0Array (mesh:Byte ptr) 
	NEWTON_API dFloat64* NewtonMeshGetUV1Array (mesh:Byte ptr) 

	Function NewtonMeshGetVertexCount (mesh:Byte ptr) 
	Function NewtonMeshGetVertexStrideInByte (mesh:Byte ptr) 
	NEWTON_API dFloat64* NewtonMeshGetVertexArray (mesh:Byte ptr) 


	Function* NewtonMeshGetFirstVertex (mesh:Byte ptr)
	Function* NewtonMeshGetNextVertex (mesh:Byte ptr, vertex:Byte Ptr)
	Function NewtonMeshGetVertexIndex (mesh:Byte ptr, vertex:Byte Ptr)

	Function* NewtonMeshGetFirstPoint (mesh:Byte ptr)
	Function* NewtonMeshGetNextPoint (mesh:Byte ptr, point:Byte Ptr)
	Function NewtonMeshGetPointIndex (mesh:Byte ptr, point:Byte Ptr)
	Function NewtonMeshGetVertexIndexFromPoint (mesh:Byte ptr, point:Byte Ptr)
	

	Function* NewtonMeshGetFirstEdge (mesh:Byte ptr)
	Function* NewtonMeshGetNextEdge (mesh:Byte ptr, edge:Byte Ptr)
	Function NewtonMeshGetEdgeIndices (mesh:Byte ptr, edge:Byte Ptr, int* const v0, int* const v1)
	'Function NewtonMeshGetEdgePointIndices (mesh:Byte ptr, edge:Byte Ptr, int* const v0, int* const v1)

	Function* NewtonMeshGetFirstFace (mesh:Byte ptr)
	Function* NewtonMeshGetNextFace (mesh:Byte ptr, face:Byte Ptr)
	Function NewtonMeshIsFaceOpen (mesh:Byte ptr, face:Byte Ptr)
	Function NewtonMeshGetFaceMaterial (mesh:Byte ptr, face:Byte Ptr)
	Function NewtonMeshGetFaceIndexCount (mesh:Byte ptr, face:Byte Ptr)
	Function NewtonMeshGetFaceIndices (mesh:Byte ptr, face:Byte Ptr, int* const indices)
	Function NewtonMeshGetFacePointIndices (mesh:Byte ptr, face:Byte Ptr, int* const indices)
End Rem
End Extern
