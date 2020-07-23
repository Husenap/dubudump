#include "DubuDump.h"

#include <fmt/color.h>
#include <fmt/format.h>
#include <fstream>
#include <iostream>

namespace dd {

DubuDump::DubuDump(DubuDumpConfig config)
    : mConfig(config) {}

bool DubuDump::dump() {
	auto& [filepath, useColors, showAddress, showAscii, chunkSize] = mConfig;

	fmt::rgb addressColor(255, 255, 255);
	fmt::rgb hexColor(255, 255, 255);
	fmt::rgb asciiColor(255, 255, 255);
	if (useColors) {
		addressColor = fmt::rgb(255, 0, 255);
		hexColor     = fmt::rgb(255, 0, 0);
		asciiColor   = fmt::rgb(255, 255, 0);
	}

	if (!std::filesystem::exists(filepath)) {
		std::cerr << fmt::format("The path {} doesn't exist!", filepath.generic_string().c_str()) << std::endl;
		return false;
	}

	if (std::filesystem::is_directory(filepath)) {
		std::cerr << fmt::format("The path {} is a directory, expected file!", filepath.generic_string().c_str())
		          << std::endl;
		return false;
	}

	std::ifstream file(filepath, std::ios::binary | std::ios::ate | std::ios::out);

	if (file.fail()) {
		std::cerr << "Failed to open file!" << std::endl;
		return false;
	}

	long long fileSize = file.tellg();
	file.seekg(0);

	std::vector<char> hexBuffer(chunkSize + 1ll, '\0');
	std::vector<char> asciiBuffer(chunkSize + 1ll, '\0');

	for (long long address = 0; address < fileSize; address += chunkSize) {
		long long bytes = fileSize - address;

		if (bytes > chunkSize) {
			bytes = chunkSize;
		}

		file.read(hexBuffer.data(), bytes);

		if (showAddress) {
			fmt::print(fmt::fg(addressColor), "{:08X}  ", address);
		}

		for (long long byteIndex = 0; byteIndex < chunkSize; ++byteIndex) {
			if (byteIndex < bytes) {
				fmt::print(fmt::fg(hexColor), "{:02X} ", hexBuffer[byteIndex]);
			} else {
				fmt::print(fmt::fg(hexColor), "   ");
			}
		}

		if (showAscii) {
			std::transform(hexBuffer.begin(), hexBuffer.begin() + bytes, asciiBuffer.begin(), [](char c) {
				return std::isprint(c) ? c : '.';
			});
			asciiBuffer[bytes] = '\0';

			fmt::print(fmt::fg(asciiColor), " |{0:<{1}}|", asciiBuffer.data(), chunkSize);
		}

		std::cout << std::endl;
	}

	file.close();

	return true;
}

}  // namespace dd