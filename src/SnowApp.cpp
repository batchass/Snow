#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "cinder/Rand.h"
#include "cinder/Perlin.h"
#include "ParticleSystem.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class SnowApp : public AppNative {
public:
	void setup();
	void mouseDown( MouseEvent event );	
	void update();
	void draw();
	
private:
	int							mRenderWidth;
	int							mRenderHeight;
	int							mNumParticles;
	ParticleSystem				mParticleSystem;
	float						mFrequency;
	Perlin						mPerlin;
	float						maxAlignSpeed;
	bool						mMouseDown;
	float						repulsionFactor;
	Surface8u					mImageOutput;
};

void SnowApp::setup()
{
	mRenderWidth = 1024;
	mRenderHeight = 768;
}

void SnowApp::mouseDown( MouseEvent event )
{
}

void SnowApp::update()
{
	/*float x = ci::randFloat( 0.0f, mRenderWidth );
	float y = ci::randFloat( 0.0f, 5.0f );
	float radius = ci::randFloat( 0.8f, 3.0f );
	float mass = Rand::randFloat(1.f, 5.f);
	float drag = 0.95f;
	Particle *particle = new Particle( Vec2f( x, y ), radius, mass, drag );
	mParticleSystem.addParticle( particle );*/
	std::vector<Particle*>::iterator it;
	for(it = mParticleSystem.particles.begin(); it != mParticleSystem.particles.end(); ++it ) 
	{
		if ( (*it)->position.y > mRenderHeight -20 ) mParticleSystem.destroyParticle( *it );
		break;
	}
	mFrequency = 0.01f;
	mPerlin = Perlin();

	// wind
	Vec2f oscilationVec;
	oscilationVec.x = sin(getElapsedSeconds()*0.2f)*0.1f;
	oscilationVec.y = 0.2f;//sin(getElapsedSeconds()*0.6f)*0.2f;
	mNumParticles = 0;
	//std::vector<Particle*>::iterator it;
	for(it = mParticleSystem.particles.begin(); it != mParticleSystem.particles.end(); ++it ) 
	{
		Vec2f windForce = mPerlin.dfBm( (*it)->position * mFrequency );
		(*it)->forces += windForce * 0.1f;
		(*it)->forces += oscilationVec;
		mNumParticles++;
	}
	mParticleSystem.update();
}

void SnowApp::draw()
{
	// clear out the window with black
	gl::clear( Color( 0, 0, 0 ) ); 
	gl::color( Color( 255, 255, 255 ) );
	//gl::setMatricesWindow( getWindowWidth(), getWindowHeight() );
	mParticleSystem.draw();
}

CINDER_APP_NATIVE( SnowApp, RendererGl )
