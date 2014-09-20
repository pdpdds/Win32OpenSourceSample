#include <cstring>
#include <vector>
#include <deque>

#include <OIS/OIS.h>

namespace boost { class thread; }

enum KeyEventType
{
	KeyPressed, KeyReleased
};
enum MouseEventType
{
	MousePressed, MouseReleased, MouseMoved
};

enum EventType
{
	ET_Keyboard, ET_Mouse /*, Joystick */
};

struct InputEvent
{
	EventType eventType;
};

struct MouseEvent : public InputEvent
{
	MouseEventType mouseEventType;
	OIS::MouseButtonID buttonID;
	size_t x,y,z;
};

struct KeyEvent : public InputEvent
{
	KeyEventType keyEventType;
	OIS::KeyCode keyCode;
	unsigned int text;
};

class InputManager : public OIS::KeyListener, public OIS::MouseListener
{
public:
	InputManager();
	~InputManager();

	void destroy();
	void create(const size_t& windowID);
	void recreate(const size_t& windowID);
	
	void process();
	
	// poll events
	InputEvent* pollEvents();
	
	bool mShutdownThread;
	
protected:
	boost::thread* mThread;

	OIS::InputManager* mOIS;
	OIS::Mouse* mOISMouse;
	OIS::Keyboard* mOISKeyboard;
	
	// event queue
	std::deque<InputEvent*> mEvents;
	
	virtual bool mouseMoved(const OIS::MouseEvent&);
	virtual bool mousePressed(const OIS::MouseEvent&, OIS::MouseButtonID);
	virtual bool mouseReleased(const OIS::MouseEvent&, OIS::MouseButtonID);
	virtual bool keyPressed(const OIS::KeyEvent&);
	virtual bool keyReleased(const OIS::KeyEvent&);
};
