#include "btrfly.h"
#include "cinder/Rand.h"

namespace BFly
{
		//class ButterFly
		ButterFly::ButterFly()
		{
			/*
			butterfly f = new Bu
			*/
		}

		//ugh initialization list!!!
		ButterFly::ButterFly(Vec2i pPos, Vec2i pVel, gl::Texture pTex):mPos(pPos),mVel(pVel),mTexture(pTex)
		{
		}

		ButterFly::~ButterFly()
		{
		}

		void ButterFly::step()
		{
			//move towards target
			if(mTargetLife>0)
				--mTargetLife;
			else
			{
				//reset target
				mTarget = Vec2i(randInt(0,640), randInt(0,240));
				//reset targetlife
			}
		}

		void ButterFly::show()
		{
			gl::draw(mTexture, mPos);
		}

		//class Flight
		Flight::Flight()
		{
		}

		Flight::~Flight()
		{
		}

		void Flight::step()
		{
			for(vector<ButterFly>::iterator bfit=mRabble.begin();bfit!=mRabble.end();++bfit)
				bfit->step();
		}

		void Flight::show()
		{
			for(vector<ButterFly>::iterator bfit=mRabble.begin();bfit!=mRabble.end();++bfit)
				bfit->show();
		}
}