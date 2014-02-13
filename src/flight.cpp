#include "cinder/Rand.h"
#include "btrfly.h"
#include "flight.h"

namespace BFly
{
	Flight::Flight()
	{
	}

	Flight::Flight(int pCount, vector<gl::Texture> pTex)
	{
		mCount = pCount;
		for(int i=0;i<pCount;++i)
		{
			int tid = randInt(0,4);
			Vec2i pos = Vec2i(randInt(0,(639-48)),randInt(0,(479-48)));
			gl::Texture tex = pTex.at(tid);
			mRabble.push_back(ButterFly(pos,tex));
		}
	}
	Flight::~Flight()
	{
	}

	void Flight::step(int pSpriteId, bool pState)
	{
		mSpriteId=pSpriteId;
		mLayer0.clear();
		mLayer2.clear();
		
		for(int bi=pState;bi<mRabble.size();++bi)
		{
			mRabble.at(bi).step();
			Vec2i vel = mRabble.at(bi).getVel();
			if(vel.x<0)
				mLayer0.push_back(mRabble.at(bi));
			else
				mLayer2.push_back(mRabble.at(bi));
		}
		/*
		for(vector<ButterFly>::iterator bfit=mRabble.begin();bfit!=mRabble.end();++bfit)
		{
			bfit->step();
			Vec2i vel = bfit->getVel();
			if(vel.x<0)
				mLayer0.push_back(*(bfit));
			else
				mLayer2.push_back(*(bfit));
		}*/

	}

	void Flight::showL0()
	{
		int bid=0;
		for(vector<ButterFly>::iterator bfit=mLayer0.begin();bfit!=mLayer0.end();++bfit)
		{
			int sid = (mSpriteId+bid)%9;
			bfit->show(sid);
			bid+=1;
		}
	}

	void Flight::showL2()
	{
		int bid=0;
		for(vector<ButterFly>::iterator bfit=mLayer2.begin();bfit!=mLayer2.end();++bfit)
		{
			int sid = (mSpriteId+bid)%9;
			bfit->show(sid);
			bid+=1;
		}
	}
}