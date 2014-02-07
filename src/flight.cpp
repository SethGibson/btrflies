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