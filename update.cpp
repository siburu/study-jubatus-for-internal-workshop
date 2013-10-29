#include <iostream>
#include <fstream>
#include <string>
#include <utility>
#include <vector>

#include <jubatus/client.hpp>

#include <cassert>
#include <unistd.h>
#include <boost/filesystem.hpp>

namespace fs = boost::filesystem;

using std::make_pair;
using std::pair;
using std::string;
using std::vector;
using jubatus::client::common::datum;
using jubatus::classifier::estimate_result;
using jubatus::classifier::labeled_datum;

datum make_datum(const char *filedata) {
	datum d;
	d.add_string("filedata", filedata);
	return d;
}

int main(int argc, char *argv[]) {
	assert(2 == argc);

	string host = "127.0.0.1";
	int port = 9199;
	string name = "test";
	jubatus::classifier::client::classifier *client;
	client = new jubatus::classifier::client::classifier(host, port, name, 1.0);

	fs::path rootdir(argv[1]);
	typedef fs::directory_iterator dirit_t;

	dirit_t endit;
	for (dirit_t dirit(rootdir); dirit != endit; dirit++) {
		if (!fs::is_directory(dirit->status()))
			continue;

		fs::path dir = dirit->path();
		for (dirit_t dirit(dir); dirit != endit; dirit++) {
			if (!fs::is_regular_file(dirit->status()))
				continue;

			fs::path file = dirit->path();
			std::ifstream ifs(file.c_str());
			char rbuf[1024*1024];
			ifs.read(rbuf, sizeof(rbuf) - 1);
			rbuf[ifs.gcount()] = '\0';

			std::vector<labeled_datum> train_data;
			train_data.push_back(
					labeled_datum(dir.c_str(), make_datum(rbuf)));

retry:
			try {
				client->train(train_data);
			} catch (msgpack::rpc::timeout_error &te) {
				std::cout << "timed out: " << file.c_str() << std::endl;
				sleep(1);
				delete client;
				client = new jubatus::classifier::client::classifier(
						host, port, name, 1.0);
				goto retry;
			}
		}
	}

	return 0;
}
