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
#include "dgBody.h"
#include "dgWorld.h"
#include "dgMeshEffect.h"
#include "dgCollisionConvexHull.h"
#include "dgMeshEffectSolidTree.h"



/*
class dgClusterFace
{
	public:
	dgClusterFace()
	{
	}
	~dgClusterFace()
	{
	}

	dgEdge* m_edge;
	dgFloat32 m_area;
	dgFloat32 m_perimeter;
	dgVector m_normal;
};

class dgClusterList: public dgList <dgClusterFace>
{	
	public:
	dgClusterList(dgMemoryAllocator* const allocator) 
		:dgList <dgClusterFace>(allocator)
	{
	}

	~dgClusterList()
	{
	}


	int AddVertexToPool (const dgMeshEffect& mesh, dgVector* const vertexPool, dgInt32* const vertexMarks, dgInt32 vertexMark)
	{
		int count = 0;

		const dgVector* const points = (dgVector*) mesh.GetVertexPool();
		for (dgListNode* node = GetFirst(); node; node = node->GetNext()) {
			dgClusterFace& face = node->GetInfo();

			dgEdge* edge = face.m_edge;
			do {
				dgInt32 index = edge->m_incidentVertex;
				if (vertexMarks[index] != vertexMark) {
					vertexMarks[index] = vertexMark;
					vertexPool[count] = points[index];
					count ++;
				}
				edge = edge->m_next;
			} while (edge != face.m_edge);
		}
		return count;
	}

	dgFloat32 CalculateTriagleConcavity2 (const dgCollisionConvexHull& collision, dgClusterFace& info, const dgEdge* const triangle, const dgVector* const points) const 
	{
		int stack = 1;
		dgVector pool[256][3];
		pool[0][0] = points[triangle->m_incidentVertex];
		pool[0][1] = points[triangle->m_next->m_incidentVertex];
		pool[0][2] = points[triangle->m_next->m_next->m_incidentVertex];

		dgFloat32 concavity = dgFloat32 (0.0f);
		const dgFloat32 minArea = dgFloat32 (0.125f); 
		const dgFloat32 minArea2 = minArea * minArea * 0.5f;
		

		const dgVector step (info.m_normal.Scale (dgFloat32 (4.0f) * ((dgCollision&)collision).GetBoxMaxRadius()));
		while (stack) {
			stack --;
			dgVector p0 (pool[stack][0]);
			dgVector p1 (pool[stack][1]);
			dgVector p2 (pool[stack][2]);
			dgVector q1 ((p0 + p1 + p2).Scale (dgFloat32 (1.0f / 3.0f)));
			dgVector q0 (q1 + step);

			dgContactPoint contactOut;
			dgFloat32 param = collision.RayCast(q0, q1, contactOut, NULL, NULL, NULL);
			if (param > dgFloat32 (1.0f)) {
				param = dgFloat32 (1.0f);
			}
			dgVector dq (step.Scale (dgFloat32 (1.0f) - param));
			dgFloat32 lenght2 = dq % dq;
			if (lenght2 > concavity) {
				concavity = lenght2;
			}


			if ((stack + 3) <= sizeof (pool) / (3 * sizeof (pool[0][0]))) {
				dgVector edge10 (p1 - p0);
				dgVector edge20 (p2 - p0);
				dgVector n (edge10 * edge20);
				dgFloat32 area2 = n % n;
				if (area2 > minArea2) {
					dgVector p01 ((p0 + p1).Scale (dgFloat32 (0.5f)));
					dgVector p12 ((p1 + p2).Scale (dgFloat32 (0.5f)));
					dgVector p20 ((p2 + p0).Scale (dgFloat32 (0.5f)));

					pool[stack][0] = p0;
					pool[stack][1] = p01;
					pool[stack][2] = p20;

					pool[stack + 1][0] = p1;
					pool[stack + 1][1] = p12;
					pool[stack + 1][2] = p01;

					pool[stack + 2][0] = p2;
					pool[stack + 2][1] = p20;
					pool[stack + 2][2] = p12;

					stack += 3;
				}
			}
		}
		return concavity;
	}

	dgFloat32 CalculateConcavity2 (const dgCollisionConvexHull& collision, const dgMeshEffect& mesh)
	{
		dgFloat32 concavity = dgFloat32 (0.0f);
		const dgVector* const points = (dgVector*) mesh.GetVertexPool();
		for (dgListNode* node = GetFirst(); node; node = node->GetNext()) {
			dgClusterFace& info = node->GetInfo();
			for (dgEdge* edge = info.m_edge->m_next->m_next; edge != info.m_edge; edge = edge = edge->m_next) {
				dgFloat32 val = CalculateTriagleConcavity2 (collision, info, edge->m_prev->m_prev, points); 
				if (val > concavity) {
					concavity = val;
				}
			}
		}
		return concavity;
	}


	dgFloat32 m_area;
	dgFloat32 m_perimeter;
};


class dgPairProxi
{
	public:
	dgPairProxi()
	{
		m_edgeA = NULL;
		m_edgeB = NULL;
		m_area = dgFloat32 (0.0f);
		m_perimeter = dgFloat32 (0.0f);
	}
	dgEdge* m_edgeA;
	dgEdge* m_edgeB;
	dgFloat32 m_area;
	dgFloat32 m_perimeter;
};



dgMeshEffect::dgMeshEffect (const dgMeshEffect& source, dgFloat32 absoluteconcavity)
	:dgPolyhedra(source.GetAllocator())
{
	Init(true);

	dgMeshEffect mesh (source);
	int faceCount = mesh.GetTotalFaceCount() + 1;
	dgStack<dgClusterList> clusterPool(faceCount);
	dgClusterList* const clusters = &clusterPool[0];
	for (dgInt32 i = 0; i < faceCount; i ++) {
		clusters[i] = dgClusterList(mesh.GetAllocator());
	}

	int faceIndex = 1;
	int meshMask = mesh.IncLRU();
	const dgVector* const points = mesh.m_points;

	// enumerate all faces, and initialize cluster pool
	dgMeshEffect::Iterator iter (mesh);
	
	for (iter.Begin(); iter; iter ++) {
		dgEdge* edge = &(*iter);
		edge->m_userData = 0;
		if ((edge->m_mark != meshMask) && (edge->m_incidentFace > 0)) {
			dgFloat32 perimeter = dgFloat32 (0.0f);
			dgVector p0 (points[edge->m_prev->m_incidentVertex]);
			dgEdge* ptr = edge;
			do {
				dgVector p1 (points[ptr->m_incidentVertex]);
				dgVector p1p0 (p1 - p0);
				perimeter += dgSqrt (p1p0 % p1p0);
				p0 = p1;

				ptr->m_incidentFace = faceIndex;

				ptr->m_mark = meshMask;	
				ptr = ptr->m_next;
			} while (ptr != edge);

			dgVector normal = mesh.FaceNormal(edge, &points[0][0], sizeof (dgVector));
			dgFloat32 mag = dgSqrt (normal % normal);

			dgClusterFace& faceInfo = clusters[faceIndex].Append()->GetInfo();

			faceInfo.m_edge = edge;
			faceInfo.m_perimeter = perimeter;
			faceInfo.m_area = dgFloat32 (0.5f) * mag;
			faceInfo.m_normal = normal.Scale (1.0f / mag);

			clusters[faceIndex].m_perimeter = perimeter; 
			clusters[faceIndex].m_area = faceInfo.m_area;

			faceIndex ++;
		}
	}

	_ASSERTE (faceCount == faceIndex);

	// recalculate all edge cost
	dgMatrix matrix (dgGetIdentityMatrix());
	dgStack<int> vertexMarks (mesh.GetVertexCount());
	dgStack<dgVector> vertexArray (mesh.GetVertexCount() * 2);

	dgVector* const vertexPool = &vertexArray[0];
	memset (&vertexMarks[0], 0, vertexMarks.GetSizeInBytes());

	dgList<dgPairProxi> proxiList (mesh.GetAllocator());
	dgUpHeap<dgList<dgPairProxi>::dgListNode*, dgFloat32> heap (mesh.GetCount() + 1000, mesh.GetAllocator());
	
	int vertexMark = 0;

	dgFloat32 diagonalInv = dgFloat32 (1.0f);
	dgFloat32 aspectRatioCoeficent = absoluteconcavity / dgFloat32 (100.0f);
	meshMask = mesh.IncLRU();
	for (int faceIndex = 1; faceIndex < faceCount; faceIndex ++) {
		dgClusterList& clusterListA = clusters[faceIndex];
		_ASSERTE (clusterListA.GetFirst()->GetInfo().m_edge->m_incidentFace == faceIndex); 

		vertexMark ++;
		int vertexCount = clusterListA.AddVertexToPool (mesh, &vertexPool[0], &vertexMarks[0], vertexMark);
		dgClusterFace& clusterFaceA = clusterListA.GetFirst()->GetInfo();
		dgEdge* edge = clusterFaceA.m_edge;
		do {
			dgInt32 twinFaceIndex = edge->m_twin->m_incidentFace;

			if ((edge->m_mark != meshMask) && (twinFaceIndex != faceIndex) && (twinFaceIndex > 0)) {

				dgClusterList& clusterListB = clusters[twinFaceIndex];

				vertexMark ++;
				int extraCount = clusterListB.AddVertexToPool (mesh, &vertexPool[vertexCount], &vertexMarks[0], vertexMark);

				int count = vertexCount + extraCount;
				dgCollisionConvexHull collision (mesh.GetAllocator(), 0, count, sizeof (dgVector), 0.0f, &vertexPool[0].m_x, matrix);

				dgFloat32 concavity = dgFloat32 (0.0f);
				if (collision.GetVertexCount()) {
					concavity = dgSqrt (GetMax(clusterListA.CalculateConcavity2 (collision, mesh), clusterListB.CalculateConcavity2 (collision, mesh)));
					if (concavity < dgFloat32 (1.0e-3f)) {
						concavity = dgFloat32 (0.0f);
					}
				}

				dgVector p0 (points[edge->m_incidentVertex]);
				dgVector p1 (points[edge->m_twin->m_incidentVertex]);
				dgVector p1p0 (p1 - p0);
				dgFloat32 edgeLength = dgFloat32 (2.0f) * dgSqrt (p1p0 % p1p0);

				dgFloat32 area = clusterListA.m_area + clusterListB.m_area;
				dgFloat32 perimeter = clusterListA.m_perimeter + clusterListB.m_perimeter - edgeLength;
				dgFloat32 edgeCost = perimeter * perimeter / (dgFloat32 (4.0f * 3.141592f) * area);
				dgFloat32 cost = diagonalInv * (concavity + edgeCost * aspectRatioCoeficent);

				dgList<dgPairProxi>::dgListNode* pairNode = proxiList.Append();
				dgPairProxi& pair = pairNode->GetInfo();
				pair.m_edgeA = edge;
				pair.m_edgeB = edge->m_twin;
				pair.m_area = area;
				pair.m_perimeter = perimeter;
				edge->m_userData = dgUnsigned64 (pairNode);
				edge->m_twin->m_userData = dgUnsigned64 (pairNode);
				heap.Push(pairNode, cost);
			}

			edge->m_mark = meshMask;
			edge->m_twin->m_mark = meshMask;

			edge = edge->m_next;
		} while (edge != clusterFaceA.m_edge);
	}

	while (heap.GetCount() && (heap.Value() < absoluteconcavity)) {

		dgList<dgPairProxi>::dgListNode* pairNode = heap[0];
		heap.Pop();
		dgPairProxi& pair = pairNode->GetInfo();

		_ASSERTE ((pair.m_edgeA && pair.m_edgeA) || (!pair.m_edgeA && !pair.m_edgeA));
		if (pair.m_edgeA && pair.m_edgeB) {

			_ASSERTE (pair.m_edgeA->m_incidentFace != pair.m_edgeB->m_incidentFace);
			if (pair.m_edgeA->m_incidentFace > pair.m_edgeB->m_incidentFace) {
				Swap (pair.m_edgeA->m_incidentFace, pair.m_edgeB->m_incidentFace);			
			}

			dgInt32 faceIndexA = pair.m_edgeA->m_incidentFace;
			dgInt32 faceIndexB = pair.m_edgeB->m_incidentFace;
			dgClusterList& listA = clusters[faceIndexA];
			dgClusterList& listB = clusters[faceIndexB];

			while (listB.GetFirst()) {
				dgClusterList::dgListNode* nodeB = listB.GetFirst();
				listB.Unlink(nodeB);
				dgClusterFace& faceB = nodeB->GetInfo();

				dgEdge* ptr = faceB.m_edge;
				do {
					ptr->m_incidentFace = faceIndexA;
					ptr = ptr->m_next;
				} while (ptr != faceB.m_edge);
				listA.Append (nodeB);
			}
			listA.m_area = pair.m_area; 
			listA.m_perimeter = pair.m_perimeter;

			dgInt32 mark = mesh.IncLRU();
			for (dgClusterList::dgListNode* node = listA.GetFirst(); node; node = node->GetNext()) {
				dgClusterFace& face = node->GetInfo();
				dgEdge* ptr = face.m_edge;
				do {
					if (ptr->m_userData) {
						dgPairProxi& pairNode = *((dgPairProxi*)ptr->m_userData);
						pairNode.m_edgeA = NULL;
						pairNode.m_edgeB = NULL;
					}
					ptr->m_userData = 0;
					ptr->m_twin->m_userData = 0;

					if ((ptr->m_twin->m_incidentFace == faceIndexA) || (ptr->m_twin->m_incidentFace < 0)){
						ptr->m_mark = mark;
						//ptr->m_userData = 0;
						ptr->m_twin->m_mark = mark;
						//ptr->m_twin->m_userData = 0;
					}	

					if (ptr->m_mark != mark) {
						dgClusterList& adjacentList = clusters[ptr->m_twin->m_incidentFace];
						for (dgClusterList::dgListNode* adjacentNode = adjacentList.GetFirst(); adjacentNode; adjacentNode = adjacentNode->GetNext()) {
							dgClusterFace& adjacentFace = adjacentNode->GetInfo();
							dgEdge* adjecentEdge = adjacentFace.m_edge;
							do {
								if (adjecentEdge->m_twin->m_incidentFace == faceIndexA) {
									adjecentEdge->m_twin->m_mark = mark;
								}
								adjecentEdge = adjecentEdge->m_next;
							} while (adjecentEdge != adjacentFace.m_edge);
						}
						ptr->m_mark = mark - 1;
					}
					ptr = ptr->m_next;
				} while (ptr != face.m_edge);
			}

			vertexMark ++;
			int vertexCount = listA.AddVertexToPool (mesh, &vertexPool[0], &vertexMarks[0], vertexMark);
			for (dgClusterList::dgListNode* node = listA.GetFirst(); node; node = node->GetNext()) {
				dgClusterFace& face = node->GetInfo();
				dgEdge* edge = face.m_edge;
				do {
					if ((edge->m_mark != mark) && (edge->m_twin->m_incidentFace > 0)) {
						dgEdge* twin = edge->m_twin;
						_ASSERTE (edge->m_userData == 0);
						_ASSERTE (twin->m_userData == 0);
						dgClusterList& neighbourgCluster = clusters[twin->m_incidentFace];


						vertexMark ++;
						int extraCount = neighbourgCluster.AddVertexToPool (mesh, &vertexPool[vertexCount], &vertexMarks[0], vertexMark);
						int count = vertexCount + extraCount;

						dgCollisionConvexHull collision (mesh.GetAllocator(), 0, count, sizeof (dgVector), 0.0f, &vertexPool[0].m_x, matrix);

						dgFloat32 concavity = dgFloat32 (0.0f);
						if (collision.GetVertexCount()) {
							concavity = dgSqrt (GetMax(listA.CalculateConcavity2 (collision, mesh), neighbourgCluster.CalculateConcavity2 (collision, mesh)));
							if (concavity < dgFloat32 (1.0e-3f)) {
								concavity = dgFloat32 (0.0f);
							} else {
								concavity *= dgFloat32 (1.0f);
							}
						}

						dgFloat32 edgeLength = dgFloat32 (0.0f);
						dgClusterList& adjacentList = clusters[edge->m_twin->m_incidentFace];
						for (dgClusterList::dgListNode* adjacentNode = adjacentList.GetFirst(); adjacentNode; adjacentNode = adjacentNode->GetNext()) {
							dgClusterFace& adjacentFace = adjacentNode->GetInfo();
							dgEdge* adjecentEdge = adjacentFace.m_edge;
							do {
								if (adjecentEdge->m_twin->m_incidentFace == faceIndexA) {
									dgVector p0 (points[adjecentEdge->m_incidentVertex]);
									dgVector p1 (points[adjecentEdge->m_twin->m_incidentVertex]);
									dgVector p1p0 (p1 - p0);
									edgeLength += dgFloat32 (2.0f) * dgSqrt (p1p0 % p1p0);
								}
								adjecentEdge = adjecentEdge->m_next;
							} while (adjecentEdge != adjacentFace.m_edge);
						}
						

						dgFloat32 area = listA.m_area + adjacentList.m_area;
						dgFloat32 perimeter = listA.m_perimeter + adjacentList.m_perimeter - edgeLength;
						dgFloat32 edgeCost = perimeter * perimeter / (dgFloat32 (4.0f * 3.141592f) * area);
						dgFloat32 cost = diagonalInv * (concavity + edgeCost * aspectRatioCoeficent);

						dgList<dgPairProxi>::dgListNode* pairNode = proxiList.Append();

						dgPairProxi& pair = pairNode->GetInfo();
						pair.m_edgeA = edge;
						pair.m_edgeB = edge->m_twin;
						pair.m_area = area;
						pair.m_perimeter = perimeter;
						edge->m_userData = dgUnsigned64 (pairNode);
						edge->m_twin->m_userData = dgUnsigned64 (pairNode);

						if ((heap.GetCount() + 20) > heap.GetMaxCount()) {
							for (dgInt32 i = heap.GetCount() - 1; i >= 0; i --) {
								dgList<dgPairProxi>::dgListNode* emptyNode = heap[i];
								dgPairProxi& emptyPair = emptyNode->GetInfo();
								if ((emptyPair.m_edgeA == NULL) && (emptyPair.m_edgeB == NULL)) {
									heap.Remove(i);
								}
							}
						}
						heap.Push(pairNode, cost);
					}


					edge = edge->m_next;						
				} while (edge != face.m_edge);
			}
		}

		proxiList.Remove(pairNode);
	}



	BeginPolygon();
	
	dgFloat32 layer = dgFloat32 (0.0f);
	dgVertexAtribute polygon[128];
	memset (polygon, 0, sizeof (polygon));
	for (dgInt32 i = 0; i < faceCount; i ++) {
		dgClusterList& clusterList = clusters[i];
		
		if (clusterList.GetCount()) {
			for (dgClusterList::dgListNode* node = clusterList.GetFirst(); node; node = node->GetNext()) {
				dgClusterFace& face = node->GetInfo();
				dgEdge* edge = face.m_edge;
				dgEdge* sourceEdge = source.FindEdge(edge->m_incidentVertex, edge->m_twin->m_incidentVertex);
				int count = 0;
				do {
					dgInt32 index = edge->m_incidentVertex;
					polygon[count] = source.m_attib[dgInt32 (sourceEdge->m_userData)];
					polygon[count].m_vertex = points[index];
					polygon[count].m_vertex.m_w = layer;

					count ++;
					sourceEdge = sourceEdge->m_next;
					edge = edge->m_next;
				} while (edge != face.m_edge);
				AddPolygon(count, &polygon[0].m_vertex.m_x, sizeof (dgVertexAtribute), 0);
			}
			layer += dgFloat32 (1.0f);
		}
		clusters[i].~dgClusterList();
	}
	EndPolygon();
	ConvertToPolygons();
}
*/



/*
class dgFlatClipEdgeAttr
{
	public:
	dgInt32 m_rightIndex;
	dgInt32 m_leftIndex;
	dgInt32 m_leftEdgeAttr;
	dgInt32 m_leftTwinAttr;
	dgInt32 m_rightEdgeAttr;
	dgInt32 m_rightTwinAttr;
	dgEdge* m_edge;
	dgEdge* m_twin;
};
*/





dgMeshEffect::dgMeshEffect(dgMemoryAllocator* const allocator, bool preAllocaBuffers)
	:dgPolyhedra(allocator)
{
	Init(preAllocaBuffers);
}

dgMeshEffect::dgMeshEffect (dgMemoryAllocator* const allocator, const dgMatrix& planeMatrix, dgFloat32 witdth, dgFloat32 breadth, dgInt32 material, const dgMatrix& textureMatrix0, const dgMatrix& textureMatrix1)
	:dgPolyhedra(allocator)
{
	_ASSERTE (0);
/*
	dgVector face[4];
	dgInt32 index[4];
	dgInt64 attrIndex[4];

	Init(true);

	m_isFlagFace = 1;
	face[0] = dgVector (dgFloat32 (0.0f), -witdth, -breadth, dgFloat32 (0.0f));
	face[1] = dgVector (dgFloat32 (0.0f),  witdth, -breadth, dgFloat32 (0.0f));
	face[2] = dgVector (dgFloat32 (0.0f),  witdth,  breadth, dgFloat32 (0.0f));
	face[3] = dgVector (dgFloat32 (0.0f), -witdth,  breadth, dgFloat32 (0.0f));

	for (dgInt32 i = 0; i < 4; i ++) {
		dgVector uv0 (textureMatrix0.TransformVector(face[i]));
		dgVector uv1 (textureMatrix1.TransformVector(face[i]));

		m_points[i] = planeMatrix.TransformVector(face[i]);

		m_attib[i].m_vertex.m_x = m_points[i].m_x;
		m_attib[i].m_vertex.m_y = m_points[i].m_y;
		m_attib[i].m_vertex.m_z = m_points[i].m_z;

		m_attib[i].m_normal.m_x = planeMatrix.m_front.m_x;
		m_attib[i].m_normal.m_y = planeMatrix.m_front.m_y;
		m_attib[i].m_normal.m_z = planeMatrix.m_front.m_z;
		
		m_attib[i].m_u0 = uv0.m_y;
		m_attib[i].m_v0 = uv0.m_z;

		m_attib[i].m_u1 = uv1.m_y;
		m_attib[i].m_v1 = uv1.m_z;

		m_attib[i].m_material = material;

		index[i] = i;
		attrIndex[i] = i;
	}

	m_pointCount = 4;
	m_atribCount = 4;
	BeginFace();
	AddFace (4, index, attrIndex);
	EndFace();
*/
}


dgMeshEffect::dgMeshEffect(dgPolyhedra& mesh, const dgMeshEffect& source)
	:dgPolyhedra (mesh) 
{
	m_isFlagFace = source.m_isFlagFace;
	m_pointCount = source.m_pointCount;
	m_maxPointCount = source.m_maxPointCount;
	m_points = (dgBigVector*) GetAllocator()->MallocLow(dgInt32 (m_maxPointCount * sizeof(dgBigVector)));
	memcpy (m_points, source.m_points, m_pointCount * sizeof(dgBigVector));

	m_atribCount = source.m_atribCount;
	m_maxAtribCount = source.m_maxAtribCount;
	m_attib = (dgVertexAtribute*) GetAllocator()->MallocLow(dgInt32 (m_maxAtribCount * sizeof(dgVertexAtribute)));
	memcpy (m_attib, source.m_attib, m_atribCount * sizeof(dgVertexAtribute));
}


dgMeshEffect::dgMeshEffect(const dgMeshEffect& source)
	:dgPolyhedra (source) 
{
	m_isFlagFace = source.m_isFlagFace;
	m_pointCount = source.m_pointCount;
	m_maxPointCount = source.m_maxPointCount;
	m_points = (dgBigVector*) GetAllocator()->MallocLow(dgInt32 (m_maxPointCount * sizeof(dgBigVector)));
	memcpy (m_points, source.m_points, m_pointCount * sizeof(dgBigVector));

	m_atribCount = source.m_atribCount;
	m_maxAtribCount = source.m_maxAtribCount;
	m_attib = (dgVertexAtribute*) GetAllocator()->MallocLow(dgInt32 (m_maxAtribCount * sizeof(dgVertexAtribute)));
	memcpy (m_attib, source.m_attib, m_atribCount * sizeof(dgVertexAtribute));
}


dgMeshEffect::dgMeshEffect(dgCollision* const collision)
	:dgPolyhedra (collision->GetAllocator()) 
{
	class dgMeshEffectBuilder
	{
		public:
		dgMeshEffectBuilder ()
		{
			m_brush = 0;
			m_faceCount = 0;
			m_vertexCount = 0;
			m_maxFaceCount = 32;
			m_maxVertexCount = 32;
			m_vertex = (dgVector*) dgMallocStack(m_maxVertexCount * sizeof(dgVector));
			m_faceIndexCount = (dgInt32*) dgMallocStack(m_maxFaceCount * sizeof(dgInt32));
		}

		~dgMeshEffectBuilder ()
		{
			dgFreeStack (m_faceIndexCount);
			dgFreeStack (m_vertex);
		}

		static void GetShapeFromCollision (void* userData, dgInt32 vertexCount, const dgFloat32* faceVertex, dgInt32 id)
		{
			dgInt32 vertexIndex; 
			dgMeshEffectBuilder& builder = *((dgMeshEffectBuilder*)userData);


			if (builder.m_faceCount >= builder.m_maxFaceCount) {
				dgInt32* index;

				builder.m_maxFaceCount *= 2;
				index = (dgInt32*) dgMallocStack(builder.m_maxFaceCount * sizeof(dgInt32));
				memcpy (index, builder.m_faceIndexCount, builder.m_faceCount * sizeof(dgInt32));
				dgFreeStack(builder.m_faceIndexCount);
				builder.m_faceIndexCount = index;
			}
			builder.m_faceIndexCount[builder.m_faceCount] = vertexCount;
			builder.m_faceCount = builder.m_faceCount + 1;

			vertexIndex = builder.m_vertexCount; 
			dgFloat32 brush = dgFloat32 (builder.m_brush);
			for (dgInt32 i = 0; i < vertexCount; i ++) {
				if (vertexIndex >= builder.m_maxVertexCount) {
					dgVector* points;

					builder.m_maxVertexCount *= 2;
					points = (dgVector*) dgMallocStack(builder.m_maxVertexCount * sizeof(dgVector));
					memcpy (points, builder.m_vertex, vertexIndex * sizeof(dgVector));
					dgFreeStack(builder.m_vertex);
					builder.m_vertex = points;
				}

				builder.m_vertex[vertexIndex].m_x = faceVertex[i * 3 + 0];
				builder.m_vertex[vertexIndex].m_y = faceVertex[i * 3 + 1];
				builder.m_vertex[vertexIndex].m_z = faceVertex[i * 3 + 2];
				builder.m_vertex[vertexIndex].m_w = brush;
				vertexIndex ++;
			}

			builder.m_vertexCount = vertexIndex;
		}

		dgInt32 m_brush;
		dgInt32 m_vertexCount;
		dgInt32 m_maxVertexCount;

		dgInt32 m_faceCount;
		dgInt32 m_maxFaceCount;

		dgVector* m_vertex;
		dgInt32* m_faceIndexCount;
	};

	dgMeshEffectBuilder builder;

	if (collision->IsType (dgCollision::dgCollisionCompound_RTTI)) {
		dgCollisionInfo collisionInfo;
		collision->GetCollisionInfo (&collisionInfo);

		dgMatrix matrix (collisionInfo.m_offsetMatrix);
		dgCollisionInfo::dgCoumpountCollisionData& data = collisionInfo.m_compoundCollision;
		for (dgInt32 i = 0; i < data.m_chidrenCount; i ++) {
			builder.m_brush = i;
			dgCollision* const childShape = data.m_chidren[i];
			childShape->DebugCollision (matrix, (OnDebugCollisionMeshCallback) dgMeshEffectBuilder::GetShapeFromCollision, &builder);
		}

	} else {
		dgMatrix matrix (dgGetIdentityMatrix());
		collision->DebugCollision (matrix, (OnDebugCollisionMeshCallback) dgMeshEffectBuilder::GetShapeFromCollision, &builder);
	}

	dgStack<dgInt32>indexList (builder.m_vertexCount);

	dgVertexListToIndexList (&builder.m_vertex[0].m_x, sizeof (dgVector), sizeof (dgVector), 0, builder.m_vertexCount, &indexList[0], DG_VERTEXLIST_INDEXLIST_TOL);	

	dgStack<dgInt32> materialIndex(builder.m_faceCount);
	dgStack<dgInt32> m_normalUVIndex(builder.m_vertexCount);

	dgVector normalUV(dgFloat32 (0.0f), dgFloat32 (0.0f), dgFloat32 (0.0f), dgFloat32 (0.0f));

	memset (&materialIndex[0], 0, size_t (materialIndex.GetSizeInBytes()));
	memset (&m_normalUVIndex[0], 0, size_t (m_normalUVIndex.GetSizeInBytes()));

	Init(true);
	BuildFromVertexListIndexList(builder.m_faceCount, builder.m_faceIndexCount, &materialIndex[0],
								 &builder.m_vertex[0].m_x, sizeof (dgVector), &indexList[0],
								 &normalUV.m_x, sizeof (dgVector), &m_normalUVIndex[0],
								 &normalUV.m_x, sizeof (dgVector), &m_normalUVIndex[0],
								 &normalUV.m_x, sizeof (dgVector), &m_normalUVIndex[0]);

	CalculateNormals(dgFloat32 (45.0f * 3.1416f/180.0f));
}



dgMeshEffect::~dgMeshEffect(void)
{
	GetAllocator()->FreeLow (m_points);
	GetAllocator()->FreeLow (m_attib);
}


void dgMeshEffect::Init(bool preAllocaBuffers)
{
	m_isFlagFace = 0;
	m_pointCount = 0;
	m_atribCount = 0;
	m_maxPointCount = DG_MESH_EFFECT_INITIAL_VERTEX_SIZE;
	m_maxAtribCount = DG_MESH_EFFECT_INITIAL_VERTEX_SIZE;

	m_points = NULL;
	m_attib = NULL;
	if (preAllocaBuffers) {
		m_points = (dgBigVector*) GetAllocator()->MallocLow(dgInt32 (m_maxPointCount * sizeof(dgBigVector)));
		m_attib = (dgVertexAtribute*) GetAllocator()->MallocLow(dgInt32 (m_maxAtribCount * sizeof(dgVertexAtribute)));
	}
}


void dgMeshEffect::Triangulate  ()
{
	dgPolyhedra polygon(GetAllocator());

	dgInt32 mark = IncLRU();
	polygon.BeginFace();
	dgPolyhedra::Iterator iter (*this);
	for (iter.Begin(); iter; iter ++){
		dgEdge* const face = &(*iter);

		if ((face->m_mark != mark) && (face->m_incidentFace > 0)) {
			dgInt32	index[DG_MESH_EFFECT_POINT_SPLITED];

			dgEdge* ptr = face;
			dgInt32 indexCount = 0;
			do {
				dgInt32 attribIndex = dgInt32 (ptr->m_userData);
				m_attib[attribIndex].m_vertex.m_w = dgFloat64 (ptr->m_incidentVertex);
				ptr->m_mark = mark;
				index[indexCount] = attribIndex;
				indexCount ++;
				ptr = ptr->m_next;
			} while (ptr != face);
			polygon.AddFace(indexCount, index);
		}
	}
	polygon.EndFace();


	dgPolyhedra leftOversOut(GetAllocator());
	polygon.Triangulate(&m_attib[0].m_vertex.m_x, sizeof (dgVertexAtribute), &leftOversOut);
	_ASSERTE (leftOversOut.GetCount() == 0);


	RemoveAll();
	SetLRU (0);

	mark = polygon.IncLRU();
	BeginFace();
	dgPolyhedra::Iterator iter1 (polygon);
	for (iter1.Begin(); iter1; iter1 ++){
		dgEdge* const face = &(*iter1);
		if ((face->m_mark != mark) && (face->m_incidentFace > 0)) {
			dgInt32	index[DG_MESH_EFFECT_POINT_SPLITED];
			dgInt64	userData[DG_MESH_EFFECT_POINT_SPLITED];

			dgEdge* ptr = face;
			dgInt32 indexCount = 0;
			do {
				ptr->m_mark = mark;
				index[indexCount] = dgInt32 (m_attib[ptr->m_incidentVertex].m_vertex.m_w);

				userData[indexCount] = ptr->m_incidentVertex;
				indexCount ++;
				ptr = ptr->m_next;
			} while (ptr != face);
			AddFace(indexCount, index, userData);
		}
	}
	EndFace();

	for (iter.Begin(); iter; iter ++){
		dgEdge* const face = &(*iter);
		if (face->m_incidentFace > 0) {
			dgInt32 attribIndex = dgInt32 (face->m_userData);
			m_attib[attribIndex].m_vertex.m_w = m_points[face->m_incidentVertex].m_w;
		}
	}

}

void dgMeshEffect::ConvertToPolygons ()
{
	dgPolyhedra polygon(GetAllocator());

	dgInt32 mark = IncLRU();
	polygon.BeginFace();
	dgPolyhedra::Iterator iter (*this);
	for (iter.Begin(); iter; iter ++){
		dgEdge* const face = &(*iter);
		if ((face->m_mark != mark) && (face->m_incidentFace > 0)) {
			dgInt32	index[DG_MESH_EFFECT_POINT_SPLITED];

			dgEdge* ptr = face;
			dgInt32 indexCount = 0;
			do {
				dgInt32 attribIndex = dgInt32 (ptr->m_userData);
				m_attib[attribIndex].m_vertex.m_w = dgFloat32 (ptr->m_incidentVertex);
				ptr->m_mark = mark;
				index[indexCount] = attribIndex;
				indexCount ++;
				ptr = ptr->m_next;
			} while (ptr != face);
			polygon.AddFace(indexCount, index);
		}
	}
	polygon.EndFace();


	dgPolyhedra leftOversOut(GetAllocator());
	polygon.ConvexPartition (&m_attib[0].m_vertex.m_x, sizeof (dgVertexAtribute), &leftOversOut);
	_ASSERTE (leftOversOut.GetCount() == 0);

	RemoveAll();
	SetLRU (0);

	mark = polygon.IncLRU();
	BeginFace();
	dgPolyhedra::Iterator iter1 (polygon);
	for (iter1.Begin(); iter1; iter1 ++){
		dgEdge* const face = &(*iter1);
		if ((face->m_mark != mark) && (face->m_incidentFace > 0)) {
			dgInt32	index[DG_MESH_EFFECT_POINT_SPLITED];
			dgInt64	userData[DG_MESH_EFFECT_POINT_SPLITED];

			dgEdge* ptr = face;
			dgInt32 indexCount = 0;
			do {
				ptr->m_mark = mark;
				index[indexCount] = dgInt32 (m_attib[ptr->m_incidentVertex].m_vertex.m_w);

				userData[indexCount] = ptr->m_incidentVertex;
				indexCount ++;
				ptr = ptr->m_next;
			} while (ptr != face);
			AddFace(indexCount, index, userData);
		}
	}
	EndFace();

	for (iter.Begin(); iter; iter ++){
		dgEdge* const face = &(*iter);
		if (face->m_incidentFace > 0) {
			dgInt32 attribIndex = dgInt32 (face->m_userData);
			m_attib[attribIndex].m_vertex.m_w = m_points[face->m_incidentVertex].m_w;
		}
	}
}

void dgMeshEffect::RemoveUnusedVertices(dgInt32* const vertexMap)
{
	dgPolyhedra polygon(GetAllocator());
	dgStack<dgInt32>attrbMap(m_atribCount);

	memset(&vertexMap[0], -1, m_pointCount * sizeof (int));
	memset(&attrbMap[0], -1, m_atribCount * sizeof (int));

	int attribCount = 0;
	int vertexCount = 0;

	dgStack<dgBigVector>points (m_pointCount);
	dgStack<dgVertexAtribute>atributes (m_atribCount);

	int mark = IncLRU();
	polygon.BeginFace();
	dgPolyhedra::Iterator iter (*this);
	for (iter.Begin(); iter; iter ++){
		dgEdge* const face = &(*iter);
		if ((face->m_mark != mark) && (face->m_incidentFace > 0)) {
			dgInt32	vertex[DG_MESH_EFFECT_POINT_SPLITED];
			dgInt64	userData[DG_MESH_EFFECT_POINT_SPLITED];
			int indexCount = 0;
			dgEdge* ptr = face;
			do {
				ptr->m_mark = mark;

				int index = ptr->m_incidentVertex;
				if (vertexMap[index] == -1) {
					vertexMap[index] = vertexCount;
					points[vertexCount] = m_points[index];
					vertexCount ++;
				}
				vertex[indexCount] = vertexMap[index];

				index = int (ptr->m_userData);
				if (attrbMap[index] == -1) {
					attrbMap[index] = attribCount;
					atributes[attribCount] = m_attib[index];
					attribCount ++;
				}
				userData[indexCount] = attrbMap[index];
				indexCount ++;

				ptr = ptr->m_next;
			} while (ptr != face);
			polygon.AddFace(indexCount, vertex, userData);
		}
	}
	polygon.EndFace();

	m_pointCount = vertexCount;
	memcpy (&m_points[0].m_x, &points[0].m_x, m_pointCount * sizeof (dgBigVector));
	 
	m_atribCount = attribCount;
	memcpy (&m_attib[0].m_vertex.m_x, &atributes[0].m_vertex.m_x, m_atribCount * sizeof (dgVertexAtribute));


	RemoveAll();
	SetLRU (0);

	BeginFace();
	dgPolyhedra::Iterator iter1 (polygon);
	for (iter1.Begin(); iter1; iter1 ++){
		dgEdge* const face = &(*iter1);
		if ((face->m_mark != mark) && (face->m_incidentFace > 0)) {
			dgInt32	index[DG_MESH_EFFECT_POINT_SPLITED];
			dgInt64	userData[DG_MESH_EFFECT_POINT_SPLITED];

			dgEdge* ptr = face;
			dgInt32 indexCount = 0;
			do {
				ptr->m_mark = mark;
				index[indexCount] = ptr->m_incidentVertex;
				userData[indexCount] = dgInt64 (ptr->m_userData);
				indexCount ++;
				ptr = ptr->m_next;
			} while (ptr != face);
			AddFace(indexCount, index, userData);
		}
	}
	EndFace();
	PackVertexArrays ();
}

dgMatrix dgMeshEffect::CalculateOOBB (dgBigVector& size) const
{
	dgSphere sphere (CalculateSphere (&m_points[0].m_x, sizeof (dgBigVector), NULL));
	size = sphere.m_size;

	dgMatrix permuation (dgGetIdentityMatrix());
	permuation[0][0] = dgFloat32 (0.0f);
	permuation[0][1] = dgFloat32 (1.0f);
	permuation[1][1] = dgFloat32 (0.0f);
	permuation[1][2] = dgFloat32 (1.0f);
	permuation[2][2] = dgFloat32 (0.0f);
	permuation[2][0] = dgFloat32 (1.0f);

	while ((size.m_x < size.m_y) || (size.m_x < size.m_z)) {
		sphere = permuation * sphere;
		size = permuation.UnrotateVector(size);
	}

	return sphere;
}

void dgMeshEffect::CalculateAABB (dgBigVector& minBox, dgBigVector& maxBox) const
{
	dgBigVector minP ( dgFloat64 (1.0e15f),  dgFloat64 (1.0e15f),  dgFloat64 (1.0e15f), dgFloat64 (0.0f)); 
	dgBigVector maxP (-dgFloat64 (1.0e15f), -dgFloat64 (1.0e15f), -dgFloat64 (1.0e15f), dgFloat64 (0.0f)); 

	dgPolyhedra::Iterator iter (*this);
	const dgBigVector* const points = &m_points[0];
	for (iter.Begin(); iter; iter ++){
		dgEdge* const edge = &(*iter);
		const dgBigVector& p (points[edge->m_incidentVertex]);

		minP.m_x = GetMin (p.m_x, minP.m_x); 
		minP.m_y = GetMin (p.m_y, minP.m_y); 
		minP.m_z = GetMin (p.m_z, minP.m_z); 

		maxP.m_x = GetMax (p.m_x, maxP.m_x); 
		maxP.m_y = GetMax (p.m_y, maxP.m_y); 
		maxP.m_z = GetMax (p.m_z, maxP.m_z); 
	}

	minBox = minP;
	maxBox = maxP;
}

void dgMeshEffect::EnumerateAttributeArray (dgVertexAtribute* const attib)
{
	dgInt32 index = 0;
	dgPolyhedra::Iterator iter (*this);	
	for(iter.Begin(); iter; iter ++){
		dgEdge* const edge = &(*iter);
		_ASSERTE (index < GetCount());
		attib[index] = m_attib[dgInt32 (edge->m_userData)];
		edge->m_userData = dgUnsigned64 (index);
		index ++;
	}
}

void dgMeshEffect::ApplyAttributeArray (dgVertexAtribute* const attib)
{
	dgStack<dgInt32>indexMap (GetCount());

	m_atribCount = dgVertexListToIndexList (&attib[0].m_vertex.m_x, sizeof (dgVertexAtribute), sizeof (dgVertexAtribute) / sizeof(dgFloat64), GetCount(), &indexMap[0], DG_VERTEXLIST_INDEXLIST_TOL);
	m_maxAtribCount = m_atribCount;

	GetAllocator()->FreeLow (m_attib);
	m_attib = (dgVertexAtribute*) GetAllocator()->MallocLow(dgInt32 (m_atribCount * sizeof(dgVertexAtribute)));
	memcpy (m_attib, attib, m_atribCount * sizeof(dgVertexAtribute));

	dgPolyhedra::Iterator iter (*this);	
	for(iter.Begin(); iter; iter ++){
		dgEdge* const edge = &(*iter);
		dgInt32 index = indexMap[dgInt32 (edge->m_userData)];
		_ASSERTE (index >=0);
		_ASSERTE (index < m_atribCount);
		edge->m_userData = dgUnsigned64 (index);
	}
}

dgBigVector dgMeshEffect::GetOrigin ()const
{
	dgBigVector origin (dgFloat64 (0.0f), dgFloat64 (0.0f), dgFloat64 (0.0f), dgFloat64 (0.0f));	
	for (dgInt32 i = 0; i < m_pointCount; i ++) {
		origin += m_points[i];
	}	
	return origin.Scale (dgFloat64 (1.0f) / m_pointCount);
}


void dgMeshEffect::FixCylindricalMapping (dgVertexAtribute* attribArray) const
{
	dgPolyhedra::Iterator iter (*this);	
	for(iter.Begin(); iter; iter ++){
		dgEdge* const edge = &(*iter);
		dgVertexAtribute& attrib0 = attribArray[dgInt32 (edge->m_userData)];
		dgVertexAtribute& attrib1 = attribArray[dgInt32 (edge->m_next->m_userData)];

		dgFloat64 error = fabs (attrib0.m_u0 - attrib1.m_u0);
		if (error > dgFloat32 (0.6f)) {
			if (attrib0.m_u0 < attrib1.m_u0) {
				attrib0.m_u0 += dgFloat32 (1.0f);
				attrib0.m_u1 = attrib0.m_u0;
			} else {
				attrib1.m_u0 += dgFloat32 (1.0f);
				attrib1.m_u1 = attrib1.m_u0;
			}
			
		}
	}

	for(iter.Begin(); iter; iter ++){
		dgEdge* const edge = &(*iter);
		dgVertexAtribute& attrib0 = attribArray[dgInt32 (edge->m_userData)];
		dgVertexAtribute& attrib1 = attribArray[dgInt32 (edge->m_next->m_userData)];

		dgFloat64 error = fabs (attrib0.m_u0 - attrib1.m_u0);
		if (error > dgFloat32 (0.6f)) {
			if (attrib0.m_u0 < attrib1.m_u0) {
				attrib0.m_u0 += dgFloat32 (1.0f);
				attrib0.m_u1 = attrib0.m_u0;
			} else {
				attrib1.m_u0 += dgFloat32 (1.0f);
				attrib1.m_u1 = attrib1.m_u0;
			}
		}
	}
}


void dgMeshEffect::SphericalMapping (dgInt32 material)
{
	dgBigVector origin (GetOrigin());

	dgStack<dgBigVector>sphere (m_pointCount);
	for (dgInt32 i = 0; i < m_pointCount; i ++) {
		dgBigVector point (m_points[i] - origin);
		point = point.Scale (1.0f / dgSqrt (point % point));

		dgFloat64 u = dgAtan2 (point.m_z, point.m_y);
		if (u < dgFloat32 (0.0f)) {
			u += dgFloat32 (3.141592f * 2.0f);
		}
		dgFloat64 v = ClampValue(point.m_x, dgFloat64(-0.9999f), dgFloat64(0.9999f)) * dgFloat64 (0.5f * 3.141592f);

		sphere[i].m_x = dgFloat64 (1.0f) - u * dgFloat64 (1.0f / (2.0f * 3.141592f));
		sphere[i].m_y = dgFloat64 (0.5f) * (dgFloat64 (1.0f) + v / dgFloat64 (0.5f * 3.141592f));
	}


	dgStack<dgVertexAtribute>attribArray (GetCount());
	EnumerateAttributeArray (&attribArray[0]);

	dgPolyhedra::Iterator iter (*this);	
	for(iter.Begin(); iter; iter ++){
		dgEdge* edge;
		edge = &(*iter);
		dgVertexAtribute& attrib = attribArray[dgInt32 (edge->m_userData)];
		attrib.m_u0 = sphere[edge->m_incidentVertex].m_x;
		attrib.m_v0 = sphere[edge->m_incidentVertex].m_y;
		attrib.m_u1 = sphere[edge->m_incidentVertex].m_x;
		attrib.m_v1 = sphere[edge->m_incidentVertex].m_y;
		attrib.m_material = material;
	}

	FixCylindricalMapping (&attribArray[0]);
	ApplyAttributeArray (&attribArray[0]);
}

void dgMeshEffect::CylindricalMapping (dgInt32 cylinderMaterial, dgInt32 capMaterial)
{
_ASSERTE (0);
/*
	dgVector origin (GetOrigin());

	dgStack<dgVector>cylinder (m_pointCount);

	dgFloat32 xMax;
	dgFloat32 xMin;

	xMin= dgFloat32 (1.0e10f);
	xMax= dgFloat32 (-1.0e10f);
	for (dgInt32 i = 0; i < m_pointCount; i ++) {
		cylinder[i] = m_points[i] - origin;
		xMin = GetMin (xMin, cylinder[i].m_x);
		xMax = GetMax (xMax, cylinder[i].m_x);
	}

	dgFloat32 xscale = dgFloat32 (1.0f)/ (xMax - xMin);
	for (dgInt32 i = 0; i < m_pointCount; i ++) {
		dgFloat32 u;
		dgFloat32 v;
		dgFloat32 y;
		dgFloat32 z;
		y = cylinder[i].m_y;
		z = cylinder[i].m_z;
		u = dgAtan2 (z, y);
		if (u < dgFloat32 (0.0f)) {
			u += dgFloat32 (3.141592f * 2.0f);
		}
		v = (cylinder[i].m_x - xMin) * xscale;

		cylinder[i].m_x = dgFloat32 (1.0f) - u * dgFloat32 (1.0f / (2.0f * 3.141592f));
		cylinder[i].m_y = v;
	}

	dgStack<dgVertexAtribute>attribArray (GetCount());
	EnumerateAttributeArray (&attribArray[0]);

	dgPolyhedra::Iterator iter (*this);	
	for(iter.Begin(); iter; iter ++){
		dgEdge* edge;
		edge = &(*iter);
		dgVertexAtribute& attrib = attribArray[dgInt32 (edge->m_userData)];
		attrib.m_u0 = cylinder[edge->m_incidentVertex].m_x;
		attrib.m_v0 = cylinder[edge->m_incidentVertex].m_y;
		attrib.m_u1 = cylinder[edge->m_incidentVertex].m_x;
		attrib.m_v1 = cylinder[edge->m_incidentVertex].m_y;
		attrib.m_material = cylinderMaterial;
	}

	FixCylindricalMapping (&attribArray[0]);

	dgInt32 mark;
	mark = IncLRU();
	for(iter.Begin(); iter; iter ++){
		dgEdge* edge;
		edge = &(*iter);
		if (edge->m_mark < mark){
			const dgVector& p0 = m_points[edge->m_incidentVertex];
			const dgVector& p1 = m_points[edge->m_next->m_incidentVertex];
			const dgVector& p2 = m_points[edge->m_prev->m_incidentVertex];

			edge->m_mark = mark;
			edge->m_next->m_mark = mark;
			edge->m_prev->m_mark = mark;

			dgVector e0 (p1 - p0);
			dgVector e1 (p2 - p0);
			dgVector n (e0 * e1);
			if ((n.m_x * n.m_x) > (dgFloat32 (0.99f) * (n % n))) {
				dgEdge* ptr;

				ptr = edge;
				do {
					dgVertexAtribute& attrib = attribArray[dgInt32 (ptr->m_userData)];
					dgVector p (m_points[ptr->m_incidentVertex] - origin);
					p.m_x = dgFloat32 (0.0f);
					p = p.Scale (dgFloat32 (dgRsqrt(p % p)));
					attrib.m_u0 = dgFloat32 (0.5f) + p.m_y * dgFloat32 (0.5f);
					attrib.m_v0 = dgFloat32 (0.5f) + p.m_z * dgFloat32 (0.5f);
					attrib.m_u1 = dgFloat32 (0.5f) + p.m_y * dgFloat32 (0.5f);
					attrib.m_v1 = dgFloat32 (0.5f) + p.m_z * dgFloat32 (0.5f);
					attrib.m_material = capMaterial;

					ptr = ptr->m_next;
				}while (ptr !=  edge);
			}
		}
	}

	ApplyAttributeArray (&attribArray[0]);
*/
}

void dgMeshEffect::BoxMapping (dgInt32 front, dgInt32 side, dgInt32 top)
{
	dgBigVector minVal;
	dgBigVector maxVal;
	dgInt32 materialArray[3];

	GetMinMax (minVal, maxVal, &m_points[0][0], m_pointCount, sizeof (dgBigVector));
	dgBigVector dist (maxVal - minVal);
	dgBigVector scale (dgFloat64 (1.0f)/ dist[0], dgFloat64 (1.0f)/ dist[1], dgFloat64 (1.0f)/ dist[2], dgFloat64 (0.0f));

	dgStack<dgVertexAtribute>attribArray (GetCount());
	EnumerateAttributeArray (&attribArray[0]);

	materialArray[0] = front;
	materialArray[1] = side;
	materialArray[2] = top;

	dgInt32 mark = IncLRU();
	dgPolyhedra::Iterator iter (*this);	
	for(iter.Begin(); iter; iter ++){
		dgEdge* const edge = &(*iter);
		if (edge->m_mark < mark){
			const dgBigVector& p0 = m_points[edge->m_incidentVertex];
			const dgBigVector& p1 = m_points[edge->m_next->m_incidentVertex];
			const dgBigVector& p2 = m_points[edge->m_prev->m_incidentVertex];

			edge->m_mark = mark;
			edge->m_next->m_mark = mark;
			edge->m_prev->m_mark = mark;

			dgBigVector e0 (p1 - p0);
			dgBigVector e1 (p2 - p0);
			dgBigVector n (e0 * e1);
			
			dgInt32 index = 0;
			dgFloat64 maxProjection = dgFloat32 (0.0f);

			for (dgInt32 i = 0; i < 3; i ++) {
				dgFloat64 proj = fabs (n[i]);
				if (proj > maxProjection) {
					index = i;
					maxProjection = proj;
				}
			}

			dgInt32 u = (index + 1) % 3;
			dgInt32 v = (u + 1) % 3;
			if (index == 1) {
				Swap (u, v);
			}
			dgEdge* ptr = edge;
			do {
				dgVertexAtribute& attrib = attribArray[dgInt32 (ptr->m_userData)];
				dgBigVector p (scale.CompProduct(m_points[ptr->m_incidentVertex] - minVal));
				attrib.m_u0 = p[u];
				attrib.m_v0 = p[v];
				attrib.m_u1 = dgFloat64(0.0f);
				attrib.m_v1 = dgFloat64(0.0f);
				attrib.m_material = materialArray[index];

				ptr = ptr->m_next;
			}while (ptr !=  edge);
		}
	}

	ApplyAttributeArray (&attribArray[0]);
}

void dgMeshEffect::UniformBoxMapping (dgInt32 material, const dgMatrix& textureMatrix)
{
	dgStack<dgVertexAtribute>attribArray (GetCount());
	EnumerateAttributeArray (&attribArray[0]);

	dgInt32 mark = IncLRU();
	for (dgInt32 i = 0; i < 3; i ++) {
		dgMatrix rotationMatrix (dgGetIdentityMatrix());
		if (i == 1) {
			rotationMatrix = dgYawMatrix(dgFloat32 (90.0f * 3.1416f / 180.0f));
		} else if (i == 2) {
			rotationMatrix = dgPitchMatrix(dgFloat32 (90.0f * 3.1416f / 180.0f));
		}

		dgPolyhedra::Iterator iter (*this);	

		for(iter.Begin(); iter; iter ++){
			dgEdge* const edge = &(*iter);
			if (edge->m_mark < mark){
				dgBigVector n (FaceNormal(edge, &m_points[0].m_x, sizeof (dgBigVector)));
				dgVector normal (rotationMatrix.RotateVector(dgVector (n.Scale (dgFloat64 (1.0f) / sqrt (n % n)))));
				normal.m_x = dgAbsf (normal.m_x);
				normal.m_y = dgAbsf (normal.m_y);
				normal.m_z = dgAbsf (normal.m_z);
				if ((normal.m_z >= (normal.m_x - dgFloat32 (1.0e-4f))) && (normal.m_z >= (normal.m_y - dgFloat32 (1.0e-4f)))) {
					dgEdge* ptr = edge;
					do {
						ptr->m_mark = mark;
						dgVertexAtribute& attrib = attribArray[dgInt32 (ptr->m_userData)];
						dgVector p (textureMatrix.TransformVector(rotationMatrix.RotateVector(m_points[ptr->m_incidentVertex])));
						attrib.m_u0 = p.m_x;
						attrib.m_v0 = p.m_y;
						attrib.m_u1 = dgFloat32 (0.0f);
						attrib.m_v1 = dgFloat32 (0.0f);
						attrib.m_material = material;
						ptr = ptr->m_next;
					}while (ptr !=  edge);
				}
			}
		}
	}

	ApplyAttributeArray (&attribArray[0]);
}

void dgMeshEffect::CalculateNormals (dgFloat64 angleInRadians)
{
	dgStack<dgBigVector>faceNormal (GetCount());
	dgStack<dgVertexAtribute>attribArray (GetCount());
	EnumerateAttributeArray (&attribArray[0]);

	dgInt32 mark = IncLRU();
	dgPolyhedra::Iterator iter (*this);	
	for(iter.Begin(); iter; iter ++){
		dgEdge* const edge = &(*iter);
		if ((edge->m_mark < mark) && (edge->m_incidentFace > 0)) {
			dgBigVector normal (FaceNormal (edge, &m_points[0].m_x, sizeof (m_points[0])));
			normal = normal.Scale (dgFloat32 (1.0f) / (sqrt(normal % normal) + dgFloat32(1.0e-16f)));

			dgEdge* ptr = edge;
			do {
				dgInt32 index = dgInt32 (ptr->m_userData);
				_ASSERTE (index < GetCount());
				dgVertexAtribute& attrib = attribArray[index];
				faceNormal[index] = normal;
				attrib.m_normal_x = normal.m_x;
				attrib.m_normal_y = normal.m_y;
				attrib.m_normal_z = normal.m_z;

				ptr->m_mark = mark;
				ptr = ptr->m_next;
			} while (ptr !=  edge);
		}
	}


	dgFloat32 smoothValue = dgCos (angleInRadians); 
	mark = IncLRU();
	for(iter.Begin(); iter; iter ++){
		dgEdge* edge = &(*iter);
		if (edge->m_incidentFace > 0) {
			dgEdge* const twin = edge->m_twin->m_prev;
			if (twin->m_incidentFace > 0) {
				dgInt32 edgeIndex = dgInt32 (edge->m_userData);
				dgInt32 twinIndex = dgInt32 (twin->m_userData);
				dgFloat64 test = faceNormal[edgeIndex] % faceNormal[twinIndex];
				if (test >= smoothValue) {
					dgVertexAtribute& attrib = attribArray[edgeIndex];
					attrib.m_normal_x += faceNormal[twinIndex].m_x;
					attrib.m_normal_y += faceNormal[twinIndex].m_y;
					attrib.m_normal_z += faceNormal[twinIndex].m_z;
				}
			}
		}
	}

	for(iter.Begin(); iter; iter ++){
		dgEdge* const edge = &(*iter);
		if (edge->m_incidentFace > 0) {
			dgInt32 edgeIndex = dgInt32 (edge->m_userData);
			_ASSERTE (edgeIndex < GetCount());
			dgVertexAtribute& attrib = attribArray[edgeIndex];

			dgBigVector normal (attrib.m_normal_x, attrib.m_normal_y, attrib.m_normal_z, dgFloat32 (0.0f));
			normal = normal.Scale (dgFloat32 (1.0f) / (sqrt (normal % normal) + dgFloat32(1.0e-16f)));
			attrib.m_normal_x = normal.m_x;
			attrib.m_normal_y = normal.m_y;
			attrib.m_normal_z = normal.m_z;
		}
	}

	ApplyAttributeArray (&attribArray[0]);
}


void dgMeshEffect::BeginPolygon ()
{
	m_pointCount = 0;
	m_atribCount = 0;
	RemoveAll();
	BeginFace();
}


void dgMeshEffect::AddAtribute (const dgVertexAtribute& attib)
{
	if (m_atribCount >= m_maxAtribCount) {
		m_maxAtribCount *= 2;
		dgVertexAtribute* const attibArray = (dgVertexAtribute*) GetAllocator()->MallocLow(dgInt32 (m_maxAtribCount * sizeof(dgVertexAtribute)));
		memcpy (attibArray, m_attib, m_atribCount * sizeof(dgVertexAtribute));
		GetAllocator()->FreeLow(m_attib);
		m_attib = attibArray;
	}

	m_attib[m_atribCount] = attib;
	m_attib[m_atribCount].m_vertex.m_x = QuantizeCordinade(m_attib[m_atribCount].m_vertex.m_x);
	m_attib[m_atribCount].m_vertex.m_y = QuantizeCordinade(m_attib[m_atribCount].m_vertex.m_y);
	m_attib[m_atribCount].m_vertex.m_z = QuantizeCordinade(m_attib[m_atribCount].m_vertex.m_z);
	m_atribCount ++;
}

void dgMeshEffect::AddVertex(const dgBigVector& vertex)
{
	if (m_pointCount >= m_maxPointCount) {
		m_maxPointCount *= 2;
		dgBigVector* const points = (dgBigVector*) GetAllocator()->MallocLow(dgInt32 (m_maxPointCount * sizeof(dgBigVector)));
		memcpy (points, m_points, m_pointCount * sizeof(dgBigVector));
		GetAllocator()->FreeLow(m_points);
		m_points = points;
	}
	
	m_points[m_pointCount].m_x = QuantizeCordinade(vertex[0]);
	m_points[m_pointCount].m_y = QuantizeCordinade(vertex[1]);
	m_points[m_pointCount].m_z = QuantizeCordinade(vertex[2]);
	m_points[m_pointCount].m_w = vertex.m_w;
	m_pointCount ++;
}


void dgMeshEffect::AddPoint(const dgFloat64* vertex, dgInt32 material)
{
	dgVertexAtribute attib;
	AddVertex(dgBigVector (vertex[0], vertex[1], vertex[2], vertex[3]));
	
	attib.m_vertex.m_x = m_points[m_pointCount -1].m_x;
	attib.m_vertex.m_y = m_points[m_pointCount -1].m_y;
	attib.m_vertex.m_z = m_points[m_pointCount -1].m_z;
	attib.m_vertex.m_w = m_points[m_pointCount -1].m_w;

	attib.m_normal_x = vertex[4];
	attib.m_normal_y = vertex[5];
	attib.m_normal_z = vertex[6];
	attib.m_u0 = vertex[7];
	attib.m_v0 = vertex[8];
	attib.m_u1 = vertex[9];
	attib.m_v1 = vertex[10];
	attib.m_material = material;

	AddAtribute (attib);
}

void dgMeshEffect::PackVertexArrays ()
{
	if (m_maxPointCount > m_pointCount) {
		dgBigVector* const points = (dgBigVector*) GetAllocator()->MallocLow(dgInt32 (m_pointCount * sizeof(dgBigVector)));
		memcpy (points, m_points, m_pointCount * sizeof(dgBigVector));
		GetAllocator()->FreeLow(m_points);
		m_points = points;
		m_maxPointCount = m_pointCount;
	}


	if (m_maxAtribCount > m_atribCount) {
		dgVertexAtribute* const attibArray = (dgVertexAtribute*) GetAllocator()->MallocLow(dgInt32 (m_atribCount * sizeof(dgVertexAtribute)));
		memcpy (attibArray, m_attib, m_atribCount * sizeof(dgVertexAtribute));
		GetAllocator()->FreeLow(m_attib);
		m_attib = attibArray;
		m_maxAtribCount = m_atribCount;
	}
};


void dgMeshEffect::AddPolygon (dgInt32 count, const dgFloat64* const vertexList, dgInt32 strideIndBytes, dgInt32 material)
{
	dgInt32 stride = dgInt32 (strideIndBytes / sizeof (dgFloat64));

	if (count > 3) {
		dgPolyhedra polygon (GetAllocator());
		dgInt32 indexList[256];
		_ASSERTE (count < sizeof (indexList)/sizeof(indexList[0]));
		for (dgInt32 i = 0; i < count; i ++) {
			indexList[i] = i;
		}

		polygon.BeginFace();
		polygon.AddFace(count, indexList, NULL);
		polygon.EndFace();
		polygon.Triangulate(vertexList, strideIndBytes, NULL);

		dgInt32 mark = polygon.IncLRU();
		dgPolyhedra::Iterator iter (polygon);
		for (iter.Begin(); iter; iter ++) {
			dgEdge* const edge = &iter.GetNode()->GetInfo();
			if ((edge->m_incidentFace > 0) && (edge->m_mark < mark)) {
				dgInt32 i0 = edge->m_incidentVertex;
				dgInt32 i1 = edge->m_next->m_incidentVertex;
				dgInt32 i2 = edge->m_next->m_next->m_incidentVertex;
				edge->m_mark = mark;
				edge->m_next->m_mark = mark;
				edge->m_next->m_next->m_mark = mark;

				#ifdef _DEBUG
					dgBigVector p0 (&vertexList[i0 * stride]);
					dgBigVector p1 (&vertexList[i1 * stride]);
					dgBigVector p2 (&vertexList[i2 * stride]);
					dgBigVector e1 (p1 - p0);
					dgBigVector e2 (p2 - p0);
					dgBigVector n (e1 * e2);
					dgFloat64 mag2 = n % n;
					_ASSERTE (mag2 > dgFloat32 (0.0f));
				#endif
				AddPoint(vertexList + i0 * stride, material);
				AddPoint(vertexList + i1 * stride, material);
				AddPoint(vertexList + i2 * stride, material);
			}
		}

	} else {

		dgBigVector p0 (&vertexList[0 * stride]);
		dgBigVector p1 (&vertexList[1 * stride]);
		dgBigVector p2 (&vertexList[2 * stride]);
		dgBigVector e1 (p1 - p0);
		dgBigVector e2 (p2 - p0);
		dgBigVector n (e1 * e2);
//		dgFloat32 mag2 = dgSqrt (n % n);
//		if (mag2 > DG_MESH_EFFECT_TRIANGLE_MIN_AREA) {
		dgFloat64 mag2 = n % n;
		_ASSERTE (mag2 > dgFloat32 (0.0f));
		if (mag2 > dgFloat32 (0.0f)) {
			AddPoint(vertexList, material);
			AddPoint(vertexList + stride, material);
			AddPoint(vertexList + stride + stride, material);
		}
	}
}


void dgMeshEffect::EndPolygon (dgFloat64 tol)
{
	dgStack<dgInt32>indexMap(m_pointCount);
	dgStack<dgInt32>attrIndexMap(m_atribCount);

#ifdef _DEBUG
	for (dgInt32 i = 0; i < m_pointCount; i += 3) {
		dgBigVector p0 (m_points[i + 0]);
		dgBigVector p1 (m_points[i + 1]);
		dgBigVector p2 (m_points[i + 2]);
		dgBigVector e1 (p1 - p0);
		dgBigVector e2 (p2 - p0);
		dgBigVector n (e1 * e2);
		dgFloat64 mag2 = n % n;
//		_ASSERTE (mag2 > DG_MESH_EFFECT_TRIANGLE_MIN_AREA);
		_ASSERTE (mag2 > dgFloat32 (0.0f));
	}
#endif

	dgInt32 triangCount = m_pointCount / 3;
	m_pointCount = dgVertexListToIndexList (&m_points[0].m_x, sizeof (dgBigVector), sizeof (dgBigVector)/sizeof (dgFloat64), m_pointCount, &indexMap[0], tol);
	m_atribCount = dgVertexListToIndexList (&m_attib[0].m_vertex.m_x, sizeof (dgVertexAtribute), sizeof (dgVertexAtribute)/sizeof (dgFloat64), m_atribCount, &attrIndexMap[0], tol);

	for (dgInt32 i = 0; i < triangCount; i ++) {
		dgInt32 index[3];
		dgInt64 userdata[3];

		index[0] = indexMap[i * 3 + 0];
		index[1] = indexMap[i * 3 + 1];
		index[2] = indexMap[i * 3 + 2];


		dgBigVector e1 (m_points[index[1]] - m_points[index[0]]);
		dgBigVector e2 (m_points[index[2]] - m_points[index[0]]);

		dgBigVector n (e1 * e2);
		dgFloat64 mag2 = n % n;
		if (mag2 > dgFloat64 (1.0e-12f)) {
			userdata[0] = attrIndexMap[i * 3 + 0];
			userdata[1] = attrIndexMap[i * 3 + 1];
			userdata[2] = attrIndexMap[i * 3 + 2];
			dgEdge* edge = AddFace (3, index, userdata);
			if (!edge) {
				_ASSERTE ((m_pointCount + 3) <= m_maxPointCount);

				m_points[m_pointCount + 0] = m_points[index[0]];
				m_points[m_pointCount + 1] = m_points[index[1]];
				m_points[m_pointCount + 2] = m_points[index[2]];

				index[0] = m_pointCount + 0;
				index[1] = m_pointCount + 1;
				index[2] = m_pointCount + 2;

				m_pointCount += 3;
				edge = AddFace (3, index, userdata);
				_ASSERTE (edge);
			}
		}
	}
	EndFace();

	RepairTJoints ();


#ifdef _DEBUG
	dgPolyhedra::Iterator iter (*this);	
	for (iter.Begin(); iter; iter ++){
		dgEdge* const face = &(*iter);
		if (face->m_incidentFace > 0) {
			dgBigVector p0 (m_points[face->m_incidentVertex]);
			dgBigVector p1 (m_points[face->m_next->m_incidentVertex]);
			dgBigVector p2 (m_points[face->m_next->m_next->m_incidentVertex]);
			dgBigVector e1 (p1 - p0);
			dgBigVector e2 (p2 - p0);
			dgBigVector n (e1 * e2);
//			_ASSERTE (mag2 > DG_MESH_EFFECT_TRIANGLE_MIN_AREA);
			dgFloat64 mag2 = n % n;
			_ASSERTE (mag2 > dgFloat32 (0.0f));
		}
	}
#endif
}


void dgMeshEffect::BuildFromVertexListIndexList(
	dgInt32 faceCount, const dgInt32* const faceIndexCount, const dgInt32* const faceMaterialIndex, 
	const dgFloat32* const vertex, dgInt32 vertexStrideInBytes, const dgInt32* const vertexIndex,
	const dgFloat32* const normal, dgInt32  normalStrideInBytes, const dgInt32* const normalIndex,
	const dgFloat32* const uv0, dgInt32  uv0StrideInBytes, const dgInt32* const uv0Index,
	const dgFloat32* const uv1, dgInt32  uv1StrideInBytes, const dgInt32* const uv1Index)
{
	BeginPolygon ();

	// calculate vertex Count
	dgInt32 acc = 0;
	dgInt32 vertexCount = 0;
	for (dgInt32 j = 0; j < faceCount; j ++) {
		int count = faceIndexCount[j];
		for (int i = 0; i < count; i ++) {
			vertexCount = GetMax(vertexCount, vertexIndex[acc + i] + 1);
		}
		acc += count;
	}
		
	dgInt32 layerCountBase = 0;
	dgInt32 vertexStride = dgInt32 (vertexStrideInBytes / sizeof (dgFloat32));
	for (int i = 0; i < vertexCount; i ++) {
		int index = i * vertexStride;
		AddVertex (dgBigVector (vertex[index + 0], vertex[index + 1], vertex[index + 2], vertex[index + 3]));
		layerCountBase += (vertex[index + 3]) > dgFloat32(layerCountBase);
	}


	acc = 0;
	dgInt32 normalStride = dgInt32 (normalStrideInBytes / sizeof (dgFloat32));
	dgInt32 uv0Stride = dgInt32 (uv0StrideInBytes / sizeof (dgFloat32));
	dgInt32 uv1Stride = dgInt32 (uv1StrideInBytes / sizeof (dgFloat32));
	for (dgInt32 j = 0; j < faceCount; j ++) {
		dgInt32 indexCount = faceIndexCount[j];
		dgInt32 materialIndex = faceMaterialIndex[j];
		for (dgInt32 i = 0; i < indexCount; i ++) {
			dgVertexAtribute point;
			dgInt32 index = vertexIndex[acc + i];
			point.m_vertex = m_points[index];
			
			index = normalIndex[(acc + i)] * normalStride;
			point.m_normal_x =  normal[index + 0];
			point.m_normal_y =  normal[index + 1];
			point.m_normal_z =  normal[index + 2];

			index = uv0Index[(acc + i)] * uv0Stride;
			point.m_u0 = uv0[index + 0];
			point.m_v0 = uv0[index + 1];
			
			index = uv1Index[(acc + i)] * uv1Stride;
			point.m_u1 = uv1[index + 0];
			point.m_v1 = uv1[index + 1];

			point.m_material = materialIndex;
			AddAtribute(point);
		}
		acc += indexCount;
	}


	dgStack<dgInt32>attrIndexMap(m_atribCount);
	m_atribCount = dgVertexListToIndexList (&m_attib[0].m_vertex.m_x, sizeof (dgVertexAtribute), sizeof (dgVertexAtribute) / sizeof (dgFloat64), m_atribCount, &attrIndexMap[0], DG_VERTEXLIST_INDEXLIST_TOL);

	bool hasFaces = true;
	dgStack<dgInt8> faceMark (faceCount);
	memset (&faceMark[0], 1, size_t (faceMark.GetSizeInBytes()));
	
	dgInt32 layerCount = 0;
	while (hasFaces) {
		acc = 0;
		hasFaces = false;
		dgInt32 vertexBank = layerCount * vertexCount;
		for (dgInt32 j = 0; j < faceCount; j ++) {
			dgInt32 index[256];
			dgInt64 userdata[256];

			int indexCount = faceIndexCount[j];
			_ASSERTE (indexCount < sizeof (index) / sizeof (index[0]));

			if (faceMark[j]) {
				for (int i = 0; i < indexCount; i ++) {
					index[i] = vertexIndex[acc + i] + vertexBank;
					userdata[i] = attrIndexMap[acc + i];
				}
				dgEdge* const edge = AddFace (indexCount, index, userdata);
				if (edge) {
					faceMark[j] = 0;
				} else {
					// check if the face is not degenerated
					bool degeneratedFace = false;
					for (int i = 0; i < indexCount - 1; i ++) {
						for (int k = i + 1; k < indexCount; k ++) {
							if (index[i] == index[k]) {
								degeneratedFace = true;		
							}
						}
					}
					if (degeneratedFace) {
						faceMark[j] = 0;
					} else {
						hasFaces = true;
					}
				}
			}
			acc += indexCount;
		}
		if (hasFaces) {
			layerCount ++;
			for (int i = 0; i < vertexCount; i ++) {
				int index = i * vertexStride;
				AddVertex (dgBigVector (vertex[index + 0], vertex[index + 1], vertex[index + 2], dgFloat64 (layerCount + layerCountBase)));
			}
		}
	}

	EndFace();
	PackVertexArrays ();
}


dgInt32 dgMeshEffect::GetTotalFaceCount() const
{
	return GetFaceCount();
}

dgInt32 dgMeshEffect::GetTotalIndexCount() const
{
	Iterator iter (*this);
	dgInt32 count = 0;
	dgInt32 mark = IncLRU();
	for (iter.Begin(); iter; iter ++) {
		dgEdge* const edge = &(*iter);
		if (edge->m_mark == mark) {
			continue;
		}

		if (edge->m_incidentFace < 0) {
			continue;
		}
		
		dgEdge* ptr = edge;
		do {
			count ++;
			ptr->m_mark = mark;
			ptr = ptr->m_next;
		} while (ptr != edge);
	}
	return count;
}

void dgMeshEffect::GetFaces (dgInt32* const facesIndex, dgInt32* const materials, void** const faceNodeList) const
{
	Iterator iter (*this);

	dgInt32 faces = 0;
	dgInt32 indexCount = 0;
	dgInt32 mark = IncLRU();
	for (iter.Begin(); iter; iter ++) {
		dgEdge* edge = &(*iter);
		if (edge->m_mark == mark) {
			continue;
		}

		if (edge->m_incidentFace < 0) {
			continue;
		}

		dgInt32 faceCount = 0;
		dgEdge* ptr = edge;
		do {
//			indexList[indexCount] = dgInt32 (ptr->m_userData);
			faceNodeList[indexCount] = GetNodeFromInfo (*ptr);
			indexCount ++;
			faceCount ++;
			ptr->m_mark = mark;
			ptr = ptr->m_next;
		} while (ptr != edge);

		facesIndex[faces] = faceCount;
		materials[faces] = dgFastInt(m_attib[dgInt32 (edge->m_userData)].m_material);
		faces ++;
	}
}

void* dgMeshEffect::GetFirstVertex ()
{
	_ASSERTE (0);
	return 0;
	/*

	Iterator iter (*this);
	iter.Begin();

	dgTreeNode* node = NULL;
	if (iter) {
		int mark = IncLRU();
		node = iter.GetNode();

		dgEdge* edge = &node->GetInfo();
		dgEdge* ptr = edge;
		do {
			ptr->m_mark = mark;
			ptr = ptr->m_twin->m_next;
		} while (ptr != edge);
	}
	return node; 
*/
}

void* dgMeshEffect::GetNextVertex (const void* vertex)
{
	dgTreeNode* node = (dgTreeNode*) vertex;
	int mark = node->GetInfo().m_mark;

	Iterator iter (*this);
	iter.Set (node);
	for (iter ++; iter; iter ++) {
		dgTreeNode* node = iter.GetNode();
		if (node->GetInfo().m_mark != mark) {
			dgEdge* edge = &node->GetInfo();
			dgEdge* ptr = edge;
			do {
				ptr->m_mark = mark;
				ptr = ptr->m_twin->m_next;
			} while (ptr != edge);
			return node; 
		}
	}
	return NULL; 
}

int dgMeshEffect::GetVertexIndex (const void* vertex) const
{
	dgTreeNode* node = (dgTreeNode*) vertex;
	dgEdge* edge = &node->GetInfo();
	return edge->m_incidentVertex;
}


void* dgMeshEffect::GetFirstPoint ()
{
_ASSERTE (0);
return 0;
/*

	Iterator iter (*this);

	dgTreeNode* node = NULL;
	int mark = IncLRU();
	for (iter.Begin(); iter; iter ++) {
		node = iter.GetNode();
		dgEdge* edge = &node->GetInfo();
		edge->m_mark = mark;
		if (edge->m_incidentFace < 0) {
			dgEdge* ptr = edge;
			do {
				ptr->m_mark = mark;
				ptr = ptr->m_next;
			} while (ptr != edge);
		} else {
			break;
		}
	}
	return node; 
*/
}

void* dgMeshEffect::GetNextPoint (const void* point)
{
	dgTreeNode* node = (dgTreeNode*) point;
	int mark = node->GetInfo().m_mark;

	Iterator iter (*this);
	iter.Set (node);
	for (iter ++; iter; iter ++) {
		dgTreeNode* node = iter.GetNode();

		dgEdge* edge = &node->GetInfo();
		if (edge->m_mark != mark) {
			edge->m_mark = mark;
			if (edge->m_incidentFace < 0) {
				dgEdge* ptr = edge;
				do {
					ptr->m_mark = mark;
					ptr = ptr->m_next;
				} while (ptr != edge);
			} else {
				return node; 
			}
		}
	}
	return NULL; 
}

int dgMeshEffect::GetPointIndex (const void* point) const
{
	dgTreeNode* node = (dgTreeNode*) point;
	dgEdge* edge = &node->GetInfo();
	return int (edge->m_userData);
}

int dgMeshEffect::GetVertexIndexFromPoint (const void* point) const
{
	return GetVertexIndex (point);
}


dgEdge* dgMeshEffect::ConectVertex (dgEdge* const e0, dgEdge* const e1)
{
	dgEdge* const edge = AddHalfEdge(e1->m_incidentVertex, e0->m_incidentVertex);
	dgEdge* const twin = AddHalfEdge(e0->m_incidentVertex, e1->m_incidentVertex);
	_ASSERTE ((edge && twin) || !(edge || twin));
	if (edge) {
		edge->m_twin = twin;
		twin->m_twin = edge;

		edge->m_incidentFace = e0->m_incidentFace;
		twin->m_incidentFace = e1->m_incidentFace;

		edge->m_userData = e1->m_userData;
		twin->m_userData = e0->m_userData;

		edge->m_next = e0;
		edge->m_prev = e1->m_prev;

		twin->m_next = e1;
		twin->m_prev = e0->m_prev;

		e0->m_prev->m_next = twin;
		e0->m_prev = edge;

		e1->m_prev->m_next = edge;
		e1->m_prev = twin;
	}

	return edge;
}


//int dgMeshEffect::GetVertexAttributeIndex (const void* vertex) const
//{
//	dgTreeNode* node = (dgTreeNode*) vertex;
//	dgEdge* edge = &node->GetInfo();
//	return int (edge->m_userData);
//}


void* dgMeshEffect::GetFirstEdge ()
{
	_ASSERTE (0);
	return 0;
	/*

	Iterator iter (*this);
	iter.Begin();

	dgTreeNode* node = NULL;
	if (iter) {
		int mark = IncLRU();

		node = iter.GetNode();

		dgEdge* edge = &node->GetInfo();
		edge->m_mark = mark;
		edge->m_twin->m_mark = mark;
	}
	return node; 
*/
}

void* dgMeshEffect::GetNextEdge (const void* edge)
{
	dgTreeNode* node = (dgTreeNode*) edge;
	int mark = node->GetInfo().m_mark;

	Iterator iter (*this);
	iter.Set (node);
	for (iter ++; iter; iter ++) {
		dgTreeNode* node = iter.GetNode();
		if (node->GetInfo().m_mark != mark) {
			node->GetInfo().m_mark = mark;
			node->GetInfo().m_twin->m_mark = mark;
			return node; 
		}
	}
	return NULL; 
}

void dgMeshEffect::GetEdgeIndex (const void* edge, dgInt32& v0, dgInt32& v1) const
{
	dgTreeNode* node = (dgTreeNode*) edge;
	v0 = node->GetInfo().m_incidentVertex;
	v1 = node->GetInfo().m_twin->m_incidentVertex;
}

//void dgMeshEffect::GetEdgeAttributeIndex (const void* edge, dgInt32& v0, dgInt32& v1) const
//{
//	dgTreeNode* node = (dgTreeNode*) edge;
//	v0 = int (node->GetInfo().m_userData);
//	v1 = int (node->GetInfo().m_twin->m_userData);
//}


void* dgMeshEffect::GetFirstFace ()
{
	_ASSERTE (0);
	return 0;
	/*

	Iterator iter (*this);
	iter.Begin();

	dgTreeNode* node = NULL;
	if (iter) {
		int mark = IncLRU();
		node = iter.GetNode();

		dgEdge* edge = &node->GetInfo();
		dgEdge* ptr = edge;
		do {
			ptr->m_mark = mark;
			ptr = ptr->m_next;
		} while (ptr != edge);
	}

	return node;
*/
}

void* dgMeshEffect::GetNextFace (const void* face)
{
	dgTreeNode* node = (dgTreeNode*) face;
	int mark = node->GetInfo().m_mark;

	Iterator iter (*this);
	iter.Set (node);
	for (iter ++; iter; iter ++) {
		dgTreeNode* node = iter.GetNode();
		if (node->GetInfo().m_mark != mark) {
			dgEdge* edge = &node->GetInfo();
			dgEdge* ptr = edge;
			do {
				ptr->m_mark = mark;
				ptr = ptr->m_next;
			} while (ptr != edge);
			return node; 
		}
	}
	return NULL; 
}


int dgMeshEffect::IsFaceOpen (const void* face) const
{
	dgTreeNode* node = (dgTreeNode*) face;
	dgEdge* edge = &node->GetInfo();
	return (edge->m_incidentFace > 0) ? 0 : 1;
}

int dgMeshEffect::GetFaceMaterial (const void* face) const
{
	dgTreeNode* const node = (dgTreeNode*) face;
	dgEdge* const edge = &node->GetInfo();
	return dgInt32 (m_attib[edge->m_userData].m_material);
}

int dgMeshEffect::GetFaceIndexCount (const void* face) const
{
	int count = 0;
	dgTreeNode* node = (dgTreeNode*) face;
	dgEdge* edge = &node->GetInfo();
	dgEdge* ptr = edge;
	do {
		count ++;
		ptr = ptr->m_next;
	} while (ptr != edge);
	return count; 
}

void dgMeshEffect::GetFaceIndex (const void* face, int* indices) const
{
	int count = 0;
	dgTreeNode* node = (dgTreeNode*) face;
	dgEdge* edge = &node->GetInfo();
	dgEdge* ptr = edge;
	do {
		indices[count] =  ptr->m_incidentVertex;
		count ++;
		ptr = ptr->m_next;
	} while (ptr != edge);
}

void dgMeshEffect::GetFaceAttributeIndex (const void* face, int* indices) const
{
	int count = 0;
	dgTreeNode* node = (dgTreeNode*) face;
	dgEdge* edge = &node->GetInfo();
	dgEdge* ptr = edge;
	do {
		indices[count] = int (ptr->m_userData);
		count ++;
		ptr = ptr->m_next;
	} while (ptr != edge);
}




/*
dgInt32 GetTotalFaceCount() const;
{
	dgInt32 mark;
	dgInt32 count;
	dgInt32 materialCount;
	dgInt32 materials[256];
	dgInt32 streamIndexMap[256];
	dgIndexArray* array; 

	count = 0;
	materialCount = 0;

	array = (dgIndexArray*) GetAllocator()->MallocLow (4 * sizeof (dgInt32) * GetCount() + sizeof (dgIndexArray) + 2048);
	array->m_indexList = (dgInt32*)&array[1];

	mark = IncLRU();
	dgPolyhedra::Iterator iter (*this);	
	memset(streamIndexMap, 0, sizeof (streamIndexMap));
	for(iter.Begin(); iter; iter ++){

		dgEdge* edge;
		edge = &(*iter);
		if ((edge->m_incidentFace >= 0) && (edge->m_mark != mark)) {
			dgEdge* ptr;
			dgInt32 hashValue;
			dgInt32 index0;
			dgInt32 index1;

			ptr = edge;
			ptr->m_mark = mark;
			index0 = dgInt32 (ptr->m_userData);

			ptr = ptr->m_next;
			ptr->m_mark = mark;
			index1 = dgInt32 (ptr->m_userData);

			ptr = ptr->m_next;
			do {
				ptr->m_mark = mark;

				array->m_indexList[count * 4 + 0] = index0;
				array->m_indexList[count * 4 + 1] = index1;
				array->m_indexList[count * 4 + 2] = dgInt32 (ptr->m_userData);
				array->m_indexList[count * 4 + 3] = m_attib[dgInt32 (edge->m_userData)].m_material;
				index1 = dgInt32 (ptr->m_userData);

				hashValue = array->m_indexList[count * 4 + 3] & 0xff;
				streamIndexMap[hashValue] ++;
				materials[hashValue] = array->m_indexList[count * 4 + 3];
				count ++;

				ptr = ptr->m_next;
			} while (ptr != edge);
		}
	}
*/




void dgMeshEffect::GetVertexStreams (dgInt32 vetexStrideInByte, dgFloat32* const vertex, 
									 dgInt32 normalStrideInByte, dgFloat32* const normal, 
									 dgInt32 uvStrideInByte0, dgFloat32* const uv0, 
									 dgInt32 uvStrideInByte1, dgFloat32* const uv1)
{
	uvStrideInByte0 /= sizeof (dgFloat32);
	uvStrideInByte1 /= sizeof (dgFloat32);
	vetexStrideInByte /= sizeof (dgFloat32);
	normalStrideInByte /= sizeof (dgFloat32);
	for (dgInt32 i = 0; i < m_atribCount; i ++)	{
		dgInt32 j = i * vetexStrideInByte;
		vertex[j + 0] = dgFloat32 (m_attib[i].m_vertex.m_x);
		vertex[j + 1] = dgFloat32 (m_attib[i].m_vertex.m_y);
		vertex[j + 2] = dgFloat32 (m_attib[i].m_vertex.m_z);

		j = i * normalStrideInByte;
		normal[j + 0] = dgFloat32 (m_attib[i].m_normal_x);
		normal[j + 1] = dgFloat32 (m_attib[i].m_normal_y);
		normal[j + 2] = dgFloat32 (m_attib[i].m_normal_z);

		j = i * uvStrideInByte1;
		uv1[j + 0] = dgFloat32 (m_attib[i].m_u1);
		uv1[j + 1] = dgFloat32 (m_attib[i].m_v1);

		j = i * uvStrideInByte0;
		uv0[j + 0] = dgFloat32 (m_attib[i].m_u0);
		uv0[j + 1] = dgFloat32 (m_attib[i].m_v0);
	}
}


void dgMeshEffect::GetIndirectVertexStreams(
	dgInt32 vetexStrideInByte, dgFloat64* const vertex, dgInt32* const vertexIndices, dgInt32* const vertexCount,
	dgInt32 normalStrideInByte, dgFloat64* const normal, dgInt32* const normalIndices, dgInt32* const normalCount,
	dgInt32 uvStrideInByte0, dgFloat64* const uv0, dgInt32* const uvIndices0, dgInt32* const uvCount0,
	dgInt32 uvStrideInByte1, dgFloat64* const uv1, dgInt32* const uvIndices1, dgInt32* const uvCount1)
{
/*
	GetVertexStreams (vetexStrideInByte, vertex, normalStrideInByte, normal, uvStrideInByte0, uv0, uvStrideInByte1, uv1);

	*vertexCount = dgVertexListToIndexList(vertex, vetexStrideInByte, vetexStrideInByte, 0, m_atribCount, vertexIndices, dgFloat32 (0.0f));
	*normalCount = dgVertexListToIndexList(normal, normalStrideInByte, normalStrideInByte, 0, m_atribCount, normalIndices, dgFloat32 (0.0f));

	dgTriplex* const tmpUV = (dgTriplex*) GetAllocator()->MallocLow (dgInt32 (sizeof (dgTriplex) * m_atribCount));
	dgInt32 stride = dgInt32 (uvStrideInByte1 /sizeof (dgFloat32));
	for (dgInt32 i = 0; i < m_atribCount; i ++){
		tmpUV[i].m_x = uv1[i * stride + 0];
		tmpUV[i].m_y = uv1[i * stride + 1];
		tmpUV[i].m_z = dgFloat32 (0.0f);
	}

	dgInt32 count = dgVertexListToIndexList(&tmpUV[0].m_x, sizeof (dgTriplex), sizeof (dgTriplex), 0, m_atribCount, uvIndices1, dgFloat32 (0.0f));
	for (dgInt32 i = 0; i < count; i ++){
		uv1[i * stride + 0] = tmpUV[i].m_x;
		uv1[i * stride + 1] = tmpUV[i].m_y;
	}
	*uvCount1 = count;

	stride = dgInt32 (uvStrideInByte0 /sizeof (dgFloat32));
	for (dgInt32 i = 0; i < m_atribCount; i ++){
		tmpUV[i].m_x = uv0[i * stride + 0];
		tmpUV[i].m_y = uv0[i * stride + 1];
		tmpUV[i].m_z = dgFloat32 (0.0f);
	}
	count = dgVertexListToIndexList(&tmpUV[0].m_x, sizeof (dgTriplex), sizeof (dgTriplex), 0, m_atribCount, uvIndices0, dgFloat32 (0.0f));
	for (dgInt32 i = 0; i < count; i ++){
		uv0[i * stride + 0] = tmpUV[i].m_x;
		uv0[i * stride + 1] = tmpUV[i].m_y;
	}
	*uvCount0 = count;

	GetAllocator()->FreeLow (tmpUV);
*/
}

dgMeshEffect::dgIndexArray* dgMeshEffect::MaterialGeometryBegin()
{
	dgInt32 materials[256];
	dgInt32 streamIndexMap[256];

	dgInt32 count = 0;
	dgInt32 materialCount = 0;
	
	dgIndexArray* const array = (dgIndexArray*) GetAllocator()->MallocLow (dgInt32 (4 * sizeof (dgInt32) * GetCount() + sizeof (dgIndexArray) + 2048));
	array->m_indexList = (dgInt32*)&array[1];
	
	dgInt32 mark = IncLRU();
	dgPolyhedra::Iterator iter (*this);	
	memset(streamIndexMap, 0, sizeof (streamIndexMap));
	for(iter.Begin(); iter; iter ++){
		dgEdge* const edge = &(*iter);
		if ((edge->m_incidentFace >= 0) && (edge->m_mark != mark)) {
			dgEdge* ptr = edge;
			ptr->m_mark = mark;
			dgInt32 index0 = dgInt32 (ptr->m_userData);

			ptr = ptr->m_next;
			ptr->m_mark = mark;
			dgInt32 index1 = dgInt32 (ptr->m_userData);

			ptr = ptr->m_next;
			do {
				ptr->m_mark = mark;

				array->m_indexList[count * 4 + 0] = index0;
				array->m_indexList[count * 4 + 1] = index1;
				array->m_indexList[count * 4 + 2] = dgInt32 (ptr->m_userData);
				array->m_indexList[count * 4 + 3] = dgInt32 (m_attib[dgInt32 (edge->m_userData)].m_material);
				index1 = dgInt32 (ptr->m_userData);

				dgInt32 hashValue = array->m_indexList[count * 4 + 3] & 0xff;
				streamIndexMap[hashValue] ++;
				materials[hashValue] = array->m_indexList[count * 4 + 3];
				count ++;

				ptr = ptr->m_next;
			} while (ptr != edge);
		}
	}

	array->m_indexCount = count;
	array->m_materialCount = materialCount;

	count = 0;
	for (dgInt32 i = 0; i < 256;i ++) {
		if (streamIndexMap[i]) {
			array->m_materials[count] = materials[i];
			array->m_materialsIndexCount[count] = streamIndexMap[i] * 3;
			count ++;
		}
	}

	array->m_materialCount = count;

	return array;
}

void dgMeshEffect::MaterialGeomteryEnd(dgIndexArray* handle)
{
	GetAllocator()->FreeLow (handle);
}


dgInt32 dgMeshEffect::GetFirstMaterial (dgIndexArray* handle)
{
	return GetNextMaterial (handle, -1);
}

dgInt32 dgMeshEffect::GetNextMaterial (dgIndexArray* handle, dgInt32 materialId)
{
	materialId ++;
	if(materialId >= handle->m_materialCount) {
		materialId = -1;
	}
	return materialId;
}

void dgMeshEffect::GetMaterialGetIndexStream (dgIndexArray* handle, dgInt32 materialHandle, dgInt32* indexArray)
{
	dgInt32 index;
	dgInt32 textureID;

	index = 0;
	textureID = handle->m_materials[materialHandle];
	for (dgInt32 j = 0; j < handle->m_indexCount; j ++) {
		if (handle->m_indexList[j * 4 + 3] == textureID) {
			indexArray[index + 0] = handle->m_indexList[j * 4 + 0];
			indexArray[index + 1] = handle->m_indexList[j * 4 + 1];
			indexArray[index + 2] = handle->m_indexList[j * 4 + 2];

			index += 3;
		}
	}
}


void dgMeshEffect::GetMaterialGetIndexStreamShort (dgIndexArray* handle, dgInt32 materialHandle, dgInt16* indexArray)
{
	dgInt32 index;
	dgInt32 textureID;

	index = 0;
	textureID = handle->m_materials[materialHandle];
	for (dgInt32 j = 0; j < handle->m_indexCount; j ++) {
		if (handle->m_indexList[j * 4 + 3] == textureID) {
			indexArray[index + 0] = (dgInt16)handle->m_indexList[j * 4 + 0];
			indexArray[index + 1] = (dgInt16)handle->m_indexList[j * 4 + 1];
			indexArray[index + 2] = (dgInt16)handle->m_indexList[j * 4 + 2];
			index += 3;
		}
	}
}

/*
dgInt32 dgMeshEffect::GetEffectiveVertexCount() const
{
	dgInt32 mark;
	dgInt32 count;

	count = 0;
	mark = IncLRU();
	dgPolyhedra::Iterator iter (*this);
	for (iter.Begin(); iter; iter ++){
		dgEdge* vertex;
	
		vertex = &(*iter);
		if (vertex->m_mark != mark) {
			dgEdge* ptr;

			ptr = vertex;
			do {
				ptr->m_mark = mark;
				ptr = ptr->m_twin->m_next;
			} while (ptr != vertex);
			count ++;
		}
	}
	return count;
}
*/


dgCollision* dgMeshEffect::CreateConvexCollision(dgFloat64 tolerance, dgInt32 shapeID, const dgMatrix& srcMatrix) const
{
	dgStack<dgVector> poolPtr (m_pointCount * 2); 
	dgVector* const pool = &poolPtr[0];

	dgBigVector minBox;
	dgBigVector maxBox;
	CalculateAABB (minBox, maxBox);
	dgVector com ((minBox + maxBox).Scale (dgFloat32 (0.5f)));

	dgInt32 count = 0;
	dgInt32 mark = IncLRU();
	dgPolyhedra::Iterator iter (*this);
	for (iter.Begin(); iter; iter ++){
		dgEdge* const vertex = &(*iter);
		if (vertex->m_mark != mark) {
			dgEdge* ptr = vertex;
			do {
				ptr->m_mark = mark;
				ptr = ptr->m_twin->m_next;
			} while (ptr != vertex);

			if (count < dgInt32 (poolPtr.GetElementsCount())) {
				const dgBigVector p = m_points[vertex->m_incidentVertex];
				pool[count] = dgVector (p) - com;
				count ++;
			}
		}
	}

	dgMatrix matrix (srcMatrix);
	matrix.m_posit += matrix.RotateVector(com);
	matrix.m_posit.m_w = dgFloat32 (1.0f);

	dgStack<dgInt32> buffer(dgInt32 (2 + 3 * count + sizeof (dgMatrix) / sizeof (dgInt32)));  

	memset (&buffer[0], 0, size_t (buffer.GetSizeInBytes()));
	buffer[0] = m_convexHullCollision;
	buffer[1] = shapeID;
	for (dgInt32 i = 0; i < count; i ++) {
		buffer[2 + i * 3 + 0] = dgInt32 (dgCollision::Quantize (pool[i].m_x));
		buffer[2 + i * 3 + 1] = dgInt32 (dgCollision::Quantize (pool[i].m_y));
		buffer[2 + i * 3 + 2] = dgInt32 (dgCollision::Quantize (pool[i].m_z));
	}
	memcpy (&buffer[2 + count * 3], &matrix, sizeof (dgMatrix));
	dgUnsigned32 crc = dgCollision::MakeCRC(&buffer[0], buffer.GetSizeInBytes());

	dgCollisionConvexHull* collision = new (GetAllocator()) dgCollisionConvexHull (GetAllocator(), crc, count, sizeof (dgVector), dgFloat32 (tolerance), &pool[0].m_x, matrix);
	if (!collision->GetVertexCount()) {
		collision->Release();
		collision = NULL;
	} else {
		collision->SetUserDataID (dgUnsigned32 (shapeID));
	}

	return collision;
}


/*
dgEdge* dgMeshEffect::InsertFaceVertex (dgEdge* const face, const dgVector& point)
{
	dgInt32 v0;
	dgInt32 v1;
	dgInt32 v2;
	dgInt32 vertexIndex;
	dgInt32 attibuteIndex;
	dgFloat32 va;
	dgFloat32 vb;
	dgFloat32 vc;
	dgFloat32 den;
	dgFloat32 alpha0;
	dgFloat32 alpha1;
	dgFloat32 alpha2;
	dgFloat32 alpha3;
	dgFloat32 alpha4;
	dgFloat32 alpha5;
	dgFloat32 alpha6;
	dgVertexAtribute attribute;
	dgEdge* face0;
	dgEdge* face1;
	dgEdge* face2;
	dgEdge* edge0;
	dgEdge* twin0;
	dgEdge* edge1;
	dgEdge* twin1;
	dgEdge* edge2;
	dgEdge* twin2;


	v0 = face->m_incidentVertex;
	v1 = face->m_next->m_incidentVertex;
	v2 = face->m_prev->m_incidentVertex;

	const dgVector& p0 = m_points[v0];
	const dgVector& p1 = m_points[v1];
	const dgVector& p2 = m_points[v2];

	dgVector p10 (p1 - p0);
	dgVector p20 (p2 - p0);
	dgVector p_p0 (point - p0);
	dgVector p_p1 (point - p1);
	dgVector p_p2 (point - p2);

	alpha1 = p10 % p_p0;
	alpha2 = p20 % p_p0;
	alpha3 = p10 % p_p1;
	alpha4 = p20 % p_p1;
	alpha5 = p10 % p_p2;
	alpha6 = p20 % p_p2;

	_ASSERTE (!((alpha1 <= dgFloat32 (0.0f)) && (alpha2 <= dgFloat32 (0.0f))));
	_ASSERTE (!((alpha6 >= dgFloat32 (0.0f)) && (alpha5 <= alpha6)));
	_ASSERTE (!((alpha3 >= dgFloat32 (0.0f)) && (alpha4 <= alpha3)));

	vc = alpha1 * alpha4 - alpha3 * alpha2;
	vb = alpha5 * alpha2 - alpha1 * alpha6;
	va = alpha3 * alpha6 - alpha5 * alpha4;

	_ASSERTE (!((vc <= dgFloat32 (0.0f)) && (alpha1 >= dgFloat32 (0.0f)) && (alpha3 <= dgFloat32 (0.0f))));
	_ASSERTE (!((vb <= dgFloat32 (0.0f)) && (alpha2 >= dgFloat32 (0.0f)) && (alpha6 <= dgFloat32 (0.0f))));
	_ASSERTE (!((va <= dgFloat32 (0.0f)) && ((alpha4 - alpha3) >= dgFloat32 (0.0f)) && ((alpha5 - alpha6) >= dgFloat32 (0.0f))));

	den = float(dgFloat32 (1.0f)) / (va + vb + vc);

	alpha0 = va * den;
	alpha1 = vb * den;
	alpha2 = vc * den;


	//dgVector p (p0.Scale (alpha0) + p1.Scale (alpha1) + p2.Scale (alpha2));
	//alpha3 *= 1;


	const dgVertexAtribute& attr0 = m_attib[face->m_userData];
	const dgVertexAtribute& attr1 = m_attib[face->m_next->m_userData];
	const dgVertexAtribute& attr2 = m_attib[face->m_prev->m_userData];
	dgVector normal (attr0.m_normal.m_x * alpha0 + attr1.m_normal.m_x * alpha1 + attr0.m_normal.m_x * alpha2,
					 attr0.m_normal.m_y * alpha0 + attr1.m_normal.m_y * alpha1 + attr0.m_normal.m_y * alpha2,
					 attr0.m_normal.m_z * alpha0 + attr1.m_normal.m_z * alpha1 + attr0.m_normal.m_z * alpha2, dgFloat32 (0.0f));
	normal = normal.Scale (dgRsqrt (normal % normal));

	attribute.m_vertex.m_x = point.m_x;
	attribute.m_vertex.m_y = point.m_y;
	attribute.m_vertex.m_z = point.m_z;
	attribute.m_normal.m_y = normal.m_y;
	attribute.m_normal.m_z = normal.m_z;
	attribute.m_normal.m_x = normal.m_x;
	attribute.m_normal.m_y = normal.m_y;
	attribute.m_normal.m_z = normal.m_z;
	attribute.m_u = attr0.m_u * alpha0 +  attr1.m_u * alpha1 + attr2.m_u * alpha2;
	attribute.m_v = attr0.m_v * alpha0 +  attr1.m_v * alpha1 + attr2.m_v * alpha2;
	_ASSERTE (attr0.m_material == attr1.m_material);
	_ASSERTE (attr0.m_material == attr2.m_material);
	AddVertex (&attribute.m_vertex.m_x, attr0.m_material);

	vertexIndex = m_pointCount - 1;
	attibuteIndex = m_atribCount - 1;

	face0 = face;
	face1 = face->m_next;
	face2 = face->m_prev;

	edge0 = AddHalfEdge(vertexIndex, v0);
	twin0 = AddHalfEdge(v0, vertexIndex);

	edge1 = AddHalfEdge(vertexIndex, v1);
	twin1 = AddHalfEdge(v1, vertexIndex);

	edge2 = AddHalfEdge(vertexIndex, v2);
	twin2 = AddHalfEdge(v2, vertexIndex);

	edge0->m_incidentFace = face->m_incidentFace;
	twin0->m_incidentFace = face->m_incidentFace; 

	edge1->m_incidentFace = face->m_incidentFace;
	twin1->m_incidentFace = face->m_incidentFace;

	edge2->m_incidentFace = face->m_incidentFace;
	twin2->m_incidentFace = face->m_incidentFace;

	edge0->m_userData = attibuteIndex;
	edge1->m_userData = attibuteIndex;
	edge2->m_userData = attibuteIndex;

	twin0->m_userData = face0->m_userData;
	edge1->m_userData = face1->m_userData;
	edge2->m_userData = face2->m_userData;

	edge0->m_twin = twin0;
	twin0->m_twin = edge0;

	edge1->m_twin = twin1;
	twin1->m_twin = edge1;

	edge2->m_twin = twin2;
	twin2->m_twin = edge2;

	edge0->m_next = face0;
	edge1->m_next = face1;
	edge2->m_next = face2;

	edge0->m_prev = twin1;
	edge1->m_prev = twin2;
	edge2->m_prev = twin0;

	twin0->m_next = edge2;
	twin1->m_next = edge0;
	twin2->m_next = edge1;

	twin0->m_prev = face2;
	twin1->m_prev = face0;
	twin2->m_prev = face1;

	face0->m_next = twin1;
	face1->m_next = twin2;
	face2->m_next = twin0;

	face0->m_prev = edge0;
	face1->m_prev = edge1;
	face2->m_prev = edge2;

	return edge0;
}


dgInt32 dgMeshEffect::RayIntersection (dgFloat32& p0p1, const dgVector& p0, const dgVector& p1, dgFloat32& q0q1, const dgVector& q0, const dgVector& q1) const
{
	dgInt32 ret;
	dgFloat64 a;
	dgFloat64 b;
	dgFloat64 c;
	dgFloat64 d;
	dgFloat64 e;
	dgFloat64 D;
	dgBigVector ray_p0 (p0);
	dgBigVector ray_p1 (p1);
	dgBigVector ray_q0 (q0);
	dgBigVector ray_q1 (q1);
	dgBigVector u (ray_p1 - ray_p0);
	dgBigVector v (ray_q1 - ray_q0);

	a = u % u;        // always >= 0
	b = u % v;
	c = v % v;        // always >= 0
	D = a*c - b*b;   // always >= 0

	ret = 0;
	if (D > dgFloat64 (1.0e-8f)) { // the lines are almost parallel
		dgFloat64 sN;
		dgFloat64 tN;
		dgFloat64 fracsN;
		dgFloat64 fractN;
		dgBigVector w (ray_p0 - ray_q0);

		ret = 1;

		d = u % w;
		e = v % w;
		sN = (b*e - c*d) / D;
		tN = (a*e - b*d) / D;
		fracsN = DG_QUANTIZE_TOLERANCE / sqrt (a);
		fractN = DG_QUANTIZE_TOLERANCE / sqrt (c);

		if (sN < -fracsN) {
			ret = 0;
		} else if (sN < fracsN) {
			sN = dgFloat64 (0.0f);
		}
		if (sN > (dgFloat64 (1.0f) + fracsN)) {
			ret = 0;
		} else if (sN > (dgFloat64 (1.0f) - fracsN)) {
			sN = dgFloat64 (1.0f);
		}

		if (tN < -fractN) {
			ret = 0;
		} else if (tN < fractN) {
			tN = dgFloat64 (0.0f);
		}
		if (tN > (dgFloat64 (1.0f) + fractN)) {
			ret = 0;
		} else if (tN > (dgFloat64 (1.0f) - fractN)) {
			tN = dgFloat64 (1.0f);
		}

		if (ret) {
			dgBigVector p (ray_p0 + u.Scale (sN));
			dgBigVector q (ray_q0 + v.Scale (tN));
			dgBigVector dist (p - q);

			d = dist % dist;
			if (d > (dgFloat32 (16.0f) * DG_QUANTIZE_TOLERANCE * DG_QUANTIZE_TOLERANCE)) {
				ret = 0;
			}
		}

		p0p1 = dgFloat32 (sN);
		q0q1 = dgFloat32 (tN);
	}
	return ret;
}
*/




void dgMeshEffect::TransformMesh (const dgMatrix& matrix)
{
_ASSERTE (0);
 /*
	dgMatrix normalMatrix (matrix);
	normalMatrix.m_posit = dgVector (dgFloat32 (0.0f), dgFloat32 (0.0f), dgFloat32 (0.0f), dgFloat32 (1.0f));

	matrix.TransformTriplex (&m_points->m_x, sizeof (dgVector), &m_points->m_x, sizeof (dgVector), m_pointCount);
	matrix.TransformTriplex (&m_attib[0].m_vertex.m_x, sizeof (dgVertexAtribute), &m_attib[0].m_vertex.m_x, sizeof (dgVertexAtribute), m_atribCount);
	normalMatrix.TransformTriplex (&m_attib[0].m_normal.m_x, sizeof (dgVertexAtribute), &m_attib[0].m_normal.m_x, sizeof (dgVertexAtribute), m_atribCount);
*/
}


dgMeshEffect::dgVertexAtribute dgMeshEffect::InterpolateEdge (dgEdge* const edge, dgFloat64 param) const
{
	dgVertexAtribute attrEdge;
	dgFloat64 t1 = param;
	dgFloat64 t0 = dgFloat64 (1.0f) - t1;
	_ASSERTE (t1 >= dgFloat64(0.0f));
	_ASSERTE (t1 <= dgFloat64(1.0f));

	const dgVertexAtribute& attrEdge0 = m_attib[edge->m_userData];
	const dgVertexAtribute& attrEdge1 = m_attib[edge->m_next->m_userData];

	attrEdge.m_vertex.m_x = attrEdge0.m_vertex.m_x * t0 + attrEdge1.m_vertex.m_x * t1;
	attrEdge.m_vertex.m_y = attrEdge0.m_vertex.m_y * t0 + attrEdge1.m_vertex.m_y * t1;
	attrEdge.m_vertex.m_z = attrEdge0.m_vertex.m_z * t0 + attrEdge1.m_vertex.m_z * t1;
	attrEdge.m_vertex.m_w = dgFloat32(0.0f);
	attrEdge.m_normal_x = attrEdge0.m_normal_x * t0 +  attrEdge1.m_normal_x * t1; 
	attrEdge.m_normal_y = attrEdge0.m_normal_y * t0 +  attrEdge1.m_normal_y * t1; 
	attrEdge.m_normal_z = attrEdge0.m_normal_z * t0 +  attrEdge1.m_normal_z * t1; 
	attrEdge.m_u0 = attrEdge0.m_u0 * t0 +  attrEdge1.m_u0 * t1;
	attrEdge.m_v0 = attrEdge0.m_v0 * t0 +  attrEdge1.m_v0 * t1;
	attrEdge.m_u1 = attrEdge0.m_u1 * t0 +  attrEdge1.m_u1 * t1;
	attrEdge.m_v1 = attrEdge0.m_v1 * t0 +  attrEdge1.m_v1 * t1;
	attrEdge.m_material = attrEdge0.m_material;
	return attrEdge;
}

bool dgMeshEffect::Sanity () const
{
	_ASSERTE (0);
return false;
/*
	Iterator iter (*this);
	for (iter.Begin(); iter; iter ++) {
		const dgEdge* const edge = &iter.GetNode()->GetInfo();
		if (edge->m_incidentFace > 0) {
			const dgVertexAtribute& attrEdge0 = m_attib[edge->m_userData];
			dgVector p0 (m_points[edge->m_incidentVertex]);
			dgVector q0 (attrEdge0.m_vertex);
			dgVector delta0 (p0 - q0);
			dgFloat32 error0 = delta0 % delta0;
			if (error0 > dgFloat32 (1.0e-15f)) {
				return false;
			}

			const dgVertexAtribute& attrEdge1 = m_attib[edge->m_next->m_userData];
			dgVector p1 (m_points[edge->m_next->m_incidentVertex]);
			dgVector q1 (attrEdge1.m_vertex);
			dgVector delta1 (p1 - q1);
			dgFloat32 error1 = delta1 % delta1;
			if (error1 > dgFloat32 (1.0e-15f)) {
				return false;
			}
		}
	}
	return true;
*/
}


dgEdge* dgMeshEffect::InsertEdgeVertex (dgEdge* const edge, dgFloat64 param)
{

	dgEdge* const twin = edge->m_twin;
	dgVertexAtribute attrEdge (InterpolateEdge (edge, param));
	dgVertexAtribute attrTwin (InterpolateEdge (twin, dgFloat32 (1.0f) - param));

	attrTwin.m_vertex = attrEdge.m_vertex;
	AddPoint(&attrEdge.m_vertex.m_x, dgFastInt (attrEdge.m_material));
	AddAtribute (attrTwin);

	dgInt32 edgeAttrV0 = dgInt32 (edge->m_userData);
	dgInt32 twinAttrV0 = dgInt32 (twin->m_userData);

	dgEdge* const faceA0 = edge->m_next;
	dgEdge* const faceA1 = edge->m_prev;
	dgEdge* const faceB0 = twin->m_next;
	dgEdge* const faceB1 = twin->m_prev;

//	SpliteEdgeAndTriangulate (m_pointCount - 1, edge);
	SpliteEdge (m_pointCount - 1, edge);

	faceA0->m_prev->m_userData = dgUnsigned64 (m_atribCount - 2);
	faceA1->m_next->m_userData = dgUnsigned64 (edgeAttrV0);

	faceB0->m_prev->m_userData = dgUnsigned64 (m_atribCount - 1);
	faceB1->m_next->m_userData = dgUnsigned64 (twinAttrV0);

	return faceA1->m_next;
}



dgMeshEffect::dgVertexAtribute dgMeshEffect::InterpolateVertex (const dgBigVector& srcPoint, dgEdge* const face) const
{
	//this should use Googol extended precision floats, because some face coming from Voronoi decomposition and booleans
	//clipping has extreme aspect ratios, for now just use float64
	const dgBigVector point (srcPoint);

	dgVertexAtribute attribute;
	memset (&attribute, 0, sizeof (attribute));
	dgFloat64 tol = dgFloat32 (1.0e-4f);
	for (dgInt32 i = 0; i < 4; i ++) {
		dgEdge* ptr = face;
		dgEdge* const edge0 = ptr;
		dgBigVector q0 (m_points[ptr->m_incidentVertex]);

		ptr = ptr->m_next;
		const dgEdge* edge1 = ptr;
		dgBigVector q1 (m_points[ptr->m_incidentVertex]);

		ptr = ptr->m_next;
		const dgEdge* edge2 = ptr;
		do {
			const dgBigVector q2 (m_points[ptr->m_incidentVertex]);

			dgBigVector p10 (q1 - q0);
			dgBigVector p20 (q2 - q0);
#ifdef	_DEBUG		
			dgFloat64 dot = p20 % p10;
			dgFloat64 mag1 = p10 % p10;
			dgFloat64 mag2 = p20 % p20;
			dgFloat64 collinear = dot * dot - mag2 * mag1;
			_ASSERTE (fabs (collinear) > dgFloat64 (1.0e-8f));
#endif

			dgBigVector p_p0 (point - q0);
			dgBigVector p_p1 (point - q1);
			dgBigVector p_p2 (point - q2);

			dgFloat64 alpha1 = p10 % p_p0;
			dgFloat64 alpha2 = p20 % p_p0;
			dgFloat64 alpha3 = p10 % p_p1;
			dgFloat64 alpha4 = p20 % p_p1;
			dgFloat64 alpha5 = p10 % p_p2;
			dgFloat64 alpha6 = p20 % p_p2;

			dgFloat64 vc = alpha1 * alpha4 - alpha3 * alpha2;
			dgFloat64 vb = alpha5 * alpha2 - alpha1 * alpha6;
			dgFloat64 va = alpha3 * alpha6 - alpha5 * alpha4;
			dgFloat64 den = va + vb + vc;
			dgFloat64 minError = den * (-tol);
			dgFloat64 maxError = den * (dgFloat32 (1.0f) + tol);
			if ((va > minError) && (vb > minError) && (vc > minError) && (va < maxError) && (vb < maxError) && (vc < maxError)) {
				edge2 = ptr;

				den = dgFloat64 (1.0f) / (va + vb + vc);

				dgFloat64 alpha0 = dgFloat32 (va * den);
				dgFloat64 alpha1 = dgFloat32 (vb * den);
				dgFloat64 alpha2 = dgFloat32 (vc * den);

				const dgVertexAtribute& attr0 = m_attib[edge0->m_userData];
				const dgVertexAtribute& attr1 = m_attib[edge1->m_userData];
				const dgVertexAtribute& attr2 = m_attib[edge2->m_userData];
				dgBigVector normal (attr0.m_normal_x * alpha0 + attr1.m_normal_x * alpha1 + attr2.m_normal_x * alpha2,
									attr0.m_normal_y * alpha0 + attr1.m_normal_y * alpha1 + attr2.m_normal_y * alpha2,
									attr0.m_normal_z * alpha0 + attr1.m_normal_z * alpha1 + attr2.m_normal_z * alpha2, dgFloat32 (0.0f));
				normal = normal.Scale (dgFloat64 (1.0f) / sqrt (normal % normal));

	#ifdef _DEBUG
				dgBigVector testPoint (attr0.m_vertex.m_x * alpha0 + attr1.m_vertex.m_x * alpha1 + attr2.m_vertex.m_x * alpha2,
									   attr0.m_vertex.m_y * alpha0 + attr1.m_vertex.m_y * alpha1 + attr2.m_vertex.m_y * alpha2,
									   attr0.m_vertex.m_z * alpha0 + attr1.m_vertex.m_z * alpha1 + attr2.m_vertex.m_z * alpha2, dgFloat32 (0.0f));
				_ASSERTE (fabs (testPoint.m_x - point.m_x) < dgFloat32 (1.0e-2f));
				_ASSERTE (fabs (testPoint.m_y - point.m_y) < dgFloat32 (1.0e-2f));
				_ASSERTE (fabs (testPoint.m_z - point.m_z) < dgFloat32 (1.0e-2f));
	#endif


				attribute.m_vertex.m_x = point.m_x;
				attribute.m_vertex.m_y = point.m_y;
				attribute.m_vertex.m_z = point.m_z;
				attribute.m_vertex.m_w = point.m_w;
				attribute.m_normal_x = normal.m_x;
				attribute.m_normal_y = normal.m_y;
				attribute.m_normal_z = normal.m_z;
				attribute.m_u0 = attr0.m_u0 * alpha0 +  attr1.m_u0 * alpha1 + attr2.m_u0 * alpha2;
				attribute.m_v0 = attr0.m_v0 * alpha0 +  attr1.m_v0 * alpha1 + attr2.m_v0 * alpha2;
				attribute.m_u1 = attr0.m_u1 * alpha0 +  attr1.m_u1 * alpha1 + attr2.m_u1 * alpha2;
				attribute.m_v1 = attr0.m_v1 * alpha0 +  attr1.m_v1 * alpha1 + attr2.m_v1 * alpha2;

				attribute.m_material = attr0.m_material;
				_ASSERTE (attr0.m_material == attr1.m_material);
				_ASSERTE (attr0.m_material == attr2.m_material);
				return attribute; 
			}
			
			q1 = q2;
			edge1 = ptr;

			ptr = ptr->m_next;
		} while (ptr != face);
		tol *= dgFloat64 (2.0f);
	}
	// this should never happens
	_ASSERTE (0);
	return attribute;
}




void dgMeshEffect::MergeFaces (const dgMeshEffect* const source)
{
	dgInt32 mark = source->IncLRU();
	dgPolyhedra::Iterator iter (*source);
	for(iter.Begin(); iter; iter ++){
		dgEdge* const edge = &(*iter);
		if ((edge->m_incidentFace > 0) && (edge->m_mark < mark)) {
			dgVertexAtribute face[DG_MESH_EFFECT_POINT_SPLITED];

			dgInt32 count = 0;
			dgEdge* ptr = edge;
			do {
				ptr->m_mark = mark;
				face[count] = source->m_attib[ptr->m_userData];
				count ++;
				_ASSERTE (count < (sizeof (face) / sizeof (face[0])));
				ptr = ptr->m_next;
			} while (ptr != edge);
			AddPolygon(count, &face[0].m_vertex.m_x, sizeof (dgVertexAtribute), dgFastInt (face[0].m_material));
		}
	}
}


void dgMeshEffect::ReverseMergeFaces (dgMeshEffect* const source)
{
_ASSERTE (0);
 /*

	dgInt32 mark;

	mark = source->IncLRU();
	dgPolyhedra::Iterator iter (*source);
	for(iter.Begin(); iter; iter ++){
		dgEdge* edge;
		edge = &(*iter);
		if ((edge->m_incidentFace > 0) && (edge->m_mark < mark)) {
			dgInt32 count;
			dgEdge * ptr;
			dgVertexAtribute face[8];

			count = 0;
			ptr = edge;
			do {
				ptr->m_mark = mark;
				face[count] = source->m_attib[ptr->m_userData];
				face[count].m_normal.m_x *= dgFloat32 (-1.0f);
				face[count].m_normal.m_y *= dgFloat32 (-1.0f);
				face[count].m_normal.m_z *= dgFloat32 (-1.0f);
				count ++;
				ptr = ptr->m_prev;
			} while (ptr != edge);

			_ASSERTE (count == 3);
			AddPolygon(count, &face[0].m_vertex.m_x, sizeof (dgVertexAtribute), face[0].m_material);
		}
	}
*/
}






dgMeshEffect* dgMeshEffect::Union (const dgMatrix& matrix, const dgMeshEffect* clipMesh) const
{
	dgMeshEffect clipper (*clipMesh);
	dgMeshEffect* leftMeshSource;
	dgMeshEffect* rightMeshSource;
	dgMeshEffect* leftMeshClipper;
	dgMeshEffect* rightMeshClipper;
	dgMeshEffect* result;

	clipper.TransformMesh (matrix);

	result = NULL;
	leftMeshSource = NULL;
	rightMeshSource = NULL;
	leftMeshClipper = NULL;
	rightMeshClipper = NULL;

	ClipMesh (&clipper, &leftMeshSource, &rightMeshSource);
	if (leftMeshSource && rightMeshSource) {
		clipper.ClipMesh (this, &leftMeshClipper, &rightMeshClipper);
		if (leftMeshSource && rightMeshSource) {
			result = new (GetAllocator()) dgMeshEffect (GetAllocator(), true);
			result->BeginPolygon();
			result->MergeFaces(rightMeshSource);
			result->MergeFaces(rightMeshClipper);
			result->EndPolygon(dgFloat64 (1.0e-5f));
		}
	}

	if (leftMeshClipper) {
		delete leftMeshClipper;
	}

	if (rightMeshClipper) {
		delete rightMeshClipper;
	}

	if (leftMeshSource) {
		delete leftMeshSource;
	}

	if (rightMeshSource) {
		delete rightMeshSource;
	}

	return result;

}


dgMeshEffect* dgMeshEffect::Difference (const dgMatrix& matrix, const dgMeshEffect* clipMesh) const
{
//	dgMeshEffect source (*this);
	dgMeshEffect clipper (*clipMesh);
	dgMeshEffect* leftMeshSource;
	dgMeshEffect* rightMeshSource;
	dgMeshEffect* leftMeshClipper;
	dgMeshEffect* rightMeshClipper;
	dgMeshEffect* result;

	clipper.TransformMesh (matrix);

	result = NULL;
	leftMeshSource = NULL;
	rightMeshSource = NULL;
	leftMeshClipper = NULL;
	rightMeshClipper = NULL;

	ClipMesh (&clipper, &leftMeshSource, &rightMeshSource);
	if (leftMeshSource && rightMeshSource) {
		clipper.ClipMesh (this, &leftMeshClipper, &rightMeshClipper);
		if (leftMeshSource && rightMeshSource) {
			result = new (GetAllocator()) dgMeshEffect (GetAllocator(), true);

			result->BeginPolygon();
			result->MergeFaces(rightMeshSource);
			result->ReverseMergeFaces(leftMeshClipper);
			result->EndPolygon(dgFloat64 (1.0e-5f));
		}
	}

	if (leftMeshClipper) {
		delete leftMeshClipper;
	}

	if (rightMeshClipper) {
		delete rightMeshClipper;
	}

	if (leftMeshSource) {
		delete leftMeshSource;
	}

	if (rightMeshSource) {
		delete rightMeshSource;
	}

	return result;
}


dgMeshEffect* dgMeshEffect::Intersection (const dgMatrix& matrix, const dgMeshEffect* clipMesh) const
{
//	dgMeshEffect source (*this);
	dgMeshEffect clipper (*clipMesh);
	dgMeshEffect* leftMeshSource;
	dgMeshEffect* rightMeshSource;
	dgMeshEffect* leftMeshClipper;
	dgMeshEffect* rightMeshClipper;
	dgMeshEffect* result;

	clipper.TransformMesh (matrix);

	result = NULL;
	leftMeshSource = NULL;
	rightMeshSource = NULL;
	leftMeshClipper = NULL;
	rightMeshClipper = NULL;

	ClipMesh (&clipper, &leftMeshSource, &rightMeshSource);
	if (leftMeshSource && rightMeshSource) {
		clipper.ClipMesh (this, &leftMeshClipper, &rightMeshClipper);
		if (leftMeshSource && rightMeshSource) {
			result = new (GetAllocator()) dgMeshEffect (GetAllocator(), true);
			result->BeginPolygon();
			result->MergeFaces(leftMeshSource);
			result->MergeFaces(leftMeshClipper);
			result->EndPolygon(dgFloat64 (1.0e-5f));
		}
	}

	if (leftMeshClipper) {
		delete leftMeshClipper;
	}

	if (rightMeshClipper) {
		delete rightMeshClipper;
	}

	if (leftMeshSource) {
		delete leftMeshSource;
	}

	if (rightMeshSource) {
		delete rightMeshSource;
	}

	return result;
}


void dgMeshEffect::ClipMesh (const dgMatrix& matrix, const dgMeshEffect* clipMesh, dgMeshEffect** left, dgMeshEffect** right) const
{
	dgMeshEffect clipper (*clipMesh);
	clipper.TransformMesh (matrix);

	if (clipMesh->m_isFlagFace) {
		*left = NULL;
		*right = NULL;
		PlaneClipMesh (&clipper, left, right);
	
	} else {
		dgMeshEffect* leftMeshSource;
		dgMeshEffect* rightMeshSource;
		dgMeshEffect* leftMeshClipper;
		dgMeshEffect* rightMeshClipper;

		leftMeshSource = NULL;
		rightMeshSource = NULL;
		leftMeshClipper = NULL;
		rightMeshClipper = NULL;

		ClipMesh (&clipper, &leftMeshSource, &rightMeshSource);
		if (leftMeshSource && rightMeshSource) {
			clipper.ClipMesh (this, &leftMeshClipper, &rightMeshClipper);
			if (leftMeshSource && rightMeshSource) {

				dgMeshEffect* leftMesh;
				dgMeshEffect* rightMesh;

				leftMesh = new (GetAllocator()) dgMeshEffect (GetAllocator(), true);
				rightMesh = new (GetAllocator()) dgMeshEffect (GetAllocator(), true);

				leftMesh->BeginPolygon();
				rightMesh->BeginPolygon();

				leftMesh->MergeFaces(leftMeshSource);
				leftMesh->MergeFaces(leftMeshClipper);

				rightMesh->MergeFaces(rightMeshSource);
				rightMesh->ReverseMergeFaces(leftMeshClipper);

				leftMesh->EndPolygon(dgFloat64 (1.0e-5f));
				rightMesh->EndPolygon(dgFloat64 (1.0e-5f));

				*left = leftMesh;
				*right = rightMesh;
			}
		}


		if (leftMeshClipper) {
			delete leftMeshClipper;
		}

		if (rightMeshClipper) {
			delete rightMeshClipper;
		}


		if (leftMeshSource) {
			delete leftMeshSource;
		}

		if (rightMeshSource) {
			delete rightMeshSource;
		}
	}
}





dgMeshEffectSolidTree* dgMeshEffect::CreateSolidTree() const
{
	dgMeshEffectSolidTree* tree = NULL;
	dgInt32 mark = IncLRU();
	dgPolyhedra::Iterator srcIter (*this);
	for (srcIter.Begin(); srcIter; srcIter ++){
		dgEdge* const face = &(*srcIter);
		if ((face->m_incidentFace > 0) && (face->m_mark != mark)) {
			dgEdge* ptr = face;
			do {
				ptr->m_mark = mark;
				ptr = ptr->m_next;
			} while (ptr != face);

			if (!tree) {
				dgBigVector normal (FaceNormal (face, &m_points[0][0], sizeof (dgBigVector)));
				dgFloat64 mag2 = normal % normal;
				if (mag2 > dgFloat32 (1.0e-10f)) {
					tree = new (GetAllocator()) dgMeshEffectSolidTree (*this, face);
				}
			} else {
				tree->AddFace (*this, face);
			}
		}
	}
	_ASSERTE (tree);
	return tree;
}

void dgMeshEffect::DestroySolidTree (dgMeshEffectSolidTree* const tree)
{
	delete tree;
}



void dgMeshEffect::ClipMesh (const dgMeshEffect* const clipMesh, dgMeshEffect** left, dgMeshEffect** right) const
{
	const dgMeshEffectSolidTree* const clipper = clipMesh->CreateSolidTree();
	_ASSERTE (clipper);
	ClipMesh (clipper, left, right);
	delete clipper;
}


bool dgMeshEffect::CheckIntersection (const dgMeshEffectSolidTree* const solidTree, dgFloat64 scale) const
{
_ASSERTE (0);
return false;
/*

	if (solidTree) {
		dgInt32 mark;
		dgInt32 count;
		dgVector center (dgFloat32 (0.0f), dgFloat32 (0.0f), dgFloat32 (0.0f), dgFloat32 (0.0f));

		count = 0;
		mark = IncLRU();
		dgPolyhedra::Iterator iter (*this);
		for (iter.Begin(); iter; iter ++){
			dgEdge* face;
			face = &(*iter);
			if (face->m_mark != mark) {
				dgEdge* ptr;
				ptr = face;
				do {
					ptr->m_mark = mark;
					ptr = ptr->m_twin->m_next;
				} while (ptr != face);
				count ++;
				center += m_points[face->m_incidentVertex];
			}
		}
		center = center.Scale (dgFloat32 (1.0f) / dgFloat32(count));

		dgMatrix matrix (dgGetIdentityMatrix());
		matrix[0][0] = scale;
		matrix[1][1] = scale;
		matrix[2][2] = scale;
		matrix.m_posit = center - matrix.RotateVector(center);
		matrix[3][3] = dgFloat32 (1.0f);

		mark = IncLRU();
		for (iter.Begin(); iter; iter ++){
			dgEdge* face;
			face = &(*iter);
			if (face->m_incidentFace > 0) {
				if (face->m_mark != mark) {

					dgInt32 stack;	
					dgInt32 frontCount;
					dgInt32 backCount;
					dgEdge* ptr;
					dgMeshTreeCSGFace* meshFace;
					dgMeshTreeCSGPointsPool points;
					dgMeshTreeCSGFace* faceOnStack[DG_MESH_EFFECT_BOLLEAN_STACK];
					const dgMeshEffectSolidTree* stackPool[DG_MESH_EFFECT_BOLLEAN_STACK];

					backCount = 0;
					frontCount = 0;
					meshFace = new (GetAllocator()) dgMeshTreeCSGFace(GetAllocator());
					ptr = face;
					do {
						dgInt32 index;
						index = points.AddPoint (matrix.TransformVector(m_points[ptr->m_incidentVertex]));
						meshFace->AddPoint (index);

						ptr->m_mark = mark;
						ptr = ptr->m_next;
					} while (ptr != face);


					stack = 1;
					stackPool[0] = solidTree;
					faceOnStack[0] = meshFace;
					meshFace->AddRef();

					while (stack) {
						dgMeshTreeCSGFace* rootFace; 
						dgMeshTreeCSGFace* leftFace; 
						dgMeshTreeCSGFace* rightFace;
						const dgMeshEffectSolidTree* root;

						stack --;
						root = stackPool[stack];
						rootFace = faceOnStack[stack];

						ClipFace (root->m_plane, rootFace, &leftFace, &rightFace, points);
						rootFace->Release();

						if (rightFace) {
							_ASSERTE (rightFace->CheckConvex(this, face, points));

							if (root->m_front) {
								stackPool[stack] = root->m_front;
								faceOnStack[stack] = rightFace;
								stack ++;
								_ASSERTE (stack < sizeof (stackPool) / sizeof (stackPool[0]));
							} else {
								rightFace->Release();
								frontCount ++;
							}
						}

						if (leftFace) {
							_ASSERTE (leftFace->CheckConvex(this, face, points));
							if (root->m_back) {
								stackPool[stack] = root->m_back;
								faceOnStack[stack] = leftFace;
								stack ++;
								_ASSERTE (stack < sizeof (stackPool) / sizeof (stackPool[0]));
							} else {
								leftFace->Release();
								backCount ++;
							}
						}
					}

					meshFace->Release();
					if (backCount) {
						return true;
					}
				}
			}
		}
	}
	return false;
*/
}



void dgMeshEffect::PlaneClipMesh (const dgMeshEffect* planeMesh, dgMeshEffect** left, dgMeshEffect** right) const
{
_ASSERTE (0);
/*

	dgEdge* face;
	dgMeshEffect* leftMesh;
	dgMeshEffect* rightMesh;

	_ASSERTE (planeMesh->m_isFlagFace);
	face = &planeMesh->GetRoot()->GetInfo();
	if (face->m_incidentFace < 0) {
		face = face->m_twin;
	}
	_ASSERTE (face->m_incidentFace > 0);
	dgVector normal (planeMesh->FaceNormal (face, &planeMesh->m_points[0][0], sizeof (dgVector)));
	normal = normal.Scale (dgRsqrt (normal % normal));
	const dgVector& point = planeMesh->m_points[face->m_incidentVertex];
	dgPlane plane (normal, -(point % normal));

	dgMeshEffect tmp (*this);
	tmp.PlaneClipMesh (plane, left, right);

//	_ASSERTE (tmp.CheckSingleMesh());

	leftMesh = *left;
	rightMesh = *right;

	if (leftMesh && rightMesh) {
		_ASSERTE (leftMesh->GetCount());
		_ASSERTE (rightMesh->GetCount());
		if (!(leftMesh->PlaneApplyCap (planeMesh, plane) && rightMesh->PlaneApplyCap (planeMesh, plane.Scale (dgFloat32 (-1.0f))))) {
			leftMesh->Release();
			rightMesh->Release();
			*left = NULL;
			*right = NULL;
		} else {
			leftMesh->Triangulate ();
			rightMesh->Triangulate ();
//			_ASSERTE (rightMesh->CheckSingleMesh());
//			_ASSERTE (leftMesh->CheckSingleMesh());
		}
	}
*/
}


void dgMeshEffect::PlaneClipMesh (const dgPlane& plane, dgMeshEffect** left, dgMeshEffect** right)
{
_ASSERTE (0);
 /*

	dgInt32 mark;	
	dgInt32 rightCount;
	dgInt32 leftCount;
	dgMeshEffect* leftMesh;
	dgMeshEffect* rightMesh;

	*left = NULL;
	*right = NULL;

	dgTree<dgFlatClipEdgeAttr, dgEdge*> leftFilter(GetAllocator());
	dgTree<dgFlatClipEdgeAttr, dgEdge*> rightFilter(GetAllocator());
	dgStack<dgInt8> vertexSidePool(GetCount() * 2 + 256);
	dgInt8* const vertexSide = &vertexSidePool[0];

	leftCount = 0;
	rightCount = 0;
	mark = IncLRU();
	dgPolyhedra::Iterator iter (*this);
	for (iter.Begin(); iter; iter ++){
		dgEdge* vertex;

		vertex = &(*iter);
		if (vertex->m_mark != mark) {
			dgEdge* ptr;
			dgFloat32 test;

			ptr = vertex;
			do {
				ptr->m_mark = mark;
				ptr = ptr->m_twin->m_next;
			} while (ptr != vertex);

			test = plane.Evalue(m_points[vertex->m_incidentVertex]);
			if (test >= dgFloat32 (1.0e-3f)) {
				rightCount ++;
				vertexSide[vertex->m_incidentVertex] = 1;
			} else if (test <= dgFloat32 (-1.0e-3f)) {
				leftCount ++;
				vertexSide[vertex->m_incidentVertex] = -2;
			} else {
				vertexSide[vertex->m_incidentVertex] = 0;
			}
			m_points[vertex->m_incidentVertex].m_w = test;
		}
	}


	if ((rightCount == 0) || (leftCount == 0)) {
		return;
	}

	mark = IncLRU();
	for (iter.Begin(); iter; ){
		dgEdge* edge;

		edge = &(*iter);
		iter ++;
		if (&(*iter) == edge->m_twin) {
			iter ++;
		}
		if (edge->m_mark != mark) {
			edge->m_twin->m_mark = mark;

			if (vertexSide[edge->m_incidentVertex] * vertexSide[edge->m_twin->m_incidentVertex] < 0) {
				dgFloat32 test0;
				dgFloat32 param;

				test0 = m_points[edge->m_incidentVertex].m_w;

				dgVector dp (m_points[edge->m_twin->m_incidentVertex] - m_points[edge->m_incidentVertex]);
				param = -test0 / (plane % dp);

				InsertEdgeVertex (edge, param);
				vertexSide[m_pointCount - 1] = 0;
			}
		}
	}

	mark = IncLRU();
	for (iter.Begin(); iter; iter ++){
		dgEdge* face;

		face = &(*iter);
		if ((face->m_incidentFace > 0) && (face->m_mark != mark) && (vertexSide[face->m_incidentVertex] == 0)) {
			dgInt32 side;
			dgEdge* ptr;

			ptr = face;
			do {
				ptr->m_mark = mark;
				ptr = ptr->m_next;
			} while (ptr != face);

			side = 0;
			ptr = face->m_next;
			do {
				side |= vertexSide[ptr->m_incidentVertex];
				if (vertexSide[ptr->m_incidentVertex] == 0) {
					_ASSERTE (side != -1);
					if (side > 0) {
						if (ptr->m_next != face) {
							dgEdge* back;
							dgEdge* front;

							front = AddHalfEdge(ptr->m_incidentVertex, face->m_incidentVertex);
							back = AddHalfEdge(face->m_incidentVertex, ptr->m_incidentVertex);
							_ASSERTE (back);
							_ASSERTE (front);

							back->m_mark = mark;
							front->m_mark = mark;

							back->m_incidentFace = face->m_incidentFace;
							front->m_incidentFace = face->m_incidentFace;

							back->m_userData = face->m_userData;
							front->m_userData = ptr->m_userData;

							back->m_twin = front;
							front->m_twin = back;

							back->m_next = ptr;
							front->m_next = face;

							back->m_prev = face->m_prev;
							front->m_prev = ptr->m_prev;

							ptr->m_prev->m_next = front;
							ptr->m_prev = back;

							face->m_prev->m_next = back;
							face->m_prev = front;
						}

					} else if (side < 0){
						if (ptr->m_next != face) {
							dgEdge* back;
							dgEdge* front;

							back = AddHalfEdge(ptr->m_incidentVertex, face->m_incidentVertex);
							front = AddHalfEdge(face->m_incidentVertex, ptr->m_incidentVertex);
							_ASSERTE (back);
							_ASSERTE (front);

							back->m_mark = mark;
							front->m_mark = mark;

							back->m_incidentFace = face->m_incidentFace;
							front->m_incidentFace = face->m_incidentFace;

							back->m_userData = ptr->m_userData;
							front->m_userData = face->m_userData;

							back->m_twin = front;
							front->m_twin = back;

							back->m_next = face;
							front->m_next = ptr;

							back->m_prev = ptr->m_prev;
							front->m_prev = face->m_prev;

							ptr->m_prev->m_next = back;
							ptr->m_prev = front;

							face->m_prev->m_next = front;
							face->m_prev = back;
						}
					}
					break;
				}
				ptr = ptr->m_next;
			} while (ptr != face);
		}
	}

	leftMesh = new (GetAllocator()) dgMeshEffect (GetAllocator(), true);
	rightMesh = new (GetAllocator()) dgMeshEffect (GetAllocator(), true);

	mark = IncLRU();
	leftMesh->BeginPolygon();
	rightMesh->BeginPolygon(); 
	for (iter.Begin(); iter; iter ++){
		dgEdge* face;

		face = &(*iter);
		if ((face->m_incidentFace > 0) && (face->m_mark != mark) && (vertexSide[face->m_incidentVertex] != 0)) {
			dgInt32 count;
			dgEdge* ptr;
			dgVertexAtribute att[128];

			count = 0;
			ptr = face;
			do {
				att[count] = m_attib[ptr->m_userData];
				count ++;
				ptr->m_mark = mark;
				ptr = ptr->m_next;
			} while (ptr != face);

			if (vertexSide[face->m_incidentVertex] > 0) {
				rightMesh->AddPolygon(count, &att[0].m_vertex.m_x, sizeof (dgVertexAtribute), att[0].m_material);
			} else {
				leftMesh->AddPolygon(count, &att[0].m_vertex.m_x, sizeof (dgVertexAtribute), att[0].m_material);
			}
		}
	}

	leftMesh->EndPolygon();
	rightMesh->EndPolygon(); 

	if (!(leftMesh->GetCount() && rightMesh->GetCount())) {
		leftMesh->Release();
		rightMesh->Release();
		rightMesh = NULL;
		leftMesh = NULL;
	}

	*left = leftMesh;
	*right = rightMesh;
*/
}


bool dgMeshEffect::CheckSingleMesh() const
{
_ASSERTE (0);
return false;
 /*

	bool ret;
	dgPolyhedra firstSegment(GetAllocator());
	dgPolyhedra secundSegment(GetAllocator());

	dgPolyhedra::Iterator iter (*this);
	for (iter.Begin(); iter; iter ++){
		dgFloat32 err2;
		dgEdge* vertex;
		vertex = &(*iter);
		if (vertex->m_incidentFace >= 0) {
			dgVector p (m_attib[vertex->m_userData].m_vertex.m_x, m_attib[vertex->m_userData].m_vertex.m_y, m_attib[vertex->m_userData].m_vertex.m_z, dgFloat32 (0.0f));
			dgVector err (m_points[vertex->m_incidentVertex] - p);
			err2 = err % err;
			_ASSERTE (err2 < dgFloat32 (1.0e-10f));
		} 
	}

	BeginConectedSurface();
	GetConectedSurface (firstSegment);
	GetConectedSurface (secundSegment);
	EndConectedSurface();
	ret = (firstSegment.GetCount() > 0) & (secundSegment.GetCount() == 0);
	return ret;
*/
}

dgInt32 dgMeshEffect::PlaneApplyCap (const dgMeshEffect* planeMesh, const dgPlane& faceNormal)
{
_ASSERTE (0);
return 0;
/*

	dgInt32 ret;
	dgInt32 mark;

	dgEdge* plane;
	plane = &planeMesh->GetRoot()->GetInfo();
	if (plane->m_incidentFace < 0) {
		plane = plane->m_twin;
	}
	_ASSERTE (plane->m_incidentFace > 0);

	ret = 0;
	mark = IncLRU();
	dgPolyhedra::Iterator iter (*this);
	for (iter.Begin(); iter; ) {
		dgInt32 isBorder;
		dgEdge* face;
		face = &(*iter);

		iter++;

		isBorder = 0;
		if ((face->m_incidentFace < 0) && (face->m_mark != mark)) {
			dgFloat32 maxDist;
			dgEdge* ptr;

			maxDist = dgFloat32 (0.0f);
			ptr = face;
			do {
				maxDist = GetMax (maxDist, dgAbsf (faceNormal.Evalue(m_points[ptr->m_incidentVertex])));
				ptr->m_mark = mark;
				ptr = ptr->m_next;
			} while (ptr != face); 

			if (maxDist <= dgFloat32 (1.5e-3f)) {
				bool haveColinear;

				haveColinear = true;
				ptr = face;
				while (haveColinear) {
					haveColinear = false;
					do {
						if (ptr->m_next->m_twin->m_next->m_twin != ptr) {
							dgFloat32 err;
							dgFloat32 mag00;
							dgFloat32 mag11;
							dgFloat32 mag01;
							dgFloat32 epsilon;

							dgVector e0 (m_points[ptr->m_next->m_incidentVertex] - m_points[ptr->m_incidentVertex]);
							dgVector e1 (m_points[ptr->m_next->m_next->m_incidentVertex] - m_points[ptr->m_next->m_incidentVertex]);

							mag00 = e0 % e0;
							mag11 = e1 % e1;
							mag01 = e0 % e1;

							epsilon = dgFloat32 (1.0e-6f) * mag00 * mag11;
							err = mag01 * mag01 - mag00 * mag11;
							if (dgAbsf (err) < epsilon) {
								dgEdge* ptr1;
								_ASSERTE (ptr->m_twin->m_incidentFace >= 0);

								dgVector normal0 (FaceNormal(ptr->m_twin, &m_points[0].m_x, sizeof (dgVector)));
								mag00 = normal0 % normal0;
								ptr1 = ptr->m_twin->m_prev->m_twin;
								do {
									dgVector normal1 (FaceNormal(ptr1->m_twin, &m_points[0].m_x, sizeof (dgVector)));
								
									mag11 = normal1 % normal1;
									mag01 = normal0 % normal1;
									epsilon = dgFloat32 (1.0e-6f) * mag00 * mag11;
									err = mag01 * mag01 - mag00 * mag11;
									if (dgAbsf (err) < epsilon) {
										if (iter && ((&(*iter) == ptr1) || (&(*iter) == ptr1->m_twin))) {
											iter --;
										}
										if (iter && ((&(*iter) == ptr1) || (&(*iter) == ptr1->m_twin))) {
											iter --;
										}
										haveColinear = true;
										DeleteEdge(ptr1);
										ptr1 = ptr->m_twin;
										
									}

									ptr1 = ptr1->m_prev->m_twin;

								} while (ptr1 != ptr->m_next);

								if (ptr->m_next->m_twin->m_next->m_twin == ptr) {
									dgEdge* next;
									dgTreeNode* node;

									if (iter && ((&(*iter) == ptr->m_next) || (&(*iter) == ptr->m_next->m_twin))) {
										iter --;
										if (iter && ((&(*iter) == ptr->m_next) || (&(*iter) == ptr->m_next->m_twin))) {
											iter --;
										}
									}

									if (ptr->m_next == face) {
										face = face->m_prev;
									}
									
									ptr->m_twin->m_userData = ptr->m_next->m_twin->m_userData;
									ptr->m_twin->m_incidentVertex = ptr->m_next->m_twin->m_incidentVertex;

									next = ptr->m_next;
									ptr->m_next->m_next->m_prev = ptr;
									ptr->m_next = ptr->m_next->m_next;
									ptr->m_twin->m_prev->m_prev->m_next = ptr->m_twin;
									ptr->m_twin->m_prev = ptr->m_twin->m_prev->m_prev;

									next->m_next = next->m_twin;
									next->m_prev = next->m_twin;
									next->m_twin->m_next = next;
									next->m_twin->m_prev = next;
									DeleteEdge (next);

									node = GetNodeFromInfo(*ptr);
									dgPairKey key0 (ptr->m_incidentVertex, ptr->m_twin->m_incidentVertex);
									if (Find(key0.GetVal())) {
										return 0;
									}
									node = ReplaceKey (node, key0.GetVal());

									node = GetNodeFromInfo(*ptr->m_twin);
									dgPairKey key1 (ptr->m_twin->m_incidentVertex, ptr->m_incidentVertex);
									if (Find(key1.GetVal())) {
										return 0;
									}
									node = ReplaceKey (node, key1.GetVal());
								}
							}
						}

						ptr = ptr->m_next;
					} while (ptr != face) ; 
				}


				ptr = face;
				do {
					dgVertexAtribute attrib (planeMesh->InterpolateVertex (m_points[ptr->m_incidentVertex], plane));
					attrib.m_normal.m_x = faceNormal.m_x;
					attrib.m_normal.m_y = faceNormal.m_y;
					attrib.m_normal.m_z = faceNormal.m_z;

					AddAtribute(attrib);

					ptr->m_userData = m_atribCount - 1;
					ptr->m_incidentFace = 1;

					ptr = ptr->m_next;
				} while (ptr != face); 
//				dgVector normal;
//				TriangulateFace (face, &m_points[0].m_x, sizeof (dgVector), normal);
				ret = 1;
			}
		}
	}
	return ret;
*/
}

bool dgMeshEffect::HasOpenEdges () const
{
	dgPolyhedra::Iterator iter (*this);
	for (iter.Begin(); iter; iter ++){
		dgEdge* const face = &(*iter);
		if (face->m_incidentFace < 0){
			return true;
		}
	}
	return false;
}




dgFloat64 dgMeshEffect::CalculateVolume () const
{
	_ASSERTE (0);
	return 0;
	/*

	dgPolyhedraMassProperties localData;

	dgInt32 mark = IncLRU();
	dgPolyhedra::Iterator iter (*this);
	for (iter.Begin(); iter; iter ++){
		dgInt32 count;
		dgEdge* ptr;
		dgEdge* face;
		dgVector points[256];
		
		face = &(*iter);
		if ((face->m_incidentFace > 0) && (face->m_mark != mark)) {
			count = 0;
			ptr = face;
			do {
				points[count] = m_points[ptr->m_incidentVertex];
				count ++;
				ptr->m_mark = mark;
				ptr = ptr->m_next;
			} while (ptr != face);
			localData.AddCGFace (count, points);
		}
	}

	dgFloat32 volume;
	dgVector p0;
	dgVector p1;
	dgVector com;
	dgVector inertia;
	dgVector crossInertia;
	volume = localData.MassProperties (com, inertia, crossInertia);
	return volume;
*/
}




bool dgMeshEffect::SeparateDuplicateLoops (dgEdge* const face)
{
	for (dgEdge* ptr0 = face; ptr0 != face->m_prev; ptr0 = ptr0->m_next) {
		dgInt32 index = ptr0->m_incidentVertex;

		dgEdge* ptr1 = ptr0->m_next; 
		do {
			if (ptr1->m_incidentVertex == index) {
				dgEdge* const ptr00 = ptr0->m_prev;
				dgEdge* const ptr11 = ptr1->m_prev;

				ptr00->m_next = ptr1;
				ptr1->m_prev = ptr00;

				ptr11->m_next = ptr0;
				ptr0->m_prev = ptr11;

				return true;
			}

			ptr1 = ptr1->m_next;
		} while (ptr1 != face);
	}

	return false;
}


dgMeshEffect* dgMeshEffect::GetNextLayer (dgInt32 mark)
{
	Iterator iter(*this);
	dgEdge* edge = NULL;
	for (iter.Begin (); iter; iter ++) {
		edge = &(*iter);
		if ((edge->m_mark < mark) && (edge->m_incidentFace > 0)) {
			break;
		}
	}

	if (!edge) {
		return NULL;
	}

	dgInt32 layer = dgInt32 (m_points[edge->m_incidentVertex].m_w);
	dgPolyhedra polyhedra(GetAllocator());

	polyhedra.BeginFace ();
	for (iter.Begin (); iter; iter ++) {
		dgEdge* const edge = &(*iter);
		if ((edge->m_mark < mark) && (edge->m_incidentFace > 0)) {
			dgInt32 thislayer = dgInt32 (m_points[edge->m_incidentVertex].m_w);
			if (thislayer == layer) {
				dgEdge* ptr = edge;
				dgInt32 count = 0;
				dgInt32 faceIndex[256];
				dgInt64 faceDataIndex[256];
				do {
					ptr->m_mark = mark;
					faceIndex[count] = ptr->m_incidentVertex;
					faceDataIndex[count] = ptr->m_userData;
					count ++;
					_ASSERTE (count < sizeof (faceIndex)/ sizeof(faceIndex[0]));
					ptr = ptr->m_next;
				} while (ptr != edge);
				polyhedra.AddFace (count, &faceIndex[0], &faceDataIndex[0]);
			}
		}
	}
	polyhedra.EndFace ();

	dgMeshEffect* solid = NULL;
	if (polyhedra.GetCount()) {
		solid = new (GetAllocator()) dgMeshEffect(polyhedra, *this);
		solid->SetLRU(mark);
	}
	return solid;
}


void dgMeshEffect::ClipMesh (const dgMeshEffectSolidTree* const clipper, dgMeshEffect** left, dgMeshEffect** right) const
{
	dgMeshEffect mesh (dgMeshEffect (*this));
	mesh.Triangulate();


	dgTree<dgEdge*,dgEdge*> edgeList(GetAllocator());

	dgPolyhedra::Iterator iter (mesh);
	for (iter.Begin(); iter; iter ++){
		dgEdge* const edge = &(*iter);
		edgeList.Insert(edge, edge);
	}


	dgMeshEffect* const leftMesh = new (GetAllocator()) dgMeshEffect (GetAllocator(), true);
	dgMeshEffect* const rightMesh = new (GetAllocator()) dgMeshEffect (GetAllocator(), true);

	leftMesh->BeginPolygon();
	rightMesh->BeginPolygon();


	dgInt32 rightFaceId = 1 << 24;
	dgInt32 leftFaceId =  2 << 24;
	dgInt32 mark = mesh.IncLRU();
	while (edgeList.GetRoot()){
		dgEdge* face = edgeList.GetRoot()->GetInfo();
		edgeList.Remove (edgeList.GetRoot());
		
		if ((face->m_incidentFace > 0) && (face->m_mark != mark)) {
			dgMeshTreeCSGFace clipFace (mesh, face);
			dgEdge* ptr = face;
			do {
				edgeList.Remove(ptr);
				_ASSERTE (ptr->m_incidentFace < (leftFaceId | rightFaceId));
				ptr->m_mark = mark;
				ptr = ptr->m_next;
			} while (ptr != face);

			dgEdge* faceOnStack[DG_MESH_EFFECT_BOLLEAN_STACK];
			const dgMeshEffectSolidTree* stackPool[DG_MESH_EFFECT_BOLLEAN_STACK];

			dgInt32 stack = 1;
			ptr = &clipFace.GetRoot()->GetInfo();
			if (ptr->m_incidentFace < 0) {
				ptr = ptr->m_twin;
			}
			_ASSERTE (ptr->m_incidentFace > 0);
			faceOnStack[0] = ptr;
			stackPool[0] = clipper;


			bool hasLeftFaces = false;
			bool hasRightFaces = false;
			while (stack) {
				stack --;
				dgEdge* const face = faceOnStack[stack];
				const dgMeshEffectSolidTree* const root = stackPool[stack];

				dgEdge* leftFace; 
				dgEdge* rightFace;
				clipFace.ClipFace (face, root->m_normal, root->m_origin, &leftFace, &rightFace);

				if (rightFace) {
					dgEdge* ptr = rightFace;
					do {
						ptr->m_incidentFace = rightFaceId + 1;
						ptr = ptr->m_next;
					} while (ptr != rightFace);

					if (root->m_front) {
						stackPool[stack] = root->m_front;
						faceOnStack[stack] = rightFace;
						stack ++;
						_ASSERTE (stack < sizeof (stackPool) / sizeof (stackPool[0]));
					} else {
						hasRightFaces = true;
					}
				}

				if (leftFace) {
					dgEdge* ptr = leftFace;
					do {
						ptr->m_incidentFace = leftFaceId + 1;
						ptr = ptr->m_next;
					} while (ptr != leftFace);

					if (root->m_back) {
						stackPool[stack] = root->m_back;
						faceOnStack[stack] = leftFace;
						stack ++;
						_ASSERTE (stack < sizeof (stackPool) / sizeof (stackPool[0]));
					} else {
						hasLeftFaces = true;
					}
				}
			}

			_ASSERTE (clipFace.CheckConsistency ());
			if (hasLeftFaces && hasRightFaces) {

				dgInt32 clipMark = clipFace.IncLRU();
				dgMeshTreeCSGFace::Iterator clipFaceIter (clipFace);
				for (clipFaceIter.Begin(); clipFaceIter; clipFaceIter ++) {
					dgEdge* const edge = &(*clipFaceIter);
					if ((edge->m_incidentFace > 0) && (edge->m_mark != clipMark)) {
						dgInt32 faceId = edge->m_incidentFace;

						_ASSERTE ((faceId & leftFaceId) | (faceId & rightFaceId));

						dgInt32 count = 0;
						dgVertexAtribute facePoints[256];

						dgEdge* ptr = edge;
						do {
							_ASSERTE (ptr->m_incidentFace & faceId);
							_ASSERTE ((ptr->m_incidentFace & leftFaceId) | (ptr->m_incidentFace & rightFaceId));

							facePoints[count] = clipFace.m_points[ptr->m_incidentVertex];
							_ASSERTE (facePoints[count].m_vertex.m_w == dgFloat64 (0.0f));
							count ++;
							ptr->m_mark = clipMark;
							ptr = ptr->m_next;
						} while (ptr != edge);


						if (faceId & leftFaceId) {
							leftMesh->AddPolygon(count, &facePoints[0].m_vertex.m_x, sizeof (dgVertexAtribute), dgFastInt (facePoints[0].m_material));
						} else {
							rightMesh->AddPolygon(count, &facePoints[0].m_vertex.m_x, sizeof (dgVertexAtribute), dgFastInt (facePoints[0].m_material));
						}
					}
				}

			} else {
				_ASSERTE ((hasLeftFaces & !hasRightFaces) | (!hasLeftFaces & hasRightFaces));
				//dgInt32 faceId = hasLeftFaces ? leftFaceId : rightFaceId;
				dgMeshEffect* const dst = hasLeftFaces ? leftMesh : rightMesh;
				dgEdge* ptr = face;
				dgInt32 count = 0;
				dgVertexAtribute facePoints[256];
				do {
					//ptr->m_incidentFace |= faceId;
					facePoints[count] = mesh.m_attib[ptr->m_userData];
					_ASSERTE (facePoints[count].m_vertex.m_w == dgFloat64 (0.0f));
					count ++;
					ptr = ptr->m_next;
				} while (ptr != face);
				dst->AddPolygon(count, &facePoints[0].m_vertex.m_x, sizeof (dgVertexAtribute), dgFastInt (facePoints[0].m_material));
			}
		}
	}
	leftMesh->EndPolygon(dgFloat64 (dgFloat64 (1.0e-5f)));
	rightMesh->EndPolygon(dgFloat64 (dgFloat64 (1.0e-5f)));

	if (leftMesh->GetCount() && rightMesh->GetCount()) {
		*left = leftMesh;
		*right = rightMesh;
	} else if (rightMesh->GetCount()){
		*left = NULL;
		*right = rightMesh;
		leftMesh->Release();
	} else {
		*right = NULL;
		*left = leftMesh;
		rightMesh->Release();
	}
}


void dgMeshEffect::RepairTJoints ()
{
	dgInt32 mark = IncLRU();
	dgPolyhedra::Iterator iter (*this);
#ifdef _DEBUG
	for (iter.Begin(); iter; ) {
		dgEdge* const face = &(*iter);
		iter ++;
		if ((face->m_incidentFace < 0) && (face->m_mark != mark)) {
			for (dgEdge* ptr = face; ptr != face->m_prev; ptr = ptr->m_next) {
				dgBigVector p0 (m_points[ptr->m_incidentVertex]);
				for (dgEdge* ptr1 = ptr->m_next; ptr1 != face; ptr1 = ptr1->m_next) {
					if (ptr->m_incidentVertex != ptr1->m_incidentVertex) {
						dgBigVector p1 (m_points[ptr1->m_incidentVertex]);
						dgBigVector dp (p1 - p0);
						dgFloat64 err2 (dp % dp);
						if (err2 < dgFloat64 (1.0e-16f)) {
							_ASSERTE (0);
						}
					}
				} 
			}
		}
	}
	mark = IncLRU();
#endif

	for (iter.Begin(); iter; ) {
		dgEdge* const face = &(*iter);
		iter ++;
		if ((face->m_incidentFace < 0) && (face->m_mark != mark)) {
			// vertices project 
			while (SeparateDuplicateLoops (face));

			dgBigVector dir (dgFloat64 (0.0f), dgFloat64 (0.0f), dgFloat64 (0.0f), dgFloat64 (0.0f));
			dgFloat64 lengh2 = dgFloat64 (0.0f);
			dgEdge* ptr = face;
			do {
				dgBigVector dir1 (m_points[ptr->m_next->m_incidentVertex] - m_points[ptr->m_incidentVertex]);
				dgFloat64 val = dir1 % dir1;
				if (val > lengh2) {
					lengh2 = val;
					dir = dir1;
				}
				ptr = ptr->m_next;
			} while (ptr != face);

			_ASSERTE (lengh2 > dgFloat32 (0.0f));

			dgEdge* lastEdge = NULL;
			dgEdge* firstEdge = NULL;
			dgFloat64 minVal = dgFloat64 (-1.0e10f);
			dgFloat64 maxVal = dgFloat64 (-1.0e10f);
			ptr = face;
			do {
				const dgBigVector& p = m_points[ptr->m_incidentVertex];
				dgFloat64 val = p % dir;
				if (val > maxVal) {
					maxVal = val;
					lastEdge = ptr;
				}
				val *= dgFloat64 (-1.0f);
				if (val > minVal) {
					minVal = val;
					firstEdge = ptr;
				}

				ptr->m_mark = mark;
				ptr = ptr->m_next;
			} while (ptr != face);

			_ASSERTE (firstEdge);
			_ASSERTE (lastEdge);

			bool isTJoint = true;
			dgBigVector p0 (m_points[firstEdge->m_incidentVertex]);
			dgBigVector p1 (m_points[lastEdge->m_incidentVertex]);
			dgBigVector p1p0 (p1 - p0);
			dgFloat64 den = p1p0 % p1p0;
			ptr = firstEdge->m_next;
			do {
				dgBigVector p2 (m_points[ptr->m_incidentVertex]);
				dgFloat64 num = (p2 - p0) % p1p0;
				dgBigVector q (p0 + p1p0.Scale (num / den));
				dgBigVector dist (p2 - q);
				dgFloat64 err2 = dist % dist;
				isTJoint &= (err2 < (dgFloat64 (1.0e-4f) * dgFloat64 (1.0e-4f)));
				ptr = ptr->m_next;
			} while (isTJoint && (ptr != firstEdge));

			if (isTJoint) {
				do {
					dgEdge* next = NULL;

					const dgBigVector p0 = m_points[firstEdge->m_incidentVertex];
					const dgBigVector p1 = m_points[firstEdge->m_next->m_incidentVertex];
					const dgBigVector p2 = m_points[firstEdge->m_prev->m_incidentVertex];

					dgBigVector p1p0 (p1 - p0);
					dgBigVector p2p0 (p2 - p0);
					dgFloat64 dist10 = p1p0 % p1p0;
					dgFloat64 dist20 = p2p0 % p2p0;

					dgEdge* begin = NULL;
					dgEdge* last = NULL;
					if (dist20 > dist10) {
						dgFloat64 t = (p1p0 % p2p0) / dist20;
						_ASSERTE (t > dgFloat32 (0.0f));
						_ASSERTE (t < dgFloat32 (1.0f));

						if (firstEdge->m_next->m_next->m_next != firstEdge) {
							ConectVertex (firstEdge->m_prev, firstEdge->m_next);
							next = firstEdge->m_next->m_twin->m_next;
						}
						_ASSERTE (firstEdge->m_next->m_next->m_next == firstEdge);

#ifdef _DEBUG
						dgEdge* tmp = firstEdge->m_twin;
						do {
							_ASSERTE (tmp->m_incidentFace > 0);
						} while (tmp != firstEdge->m_twin); 
#endif

						begin = firstEdge->m_next;
						last = firstEdge;
						firstEdge->m_userData = firstEdge->m_prev->m_twin->m_userData;
						firstEdge->m_incidentFace = firstEdge->m_prev->m_twin->m_incidentFace;
						dgVertexAtribute attrib (InterpolateEdge (firstEdge->m_prev->m_twin, t));
						attrib.m_vertex = m_points[firstEdge->m_next->m_incidentVertex];
						AddAtribute(attrib);
						firstEdge->m_next->m_incidentFace = firstEdge->m_prev->m_twin->m_incidentFace;
						firstEdge->m_next->m_userData = dgUnsigned64 (m_atribCount - 1);

						bool restart = false;
						if ((firstEdge->m_prev == &(*iter)) || (firstEdge->m_prev->m_twin == &(*iter))) {
							restart = true;
						}
						DeleteEdge(firstEdge->m_prev);
						if (restart) {
							iter.Begin();
						}

					} else {

						_ASSERTE (dist20 < dist10);

						dgFloat64 t = (p1p0 % p2p0) / dist10;
						_ASSERTE (t > dgFloat32 (0.0f));
						_ASSERTE (t < dgFloat32 (1.0f));

						if (firstEdge->m_next->m_next->m_next != firstEdge) {
							ConectVertex (firstEdge->m_next, firstEdge->m_prev);
							next = firstEdge->m_next->m_twin;
						}
						_ASSERTE (firstEdge->m_next->m_next->m_next == firstEdge);

#ifdef _DEBUG
						dgEdge* tmp = firstEdge->m_twin;
						do {
							_ASSERTE (tmp->m_incidentFace > 0);
						} while (tmp != firstEdge->m_twin); 
#endif

						begin = firstEdge->m_prev;
						last = firstEdge->m_next;
						firstEdge->m_next->m_userData = firstEdge->m_twin->m_userData;
						firstEdge->m_next->m_incidentFace = firstEdge->m_twin->m_incidentFace;
						dgVertexAtribute attrib (InterpolateEdge (firstEdge->m_twin, dgFloat64 (1.0f) - t));
						attrib.m_vertex = m_points[firstEdge->m_prev->m_incidentVertex];
						AddAtribute(attrib);
						firstEdge->m_prev->m_incidentFace = firstEdge->m_twin->m_incidentFace;
						firstEdge->m_prev->m_userData = dgUnsigned64 (m_atribCount - 1);

						bool restart = false;
						if ((firstEdge == &(*iter)) || (firstEdge->m_twin == &(*iter))) {
							restart = true;
						}
						DeleteEdge(firstEdge);
						if (restart) {
							iter.Begin();
						}
					}

					_ASSERTE (begin);
					_ASSERTE (last);
					for (dgEdge* ptr = begin->m_next->m_next; ptr != last; ptr = ptr->m_next) {
						dgEdge* const e = AddHalfEdge (begin->m_incidentVertex, ptr->m_incidentVertex);
						dgEdge* const t = AddHalfEdge (ptr->m_incidentVertex, begin->m_incidentVertex);
						if (e && t) {
							_ASSERTE (e);
							_ASSERTE (t);
							e->m_twin = t;
							t->m_twin = e;

							e->m_incidentFace = ptr->m_incidentFace;
							t->m_incidentFace = ptr->m_incidentFace;

							e->m_userData = last->m_next->m_userData;
							t->m_userData = ptr->m_userData;

							t->m_prev = ptr->m_prev;
							ptr->m_prev->m_next = t;
							e->m_next = ptr;
							ptr->m_prev = e;
							t->m_next = last->m_next;
							e->m_prev = last;
							last->m_next->m_prev = t;
							last->m_next = e;
						}
					}

					firstEdge = next;
				} while (firstEdge);
			}
		}
	}

	DeleteDegenerateFaces(&m_points[0].m_x, sizeof (m_points[0]), dgFloat64 (1.0e-7f));
}
