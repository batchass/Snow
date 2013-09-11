
#include "SnowApp.h"
void SnowApp::prepareSettings( Settings *settings )
{
	settings->setWindowSize( 200, 200 );
	settings->setFrameRate( 60.0f );
	settings->enableConsoleWindow();
}
void SnowApp::setup()
{
	lastUpdate = getElapsedSeconds();
	mUpdateInterval = 2.0f;
	mMainDisplayWidth = Display::getMainDisplay()->getWidth();
	mRenderX = mMainDisplayWidth;
	mRenderY = 0;
	for (auto display : Display::getDisplays() )
	{
		std::cout << "Reso:" << display->getHeight() << "\n"; //getSize()
		//display->getMainDisplay
		mRenderWidth = display->getWidth();
		mRenderHeight = display->getHeight();
	}
	//store window
	controlWindow = this->getWindow();
	int uniqueId = getNumWindows();
	controlWindow->getSignalClose().connect(
		[uniqueId,this] { shutdown(); this->console() << "You quit console window #" << uniqueId << std::endl; }
	);

	gui = new ciUICanvas(0,0,getWindowWidth(), getWindowHeight());
	gui->setTheme( CI_UI_THEME_RUSTICORANGE );

	gui->addWidgetDown( new ciUIRotarySlider( 50, 0.0, 6.0, 2.0, "rate" ) ); 
	gui->registerUIEvents(this, &SnowApp::guiEvent); 

	mFrequency = 0.01f;
	mPerlin = Perlin();
	createNewWindow();

}
void SnowApp::guiEvent(ciUIEvent *event)
{
	string name = event->widget->getName(); 
	cout << name << endl; 
	if(name == "rate")
	{
		ciUIRotarySlider *rate = (ciUIRotarySlider *) event->widget; 
		cout << rate->getScaledValue() << endl;
		cout << rate->getValue() << endl;
		mUpdateInterval = rate->getScaledValue(); 
	}
}
void SnowApp::createNewWindow()
{
	WindowRef renderWindow = createWindow( Window::Format().size( mRenderWidth, mRenderHeight ) );
	// create instance of the window and store in vector
	RenderWindow rWin = RenderWindow("name", mRenderWidth, mRenderHeight, renderWindow);	
	renderWindows.push_back( rWin );
	renderWindow->setPos(mRenderX, mRenderY);
	renderWindow->setBorderless();
	renderWindow->setAlwaysOnTop();

	HWND hWnd = (HWND)renderWindow->getNative();

	HRESULT hr = S_OK;
	// Create and populate the Blur Behind structure
	DWM_BLURBEHIND bb = {0};

	// Enable Blur Behind and apply to the entire client area
	bb.dwFlags = DWM_BB_ENABLE;
	bb.fEnable = true;
	bb.hRgnBlur = NULL;

	// Apply Blur Behind
	hr = DwmEnableBlurBehindWindow(hWnd, &bb);
	if (SUCCEEDED(hr))
	{
		HRESULT hr = S_OK;

		// Set the margins, extending the bottom margin.
		MARGINS margins = {-1};

		// Extend the frame on the bottom of the client area.
		hr = DwmExtendFrameIntoClientArea(hWnd,&margins);
	}

	// for demonstration purposes, we'll connect a lambda unique to this window which fires on close
	int uniqueId = getNumWindows();
	renderWindow->getSignalClose().connect(
		[uniqueId,this] { shutdown(); this->console() << "You closed window #" << uniqueId << std::endl; }
	);

}
void SnowApp::mouseDown( MouseEvent event )
{
}

void SnowApp::update()
{
	if ( getElapsedFrames() - lastUpdate > mUpdateInterval )
	{
		lastUpdate = getElapsedFrames();
		std::vector<Particle*>::iterator it;
		for(it = mParticleSystem.particles.begin(); it != mParticleSystem.particles.end(); ++it ) 
		{
			if ( (*it)->position.y > mRenderHeight -20 ) mParticleSystem.destroyParticle( *it );
			break;
		}

		float x = ci::randFloat( 0.0f, mRenderWidth );
		float y = ci::randFloat( 0.0f, 5 );
		float radius = ci::randFloat( 1.0f, 5.0f );
		float mass = radius;radius;
		float drag = 0.95f;
		Particle *particle = new Particle( Vec2f( x, y ), radius, mass, drag );
		mParticleSystem.addParticle( particle );

		// wind
		Vec2f oscilationVec;
		oscilationVec.x = sin(getElapsedSeconds()*0.2f)*0.1f;
		oscilationVec.y = 0.2f;
		mNumParticles = 0;

		for(it = mParticleSystem.particles.begin(); it != mParticleSystem.particles.end(); ++it ) 
		{
			Vec2f windForce = mPerlin.dfBm( (*it)->position * mFrequency );
			(*it)->forces += windForce * 0.1f;
			(*it)->forces += oscilationVec;
			mNumParticles++;
		}
		mParticleSystem.update();
	}
}

void SnowApp::draw()
{
	// clear out the window with transparency
	gl::clear( ColorAf( 0.0f, 0.0f, 0.0f, 0.0f ) );

	// Draw on render window only
	if (app::getWindow() == controlWindow)	
	{
		 gui->draw();
	}
	else
	{
		// set the drawing color to white
		gl::color( Color::white() );
		//gl::setMatricesWindow( getWindowWidth(), getWindowHeight() );
		mParticleSystem.draw();
	}
}

CINDER_APP_NATIVE( SnowApp, RendererGl )
