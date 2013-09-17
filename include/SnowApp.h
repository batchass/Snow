#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "cinder/Rand.h"
#include "cinder/Perlin.h"
#include "ParticleSystem.h"
#include "dwmapi.h"
#include "ciUI.h"
#include "OscListener.h"

using namespace ci;
using namespace ci::app;
using namespace std;

// RenderWindow class
class RenderWindow
{
	public:
		RenderWindow( string name, int width, int height, WindowRef wRef )
			: mName( name ), mWidth ( width ), mHeight ( height ), mWRef ( wRef )
		{
			
		}
		
		WindowRef mWRef;
		
	private:
		string mName;
		int mWidth;
		int mHeight;
		
};

class SnowApp : public AppNative {
public:
	void prepareSettings( AppBasic::Settings *settings );
	void setup();
	void mouseDown( MouseEvent event );	
	void update();
	void draw();
	void quitProgram();
	void createNewWindow();
	ciUICanvas *gui;   
	void guiEvent(ciUIEvent *event);
private:
	vector<RenderWindow>	renderWindows;
	WindowRef				controlWindow;
	int							mMainDisplayWidth;
	int							mRenderX;
	int							mRenderY;
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
	int							mUpdateInterval;
	double						lastUpdate;
	osc::Listener 				receiver;

};