#include "Application.h"
#include "VideoPlayer.h"

#ifdef _DEBUG
#pragma comment(lib, "OgreMain_d.lib")
#pragma comment(lib, "OIS_d.lib")
#pragma comment(lib, "boost_graph-vc120-mt-1_55.lib")
#pragma comment(lib, "boost_thread-vc120-mt-gd-1_55.lib")
#pragma comment(lib, "boost_system-vc120-mt-gd-1_55.lib")
#else
#pragma comment(lib, "OgreMain.lib")
#pragma comment(lib, "OIS.lib")
#pragma comment(lib, "boost_graph-vc120-1_55.lib")
#pragma comment(lib, "boost_thread-vc120-mt-1_55.lib")
#pragma comment(lib, "boost_system-vc120-mt-1_55.lib")
#endif

#pragma comment(lib, "swscale.lib")
#pragma comment(lib, "SDL2main.lib")
#pragma comment(lib, "SDL2.lib")
#pragma comment(lib, "avutil.lib")
#pragma comment(lib, "avcodec.lib")
#pragma comment(lib, "avfilter.lib")
#pragma comment(lib, "avdevice.lib")
#pragma comment(lib, "avformat.lib")


int main(int argc, char *argv[])
{
  if (argc < 2)
  {
    std::cout << "Usage: " << argv[0] << " <Video resource name>" << std::endl;
    exit(0);
  }

  Application app;
  app.run ();

  VideoPlayer player;
  player.playVideo (argv[1]);
  while (player.isPlaying ())
  {
    player.update ();
	if (player.isPlaying() == false || !app.render())
      break;
  }

  return 0;
}
