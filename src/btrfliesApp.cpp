#include <vector>
#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Texture.h"
#include "cinder/gl/Fbo.h"
#include "cinder/gl/GlslProg.h"
#include "cinder/ImageIo.h"
#include "cinder/Surface.h"
#include "btrfly.h"
#include "flight.h"
#include "util_pipeline.h"

using namespace ci;
using namespace ci::app;
using namespace std;
using namespace BFly;

class btrfliesApp : public AppNative
{
public:
	void prepareSettings(Settings *pSettings);
	void setup();
	void keyDown( KeyEvent event);
	void update();
	void draw();
	void quit();

private:
	void getMasked();

	bool mDrawFeed;
	Surface8u mSurfL0, mSurfL1, mSurfL2;
	gl::Texture mBkg;
	gl::Texture mRGBFeed, mSegFeed;
	gl::Fbo mFBO;
	gl::GlslProg mShader;
	vector<gl::Texture> mDrawTex;	
	UtilPipeline mPXC;
	Flight mSwarm;

};

void btrfliesApp::prepareSettings(Settings *pSettings)
{
	pSettings->setWindowSize(640,480);
	pSettings->setFrameRate(30);
}

void btrfliesApp::setup()
{
	mDrawTex.push_back(loadImage(loadAsset("btrflySprite01.png")));
	mDrawTex.push_back(loadImage(loadAsset("btrflySprite02.png")));
	mDrawTex.push_back(loadImage(loadAsset("btrflySprite03.png")));
	mDrawTex.push_back(loadImage(loadAsset("btrflySprite04.png")));
	mBkg = loadImage(loadAsset("garden.jpg"));
	mSwarm = Flight(25, mDrawTex);

	mFBO = gl::Fbo(640,480,true);
	try
	{
		mShader = gl::GlslProg(loadAsset("bflies.vert"),loadAsset("bflies.frag"));
	}
	catch(const exception &e)
	{
		console() <<e.what()<<endl;
	}
	gl::enableAlphaBlending();
	
	mPXC.EnableImage(PXCImage::COLOR_FORMAT_RGB24);
	mPXC.EnableGesture();
	mPXC.EnableSegmentation();
	mPXC.Init();

	mDrawFeed = false;
}

void btrfliesApp::keyDown( KeyEvent event )
{
	if(event.getCode()==KeyEvent::KEY_b)
		mDrawFeed = !mDrawFeed;
}

void btrfliesApp::update()
{
	int spriteId = getElapsedFrames()%9;
	mSwarm.step(spriteId);
	
	if(mPXC.AcquireFrame(true))
	{
		PXCImage *rgbImg = mPXC.QueryImage(PXCImage::IMAGE_TYPE_COLOR);
		PXCImage *segImg = mPXC.QuerySegmentationImage();
		PXCImage::ImageData rgbData;
		PXCImage::ImageData segData;
		if(rgbImg->AcquireAccess(PXCImage::ACCESS_READ, &rgbData)>=PXC_STATUS_NO_ERROR)
		{
			mRGBFeed = gl::Texture(rgbData.planes[0], GL_BGR, 640,480);
			rgbImg->ReleaseAccess(&rgbData);
		}
		if(segImg->AcquireAccess(PXCImage::ACCESS_READ, &segData)>=PXC_STATUS_NO_ERROR)
		{
			mSegFeed = gl::Texture(segData.planes[0], GL_LUMINANCE, 320,240);
			segImg->ReleaseAccess(&segData);
		}

		mPXC.ReleaseFrame();
	}
}

void btrfliesApp::draw()
{
	gl::clear(Color(0,0,0));
	gl::color(Color::white());

	if(mDrawFeed)
		gl::draw(mRGBFeed,Vec2f::zero());
	else
		gl::draw(mBkg,Vec2f::zero());
	mSwarm.showL0();
	getMasked();
	mSwarm.showL2();
}

void btrfliesApp::quit()
{
	mPXC.Close();
}

void btrfliesApp::getMasked()
{
	mFBO.bindFramebuffer();
	gl::disableAlphaBlending();
	gl::clear(ColorA(0,0,0,1));
	gl::color(ColorAf(Color::white()));
	gl::draw(mSegFeed, Rectf(0,0,640,480));
	mFBO.unbindFramebuffer();

	mShader.bind();
	mShader.uniform("texRGB",0);
	mRGBFeed.bind(0);
	mShader.uniform("texMask",1);
	mFBO.bindTexture(1);
	
	gl::enableAlphaBlending();
	gl::drawSolidRect(Rectf(0,0,640,480));
	mRGBFeed.unbind();
	mFBO.unbindTexture();
	mShader.unbind();
}

CINDER_APP_NATIVE( btrfliesApp, RendererGl )
