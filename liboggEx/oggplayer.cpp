#include <oggplayer.h>
#include "imp.hpp"

OggPlayer::~OggPlayer() {
	imp->refs--;
	if(imp->refs==0){
		close();
		delete imp;
	}
}

OggPlayer::OggPlayer() {
	imp = new Imp;
}

OggPlayer::OggPlayer(OggPlayer& os) {
	imp = os.imp;
	imp->refs++;
}

OggPlayer& OggPlayer::operator =(OggPlayer& os){
	if(this==&os) return *this;
	imp->refs--;
	if(imp->refs==0){
		close();
		delete imp;
	}
	imp = os.imp;
	imp->refs++;
	return *this;
}

OggPlayer::OggPlayer(std::string path, AudioFormat audio_format, int channels,
					 int rate, VideoFormat video_format){
	imp = new Imp;
	open(path, audio_format, channels, rate, video_format);
}

void OggPlayer::open(std::string path, AudioFormat audio_format, int channels,
					 int rate, VideoFormat video_format) {
	if(playing()) close();
	imp->open(path, audio_format, channels, rate, video_format);
}

void OggPlayer::close() {
	imp->playing = false;
	// wait for play thread to terminate (if any)
	imp->play_thread.join();
}

bool OggPlayer::fail() {
	return imp->failbit;
}

void OggPlayer::play() {
	imp->play();
}

bool OggPlayer::playing() {
	return imp->playing;
}


int OggPlayer::audio_read(char* buf, unsigned int size) {
	if(NULL==buf || !imp->vorbis_p || !playing() || !imp->ready()) return 0;
	boost::unique_lock<boost::mutex> lock(imp->audio_mut);

	while ((imp->cirbuf.size() < size) && playing()) {
		if(imp->cirbuf.capacity()< size)
		{
			imp->cirbuf.set_capacity(2*size);
		}
		imp->audio_ready_cond.wait(lock);
	}

	unsigned int k = 0;
	while (k < size && !imp->cirbuf.empty()) {
		buf[k++] = imp->cirbuf.front();
		imp->cirbuf.pop_front();
	}
	imp->audio_bytes_played+=k;

	return k;
}

bool OggPlayer::yuv_buffer_try_lock(YUVBuffer *buffer) {
	if (!playing() || !imp->theora_p || !imp->ready()) 
		return false;
	// no new data, no reason to lock
	if(imp->last_frame_read==imp->frame)
		return false;
	imp->video_lock.lock();
	while (!imp->videobuf_ready && playing()) {
		imp->video_ready_cond.wait(imp->video_lock);
	}
	//ready and locked!
	yuv_buffer yuv;
	theora_decode_YUVout(&(imp->t_state), &yuv);

	buffer->y_width = yuv.y_width;
	buffer->y_height = yuv.y_height;
	buffer->y_stride = yuv.y_stride;
	buffer->uv_width = yuv.uv_width;
	buffer->uv_height = yuv.uv_height;
	buffer->uv_stride = yuv.uv_stride;
	buffer->y = yuv.y;
	buffer->u = yuv.u;
	buffer->v = yuv.v;

	imp->last_frame_read = imp->frame;
	return true;
}

void OggPlayer::yuv_buffer_unlock() {
	imp->video_lock.unlock();
}

inline int clamp(int val){
	if(val<0) return 0;
	if(val>255) return 255;
	return val;
}

bool OggPlayer::video_read(char* buf, int stride) {
	YUVBuffer yuv;
	if(NULL==buf) return false;
	if (!yuv_buffer_try_lock(&yuv))
		return false;

	if(0==stride) stride=width()*imp->pixel_format.bpp;
	int h = height(), w = width();

	int uv_ki=yuv.y_width/yuv.uv_width;
	int uv_kj=yuv.y_height/yuv.uv_height;

	int y_offset = offset_x() + yuv.y_stride * offset_y();
	int uv_offset = offset_x()/uv_ki+yuv.uv_stride * offset_y()/uv_kj;
	int y_p,uv_p,b_p;

	for(int j=0;j<h;j++){
		y_p=y_offset+j*yuv.y_stride;
		b_p=j*stride;
		uv_p=uv_offset+j/uv_kj*yuv.uv_stride;

		for(int i=0;i<w;i++){
			//http://en.wikipedia.org/wiki/YUV
			int y = yuv.y[y_p]-16;
			int u = yuv.u[uv_p]-128;
			int v = yuv.v[uv_p]-128;
			int r =clamp((y*298+409*v+128)>>8);
			int g =clamp((y*298-100*v-208*u+128)>>8);
			int b =clamp((y*298+516*u+128)>>8);
			buf[b_p+imp->pixel_format.r_offset]=r;
			buf[b_p+imp->pixel_format.g_offset]=g;
			buf[b_p+imp->pixel_format.b_offset]=b;
			if(4==imp->pixel_format.bpp)
				buf[b_p+3]=255;

			b_p+=imp->pixel_format.bpp;
			y_p++;
			if( i%uv_ki == uv_ki-1 ) uv_p++;
		}
	}
	yuv_buffer_unlock();
	return true;
}

int OggPlayer::width() {
	return imp->t_info.width;
}
int OggPlayer::height() {
	return imp->t_info.height;
}
int OggPlayer::offset_x() {
	return imp->t_info.offset_x;
}
int OggPlayer::offset_y() {
	return imp->t_info.offset_y;
}

