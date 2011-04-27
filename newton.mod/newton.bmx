
Strict

Rem
	bbdoc: Netwon Module for BlitzMax
End Rem
Module sys87.Newton
ModuleInfo "Author: Kevin Primm"
ModuleInfo "Version: 0.01"
ModuleInfo "Newton Version: 2.33"
?Linux
ModuleInfo "CC_OPTS: -Wall -Wno-strict-aliasing -D_LINUX_VER -O2 -g -fpic -msse -msse2 -ffloat-store -ffast-math -freciprocal-math -funsafe-math-optimizations -fsingle-precision-constant"
?

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

Rem

#define NEWTON_MAJOR_VERSION 2 
#define NEWTON_MINOR_VERSION 31 

	#define NEWTON_PROFILER_WORLD_UPDATE					0

	#define NEWTON_PROFILER_COLLISION_UPDATE				1
	#define NEWTON_PROFILER_FORCE_CALLBACK_UPDATE			2
	#define NEWTON_PROFILER_COLLISION_UPDATE_BROAD_PHASE	3
	#define NEWTON_PROFILER_COLLISION_UPDATE_NARROW_PHASE	4

	#define NEWTON_PROFILER_DYNAMICS_UPDATE					5
	#define NEWTON_PROFILER_DYNAMICS_CONSTRAINT_GRAPH		6
	#define NEWTON_PROFILER_DYNAMICS_SOLVE_CONSTRAINT_GRAPH	7

	typedef struct NewtonMesh{} NewtonMesh;
	typedef struct NewtonBody{} NewtonBody;
	typedef struct NewtonWorld{} NewtonWorld;
	typedef struct NewtonJoint{} NewtonJoint;
	typedef struct NewtonMaterial{} NewtonMaterial;
	typedef struct NewtonCollision{} NewtonCollision;
	typedef struct NewtonSceneProxy{} NewtonSceneProxy;
	typedef struct NewtonbreakableComponentMesh{} NewtonbreakableComponentMesh;

//	typedef struct NewtonRagDoll{} NewtonRagDoll;
//	typedef struct NewtonRagDollBone{} NewtonRagDollBone;

	#define SERIALIZE_ID_BOX					0	
	#define SERIALIZE_ID_CONE					1
	#define SERIALIZE_ID_SPHERE					2
	#define SERIALIZE_ID_CAPSULE				3
	#define SERIALIZE_ID_CYLINDER				4
	#define SERIALIZE_ID_COMPOUND				5
	#define SERIALIZE_ID_CONVEXHULL				6
	#define SERIALIZE_ID_CONVEXMODIFIER			7
	#define SERIALIZE_ID_CHAMFERCYLINDER		8
	#define SERIALIZE_ID_TREE					9
	#define SERIALIZE_ID_NULL					10
	#define SERIALIZE_ID_HEIGHTFIELD			11
	#define SERIALIZE_ID_USERMESH				12
	#define SERIALIZE_ID_SCENE					13
	#define SERIALIZE_ID_COMPOUND_BREAKABLE		14

	struct NewtonCollisionInfoRecord
	{
		dFloat m_offsetMatrix[4][4];
		int m_collisionType;				// tag id to identify the collision primitive
		int m_referenceCount;				// the current reference count for this collision		
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
			dFloat m_paramArray[64];		    // user define collision can use this to store information
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
		dFloat m_boxP0[4];						// lower bounding box of intersection query in local space
		dFloat m_boxP1[4];						// upper bounding box of intersection query in local space
		int m_threadNumber;						// current thread executing this query
		int	m_faceCount;                        // the application should set here how many polygons intersect the query box
		int m_vertexStrideInBytes;              // the application should set here the size of each vertex
		void* m_userData;                       // user data passed to the collision geometry at creation time
		dFloat* m_vertex;                       // the application should the pointer to the vertex array. 
	 	int* m_userAttribute;                   // the application should set here the pointer to the user data, one for each face
		int* m_faceIndexCount;                  // the application should set here the pointer to the vertex count of each face.
		int* m_faceVertexIndex;                 // the application should set here the pointer index array for each vertex on a face.
		NewtonBody* m_objBody;                  // pointer to the colliding body
		NewtonBody* m_polySoupBody;             // pointer to the rigid body owner of this collision tree 
	};

	struct NewtonWorldConvexCastReturnInfo
	{
		dFloat m_point[4];						// collision point in global space
		dFloat m_normal[4];						// surface normal at collision point in global space
		dFloat m_normalOnHitPoint[4];           // surface normal at the surface of the hit body, 
												// is the same as the normal calculated by a ray cast hitting the body at the hit point
		dFloat m_penetration;                   // contact penetration at collision point
		int    m_contactID;	                    // collision ID at contact point
		const  NewtonBody* m_hitBody;			// body hit at contact point
	};
	
	struct NewtonUserMeshCollisionRayHitDesc
	{
		dFloat m_p0[4];							// ray origin in collision local space
		dFloat m_p1[4];                         // ray destination in collision local space   
		dFloat m_normalOut[4];					// copy here the normal at the ray intersection
		int m_userIdOut;                        // copy here a user defined id for further feedback  
		void* m_userData;                       // user data passed to the collision geometry at creation time
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
	// Newton callback functions
	typedef void* (*NewtonAllocMemory) (int sizeInBytes);
	typedef void (*NewtonFreeMemory) (void* const ptr, int sizeInBytes);
	typedef void (*NewtonDestroyWorld) (newtonWorld:Byte Ptr);

	typedef unsigned (*NewtonGetTicksCountCallback) ();

	typedef void (*NewtonSerialize) (void* const serializeHandle, const void* buffer, int size);
	typedef void (*NewtonDeserialize) (void* const serializeHandle, void* buffer, int size);
	
	// user collision callbacks	
	typedef void (*NewtonUserMeshCollisionDestroyCallback) (void* const userData);
	typedef void (*NewtonUserMeshCollisionCollideCallback) (NewtonUserMeshCollisionCollideDesc* const collideDescData);
	typedef dFloat (*NewtonUserMeshCollisionRayHitCallback) (NewtonUserMeshCollisionRayHitDesc* const lineDescData);
	typedef void (*NewtonUserMeshCollisionGetCollisionInfo) (void* const userData, NewtonCollisionInfoRecord* const infoRecord);
	typedef int (*NewtonUserMeshCollisionGetFacesInAABB) (void* const userData, const dFloat* const p0, const dFloat* const p1,
														   const dFloat** const vertexArray, int* const vertexCount, int* const vertexStrideInBytes, 
		                                                   const int* const indexList, int maxIndexCount, const int* const userDataList);

	typedef dFloat (*NewtonCollisionTreeRayCastCallback) (const NewtonBody* const body, const NewtonCollision* const treeCollision, dFloat interception, dFloat* normal, int faceId, void* usedData);
	typedef dFloat (*NewtonHeightFieldRayCastCallback) (const NewtonBody* const body, const NewtonCollision* const heightFieldCollision, dFloat interception, int row, int col, dFloat* normal, int faceId, void* usedData);

	
	// collision tree call back (obsoleted no recommended)
	typedef void (*NewtonTreeCollisionCallback) (const NewtonBody* const bodyWithTreeCollision, const NewtonBody* const body, int faceID, 
												 int vertexCount, const dFloat* const vertex, int vertexStrideInBytes); 

	typedef void (*NewtonBodyDestructor) (const NewtonBody* const body);
	typedef void (*NewtonApplyForceAndTorque) (const NewtonBody* const body, dFloat timestep, int threadIndex);
	typedef void (*NewtonSetTransform) (const NewtonBody* const body, const dFloat* const matrix, int threadIndex);

	typedef int (*NewtonIslandUpdate) (newtonWorld:Byte Ptr, const void* islandHandle, int bodyCount);
	typedef void (*NewtonBodyLeaveWorld) (const NewtonBody* const body, int threadIndex);
	typedef void (*NewtonDestroyBodyByExeciveForce) (const NewtonBody* const body, const NewtonJoint* const contact);
	typedef void (*NewtonCollisionDestructor) (newtonWorld:Byte Ptr, const NewtonCollision* const collision);

	typedef int (*NewtonCollisionCompoundBreakableCallback) (NewtonMesh* const mesh, void* const userData, dFloat* const planeMatrixOut);

	typedef int (*NewtonGetBuoyancyPlane) (const int collisionID, void* const context, const dFloat* const globalSpaceMatrix, dFloat* const globalSpacePlane);
	typedef unsigned (*NewtonWorldRayPrefilterCallback)(const NewtonBody* const body, const NewtonCollision* const collision, void* const userData);
	typedef dFloat (*NewtonWorldRayFilterCallback)(const NewtonBody* const body, const dFloat* const hitNormal, int collisionID, void* const userData, dFloat intersectParam);
	

	typedef int  (*NewtonOnAABBOverlap) (const NewtonMaterial* const material, const NewtonBody* const body0, const NewtonBody* const body1, int threadIndex);
	typedef void (*NewtonContactsProcess) (const NewtonJoint* const contact, dFloat timestep, int threadIndex);

	typedef void (*NewtonBodyIterator) (const NewtonBody* const body, void* const userData);
	typedef void (*NewtonJointIterator) (const NewtonJoint* const joint, void* const userData);
	typedef void (*NewtonCollisionIterator) (void* const userData, int vertexCount, const dFloat* const faceArray, int faceId);

	typedef void (*NewtonBallCallBack) (const NewtonJoint* const ball, dFloat timestep);
	typedef unsigned (*NewtonHingeCallBack) (const NewtonJoint* const hinge, NewtonHingeSliderUpdateDesc* const desc);
	typedef unsigned (*NewtonSliderCallBack) (const NewtonJoint* const slider, NewtonHingeSliderUpdateDesc* const desc);
	typedef unsigned (*NewtonUniversalCallBack) (const NewtonJoint* const universal, NewtonHingeSliderUpdateDesc* const desc);
	typedef unsigned (*NewtonCorkscrewCallBack) (const NewtonJoint* const corkscrew, NewtonHingeSliderUpdateDesc* const desc);

	typedef void (*NewtonUserBilateralCallBack) (const NewtonJoint* const userJoint, dFloat timestep, int threadIndex);
	typedef void (*NewtonUserBilateralGetInfoCallBack) (const NewtonJoint* const userJoint, NewtonJointRecord* const info);

	typedef void (*NewtonConstraintDestructor) (const NewtonJoint*const  me);

//	typedef void (*NewtonSetRagDollTransform) (const NewtonRagDollBone* const bone);
//	typedef void (*NewtonBodyActivationState) (const NewtonBody* const body, unsigned state);
//	typedef void (*NewtonVehicleTireUpdate) (const NewtonJoint* const vehicle, dFloat timestep);
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

Rem

	Function NewtonSetPerformanceClock (newtonWorld:Byte Ptr, NewtonGetTicksCountCallback callback);
	NEWTON_API unsigned NewtonReadPerformanceTicks (newtonWorld:Byte Ptr, unsigned performanceEntry);
	NEWTON_API unsigned NewtonReadThreadPerformanceTicks (const NewtonWorld* newtonWorld, unsigned threadIndex);

	Function NewtonWorldCriticalSectionLock (newtonWorld:Byte Ptr);
	Function NewtonWorldCriticalSectionUnlock (newtonWorld:Byte Ptr);
	Function NewtonSetThreadsCount (newtonWorld:Byte Ptr, int threads);
	Function NewtonGetThreadsCount(newtonWorld:Byte Ptr);
	Function NewtonGetMaxThreadsCount(newtonWorld:Byte Ptr);

End Rem

	Function NewtonSetFrictionModel(newtonWorld:Byte Ptr, model)
	Function NewtonSetMinimumFrameRate(newtonWorld:Byte Ptr, frameRate#)
	Function NewtonSetBodyLeaveWorldEvent(newtonWorld:Byte Ptr, callback:Byte Ptr)
	Function NewtonSetWorldSize(newtonWorld:Byte Ptr, minPoint:Float Ptr,  maxPoint:Float Ptr)
	Function NewtonSetIslandUpdateEvent(newtonWorld:Byte Ptr, islandUpdate:Byte Ptr)
	Function NewtonSetCollisionDestructor(newtonWorld:Byte Ptr, callback:Byte Ptr)
	Function NewtonSetDestroyBodyByExeciveForce(newtonWorld:Byte Ptr, callback:Byte Ptr)

Rem

//	Function NewtonWorldForEachBodyDo (newtonWorld:Byte Ptr, NewtonBodyIterator callback);
	Function NewtonWorldForEachJointDo (newtonWorld:Byte Ptr, NewtonJointIterator callback, void* const userData);
	Function NewtonWorldForEachBodyInAABBDo (newtonWorld:Byte Ptr, const dFloat* const p0, const dFloat* const p1, NewtonBodyIterator callback, void* const userData);

	
	Function NewtonWorldSetUserData (newtonWorld:Byte Ptr, void* const userData);
	Function* NewtonWorldGetUserData (newtonWorld:Byte Ptr);
	Function NewtonWorldSetDestructorCallBack (newtonWorld:Byte Ptr, NewtonDestroyWorld destructor);
	NEWTON_API NewtonDestroyWorld NewtonWorldGetDestructorCallBack (newtonWorld:Byte Ptr);

	Function NewtonWorldRayCast (newtonWorld:Byte Ptr, const dFloat* const p0, const dFloat* const p1, NewtonWorldRayFilterCallback filter, void* const userData, 
										NewtonWorldRayPrefilterCallback prefilter);
	Function NewtonWorldConvexCast (newtonWorld:Byte Ptr, const dFloat* const matrix, const dFloat* const target, const NewtonCollision* shape, dFloat* const hitParam, void* const userData,  
										  NewtonWorldRayPrefilterCallback prefilter, NewtonWorldConvexCastReturnInfo* info, int maxContactsCount, int threadIndex);
									   

	// world utility functions
	Function NewtonWorldGetBodyCount(newtonWorld:Byte Ptr);
	Function NewtonWorldGetConstraintCount(newtonWorld:Byte Ptr);

	// Function NewtonGetActiveBodiesCount();
	// Function NewtonGetActiveConstraintsCount();
	// NEWTON_API dFloat NewtonGetGlobalScale (newtonWorld:Byte Ptr);



	// **********************************************************************************************
	//
	// Simulation islands 
	//
	// **********************************************************************************************
	NEWTON_API NewtonBody* NewtonIslandGetBody (const void* const island, int bodyIndex);
	Function NewtonIslandGetBodyAABB (const void* const island, int bodyIndex, dFloat* const p0, dFloat* const p1);

	// **********************************************************************************************
	//
	// Physics Material Section
	//
	// **********************************************************************************************
	Function NewtonMaterialCreateGroupID(newtonWorld:Byte Ptr);
	Function NewtonMaterialGetDefaultGroupID(newtonWorld:Byte Ptr);
	Function NewtonMaterialDestroyAllGroupID(newtonWorld:Byte Ptr);

	// material definitions that can not be overwritten in function callback
	Function* NewtonMaterialGetUserData (newtonWorld:Byte Ptr, int id0, int id1);
	Function NewtonMaterialSetSurfaceThickness (newtonWorld:Byte Ptr, int id0, int id1, dFloat thickness);
	Function NewtonMaterialSetContinuousCollisionMode (newtonWorld:Byte Ptr, int id0, int id1, int state);
	Function NewtonMaterialSetCollisionCallback (newtonWorld:Byte Ptr, int id0, int id1, void* const userData,
														NewtonOnAABBOverlap aabbOverlap, NewtonContactsProcess process);

	Function NewtonMaterialSetDefaultSoftness (newtonWorld:Byte Ptr, int id0, int id1, dFloat value);
	Function NewtonMaterialSetDefaultElasticity (newtonWorld:Byte Ptr, int id0, int id1, dFloat elasticCoef);
	Function NewtonMaterialSetDefaultCollidable (newtonWorld:Byte Ptr, int id0, int id1, int state);
	Function NewtonMaterialSetDefaultFriction (newtonWorld:Byte Ptr, int id0, int id1, dFloat staticFriction, dFloat kineticFriction);

	NEWTON_API NewtonMaterial* NewtonWorldGetFirstMaterial (newtonWorld:Byte Ptr);
	NEWTON_API NewtonMaterial* NewtonWorldGetNextMaterial (newtonWorld:Byte Ptr, const NewtonMaterial* const material);

	NEWTON_API NewtonBody* NewtonWorldGetFirstBody (newtonWorld:Byte Ptr);
	NEWTON_API NewtonBody* NewtonWorldGetNextBody (newtonWorld:Byte Ptr, const NewtonBody* const curBody);


	// **********************************************************************************************
	//
	// Physics Contact control functions
	//
	// **********************************************************************************************
	Function *NewtonMaterialGetMaterialPairUserData (const NewtonMaterial* const material);
	NEWTON_API unsigned NewtonMaterialGetContactFaceAttribute (const NewtonMaterial* const material);
	NEWTON_API unsigned NewtonMaterialGetBodyCollisionID (const NewtonMaterial* const material, const NewtonBody* const body);
	NEWTON_API dFloat NewtonMaterialGetContactNormalSpeed (const NewtonMaterial* const material);
	Function NewtonMaterialGetContactForce (const NewtonMaterial* const material, NewtonBody* const body, dFloat* const force);
	Function NewtonMaterialGetContactPositionAndNormal (const NewtonMaterial* const material, NewtonBody* const body, dFloat* const posit, dFloat* const normal);
	Function NewtonMaterialGetContactTangentDirections (const NewtonMaterial* const material, NewtonBody* const body, dFloat* const dir0, dFloat* const dir1);
	NEWTON_API dFloat NewtonMaterialGetContactTangentSpeed (const NewtonMaterial* const material, int index);
	
	Function NewtonMaterialSetContactSoftness (const NewtonMaterial* const material, dFloat softness);
	Function NewtonMaterialSetContactElasticity (const NewtonMaterial* const material, dFloat restitution);
	Function NewtonMaterialSetContactFrictionState (const NewtonMaterial* const material, int state, int index);
	Function NewtonMaterialSetContactFrictionCoef (const NewtonMaterial* const material, dFloat staticFrictionCoef, dFloat kineticFrictionCoef, int index);
	
	Function NewtonMaterialSetContactNormalAcceleration (const NewtonMaterial* const material, dFloat accel);
	Function NewtonMaterialSetContactNormalDirection (const NewtonMaterial* const material, const dFloat* const directionVector);

	Function NewtonMaterialSetContactTangentAcceleration (const NewtonMaterial* const material, dFloat accel, int index);
	Function NewtonMaterialContactRotateTangentDirections (const NewtonMaterial* const material, const dFloat* const directionVector);

End Rem
	

	Function NewtonCreateNull:Byte Ptr (newtonWorld:Byte Ptr)
	Function NewtonCreateSphere:Byte Ptr (newtonWorld:Byte Ptr, radiusX#, radiusY#, radiusZ#, shapeID, offsetMatrix:Float Ptr)
	Function NewtonCreateBox:Byte Ptr(newtonWorld:Byte Ptr, dx#, dy#, dz#, shapeID, offsetMatrix:Float Ptr)
	Function NewtonCreateCone:Byte Ptr (newtonWorld:Byte Ptr, radius#, height#, shapeID, offsetMatrix:Float Ptr)
	Function NewtonCreateCapsule:Byte Ptr (newtonWorld:Byte Ptr, radius#, height#, shapeID, offsetMatrix:Float Ptr)
	Function NewtonCreateCylinder:Byte Ptr (newtonWorld:Byte Ptr, radius#, height#, shapeID, offsetMatrix:Float Ptr)
	Function NewtonCreateChamferCylinder:Byte Ptr (newtonWorld:Byte Ptr, radius#, height#, shapeID,  offsetMatrix:Float Ptr)
	Function NewtonCreateConvexHul:Byte Ptr (newtonWorld:Byte Ptr, count, vertexCloud:Float Ptr, strideInBytes, tolerance#, shapeID, offsetMatrix:Float Ptr)
	Function NewtonCreateConvexHullFromMesh:Byte Ptr (newtonWorld:Byte Ptr, mesh:Byte Ptr, tolerance#, shapeID)

Rem

	NEWTON_API NewtonCollision* NewtonCreateConvexHullModifier (newtonWorld:Byte Ptr, const NewtonCollision* const convexHullCollision, int shapeID);
	Function NewtonConvexHullModifierGetMatrix (const NewtonCollision* const convexHullCollision, dFloat* const matrix);
	Function NewtonConvexHullModifierSetMatrix (const NewtonCollision* const convexHullCollision, const dFloat* const matrix);

	Function NewtonCollisionIsTriggerVolume(const NewtonCollision* const convexCollision);
	Function NewtonCollisionSetAsTriggerVolume(const NewtonCollision* convexCollision, int trigger);

	Function NewtonCollisionSetMaxBreakImpactImpulse(const NewtonCollision* const convexHullCollision, dFloat maxImpactImpulse);
	NEWTON_API dFloat NewtonCollisionGetMaxBreakImpactImpulse(const NewtonCollision* const convexHullCollision);

	Function NewtonCollisionSetUserID (const NewtonCollision* convexCollision, unsigned id);
	NEWTON_API unsigned NewtonCollisionGetUserID (const NewtonCollision* const convexCollision);

	Function NewtonConvexHullGetFaceIndices (const NewtonCollision* const convexHullCollision, int face, int* const faceIndices);
	NEWTON_API dFloat NewtonConvexCollisionCalculateVolume (const NewtonCollision* const convexCollision);
	Function NewtonConvexCollisionCalculateInertialMatrix (const NewtonCollision* convexCollision, dFloat* const inertia, dFloat* const origin);	


	Function NewtonCollisionMakeUnique (newtonWorld:Byte Ptr, const NewtonCollision* const collision);
	Function NewtonReleaseCollision (newtonWorld:Byte Ptr, const NewtonCollision* const collision);
	Function NewtonAddCollisionReference (const NewtonCollision* const collision);

	
	// **********************************************************************************************
	//
	// mass/spring/damper collision shape
	//
	// **********************************************************************************************
//	NEWTON_API NewtonCollision* NewtonCreateSoftShape (newtonWorld:Byte Ptr);
//	Function NewtonSoftBodySetMassCount (const NewtonCollision* convexCollision, int count);
//	Function NewtonSoftBodySetSpringCount (const NewtonCollision* convexCollision, int count);

//	Function NewtonSoftBodySetMass (const NewtonCollision* convexCollision, int index, dFloat mass, dFloat* position);
//	Function NewtonSoftBodySetSpring (const NewtonCollision* convexCollision, int index, int mass0, int mass1, dFloat stiffness, dFloat damper);
//	Function NewtonSoftBodyGetMassArray (const NewtonCollision* convexCollision, dFloat* masses, dFloat** positions);	


	// **********************************************************************************************
	//
	// complex collision primitives creation functions
	//
	// **********************************************************************************************
	NEWTON_API NewtonCollision* NewtonCreateCompoundCollision (newtonWorld:Byte Ptr, int count, NewtonCollision* const collisionPrimitiveArray[], int shapeID);
	//NEWTON_API NewtonCollision* NewtonCreateCompoundCollisionFromMesh (newtonWorld:Byte Ptr, const NewtonMesh* const mesh, dFloat concavity, int shapeID, int subShapeID);
	//NEWTON_API NewtonCollision* NewtonCreateCompoundCollisionFromMesh(newtonWorld:Byte Ptr, const NewtonMesh* const mesh, int maxSubShapesCount, int shapeID, int subShapeID);

	// **********************************************************************************************
	//
	// complex breakable collision primitives interface
	//
	// **********************************************************************************************
//	NEWTON_API NewtonCollision* NewtonCreateCompoundBreakable (newtonWorld:Byte Ptr, int meshCount, 
//															   NewtonMesh* const solids[], NewtonMesh* const splitePlanes[], 
//															   dFloat* const matrixPallete, int* const shapeIDArray, dFloat* const densities,
//															   int shapeID, int debriID, NewtonCollisionCompoundBreakableCallback callback, void* buildUsedData);

	NEWTON_API NewtonCollision* NewtonCreateCompoundBreakable (newtonWorld:Byte Ptr, int meshCount, 
															   const NewtonMesh** const solids, const int* const shapeIDArray, 
															   const dFloat* const densities, const int* const internalFaceMaterial, 
															   int shapeID, int debriID, dFloat debriSeparationGap);


	Function NewtonCompoundBreakableResetAnchoredPieces (const NewtonCollision* const compoundBreakable);
	Function NewtonCompoundBreakableSetAnchoredPieces (const NewtonCollision* const compoundBreakable, int fixShapesCount, dFloat* const matrixPallete, NewtonCollision** const fixedShapesArray);

	Function NewtonCompoundBreakableGetVertexCount (const NewtonCollision* const compoundBreakable); 
	Function NewtonCompoundBreakableGetVertexStreams (const NewtonCollision* const compoundBreakable, int vertexStrideInByte, dFloat* const vertex,
															int normalStrideInByte, dFloat* const normal,	int uvStrideInByte, dFloat* const uv);

	
	NEWTON_API NewtonbreakableComponentMesh* NewtonBreakableGetMainMesh (const NewtonCollision* const compoundBreakable);
	NEWTON_API NewtonbreakableComponentMesh* NewtonBreakableGetFirstComponent (const NewtonCollision* const compoundBreakable);
	NEWTON_API NewtonbreakableComponentMesh* NewtonBreakableGetNextComponent (const NewtonbreakableComponentMesh* const component);

	Function NewtonBreakableBeginDelete (const NewtonCollision* const compoundBreakable);
	NEWTON_API NewtonBody* NewtonBreakableCreateDebrieBody (const NewtonCollision* const compoundBreakable, const NewtonbreakableComponentMesh* const component);
	Function NewtonBreakableDeleteComponent (const NewtonCollision* const compoundBreakable, const NewtonbreakableComponentMesh* const component);
	Function NewtonBreakableEndDelete (const NewtonCollision* const compoundBreakable);


	Function NewtonBreakableGetComponentsInRadius (const NewtonCollision* const compoundBreakable, const dFloat* position, dFloat radius, NewtonbreakableComponentMesh** const segments, int maxCount);

	Function* NewtonBreakableGetFirstSegment (const NewtonbreakableComponentMesh* const breakableComponent); 
	Function* NewtonBreakableGetNextSegment (const void* const segment); 

	Function NewtonBreakableSegmentGetMaterial (const void* const segment); 
	Function NewtonBreakableSegmentGetIndexCount (const void* const segment); 
	Function NewtonBreakableSegmentGetIndexStream (const NewtonCollision* const compoundBreakable, const NewtonbreakableComponentMesh* const meshOwner, const void* const segment, int* const index); 
	Function NewtonBreakableSegmentGetIndexStreamShort (const NewtonCollision* const compoundBreakable, const NewtonbreakableComponentMesh* const meshOwner, const void* const segment, short int* const index); 


	NEWTON_API NewtonCollision* NewtonCreateUserMeshCollision (newtonWorld:Byte Ptr, const dFloat* const minBox, 
		const dFloat* const maxBox, void* const userData, NewtonUserMeshCollisionCollideCallback collideCallback, 
		NewtonUserMeshCollisionRayHitCallback rayHitCallback, NewtonUserMeshCollisionDestroyCallback destroyCallback,
		NewtonUserMeshCollisionGetCollisionInfo getInfoCallback, NewtonUserMeshCollisionGetFacesInAABB facesInAABBCallback, int shapeID);


	NEWTON_API NewtonCollision* NewtonCreateSceneCollision (newtonWorld:Byte Ptr, int shapeID);
	NEWTON_API NewtonSceneProxy* NewtonSceneCollisionCreateProxy (NewtonCollision* const scene, NewtonCollision* const collision, const dFloat* const matrix);

	Function NewtonSceneCollisionDestroyProxy (NewtonCollision* const scene, NewtonSceneProxy* Proxy);
	Function NewtonSceneProxySetMatrix (NewtonSceneProxy* const proxy, const dFloat* matrix);
	Function NewtonSceneProxyGetMatrix (NewtonSceneProxy* const proxy, dFloat* matrix);
	Function NewtonSceneSetProxyUserData (NewtonSceneProxy* const proxy, void* userData);
	Function* NewtonSceneGetProxyUserData (NewtonSceneProxy* const proxy);

	NEWTON_API NewtonSceneProxy* NewtonSceneGetFirstProxy (NewtonCollision* const scene);
	NEWTON_API NewtonSceneProxy* NewtonSceneGetNextProxy (NewtonCollision* const scene, NewtonSceneProxy* const proxy);
	
	Function NewtonSceneCollisionOptimize (NewtonCollision* scene);

	//  ***********************************************************************************************************
	//
	//	Collision serialization functions
	//
	// ***********************************************************************************************************
	NEWTON_API NewtonCollision* NewtonCreateCollisionFromSerialization (newtonWorld:Byte Ptr, NewtonDeserialize deserializeFunction, void* const serializeHandle);
	Function NewtonCollisionSerialize (newtonWorld:Byte Ptr, const NewtonCollision* const collision, NewtonSerialize serializeFunction, void* const serializeHandle);
	Function NewtonCollisionGetInfo (const NewtonCollision* const collision, NewtonCollisionInfoRecord* const collisionInfo);

	// **********************************************************************************************
	//
	// Static collision shapes functions
	//
	// **********************************************************************************************
	NEWTON_API NewtonCollision* NewtonCreateHeightFieldCollision (newtonWorld:Byte Ptr, int width, int height, int gridsDiagonals,
																  const unsigned short* const elevationMap, const char* const attributeMap,
																  dFloat horizontalScale, dFloat verticalScale, int shapeID);
	Function NewtonHeightFieldSetUserRayCastCallback (const NewtonCollision* const treeCollision, NewtonHeightFieldRayCastCallback rayHitCallback);

	
	NEWTON_API NewtonCollision* NewtonCreateTreeCollision (newtonWorld:Byte Ptr, int shapeID);
	Function NewtonTreeCollisionSetUserRayCastCallback (const NewtonCollision* const treeCollision, NewtonCollisionTreeRayCastCallback rayHitCallback);

	Function NewtonTreeCollisionBeginBuild (const NewtonCollision* treeCollision);
	Function NewtonTreeCollisionAddFace (const NewtonCollision* const treeCollision, int vertexCount, const dFloat* const vertexPtr, int strideInBytes, int faceAttribute);
	Function NewtonTreeCollisionEndBuild (const NewtonCollision* const treeCollision, int optimize);

	Function NewtonTreeCollisionGetFaceAtribute (const NewtonCollision* const treeCollision, const int* const faceIndexArray); 
	Function NewtonTreeCollisionSetFaceAtribute (const NewtonCollision* const treeCollision, const int* const faceIndexArray, int attribute); 
	Function NewtonTreeCollisionGetVertexListIndexListInAABB (const NewtonCollision* const treeCollision, const dFloat* const p0, const dFloat* const p1, const dFloat** const vertexArray, int* const vertexCount, int* const vertexStrideInBytes, const int* const indexList, int maxIndexCount, const int* const faceAttribute); 


	Function NewtonStaticCollisionSetDebugCallback (const NewtonCollision* const staticCollision, NewtonTreeCollisionCallback userCallback);

	// **********************************************************************************************
	//
	// General purpose collision library functions
	//
	// **********************************************************************************************
	Function NewtonCollisionPointDistance (newtonWorld:Byte Ptr, const dFloat* const point,
		const NewtonCollision* const collision, const dFloat* const matrix, dFloat* const contact, dFloat* const normal, int threadIndex);

	Function NewtonCollisionClosestPoint (newtonWorld:Byte Ptr, 
		const NewtonCollision* const collisionA, const dFloat* const matrixA, const NewtonCollision* const collisionB, const dFloat* const matrixB,
		dFloat* const contactA, dFloat* const contactB, dFloat* const normalAB, int threadIndex);
	
	Function NewtonCollisionCollide (newtonWorld:Byte Ptr, int maxSize,
		const NewtonCollision* const collisionA, const dFloat* const matrixA, 
		const NewtonCollision* const collisionB, const dFloat* const matrixB,
		dFloat* const contacts, dFloat* const normals, dFloat* const penetration, int threadIndex);

	Function NewtonCollisionCollideContinue (newtonWorld:Byte Ptr, int maxSize, const dFloat timestep, 
		const NewtonCollision* const collisionA, const dFloat* const matrixA, const dFloat* const velocA, const dFloat* omegaA, 
		const NewtonCollision* const collisionB, const dFloat* const matrixB, const dFloat* const velocB, const dFloat* const omegaB, 
		dFloat* const timeOfImpact, dFloat* const contacts, dFloat* const normals, dFloat* const penetration, int threadIndex);

	Function NewtonCollisionSupportVertex (const NewtonCollision* const collision, const dFloat* const dir, dFloat* const vertex);
	NEWTON_API dFloat NewtonCollisionRayCast (const NewtonCollision* const collision, const dFloat* const p0, const dFloat* const p1, dFloat* const normal, int* const attribute);
	Function NewtonCollisionCalculateAABB (const NewtonCollision* const collision, const dFloat* const matrix, dFloat* const p0, dFloat* const p1);
	Function NewtonCollisionForEachPolygonDo (const NewtonCollision* const collision, const dFloat* const matrix, NewtonCollisionIterator callback, void* const userData);
	
	
	// **********************************************************************************************
	//
	// transforms utility functions
	//
	// **********************************************************************************************
	Function NewtonGetEulerAngle (const dFloat* const matrix, dFloat* const eulersAngles);
	Function NewtonSetEulerAngle (const dFloat* const eulersAngles, dFloat* const matrix);
	NEWTON_API dFloat NewtonCalculateSpringDamperAcceleration (dFloat dt, dFloat ks, dFloat x, dFloat kd, dFloat s);

	// **********************************************************************************************
	//
	// body manipulation functions
	//
	// **********************************************************************************************
	NEWTON_API NewtonBody* NewtonCreateBody (newtonWorld:Byte Ptr, const NewtonCollision* const collision, const dFloat* const matrix);
	Function  NewtonDestroyBody(newtonWorld:Byte Ptr, const NewtonBody* const body);

	Function  NewtonBodyAddForce (const NewtonBody* const body, const dFloat* const force);
	Function  NewtonBodyAddTorque (const NewtonBody* const body, const dFloat* const torque);
	Function  NewtonBodyCalculateInverseDynamicsForce (const NewtonBody* const body, dFloat timestep, const dFloat* const desiredVeloc, dFloat* const forceOut);

	Function  NewtonBodySetMatrix (const NewtonBody* const body, const dFloat* const matrix);
	Function  NewtonBodySetMatrixRecursive (const NewtonBody* const body, const dFloat* const matrix);
	Function  NewtonBodySetMassMatrix (const NewtonBody* const body, dFloat mass, dFloat Ixx, dFloat Iyy, dFloat Izz);
	Function  NewtonBodySetMaterialGroupID (const NewtonBody* const body, int id);
	Function  NewtonBodySetContinuousCollisionMode (const NewtonBody* const body, unsigned state);
	Function  NewtonBodySetJointRecursiveCollision (const NewtonBody* const body, unsigned state);
	Function  NewtonBodySetOmega (const NewtonBody* const body, const dFloat* const omega);
	Function  NewtonBodySetVelocity (const NewtonBody* const body, const dFloat* const velocity);
	Function  NewtonBodySetForce (const NewtonBody* const body, const dFloat* const force);
	Function  NewtonBodySetTorque (const NewtonBody* const body, const dFloat* const torque);
	
	Function  NewtonBodySetCentreOfMass  (const NewtonBody* const body, const dFloat* const com);
	Function  NewtonBodySetLinearDamping (const NewtonBody* const body, dFloat linearDamp);
	Function  NewtonBodySetAngularDamping (const NewtonBody* const body, const dFloat* const angularDamp);
	Function  NewtonBodySetUserData (const NewtonBody* const body, void* const userData);
	Function  NewtonBodySetCollision (const NewtonBody* const body, const NewtonCollision* const collision);

	

	Function  NewtonBodyGetSleepState (const NewtonBody* const body);
	Function  NewtonBodyGetAutoSleep (const NewtonBody* const body);
	Function NewtonBodySetAutoSleep (const NewtonBody* const body, int state);
	
	Function  NewtonBodyGetFreezeState(const NewtonBody* const body);
	Function NewtonBodySetFreezeState (const NewtonBody* const body, int state);


//	Function NewtonBodySetAutoFreeze(const NewtonBody* const body, int state);
//	Function NewtonBodyCoriolisForcesMode (const NewtonBody* const body, int mode);
//	Function NewtonBodySetGyroscopicForcesMode (const NewtonBody* const body, int mode);
//	Function  NewtonBodyGetGyroscopicForcesMode (const NewtonBody* const body);
//	Function  NewtonBodyGetFreezeState (const NewtonBody* const body);
//	Function NewtonBodySetFreezeState  (const NewtonBody* const body, int state);
//	Function NewtonBodyGetFreezeTreshold (const NewtonBody* const body, dFloat* freezeSpeed2, dFloat* freezeOmega2);
//	Function NewtonBodySetFreezeTreshold (const NewtonBody* const body, dFloat freezeSpeed2, dFloat freezeOmega2, int framesCount);
//	Function NewtonBodySetAutoactiveCallback (const NewtonBody* const body, NewtonBodyActivationState callback);
	

	Function  NewtonBodySetDestructorCallback (const NewtonBody* const body, NewtonBodyDestructor callback);

	Function  NewtonBodySetTransformCallback (const NewtonBody* const body, NewtonSetTransform callback);
	NEWTON_API NewtonSetTransform NewtonBodyGetTransformCallback (const NewtonBody* const body);

	Function  NewtonBodySetForceAndTorqueCallback (const NewtonBody* const body, NewtonApplyForceAndTorque callback);
	NEWTON_API NewtonApplyForceAndTorque NewtonBodyGetForceAndTorqueCallback (const NewtonBody* const body);
	Function* NewtonBodyGetUserData (const NewtonBody* const body);
	NEWTON_API NewtonWorld* NewtonBodyGetWorld (const NewtonBody* const body);
	NEWTON_API NewtonCollision* NewtonBodyGetCollision (const NewtonBody* const body);
	Function   NewtonBodyGetMaterialGroupID (const NewtonBody* const body);

	Function   NewtonBodyGetContinuousCollisionMode (const NewtonBody* const body);
	Function   NewtonBodyGetJointRecursiveCollision (const NewtonBody* const body);

	Function  NewtonBodyGetMatrix(const NewtonBody* const body, dFloat* const matrix);
	Function  NewtonBodyGetRotation(const NewtonBody* const body, dFloat* const rotation);
	Function  NewtonBodyGetMassMatrix (const NewtonBody* const body, dFloat* mass, dFloat* const Ixx, dFloat* const Iyy, dFloat* const Izz);
	Function  NewtonBodyGetInvMass(const NewtonBody* const body, dFloat* const invMass, dFloat* const invIxx, dFloat* const invIyy, dFloat* const invIzz);
	Function  NewtonBodyGetOmega(const NewtonBody* const body, dFloat* const vector);
	Function  NewtonBodyGetVelocity(const NewtonBody* const body, dFloat* const vector);
	Function  NewtonBodyGetForce(const NewtonBody* const body, dFloat* const vector);
	Function  NewtonBodyGetTorque(const NewtonBody* const body, dFloat* const vector);
	Function  NewtonBodyGetForceAcc(const NewtonBody* const body, dFloat* const vector);
	Function  NewtonBodyGetTorqueAcc(const NewtonBody* const body, dFloat* const vector);
	Function  NewtonBodyGetCentreOfMass (const NewtonBody* const body, dFloat* const com);

	NEWTON_API dFloat NewtonBodyGetLinearDamping (const NewtonBody* const body);
	Function  NewtonBodyGetAngularDamping (const NewtonBody* const body, dFloat* const vector);
	Function  NewtonBodyGetAABB (const NewtonBody* const body, dFloat* const p0, dFloat* const p1);
	NEWTON_API NewtonJoint* NewtonBodyGetFirstJoint (const NewtonBody* const body);
	NEWTON_API NewtonJoint* NewtonBodyGetNextJoint (const NewtonBody* const body, const NewtonJoint* const joint);
	NEWTON_API NewtonJoint* NewtonBodyGetFirstContactJoint (const NewtonBody* const body);
	NEWTON_API NewtonJoint* NewtonBodyGetNextContactJoint (const NewtonBody* const body, const NewtonJoint* const contactJoint);

	Function* NewtonContactJointGetFirstContact (const NewtonJoint* const contactJoint);
	Function* NewtonContactJointGetNextContact (const NewtonJoint* const contactJoint, void* const contact);

	Function NewtonContactJointGetContactCount(const NewtonJoint* const contactJoint);
	Function NewtonContactJointRemoveContact(const NewtonJoint* const contactJoint, void* const contact); 

	NEWTON_API NewtonMaterial* NewtonContactGetMaterial (const void* const contact);

	
	Function  NewtonBodyAddBuoyancyForce (const NewtonBody* const body, dFloat fluidDensity, 
		dFloat fluidLinearViscosity, dFloat fluidAngularViscosity, 
		const dFloat* const gravityVector, NewtonGetBuoyancyPlane buoyancyPlane, void* const context);

//	Function NewtonBodyForEachPolygonDo (const NewtonBody* const body, NewtonCollisionIterator callback);
	Function NewtonBodyAddImpulse (const NewtonBody* const body, const dFloat* const pointDeltaVeloc, const dFloat* const pointPosit);


	// **********************************************************************************************
	//
	// Common joint functions
	//
	// **********************************************************************************************
	Function* NewtonJointGetUserData (const NewtonJoint* const joint);
	Function NewtonJointSetUserData (const NewtonJoint* const joint, void* const userData);

	NEWTON_API NewtonBody* NewtonJointGetBody0 (const NewtonJoint* const joint);
	NEWTON_API NewtonBody* NewtonJointGetBody1 (const NewtonJoint* const joint);

	Function NewtonJointGetInfo  (const NewtonJoint* const joint, NewtonJointRecord* const info);
	Function NewtonJointGetCollisionState (const NewtonJoint* const joint);
	Function NewtonJointSetCollisionState (const NewtonJoint* const joint, int state);

	NEWTON_API dFloat NewtonJointGetStiffness (const NewtonJoint* const joint);
	Function NewtonJointSetStiffness (const NewtonJoint* const joint, dFloat state);

	Function NewtonDestroyJoint(newtonWorld:Byte Ptr, const NewtonJoint* const joint);
	Function NewtonJointSetDestructor (const NewtonJoint* const joint, NewtonConstraintDestructor destructor);

	


	// **********************************************************************************************
	//
	// Ball and Socket joint functions
	//
	// **********************************************************************************************
	NEWTON_API NewtonJoint* NewtonConstraintCreateBall (newtonWorld:Byte Ptr, const dFloat* pivotPoint, 
														const NewtonBody* const childBody, const NewtonBody* const parentBody);
	Function NewtonBallSetUserCallback (const NewtonJoint* const ball, NewtonBallCallBack callback);
	Function NewtonBallGetJointAngle (const NewtonJoint* const ball, dFloat* angle);
	Function NewtonBallGetJointOmega (const NewtonJoint* const ball, dFloat* omega);
	Function NewtonBallGetJointForce (const NewtonJoint* const ball, dFloat* const force);
	Function NewtonBallSetConeLimits (const NewtonJoint* const ball, const dFloat* pin, dFloat maxConeAngle, dFloat maxTwistAngle);

	// **********************************************************************************************
	//
	// Hinge joint functions
	//
	// **********************************************************************************************
	NEWTON_API NewtonJoint* NewtonConstraintCreateHinge (newtonWorld:Byte Ptr, 
		const dFloat* pivotPoint, const dFloat* pinDir, 
		const NewtonBody* const childBody, const NewtonBody* const parentBody);

	Function NewtonHingeSetUserCallback (const NewtonJoint* const hinge, NewtonHingeCallBack callback);
	NEWTON_API dFloat NewtonHingeGetJointAngle (const NewtonJoint* const hinge);
	NEWTON_API dFloat NewtonHingeGetJointOmega (const NewtonJoint* const hinge);
	Function NewtonHingeGetJointForce (const NewtonJoint* const hinge, dFloat* const force);
	NEWTON_API dFloat NewtonHingeCalculateStopAlpha (const NewtonJoint* const hinge, const NewtonHingeSliderUpdateDesc* const desc, dFloat angle);

	// **********************************************************************************************
	//
	// Slider joint functions
	//
	// **********************************************************************************************
	NEWTON_API NewtonJoint* NewtonConstraintCreateSlider (newtonWorld:Byte Ptr, 
		const dFloat* pivotPoint, const dFloat* pinDir, 
		const NewtonBody* const childBody, const NewtonBody* const parentBody);
	Function NewtonSliderSetUserCallback (const NewtonJoint* const slider, NewtonSliderCallBack callback);
	NEWTON_API dFloat NewtonSliderGetJointPosit (const NewtonJoint* slider);
	NEWTON_API dFloat NewtonSliderGetJointVeloc (const NewtonJoint* slider);
	Function NewtonSliderGetJointForce (const NewtonJoint* const slider, dFloat* const force);
	NEWTON_API dFloat NewtonSliderCalculateStopAccel (const NewtonJoint* const slider, const NewtonHingeSliderUpdateDesc* const desc, dFloat position);


	// **********************************************************************************************
	//
	// Corkscrew joint functions
	//
	// **********************************************************************************************
	NEWTON_API NewtonJoint* NewtonConstraintCreateCorkscrew (newtonWorld:Byte Ptr, 
		const dFloat* pivotPoint, const dFloat* pinDir, 
		const NewtonBody* const childBody, const NewtonBody* const parentBody);
	Function NewtonCorkscrewSetUserCallback (const NewtonJoint* const corkscrew, NewtonCorkscrewCallBack callback);
	NEWTON_API dFloat NewtonCorkscrewGetJointPosit (const NewtonJoint* const corkscrew);
	NEWTON_API dFloat NewtonCorkscrewGetJointAngle (const NewtonJoint* const corkscrew);
	NEWTON_API dFloat NewtonCorkscrewGetJointVeloc (const NewtonJoint* const corkscrew);
	NEWTON_API dFloat NewtonCorkscrewGetJointOmega (const NewtonJoint* const corkscrew);
	Function NewtonCorkscrewGetJointForce (const NewtonJoint* const corkscrew, dFloat* const force);
	NEWTON_API dFloat NewtonCorkscrewCalculateStopAlpha (const NewtonJoint* const corkscrew, const NewtonHingeSliderUpdateDesc* const desc, dFloat angle);
	NEWTON_API dFloat NewtonCorkscrewCalculateStopAccel (const NewtonJoint* const corkscrew, const NewtonHingeSliderUpdateDesc* const desc, dFloat position);


	// **********************************************************************************************
	//
	// Universal joint functions
	//
	// **********************************************************************************************
	NEWTON_API NewtonJoint* NewtonConstraintCreateUniversal (newtonWorld:Byte Ptr, 
		const dFloat* pivotPoint, const dFloat* pinDir0, const dFloat* pinDir1, 
		const NewtonBody* const childBody, const NewtonBody* const parentBody);
	Function NewtonUniversalSetUserCallback (const NewtonJoint* const universal, NewtonUniversalCallBack callback);
	NEWTON_API dFloat NewtonUniversalGetJointAngle0 (const NewtonJoint* const universal);
	NEWTON_API dFloat NewtonUniversalGetJointAngle1 (const NewtonJoint* const universal);
	NEWTON_API dFloat NewtonUniversalGetJointOmega0 (const NewtonJoint* const universal);
	NEWTON_API dFloat NewtonUniversalGetJointOmega1 (const NewtonJoint* const universal);
	Function NewtonUniversalGetJointForce (const NewtonJoint* const universal, dFloat* const force);
	NEWTON_API dFloat NewtonUniversalCalculateStopAlpha0 (const NewtonJoint* const universal, const NewtonHingeSliderUpdateDesc* const desc, dFloat angle);
	NEWTON_API dFloat NewtonUniversalCalculateStopAlpha1 (const NewtonJoint* const universal, const NewtonHingeSliderUpdateDesc* const desc, dFloat angle);


	// **********************************************************************************************
	//
	// Up vector joint functions
	//
	// **********************************************************************************************
	NEWTON_API NewtonJoint* NewtonConstraintCreateUpVector (newtonWorld:Byte Ptr, const dFloat* pinDir, const NewtonBody* const body); 
	Function NewtonUpVectorGetPin (const NewtonJoint* const upVector, dFloat *pin);
	Function NewtonUpVectorSetPin (const NewtonJoint* const upVector, const dFloat *pin);


	// **********************************************************************************************
	//
	// User defined bilateral Joint
	//
	// **********************************************************************************************
	NEWTON_API NewtonJoint* NewtonConstraintCreateUserJoint (newtonWorld:Byte Ptr, int maxDOF, 
															 NewtonUserBilateralCallBack callback,
															 NewtonUserBilateralGetInfoCallBack getInfo,
															 const NewtonBody* const childBody, const NewtonBody* const parentBody) ; 
	Function NewtonUserJointSetFeedbackCollectorCallback (const NewtonJoint* const joint, NewtonUserBilateralCallBack getFeedback);
	Function NewtonUserJointAddLinearRow (const NewtonJoint* const joint, const dFloat* const pivot0, const dFloat* const pivot1, const dFloat* const dir);
	Function NewtonUserJointAddAngularRow (const NewtonJoint* const joint, dFloat relativeAngle, const dFloat* const dir);
	Function NewtonUserJointAddGeneralRow (const NewtonJoint* const joint, const dFloat* const jacobian0, const dFloat* const jacobian1);
	Function NewtonUserJointSetRowMinimumFriction (const NewtonJoint* const joint, dFloat friction);
	Function NewtonUserJointSetRowMaximumFriction (const NewtonJoint* const joint, dFloat friction);
	Function NewtonUserJointSetRowAcceleration (const NewtonJoint* const joint, dFloat acceleration);
	Function NewtonUserJointSetRowSpringDamperAcceleration (const NewtonJoint* const joint, dFloat springK, dFloat springD);
	Function NewtonUserJointSetRowStiffness (const NewtonJoint* const joint, dFloat stiffness);
	NEWTON_API dFloat NewtonUserJointGetRowForce (const NewtonJoint* const joint, int row);
	

	// **********************************************************************************************
	//
	// Mesh joint functions
	//
	// **********************************************************************************************
	NEWTON_API NewtonMesh* NewtonMeshCreate(newtonWorld:Byte Ptr);
	NEWTON_API NewtonMesh* NewtonMeshCreateFromMesh(const NewtonMesh* const mesh);
	NEWTON_API NewtonMesh* NewtonMeshCreateFromCollision(const NewtonCollision* const collision);
	NEWTON_API NewtonMesh* NewtonMeshConvexHull (newtonWorld:Byte Ptr, int count, const dFloat* const vertexCloud, int strideInBytes, dFloat tolerance);
	NEWTON_API NewtonMesh* NewtonMeshCreatePlane (newtonWorld:Byte Ptr, const dFloat* const locationMatrix, dFloat witdth, dFloat breadth, int material, const dFloat* const textureMatrix0, const dFloat* const textureMatrix1);
	
	Function NewtonMeshDestroy(const NewtonMesh* const mesh);

	Function NewtonMeshCalculateOOBB(const NewtonMesh* const mesh, dFloat* const matrix, dFloat* const x, dFloat* const y, dFloat* const z);

	Function NewtonMeshCalculateVertexNormals(const NewtonMesh* const mesh, dFloat angleInRadians);
	Function NewtonMeshApplySphericalMapping(const NewtonMesh* const mesh, int material);
	Function NewtonMeshApplyBoxMapping(const NewtonMesh* const mesh, int front, int side, int top);
	Function NewtonMeshApplyCylindricalMapping(const NewtonMesh* const mesh, int cylinderMaterial, int capMaterial);
	
	Function NewtonMeshIsOpenMesh (const NewtonMesh* const mesh);
	Function NewtonMeshFixTJoints (const NewtonMesh* const mesh);


	Function NewtonMeshPolygonize (const NewtonMesh* const mesh);
	Function NewtonMeshTriangulate (const NewtonMesh* const mesh);
	NEWTON_API NewtonMesh* NewtonMeshUnion (const NewtonMesh* const mesh, const NewtonMesh* const clipper, const dFloat* const clipperMatrix);
	NEWTON_API NewtonMesh* NewtonMeshDifference (const NewtonMesh* const mesh, const NewtonMesh* const clipper, const dFloat* const clipperMatrix);
	NEWTON_API NewtonMesh* NewtonMeshIntersection (const NewtonMesh* const mesh, const NewtonMesh* const clipper, const dFloat* const clipperMatrix);
	Function NewtonMeshClip (const NewtonMesh* const mesh, const NewtonMesh* const clipper, const dFloat* const clipperMatrix, NewtonMesh** const topMesh, NewtonMesh** const bottomMesh);

	NEWTON_API NewtonMesh* NewtonMeshConvexDecomposition (const NewtonMesh* const mesh, int maxCount);
	NEWTON_API NewtonMesh* NewtonMeshVoronoiDecomposition (const NewtonMesh* const mesh, int pointCount, int pointStrideInBytes, const dFloat* const pointCloud, int internalMaterial, const dFloat* const textureMatrix);


	Function NewtonRemoveUnusedVertices(const NewtonMesh* const mesh, int* const vertexRemapTable);

	Function NewtonMeshBeginFace(const NewtonMesh* const mesh);
	Function NewtonMeshAddFace(const NewtonMesh* const mesh, int vertexCount, const dFloat* const vertex, int strideInBytes, int materialIndex);
	Function NewtonMeshEndFace(const NewtonMesh* const mesh);

	Function NewtonMeshBuildFromVertexListIndexList(const NewtonMesh* const mesh,
		int faceCount, const int* const faceIndexCount, const int* const faceMaterialIndex, 
		const dFloat* const vertex, int vertexStrideInBytes, const int* const vertexIndex,
		const dFloat* const normal, int normalStrideInBytes, const int* const normalIndex,
		const dFloat* const uv0, int uv0StrideInBytes, const int* const uv0Index,
		const dFloat* const uv1, int uv1StrideInBytes, const int* const uv1Index);



	Function NewtonMeshGetVertexStreams (const NewtonMesh* const mesh, 
												int vertexStrideInByte, dFloat* const vertex,
												int normalStrideInByte, dFloat* const normal,
												int uvStrideInByte0, dFloat* const uv0,
												int uvStrideInByte1, dFloat* const uv1);
	Function NewtonMeshGetIndirectVertexStreams(const NewtonMesh* const mesh, 
													   int vertexStrideInByte, dFloat* const vertex, int* const vertexIndices, int* const vertexCount,
													   int normalStrideInByte, dFloat* const normal, int* const normalIndices, int* const normalCount,
													   int uvStrideInByte0, dFloat* const uv0, int* const uvIndices0, int* const uvCount0,
													   int uvStrideInByte1, dFloat* const uv1, int* const uvIndices1, int* const uvCount1);
	Function* NewtonMeshBeginHandle (const NewtonMesh* const mesh); 
	Function NewtonMeshEndHandle (const NewtonMesh* const mesh, void* const handle); 
	Function NewtonMeshFirstMaterial (const NewtonMesh* const mesh, void* const handle); 
	Function NewtonMeshNextMaterial (const NewtonMesh* const mesh, void* const handle, int materialId); 
	Function NewtonMeshMaterialGetMaterial (const NewtonMesh* const mesh, void* const handle, int materialId); 
	Function NewtonMeshMaterialGetIndexCount (const NewtonMesh* const mesh, void* const handle, int materialId); 
	Function NewtonMeshMaterialGetIndexStream (const NewtonMesh* const mesh, void* const handle, int materialId, int* const index); 
	Function NewtonMeshMaterialGetIndexStreamShort (const NewtonMesh* const mesh, void* const handle, int materialId, short int* const index); 

	NEWTON_API NewtonMesh* NewtonMeshCreateFirstSingleSegment (const NewtonMesh* const mesh); 
	NEWTON_API NewtonMesh* NewtonMeshCreateNextSingleSegment (const NewtonMesh* const mesh, const NewtonMesh* const segment); 

	NEWTON_API NewtonMesh* NewtonMeshCreateFirstLayer (const NewtonMesh* const mesh); 
	NEWTON_API NewtonMesh* NewtonMeshCreateNextLayer (const NewtonMesh* const mesh, const NewtonMesh* const segment); 


	Function NewtonMeshGetTotalFaceCount (const NewtonMesh* const mesh); 
	Function NewtonMeshGetTotalIndexCount (const NewtonMesh* const mesh); 
	Function NewtonMeshGetFaces (const NewtonMesh* const mesh, int* const faceIndexCount, int* const faceMaterial, void** const faceIndices); 


	Function NewtonMeshGetPointCount (const NewtonMesh* const mesh); 
	Function NewtonMeshGetPointStrideInByte (const NewtonMesh* const mesh); 
	NEWTON_API dFloat64* NewtonMeshGetPointArray (const NewtonMesh* const mesh); 
	NEWTON_API dFloat64* NewtonMeshGetNormalArray (const NewtonMesh* const mesh); 
	NEWTON_API dFloat64* NewtonMeshGetUV0Array (const NewtonMesh* const mesh); 
	NEWTON_API dFloat64* NewtonMeshGetUV1Array (const NewtonMesh* const mesh); 

	Function NewtonMeshGetVertexCount (const NewtonMesh* const mesh); 
	Function NewtonMeshGetVertexStrideInByte (const NewtonMesh* const mesh); 
	NEWTON_API dFloat64* NewtonMeshGetVertexArray (const NewtonMesh* const mesh); 


	Function* NewtonMeshGetFirstVertex (const NewtonMesh* const mesh);
	Function* NewtonMeshGetNextVertex (const NewtonMesh* const mesh, const void* const vertex);
	Function NewtonMeshGetVertexIndex (const NewtonMesh* const mesh, const void* const vertex);

	Function* NewtonMeshGetFirstPoint (const NewtonMesh* const mesh);
	Function* NewtonMeshGetNextPoint (const NewtonMesh* const mesh, const void* const point);
	Function NewtonMeshGetPointIndex (const NewtonMesh* const mesh, const void* const point);
	Function NewtonMeshGetVertexIndexFromPoint (const NewtonMesh* const mesh, const void* const point);
	

	Function* NewtonMeshGetFirstEdge (const NewtonMesh* const mesh);
	Function* NewtonMeshGetNextEdge (const NewtonMesh* const mesh, const void* const edge);
	Function NewtonMeshGetEdgeIndices (const NewtonMesh* const mesh, const void* const edge, int* const v0, int* const v1);
	//Function NewtonMeshGetEdgePointIndices (const NewtonMesh* const mesh, const void* const edge, int* const v0, int* const v1);

	Function* NewtonMeshGetFirstFace (const NewtonMesh* const mesh);
	Function* NewtonMeshGetNextFace (const NewtonMesh* const mesh, const void* const face);
	Function NewtonMeshIsFaceOpen (const NewtonMesh* const mesh, const void* const face);
	Function NewtonMeshGetFaceMaterial (const NewtonMesh* const mesh, const void* const face);
	Function NewtonMeshGetFaceIndexCount (const NewtonMesh* const mesh, const void* const face);
	Function NewtonMeshGetFaceIndices (const NewtonMesh* const mesh, const void* const face, int* const indices);
	Function NewtonMeshGetFacePointIndices (const NewtonMesh* const mesh, const void* const face, int* const indices);
End Rem
End Extern