#include <vector>
#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Texture.h"
#include "cinder/ImageIo.h"
#include "btrfly.h"

using namespace ci;
using namespace ci::app;
using namespace std;
using namespace BFly;

class btrfliesApp : public AppNative
{
public:
	void prepareSettings(Settings *pSettings);
	void setup();
	void mouseDown( MouseEvent event );	
	void update();
	void draw();

private:
	vector<gl::Texture> mDrawTex;
	gl::Texture mBkg;
	Flight mSwarm;
};

void btrfliesApp::prepareSettings(Settings *pSettings)
{
	pSettings->setWindowSize(640,480);
	pSettings->setFrameRate(30);
}

void btrfliesApp::setup()
{
	mDrawTex.push_back(loadImage(loadAsset("bfly_01.png")));
	mDrawTex.push_back(loadImage(loadAsset("bfly_02.png")));
	mDrawTex.push_back(loadImage(loadAsset("bfly_03.png")));
	mDrawTex.push_back(loadImage(loadAsset("bfly_04.png")));
	mBkg = loadImage(loadAsset("garden.jpg"));
	mSwarm = Flight(10, mDrawTex);
	gl::enableAlphaBlending();
}

void btrfliesApp::mouseDown( MouseEvent event )
{
}

void btrfliesApp::update()
{
	mSwarm.step();
}

void btrfliesApp::draw()
{
	// clear out the window with black
	gl::clear( Color( 0, 0, 0 ) ); 

	gl::draw(mBkg, Vec2f::zero());
	mSwarm.showL0();
	gl::color(Color::white());
	gl::drawSolidRect(Rectf(160.f,120.f,480.f,480.f));
	mSwarm.showL2();
}

CINDER_APP_NATIVE( btrfliesApp, RendererGl )
