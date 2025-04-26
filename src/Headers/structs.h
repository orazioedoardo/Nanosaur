//
// structs.h
//

#pragma once


#include <QD3D.h>
#include "globals.h"
#include "renderer.h"
#include "input.h"

#define	MAX_ANIMS			20
#define	MAX_KEYFRAMES		15
#define	MAX_JOINTS			20
#define	MAX_CHILDREN		(MAX_JOINTS-1)
#define	MAX_LIMBS			MAX_JOINTS

#define MAX_FLAGS_IN_OBJNODE			4		// # flags in ObjNode
#define MAX_BOXES_PER_OBJNODE			1		// max collision boxes per objnode


#define	MAX_DECOMPOSED_POINTS	1200
#define	MAX_DECOMPOSED_NORMALS	800
#define	MAX_POINT_REFS			10		// max times a point can be re-used in multiple places
#define	MAX_DECOMPOSED_TRIMESHES 10



		/* COLLISION BOX */
		
typedef struct
{
	long	left,right,front,back,top,bottom;
	long	oldLeft,oldRight,oldFront,oldBack,oldTop,oldBottom;
}CollisionBoxType;


		/****************************/
		/* POLYGONAL COLLISION INFO */
		/****************************/
		
typedef struct
{
	TQ3Point3D			verts[3];			// coords of each vertex
	TQ3PlaneEquation	planeEQ;			// plane equation of triangle
}CollisionTriangleType;

typedef struct
{
	TQ3BoundingBox			bBox;				// bounding box of these triangles
	short					numTriangles;		// # triangles in list
	CollisionTriangleType	*triangles;			// ptr to list of collision triangles
}TriangleCollisionList;


//*************************** SKELETON *****************************************/

		/* BONE SPECIFICATIONS */
		//
		//
		// NOTE: Similar to joint definition but lacks animation, rot/scale info.
		//
		
typedef struct
{
	int					parentBone;						// index to previous bone
	TQ3Point3D			coord;							// absolute coord (not relative to parent!) 
	UInt16				numPointsAttachedToBone;		// # vertices/points that this bone has
	UInt16				*pointList;						// indecies into gDecomposedPointList
	UInt16				numNormalsAttachedToBone;		// # vertex normals this bone has
	UInt16				*normalList;					// indecies into gDecomposedNormalsList
}BoneDefinitionType;


			/* DECOMPOSED POINT INFO */
			
typedef struct
{
	TQ3Point3D	realPoint;							// point coords as imported in 3DMF model
	TQ3Point3D	boneRelPoint;						// point relative to bone coords (offset from bone)
	
	Byte		numRefs;							// # of places this point is used in the geometry data
	Byte		whichTriMesh[MAX_POINT_REFS];		// index to trimeshes
	short		whichPoint[MAX_POINT_REFS];			// index into pointlist of triMesh above
	short		whichNormal[MAX_POINT_REFS];		// index into gDecomposedNormalsList
}DecomposedPointType;



		/* CURRENT JOINT STATE */
		
typedef struct
{
	SInt32		tick;					// time at which this state exists
	SInt32		accelerationMode;		// mode of in/out acceleration
	TQ3Point3D	coord;					// current 3D coords of joint (relative to link)
	TQ3Vector3D	rotation;				// current rotation values of joint (relative to link)
	TQ3Vector3D	scale;					// current scale values of joint mesh
}JointKeyframeType;


		/* JOINT DEFINITIONS */
		
typedef struct
{
	signed char			numKeyFrames[MAX_ANIMS];				// # keyframes
	JointKeyframeType 	**keyFrames;							// 2D array of keyframe data keyFrames[anim#][keyframe#]
}JointKeyFrameHeader;

			/* ANIM EVENT TYPE */
			
typedef struct
{
	short	time;
	Byte	type;
	Byte	value;
}AnimEventType;


			/* SKELETON INFO */
		
typedef struct
{
	Byte				NumBones;						// # joints in this skeleton object
	JointKeyFrameHeader	JointKeyframes[MAX_JOINTS];		// array of joint definitions

	Byte				numChildren[MAX_JOINTS];		// # children each joint has
	Byte				childIndecies[MAX_JOINTS][MAX_CHILDREN];	// index to each child

	Byte				NumAnims;						// # animations in this skeleton object
	Byte				*NumAnimEvents;					// ptr to array containing the # of animevents for each anim
	AnimEventType		**AnimEventsList;				// 2 dimensional array which holds a anim event list for each anim AnimEventsList[anim#][event#]

	BoneDefinitionType	*Bones;							// data which describes bone heirarachy

	int					numDecomposedTriMeshes;			// # trimeshes in skeleton
	TQ3TriMeshData		**decomposedTriMeshPtrs;		// array of triMeshData

	int					numDecomposedPoints;			// # shared points in skeleton
	DecomposedPointType	*decomposedPointList;			// array of shared points

	int					numDecomposedNormals;			// # shared normal vectors
	TQ3Vector3D			*decomposedNormalsList;			// array of shared normals

	TQ3MetaFile			*associated3DMF;					// associated 3DMF file

	int					numTextures;
	GLuint				*textureNames;
}SkeletonDefType;


		/* THE STRUCTURE ATTACHED TO AN OBJNODE */
		//
		// This contains all of the local skeleton data for a particular ObjNode
		//

typedef struct
{
	Byte			AnimNum;						// animation #

	Boolean			IsMorphing;						// flag set when morphing from an anim to another
	float			MorphSpeed;						// speed of morphing (1.0 = normal)
	float			MorphPercent;					// percentage of morph from kf1 to kf2 (0.0 - 1.0)

	JointKeyframeType	JointCurrentPosition[MAX_JOINTS];	// for each joint, holds current interpolated keyframe values
	JointKeyframeType	MorphStart[MAX_JOINTS];		// morph start & end keyframes for each joint
	JointKeyframeType	MorphEnd[MAX_JOINTS];

	float			CurrentAnimTime;				// current time index for animation	
	float			LoopBackTime;					// time to loop or zigzag back to (default = 0 unless set by a setmarker)
	float			MaxAnimTime;					// duration of current anim
	float			AnimSpeed;						// time factor for speed of executing current anim (1.0 = normal time)
	Byte			AnimEventIndex;					// current index into anim event list
	Byte			AnimDirection;					// if going forward in timeline or backward			
	Byte			EndMode;						// what to do when reach end of animation
	Boolean			AnimHasStopped;					// flag gets set when anim has reached end of sequence (looping anims don't set this!)

	TQ3Matrix4x4	jointTransformMatrix[MAX_JOINTS];	// holds matrix xform for each joint

	const SkeletonDefType	*skeletonDefinition;	// point to skeleton's common/shared data
}SkeletonObjDataType;


			/* TERRAIN ITEM ENTRY TYPE */

struct TerrainItemEntryType
{
	UInt16	x,y;
	UInt16	type;
	Byte	parm[4];
	
	UInt16	flags;							

	// Source port fix: changed from pointers to 32-bit longs for 64-bit compatibility.
	// These "pointers" were loaded from Level1.ter -- they're actually just stored as zeores
	// in the file and the pointers are properly set in BuildTerrainItemList.
	SInt32	prevItemIdx;	// index of previous item in linked list (-1 == none)
	SInt32	nextItemIdx;	// index of next item in linked list (-1 == none)
};
typedef struct TerrainItemEntryType TerrainItemEntryType;




			/****************************/
			/*  OBJECT RECORD STRUCTURE */
			/****************************/

struct ObjNode
{
	struct ObjNode	*PrevNode;			// address of previous node in linked list
	struct ObjNode	*NextNode;			// address of next node in linked list
	struct ObjNode	*ChainNode;
	struct ObjNode	*ChainHead;			// a chain's head (link back to 1st obj in chain)

	short			Slot;				// sort value
	Byte			Genre;				// obj genre
	Byte			Type;				// obj type
	Byte			Group;				// obj group
	void			(*MoveCall)(struct ObjNode *);	// pointer to object's move routine
	TQ3Point3D		Coord;				// coord of object
	TQ3Point3D		OldCoord;			// coord @ previous frame
	TQ3Vector3D		Delta;				// delta velocity of object
	TQ3Vector3D		Rot;				// rotation of object
	TQ3Vector3D		RotDelta;			// rotation delta
	TQ3Vector3D		Scale;				// scale of object
	float			Speed;				// speed: sqrt(dx2 * dz2)
	float			Accel;				// current acceleration value
	TQ3Vector2D		TerrainAccel;		// force added by terrain slopes
	TQ3Point2D		TargetOff;			// target offsets
	uint32_t		CType;				// collision type bits
	uint32_t		CBits;				// collision attribute bits
	Byte			Kind;				// kind
	signed char		Flag[6];
	long			Special[6];
	float			SpecialF[6];
	struct ObjNode	*SpecialRef[6];		// source port addition for 64-bit compat
	float			Health;				// health 0..1
	float			Damage;				// damage
	
	uint32_t		StatusBits;			// various status bits
	
	struct	ObjNode	*ShadowNode;		// ptr to node's shadow (if any)
	struct	ObjNode	*PlatformNode;		// ptr to object which it on top of.
	struct	ObjNode	*CarriedObj;		// ptr to object being carried/pickedup
	
	Byte				NumCollisionBoxes;
	CollisionBoxType	CollisionBoxes[MAX_BOXES_PER_OBJNODE];				// array of collision boxes
	short			LeftOff,RightOff,FrontOff,BackOff,TopOff,BottomOff;		// box offsets (only used by simple objects with 1 collision box)
	
	TriangleCollisionList	*CollisionTriangles; // ptr to triangle collision data
	
	short				StreamingEffect;		// streaming effect (-1 = none)
	
	TQ3Matrix4x4		BaseTransformMatrix;	// matrix which contains all of the transforms for the object as a whole

	int						NumMeshes;
	TQ3TriMeshData*			MeshList[MAX_DECOMPOSED_TRIMESHES];
	bool					OwnsMeshTexture[MAX_DECOMPOSED_TRIMESHES];		// if true, DeleteObject will call glDeleteTextures on the corresponding mesh's texture (if any)
	bool					OwnsMeshMemory[MAX_DECOMPOSED_TRIMESHES];		// if true, DeleteObject will call Q3TriMeshData_Dispose on the corresponding mesh

	RenderModifiers			RenderModifiers;

	float				Radius;					// radius use for object culling calculation

	SkeletonObjDataType	*Skeleton;				// pointer to skeleton record data

	TerrainItemEntryType *TerrainItemPtr;		// if item was from terrain, then this pts to entry in array
};
typedef struct ObjNode ObjNode;


		/* NEW OBJECT DEFINITION TYPE */
		
typedef struct
{
	Byte		genre,group,type,animNum;
	TQ3Point3D	coord;
	uint32_t	flags;
	short		slot;
	void		(*moveCall)(ObjNode *);
	float		rot,scale;
}NewObjectDefinitionType;


		/* TIME PORTAL STRUCT */
		
typedef struct
{
	TQ3Point2D	coord;
}TimePortalType;


		/* PREFERENCES */
		
typedef struct
{
	char magic[32];
	Boolean	highQualityTextures;
	Boolean	canDoFog;
	Boolean	whiteSky;
	Boolean	shadows;
	Boolean	dust;
	Boolean fullscreen;
	Byte antialiasingLevel;
	Byte displayNumMinus1;
	Boolean mainMenuHelp;
	Boolean vsync;
	Boolean extreme;
	Boolean	music;
	Boolean	ambientSounds;
	Boolean	debugInfoInTitleBar;
	Boolean	nanosaurTeethFix;
	Boolean	force4x3;
	KeyBinding keys[NUM_CONTROL_NEEDS];
}PrefsType;

#define PREFS_MAGIC "Nanosaur Prefs v5"

