#include "btrfly.h"
#include "cinder/Rand.h"

namespace BFly
{b
		//class ButterFly
		ButterFly::ButterFly()
		{
		}

		//ugh initialization list!!!
		ButterFly::ButterFly(Vec2i pPos, gl::Texture pTex):mPos(pPos),mTexture(pTex)
		{
			mVel = Vec2f(2,1.5f);
			if(mPos.x<320)
				mVel.x = -2;
			if(mPos.y<240)
				mVel.y = -1.5f;
			mSpd = Vec2f(randFloat(1.0f,1.75f),randFloat(1.0f,1.75f));
		}

		ButterFly::~ButterFly()
		{
		}

		void ButterFly::step()
		{
			if(mPos.x<0||mPos.x>639-48)
				mVel.x = -mVel.x;
			if(mPos.y<0||mPos.y>479-48)
				mVel.y = -mVel.y;
			mPos+=mVel*mSpd;
		}

		void ButterFly::show()
		{
			gl::draw(mTexture, Rectf(mPos.x,mPos.y,mPos.x+48,mPos.y+48));
		}

		Vec2f ButterFly::getVel()
		{
			return mVel;
		}
		//class Flight
		Flight::Flight()
		{
		}

		Flight::Flight(int pCount, vector<gl::Texture> pTex)
		{
			mCount = pCount;
			for(int i=0;i<pCount;++i)
			{
				int tid = randInt(0,3);
				Vec2i pos = Vec2i(randInt(0,(639-48)),randInt(0,(479-48)));
				gl::Texture tex = pTex.at(tid);
				mRabble.push_back(ButterFly(pos,tex));
			}
		}
		Flight::~Flight()
		{
		}

		void Flight::step()
		{
			mLayer0.clear();
			mLayer2.clear();
			for(vector<ButterFly>::iterator bfit=mRabble.begin();bfit!=mRabble.end();++bfit)
			{
				bfit->step();
				if(bfit->getVel().x<0)
					mLayer0.push_back(*bfit);
				else
					mLayer2.push_back(*bfit);
			}
		}

		void Flight::showL0()
		{
			for(vector<ButterFly>::iterator bfit=mLayer0.begin();bfit!=mLayer0.end();++bfit)
				bfit->show();
		}

		void Flight::showL2()
		{
			for(vector<ButterFly>::iterator bfit=mLayer2.begin();bfit!=mLayer2.end();++bfit)
				bfit->show();
		}
}