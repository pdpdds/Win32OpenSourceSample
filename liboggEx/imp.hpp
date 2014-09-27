#include <oggplayer.h>
#include "SDL_audiocvt.hpp"
#include <fstream>
#include <vector>
#include <theora/theora.h>
#include <vorbis/codec.h>
#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/locks.hpp>
#include <boost/thread/condition.hpp>
#include <boost/circular_buffer.hpp>
#include <boost/timer.hpp>

struct OggPlayer::Imp {

	Imp() :
		cirbuf(0),video_lock(video_mut, boost::defer_lock){
		failbit = false;
		playing = false;
		need_close = false;
		time_factor = 1;
		refs = 1;
	}

	~Imp() {
		// open() tears down the partial setup on fail
		if (!failbit)
			close();
	}
	int queue_page(ogg_page * page);
	bool buffer_data();
	double get_time();
	void close();
	bool init_decoders();
	bool parse_headers();
	void open(std::string path, AudioFormat audio_format, int channels,
			int rate, VideoFormat video_format);
	bool decode_audio();
	bool decode_video();
	bool ready();
	void play();
	void play_loop();

	bool failbit;
	std::ifstream file_in;

	int theora_p;
	int vorbis_p;

	ogg_sync_state o_sync;

	vorbis_comment v_comment;
	theora_info t_info;
	theora_comment t_comment;
	vorbis_info v_info;

	ogg_page o_page;
	ogg_packet o_packet;

	ogg_stream_state o_tsstate;
	ogg_stream_state o_vsstate;

	theora_state t_state;
	vorbis_dsp_state v_state;
	vorbis_block v_block;

	int pp_level_max;
	int pp_level;
	int pp_inc;

	char* audio_buffer;
	boost::circular_buffer<char> cirbuf;
	unsigned int audio_buffer_size;
	SDL_AudioCVT cvt;

	bool playing;

	boost::thread play_thread;
	boost::mutex audio_mut;
	boost::mutex video_mut;
	boost::condition_variable audio_ready_cond;
	boost::condition_variable video_ready_cond;

	bool videobuf_ready;
	double videobuf_time;

	bool audio_cache_ready;

	// This lock should only be used from the main thread
	boost::unique_lock<boost::mutex> video_lock;
	// These variables are used to identify the frames (not just count them, not a debug variable)
	// see video_buffer_try_lock and video_read
	int frame;
	int last_frame_read;

	int audio_bytes_played;
	struct AudioGranulePos {
		int pos;
		double set_time;
	};
	std::deque<AudioGranulePos> audio_granule_poses;
	double time_factor;

	struct PixelFormat{
		void set(int r,int g,int b,int bpp){
			r_offset = r;
			g_offset = g;
			b_offset = b;
			this->bpp =bpp;
		}
		int r_offset;
		int g_offset;
		int b_offset;
		// if bpp=4 we have an alpha channel at offset 4
		int bpp;

	} pixel_format;
	bool need_close;
	boost::timer timer;
	int refs;
};
