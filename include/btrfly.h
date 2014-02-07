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

	private:
		int mTargetLife, mSpriteId;
		float mSize;
		Vec2i mPos;
		Vec2f mVel;
		gl::Texture mTexture;
	};
}

#endif __BTRFLY_H__