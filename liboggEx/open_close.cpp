#include "imp.hpp"

	void OggPlayer::Imp::close() {
		if(!need_close) return;

		if (vorbis_p) {
			ogg_stream_clear(&o_vsstate);
			vorbis_block_clear(&v_block);
			vorbis_dsp_clear(&v_state);
		}
		vorbis_comment_clear(&v_comment);
		vorbis_info_clear(&v_info);

		if (theora_p) {
			ogg_stream_clear(&o_tsstate);
			theora_clear(&t_state);
		}
		theora_comment_clear(&t_comment);
		theora_info_clear(&t_info);

		ogg_sync_clear(&o_sync);
		file_in.close();
		if (audio_buffer){
			delete[] audio_buffer;
			audio_buffer = NULL;
		}
		need_close = false;
	}
	bool OggPlayer::Imp::parse_headers() {
		int stateflag = 0;
		while (!stateflag) {
			if (!buffer_data())
				break;
			while (ogg_sync_pageout(&o_sync, &o_page) > 0) {
				ogg_stream_state test;

				/* is this a mandated initial header? If not, stop parsing */
				if (!ogg_page_bos(&o_page)) {
					/* don't leak the page; get it into the appropriate stream */
					queue_page(&o_page);
					stateflag = 1;
					break;
				}

				ogg_stream_init(&test, ogg_page_serialno(&o_page));
				ogg_stream_pagein(&test, &o_page);
				ogg_stream_packetout(&test, &o_packet);

				/* identify the codec: try theora */
				if (!theora_p && theora_decode_header(&t_info, &t_comment,
						&o_packet) >= 0) {
					/* it is theora */
					memcpy(&o_tsstate, &test, sizeof(test));
					theora_p = 1;
				} else if (!vorbis_p && vorbis_synthesis_headerin(&v_info,
						&v_comment, &o_packet) >= 0) {
					/* it is vorbis */
					memcpy(&o_vsstate, &test, sizeof(test));
					vorbis_p = 1;
				} else {
					/* whatever it is, we don't care about it */
					ogg_stream_clear(&test);
				}
			}
		}

		// we're expecting more header packets.
		while ((theora_p && theora_p < 3) || (vorbis_p && vorbis_p < 3)) {
			int ret;

			// look for further theora headers
			while (theora_p && (theora_p < 3) && (ret = ogg_stream_packetout(
					&o_tsstate, &o_packet))) {
				if (ret < 0) {
					return false;
				}
				if (theora_decode_header(&t_info, &t_comment, &o_packet)) {
					return false;
				}
				theora_p++;
			}

			// look for more vorbis header packets
			while (vorbis_p && (vorbis_p < 3) && (ret = ogg_stream_packetout(
					&o_vsstate, &o_packet))) {
				if (ret < 0) {
					return false;
				}
				if (vorbis_synthesis_headerin(&v_info, &v_comment, &o_packet)) {
					return false;
				}
				vorbis_p++;
				if (vorbis_p == 3)
					break;
			}

			// The header pages/packets will arrive before anything else we
			//   care about, or the stream is not obeying spec

			if (ogg_sync_pageout(&o_sync, &o_page) > 0) {
				queue_page(&o_page); /* demux into the appropriate stream */
			} else {
				if (!buffer_data()) /* someone needs more data */{
					return false;
				}
			}
		}
		return true;

	}

	bool OggPlayer::Imp::init_decoders() {
		if (theora_p) {
			theora_decode_init(&t_state, &t_info);
			theora_control(&t_state, TH_DECCTL_GET_PPLEVEL_MAX, &pp_level_max,
					sizeof(pp_level_max));
			pp_level = pp_level_max;
			theora_control(&t_state, TH_DECCTL_SET_PPLEVEL, &pp_level,
					sizeof(pp_level));
			pp_inc = 0;
		} else {
			return false;
		}
		if (vorbis_p) {
			vorbis_synthesis_init(&v_state, &v_info);
			vorbis_block_init(&v_state, &v_block);
		} else {
			return false;
		}
		return true;
	}

	void OggPlayer::Imp::open(std::string path, AudioFormat audio_format, int channels,
			int rate, VideoFormat video_format) {
				
		theora_p = 0;
		vorbis_p = 0;
		videobuf_ready = false;
		frame=0;
		last_frame_read=0;

		// 1) open the input file
		file_in.open(path.c_str(), std::ios::binary);
		if (file_in.fail()) {
			failbit = true;
			return;
		}

		// 2) init some structs
		ogg_sync_init(&o_sync);
		vorbis_info_init(&v_info);
		vorbis_comment_init(&v_comment);
		theora_info_init(&t_info);
		theora_comment_init(&t_comment);

		// 3) parse headers
		if (!failbit && !parse_headers()) {
			failbit = true;
		}

		// 3) init decoders
		if (!failbit && !init_decoders()) {
			failbit = true;
		}

		// 4) if fail, tear down the partial setup
		if (failbit) {
			theora_info_clear(&t_info);
			theora_comment_clear(&t_comment);
			vorbis_info_clear(&v_info);
			vorbis_comment_clear(&v_comment);
			ogg_sync_clear(&o_sync);
			file_in.close();
			return;
		}

		// 5) init audio conversion
		int ret = SDL_BuildAudioCVT(&cvt, AUDIO_S16, v_info.channels,
				v_info.rate, audio_format, channels, rate);
		if (-1 == ret) {
			failbit = true;
			return;
		}
		// play() will allocate memory when needed
		audio_buffer_size = 0;
		audio_buffer = NULL;

		switch(video_format){
		//pixel_format.set(r_offset,g_offset,b_offset,bpp not a_offset);
		case VF_RGB:
			pixel_format.set(0,1,2,3);
			break;
		case VF_BGR:
			pixel_format.set(2,1,0,3);
			break;
		case VF_RGBA:
			pixel_format.set(0,1,2,4);
			break;
		case VF_BGRA:
			pixel_format.set(2,1,0,4);
			break;
		}
		need_close = true;
	}

