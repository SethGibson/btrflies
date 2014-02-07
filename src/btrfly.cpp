#include "btrfly.h"
#include "cinder/Rand.h"

namespace BFly
{
	//class ButterFly
	ButterFly::ButterFly()
	{
	}

	//ugh initialization list!!!
	ButterFly::ButterFly(Vec2i pPos, gl::Texture pTex):mPos(pPos),mTexture(pTex)
	{
	}

	ButterFly::~ButterFly()
	{
	}

	void ButterFly::step()
	{
		//
	}

	void ButterFly::show()
	{
		int tl = mSpriteId*128;
		int br = tl+127;
		int ssize = 128*mSize;

		gl::draw(mTexture, Area(tl,0,br,127), Rectf(mPos,Vec2i(ssize,ssize)));
	}
}