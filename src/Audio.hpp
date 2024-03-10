#pragma once
#include "Include.hpp"

namespace Audio {
	struct audioInfoType {
		void* data;
		std::string filename;
		uint64_t framesno;
		uint64_t bytesize;

		audioInfoType() {
			this->data = nullptr;
			this->filename = {};
			this->framesno = 0;
			this->bytesize = 0;
		}
	};

	audioInfoType loadFile(const std::string& aFilename) noexcept;
	void disposeOfFile(audioInfoType& aInfo) noexcept;
	void loadFilesInCurrentDirectory(std::vector<audioInfoType>& arr) noexcept;
	void cleanup(std::vector<audioInfoType>& arr);
}