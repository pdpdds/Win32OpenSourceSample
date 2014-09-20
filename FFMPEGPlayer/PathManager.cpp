#include "PathManager.h"

#include <stdlib.h> // getenv
#include <vector>
#include <boost/filesystem.hpp>

// Static members
std::string PathManager::ogre_plugin_dir;

void PathManager::init()
{
	/// Search for the OGRE plugin folder
	ogre_plugin_dir = "";
	
	 // Use environment variable, if present
	char *plugindir = getenv("OGRE_PLUGIN_DIR");
	if (plugindir)
	{
		ogre_plugin_dir = plugindir;
		return;
	}
	
	#ifdef _WIN32
		// Windows: we assume the ogre plugins are in the directory the program is run from (".")
		ogre_plugin_dir = ".";
	#else
		// Linux: Check a list of common path names
		typedef std::vector<boost::filesystem::path> Paths;
		Paths dirs;
		#if defined(__amd64__) || defined(__amd64) || defined(__x86_64__) || defined(_M_X64)
			dirs.push_back("/usr/local/lib64");
			dirs.push_back("/usr/lib64");
		#else
			dirs.push_back("/usr/local/lib32");
			dirs.push_back("/usr/lib32");
		#endif
		dirs.push_back("/usr/local");
		dirs.push_back("/usr/lib");
		// Loop through the paths and pick the first one that contains a plugin
		for (Paths::const_iterator p = dirs.begin(); p != dirs.end(); ++p) {
			if (boost::filesystem::exists(*p / "OGRE/RenderSystem_GL.so")) {
				ogre_plugin_dir = (*p / "OGRE").string();
				break;
			} else if (boost::filesystem::exists(*p / "ogre/RenderSystem_GL.so")) {
				ogre_plugin_dir = (*p / "ogre").string();
				break;
			}
		}
	#endif
}
