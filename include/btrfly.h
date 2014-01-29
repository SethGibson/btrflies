#ifndef __BTRFLY_H__
#define __BTRFLY_H__
#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Texture.h"
#include <vector>

using namespace std;
using namespace ci;

namespace BFly
{
	class ButterFly
	{
	public:
		ButterFly();
		~ButterFly();

		void step();
		void show();

	private:
		bool mSeeking;
		Vec2i mTarget;
	};

	class Flight
	{
	public:
		Flight();
		~Flight();

		void step();
		void show();
		void add();
		void remove();

	private:
		vector<ButterFly> mRabble;
	};
}
#endif __BTRFLY_H__