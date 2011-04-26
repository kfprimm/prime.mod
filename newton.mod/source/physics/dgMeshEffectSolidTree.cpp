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

#include "dgPhysicsStdafx.h"
#include "dgMeshEffect.h"
#include "dgMeshEffectSolidTree.h"


dgMeshTreeCSGFace::dgMeshTreeCSGFace (const dgMeshEffect& mesh, dgEdge* const face)
	:dgPolyhedra (mesh.GetAllocator()), m_count (0)
{
	const dgEdge* ptr = face;
	const dgMeshEffect::dgVertexAtribute* const attib = mesh.m_attib;

	dgInt32 indexList[256];
	do {
		indexList[m_count] = m_count;	
		AddPoint (attib[dgInt32 (ptr->m_userData)]);
		ptr = ptr->m_next;
	} while (ptr != face);

	m_baseCount = m_count;

	BeginFace();
	AddFace(m_count, indexList);
	EndFace();

	_ASSERTE (CheckConsistency ());
}


dgInt32 dgMeshTreeCSGFace::AddPoint (const dgMeshEffect::dgVertexAtribute& point)
{
	m_points[m_count] = point;
	m_count ++;
	_ASSERTE (m_count < sizeof (m_points)/sizeof (m_points[0]));
	return (m_count - 1);
}


bool dgMeshTreeCSGFace::CheckConsistency () const
{
	dgInt32 outFaceCount = 0;
	dgInt32 mark = IncLRU();
	dgMeshTreeCSGFace::Iterator iter (*this);
	for (iter.Begin(); iter; iter ++) {
		dgEdge* const edge = &(*iter);
		if ((edge->m_incidentFace < 0) && ((edge->m_mark != mark))){
			outFaceCount ++;
			dgEdge* ptr = edge;
			do {
				//dgInt32 i = ptr->m_incidentVertex;
				//dgTrace (("%f %f %f\n", m_points[i].m_x, m_points[i].m_y, m_points[i].m_z));
				ptr->m_mark = mark;
				ptr = ptr->m_next;
			} while (ptr != edge);
			//dgTrace (("\n"));
		} 
	}
	return (outFaceCount == 1) ? true : false;
}

dgMeshEffectSolidTree::CSGConvexCurve::CSGConvexCurve ()
	:dgList<dgHugeVector>(NULL)
{
}

dgMeshEffectSolidTree::CSGConvexCurve::CSGConvexCurve (dgMemoryAllocator* const allocator)
	:dgList<dgHugeVector>(allocator)
{
}

bool dgMeshEffectSolidTree::CSGConvexCurve::CheckConvex(const dgHugeVector& normal, const dgHugeVector& point) const
{
	dgHugeVector p1 (GetLast()->GetInfo());
	dgHugeVector p0 (GetLast()->GetPrev()->GetInfo());
	dgHugeVector e0 (p0 - p1);
	for (CSGConvexCurve::dgListNode* node = GetFirst(); node; node = node->GetNext()) {
		dgHugeVector p2 (node->GetInfo());
		dgHugeVector e1 (p2 - p1);

		dgHugeVector n (e1 * e0);
		dgGoogol convex = n % normal;
		if (convex.GetAproximateValue() < dgFloat64 (-1.0e10f)) {
			return false;
		}
		p1 = p2;
		e0 = e1.Scale (dgGoogol(-1.0f));
	}
	return true;
};




dgMeshEffectSolidTree::dgMeshEffectSolidTree (const dgMeshEffect& mesh, dgEdge* const face)
{
	BuildPlane (mesh, face, m_normal, m_origin);
	//normal = normal.Scale (1.0f / sqrt (normal % normal));
	//m_plane = dgBigPlane (normal, - (normal % origin));
	m_front = NULL;
	m_back = NULL;
}

dgMeshEffectSolidTree::dgMeshEffectSolidTree (const dgHugeVector& normal, const dgHugeVector& origin)
	:m_origin (origin), m_normal (normal)
{
	m_front = NULL;
	m_back = NULL;
}


dgMeshEffectSolidTree::~dgMeshEffectSolidTree()
{
	if (m_front) {
		delete m_front; 
	}

	if (m_back)	{
		delete m_back;
	}
}



void dgMeshEffectSolidTree::BuildPlane (const dgMeshEffect& mesh, dgEdge* const face, dgHugeVector& normal, dgHugeVector& point) const
{
	point = dgHugeVector(mesh.m_points[face->m_incidentVertex]);

	dgEdge* edge = face;
	normal = dgHugeVector (dgFloat32 (0.0f), dgFloat32 (0.0f), dgFloat32 (0.0f), dgFloat32 (0.0f));

	dgHugeVector p0 (mesh.m_points[edge->m_incidentVertex]);
	edge = edge->m_next;

	dgHugeVector p1 (mesh.m_points[edge->m_incidentVertex]);
	dgHugeVector e1 (p1 - p0);
	for (edge = edge->m_next; edge != face; edge = edge->m_next) {
		dgHugeVector p2 (mesh.m_points[edge->m_incidentVertex]);
		dgHugeVector e2 (p2 - p0);
		normal += e1 * e2;
		e1 = e2;
	} 
}


void dgMeshEffectSolidTree::AddFace (const dgMeshEffect& mesh, dgEdge* const face)
{
	dgBigVector normal (mesh.FaceNormal (face, &mesh.m_points[0][0], sizeof (dgBigVector)));
	dgFloat64 mag2 = normal % normal;

	if (mag2 > dgFloat32 (1.0e-14f)) {

		CSGConvexCurve faces[DG_MESH_EFFECT_BOLLEAN_STACK];
		dgMeshEffectSolidTree* pool[DG_MESH_EFFECT_BOLLEAN_STACK];

		dgHugeVector point;
		dgHugeVector normal;
		BuildPlane (mesh, face, normal, point);

		dgEdge* ptr = face;
		faces[0].SetAllocator(mesh.GetAllocator());
		do {
			faces[0].Append(mesh.m_points[ptr->m_incidentVertex]);
			ptr = ptr->m_next;
		} while (ptr != face);

		dgInt32 stack = 1;
		pool[0] = this;
		while (stack) {

			stack --;
			dgMeshEffectSolidTree* const root = pool[stack];
			CSGConvexCurve& curve = faces[stack];
			_ASSERTE (curve.CheckConvex(normal, point));

			dgGoogol minDist (dgFloat64 (0.0f));
			dgGoogol maxDist (dgFloat64 (0.0f));
			for (CSGConvexCurve::dgListNode* node = curve.GetFirst(); node; node = node->GetNext()) {
				dgGoogol dist = root->m_normal % (node->GetInfo() - root->m_origin);
				dgGoogol error = dist - minDist;
				if (error.GetAproximateValue() < dgFloat64 (0.0f)) {
					minDist = dist;
				}
				error = dist - maxDist;
				if (error.GetAproximateValue() > dgFloat64 (0.0f)) {
					maxDist = dist;
				} 
			} 

			dgFloat64 minVal = minDist.GetAproximateValue();
			if (minVal > dgFloat64 (-1.0e-2f)) {
				if (minVal != dgFloat64 (0.0f)) {
					dgFloat64 t = minVal / (root->m_normal % root->m_normal).GetAproximateValue();
					dgHugeVector dist(root->m_normal.Scale (dgGoogol(t)));
					dgFloat64 dist2 = (dist % dist).GetAproximateValue();
					if (dist2 < dgFloat64 (1.0e-10f)) {
						minDist = dgGoogol (dgFloat64 (0.0f));
						minVal = dgFloat64 (0.0f);
					}
				}
			}

			dgFloat64 maxVal = maxDist.GetAproximateValue();
			if (maxVal < dgFloat64 (1.0e-2f)) {
				if (maxVal != dgFloat64 (0.0f)) {
					dgFloat64 t = maxVal / (root->m_normal % root->m_normal).GetAproximateValue();
					dgHugeVector dist(root->m_normal.Scale (dgGoogol(t)));
					dgFloat64 dist2 = (dist % dist).GetAproximateValue();
					if (dist2 < dgFloat64 (1.0e-10f)) {
						maxDist = dgGoogol (dgFloat64 (0.0f));
						maxVal = dgFloat64 (0.0f);
					}
				}
			}

			if ((minVal < dgFloat64 (0.0f)) && (maxVal > dgFloat64 (0.0f))) {

				CSGConvexCurve tmp(mesh.GetAllocator());
				for (CSGConvexCurve::dgListNode* node = curve.GetFirst(); node; node = node->GetNext()) {
					tmp.Append(node->GetInfo());
				}
				curve.RemoveAll();

				if (!root->m_back) {
					root->m_back = new (mesh.GetAllocator()) dgMeshEffectSolidTree (normal, point);
				} else {
					dgHugeVector p0 (tmp.GetLast()->GetInfo());
					CSGConvexCurve& backFace = faces[stack];

					backFace.SetAllocator(mesh.GetAllocator());
					dgGoogol test0 = root->m_normal % (p0 - root->m_origin);
					for (CSGConvexCurve::dgListNode* node = tmp.GetFirst(); node; node = node->GetNext()) {

						dgHugeVector p1 (node->GetInfo());
						dgGoogol test1 = root->m_normal % (p1 - root->m_origin);
						if (test0.GetAproximateValue() <= dgFloat64 (0.0f)) {

							backFace.Append(p0);
							if (test1.GetAproximateValue() > dgFloat64 (0.0f)) {
								dgHugeVector dp (p1 - p0);
								dgGoogol den = root->m_normal % dp;
								dgHugeVector p (p0 + dp.Scale (-test0.GetAproximateValue() / den.GetAproximateValue()));
								backFace.Append(p);
							}

						} else if (test1.GetAproximateValue() < dgFloat64 (0.0f)) {
							dgHugeVector dp (p1 - p0);
							dgGoogol den = root->m_normal % dp;
							dgHugeVector p (p0 + dp.Scale (-test0.GetAproximateValue() / den.GetAproximateValue()));
							backFace.Append(p);
						}

						test0 = test1;
						p0 = p1;
					}

					// check Here because the clipper can generate a point and lines
					_ASSERTE (!backFace.GetCount() || (backFace.GetCount() >= 3));

					pool[stack] = root->m_back;
					stack ++;
					_ASSERTE (stack < (sizeof (pool)/sizeof (pool[0])));
				}

				if (!root->m_front) {
					root->m_front = new (mesh.GetAllocator())dgMeshEffectSolidTree (normal, point);
				} else {
					dgHugeVector p0 (tmp.GetLast()->GetInfo());
					CSGConvexCurve& frontFace = faces[stack];

					frontFace.SetAllocator(mesh.GetAllocator());
					dgGoogol test0 = root->m_normal % (p0 - root->m_origin);

					for (CSGConvexCurve::dgListNode* node = tmp.GetFirst(); node; node = node->GetNext()) {
						dgHugeVector p1 (node->GetInfo());
						dgGoogol test1 = root->m_normal % (p1 - root->m_origin);
						if (test0.GetAproximateValue() >= dgFloat64 (0.0f)) {

							frontFace.Append(p0);
							if (test1.GetAproximateValue() < dgFloat32 (0.0f)) {
								dgHugeVector dp (p1 - p0);
								dgGoogol den = root->m_normal % dp;
								dgHugeVector p (p0 + dp.Scale (-test0.GetAproximateValue() / den.GetAproximateValue()));
								frontFace.Append(p);
							}
						} else if (test1.GetAproximateValue() > dgFloat64 (0.0f)) {
							dgHugeVector dp (p1 - p0);
							dgGoogol den = root->m_normal % dp;
							dgHugeVector p (p0 + dp.Scale (-test0.GetAproximateValue() / den.GetAproximateValue()));
							frontFace.Append(p);
						}

						test0 = test1;
						p0 = p1;
					}

					// check Here because the clipper can generate a point and lines
					_ASSERTE (!frontFace.GetCount() || (frontFace.GetCount() >= 3));

					pool[stack] = root->m_front;
					stack ++;
					_ASSERTE (stack < (sizeof (pool)/sizeof (pool[0])));
				}

			} else {

				if (minVal < dgFloat64 (0.0f)) {
					if (!root->m_back) {
						root->m_back = new (mesh.GetAllocator())dgMeshEffectSolidTree (normal, point);
					} else {
						pool[stack] = root->m_back;
						stack ++;
						_ASSERTE (stack < (sizeof (pool)/sizeof (pool[0])));
					}

				} else if (maxVal > dgFloat64 (0.0f)) {
					if (!root->m_front) {
						root->m_front = new (mesh.GetAllocator())dgMeshEffectSolidTree (normal, point);
					} else {
						pool[stack] = root->m_front;
						stack ++;
						_ASSERTE (stack < (sizeof (pool)/sizeof (pool[0])));
					}
				}
			}
		}
	}
}




//void dgMeshTreeCSGFace::ClipFace (dgEdge* const face, const dgHugeVector& normal, const dgHugeVector& origin, dgEdge** const poolReplacement, dgInt32 poolCount, dgEdge** leftOut, dgEdge** rightOut)
void dgMeshTreeCSGFace::ClipFace (dgEdge* const face, const dgHugeVector& normal, const dgHugeVector& origin, dgEdge** leftOut, dgEdge** rightOut)
{
	dgEdge* left = NULL;
	dgEdge* right = NULL;
	dgEdge* ptr = face;
	dgEdge* lastEdge = face;
	dgMeshEffect::dgVertexAtribute p0 (m_points[ptr->m_incidentVertex]);
	dgGoogol test0 = normal % (dgHugeVector(p0.m_vertex) - origin);
	if (ptr->m_incidentVertex > m_baseCount) {
		if (fabs (test0.GetAproximateValue()) < dgFloat64 (1.0e-12f)) {
			test0 = dgGoogol(dgFloat64 (0.0f));
		}
	}

 	*leftOut = NULL; 
	*rightOut = NULL;	
	do {
		dgMeshEffect::dgVertexAtribute p1 (m_points[ptr->m_next->m_incidentVertex]);
		dgGoogol test1 = normal % (dgHugeVector (p1.m_vertex) - origin);
		if (ptr->m_next->m_incidentVertex > m_baseCount) {
			if (fabs (test1.GetAproximateValue()) < dgFloat64 (1.0e-12f)) {
				test1 = dgGoogol(dgFloat64 (0.0f));
			}
		}

		dgFloat64 val0 = test0.GetAproximateValue();
		dgFloat64 val1 = test1.GetAproximateValue();
		if (val0 < dgFloat64 (0.0f)) {
			if (val1 > dgFloat64 (0.0f)) {
				bool replaceface = false;
				if (lastEdge == ptr) {
					replaceface = true;
				}

				dgHugeVector dp (p1.m_vertex - p0.m_vertex);
				dgGoogol den (normal % dp);
				dgFloat64 t = -test0.GetAproximateValue() / den.GetAproximateValue();
				dgHugeVector q (dgHugeVector(p0.m_vertex) + dp.Scale (t));

				dgMeshEffect::dgVertexAtribute attr;
				attr.m_vertex = dgBigVector (q.m_x.GetAproximateValue(), q.m_y.GetAproximateValue(), q.m_z.GetAproximateValue(), dgFloat64 (0.0f));

				dgBigVector normal0(p0.m_normal_x, p0.m_normal_y, p0.m_normal_z, dgFloat64 (0.0f));
				dgBigVector normal1(p1.m_normal_x, p1.m_normal_y, p1.m_normal_z, dgFloat64 (0.0f));
				dgBigVector n (normal0 + (normal1 - normal0).Scale (t));
				n = n.Scale (dgFloat64 (1.0f) / sqrt (n % n)); 

				attr.m_normal_x = n.m_x;
				attr.m_normal_y = n.m_y;
				attr.m_normal_z = n.m_z; 

				attr.m_u0 = p0.m_u0 + (p1.m_u0 - p0.m_u0) * t;
				attr.m_v0 = p0.m_v0 + (p1.m_v0 - p0.m_v0) * t;

				attr.m_u1 = p0.m_u1 + (p1.m_u1 - p0.m_u1) * t;
				attr.m_v1 = p0.m_v1 + (p1.m_v1 - p0.m_v1) * t;

				attr.m_material = p0.m_material;

				ptr = SpliteEdge (m_count, ptr);
				if (replaceface) {
					lastEdge = ptr;
				}

				AddPoint (attr);
				ptr = ptr->m_next;
				right = ptr;

			} else if (val1 < dgFloat64 (0.0f)) {
				if (!left) {
					left = ptr;
				}
//			} else {
//				_ASSERTE (0);
			}

		} else if (val0 > dgFloat64 (0.0f)) {
			if (val1 > dgFloat64 (0.0f)) {
				if (!right) {
					right = ptr;
				}
			} else if (val1 < dgFloat64 (0.0f)) {
				bool replaceface = false;
				if (lastEdge == ptr) {
					replaceface = true;
				}

				dgHugeVector dp (p1.m_vertex - p0.m_vertex);
				dgGoogol den (normal % dp);
				dgFloat64 t = -test0.GetAproximateValue() / den.GetAproximateValue();
				dgHugeVector q (dgHugeVector(p0.m_vertex) + dp.Scale (t));

				dgMeshEffect::dgVertexAtribute attr;
				attr.m_vertex = dgBigVector (q.m_x.GetAproximateValue(), q.m_y.GetAproximateValue(), q.m_z.GetAproximateValue(), dgFloat64 (0.0f));

				dgBigVector normal0(p0.m_normal_x, p0.m_normal_y, p0.m_normal_z, dgFloat64 (0.0f));
				dgBigVector normal1(p1.m_normal_x, p1.m_normal_y, p1.m_normal_z, dgFloat64 (0.0f));
				dgBigVector n (normal0 + (normal1 - normal0).Scale (t));
				n = n.Scale (dgFloat64 (1.0f) / sqrt (n % n)); 

				attr.m_normal_x = n.m_x;
				attr.m_normal_y = n.m_y;
				attr.m_normal_z = n.m_z; 

				attr.m_u0 = p0.m_u0 + (p1.m_u0 - p0.m_u0) * t;
				attr.m_v0 = p0.m_v0 + (p1.m_v0 - p0.m_v0) * t;

				attr.m_u1 = p0.m_u1 + (p1.m_u1 - p0.m_u1) * t;
				attr.m_v1 = p0.m_v1 + (p1.m_v1 - p0.m_v1) * t;

				attr.m_material = p0.m_material;

				ptr = SpliteEdge (m_count, ptr);
				if (replaceface) {
					lastEdge = ptr;
				}

				AddPoint (attr);
				ptr = ptr->m_next;
				left = ptr;

//			} else {
//				_ASSERTE (0);
			}

		} else {
			if (val1 > dgFloat64 (0.0f)) {
				right = ptr;
			} else if (val1 < dgFloat64 (0.0f)) {
				left = ptr;
			}
		}

		test0 = test1;
		p0 = p1;

		ptr = ptr->m_next;
	} while (ptr != lastEdge);



	_ASSERTE (left || right);
	if (left && right) {
		*leftOut = left; 
		*rightOut = right;

		dgEdge* const edge = AddHalfEdge (left->m_incidentVertex, right->m_incidentVertex);
		dgEdge* const twin = AddHalfEdge (right->m_incidentVertex, left->m_incidentVertex);
		_ASSERTE (edge);
		_ASSERTE (twin);

		edge->m_incidentFace = right->m_incidentFace;
		twin->m_incidentFace = left->m_incidentFace;

		edge->m_twin = twin;
		twin->m_twin = edge;

		edge->m_prev = left->m_prev;
		left->m_prev->m_next = edge;

		twin->m_prev = right->m_prev;
		right->m_prev->m_next = twin;

		edge->m_next = right;
		right->m_prev = edge;

		twin->m_next = left;
		left->m_prev = twin;

	} else if (left) {
		_ASSERTE (!right);
		*leftOut = left;
	} else {
		_ASSERTE (right);
		*rightOut = right;
	}
}



