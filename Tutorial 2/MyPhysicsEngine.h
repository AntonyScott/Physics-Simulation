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

	///Custom scene class
	class MyScene : public Scene
	{
		Plane* plane;
		Box* brick;
		CompoundObject* compound;
		GoalPost* goalPost;
		GoalCrossbar* goalCrossbar;
		SwingPost* swingPost;
		SwingTopBar* swingTopBar;
		RugbyBall* rugbyBall;
		InnerPitchLines* innerPitchLines;
		OuterPitchLines* outerPitchLines;
		BallCatapult* ballCatapult;
		//RevoluteJoint brickChain(swingTopBar, (PxTransform(PxVec3(0.f, 2.f, -38.85714287f)), ));

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

			//goal spawn function
			Goal();

			//ball spawn function
			Ball();

			//swing arch function
			SwingArch();

			//brick spawn function
			Brick();

			//catapult spawn function
			//Catapult();

			//brickChain();
		}

		void RugbyPitch() 
		{
			plane = new Plane();
			plane->Color(PxVec3(0.f / 255.f, 210.f / 255.f, 0.f / 255.f));
			plane->Material(grassMat);
			Add(plane);

			PitchLines();
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
			brick->Color(PxVec3(72.f / 255.f, 0.f / 255.f, 0.f / 255.f));
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
			rugbyBall = new RugbyBall(PxTransform(PxVec3(0.f, 2.f, -42.85714287f)));
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

		//Custom udpate function
		virtual void CustomUpdate()
		{

		}
	};
}
