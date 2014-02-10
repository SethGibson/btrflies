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
		void checkAndReflect();
		void show(int pSpriteId);
		Vec2i getVel(){return mVel;};

	private:
		int mTargetLife;
		int mSize;
		Vec2i mPos;
		Vec2f mVel;
		gl::Texture mTexture;
	};
}

#endif __BTRFLY_H__