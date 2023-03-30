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
		Knights* knights;
		RevoluteJoint* ballChain;

		//https://saferroadsconference.com/wp-content/uploads/2016/05/Peter-Cenek-Frictional-Characteristics-Roadside-Grass-Types.pdf
		PxMaterial* grassMat = CreateMaterial(0.35f, 0.5f, 0.f);
		//https://www.engineeringtoolbox.com/friction-coefficients-d_778.html
		PxMaterial* rubberMat = CreateMaterial(0.9f, 0.65f, 0.828f);
		PxMaterial* woodMat = CreateMaterial(0.5f, 0.48f, 0.6f);
		PxMaterial* metalMat = CreateMaterial(0.8f, 0.42f, 0.6f);
		PxMaterial* glassMat = CreateMaterial(0.9, 0.4, 0.69);

	public:
		///A custom scene class
		void SetVisualisation()
		{
			//visualisation parameters available to view when switching render mode using F7
			px_scene->setVisualizationParameter(PxVisualizationParameter::eSCALE, 1.0f);
			px_scene->setVisualizationParameter(PxVisualizationParameter::eCOLLISION_SHAPES, 1.0f);
			px_scene->setVisualizationParameter(PxVisualizationParameter::eACTOR_AXES, 1.0f);
			px_scene->setVisualizationParameter(PxVisualizationParameter::eBODY_LIN_VELOCITY, 1.0f);
			px_scene->setVisualizationParameter(PxVisualizationParameter::eJOINT_LIMITS, 1.0f);
			px_scene->setVisualizationParameter(PxVisualizationParameter::eJOINT_LOCAL_FRAMES, 1.0f);
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

			//truncheon spawn function, truncheon actor used in swing (called in keypress in visual debugger)
			TruncheonWeapon();

			//swing arch spawn function
			SwingArch();

			//catapult spawn function
			Catapult();
			
			//spawns in army
			KnightArmy();
		}

		void RugbyPitch() 
		{
			//this function adds a plane with a colour and grass material to the scene along with pitch lines
			plane = new Plane();
			plane->Color(PxVec3(0.f / 255.f, 210.f / 255.f, 0.f / 255.f));
			plane->Material(grassMat);
			Add(plane);

			PitchLines();
		}

		void PitchLines()
		{
			//this function spawns silver lines onto the play area of the pitch
			innerPitchLines = new InnerPitchLines();
			innerPitchLines->Color(PxVec3(191.f / 255.f, 191.f / 255.f, 191.f / 255.f));
			Add(innerPitchLines);

			outerPitchLines = new OuterPitchLines();
			outerPitchLines->Color(PxVec3(191.f / 255.f, 191.f / 255.f, 191.f / 255.f));
			Add(outerPitchLines);
		}

		void Fork(PxVec3 camPos, PxVec3 camDir)
		{
			//this function spawns a pitchfork at the camera position
			pitchfork = new Pitchfork((PxTransform(PxVec3(camPos))));
			pitchfork->Color(PxVec3(64.f / 255.f, 35.f / 255.f, 25.f / 255.f)); //colour set to light brown (wood)
			pitchfork->Material(woodMat);
			pitchfork->Get()->is<PxRigidDynamic>()->setMass(3); //mass set to 3kg
			Add(pitchfork);
			pitchfork->Get()->is<PxRigidDynamic>()->addForce(PxVec3(camDir.x, camDir.y, camDir.z)* 100000); //once spawned, force is added to the camera direction.xyz
		}

		void TruncheonWeapon() 
		{
			truncheon = new Truncheon(PxTransform(PxVec3(0.f, 2.f, -40.85714287f), PxQuat(PxPi / 4, PxVec3(-1.0f, 0.0f, 0.0f)))); //truncheon spawned in at an angle
			truncheon->Get()->is<PxRigidDynamic>()->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, true); //gravity is disabled as I want to line it up to hit the ball
			truncheon->Color(PxVec3(0.f / 255.f, 0.f / 255.f, 0.f / 255.f)); //colour set to black
			truncheon->Material(woodMat); //wooden material applied
			Add(truncheon);
		}
		
		//barrier castle function
		void Barrier() 
		{
			//barrier surronds the playing field
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
			//goal post and cross bar added to the scene along with a metal material
			goalPost = new GoalPost();
			goalPost->Material(metalMat);
			Add(goalPost);

			goalCrossbar = new GoalCrossbar();
			goalCrossbar->Material(metalMat);
			Add(goalCrossbar);
		}

		void SwingArch()
		{
			//this function forms the archway of which the ball is kicked
			swingPost = new SwingPost(PxTransform(PxVec3(0.f, 0.f, -42.85714287f)));
			Add(swingPost);

			//swingTopBar is a separate object as it will be used to as a joint attached to the truncheon actor later on
			swingTopBar = new SwingTopBar(PxTransform(PxVec3(0.f, 0.f, -42.85714287f)));
			Add(swingTopBar);
		}

		void Ball() 
		{
			rugbyBall = new RugbyBall(PxTransform(PxVec3(0.f, 1.f, -42.5f))); //transform is just before halfway line so joint can hit the ball
			rugbyBall->Material(rubberMat); //rubber friction material applied to the ball
			rugbyBall->Color(PxVec3(140.f / 255.f, 83.f / 255.f, 62.f / 255.f)); //brown colour applied
			//https://www.gilbertrugby.com/blogs/news/rugby-balls-which-ball-do-i-need#:~:text=7%20facts%20about%20Rugby%20Balls,and%20made%20of%20four%20panels.&text=It%20weighs%20410%2D460%20grams,for%20matches%20between%20young%20players.
			//rugby ball max weight is usually 460 grams
			rugbyBall->Get()->is<PxRigidDynamic>()->setMass(0.460); 
			Add(rugbyBall);
		}

		void Catapult() 
		{
			//spawns catapult and applies wood material
			ballCatapult = new BallCatapult(PxTransform(PxVec3(0.f, 0.f, -42.85714287f)));
			ballCatapult->Material(woodMat);
			ballCatapult->Color(PxVec3(0.f / 255.f, 0.f / 255.f, 0.f / 255.f));
			Add(ballCatapult);
		}

		void SwingJoint() 
		{
			//this function handles the swing joint and is called when pressing F1
			ballChain = new RevoluteJoint(swingTopBar, PxTransform(PxVec3(0.f, 8.f, 0.f)), truncheon, PxTransform(PxVec3(0.f, 6.f, -1.f)));
			truncheon->Get()->is<PxRigidDynamic>()->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, false);
			ballChain->DriveVelocity(PxReal(100));
		}

		void KnightArmy() 
		{
			//spawn function for spawing cubes "knights" that guard the goal
			knights = new Knights(PxTransform(PxVec3(0.f, 1.f, -57.14285716f)));
			knights->Color(PxVec3(191.f / 255.f, 128.f / 255.f, 105.f / 255.f));
			Add(knights);
		}

		void PlaneTranformation() 
		{
			//this function changes the plane to a glass colour and applies a glass friction material
			plane->Color(PxVec3(242.f / 255.f, 242.f / 255.f, 242.f / 255.f));
			plane->Material(glassMat);
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
