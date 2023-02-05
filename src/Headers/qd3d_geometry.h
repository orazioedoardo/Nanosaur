//
// qd3d_geometry.h
//

#include "qd3d_support.h"


enum
{
	SHARD_MODE_BOUNCE = (1),
	SHARD_MODE_UPTHRUST = (1<<1),
	SHARD_MODE_HEAVYGRAVITY = (1<<2)
};


#define	MAX_SHARDS			1500

typedef struct
{
	TQ3Vector3D				rot,rotDelta;
	TQ3Point3D				coord,coordDelta;
	float					decaySpeed,scale;
	Byte					mode;
	TQ3Matrix4x4			matrix;
	TQ3TriMeshData			*mesh;
}ShardType;

float QD3D_CalcObjectRadius(int numMeshes, TQ3TriMeshData** meshList);
void QD3D_CalcObjectBoundingBox(int numMeshes, TQ3TriMeshData** meshList, TQ3BoundingBox* boundingBox);
void QD3D_ExplodeGeometry(ObjNode *theNode, float boomForce, Byte shardMode, int shardDensity, float shardDecaySpeed);
void QD3D_ScrollUVs(int numMeshes, TQ3TriMeshData** meshList, float rawDeltaU, float rawDeltaV);
void QD3D_InitShards(void);
void QD3D_DisposeShards(void);
void QD3D_MoveShards(void);
void QD3D_DrawShards(QD3DSetupOutputType *setupInfo);
