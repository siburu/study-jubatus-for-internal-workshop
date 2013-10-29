#include <iostream>
#include <fstream>
#include <string>
#include <utility>
#include <vector>

#include <jubatus/client.hpp>

#include <cassert>

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

	jubatus::classifier::client::classifier client(host, port, name, 1.0);

	std::ifstream ifs(argv[1]);
	char rbuf[1024*1024];
	ifs.read(rbuf, sizeof(rbuf) - 1);
	rbuf[ifs.gcount()] = '\0';

	vector<datum> test_data;
	test_data.push_back(make_datum(rbuf));
	vector<vector<estimate_result> > results = client.classify(test_data);

	std::cout << results.size() << std::endl;
	for (size_t i = 0; i < results.size(); ++i) {
		for (size_t j = 0; j < results[i].size(); ++j) {
			const estimate_result& r = results[i][j];
			if (r.score > 1)
				std::cout << r.label << " " << r.score << std::endl;
		}
		std::cout << std::endl;
	}
}
