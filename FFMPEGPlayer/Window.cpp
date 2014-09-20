#include "Window.h"
#include "InputManager.h"

#include <OgreRoot.h>
#include <OgreRenderWindow.h>
#include <OgreStringConverter.h>

using namespace Ogre;

Window::Window() : 
	mWidth(0), mHeight(0), // when 0, use highest supported resolution
	mFullscreen(1), mFSAA(0), mVSync(1),
	mRenderWindow(0), mWindowTitle("OgreFramework")
{
	// get a list of supported fullscreen resolutions
	const StringVector& videoModes = Root::getSingleton().getRenderSystem()->getConfigOptions()["Video Mode"].possibleValues;
	for (int i=0; i < videoModes.size(); i++)
	{
		String mode = videoModes[i];
		StringUtil::trim(mode);

		StringVector vmopts = StringUtil::split(mode, " x");  // only resolution
		size_t w = StringConverter::parseUnsignedInt(vmopts[0]);
		size_t h = StringConverter::parseUnsignedInt(vmopts[1]);
		
		mSupportedFullscreenResolutions.push_back( std::make_pair(w, h) );
	}
}

//------------------------------------------------------------------------------//

Window::~Window()
{
	destroy();
}

//------------------------------------------------------------------------------//

void Window::recreate()
{
	destroy();
	create();
}

//------------------------------------------------------------------------------//

void Window::destroy()
{
	//mInputManager->destroy();

	if (mRenderWindow)
	{
		WindowEventUtilities::removeWindowEventListener(mRenderWindow, mListener);
		Root::getSingleton().getRenderSystem()->destroyRenderWindow(mWindowTitle);
	}
}

//------------------------------------------------------------------------------//

void Window::create()
{
	if (mWidth == 0 || mHeight == 0)
	{
		// use highest resolution (last in supported resolutions)
		std::pair<size_t, size_t> res = mSupportedFullscreenResolutions.back();
		mWidth = res.first; mHeight = res.second;
	}
	
	NameValuePairList settings;
	settings.insert(std::make_pair("title", mWindowTitle));
	settings.insert(std::make_pair("FSAA", StringConverter::toString(mFSAA)));
	settings.insert(std::make_pair("vsync", mVSync ? "true" : "false"));

	mRenderWindow = Root::getSingleton().createRenderWindow(mWindowTitle, mWidth, mHeight, mFullscreen, &settings);
	WindowEventUtilities::addWindowEventListener(mRenderWindow, mListener);
	
	size_t windowID; mRenderWindow->getCustomAttribute("WINDOW", &windowID);
	//mInputManager->create(windowID);
}
