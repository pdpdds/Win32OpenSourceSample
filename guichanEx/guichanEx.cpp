// guichanEx.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include <guichan.hpp>

#pragma comment(lib, "SDL2.lib")
#pragma comment(lib, "SDL2_TTF.lib")
#pragma comment(lib, "SDL2_IMAGE.lib")
#pragma comment(lib, "guichan.lib")

namespace globals
{
	gcn::Gui* gui;
}

// Include code to set up an SDL application with Guichan.
// The sdl.hpp file is responsible for creating and deleting
// the global Gui object.
#include "sdl.hpp"
// Include code to set up a Guichan GUI with all the widgets
// of Guichan. The code populates the global Gui object.
#include "widgets.hpp"

int _tmain(int argc, _TCHAR* argv[])
{
	try
	{
		sdl::init();

		widgets::init();
		sdl::run();
		widgets::halt();

		sdl::halt();
	}
	// Catch all Guichan exceptions.
	catch (gcn::Exception e)
	{
		std::cerr << e.getMessage() << std::endl;
		return 1;
	}
	// Catch all Std exceptions.
	catch (std::exception e)
	{
		std::cerr << "Std exception: " << e.what() << std::endl;
		return 1;
	}
	// Catch all unknown exceptions.
	catch (...)
	{
		std::cerr << "Unknown exception" << std::endl;
		return 1;
	}

	return 0;
}
