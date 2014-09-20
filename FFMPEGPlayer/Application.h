#include "BaseApplication.h"

#include <OgreRectangle2D.h>

namespace Ogre { class SceneManager; class Viewport; class Camera; }

class Application : public BaseApplication
{
public:
	Application();
	~Application();
	
  bool render();

//protected:
	virtual void createScene();
	virtual void onRenderWindowRecreated();

    virtual void update (float dt);
	
	Ogre::SceneManager* mSceneMgr;
	Ogre::Viewport* mViewport;
	Ogre::Camera* mCamera;

    Ogre::Rectangle2D* mRectangle;
};
