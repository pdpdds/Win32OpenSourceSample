#include "InputManager.h"

#include <OIS/OIS.h>
#include <boost/thread.hpp>

#include <sstream>

//------------------------------------------------------------------------------//

void InputThread(InputManager* manager)
{
	while (!manager->mShutdownThread)
	{
		manager->process();
		boost::this_thread::sleep(boost::posix_time::milliseconds(1));
	}
}

//------------------------------------------------------------------------------//

InputManager::InputManager() :
	mOIS(0), mOISMouse(0), mOISKeyboard(0), mShutdownThread(0)
{
	mThread = new boost::thread(InputThread, this);
}

InputManager::~InputManager()
{
	mShutdownThread = true;
	if (mThread->joinable())
	mThread->join();
	delete mThread;
}

//------------------------------------------------------------------------------//

void InputManager::process()
{
	if (mOISKeyboard) mOISKeyboard->capture();
	if (mOISMouse) mOISMouse->capture();
}

//------------------------------------------------------------------------------//

void InputManager::destroy()
{
	if (mOIS)
	{
		if (mOISMouse)
			mOIS->destroyInputObject(mOISMouse);
		if (mOISKeyboard)
			mOIS->destroyInputObject(mOISKeyboard);
		
		mOIS->destroyInputSystem(mOIS);
		mOIS = 0;
		
		mOISKeyboard = 0; mOISMouse = 0;
	}
}

//------------------------------------------------------------------------------//

void InputManager::recreate(const size_t& windowID)
{
	destroy();
	create(windowID);
}

//------------------------------------------------------------------------------//

void InputManager::create(const size_t& windowID)
{
	OIS::ParamList pl;
	std::ostringstream windowHndStr;
	windowHndStr << windowID;
	pl.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));

	//!todo do not hardcore these
    #ifdef OIS_LINUX_PLATFORM
	pl.insert(std::make_pair(std::string("x11_mouse_grab"), std::string("false")));
	pl.insert(std::make_pair(std::string("x11_mouse_hide"), std::string("false")));
	pl.insert(std::make_pair(std::string("x11_keyboard_grab"), std::string("false")));
    pl.insert(std::make_pair(std::string("XAutoRepeatOn"), std::string("true")));
    #endif

	mOIS = OIS::InputManager::createInputSystem( pl );
	
	// Create devices
	if (mOIS->getNumberOfDevices(OIS::OISKeyboard) > 0)
	{
		mOISKeyboard = static_cast<OIS::Keyboard*>(mOIS->createInputObject(OIS::OISKeyboard, true));
		mOISKeyboard->setEventCallback(this);
	}
		
	if (mOIS->getNumberOfDevices(OIS::OISMouse) > 0)
	{
		mOISMouse = static_cast<OIS::Mouse*>(mOIS->createInputObject(OIS::OISMouse, true));
		mOISMouse->setEventCallback(this);
	}
	//!todo Joysticks
}

//------------------------------------------------------------------------------//

bool InputManager::mouseMoved(const OIS::MouseEvent& OISEvent)
{
	MouseEvent* ev = new MouseEvent(); ev->eventType = ET_Mouse;
	ev->mouseEventType = MouseMoved;
	ev->x = OISEvent.state.X.rel;
	ev->y = OISEvent.state.Y.rel;
	ev->z = OISEvent.state.Z.rel;
	
	mEvents.push_back(ev);

	return true;
}

//------------------------------------------------------------------------------//

bool InputManager::mousePressed(const OIS::MouseEvent& OISEvent, OIS::MouseButtonID id)
{
	MouseEvent* ev = new MouseEvent(); ev->eventType = ET_Mouse;
	ev->buttonID = id; ev->mouseEventType = MousePressed;
	ev->x = OISEvent.state.X.rel;
	ev->y = OISEvent.state.Y.rel;
	ev->z = OISEvent.state.Z.rel;
	
	mEvents.push_back(ev);

	return true;
}

//------------------------------------------------------------------------------//

bool InputManager::mouseReleased(const OIS::MouseEvent& OISEvent, OIS::MouseButtonID id)
{
	MouseEvent* ev = new MouseEvent(); ev->eventType = ET_Mouse;
	ev->buttonID = id; ev->mouseEventType = MouseReleased;
	ev->x = OISEvent.state.X.rel;
	ev->y = OISEvent.state.Y.rel;
	ev->z = OISEvent.state.Z.rel;
	
	mEvents.push_back(ev);

	return true;
}

//------------------------------------------------------------------------------//

bool InputManager::keyPressed(const OIS::KeyEvent& OISEvent)
{
	KeyEvent* ev = new KeyEvent(); ev->eventType = ET_Keyboard;
	ev->keyEventType = KeyPressed;
	ev->keyCode = OISEvent.key;
	ev->text = OISEvent.text;
	
	mEvents.push_back(ev);

	return true;
}

//------------------------------------------------------------------------------//

bool InputManager::keyReleased(const OIS::KeyEvent& OISEvent)
{
	KeyEvent* ev = new KeyEvent(); ev->eventType = ET_Keyboard;
	ev->keyEventType = KeyReleased;
	ev->keyCode = OISEvent.key;
	ev->text = OISEvent.text;
		
	mEvents.push_back(ev);

	return true;
}

//------------------------------------------------------------------------------//

InputEvent* InputManager::pollEvents()
{
	if (mEvents.size() > 0)
	{
		InputEvent* ev = mEvents.front();
		mEvents.pop_front();
		return ev;
	}
	else return 0;
}
