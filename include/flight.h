#ifndef __FLIGHT_H__
#define __FLIGHT_H__
#include <vector>
#include "cinder/gl/Texture.h"
#include "btrfly.h"

using namespace std;
using namespace ci;

namespace BFly
{
	class Flight
	{
	public:
		Flight();
		Flight(int pCount, vector<gl::Texture> pTex);
		~Flight();

		void step(int pSpriteId, bool pState);
		void showL0();
		void showL2();
		int getCount(){return mCount;};

	private:
		bool mIsTracking;
		int mCount, mSpriteId;
		vector<ButterFly> mRabble;
		vector<ButterFly> mLayer0;
		vector<ButterFly> mLayer2;
	};
}
#endif