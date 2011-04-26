/* Copyright (c) <2003-2011> <Julio Jerez, Newton Game Dynamics>
* 
* This software is provided 'as-is', without any express or implied
* warranty. In no event will the authors be held liable for any damages
* arising from the use of this software.
* 
* Permission is granted to anyone to use this software for any purpose,
* including commercial applications, and to alter it and redistribute it
* freely, subject to the following restrictions:
* 
* 1. The origin of this software must not be misrepresented; you must not
* claim that you wrote the original software. If you use this software
* in a product, an acknowledgment in the product documentation would be
* appreciated but is not required.
* 
* 2. Altered source versions must be plainly marked as such, and must not be
* misrepresented as being the original software.
* 
* 3. This notice may not be removed or altered from any source distribution.
*/

#ifndef __dgMeshEffectSolidTree_H__
#define __dgMeshEffectSolidTree_H__

#include <dgRefCounter.h>

class dgCollision;
class dgMeshEffect;


class dgMeshTreeCSGFace: public dgPolyhedra
{
	public:
	dgMeshTreeCSGFace (const dgMeshEffect& mesh, dgEdge* const face);
	void ClipFace (dgEdge* const face, const dgHugeVector& normal, const dgHugeVector& origin, dgEdge** leftOut, dgEdge** rightOut);



	dgInt32 AddPoint (const dgMeshEffect::dgVertexAtribute& point);
	bool CheckConsistency () const;

	dgInt32 m_count;
	dgInt32 m_baseCount;
	dgMeshEffect::dgVertexAtribute m_points[DG_MESH_EFFECT_POINT_SPLITED];
};

class dgMeshEffectSolidTree
{
	public:
	DG_CLASS_ALLOCATOR(allocator)

	class CSGConvexCurve: public dgList<dgHugeVector>
	{
		public:
		CSGConvexCurve ();
		CSGConvexCurve (dgMemoryAllocator* const allocator);
		bool CheckConvex(const dgHugeVector& normal, const dgHugeVector& point) const;
	};

	dgMeshEffectSolidTree (const dgMeshEffect& mesh, dgEdge* const face);
	dgMeshEffectSolidTree (const dgHugeVector& normal, const dgHugeVector& point);
	~dgMeshEffectSolidTree();

	void BuildPlane (const dgMeshEffect& mesh, dgEdge* const face, dgHugeVector& normal, dgHugeVector& point) const;
	void AddFace (const dgMeshEffect& mesh, dgEdge* const face);

	dgHugeVector m_origin;
	dgHugeVector m_normal;
	dgMeshEffectSolidTree* m_back;
	dgMeshEffectSolidTree* m_front;
};
#endif
