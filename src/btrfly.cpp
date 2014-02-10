#include "btrfly.h"
#include "cinder/Rand.h"

float S_SIZES[] = {.25f,.5f,.75f};
namespace BFly
{
	//class ButterFly
	ButterFly::ButterFly()
	{
	}

	//ugh initialization list!!!
	ButterFly::ButterFly(Vec2i pPos, gl::Texture pTex):mPos(pPos),mTexture(pTex)
	{
		mSize = 128*S_SIZES[randInt(3)];
		mVel = Vec2i(320,240)-mPos;
		mVel.limit(randInt(4,8));
	}

	ButterFly::~ButterFly()
	{
	}

	void ButterFly::step()
	{
		mPos+=mVel;
		checkAndReflect();
	}

	void ButterFly::show(int pSpriteId)
	{
		int tl = pSpriteId*128;
		int br = tl+127;

		gl::draw(mTexture, Area(tl,0,br,128), Rectf(mPos,Vec2i(mPos.x+mSize,mPos.y+mSize)));
	}

	void ButterFly::checkAndReflect()
	{
		int quad = -1;
		//reflect x
		if(mPos.x<0||mPos.x>639-mSize)
		{
			if(mPos.x<0)
				quad=mPos.y<240?0:2;
			else if(mPos.x>639-mSize)
				quad=mPos.y<240?1:3;
		}

		//reflect y
		else if(mPos.y<0||mPos.y>479-mSize)
		{
			if(mPos.y<0)
				quad=mPos.x<320?0:1;
			else
				quad=mPos.x<320?2:3;
		}

		if(quad>=0)
		{
			switch(quad)
			{
				case 0:
				{
					mVel = Vec2i(randInt(320,640),randInt(240,480))-mPos;
					break;
				}
				case 1:
				{
					mVel = Vec2i(randInt(0,320),randInt(240,480))-mPos;
					break;
				}
				case 2:
				{
					mVel = Vec2i(randInt(320,640),randInt(0,240))-mPos;
					break;
				}
				case 3:
				{
					mVel = Vec2i(randInt(0,320),randInt(0,240))-mPos;
					break;
				}
			}

			mVel.limit(randInt(4,8));
		}
	}
}