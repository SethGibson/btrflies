#include <vector>
#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Texture.h"
#include "cinder/gl/Fbo.h"
#include "cinder/gl/GlslProg.h"
#include "cinder/ImageIo.h"
#include "cinder/Rand.h"
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
	Vec2f getMappedColor(int px, int py);

	bool mDrawFeed, mTracking;
	float* mUVBuffer;
	Vec2f mFingerPos;
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
	mPXC.EnableImage(PXCImage::COLOR_FORMAT_DEPTH);
	mPXC.EnableGesture();
	mPXC.EnableSegmentation();
	mPXC.Init();

	mDrawFeed = true;
}

void btrfliesApp::keyDown( KeyEvent event )
{
	if(event.getCode()==KeyEvent::KEY_b)
		mDrawFeed = !mDrawFeed;
}

void btrfliesApp::update()
{
	int spriteId = getElapsedFrames();
	mSwarm.step(spriteId, false);
	
	if(mPXC.AcquireFrame(true))
	{
		mTracking = false;
		PXCImage *rgbImg = mPXC.QueryImage(PXCImage::IMAGE_TYPE_COLOR);
		PXCImage *depthImg = mPXC.QueryImage(PXCImage::IMAGE_TYPE_DEPTH);
		PXCImage *segImg = mPXC.QuerySegmentationImage();
		PXCImage::ImageData rgbData, segData, depthData;
		
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
		if(depthImg->AcquireAccess(PXCImage::ACCESS_READ, &depthData)>=PXC_STATUS_NO_ERROR)
		{
			mUVBuffer = (float *)depthData.planes[2];
			depthImg->ReleaseAccess(&depthData);
		}

		PXCGesture *gest = mPXC.QueryGesture();
		PXCGesture::Gesture gestData;
		PXCGesture::GeoNode nodeData;
		pxcStatus foundNode = PXC_STATUS_ITEM_UNAVAILABLE;

		foundNode=gest->QueryNodeData(0,PXCGesture::GeoNode::LABEL_BODY_HAND_PRIMARY|PXCGesture::GeoNode::LABEL_FINGER_THUMB, &nodeData);
		if(foundNode<PXC_STATUS_NO_ERROR)
			foundNode=gest->QueryNodeData(0,PXCGesture::GeoNode::LABEL_BODY_HAND_PRIMARY|PXCGesture::GeoNode::LABEL_FINGER_INDEX, &nodeData);

		if(foundNode>=PXC_STATUS_NO_ERROR)
		{
			console() << "Found Hand" << endl;
			mTracking = true;
			mFingerPos.set(getMappedColor((int)nodeData.positionImage.x, (int)nodeData.positionImage.y));
		}
		else
		{
			console() << "No Hand" << endl;
			mTracking = false;
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
	gl::drawSolidCircle(mFingerPos, 10);
}

void btrfliesApp::quit()
{
	mPXC.Close();
	delete[] mUVBuffer;
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

Vec2f btrfliesApp::getMappedColor(int px, int py)
{
    float cx = mUVBuffer[(py * 320 + px) * 2] * 640 + 0.5f;
    float cy = mUVBuffer[(py * 320 + px) * 2 + 1] * 640 + 0.5f;
	return Vec2f(cx,cy);
}

CINDER_APP_NATIVE( btrfliesApp, RendererGl )