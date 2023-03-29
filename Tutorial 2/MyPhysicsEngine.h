#pragma once

#include "BasicActors.h"
#include <iostream>
#include <iomanip>

namespace PhysicsEngine
{
	using namespace std;

	//a list of colours: Circus Palette
	static const PxVec3 color_palette[] = { PxVec3(46.f / 255.f,9.f / 255.f,39.f / 255.f),PxVec3(217.f / 255.f,0.f / 255.f,0.f / 255.f),
		PxVec3(255.f / 255.f,45.f / 255.f,0.f / 255.f),PxVec3(255.f / 255.f,140.f / 255.f,54.f / 255.f),PxVec3(4.f / 255.f,117.f / 255.f,111.f / 255.f) };

	//pyramid vertices
	static PxVec3 pyramid_verts[] = { PxVec3(0,1,0), PxVec3(1,0,0), PxVec3(-1,0,0), PxVec3(0,0,1), PxVec3(0,0,-1) };
	//pyramid triangles: a list of three vertices for each triangle e.g. the first triangle consists of vertices 1, 4 and 0
	//vertices have to be specified in a counter-clockwise order to assure the correct shading in rendering
	static PxU32 pyramid_trigs[] = { 1, 4, 0, 3, 1, 0, 2, 3, 0, 4, 2, 0, 3, 2, 1, 2, 4, 1 };

	class Pyramid : public ConvexMesh
	{
	public:
		Pyramid(PxTransform pose = PxTransform(PxIdentity), PxReal density = 1.f) :
			ConvexMesh(vector<PxVec3>(begin(pyramid_verts), end(pyramid_verts)), pose, density)
		{
			//CreateShape(PxPlaneGeometry(pose, density));
		}
	};

	class PyramidStatic : public TriangleMesh
	{
	public:
		PyramidStatic(PxTransform pose = PxTransform(PxIdentity)) :
			TriangleMesh(vector<PxVec3>(begin(pyramid_verts), end(pyramid_verts)), vector<PxU32>(begin(pyramid_trigs), end(pyramid_trigs)), pose)
		{
		}
	};
	/*
	struct FilterGroup
	{
		enum Enum
		{
			ACTOR0 = (1 << 0),
			ACTOR1 = (1 << 1),
			ACTOR2 = (1 << 2)
			//add more if you need
		};
	};

	///An example class showing the use of springs (distance joints).
	/*class Trampoline
	{
		vector<DistanceJoint*> springs;
		Box* bottom, * top;

	public:
		Trampoline(const PxVec3& dimensions = PxVec3(1.f, 1.f, 1.f), PxReal stiffness = 1.f, PxReal damping = 1.f)
		{
			PxReal thickness = .1f;
			bottom = new Box(PxTransform(PxVec3(0.f, thickness, 0.f)), PxVec3(dimensions.x, thickness, dimensions.z));
			top = new Box(PxTransform(PxVec3(0.f, dimensions.y + thickness, 0.f)), PxVec3(dimensions.x, thickness, dimensions.z));
			springs.resize(4);
			springs[0] = new DistanceJoint(bottom, PxTransform(PxVec3(dimensions.x, thickness, dimensions.z)), top, PxTransform(PxVec3(dimensions.x, -dimensions.y, dimensions.z)));
			springs[1] = new DistanceJoint(bottom, PxTransform(PxVec3(dimensions.x, thickness, -dimensions.z)), top, PxTransform(PxVec3(dimensions.x, -dimensions.y, -dimensions.z)));
			springs[2] = new DistanceJoint(bottom, PxTransform(PxVec3(-dimensions.x, thickness, dimensions.z)), top, PxTransform(PxVec3(-dimensions.x, -dimensions.y, dimensions.z)));
			springs[3] = new DistanceJoint(bottom, PxTransform(PxVec3(-dimensions.x, thickness, -dimensions.z)), top, PxTransform(PxVec3(-dimensions.x, -dimensions.y, -dimensions.z)));

			for (unsigned int i = 0; i < springs.size(); i++)
			{
				springs[i]->Stiffness(stiffness);
				springs[i]->Damping(damping);
			}
		}

		void AddToScene(Scene* scene)
		{
			scene->Add(bottom);
			scene->Add(top);
		}

		~Trampoline()
		{
			for (unsigned int i = 0; i < springs.size(); i++)
				delete springs[i];
		}
	};

	///A customised collision class, implemneting various callbacks
	class MySimulationEventCallback : public PxSimulationEventCallback
	{
	public:
		//an example variable that will be checked in the main simulation loop
		bool trigger;

		MySimulationEventCallback() : trigger(false) {}

		///Method called when the contact with the trigger object is detected.
		virtual void onTrigger(PxTriggerPair* pairs, PxU32 count)
		{
			//you can read the trigger information here
			for (PxU32 i = 0; i < count; i++)
			{
				//filter out contact with the planes
				if (pairs[i].otherShape->getGeometryType() != PxGeometryType::ePLANE)
				{
					//check if eNOTIFY_TOUCH_FOUND trigger
					if (pairs[i].status & PxPairFlag::eNOTIFY_TOUCH_FOUND)
					{
						cerr << "onTrigger::eNOTIFY_TOUCH_FOUND" << endl;
						trigger = true;
					}
					//check if eNOTIFY_TOUCH_LOST trigger
					if (pairs[i].status & PxPairFlag::eNOTIFY_TOUCH_LOST)
					{
						cerr << "onTrigger::eNOTIFY_TOUCH_LOST" << endl;
						trigger = false;
					}
				}
			}
		}

		///Method called when the contact by the filter shader is detected.
		virtual void onContact(const PxContactPairHeader& pairHeader, const PxContactPair* pairs, PxU32 nbPairs)
		{
			cerr << "Contact found between " << pairHeader.actors[0]->getName() << " " << pairHeader.actors[1]->getName() << endl;

			//check all pairs
			for (PxU32 i = 0; i < nbPairs; i++)
			{
				//check eNOTIFY_TOUCH_FOUND
				if (pairs[i].events & PxPairFlag::eNOTIFY_TOUCH_FOUND)
				{
					cerr << "onContact::eNOTIFY_TOUCH_FOUND" << endl;
				}
				//check eNOTIFY_TOUCH_LOST
				if (pairs[i].events & PxPairFlag::eNOTIFY_TOUCH_LOST)
				{
					cerr << "onContact::eNOTIFY_TOUCH_LOST" << endl;
				}
			}
		}

		virtual void onConstraintBreak(PxConstraintInfo* constraints, PxU32 count) {}
		virtual void onWake(PxActor** actors, PxU32 count) {}
		virtual void onSleep(PxActor** actors, PxU32 count) {}
#if PX_PHYSICS_VERSION >= 0x304000
		virtual void onAdvance(const PxRigidBody* const* bodyBuffer, const PxTransform* poseBuffer, const PxU32 count) {}
#endif
	};

	//A simple filter shader based on PxDefaultSimulationFilterShader - without group filtering
	static PxFilterFlags CustomFilterShader(PxFilterObjectAttributes attributes0, PxFilterData filterData0,
		PxFilterObjectAttributes attributes1, PxFilterData filterData1,
		PxPairFlags& pairFlags, const void* constantBlock, PxU32 constantBlockSize)
	{
		// let triggers through
		if (PxFilterObjectIsTrigger(attributes0) || PxFilterObjectIsTrigger(attributes1))
		{
			pairFlags = PxPairFlag::eTRIGGER_DEFAULT;
			return PxFilterFlags();
		}

		pairFlags = PxPairFlag::eCONTACT_DEFAULT;
		//enable continous collision detection
//		pairFlags |= PxPairFlag::eCCD_LINEAR;


		//customise collision filtering here
		//e.g.

		// trigger the contact callback for pairs (A,B) where 
		// the filtermask of A contains the ID of B and vice versa.
		if ((filterData0.word0 & filterData1.word1) && (filterData1.word0 & filterData0.word1))
		{
			//trigger onContact callback for this pair of objects
			pairFlags |= PxPairFlag::eNOTIFY_TOUCH_FOUND;
			pairFlags |= PxPairFlag::eNOTIFY_TOUCH_LOST;
			//			pairFlags |= PxPairFlag::eNOTIFY_CONTACT_POINTS;
		}

		return PxFilterFlags();
	};*/

	///Custom scene class
	class MyScene : public Scene
	{
		Plane* plane;
		Box* brick;
		GoalPost* goalPost;
		GoalCrossbar* goalCrossbar;
		SwingPost* swingPost;
		SwingTopBar* swingTopBar;
		RugbyBall* rugbyBall;
		Pitchfork* pitchfork;
		Truncheon* truncheon;
		InnerPitchLines* innerPitchLines;
		OuterPitchLines* outerPitchLines;
		InnerBarrierLines* innerBarrierLines;
		OuterBarrierLines* outerBarrierLines;
		BallCatapult* ballCatapult;
		//Trampoline* trampoline;
		RevoluteJoint* ballChain;

		PxMaterial* rubberMat = CreateMaterial(0.9f, 0.65f, 0.828f);
		//https://saferroadsconference.com/wp-content/uploads/2016/05/Peter-Cenek-Frictional-Characteristics-Roadside-Grass-Types.pdf
		PxMaterial* grassMat = CreateMaterial(0.35f, 0.5f, 0.f);
		PxMaterial* woodMat = CreateMaterial(0.5f, 0.48f, 0.6f);
		PxMaterial* metalMat = CreateMaterial(0.8f, 0.42f, 0.6f);

	public:
		///A custom scene class
		void SetVisualisation()
		{
			px_scene->setVisualizationParameter(PxVisualizationParameter::eSCALE, 1.0f);
			px_scene->setVisualizationParameter(PxVisualizationParameter::eCOLLISION_SHAPES, 1.0f);
			px_scene->setVisualizationParameter(PxVisualizationParameter::eACTOR_AXES, 1.0f);
			px_scene->setVisualizationParameter(PxVisualizationParameter::eBODY_LIN_VELOCITY, 1.0f);
		}

		//Custom scene initialisation
		virtual void CustomInit()
		{
			SetVisualisation();

			GetMaterial()->setDynamicFriction(.2f);
			
			//spawns grass and rugby pitch lines
			RugbyPitch();

			//barrier castle spawn
			Barrier();

			//goal spawn function
			Goal();

			//ball spawn function
			Ball();

			//pitchfork spawn function
			//Fork();

			//truncheon spawn function
			TruncheonWeapon();

			//swing arch function
			SwingArch();

			//brick spawn function
			Brick();

			//swing joint function
			SwingJoint();

			//catapult spawn function
			//Catapult();

			//Trampoline::AddToScene()
			/*trampoline = new Trampoline(PxVec3(3.f, 3.f, 3.f), PxReal(5.f), PxReal(5.f));
			trampoline->AddToScene(this);*/
		}

		void RugbyPitch() 
		{
			plane = new Plane();
			plane->Color(PxVec3(0.f / 255.f, 210.f / 255.f, 0.f / 255.f));
			plane->Material(grassMat);
			Add(plane);

			PitchLines();
		}

		void Fork() 
		{
			pitchfork = new Pitchfork((PxTransform(PxVec3(0.f, 0.5f, -1.5f))));
			pitchfork->Color(PxVec3(64.f / 255.f, 35.f / 255.f, 25.f / 255.f));
			//pitchfork->Get()->is<PxRigidDynamic>()->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, true);
			pitchfork->Material(woodMat);
			Add(pitchfork);
		}

		void TruncheonWeapon() 
		{
			truncheon = new Truncheon(PxTransform(PxVec3(0.f, 2.f, -38.85714287f)));
			truncheon->Get()->is<PxRigidDynamic>()->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, true);
			truncheon->Color(PxVec3(0.f / 255.f, 0.f / 255.f, 0.f / 255.f));
			truncheon->Material(woodMat);
			Add(truncheon);
		}

		void PitchLines()
		{
			innerPitchLines = new InnerPitchLines();
			innerPitchLines->Color(PxVec3(191.f / 255.f, 191.f / 255.f, 191.f / 255.f));
			Add(innerPitchLines);

			outerPitchLines = new OuterPitchLines();
			outerPitchLines->Color(PxVec3(191.f / 255.f, 191.f / 255.f, 191.f / 255.f));
			Add(outerPitchLines);
		}

		void Barrier() 
		{
			innerBarrierLines = new InnerBarrierLines();
			innerBarrierLines->Color(PxVec3(135.f / 255.f, 139.f / 255.f, 140.f / 255.f));
			innerBarrierLines->Material(metalMat);
			Add(innerBarrierLines);

			outerBarrierLines = new OuterBarrierLines();
			outerBarrierLines->Color(PxVec3(135.f / 255.f, 139.f / 255.f, 140.f / 255.f));
			outerBarrierLines->Material(metalMat);
			Add(outerBarrierLines);
		}

		void Goal()
		{
			goalPost = new GoalPost();
			goalPost->Material(metalMat);
			Add(goalPost);

			goalCrossbar = new GoalCrossbar();
			goalCrossbar->Material(metalMat);
			Add(goalCrossbar);
		}

		void Brick() 
		{
			//brick = new Box(PxTransform(PxVec3(0, 2, 0)), PxVec3(0.193675, 0.05715, 0.092075));
			brick = new Box(PxTransform(PxVec3(0.f, 2.f, -38.85714287f)), PxVec3(0.193675f, 0.05715f, 0.092075f));
			brick->Get()->is<PxRigidDynamic>()->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, true);
			brick->Color(PxVec3(255.f / 255.f, 0.f / 255.f, 0.f / 255.f));
			Add(brick);
		}

		void SwingArch()
		{
			swingPost = new SwingPost(PxTransform(PxVec3(0.f, 0.f, -42.85714287f)));
			Add(swingPost);

			swingTopBar = new SwingTopBar(PxTransform(PxVec3(0.f, 0.f, -42.85714287f)));
			Add(swingTopBar);
		}

		void Ball() 
		{
			//rugbyBall = new RugbyBall(PxTransform(PxVec3(0.f, 2.f, -38.85714287f)));
			rugbyBall = new RugbyBall(PxTransform(PxVec3(0.f, 0.5f, -42.85714287f)));
			rugbyBall->Material(rubberMat);
			rugbyBall->Color(PxVec3(140.f / 255.f, 83.f / 255.f, 62.f / 255.f));
			Add(rugbyBall);
		}

		void Catapult() 
		{
			ballCatapult = new BallCatapult(PxTransform(PxVec3(0.f, 0.f, -42.85714287f)));
			ballCatapult->Material(woodMat);
			ballCatapult->Color(PxVec3(0.f / 255.f, 0.f / 255.f, 0.f / 255.f));
			Add(ballCatapult);
		}

		void SwingJoint() 
		{
			//ballChain = new RevoluteJoint(swingTopBar, PxTransform(PxVec3(0.f, 5.f, 0.f)), brick, PxTransform(PxVec3(0.f, 10.f, 2.f)));
			ballChain = new RevoluteJoint(swingTopBar, PxTransform(PxVec3(0.f, 5.f, 0.f)), truncheon, PxTransform(PxVec3(0.f, 10.f, 2.f)));
		}

		/// An example use of key release handling
		void ExampleKeyReleaseHandler()
		{
			cerr << "I am realeased!" << endl;
		}

		/// An example use of key presse handling
		void ExampleKeyPressHandler()
		{
			cerr << "I am pressed!" << endl;
		}

		//Custom update function
		virtual void CustomUpdate()
		{

		}
	};
}
