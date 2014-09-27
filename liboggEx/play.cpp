#include "imp.hpp"

bool OggPlayer::Imp::ready() {
	return audio_cache_ready;
}
// return true if need more data
// the return value is strange but the function is
// for internal use only and should only be called
// in play()
bool OggPlayer::Imp::decode_audio() {
	int ret;
	float **pcm;
	// if there's pending, decoded audio, grab it
	while ((ret = vorbis_synthesis_pcmout(&v_state, &pcm)) > 0) {
		boost::unique_lock<boost::mutex> lock(audio_mut);
		// first we need to convert from float to signed short
		// then we will use SDL_ConvertAudio
		// is our temp buffer big enough?
		if (cirbuf.capacity() < ret * v_info.channels * cvt.len_mult
			* sizeof(short)) {
				cirbuf.set_capacity(ret * v_info.channels * cvt.len_mult * 16
					* sizeof(short));
		}
		if (cirbuf.capacity() - cirbuf.size() < ret * v_info.channels
			* cvt.len_mult * sizeof(short)) {
				audio_cache_ready = true;
				return false;
		}
		if ((audio_buffer_size) < ret * v_info.channels * cvt.len_mult
			* sizeof(short)) {
				if (audio_buffer)
					delete[] audio_buffer;
				audio_buffer_size = ret * v_info.channels * cvt.len_mult
					* sizeof(short);
				audio_buffer = new char[audio_buffer_size];
		}
		// convert from float to short
		int k = 0;
		char* char_ab = (char*) audio_buffer;
		short* short_ab = (short*) audio_buffer;
		for (int i = 0; i < ret; i++) {
			for (int j = 0; j < v_info.channels; j++) {
				int val = (int)(pcm[j][i] * 32767.f);
				if (val > 32767)
					val = 32767;
				if (val < -32768)
					val = -32768;
				short_ab[k++] = val;

			}
		}
		vorbis_synthesis_read(&v_state, ret);
		// now we can use SDL_ConvertAudio
		cvt.len = k * sizeof(short);
		cvt.buf = (Uint8*) char_ab;
		SDL_ConvertAudio(&cvt);

		for (int i = 0; i < cvt.len_cvt; i++) {
			cirbuf.push_back(char_ab[i]);
		}


		if(v_state.granulepos!=0){
			AudioGranulePos agp;
			agp.pos = audio_bytes_played+cvt.len_cvt;
			agp.set_time = (double)v_state.granulepos/(double)v_info.rate;
			audio_granule_poses.push_back(agp);
		}

		lock.unlock();
		audio_ready_cond.notify_one();
		return false;

	} 
	// no pending audio; is there a pending packet to decode?
	if (ogg_stream_packetout(&o_vsstate, &o_packet) > 0) {
		if (vorbis_synthesis(&v_block, &o_packet) == 0) {
			vorbis_synthesis_blockin(&v_state, &v_block);
			return false;
		}
	} else { return true; }

}
// similar to decode_audio
bool OggPlayer::Imp::decode_video() {
	bool was_ready=videobuf_ready;
	boost::unique_lock<boost::mutex> lock(video_mut, boost::defer_lock);
	ogg_int64_t videobuf_granulepos = -1;
	if (!videobuf_ready){
		if(!lock.try_lock())
			return false;
	}
	while (!videobuf_ready) {
		// theora is one in, one out...
		if (ogg_stream_packetout(&o_tsstate, &o_packet) > 0) {

			if(pp_inc){
				pp_level+=pp_inc;
				theora_control(&t_state,TH_DECCTL_SET_PPLEVEL,&pp_level,sizeof(pp_level));
				pp_inc=0;
			}
			if (o_packet.granulepos >= 0) {
				theora_control(&t_state, TH_DECCTL_SET_GRANPOS,
					&o_packet.granulepos, sizeof(o_packet.granulepos));
			}
			if (theora_decode_packetin(&t_state, &o_packet) == 0) {
				videobuf_granulepos = t_state.granulepos;
				videobuf_time = theora_granule_time(&t_state,	videobuf_granulepos);

				if (videobuf_time >= get_time()){
					videobuf_ready = true;
					frame++;
				}
				else {
					// If we are too slow, reduce the pp level.
					pp_inc=pp_level>0?-1:0;
				}
			}

		} else
			break;
	}
	lock.unlock();
	if(videobuf_ready!=was_ready)
		video_ready_cond.notify_one();

	return !videobuf_ready;
}

void OggPlayer::Imp::play_loop() {
	if(!file_in.is_open()) return;
	audio_cache_ready = false;
	audio_bytes_played = 0;
	bool audio_need_data = vorbis_p;
	bool video_need_data = theora_p;
	timer.restart();
	// buffer_data() will close the file on eof
	while ((file_in.is_open() || !audio_need_data || !video_need_data) && playing) {

		// sync audio video timer
		while(!audio_granule_poses.empty() &&
			audio_granule_poses.front().pos <= audio_bytes_played){
			time_factor= audio_granule_poses.front().set_time/timer.elapsed();
			audio_granule_poses.pop_front();
		}

		if (theora_p && !videobuf_ready) {
			video_need_data = decode_video();
		}

		if (vorbis_p) {
			audio_need_data = decode_audio();
		}

		// are we at or past time for this video frame?
		if (videobuf_ready && videobuf_time <= get_time()) {
			videobuf_ready = false;
		}
		// if no data yet for somebody, grab another page
		if (file_in.is_open() && (audio_need_data || video_need_data)) {
			// buffer_data() can handle eof itself
			buffer_data();
			while (ogg_sync_pageout(&o_sync, &o_page) > 0) {
				queue_page(&o_page);
			}
			audio_need_data = false;
			video_need_data = false;
		}
	}
	playing = false;

	// do not risk a lock
	audio_ready_cond.notify_one();
	video_ready_cond.notify_one();

	// cleanup
	close();
}

void OggPlayer::Imp::play() {
	playing = true;
	play_thread = boost::thread(boost::bind(&OggPlayer::Imp::play_loop, this));
}

