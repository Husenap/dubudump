#pragma once

#include <filesystem>

namespace dd {

struct DubuDumpConfig {
	std::filesystem::path filepath;
	bool useColors;
	bool showAddress;
	bool showAscii;
	int chunkSize;
};

class DubuDump {
public:
	DubuDump(DubuDumpConfig config);

	bool dump();

private:
	const DubuDumpConfig mConfig;
};

}  // namespace dd