#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Texture.h"
#include "btrfly.h"

using namespace ci;
using namespace ci::app;
using namespace std;
using namespace BFly;

class btrfliesApp : public AppNative
{
public:
	void setup();
	void mouseDown( MouseEvent event );	
	void update();
	void draw();

private:
	gl::Texture mDrawTex;
	Flight mSwarm;
};

void btrfliesApp::setup()
{
}

void btrfliesApp::mouseDown( MouseEvent event )
{
}

void btrfliesApp::update()
{
}

void btrfliesApp::draw()
{
	// clear out the window with black
	gl::clear( Color( 0, 0, 0 ) ); 
}

CINDER_APP_NATIVE( btrfliesApp, RendererGl )
