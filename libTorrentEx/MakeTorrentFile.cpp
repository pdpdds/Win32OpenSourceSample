#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <iterator>
#include <iomanip>

#include "libtorrent/entry.hpp"
#include "libtorrent/bencode.hpp"
#include "libtorrent/torrent_info.hpp"
#include "libtorrent/file.hpp"
#include "libtorrent/file_pool.hpp"
#include "libtorrent/storage.hpp"
#include "libtorrent/hasher.hpp"
#include "libtorrent/create_torrent.hpp"

#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/bind.hpp>

using namespace boost::filesystem;
using namespace libtorrent;

// do not include files and folders whose
// name starts with a .
bool file_filter(boost::filesystem::path const& filename)
{
	if (filename.leaf().c_str()[0] == '.') return false;
	std::cerr << filename << std::endl;
	return true;
}

void print_progress(int i, int num)
{
	std::cerr << "\r" << (i + 1) << "/" << num;
}

int main(int argc, char* argv[])
{
	using namespace libtorrent;
	using namespace boost::filesystem;

	int piece_size = 256 * 1024;
	char const* creator_str = "libtorrent";

	//path::default_name_check(no_check);

	if (argc != 4 && argc != 5)
	{
		std::cerr << "usage: libtorrentMake <output torrent-file> "
			"<announce url> <file or directory to create torrent from> "
			"[url-seed]\n";
		return 1;
	}

#ifndef BOOST_NO_EXCEPTIONS
	try
	{
#endif
		file_storage fs;
		file_pool fp;
		path full_path = complete(path(argv[3]));

		add_files(fs, full_path.c_str(), file_filter);

		create_torrent t(fs, piece_size);
		t.add_tracker(argv[2]);
		set_piece_hashes(t, full_path.branch_path().c_str()
			, boost::bind(&print_progress, _1, t.num_pieces()));
		std::cerr << std::endl;
		t.set_creator(creator_str);

		if (argc == 5) t.add_url_seed(argv[4]);

		// create the torrent and print it to out
		ofstream out(complete(path(argv[1])), std::ios_base::binary);
		bencode(std::ostream_iterator<char>(out), t.generate());
#ifndef BOOST_NO_EXCEPTIONS
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << "\n";
	}
#endif

	return 0;
}