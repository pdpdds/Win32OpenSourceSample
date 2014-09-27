//=========================================================================
// Author      : Kambiz Veshgini
// License     : This library is distributed in the hope that it will be 
//               useful, but WITHOUT ANY WARRANTY; without even the implied 
//               warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
//               PURPOSE.  See the GNU Lesser General Public License for 
//               more details.
//=========================================================================

#ifndef OGGSTREAM_H_
#define OGGSTREAM_H_

#include <string>

#if defined(MSVC_COMPILING_DLL)
	#define DLL __declspec(dllexport)
#else
	#define DLL
#endif
// These formats are equivalent to those used in SDL_audio
enum AudioFormat {
	AF_U8 = 0x0008,
	AF_S8 = 0x8008,
	AF_U16LSB = 0x0010,
	AF_S16LSB = 0x8010,
	AF_U16MSB = 0x1010,
	AF_S16MSB = 0x9010,
	AF_U16 = 0x0010,
	AF_S16 = 0x8010
};

enum VideoFormat {
	VF_RGB, VF_BGR, VF_RGBA, VF_BGRA
};

struct YUVBuffer {
	int y_width;
	int y_height;
	int y_stride;
	int uv_width;
	int uv_height;
	int uv_stride;
	unsigned char *y;
	unsigned char *u;
	unsigned char *v;
};

class OggPlayer {
public:
	DLL ~OggPlayer();
	DLL OggPlayer();
	DLL OggPlayer(OggPlayer& ogg);
	DLL OggPlayer& operator =(OggPlayer& os);
	DLL OggPlayer(std::string path, AudioFormat audio_format, int channels,
			int rate, VideoFormat video_format=VF_BGRA);
	DLL void open(std::string path, AudioFormat audio_format, int channels,
			int rate, VideoFormat video_format=VF_BGRA);
	DLL void close();
	DLL bool fail();
	DLL void play();
	DLL bool playing();
	DLL int audio_read(char* buf, unsigned int size);
	DLL bool yuv_buffer_try_lock(YUVBuffer *buffer);
	DLL void yuv_buffer_unlock();
	DLL bool video_read(char* buf, int stride=0);
	DLL int width();
	DLL int height();
	DLL int offset_x();
	DLL int offset_y();
private:
	struct Imp;
	Imp* imp;
};

#endif /* OGGSTREAM_H_ */
