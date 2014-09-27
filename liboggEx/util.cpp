#include "imp.hpp"
#ifdef _WINDOWS
#include <windows.h>
#endif
int OggPlayer::Imp::queue_page(ogg_page * page) {
	if (theora_p)
		ogg_stream_pagein(&o_tsstate, page);
	if (vorbis_p)
		ogg_stream_pagein(&o_vsstate, page);
	return 0;
}

bool OggPlayer::Imp::buffer_data() {
	if (!file_in.is_open())
		return 0;
	if (file_in.eof()) {
		file_in.close();
		return 0;
	}
	char *buffer = ogg_sync_buffer(&o_sync, 4096);
	int bytes = file_in.read(buffer, 4096).gcount();
	ogg_sync_wrote(&o_sync, bytes);
	return (bytes != 0);
}

double OggPlayer::Imp::get_time() {
	double now = time_factor*timer.elapsed();
	return now;
}

