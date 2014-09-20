// SampleAvro.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "cpx.hh"
#include "avro/Encoder.hh"
#include "avro/Decoder.hh"

#pragma comment(lib, "avrocpp.lib")
int _tmain(int argc, _TCHAR* argv[])
{
	std::auto_ptr<avro::OutputStream> out = avro::memoryOutputStream();
	avro::EncoderPtr e = avro::binaryEncoder();
	e->init(*out);
	c::cpx c1;
	c1.re = 1.0;
	c1.im = 2.13;
	avro::encode(*e, c1);
	
	std::auto_ptr<avro::InputStream> in = avro::memoryInputStream(*out);
	avro::DecoderPtr d = avro::binaryDecoder();
	d->init(*in);
	
	c::cpx c2;
	avro::decode(*d, c2);
	std::cout << '(' << c2.re << ", " << c2.im << ')' << std::endl;

	return 0;
}

