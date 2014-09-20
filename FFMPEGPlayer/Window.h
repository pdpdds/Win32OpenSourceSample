#include <string>
#include <vector>

#include <OgreWindowEventUtilities.h>

class InputManager;

namespace Ogre { class RenderWindow; }

class Window
{
public:
	Window();
	~Window();

	std::string mWindowTitle;

	size_t mHeight;
	size_t mWidth;
	bool mVSync;
	bool mFullscreen;
	size_t mFSAA;
	
	std::vector< std::pair<size_t, size_t> > mSupportedFullscreenResolutions;
	
	Ogre::RenderWindow* mRenderWindow;

	void recreate();
	void destroy();
	void create();
	
	// assigned from outside
	//InputManager* mInputManager;
	Ogre::WindowEventListener* mListener;
};
