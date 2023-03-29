#pragma once

#include "PhysicsEngine.h"
#include <iostream>
#include <iomanip>

namespace PhysicsEngine
{
	///Plane class
	class Plane : public StaticActor
	{
	public:
		//A plane with default paramters: XZ plane centred at (0,0,0)
		Plane(PxVec3 normal=PxVec3(0.f, 1.f, 0.f), PxReal distance=0.f) 
			: StaticActor(PxTransformFromPlaneEquation(PxPlane(normal, distance)))
		{
			CreateShape(PxPlaneGeometry());
		}
	};

	///Sphere class
	class Sphere : public DynamicActor
	{
	public:
		//a sphere with default parameters:
		// - pose in 0,0,0
		// - dimensions: 1m
		// - density: 1kg/m^3
		Sphere(const PxTransform& pose=PxTransform(PxIdentity), PxReal radius=1.f, PxReal density=1.f) 
			: DynamicActor(pose)
		{ 
			CreateShape(PxSphereGeometry(radius), density);
		}
	};

	///Box class
	class Box : public DynamicActor
	{
	public:
		//a Box with default parameters:
		// - pose in 0,0,0
		// - dimensions: 1m x 1m x 1m
		// - denisty: 1kg/m^3
		Box(const PxTransform& pose=PxTransform(PxIdentity), PxVec3 dimensions=PxVec3(.5f, .5f, .5f), PxReal density=1.f) 
			: DynamicActor(pose)
		{ 
			CreateShape(PxBoxGeometry(dimensions), density);
		}
	};

	class Capsule : public DynamicActor
	{
	public:
		Capsule(const PxTransform& pose=PxTransform(PxIdentity), PxVec2 dimensions=PxVec2(1.f,1.f), PxReal density=1.f) 
			: DynamicActor(pose)
		{
			CreateShape(PxCapsuleGeometry(dimensions.x, dimensions.y), density);
		}
	};

	///The ConvexMesh class
	class ConvexMesh : public DynamicActor
	{
	public:
		//constructor
		ConvexMesh(const std::vector<PxVec3>& verts, const PxTransform& pose=PxTransform(PxIdentity), PxReal density=1.f)
			: DynamicActor(pose)
		{
			PxConvexMeshDesc mesh_desc;
			mesh_desc.points.count = (PxU32)verts.size();
			mesh_desc.points.stride = sizeof(PxVec3);
			mesh_desc.points.data = &verts.front();
			mesh_desc.flags = PxConvexFlag::eCOMPUTE_CONVEX;
			mesh_desc.vertexLimit = 256;

			CreateShape(PxConvexMeshGeometry(CookMesh(mesh_desc)), density);
		}

		//mesh cooking (preparation)
		PxConvexMesh* CookMesh(const PxConvexMeshDesc& mesh_desc)
		{
			PxDefaultMemoryOutputStream stream;

			if(!GetCooking()->cookConvexMesh(mesh_desc, stream))
				throw new Exception("ConvexMesh::CookMesh, cooking failed.");

			PxDefaultMemoryInputData input(stream.getData(), stream.getSize());

			return GetPhysics()->createConvexMesh(input);
		}
	};

	class CompoundObject : public DynamicActor 
	{
	public:

		CompoundObject(const PxTransform& pose = PxTransform(PxIdentity), PxVec3 dimensions = PxVec3(.5f, .5f, .5f), PxReal density = 1.f)
			: DynamicActor(pose)
		{
			for (int i = 0; i < 2; i++) 
			{
				CreateShape(PxBoxGeometry(dimensions), density);
				//color_palette[i];
			}
			GetShape(0)->setLocalPose(PxTransform(PxVec3(0.0f, 1.0f, 0.0f)));
			GetShape(1)->setLocalPose(PxTransform(PxVec3(2.0f, 1.0f, 0.0f)));
		}
	};

	class GoalPost : public StaticActor
	{
	public:
		//rugby union goal posts are 13m meters high and 5.6m across
		GoalPost(const PxTransform& pose = PxTransform(PxIdentity), PxVec3 dimensions = PxVec3(0.5f, 13.f, 0.5f), PxReal density = 1.f)
			: StaticActor(pose)
		{
			for (int i = 0; i < 2; i++)
			{
				CreateShape(PxBoxGeometry(dimensions), density);
			}
			//distance between goal posts is 5.6 meters (-2.3f and 2.3f)
			//goal posts are raised by 13f on Y axis to sit on pitch
			//they are placed on the goal line on the Z axis
			GetShape(0)->setLocalPose(PxTransform(PxVec3(-2.3f, 13.f, -71.42857145f)));
			GetShape(1)->setLocalPose(PxTransform(PxVec3(2.3f, 13.f, -71.42857145f)));
		}
	};

	class GoalCrossbar : public StaticActor
	{
	public:

		GoalCrossbar(const PxTransform& pose = PxTransform(PxIdentity), PxVec3 dimensions = PxVec3(2.5f, 0.5f, 0.5f), PxReal density = 1.f)
			: StaticActor(pose)
		{
			for (int i = 0; i < 1; i++)
			{
				CreateShape(PxBoxGeometry(dimensions), density);
			}
			GetShape(0)->setLocalPose(PxTransform(PxVec3(0.0f, 3.0f, -71.42857145f)));
		}
	};

	class InnerPitchLines : public StaticActor 
	{
		//creating a rugby union pitch, this class forms the inner lines
	public:
		InnerPitchLines(const PxTransform& pose = PxTransform(PxIdentity), PxVec3 dimensions = PxVec3(70.0f, 0.5f, 0.5f), PxReal density = 1.f)
			: StaticActor(pose) 
		{
			for (int i = 0; i < 7; i++) 
			{
				CreateShape(PxBoxGeometry(dimensions), density);
			}
			GetShape(0)->setLocalPose(PxTransform(PxVec3(0.0f, -0.49f, 0.0f)));
			GetShape(1)->setLocalPose(PxTransform(PxVec3(0.0f, -0.49f, -14.28571429f)));
			GetShape(2)->setLocalPose(PxTransform(PxVec3(0.0f, -0.49f, -28.57142858f)));
			//GetShape(3)->setLocalPose(PxTransform(PxVec3(0.0f, 0.0f, -42.85714287f)));
			//GetShape(3) is the half way line
			GetShape(3)->setLocalPose(PxTransform(PxVec3(0.0f, -0.49f, -42.85714287f)));
			GetShape(4)->setLocalPose(PxTransform(PxVec3(0.0f, -0.49f, -57.14285716f)));
			GetShape(5)->setLocalPose(PxTransform(PxVec3(0.0f, -0.49f, -71.42857145f)));
			GetShape(6)->setLocalPose(PxTransform(PxVec3(0.0f, -0.49f, -85.71428574f)));
		}
	};

	class OuterPitchLines : public StaticActor 
	{
		//creating a rugby union pitch, this class forms the outer lines
	public:
		OuterPitchLines(const PxTransform& pose = PxTransform(PxIdentity), PxVec3 dimensions = PxVec3(0.5f, 0.5f, 45.f), PxReal density = 1.f)
			: StaticActor(pose)
		{
			for (int i = 0; i < 2; i++)
			{
				CreateShape(PxBoxGeometry(dimensions), density);
			}
			GetShape(0)->setLocalPose(PxTransform(PxVec3(70.0f, -0.49f, -40.0f)));
			GetShape(1)->setLocalPose(PxTransform(PxVec3(-70.0f, -0.49f, -40.f)));
		}
	};

	//compound shape class of a rugby ball
	class RugbyBall : public DynamicActor 
	{
	public:
		//Average mass of rugby ball: 435g
		//Average volume of rugby ball : 4800 cubmic cm or 0.0148 cubic meters
		//Density: 0.46kg / 0.0148 = 31.0810810811
		RugbyBall(const PxTransform& pose = PxTransform(PxIdentity), PxReal radius = 0.1f, PxReal density = 1.f) //copied same constructor from sphere class
			: DynamicActor(pose)
		{
			for (int i = 0; i < 5; i++) //creates 5 spheres
			{
				if (i == 0) //sets first sphere with radius of 0.75f
				{
					radius = 0.40f;
				}
				else if (i > 0 and i < 3) //sets spheres 2 and 3 with radius of 0.5f
				{
					radius = 0.30f;
				}
				else 
				{
					radius = 0.20f;
				}
				CreateShape(PxSphereGeometry(radius), density);
			}
			GetShape(0)->setLocalPose((PxTransform(PxVec3(0.0f, 0.0f, 0.0f))));
			GetShape(1)->setLocalPose((PxTransform(PxVec3(0.20f, 0.0f, 0.0f))));
			GetShape(2)->setLocalPose((PxTransform(PxVec3(-0.20f, 0.0f, 0.0f))));
			GetShape(3)->setLocalPose((PxTransform(PxVec3(0.4f, 0.0f, 0.0f))));
			GetShape(4)->setLocalPose((PxTransform(PxVec3(-0.4f, 0.0f, 0.0f))));
		}
	};

	class BallCatapult : public DynamicActor 
	{
	public:

		BallCatapult(const PxTransform& pose = PxTransform(PxIdentity), PxReal density = PxReal(1.0f))
			: DynamicActor(pose) 
		{
			CreateShape(PxBoxGeometry(PxVec3(0.5f, 0.1f, 3.0f)), density);
			CreateShape(PxBoxGeometry(PxVec3(1.0f, 0.1f, 1.0f)), density);

			GetShape(1)->setLocalPose(PxTransform(PxVec3(0.0f, 0.0f, 3.5f)));

			//angle barrier
			CreateShape(PxBoxGeometry(PxVec3(1.0f, 0.75f, 0.1f)), density);
			GetShape(2)->setLocalPose(PxTransform(PxVec3(0.0f, 0.5f, 5.0f), PxQuat(PxPi / 4, PxVec3(1.0f, 0.0f, 0.0f))));
		}
	};

	class Pitchfork : public DynamicActor 
	{
	public:

		Pitchfork(const PxTransform& pose = PxTransform(PxIdentity), PxVec3 dimensions = PxVec3(.5f, .5f, .5f), PxReal density = 1.f)
		: DynamicActor(pose)
		{
			for (int i = 0; i < 6; i++) 
			{
				if (i == 0) 
				{
					dimensions = PxVec3(0.15f, 0.2f, 2.f);
				}
				else if (i == 1) 
				{
					dimensions = PxVec3(0.75f, 0.2f, 0.1f);
				}
				else
				{
					dimensions = PxVec3(0.1f, 0.2f, 0.5f);
				}
				CreateShape(PxBoxGeometry(dimensions), density);
			}
			GetShape(1)->setLocalPose(PxTransform(PxVec3(0.f, 0.f, -2.f)));
			GetShape(2)->setLocalPose(PxTransform(PxVec3(0.75f, 0.f, -2.5f)));
			GetShape(3)->setLocalPose(PxTransform(PxVec3(-0.75f, 0.f, -2.5f)));
			GetShape(4)->setLocalPose(PxTransform(PxVec3(0.25f, 0.f, -2.5f)));
			GetShape(5)->setLocalPose(PxTransform(PxVec3(-0.25f, 0.f, -2.5f)));
		}
	};

	class Truncheon : public DynamicActor 
	{
	public:
		Truncheon(const PxTransform& pose = PxTransform(PxIdentity), PxVec3 dimensions = PxVec3(.5f, .5f, .5f), PxReal density = 1.f)
			: DynamicActor(pose) 
		{
			for (int i = 0; i < 2; i++)
			{
				if (i == 0)
				{
					dimensions = PxVec3(0.2f, 0.5f, 0.2f);
				}
				else 
				{
					dimensions = PxVec3(0.25f, 0.5f, 0.25f);
				}
				CreateShape(PxBoxGeometry(dimensions), density);
			}
			GetShape(1)->setLocalPose(PxTransform(PxVec3(0.f, -1.f, 0.f)));

		}
	};

	class SwingPost : public StaticActor
	{
	public:
		//rugby union goal posts are 13m meters high and 5.6m across
		SwingPost(const PxTransform& pose = PxTransform(PxIdentity), PxVec3 dimensions = PxVec3(0.5f, 4.f, 0.5f), PxReal density = 1.f)
			: StaticActor(pose)
		{
			for (int i = 0; i < 2; i++)
			{
				CreateShape(PxBoxGeometry(dimensions), density);
			}
			//distance between goal posts is 5.6 meters (-2.3f and 2.3f)
			//goal posts are raised by 13f on Y axis to sit on pitch
			//they are placed on the goal line on the Z axis
			GetShape(0)->setLocalPose(PxTransform(PxVec3(-3.f, 4.f, 0.f)));
			GetShape(1)->setLocalPose(PxTransform(PxVec3(3.f, 4.f, 0.f)));
		}
	};

	class SwingTopBar : public StaticActor
	{
	public:

		SwingTopBar(const PxTransform& pose = PxTransform(PxIdentity), PxVec3 dimensions = PxVec3(2.5f, 0.5f, 0.5f), PxReal density = 1.f)
			: StaticActor(pose)
		{
			CreateShape(PxBoxGeometry(dimensions), density);

			GetShape(0)->setLocalPose(PxTransform(PxVec3(0.0f, 7.5f, 0.f)));
		}
	};

	class InnerBarrierLines : public StaticActor 
	{
		//creating a rugby union pitch, this class forms the inner lines
	public:
		InnerBarrierLines(const PxTransform& pose = PxTransform(PxIdentity), PxVec3 dimensions = PxVec3(71.f, 25.f, 2.f), PxReal density = 1.f)
			: StaticActor(pose) 
		{
			for (int i = 0; i < 2; i++) 
			{
				CreateShape(PxBoxGeometry(dimensions), density);
			}
			GetShape(0)->setLocalPose(PxTransform(PxVec3(0.0f, 25.f, 20.0f)));
			GetShape(1)->setLocalPose(PxTransform(PxVec3(0.0f, 25.f, -90.71428574f)));
		}
	};

	class OuterBarrierLines : public StaticActor 
	{
		//creating a rugby union pitch, this class forms the outer lines
	public:
		OuterBarrierLines(const PxTransform& pose = PxTransform(PxIdentity), PxVec3 dimensions = PxVec3(0.5f, 25.f, 55.f), PxReal density = 1.f)
			: StaticActor(pose)
		{
			for (int i = 0; i < 2; i++)
			{
				CreateShape(PxBoxGeometry(dimensions), density);
			}
			GetShape(0)->setLocalPose(PxTransform(PxVec3(71.0f, 25.f, -34.0f)));
			GetShape(1)->setLocalPose(PxTransform(PxVec3(-71.0f, 25.f, -34.f)));
		}
	};

	class Knights : public DynamicActor
	{
	public:
		//a Box with default parameters:
		// - pose in 0,0,0
		// - dimensions: 1m x 1m x 1m
		// - denisty: 1kg/m^3
		Knights(const PxTransform& pose = PxTransform(PxIdentity), PxVec3 dimensions = PxVec3(0.5f, 2.f, 0.5f), PxReal density = 1.f)
			: DynamicActor(pose)
		{
			for (int i = 0; i < 10; i++) 
			{
				CreateShape(PxBoxGeometry(dimensions), density);
			}
			GetShape(0)->setLocalPose(PxTransform(PxVec3(0, 1.f, 0.f)));
			GetShape(1)->setLocalPose(PxTransform(PxVec3(-2, 1.f, 0.f)));
			GetShape(2)->setLocalPose(PxTransform(PxVec3(-4, 1.f, 0.f)));
			GetShape(3)->setLocalPose(PxTransform(PxVec3(-6, 1.f, 0.f)));
			GetShape(4)->setLocalPose(PxTransform(PxVec3(-8, 1.f, 0.f)));
			GetShape(5)->setLocalPose(PxTransform(PxVec3(2.f, 1.f, 0.f)));
			GetShape(6)->setLocalPose(PxTransform(PxVec3(4.f, 1.f, 0.f)));
			GetShape(7)->setLocalPose(PxTransform(PxVec3(6.f, 1.f, 0.f)));
			GetShape(8)->setLocalPose(PxTransform(PxVec3(8.f, 1.f, 0.f)));
			GetShape(9)->setLocalPose(PxTransform(PxVec3(10.f, 1.f, 0.f)));
		}
	};

	///The TriangleMesh class
	class TriangleMesh : public StaticActor
	{
	public:
		//constructor
		TriangleMesh(const std::vector<PxVec3>& verts, const std::vector<PxU32>& trigs, const PxTransform& pose=PxTransform(PxIdentity))
			: StaticActor(pose)
		{
			PxTriangleMeshDesc mesh_desc;
			mesh_desc.points.count = (PxU32)verts.size();
			mesh_desc.points.stride = sizeof(PxVec3);
			mesh_desc.points.data = &verts.front();
			mesh_desc.triangles.count = (PxU32)trigs.size()/3;
			mesh_desc.triangles.stride = 3*sizeof(PxU32);
			mesh_desc.triangles.data = &trigs.front();

			CreateShape(PxTriangleMeshGeometry(CookMesh(mesh_desc)));
		}

		//mesh cooking (preparation)
		PxTriangleMesh* CookMesh(const PxTriangleMeshDesc& mesh_desc)
		{
			PxDefaultMemoryOutputStream stream;

			if(!GetCooking()->cookTriangleMesh(mesh_desc, stream))
				throw new Exception("TriangleMesh::CookMesh, cooking failed.");

			PxDefaultMemoryInputData input(stream.getData(), stream.getSize());

			return GetPhysics()->createTriangleMesh(input);
		}
	};

	//Distance joint with the springs switched on
	class DistanceJoint : public Joint
	{
	public:
		DistanceJoint(Actor* actor0, const PxTransform& localFrame0, Actor* actor1, const PxTransform& localFrame1)
		{
			PxRigidActor* px_actor0 = 0;
			if (actor0)
				px_actor0 = (PxRigidActor*)actor0->Get();

			joint = (PxJoint*)PxDistanceJointCreate(*GetPhysics(), px_actor0, localFrame0, (PxRigidActor*)actor1->Get(), localFrame1);
			joint->setConstraintFlag(PxConstraintFlag::eVISUALIZATION, true);
			((PxDistanceJoint*)joint)->setDistanceJointFlag(PxDistanceJointFlag::eSPRING_ENABLED, true);
			Damping(1.f);
			Stiffness(1.f);
		}

		void Stiffness(PxReal value)
		{
			((PxDistanceJoint*)joint)->setStiffness(value);
		}

		PxReal Stiffness()
		{
			return ((PxDistanceJoint*)joint)->getStiffness();
		}

		void Damping(PxReal value)
		{
			((PxDistanceJoint*)joint)->setDamping(value);
		}

		PxReal Damping()
		{
			return ((PxDistanceJoint*)joint)->getDamping();
		}
	};

	///Revolute Joint
	class RevoluteJoint : public Joint
	{
	public:
		RevoluteJoint(Actor* actor0, const PxTransform& localFrame0, Actor* actor1, const PxTransform& localFrame1)
		{
			PxRigidActor* px_actor0 = 0;
			if (actor0)
				px_actor0 = (PxRigidActor*)actor0->Get();

			joint = PxRevoluteJointCreate(*GetPhysics(), px_actor0, localFrame0, (PxRigidActor*)actor1->Get(), localFrame1);
			joint->setConstraintFlag(PxConstraintFlag::eVISUALIZATION, true);
		}

		void DriveVelocity(PxReal value)
		{
			//wake up the attached actors
			PxRigidDynamic* actor_0, * actor_1;
			((PxRevoluteJoint*)joint)->getActors((PxRigidActor*&)actor_0, (PxRigidActor*&)actor_1);
			/*if (actor_0)
			{
				if (actor_0->isSleeping())
					actor_0->wakeUp();
			}*/
			if (actor_1)
			{
				if (actor_1->isSleeping())
					actor_1->wakeUp();
			}
			((PxRevoluteJoint*)joint)->setDriveVelocity(value);
			((PxRevoluteJoint*)joint)->setRevoluteJointFlag(PxRevoluteJointFlag::eDRIVE_ENABLED, true);
		}

		PxReal DriveVelocity()
		{
			return ((PxRevoluteJoint*)joint)->getDriveVelocity();
		}

		void SetLimits(PxReal lower, PxReal upper)
		{
			((PxRevoluteJoint*)joint)->setLimit(PxJointAngularLimitPair(lower, upper));
			((PxRevoluteJoint*)joint)->setRevoluteJointFlag(PxRevoluteJointFlag::eLIMIT_ENABLED, true);
		}
	};
}