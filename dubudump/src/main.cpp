#include <iostream>

#include <argparse/argparse.hpp>
#include <filesystem>

#include "DubuDump.h"

int main(int argc, char* argv[]) {
	argparse::ArgumentParser parser("dubudump");

	parser.add_argument("filepath").help("path to the file that will be dumped");

	parser.add_argument("--no-color").help("don't print with colors").default_value(false).implicit_value(true);
	parser.add_argument("--no-address")
	    .help("don't show the address on the left")
	    .default_value(false)
	    .implicit_value(true);
	parser.add_argument("--no-ascii")
	    .help("don't show the ascii on the right")
	    .default_value(false)
	    .implicit_value(true);
	parser.add_argument("--chunk-size")
	    .help("the amount of bytes to display per line")
	    .default_value(16)
	    .action([](const std::string& value) { return std::stoi(value); });

	try {
		parser.parse_args(argc, argv);
	} catch (const std::runtime_error& err) {
		std::cout << err.what() << std::endl;
		std::cout << parser;
		return 0;
	}

	dd::DubuDump dubudump(dd::DubuDumpConfig{.filepath    = parser.get("filepath"),
	                                         .useColors   = !parser.get<bool>("--no-color"),
	                                         .showAddress = !parser.get<bool>("--no-address"),
	                                         .showAscii   = !parser.get<bool>("--no-ascii"),
	                                         .chunkSize   = parser.get<int>("--chunk-size")});

	if (!dubudump.dump()) {
		return 1;
	}

	return 0;
}