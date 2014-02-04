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
		ButterFly(Vec2i pPos, gl::Texture pTex);
		~ButterFly();

		void step();
		void show();
		Vec2f getVel();
	private:
		bool mSeeking;
		int mTargetLife;
		Vec2i mPos;
		Vec2f mVel, mSpd;
		gl::Texture mTexture;
	};

	class Flight
	{
	public:
		Flight();
		Flight(int pCount, vector<gl::Texture> pTex);
		~Flight();

		void step();
		void showL0();
		void showL2();

	private:
		int mCount;
		vector<ButterFly> mRabble;
		vector<ButterFly> mLayer0;
		vector<ButterFly> mLayer2;
	};
}
#endif __BTRFLY_H__