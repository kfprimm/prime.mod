
Strict

Rem
	bbdoc: Netwon Module for BlitzMax
End Rem
Module Prime.Newton
ModuleInfo "Author: Kevin Primm"
ModuleInfo "License: MIT"
ModuleInfo "Version: 0.01"
ModuleInfo "Newton Version: 3.11"

?Not Win32
ModuleInfo "CC_OPTS: -Wall -Wno-strict-aliasing -D_POSIX_VER -m32 -O2 -fpic -g -msse -msse3 -mfpmath=sse -ffloat-store -ffast-math -freciprocal-math -funsafe-math-optimizations -fsingle-precision-constant"
?Win32
ModuleInfo "CC_OPTS: -Wall -Wno-strict-aliasing -DPTW32_BUILD -DPTW32_STATIC_LIB -D_NEWTON_STATIC_LIB -D_MINGW_32_VER -m32 -O2 -g -msse -msse3 -mfpmath=sse -ffloat-store -ffast-math -freciprocal-math -funsafe-math-optimizations -fsingle-precision-constant"
?

Import "source.bmx"

'/* Copyright (c) <2003-2011> <Julio Jerez, Newton Game Dynamics>
'* 
'* This software is provided 'as-is', without any express or implied
'* warranty. In no event will the authors be held liable for any damages
'* arising from the use of this software.
'* 
'* Permission is granted to anyone to use this software for any purpose,
'* including commercial applications, and to alter it and redistribute it
'* freely, subject to the following restrictions:
'* 
'* 1. The origin of this software must not be misrepresented; you must not
'* claim that you wrote the original software. If you use this software
'* in a product, an acknowledgment in the product documentation would be
'* appreciated but is not required.
'* 
'* 2. Altered source versions must be plainly marked as such, and must not be
'* misrepresented as being the original software.
'* 
'* 3. This notice may not be removed or altered from any source distribution.
'*/
'
'
'#define NEWTON_MAJOR_VERSION 3 
'#define NEWTON_MINOR_VERSION 11 
'
'
'
'#ifdef _NEWTON_STATIC_LIB
'	#define NEWTON_API
'#else 
'	#ifdef _NEWTON_BUILD_DLL
'		#ifdef _WIN32
'			#define NEWTON_API __declspec (dllexport)
'		#else
'			#define NEWTON_API __attribute__ ((visibility("default")))
'		#endif
'	#else
'		#ifdef _WIN32
'			#define NEWTON_API __declspec (dllimport)
'		#else
'			#define NEWTON_API
'		#endif
'	#endif
'#endif
'
'
'#ifndef dLong
'	#define dLong long long		
'#endif
'
'#ifndef dFloat
'	#ifdef __USE_DOUBLE_PRECISION__
'		#define dFloat double
'	#else
'		#define dFloat float
'	#endif
'#endif
'
'#ifndef dFloat64
'	#define dFloat64 double
'#endif
'
'
'#ifdef __cplusplus 
'extern "C" {
'#endif
'
'	#define NEWTON_PROFILER_WORLD_UPDATE					0
'
'	#define NEWTON_PROFILER_COLLISION_UPDATE				1
'	#define NEWTON_PROFILER_COLLISION_UPDATE_BROAD_PHASE	2
'	#define NEWTON_PROFILER_COLLISION_UPDATE_NARROW_PHASE	3
'
'	#define NEWTON_PROFILER_DYNAMICS_UPDATE					4
'	#define NEWTON_PROFILER_DYNAMICS_CONSTRAINT_GRAPH		5
'	#define NEWTON_PROFILER_DYNAMICS_SOLVE_CONSTRAINT_GRAPH	6
'
'	#define NEWTON_PROFILER_FORCE_CALLBACK_UPDATE			7
'	#define NEWTON_PROFILER_SOFT_BODY_CALLBACK_UPDATE		8
'
'	#define NEWTON_PRE_LISTERNER_CALLBACK_UPDATE			9
'	#define NEWTON_POST_LISTERNER_CALLBACK_UPDATE			10
'
'
'
'	#define NEWTON_DYNAMIC_BODY								0
'	#define NEWTON_KINEMATIC_BODY							1
'	#define NEWTON_DEFORMABLE_BODY							2
'
'	#define SERIALIZE_ID_SPHERE								0
'	#define SERIALIZE_ID_CAPSULE							1
'	#define SERIALIZE_ID_CHAMFERCYLINDER					2
'	#define SERIALIZE_ID_TAPEREDCAPSULE						3
'	#define SERIALIZE_ID_CYLINDER							4
'	#define SERIALIZE_ID_TAPEREDCYLINDER					5
'	#define SERIALIZE_ID_BOX								6	
'	#define SERIALIZE_ID_CONE								7
'	#define SERIALIZE_ID_CONVEXHULL							8
'	#define SERIALIZE_ID_NULL								9
'	#define SERIALIZE_ID_COMPOUND							10
'	#define SERIALIZE_ID_TREE								11
'	#define SERIALIZE_ID_HEIGHTFIELD						12
'	#define SERIALIZE_ID_CLOTH_PATCH						13
'	#define SERIALIZE_ID_DEFORMABLE_SOLID					14
'	#define SERIALIZE_ID_USERMESH							15
'	#define SERIALIZE_ID_SCENE								16
'	#define SERIALIZE_ID_COMPOUND_BREAKABLE					17
'
'	typedef struct NewtonMesh{} NewtonMesh;
'	typedef struct NewtonBody{} NewtonBody;
'	typedef struct NewtonWorld{} NewtonWorld;
'	typedef struct NewtonJoint{} NewtonJoint;
'	typedef struct NewtonMaterial{} NewtonMaterial;
'	typedef struct NewtonCollision{} NewtonCollision;
'	typedef struct NewtonDeformableMeshSegment{} NewtonDeformableMeshSegment;
'	typedef struct NewtonBreakableComponentMesh{} NewtonBreakableComponentMesh;
'
'
'	typedef struct NewtonBoxParam
'	{
'		dFloat m_x;
'		dFloat m_y;
'		dFloat m_z;
'	} NewtonBoxParam;
'
'	typedef struct NewtonSphereParam
'	{
'		dFloat m_radio;
'	} NewtonSphereParam;
'
'	typedef struct NewtonCylinderParam
'	{
'		dFloat m_radio;
'		dFloat m_height;
'	} NewtonCylinderParam;
'
'	typedef struct NewtonCapsuleParam
'	{
'		dFloat m_radio;
'		dFloat m_height;
'	} NewtonCapsuleParam;
'
'	typedef struct NewtonConeParam
'	{
'		dFloat m_radio;
'		dFloat m_height;
'	} NewtonConeParam;
'
'	typedef struct NewtonTaperedCapsuleParam
'	{
'		dFloat m_radio0;
'		dFloat m_radio1;
'		dFloat m_height;
'	} NewtonTaperedCapsuleParam;
'
'
'	typedef struct NewtonTaperedCylinderParam
'	{
'		dFloat m_radio0;
'		dFloat m_radio1;
'		dFloat m_height;
'	} NewtonTaperedCylinderParam;
'
'	typedef struct NewtonChamferCylinderParam
'	{
'		dFloat m_radio;
'		dFloat m_height;
'	} NewtonChamferCylinderParam;
'
'	typedef struct NewtonConvexHullParam
'	{
'		int m_vertexCount;
'		int m_vertexStrideInBytes;
'		int m_faceCount;
'		dFloat* m_vertex;
'	} NewtonConvexHullParam;
'
'	typedef struct NewtonCompoundCollisionParam
'	{
'		int m_chidrenCount;
'	} NewtonCompoundCollisionParam;
'
'	typedef struct NewtonCollisionTreeParam
'	{
'		int m_vertexCount;
'		int m_indexCount;
'	} NewtonCollisionTreeParam;
'
'	typedef struct NewtonDeformableMeshParam
'	{
'		int m_vertexCount;
'		int m_triangleCount;
'		int m_vrtexStrideInBytes;
'		unsigned short *m_indexList;
'		dFloat *m_vertexList;
'	} NewtonDeformableMeshParam;
'
'	typedef struct NewtonHeightFieldCollisionParam
'	{
'		int m_width;
'		int m_height;
'		int m_gridsDiagonals;
'		int m_elevationDataType;	// 0 = 32 bit floats, 1 = unsigned 16 bit integers
'		dFloat m_horizonalScale;
'		dFloat m_verticalScale;
'		void* m_elevation;
'		char* m_atributes;
'	} NewtonHeightFieldCollisionParam;
'
'	typedef struct NewtonSceneCollisionParam
'	{
'		int m_childrenProxyCount;
'	} NewtonSceneCollisionParam;
'
'	typedef struct NewtonCollisionInfoRecord
'	{
'		dFloat m_offsetMatrix[4][4];
'		int m_collisionType;				// tag id to identify the collision primitive
'		int m_collisionUserID;				
'		
'		union {
'			NewtonBoxParam m_box;									
'			NewtonConeParam m_cone;
'			NewtonSphereParam m_sphere;
'			NewtonCapsuleParam m_capsule;
'			NewtonCylinderParam m_cylinder;
'			NewtonTaperedCapsuleParam m_taperedCapsule;
'			NewtonTaperedCylinderParam m_taperedCylinder;
'			NewtonChamferCylinderParam m_chamferCylinder;
'			NewtonConvexHullParam m_convexHull;
'			NewtonDeformableMeshParam m_deformableMesh;
'			NewtonCompoundCollisionParam m_compoundCollision;
'			NewtonCollisionTreeParam m_collisionTree;
'			NewtonHeightFieldCollisionParam m_heightField;
'			NewtonSceneCollisionParam m_sceneCollision;
'			dFloat m_paramArray[64];		    // user define collision can use this to store information
'		};
'	} NewtonCollisionInfoRecord;
'
'	typedef struct NewtonJointRecord
'	{
'		dFloat m_attachmenMatrix_0[4][4];
'		dFloat m_attachmenMatrix_1[4][4];
'		dFloat m_minLinearDof[3];
'		dFloat m_maxLinearDof[3];
'		dFloat m_minAngularDof[3];
'		dFloat m_maxAngularDof[3];
'		const NewtonBody* m_attachBody_0;
'		const NewtonBody* m_attachBody_1;
'		dFloat m_extraParameters[64];
'		int	m_bodiesCollisionOn;
'		char m_descriptionType[128];
'	} NewtonJointRecord;
'
'	typedef struct NewtonUserMeshCollisionCollideDesc
'	{
'		dFloat m_boxP0[4];							// lower bounding box of intersection query in local space
'		dFloat m_boxP1[4];							// upper bounding box of intersection query in local space
'		dFloat m_boxDistanceTravel[4];				// max distance that box bpxP0 and boxP1 can travel on this timestep, used this for continue collision mode.
'		int m_threadNumber;							// current thread executing this query
'		int	m_faceCount;                        	// the application should set here how many polygons intersect the query box
'		int m_vertexStrideInBytes;              	// the application should set here the size of each vertex
'		dFloat m_skinThickness;                     // this is the minimum skin separation specified by the material between these two colliding shapes
'		void* m_userData;                       	// user data passed to the collision geometry at creation time
'
'		NewtonBody* m_objBody;                  	// pointer to the colliding body
'		NewtonBody* m_polySoupBody;             	// pointer to the rigid body owner of this collision tree 
'		NewtonCollision* m_objCollision;			// collision shape of the colliding body, (no necessarily the collision of m_objBody)
'		NewtonCollision* m_polySoupCollision;		// collision shape of the collision tree, (no necessarily the collision of m_polySoupBody)
'
'		dFloat* m_vertex;                       	// the application should set here the pointer to the global vertex of the mesh. 
'		int* m_faceIndexCount;                  	// the application should set here the pointer to the vertex count of each face.
'		int* m_faceVertexIndex;                 	// the application should set here the pointer index array for each vertex on a face.
'													// the format of a face is I0, I1, I2, I3, ..., M, N, E0, E1, E2, ..., A
'                                                	// I0, I1, I2, .. are the indices to the vertex, relative to m_vertex pointer
'		                                        	// M is the index to the material sub shape id
'													// N in the index to the vertex normal relative to m_vertex pointer
'													// E0, E1, E2, ... are the indices of the the face normal that is shared to that face edge, when the edge does not share a face normal then the edge index is set to index N, which the index to the face normal    
'													// A is and estimate of the largest diagonal of the face, this used internally as a hint to improve floating point accuracy and algorithm performance. 
'	} NewtonUserMeshCollisionCollideDesc;
'
'	typedef struct NewtonWorldConvexCastReturnInfo
'	{
'		dFloat m_point[4];						// collision point in global space
'		dFloat m_normal[4];						// surface normal at collision point in global space
'		dFloat m_normalOnHitPoint[4];           // surface normal at the surface of the hit body, 
'												// is the same as the normal calculated by a ray cast hitting the body at the hit point
'		dLong m_contactID;						// collision ID at contact point
'		const NewtonBody* m_hitBody;			// body hit at contact point
'		dFloat m_penetration;                   // contact penetration at collision point
'	} NewtonWorldConvexCastReturnInfo;
'	
'	typedef struct NewtonUserMeshCollisionRayHitDesc
'	{
'		dFloat m_p0[4];							// ray origin in collision local space
'		dFloat m_p1[4];                         // ray destination in collision local space   
'		dFloat m_normalOut[4];					// copy here the normal at the ray intersection
'		dLong m_userIdOut;						// copy here a user defined id for further feedback  
'		void* m_userData;                       // user data passed to the collision geometry at creation time
'	} NewtonUserMeshCollisionRayHitDesc;
'
'	typedef struct NewtonHingeSliderUpdateDesc
'	{
'		dFloat m_accel;
'		dFloat m_minFriction;
'		dFloat m_maxFriction;
'		dFloat m_timestep;
'	} NewtonHingeSliderUpdateDesc;
'
'	typedef struct NewtonClothPatchMaterial
'	{
'		dFloat m_damper;
'		dFloat m_stiffness;
'	} NewtonClothPatchMaterial;
'
'
'	// Newton callback functions
'	typedef void* (*NewtonAllocMemory) (int sizeInBytes);
'	typedef void (*NewtonFreeMemory) (void* const ptr, int sizeInBytes);
'
'
'	typedef void (*NewtonWorldDestructorCallback) (const NewtonWorld* const world);
'	typedef void (*NewtonWorldUpdateListenerCallback) (const NewtonWorld* const world, void* const listenerUserData, dFloat timestep);
'	typedef void (*NewtonWorldDestroyListenerCallback) (const NewtonWorld* const world, void* const listenerUserData);
'
'	typedef unsigned (*NewtonGetTicksCountCallback) ();
'
'	typedef void (*NewtonSerializeCallback) (void* const serializeHandle, const void* const buffer, int size);
'	typedef void (*NewtonDeserializeCallback) (void* const serializeHandle, void* const buffer, int size);
'	
'	typedef void (*NewtonOnBodySerializationCallback) (NewtonBody* const body, NewtonSerializeCallback function, void* const serializeHandle);
'	typedef void (*NewtonOnBodyDeserializationCallback) (NewtonBody* const body, NewtonDeserializeCallback function, void* const serializeHandle);
'
'	typedef void (*NewtonOnUserCollisionSerializationCallback) (void* const userData, NewtonSerializeCallback function, void* const serializeHandle);
'
'	
'	// user collision callbacks	
'	typedef void (*NewtonUserMeshCollisionDestroyCallback) (void* const userData);
'	typedef void (*NewtonUserMeshCollisionCollideCallback) (NewtonUserMeshCollisionCollideDesc* const collideDescData);
'	typedef dFloat (*NewtonUserMeshCollisionRayHitCallback) (NewtonUserMeshCollisionRayHitDesc* const lineDescData);
'	typedef void (*NewtonUserMeshCollisionGetCollisionInfo) (void* const userData, NewtonCollisionInfoRecord* const infoRecord);
'	typedef int (*NewtonUserMeshCollisionAABBTest) (void* const userData, const dFloat* const boxP0, const dFloat* const boxP1);
'	typedef int (*NewtonUserMeshCollisionGetFacesInAABB) (void* const userData, const dFloat* const p0, const dFloat* const p1,
'														   const dFloat** const vertexArray, int* const vertexCount, int* const vertexStrideInBytes, 
'		                                                   const int* const indexList, int maxIndexCount, const int* const userDataList);
'
'	typedef int (*NewtonTreeCollisionFaceCallback) (void* const context, const dFloat* const polygon, int strideInBytes, const int* const indexArray, int indexCount);
'
'	typedef dFloat (*NewtonCollisionTreeRayCastCallback) (body:Byte Ptr, const NewtonCollision* const treeCollision, dFloat intersection, dFloat* const normal, int faceId, void* const usedData);
'	typedef dFloat (*NewtonHeightFieldRayCastCallback) (body:Byte Ptr, const NewtonCollision* const heightFieldCollision, dFloat intersection, int row, int col, dFloat* const normal, int faceId, void* const usedData);
'
'
'	typedef void (*NewtonCollisionCopyConstructionCallback) (newtonWorld:Byte Ptr, NewtonCollision* const collision, const NewtonCollision* const sourceCollision);
'	typedef void (*NewtonCollisionDestructorCallback) (newtonWorld:Byte Ptr, collision:Byte Ptr);
'
'	// collision tree call back (obsoleted no recommended)
'	typedef void (*NewtonTreeCollisionCallback) (body:Byte PtrWithTreeCollision, body:Byte Ptr, int faceID, 
'												 int vertexCount, const dFloat* const vertex, int vertexStrideInBytes); 
'
'	
'
'	typedef void (*NewtonBodyDestructor) (body:Byte Ptr);
'	typedef void (*NewtonApplyForceAndTorque) (body:Byte Ptr, dFloat timestep, int threadIndex);
'	typedef void (*NewtonSetTransform) (body:Byte Ptr, matrix:Float Ptr, int threadIndex);
'
'	typedef int (*NewtonIslandUpdate) (newtonWorld:Byte Ptr, const void* islandHandle, int bodyCount);
'	typedef void (*NewtonDestroyBodyByExeciveForce) (body:Byte Ptr, const NewtonJoint* const contact);
'	
'	typedef int (*NewtonCollisionCompoundBreakableCallback) (NewtonMesh* const mesh, void* const userData, dFloat* const planeMatrixOut);
'	typedef unsigned (*NewtonWorldRayPrefilterCallback)(body:Byte Ptr, collision:Byte Ptr, void* const userData);
'	typedef dFloat (*NewtonWorldRayFilterCallback)(body:Byte Ptr, const NewtonCollision* const shapeHit, const dFloat* const hitContact, const dFloat* const hitNormal, dLong collisionID, void* const userData, dFloat intersectParam);
'	
'
'	typedef void (*NewtonContactsProcess) (const NewtonJoint* const contact, dFloat timestep, int threadIndex);
'	typedef int  (*NewtonOnAABBOverlap) (const NewtonMaterial* const material, body:Byte Ptr0, body:Byte Ptr1, int threadIndex);
'	typedef int  (*NewtonOnCompoundSubCollisionAABBOverlap) (const NewtonMaterial* const material, body:Byte Ptr0, const void* const collsionNode0, body:Byte Ptr1, const void* const collsionNode1, int threadIndex);
'
'	typedef int (*NewtonBodyIterator) (body:Byte Ptr, void* const userData);
'	typedef void (*NewtonJointIterator) (const NewtonJoint* const joint, void* const userData);
'	typedef void (*NewtonCollisionIterator) (void* const userData, int vertexCount, const dFloat* const faceArray, int faceId);
'
'	typedef void (*NewtonBallCallback) (const NewtonJoint* const ball, dFloat timestep);
'	typedef unsigned (*NewtonHingeCallback) (const NewtonJoint* const hinge, NewtonHingeSliderUpdateDesc* const desc);
'	typedef unsigned (*NewtonSliderCallback) (const NewtonJoint* const slider, NewtonHingeSliderUpdateDesc* const desc);
'	typedef unsigned (*NewtonUniversalCallback) (const NewtonJoint* const universal, NewtonHingeSliderUpdateDesc* const desc);
'	typedef unsigned (*NewtonCorkscrewCallback) (const NewtonJoint* const corkscrew, NewtonHingeSliderUpdateDesc* const desc);
'
'	typedef void (*NewtonUserBilateralCallback) (const NewtonJoint* const userJoint, dFloat timestep, int threadIndex);
'	typedef void (*NewtonUserBilateralGetInfoCallback) (const NewtonJoint* const userJoint, NewtonJointRecord* const info);
'
'	typedef void (*NewtonConstraintDestructor) (const NewtonJoint* const me);
'
'	typedef void (*NewtonJobTask) (NewtonWorld* const world, void* const userData, int threadIndex);
'
'	typedef bool (*NewtonReportProgress) (dFloat normalizedProgressPercent);
'
'
Extern
'	// **********************************************************************************************
'	//
'	// world control functions
'	//
'	// **********************************************************************************************
'	NEWTON_API int NewtonWorldGetVersion ();
'	NEWTON_API int NewtonWorldFloatSize ();
'
'	NEWTON_API int NewtonGetMemoryUsed ();
'	NewtonSetMemorySystem (NewtonAllocMemory malloc, NewtonFreeMemory free);
'
	Function NewtonCreate:Byte Ptr ()
	Function NewtonDestroy (newtonWorld:Byte Ptr)
	Function NewtonDestroyAllBodies (newtonWorld:Byte Ptr)
'
'	NEWTON_API void* NewtonAlloc (int sizeInBytes);
'	NewtonFree (void* const ptr);
'
'	//	NewtonSetPlatformArchitecture (newtonWorld:Byte Ptr, int mode);
'	//	NEWTON_API int NewtonGetPlatformArchitecture(newtonWorld:Byte Ptr, char* description);
'
'	NEWTON_API int NewtonEnumrateDevices (newtonWorld:Byte Ptr);
'	NEWTON_API int NewtonGetCurrentDevice (newtonWorld:Byte Ptr);
'	NewtonSetCurrentDevice (newtonWorld:Byte Ptr, int deviceIndex);
'	NewtonGetDeviceString (newtonWorld:Byte Ptr, int deviceIndex, char* const vendorString, int maxSize);
'
'	NewtonInvalidateCache (newtonWorld:Byte Ptr);
	Function NewtonSetSolverModel (newtonWorld:Byte Ptr, model)
'
'	NewtonSetMultiThreadSolverOnSingleIsland (newtonWorld:Byte Ptr, int mode);
'	NEWTON_API int NewtonGetMultiThreadSolverOnSingleIsland (newtonWorld:Byte Ptr);
'
'	NewtonSetPerformanceClock (newtonWorld:Byte Ptr, NewtonGetTicksCountCallback callback);
'	NEWTON_API unsigned NewtonReadPerformanceTicks (newtonWorld:Byte Ptr, unsigned performanceEntry);
'
'	NEWTON_API int NewtonGetBroadphaseAlgorithm (newtonWorld:Byte Ptr);
'	NewtonSelectBroadphaseAlgorithm (newtonWorld:Byte Ptr, int algorithmType);
'	
	Function NewtonUpdate (newtonWorld:Byte Ptr, timestep#)
	Function NewtonUpdateAsync (newtonWorld:Byte Ptr, timestep#)
'	NewtonWaitForUpdateToFinish (newtonWorld:Byte Ptr);
'
'	NewtonSerializeToFile (newtonWorld:Byte Ptr, const char* const filename);
'	NewtonSerializeBodyArray (newtonWorld:Byte Ptr, NewtonBody** const bodyArray, int bodyCount, NewtonOnBodySerializationCallback serializeBody, NewtonSerializeCallback serializeFunction, void* const serializeHandle);
'	NewtonDeserializeBodyArray (newtonWorld:Byte Ptr, NewtonOnBodyDeserializationCallback deserializeBody, NewtonDeserializeCallback serializeFunction, void* const serializeHandle);
'
'
'	NEWTON_API unsigned NewtonReadThreadPerformanceTicks (const NewtonWorld* newtonWorld, unsigned threadIndex);
'
'	// multi threading interface 
'	NewtonWorldCriticalSectionLock (newtonWorld:Byte Ptr, int threadIndex);
'	NewtonWorldCriticalSectionUnlock (newtonWorld:Byte Ptr);
'	NewtonSetThreadsCount (newtonWorld:Byte Ptr, int threads);
'	NEWTON_API int NewtonGetThreadsCount(newtonWorld:Byte Ptr);
'	NEWTON_API int NewtonGetMaxThreadsCount(newtonWorld:Byte Ptr);
'	NewtonDispachThreadJob(newtonWorld:Byte Ptr, NewtonJobTask task, void* const usedData);
'	NewtonSyncThreadJobs(newtonWorld:Byte Ptr);
'
'	// atomic operations
'	NEWTON_API int NewtonAtomicAdd (int* const ptr, int value);
'	NEWTON_API int NewtonAtomicSwap (int* const ptr, int value);
'	NewtonYield ();
'	
'
'	NewtonSetFrictionModel (newtonWorld:Byte Ptr, int model);
'	NewtonSetMinimumFrameRate (newtonWorld:Byte Ptr, dFloat frameRate);
'	NewtonSetIslandUpdateEvent (newtonWorld:Byte Ptr, NewtonIslandUpdate islandUpdate); 
'	NewtonSetDestroyBodyByExeciveForce (newtonWorld:Byte Ptr, NewtonDestroyBodyByExeciveForce callback); 
'
'//	NewtonWorldForEachBodyDo (newtonWorld:Byte Ptr, NewtonBodyIterator callback);
'	NewtonWorldForEachJointDo (newtonWorld:Byte Ptr, NewtonJointIterator callback, void* const userData);
'	NewtonWorldForEachBodyInAABBDo (newtonWorld:Byte Ptr, const dFloat* const p0, const dFloat* const p1, NewtonBodyIterator callback, void* const userData);
'
'	
'	NewtonWorldSetUserData (newtonWorld:Byte Ptr, void* const userData);
'	NEWTON_API void* NewtonWorldGetUserData (newtonWorld:Byte Ptr);
'
'
'	NEWTON_API void* NewtonWorldGetListenerUserData (newtonWorld:Byte Ptr, void* const listener);
'	NEWTON_API void* NewtonWorldGetPreListener (newtonWorld:Byte Ptr, const char* const nameId);
'	NEWTON_API void* NewtonWorldAddPreListener (newtonWorld:Byte Ptr, const char* const nameId, void* const listenerUserData, NewtonWorldUpdateListenerCallback update, NewtonWorldDestroyListenerCallback destroy);
'	
'	NEWTON_API void*  NewtonWorldGetPostListener (newtonWorld:Byte Ptr, const char* const nameId);
'	NEWTON_API void* NewtonWorldAddPostListener (newtonWorld:Byte Ptr, const char* const nameId, void* const listenerUserData, NewtonWorldUpdateListenerCallback update, NewtonWorldDestroyListenerCallback destroy);
'
'	NewtonWorldSetDestructorCallback (newtonWorld:Byte Ptr, NewtonWorldDestructorCallback destructor);
'	NEWTON_API NewtonWorldDestructorCallback NewtonWorldGetDestructorCallback (newtonWorld:Byte Ptr);
'
'	NewtonWorldSetCollisionConstructorDestuctorCallback (newtonWorld:Byte Ptr, NewtonCollisionCopyConstructionCallback constructor, NewtonCollisionDestructorCallback destructor);
'
'	NewtonWorldRayCast (newtonWorld:Byte Ptr, const dFloat* const p0, const dFloat* const p1, NewtonWorldRayFilterCallback filter, void* const userData, NewtonWorldRayPrefilterCallback prefilter, int threadIndex);
'	NewtonWorldConvexRayCast (newtonWorld:Byte Ptr, const NewtonCollision* const shape, matrix:Float Ptr, const dFloat* const p1, NewtonWorldRayFilterCallback filter, void* const userData, NewtonWorldRayPrefilterCallback prefilter, int threadIndex);
'
'	NEWTON_API int NewtonWorldCollide (newtonWorld:Byte Ptr, matrix:Float Ptr, const NewtonCollision* const shape, void* const userData,  
'									   NewtonWorldRayPrefilterCallback prefilter, NewtonWorldConvexCastReturnInfo* const info, int maxContactsCount, int threadIndex);
'	NEWTON_API int NewtonWorldConvexCast (newtonWorld:Byte Ptr, matrix:Float Ptr, const dFloat* const target, const NewtonCollision* const shape, dFloat* const hitParam, void* const userData,  
'										  NewtonWorldRayPrefilterCallback prefilter, NewtonWorldConvexCastReturnInfo* const info, int maxContactsCount, int threadIndex);
'
'
'	// world utility functions
'	NEWTON_API int NewtonWorldGetBodyCount(newtonWorld:Byte Ptr);
'	NEWTON_API int NewtonWorldGetConstraintCount(newtonWorld:Byte Ptr);
'
'
'	// **********************************************************************************************
'	//
'	// Simulation islands 
'	//
'	// **********************************************************************************************
'	NEWTON_API NewtonBody* NewtonIslandGetBody (const void* const island, int bodyIndex);
'	NewtonIslandGetBodyAABB (const void* const island, int bodyIndex, dFloat* const p0, dFloat* const p1);
'
'	// **********************************************************************************************
'	//
'	// Physics Material Section
'	//
'	// **********************************************************************************************
'	NEWTON_API int NewtonMaterialCreateGroupID(newtonWorld:Byte Ptr);
'	NEWTON_API int NewtonMaterialGetDefaultGroupID(newtonWorld:Byte Ptr);
'	NewtonMaterialDestroyAllGroupID(newtonWorld:Byte Ptr);
'
'	// material definitions that can not be overwritten in function callback
'	NEWTON_API void* NewtonMaterialGetUserData (newtonWorld:Byte Ptr, int id0, int id1);
'	NewtonMaterialSetSurfaceThickness (newtonWorld:Byte Ptr, int id0, int id1, dFloat thickness);
'
'//	deprecated, not longer continue collision is set on the material  	
'//	NewtonMaterialSetContinuousCollisionMode (newtonWorld:Byte Ptr, int id0, int id1, int state);
'	NewtonMaterialSetCollisionCallback (newtonWorld:Byte Ptr, int id0, int id1, void* const userData, NewtonOnAABBOverlap aabbOverlap, NewtonContactsProcess process);
'	NewtonMaterialSetCompoundCollisionCallback(newtonWorld:Byte Ptr, int id0, int id1, NewtonOnCompoundSubCollisionAABBOverlap compoundAabbOverlap);
'
'	NewtonMaterialSetDefaultSoftness (newtonWorld:Byte Ptr, int id0, int id1, dFloat value);
'	NewtonMaterialSetDefaultElasticity (newtonWorld:Byte Ptr, int id0, int id1, dFloat elasticCoef);
'	NewtonMaterialSetDefaultCollidable (newtonWorld:Byte Ptr, int id0, int id1, int state);
'	NewtonMaterialSetDefaultFriction (newtonWorld:Byte Ptr, int id0, int id1, dFloat staticFriction, dFloat kineticFriction);
'
'	NEWTON_API NewtonMaterial* NewtonWorldGetFirstMaterial (newtonWorld:Byte Ptr);
'	NEWTON_API NewtonMaterial* NewtonWorldGetNextMaterial (newtonWorld:Byte Ptr, const NewtonMaterial* const material);
'
'	NEWTON_API NewtonBody* NewtonWorldGetFirstBody (newtonWorld:Byte Ptr);
'	NEWTON_API NewtonBody* NewtonWorldGetNextBody (newtonWorld:Byte Ptr, const NewtonBody* const curBody);
'
'
'	// **********************************************************************************************
'	//
'	// Physics Contact control functions
'	//
'	// **********************************************************************************************
'	*NewtonMaterialGetMaterialPairUserData (const NewtonMaterial* const material);
'	NEWTON_API unsigned NewtonMaterialGetContactFaceAttribute (const NewtonMaterial* const material);
'	NEWTON_API NewtonCollision* NewtonMaterialGetBodyCollidingShape (const NewtonMaterial* const material, body:Byte Ptr);
'	NEWTON_API dFloat NewtonMaterialGetContactNormalSpeed (const NewtonMaterial* const material);
'	NewtonMaterialGetContactForce (const NewtonMaterial* const material, NewtonBody* const body, dFloat* const force);
'	NewtonMaterialGetContactPositionAndNormal (const NewtonMaterial* const material, NewtonBody* const body, dFloat* const posit, dFloat* const normal);
'	NewtonMaterialGetContactTangentDirections (const NewtonMaterial* const material, NewtonBody* const body, dFloat* const dir0, dFloat* const dir1);
'	NEWTON_API dFloat NewtonMaterialGetContactTangentSpeed (const NewtonMaterial* const material, int index);
'
'	NEWTON_API dFloat NewtonMaterialGetContactMaxNormalImpact (const NewtonMaterial* const material);
'	NEWTON_API dFloat NewtonMaterialGetContactMaxTangentImpact (const NewtonMaterial* const material, int index);
'	
'	NewtonMaterialSetContactSoftness (const NewtonMaterial* const material, dFloat softness);
'	NewtonMaterialSetContactElasticity (const NewtonMaterial* const material, dFloat restitution);
'	NewtonMaterialSetContactFrictionState (const NewtonMaterial* const material, int state, int index);
'	NewtonMaterialSetContactFrictionCoef (const NewtonMaterial* const material, dFloat staticFrictionCoef, dFloat kineticFrictionCoef, int index);
'	
'	NewtonMaterialSetContactNormalAcceleration (const NewtonMaterial* const material, dFloat accel);
'	NewtonMaterialSetContactNormalDirection (const NewtonMaterial* const material, const dFloat* const directionVector);
'
'	NewtonMaterialSetContactTangentAcceleration (const NewtonMaterial* const material, dFloat accel, int index);
'	NewtonMaterialContactRotateTangentDirections (const NewtonMaterial* const material, const dFloat* const directionVector);
'
'	
'
'	// **********************************************************************************************
'	//
'	// convex collision primitives creation functions
'	//
'	// **********************************************************************************************
	Function NewtonCreateNull:Byte Ptr (newtonWorld:Byte Ptr)
	Function NewtonCreateSphere:Byte Ptr (newtonWorld:Byte Ptr, radius#, shapeID, offsetMatrix:Float Ptr)
	Function NewtonCreateBox:Byte Ptr (newtonWorld:Byte Ptr, dx#, dy#, dz#, shapeID, offsetMatrix:Float Ptr)
	Function NewtonCreateCone:Byte Ptr (newtonWorld:Byte Ptr, radius#, height#, shapeID, offsetMatrix:Float Ptr)
	Function NewtonCreateCapsule:Byte Ptr (newtonWorld:Byte Ptr, radius#, height#, shapeID, offsetMatrix:Float Ptr)
	Function NewtonCreateCylinder:Byte Ptr (newtonWorld:Byte Ptr, radius#, height#, shapeID, offsetMatrix:Float Ptr)
	Function NewtonCreateTaperedCapsule:Byte Ptr (newtonWorld:Byte Ptr, radio0#, radio1#, height#, shapeID, offsetMatrix:Float Ptr)
	Function NewtonCreateTaperedCylinder:Byte Ptr (newtonWorld:Byte Ptr, radio0#, radio1#, height#, shapeID, offsetMatrix:Float Ptr)
	Function NewtonCreateChamferCylinder:Byte Ptr (newtonWorld:Byte Ptr, radius#, height#, shapeID, offsetMatrix:Float Ptr)
	Function NewtonCreateConvexHull:Byte Ptr (newtonWorld:Byte Ptr, count, vertexCloud:Float Ptr, strideInBytes, tolerance#, shapeID, offsetMatrix:Float Ptr)
	Function NewtonCreateConvexHullFromMesh:Byte Ptr (newtonWorld:Byte Ptr, mesh:Byte Ptr, tolerance#, shapeID)
'
'	NEWTON_API int NewtonCollisionGetMode(const NewtonCollision* const convexCollision);
'	NewtonCollisionSetCollisonMode (convexCollision:Byte Ptr, int mode);
'
'	
'
'//	NewtonCollisionSetMaxBreakImpactImpulse(const NewtonCollision* const convexHullCollision, dFloat maxImpactImpulse);
'//	NEWTON_API dFloat NewtonCollisionGetMaxBreakImpactImpulse(const NewtonCollision* const convexHullCollision);
'
'	NEWTON_API int NewtonConvexHullGetFaceIndices (const NewtonCollision* const convexHullCollision, int face, int* const faceIndices);
'	NEWTON_API int NewtonConvexHullGetVetexData (const NewtonCollision* const convexHullCollision, dFloat** const vertexData, int* strideInBytes);
'	
'	NEWTON_API dFloat NewtonConvexCollisionCalculateVolume (const NewtonCollision* const convexCollision);
	Function NewtonConvexCollisionCalculateInertialMatrix (convexCollision:Byte Ptr, inertia:Float Ptr, origin:Float Ptr)
'	NewtonConvexCollisionCalculateBuoyancyAcceleration (const NewtonCollision* const convexCollision, matrix:Float Ptr, const dFloat* const shapeOrigin, const dFloat* const gravityVector, const dFloat* const fluidPlane, dFloat fluidDensity, dFloat fluidViscosity, dFloat* const accel, dFloat* const alpha);
'
'	NEWTON_API const void* NewtonCollisionDataPointer (const NewtonCollision* const convexCollision);
'
'	// **********************************************************************************************
'	//
'	// compound collision primitives creation functions
'	//
'	// **********************************************************************************************
'	NEWTON_API NewtonCollision* NewtonCreateCompoundCollision (newtonWorld:Byte Ptr, shapeID);
'	NEWTON_API NewtonCollision* NewtonCreateCompoundCollisionFromMesh (newtonWorld:Byte Ptr, mesh:Byte Ptr, dFloat hullTolerance, shapeID, int subShapeID);
'
'	NewtonCompoundCollisionBeginAddRemove (NewtonCollision* const compoundCollision);	
'	NEWTON_API void* NewtonCompoundCollisionAddSubCollision (NewtonCollision* const compoundCollision, NewtonCollision* const convexCollision);	
'	NewtonCompoundCollisionRemoveSubCollision (NewtonCollision* const compoundCollision, void* const collisionNode);	
'	NewtonCompoundCollisionRemoveSubCollisionByIndex (NewtonCollision* const compoundCollision, int nodeIndex);	
'	NewtonCompoundCollisionSetSubCollisionMatrix (NewtonCollision* const compoundCollision, void* const collisionNode, dFloat* const matrix);	
'	NewtonCompoundCollisionEndAddRemove (NewtonCollision* const compoundCollision);	
'
'	NEWTON_API void* NewtonCompoundCollisionGetFirstNode (NewtonCollision* const compoundCollision);
'	NEWTON_API void* NewtonCompoundCollisionGetNextNode (NewtonCollision* const compoundCollision, void* const collisionNode);
'
'	NEWTON_API void* NewtonCompoundCollisionGetNodeByIndex (NewtonCollision* const compoundCollision, int index);
'	NEWTON_API int NewtonCompoundCollisionGetNodeIndex (NewtonCollision* const compoundCollision, void* const collisionNode);
'	NEWTON_API NewtonCollision* NewtonCompoundCollisionGetCollisionFromNode (NewtonCollision* const compoundCollision, void* const collisionNode);
'
'
'	// **********************************************************************************************
'	//
'	// scene collision are static compound collision that can take polygonal static collisions
'	//
'	// **********************************************************************************************
'	NEWTON_API NewtonCollision* NewtonCreateSceneCollision (newtonWorld:Byte Ptr, shapeID);
'
'	NewtonSceneCollisionBeginAddRemove (NewtonCollision* const sceneCollision);	
'	NEWTON_API void* NewtonSceneCollisionAddSubCollision (NewtonCollision* const sceneCollision, NewtonCollision* const collision);	
'	NewtonSceneCollisionRemoveSubCollision (NewtonCollision* const compoundCollision, void* const collisionNode);	
'	NewtonSceneCollisionRemoveSubCollisionByIndex (NewtonCollision* const sceneCollision, int nodeIndex);
'	NewtonSceneCollisionSetSubCollisionMatrix (NewtonCollision* const sceneCollision, void* const collisionNode, dFloat* const matrix);	
'	NewtonSceneCollisionEndAddRemove (NewtonCollision* const sceneCollision);	
'
'	NEWTON_API void* NewtonSceneCollisionGetFirstNode (NewtonCollision* const sceneCollision);
'	NEWTON_API void* NewtonSceneCollisionGetNextNode (NewtonCollision* const sceneCollision, void* const collisionNode);
'
'	NEWTON_API void* NewtonSceneCollisionGetNodeByIndex (NewtonCollision* const sceneCollision, int index);
'	NEWTON_API int NewtonSceneCollisionGetNodeIndex (NewtonCollision* const sceneCollision, void* const collisionNode);
'	NEWTON_API NewtonCollision* NewtonSceneCollisionGetCollisionFromNode (NewtonCollision* const sceneCollision, void* const collisionNode);
'
'
'
'	// **********************************************************************************************
'	//
'	// complex breakable collision primitives interface
'	//
'	// **********************************************************************************************
'/*
'	NEWTON_API NewtonCollision* NewtonCreateCompoundBreakable (newtonWorld:Byte Ptr, int meshCount, 
'															   const NewtonMesh** const solids, const int* const shapeIDArray, 
'															   const dFloat* const densities, const int* const internalFaceMaterial, 
'															   shapeID, int debriID, dFloat debriSeparationGap);
'
'
'	NewtonCompoundBreakableResetAnchoredPieces (const NewtonCollision* const compoundBreakable);
'	NewtonCompoundBreakableSetAnchoredPieces (const NewtonCollision* const compoundBreakable, int fixShapesCount, dFloat* const matrixPallete, NewtonCollision** const fixedShapesArray);
'
'	NEWTON_API int NewtonCompoundBreakableGetVertexCount (const NewtonCollision* const compoundBreakable); 
'	NewtonCompoundBreakableGetVertexStreams (const NewtonCollision* const compoundBreakable, int vertexStrideInByte, dFloat* const vertex,
'															int normalStrideInByte, dFloat* const normal,	int uvStrideInByte, dFloat* const uv);
'
'	
'	NEWTON_API NewtonBreakableComponentMesh* NewtonBreakableGetMainMesh (const NewtonCollision* const compoundBreakable);
'	NEWTON_API NewtonBreakableComponentMesh* NewtonBreakableGetFirstComponent (const NewtonCollision* const compoundBreakable);
'	NEWTON_API NewtonBreakableComponentMesh* NewtonBreakableGetNextComponent (const NewtonBreakableComponentMesh* const component);
'
'	NewtonBreakableBeginDelete (const NewtonCollision* const compoundBreakable);
'	NEWTON_API NewtonBody* NewtonBreakableCreateDebrieBody (const NewtonCollision* const compoundBreakable, const NewtonBreakableComponentMesh* const component);
'	NewtonBreakableDeleteComponent (const NewtonCollision* const compoundBreakable, const NewtonBreakableComponentMesh* const component);
'	NewtonBreakableEndDelete (const NewtonCollision* const compoundBreakable);
'
'
'	NEWTON_API int NewtonBreakableGetComponentsInRadius (const NewtonCollision* const compoundBreakable, const dFloat* position, dFloat radius, NewtonBreakableComponentMesh** const segments, int maxCount);
'
'	NEWTON_API void* NewtonBreakableGetFirstSegment (const NewtonBreakableComponentMesh* const breakableComponent); 
'	NEWTON_API void* NewtonBreakableGetNextSegment (const void* const segment); 
'
'	NEWTON_API int NewtonBreakableSegmentGetMaterial (const void* const segment); 
'	NEWTON_API int NewtonBreakableSegmentGetIndexCount (const void* const segment); 
'	NEWTON_API int NewtonBreakableSegmentGetIndexStream (const NewtonCollision* const compoundBreakable, const NewtonBreakableComponentMesh* const meshOwner, const void* const segment, int* const index); 
'	NEWTON_API int NewtonBreakableSegmentGetIndexStreamShort (const NewtonCollision* const compoundBreakable, const NewtonBreakableComponentMesh* const meshOwner, const void* const segment, short int* const index); 
'*/
'
'	NEWTON_API NewtonCollision* NewtonCreateUserMeshCollision (newtonWorld:Byte Ptr, const dFloat* const minBox, 
'		const dFloat* const maxBox, void* const userData, NewtonUserMeshCollisionCollideCallback collideCallback, 
'		NewtonUserMeshCollisionRayHitCallback rayHitCallback, NewtonUserMeshCollisionDestroyCallback destroyCallback,
'		NewtonUserMeshCollisionGetCollisionInfo getInfoCallback, NewtonUserMeshCollisionAABBTest getLocalAABBCallback, 
'		NewtonUserMeshCollisionGetFacesInAABB facesInAABBCallback, NewtonOnUserCollisionSerializationCallback serializeCallback, shapeID);
'
'
'	//  ***********************************************************************************************************
'	//
'	//	Collision serialization functions
'	//
'	// ***********************************************************************************************************
'	NEWTON_API NewtonCollision* NewtonCreateCollisionFromSerialization (newtonWorld:Byte Ptr, NewtonDeserializeCallback deserializeFunction, void* const serializeHandle);
'	NewtonCollisionSerialize (newtonWorld:Byte Ptr, collision:Byte Ptr, NewtonSerializeCallback serializeFunction, void* const serializeHandle);
'	NewtonCollisionGetInfo (collision:Byte Ptr, NewtonCollisionInfoRecord* const collisionInfo);
'
'	// **********************************************************************************************
'	//
'	// Static collision shapes functions
'	//
'	// **********************************************************************************************
'	NEWTON_API NewtonCollision* NewtonCreateHeightFieldCollision (newtonWorld:Byte Ptr, int width, int height, int gridsDiagonals, int elevationdatType,
'																  const void* const elevationMap, const char* const attributeMap, dFloat verticalScale, dFloat horizontalScale, shapeID);
'	NewtonHeightFieldSetUserRayCastCallback (const NewtonCollision* const hightfieldCollision, NewtonHeightFieldRayCastCallback rayHitCallback);
'
'	
'	NEWTON_API NewtonCollision* NewtonCreateTreeCollision (newtonWorld:Byte Ptr, shapeID);
'	NEWTON_API NewtonCollision* NewtonCreateTreeCollisionFromMesh (newtonWorld:Byte Ptr, mesh:Byte Ptr, shapeID);
'
'	NewtonTreeCollisionSetUserRayCastCallback (const NewtonCollision* const treeCollision, NewtonCollisionTreeRayCastCallback rayHitCallback);
'
'	NewtonTreeCollisionBeginBuild (const NewtonCollision* const treeCollision);
'	NewtonTreeCollisionAddFace (const NewtonCollision* const treeCollision, int vertexCount, const dFloat* const vertexPtr, int strideInBytes, int faceAttribute);
'	NewtonTreeCollisionEndBuild (const NewtonCollision* const treeCollision, int optimize);
'
'	NEWTON_API int NewtonTreeCollisionGetFaceAtribute (const NewtonCollision* const treeCollision, const int* const faceIndexArray, int indexCount); 
'	NewtonTreeCollisionSetFaceAtribute (const NewtonCollision* const treeCollision, const int* const faceIndexArray, int indexCount, int attribute); 
'	NewtonTreeCollisionForEachFace (const NewtonCollision* const treeCollision, NewtonTreeCollisionFaceCallback forEachFaceCallback, void* const context); 
'
'	NEWTON_API int NewtonTreeCollisionGetVertexListTriangleListInAABB (const NewtonCollision* const treeCollision, const dFloat* const p0, const dFloat* const p1, const dFloat** const vertexArray, int* const vertexCount, int* const vertexStrideInBytes, const int* const indexList, int maxIndexCount, const int* const faceAttribute); 
'
'	NewtonStaticCollisionSetDebugCallback (const NewtonCollision* const staticCollision, NewtonTreeCollisionCallback userCallback);
'
'	// **********************************************************************************************
'	//
'	// General purpose collision library functions
'	//
'	// **********************************************************************************************
'
'	NEWTON_API NewtonCollision* NewtonCollisionCreateInstance (collision:Byte Ptr);
'	NEWTON_API int NewtonCollisionGetType (collision:Byte Ptr);
'
'	NewtonCollisionSetUserData (collision:Byte Ptr, void* const userData);
'	NEWTON_API void* NewtonCollisionGetUserData (collision:Byte Ptr);
'	
'	NewtonCollisionSetUserID (collision:Byte Ptr, unsigned id);
'	NEWTON_API unsigned NewtonCollisionGetUserID (collision:Byte Ptr);
'
'	NewtonCollisionSetMatrix (collision:Byte Ptr, matrix:Float Ptr);
'	NewtonCollisionGetMatrix (collision:Byte Ptr, dFloat* const matrix);
'
'	NewtonCollisionSetScale (collision:Byte Ptr, dFloat scaleX, dFloat scaleY, dFloat scaleZ);
'	NewtonCollisionGetScale (collision:Byte Ptr, dFloat* const scaleX, dFloat* const scaleY, dFloat* const scaleZ);
'	NewtonDestroyCollision (collision:Byte Ptr);
'
'	NEWTON_API dFloat NewtonCollisionGetSkinThickness (collision:Byte Ptr);
'
'	NEWTON_API int NewtonCollisionIntersectionTest (newtonWorld:Byte Ptr, 
'		collision:Byte PtrA, matrix:Float PtrA, 
'		collision:Byte PtrB, matrix:Float PtrB, int threadIndex);
'
'	NEWTON_API int NewtonCollisionPointDistance (newtonWorld:Byte Ptr, const dFloat* const point,
'		collision:Byte Ptr, matrix:Float Ptr, dFloat* const contact, dFloat* const normal, int threadIndex);
'
'	NEWTON_API int NewtonCollisionClosestPoint (newtonWorld:Byte Ptr, 
'		collision:Byte PtrA, matrix:Float PtrA, 
'		collision:Byte PtrB, matrix:Float PtrB,
'		dFloat* const contactA, dFloat* const contactB, dFloat* const normalAB, int threadIndex);
'
'	NEWTON_API int NewtonCollisionCollide (newtonWorld:Byte Ptr, int maxSize,
'		collision:Byte PtrA, matrix:Float PtrA, 
'		collision:Byte PtrB, matrix:Float PtrB,
'		dFloat* const contacts, dFloat* const normals, dFloat* const penetration, 
'		dLong* const attributeA, dLong* const attributeB, int threadIndex);
'
'	NEWTON_API int NewtonCollisionCollideContinue (newtonWorld:Byte Ptr, int maxSize, dFloat timestep, 
'		collision:Byte PtrA, matrix:Float PtrA, const dFloat* const velocA, const dFloat* omegaA, 
'		collision:Byte PtrB, matrix:Float PtrB, const dFloat* const velocB, const dFloat* const omegaB, 
'		dFloat* const timeOfImpact, dFloat* const contacts, dFloat* const normals, dFloat* const penetration, 
'		dLong* const attributeA, dLong* const attributeB, int threadIndex);
'
'	NewtonCollisionSupportVertex (collision:Byte Ptr, const dFloat* const dir, dFloat* const vertex);
'	NEWTON_API dFloat NewtonCollisionRayCast (collision:Byte Ptr, const dFloat* const p0, const dFloat* const p1, dFloat* const normal, dLong* const attribute);
'	NewtonCollisionCalculateAABB (collision:Byte Ptr, matrix:Float Ptr, dFloat* const p0, dFloat* const p1);
'	NewtonCollisionForEachPolygonDo (collision:Byte Ptr, matrix:Float Ptr, NewtonCollisionIterator callback, void* const userData);
'
'	
'	
'	// **********************************************************************************************
'	//
'	// transforms utility functions
'	//
'	// **********************************************************************************************
'	NewtonGetEulerAngle (matrix:Float Ptr, dFloat* const eulersAngles);
'	NewtonSetEulerAngle (const dFloat* const eulersAngles, dFloat* const matrix);
'	NEWTON_API dFloat NewtonCalculateSpringDamperAcceleration (dFloat dt, dFloat ks, dFloat x, dFloat kd, dFloat s);
'
'	// **********************************************************************************************
'	//
'	// body manipulation functions
'	//
'	// **********************************************************************************************
	Function NewtonCreateDynamicBody:Byte Ptr (newtonWorld:Byte Ptr, collision:Byte Ptr, matrix:Float Ptr)
	Function NewtonCreateKinematicBody:Byte Ptr (newtonWorld:Byte Ptr, collision:Byte Ptr, matrix:Float Ptr)
	Function NewtonCreateDeformableBody:Byte Ptr (newtonWorld:Byte Ptr, deformableMesh:Byte Ptr, matrix:Float Ptr)
'
	Function NewtonDestroyBody(body:Byte Ptr)
'
'	NEWTON_API int NewtonBodyGetType (body:Byte Ptr);
'	NewtonSetBodyCollidable (body:Byte Ptr, int collidableState);
'
'	 NewtonBodyAddForce (body:Byte Ptr, const dFloat* const force);
'	 NewtonBodyAddTorque (body:Byte Ptr, const dFloat* const torque);
'	 NewtonBodyCalculateInverseDynamicsForce (body:Byte Ptr, dFloat timestep, const dFloat* const desiredVeloc, dFloat* const forceOut);
'
	Function NewtonBodySetCentreOfMass (body:Byte Ptr, com:Float Ptr)
	Function NewtonBodySetMassMatrix (body:Byte Ptr, mass#, Ixx#, Iyy#, Izz#)
'
'	 NewtonBodySetMassProperties (body:Byte Ptr, dFloat mass, collision:Byte Ptr);
'	 NewtonBodySetMatrix (body:Byte Ptr, matrix:Float Ptr);
'	 NewtonBodySetMatrixRecursive (body:Byte Ptr, matrix:Float Ptr);
'	
'	 NewtonBodySetMaterialGroupID (body:Byte Ptr, int id);
'	 NewtonBodySetContinuousCollisionMode (body:Byte Ptr, unsigned state);
'	 NewtonBodySetJointRecursiveCollision (body:Byte Ptr, unsigned state);
'	 NewtonBodySetOmega (body:Byte Ptr, const dFloat* const omega:Float Ptr)
'	 NewtonBodySetVelocity (body:Byte Ptr, const dFloat* const velocity:Float Ptr)
	Function NewtonBodySetForce (body:Byte Ptr, force:Float Ptr)
	Function NewtonBodySetTorque (body:Byte Ptr, torque:Float Ptr)
'	
'	 NewtonBodySetLinearDamping (body:Byte Ptr, dFloat linearDamp);
'	 NewtonBodySetAngularDamping (body:Byte Ptr, const dFloat* const angularDamp);
'	 NewtonBodySetCollision (body:Byte Ptr, collision:Byte Ptr);
'	 NewtonBodySetCollisionScale (body:Byte Ptr, dFloat scaleX, dFloat  scaleY, dFloat scaleZ);
'
'	NEWTON_API int  NewtonBodyGetSleepState (body:Byte Ptr);
'	NewtonBodySetSleepState (body:Byte Ptr, int state);
'
'	NEWTON_API int  NewtonBodyGetAutoSleep (body:Byte Ptr);
'	NewtonBodySetAutoSleep (body:Byte Ptr, int state);
'
'	NEWTON_API int  NewtonBodyGetFreezeState(body:Byte Ptr);
'	NewtonBodySetFreezeState (body:Byte Ptr, int state);
'
	Function NewtonBodySetDestructorCallback (body:Byte Ptr, callback:Byte Ptr)
'	NEWTON_API NewtonBodyDestructor NewtonBodyGetDestructorCallback (body:Byte Ptr);
'
	Function NewtonBodySetTransformCallback (body:Byte Ptr, callback:Byte Ptr)
'	NEWTON_API NewtonSetTransform NewtonBodyGetTransformCallback (body:Byte Ptr);
'	
	Function NewtonBodySetForceAndTorqueCallback (body:Byte Ptr, callback:Byte Ptr)
'	NEWTON_API NewtonApplyForceAndTorque NewtonBodyGetForceAndTorqueCallback (body:Byte Ptr);
'
'	NEWTON_API int NewtonBodyGetID (body:Byte Ptr);
'
	Function NewtonBodySetUserData (body:Byte Ptr, userData:Byte Ptr)
	Function NewtonBodyGetUserData:Byte Ptr (body:Byte Ptr)
'
'	NEWTON_API NewtonWorld* NewtonBodyGetWorld (body:Byte Ptr);
'	NEWTON_API NewtonCollision* NewtonBodyGetCollision (body:Byte Ptr);
'	NEWTON_API int   NewtonBodyGetMaterialGroupID (body:Byte Ptr);
'
'	NEWTON_API int   NewtonBodyGetContinuousCollisionMode (body:Byte Ptr);
'	NEWTON_API int   NewtonBodyGetJointRecursiveCollision (body:Byte Ptr);
'
'	 NewtonBodyGetMatrix(body:Byte Ptr, dFloat* const matrix);
'	 NewtonBodyGetRotation(body:Byte Ptr, dFloat* const rotation);
	Function NewtonBodyGetMassMatrix (body:Byte Ptr, mass:Float Ptr, Ixx:Float Ptr, Iyy:Float Ptr, Izz:Float Ptr)
'	 NewtonBodyGetInvMass(body:Byte Ptr, dFloat* const invMass, dFloat* const invIxx, dFloat* const invIyy, dFloat* const invIzz);
'	 NewtonBodyGetInertiaMatrix(body:Byte Ptr, inertia:Float PtrMatrix);
'	 NewtonBodyGetInvInertiaMatrix(body:Byte Ptr, dFloat* const invInertiaMatrix);
'	 NewtonBodyGetOmega(body:Byte Ptr, dFloat* const vector);
'	 NewtonBodyGetVelocity(body:Byte Ptr, dFloat* const vector);
'	 NewtonBodyGetForce(body:Byte Ptr, dFloat* const vector);
'	 NewtonBodyGetTorque(body:Byte Ptr, dFloat* const vector);
'	 NewtonBodyGetForceAcc(body:Byte Ptr, dFloat* const vector);
'	 NewtonBodyGetTorqueAcc(body:Byte Ptr, dFloat* const vector);
'	 NewtonBodyGetCentreOfMass (body:Byte Ptr, dFloat* const com);
'
'	NewtonBodyGetPointVelocity (body:Byte Ptr, const dFloat* const point, dFloat* const velocOut);
'	NewtonBodyAddImpulse (body:Byte Ptr, const dFloat* const pointDeltaVeloc, const dFloat* const pointPosit);
'	NewtonBodyApplyImpulseArray (body:Byte Ptr, int impuleCount, int strideInByte, const dFloat* const impulseArray, const dFloat* const pointArray);
'
'	NewtonBodyApplyImpulsePair (body:Byte Ptr, dFloat* const linearImpulse, dFloat* const angularImpulse);
'
'	NewtonBodyIntegrateVelocity (body:Byte Ptr, dFloat timestep);
'
'	NEWTON_API dFloat NewtonBodyGetLinearDamping (body:Byte Ptr);
'	 NewtonBodyGetAngularDamping (body:Byte Ptr, dFloat* const vector);
'	 NewtonBodyGetAABB (body:Byte Ptr, dFloat* const p0, dFloat* const p1);
'
'	NEWTON_API NewtonJoint* NewtonBodyGetFirstJoint (body:Byte Ptr);
'	NEWTON_API NewtonJoint* NewtonBodyGetNextJoint (body:Byte Ptr, const NewtonJoint* const joint);
'	NEWTON_API NewtonJoint* NewtonBodyGetFirstContactJoint (body:Byte Ptr);
'	NEWTON_API NewtonJoint* NewtonBodyGetNextContactJoint (body:Byte Ptr, const NewtonJoint* const contactJoint);
'	
'	
'
'
'	// **********************************************************************************************
'	//
'	// contact joints interface
'	//
'	// **********************************************************************************************
'
'	NEWTON_API void* NewtonContactJointGetFirstContact (const NewtonJoint* const contactJoint);
'	NEWTON_API void* NewtonContactJointGetNextContact (const NewtonJoint* const contactJoint, void* const contact);
'
'	NEWTON_API int NewtonContactJointGetContactCount(const NewtonJoint* const contactJoint);
'	NewtonContactJointRemoveContact(const NewtonJoint* const contactJoint, void* const contact); 
'
'	NEWTON_API NewtonMaterial* NewtonContactGetMaterial (const void* const contact);
'
'	NEWTON_API NewtonCollision* NewtonContactGetCollision0 (const void* const contact);	
'	NEWTON_API NewtonCollision* NewtonContactGetCollision1 (const void* const contact);	
'
'	NEWTON_API void* NewtonContactGetCollisionID0 (const void* const contact);	
'	NEWTON_API void* NewtonContactGetCollisionID1 (const void* const contact);	
'	
'	
'	// **********************************************************************************************
'	//
'	// Common joint functions
'	//
'	// **********************************************************************************************
'	NEWTON_API void* NewtonJointGetUserData (const NewtonJoint* const joint);
'	NewtonJointSetUserData (const NewtonJoint* const joint, void* const userData);
'
'	NEWTON_API NewtonBody* NewtonJointGetBody0 (const NewtonJoint* const joint);
'	NEWTON_API NewtonBody* NewtonJointGetBody1 (const NewtonJoint* const joint);
'
'	NewtonJointGetInfo  (const NewtonJoint* const joint, NewtonJointRecord* const info);
'	NEWTON_API int NewtonJointGetCollisionState (const NewtonJoint* const joint);
'	NewtonJointSetCollisionState (const NewtonJoint* const joint, int state);
'
'	NEWTON_API dFloat NewtonJointGetStiffness (const NewtonJoint* const joint);
'	NewtonJointSetStiffness (const NewtonJoint* const joint, dFloat state);
'
'	NewtonDestroyJoint(newtonWorld:Byte Ptr, const NewtonJoint* const joint);
'	NewtonJointSetDestructor (const NewtonJoint* const joint, NewtonConstraintDestructor destructor);
'
'
'
'	// **********************************************************************************************
'	//
'	// particle system interface (soft bodies, individual, pressure bodies and cloth)   
'	//
'	// **********************************************************************************************
'	NEWTON_API NewtonCollision* NewtonCreateClothPatch (newtonWorld:Byte Ptr, NewtonMesh* const mesh, shapeID, NewtonClothPatchMaterial* const structuralMaterial, NewtonClothPatchMaterial* const bendMaterial);
'	NEWTON_API NewtonCollision* NewtonCreateDeformableMesh (newtonWorld:Byte Ptr, NewtonMesh* const mesh, shapeID);
'
'	NEWTON_API int NewtonDeformableMeshParticleCount (deformableMesh:Byte Ptr); 
'	NewtonDeformableMeshGetParticlePosition (NewtonCollision* const deformableMesh, int particleIndex, dFloat* const posit);
'
'	NewtonDeformableMeshBeginConfiguration (deformableMesh:Byte Ptr); 
'	NewtonDeformableMeshUnconstraintParticle (NewtonCollision* const deformableMesh, int partivleIndex);
'	NewtonDeformableMeshConstraintParticle (NewtonCollision* const deformableMesh, int partivleIndex, const dFloat* const posit, body:Byte Ptr);
'	NewtonDeformableMeshEndConfiguration (deformableMesh:Byte Ptr); 
'
'//	NewtonDeformableMeshSetPlasticity (NewtonCollision* const deformableMesh, dFloat plasticity);
'//	NewtonDeformableMeshSetStiffness (NewtonCollision* const deformableMesh, dFloat stiffness);
'	NewtonDeformableMeshSetSkinThickness (NewtonCollision* const deformableMesh, dFloat skinThickness);
'
'	NewtonDeformableMeshUpdateRenderNormals (deformableMesh:Byte Ptr); 
'	NEWTON_API int NewtonDeformableMeshGetVertexCount (deformableMesh:Byte Ptr); 
'	NewtonDeformableMeshGetVertexStreams (deformableMesh:Byte Ptr, 
'		int vertexStrideInByte, dFloat* const vertex,
'		int normalStrideInByte, dFloat* const normal, 
'		int uvStrideInByte0, dFloat* const uv0,
'		int uvStrideInByte1, dFloat* const uv1);
'	NEWTON_API NewtonDeformableMeshSegment* NewtonDeformableMeshGetFirstSegment (deformableMesh:Byte Ptr);
'	NEWTON_API NewtonDeformableMeshSegment* NewtonDeformableMeshGetNextSegment (deformableMesh:Byte Ptr, const NewtonDeformableMeshSegment* const segment);
'
'	NEWTON_API int NewtonDeformableMeshSegmentGetMaterialID (deformableMesh:Byte Ptr, const NewtonDeformableMeshSegment* const segment);
'	NEWTON_API int NewtonDeformableMeshSegmentGetIndexCount (deformableMesh:Byte Ptr, const NewtonDeformableMeshSegment* const segment);
'	NEWTON_API const short* NewtonDeformableMeshSegmentGetIndexList (deformableMesh:Byte Ptr, const NewtonDeformableMeshSegment* const segment);
'
'
'
'
'	// **********************************************************************************************
'	//
'	// Ball and Socket joint functions
'	//
'	// **********************************************************************************************
'	NEWTON_API NewtonJoint* NewtonConstraintCreateBall (newtonWorld:Byte Ptr, const dFloat* pivotPoint, 
'														const NewtonBody* const childBody, const NewtonBody* const parentBody);
'	NewtonBallSetUserCallback (const NewtonJoint* const ball, NewtonBallCallback callback);
'	NewtonBallGetJointAngle (const NewtonJoint* const ball, dFloat* angle);
'	NewtonBallGetJointOmega (const NewtonJoint* const ball, dFloat* omega);
'	NewtonBallGetJointForce (const NewtonJoint* const ball, dFloat* const force);
'	NewtonBallSetConeLimits (const NewtonJoint* const ball, const dFloat* pin, dFloat maxConeAngle, dFloat maxTwistAngle);
'
'	// **********************************************************************************************
'	//
'	// Hinge joint functions
'	//
'	// **********************************************************************************************
'	NEWTON_API NewtonJoint* NewtonConstraintCreateHinge (newtonWorld:Byte Ptr, 
'		const dFloat* pivotPoint, const dFloat* pinDir, 
'		const NewtonBody* const childBody, const NewtonBody* const parentBody);
'
'	NewtonHingeSetUserCallback (const NewtonJoint* const hinge, NewtonHingeCallback callback);
'	NEWTON_API dFloat NewtonHingeGetJointAngle (const NewtonJoint* const hinge);
'	NEWTON_API dFloat NewtonHingeGetJointOmega (const NewtonJoint* const hinge);
'	NewtonHingeGetJointForce (const NewtonJoint* const hinge, dFloat* const force);
'	NEWTON_API dFloat NewtonHingeCalculateStopAlpha (const NewtonJoint* const hinge, const NewtonHingeSliderUpdateDesc* const desc, dFloat angle);
'
'	// **********************************************************************************************
'	//
'	// Slider joint functions
'	//
'	// **********************************************************************************************
'	NEWTON_API NewtonJoint* NewtonConstraintCreateSlider (newtonWorld:Byte Ptr, 
'		const dFloat* pivotPoint, const dFloat* pinDir, 
'		const NewtonBody* const childBody, const NewtonBody* const parentBody);
'	NewtonSliderSetUserCallback (const NewtonJoint* const slider, NewtonSliderCallback callback);
'	NEWTON_API dFloat NewtonSliderGetJointPosit (const NewtonJoint* slider);
'	NEWTON_API dFloat NewtonSliderGetJointVeloc (const NewtonJoint* slider);
'	NewtonSliderGetJointForce (const NewtonJoint* const slider, dFloat* const force);
'	NEWTON_API dFloat NewtonSliderCalculateStopAccel (const NewtonJoint* const slider, const NewtonHingeSliderUpdateDesc* const desc, dFloat position);
'
'
'	// **********************************************************************************************
'	//
'	// Corkscrew joint functions
'	//
'	// **********************************************************************************************
'	NEWTON_API NewtonJoint* NewtonConstraintCreateCorkscrew (newtonWorld:Byte Ptr, 
'		const dFloat* pivotPoint, const dFloat* pinDir, 
'		const NewtonBody* const childBody, const NewtonBody* const parentBody);
'	NewtonCorkscrewSetUserCallback (const NewtonJoint* const corkscrew, NewtonCorkscrewCallback callback);
'	NEWTON_API dFloat NewtonCorkscrewGetJointPosit (const NewtonJoint* const corkscrew);
'	NEWTON_API dFloat NewtonCorkscrewGetJointAngle (const NewtonJoint* const corkscrew);
'	NEWTON_API dFloat NewtonCorkscrewGetJointVeloc (const NewtonJoint* const corkscrew);
'	NEWTON_API dFloat NewtonCorkscrewGetJointOmega (const NewtonJoint* const corkscrew);
'	NewtonCorkscrewGetJointForce (const NewtonJoint* const corkscrew, dFloat* const force);
'	NEWTON_API dFloat NewtonCorkscrewCalculateStopAlpha (const NewtonJoint* const corkscrew, const NewtonHingeSliderUpdateDesc* const desc, dFloat angle);
'	NEWTON_API dFloat NewtonCorkscrewCalculateStopAccel (const NewtonJoint* const corkscrew, const NewtonHingeSliderUpdateDesc* const desc, dFloat position);
'
'
'	// **********************************************************************************************
'	//
'	// Universal joint functions
'	//
'	// **********************************************************************************************
'	NEWTON_API NewtonJoint* NewtonConstraintCreateUniversal (newtonWorld:Byte Ptr, 
'		const dFloat* pivotPoint, const dFloat* pinDir0, const dFloat* pinDir1, 
'		const NewtonBody* const childBody, const NewtonBody* const parentBody);
'	NewtonUniversalSetUserCallback (const NewtonJoint* const universal, NewtonUniversalCallback callback);
'	NEWTON_API dFloat NewtonUniversalGetJointAngle0 (const NewtonJoint* const universal);
'	NEWTON_API dFloat NewtonUniversalGetJointAngle1 (const NewtonJoint* const universal);
'	NEWTON_API dFloat NewtonUniversalGetJointOmega0 (const NewtonJoint* const universal);
'	NEWTON_API dFloat NewtonUniversalGetJointOmega1 (const NewtonJoint* const universal);
'	NewtonUniversalGetJointForce (const NewtonJoint* const universal, dFloat* const force);
'	NEWTON_API dFloat NewtonUniversalCalculateStopAlpha0 (const NewtonJoint* const universal, const NewtonHingeSliderUpdateDesc* const desc, dFloat angle);
'	NEWTON_API dFloat NewtonUniversalCalculateStopAlpha1 (const NewtonJoint* const universal, const NewtonHingeSliderUpdateDesc* const desc, dFloat angle);
'
'
'	// **********************************************************************************************
'	//
'	// Up vector joint functions
'	//
'	// **********************************************************************************************
'	NEWTON_API NewtonJoint* NewtonConstraintCreateUpVector (newtonWorld:Byte Ptr, const dFloat* pinDir, body:Byte Ptr); 
'	NewtonUpVectorGetPin (const NewtonJoint* const upVector, dFloat *pin);
'	NewtonUpVectorSetPin (const NewtonJoint* const upVector, const dFloat *pin);
'
'
'	// **********************************************************************************************
'	//
'	// User defined bilateral Joint
'	//
'	// **********************************************************************************************
'	NEWTON_API NewtonJoint* NewtonConstraintCreateUserJoint (newtonWorld:Byte Ptr, int maxDOF, 
'															 NewtonUserBilateralCallback callback,
'															 NewtonUserBilateralGetInfoCallback getInfo,
'															 const NewtonBody* const childBody, const NewtonBody* const parentBody) ; 
'	NewtonUserJointSetFeedbackCollectorCallback (const NewtonJoint* const joint, NewtonUserBilateralCallback getFeedback);
'	NewtonUserJointAddLinearRow (const NewtonJoint* const joint, const dFloat* const pivot0, const dFloat* const pivot1, const dFloat* const dir);
'	NewtonUserJointAddAngularRow (const NewtonJoint* const joint, dFloat relativeAngle, const dFloat* const dir);
'	NewtonUserJointAddGeneralRow (const NewtonJoint* const joint, const dFloat* const jacobian0, const dFloat* const jacobian1);
'	NewtonUserJointSetRowMinimumFriction (const NewtonJoint* const joint, dFloat friction);
'	NewtonUserJointSetRowMaximumFriction (const NewtonJoint* const joint, dFloat friction);
'	NewtonUserJointSetRowAcceleration (const NewtonJoint* const joint, dFloat acceleration);
'	NewtonUserJointSetRowSpringDamperAcceleration (const NewtonJoint* const joint, dFloat springK, dFloat springD);
'	NewtonUserJointSetRowStiffness (const NewtonJoint* const joint, dFloat stiffness);
'	NEWTON_API dFloat NewtonUserJointGetRowForce (const NewtonJoint* const joint, int row);
'
'	NewtonUserJointSetSolver (const NewtonJoint* const joint, int solver, int maxContactJoints);
'	
'
'	// **********************************************************************************************
'	//
'	// Mesh joint functions
'	//
'	// **********************************************************************************************
'	NEWTON_API NewtonMesh* NewtonMeshCreate(newtonWorld:Byte Ptr);
'	NEWTON_API NewtonMesh* NewtonMeshCreateFromMesh(mesh:Byte Ptr);
'	NEWTON_API NewtonMesh* NewtonMeshCreateFromCollision(collision:Byte Ptr);
'	NEWTON_API NewtonMesh* NewtonMeshCreateConvexHull (newtonWorld:Byte Ptr, int pointCount, const dFloat* const vertexCloud, int strideInBytes, dFloat tolerance);
'	NEWTON_API NewtonMesh* NewtonMeshCreateDelaunayTetrahedralization (newtonWorld:Byte Ptr, int pointCount, const dFloat* const vertexCloud, int strideInBytes, int materialID, const dFloat* const textureMatrix);
'	NEWTON_API NewtonMesh* NewtonMeshCreateVoronoiConvexDecomposition (newtonWorld:Byte Ptr, int pointCount, const dFloat* const vertexCloud, int strideInBytes, int materialID, const dFloat* const textureMatrix);
'	NEWTON_API NewtonMesh* NewtonMeshCreateFromSerialization (newtonWorld:Byte Ptr, NewtonDeserializeCallback deserializeFunction, void* const serializeHandle);
'	NewtonMeshDestroy(mesh:Byte Ptr);
'
'	NewtonMeshSerialize (mesh:Byte Ptr, NewtonSerializeCallback serializeFunction, void* const serializeHandle);
'	NewtonMeshSaveOFF(mesh:Byte Ptr, const char* const filename);
'	NEWTON_API NewtonMesh* NewtonMeshLoadOFF(newtonWorld:Byte Ptr, const char* const filename);
'
'	NewtonMeshApplyTransform (mesh:Byte Ptr, matrix:Float Ptr);
'	NewtonMeshCalculateOOBB(mesh:Byte Ptr, dFloat* const matrix, dFloat* const x, dFloat* const y, dFloat* const z);
'
'	NewtonMeshCalculateVertexNormals(mesh:Byte Ptr, dFloat angleInRadians);
'	NewtonMeshApplySphericalMapping(mesh:Byte Ptr, int material);
'	NewtonMeshApplyBoxMapping(mesh:Byte Ptr, int front, int side, int top);
'	NewtonMeshApplyCylindricalMapping(mesh:Byte Ptr, int cylinderMaterial, int capMaterial);
'	
'	NEWTON_API int NewtonMeshIsOpenMesh (mesh:Byte Ptr);
'	NewtonMeshFixTJoints (mesh:Byte Ptr);
'
'	NewtonMeshPolygonize (mesh:Byte Ptr);
'	NewtonMeshTriangulate (mesh:Byte Ptr);
'	NEWTON_API NewtonMesh* NewtonMeshUnion (mesh:Byte Ptr, const NewtonMesh* const clipper, const dFloat* const clipperMatrix);
'	NEWTON_API NewtonMesh* NewtonMeshDifference (mesh:Byte Ptr, const NewtonMesh* const clipper, const dFloat* const clipperMatrix);
'	NEWTON_API NewtonMesh* NewtonMeshIntersection (mesh:Byte Ptr, const NewtonMesh* const clipper, const dFloat* const clipperMatrix);
'	NewtonMeshClip (mesh:Byte Ptr, const NewtonMesh* const clipper, const dFloat* const clipperMatrix, NewtonMesh** const topMesh, NewtonMesh** const bottomMesh);
'
'	NEWTON_API NewtonMesh* NewtonMeshConvexMeshIntersection (mesh:Byte Ptr, const NewtonMesh* const convexMesh);
'
'	NEWTON_API NewtonMesh* NewtonMeshSimplify (mesh:Byte Ptr, int maxVertexCount, NewtonReportProgress reportPrograssCallback);
'	NEWTON_API NewtonMesh* NewtonMeshApproximateConvexDecomposition (mesh:Byte Ptr, dFloat maxConcavity, dFloat backFaceDistanceFactor, int maxCount, int maxVertexPerHull, NewtonReportProgress reportProgressCallback);
'
'	NewtonRemoveUnusedVertices(mesh:Byte Ptr, int* const vertexRemapTable);
'
'	NewtonMeshBeginFace(mesh:Byte Ptr);
'	NewtonMeshAddFace(mesh:Byte Ptr, int vertexCount, const dFloat* const vertex, int strideInBytes, int materialIndex);
'	NewtonMeshEndFace(mesh:Byte Ptr);
'
'	NewtonMeshBuildFromVertexListIndexList(mesh:Byte Ptr,
'		int faceCount, const int* const faceIndexCount, const int* const faceMaterialIndex, 
'		const dFloat* const vertex, int vertexStrideInBytes, const int* const vertexIndex,
'		const dFloat* const normal, int normalStrideInBytes, const int* const normalIndex,
'		const dFloat* const uv0, int uv0StrideInBytes, const int* const uv0Index,
'		const dFloat* const uv1, int uv1StrideInBytes, const int* const uv1Index);
'
'
'
'	NewtonMeshGetVertexStreams (mesh:Byte Ptr, 
'												int vertexStrideInByte, dFloat* const vertex,
'												int normalStrideInByte, dFloat* const normal,
'												int uvStrideInByte0, dFloat* const uv0,
'												int uvStrideInByte1, dFloat* const uv1);
'	NewtonMeshGetIndirectVertexStreams(mesh:Byte Ptr, 
'													   int vertexStrideInByte, dFloat* const vertex, int* const vertexIndices, int* const vertexCount,
'													   int normalStrideInByte, dFloat* const normal, int* const normalIndices, int* const normalCount,
'													   int uvStrideInByte0, dFloat* const uv0, int* const uvIndices0, int* const uvCount0,
'													   int uvStrideInByte1, dFloat* const uv1, int* const uvIndices1, int* const uvCount1);
'	NEWTON_API void* NewtonMeshBeginHandle (mesh:Byte Ptr); 
'	NewtonMeshEndHandle (mesh:Byte Ptr, void* const handle); 
'	NEWTON_API int NewtonMeshFirstMaterial (mesh:Byte Ptr, void* const handle); 
'	NEWTON_API int NewtonMeshNextMaterial (mesh:Byte Ptr, void* const handle, int materialId); 
'	NEWTON_API int NewtonMeshMaterialGetMaterial (mesh:Byte Ptr, void* const handle, int materialId); 
'	NEWTON_API int NewtonMeshMaterialGetIndexCount (mesh:Byte Ptr, void* const handle, int materialId); 
'	NewtonMeshMaterialGetIndexStream (mesh:Byte Ptr, void* const handle, int materialId, int* const index); 
'	NewtonMeshMaterialGetIndexStreamShort (mesh:Byte Ptr, void* const handle, int materialId, short int* const index); 
'
'	NEWTON_API NewtonMesh* NewtonMeshCreateFirstSingleSegment (mesh:Byte Ptr); 
'	NEWTON_API NewtonMesh* NewtonMeshCreateNextSingleSegment (mesh:Byte Ptr, const NewtonMesh* const segment); 
'
'	NEWTON_API NewtonMesh* NewtonMeshCreateFirstLayer (mesh:Byte Ptr); 
'	NEWTON_API NewtonMesh* NewtonMeshCreateNextLayer (mesh:Byte Ptr, const NewtonMesh* const segment); 
'
'
'	NEWTON_API int NewtonMeshGetTotalFaceCount (mesh:Byte Ptr); 
'	NEWTON_API int NewtonMeshGetTotalIndexCount (mesh:Byte Ptr); 
'	NewtonMeshGetFaces (mesh:Byte Ptr, int* const faceIndexCount, int* const faceMaterial, void** const faceIndices); 
'
'
'	NEWTON_API int NewtonMeshGetPointCount (mesh:Byte Ptr); 
'	NEWTON_API int NewtonMeshGetPointStrideInByte (mesh:Byte Ptr); 
'	NEWTON_API dFloat64* NewtonMeshGetPointArray (mesh:Byte Ptr); 
'	NEWTON_API dFloat64* NewtonMeshGetNormalArray (mesh:Byte Ptr); 
'	NEWTON_API dFloat64* NewtonMeshGetUV0Array (mesh:Byte Ptr); 
'	NEWTON_API dFloat64* NewtonMeshGetUV1Array (mesh:Byte Ptr); 
'
'	NEWTON_API int NewtonMeshGetVertexCount (mesh:Byte Ptr); 
'	NEWTON_API int NewtonMeshGetVertexStrideInByte (mesh:Byte Ptr); 
'	NEWTON_API dFloat64* NewtonMeshGetVertexArray (mesh:Byte Ptr); 
'
'
'	NEWTON_API void* NewtonMeshGetFirstVertex (mesh:Byte Ptr);
'	NEWTON_API void* NewtonMeshGetNextVertex (mesh:Byte Ptr, const void* const vertex);
'	NEWTON_API int NewtonMeshGetVertexIndex (mesh:Byte Ptr, const void* const vertex);
'
'	NEWTON_API void* NewtonMeshGetFirstPoint (mesh:Byte Ptr);
'	NEWTON_API void* NewtonMeshGetNextPoint (mesh:Byte Ptr, const void* const point);
'	NEWTON_API int NewtonMeshGetPointIndex (mesh:Byte Ptr, const void* const point);
'	NEWTON_API int NewtonMeshGetVertexIndexFromPoint (mesh:Byte Ptr, const void* const point);
'	
'
'	NEWTON_API void* NewtonMeshGetFirstEdge (mesh:Byte Ptr);
'	NEWTON_API void* NewtonMeshGetNextEdge (mesh:Byte Ptr, const void* const edge);
'	NewtonMeshGetEdgeIndices (mesh:Byte Ptr, const void* const edge, int* const v0, int* const v1);
'	//NewtonMeshGetEdgePointIndices (mesh:Byte Ptr, const void* const edge, int* const v0, int* const v1);
'
'	NEWTON_API void* NewtonMeshGetFirstFace (mesh:Byte Ptr);
'	NEWTON_API void* NewtonMeshGetNextFace (mesh:Byte Ptr, const void* const face);
'	NEWTON_API int NewtonMeshIsFaceOpen (mesh:Byte Ptr, const void* const face);
'	NEWTON_API int NewtonMeshGetFaceMaterial (mesh:Byte Ptr, const void* const face);
'	NEWTON_API int NewtonMeshGetFaceIndexCount (mesh:Byte Ptr, const void* const face);
'	NewtonMeshGetFaceIndices (mesh:Byte Ptr, const void* const face, int* const indices);
'	NewtonMeshGetFacePointIndices (mesh:Byte Ptr, const void* const face, int* const indices);
'	NewtonMeshCalculateFaceNormal (mesh:Byte Ptr, const void* const face, dFloat64* const normal);
'
'	NewtonMeshSetFaceMaterial (mesh:Byte Ptr, const void* const face, int matId);
End Extern
