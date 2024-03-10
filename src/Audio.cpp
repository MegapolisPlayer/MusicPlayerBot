#define MINIAUDIO_IMPLEMENTATION
#include "Audio.hpp"

namespace Audio {
	audioInfoType loadFile(const std::string& filename) noexcept {
		std::cout << "Loading audio file " << filename << "\n";

		audioInfoType temp;
		temp.filename = filename;

		if (!std::filesystem::exists(filename)) {
			temp.data = nullptr;
			return temp;
		};

		ma_result result;
		ma_decoder decoder;
		ma_decoder_config config = ma_decoder_config_init(ma_format_s16, 2, 48000);

		result = ma_decoder_init_file(temp.filename.c_str(), &config, &decoder);
		if (result != MA_SUCCESS) {
			std::cout << "Could not initialize Miniaudio. Error: " << result << "\n"; std::exit(-1);
		}

		//some maths
		temp.framesno = 0;
		ma_decoder_get_length_in_pcm_frames(&decoder, &temp.framesno);
		temp.data = malloc(temp.framesno * ma_get_bytes_per_frame(ma_format_s16, 2));
		temp.bytesize = temp.framesno * ma_get_bytes_per_frame(ma_format_s16, 2);
		uint64_t frames_remaining = temp.framesno;
		uint64_t frames_read = 0;

		std::cout << "Total " << temp.framesno << " PCM frames to read.\n";

		//main reading loop
		while (frames_remaining > 0) {
			uint64_t read = ma_decoder_read_pcm_frames(
				&decoder, OFFSET_PTR(temp.data, (temp.framesno - frames_remaining) * ma_get_bytes_per_frame(ma_format_s16, 2)), temp.framesno, &frames_read
			);
			if (read < frames_remaining) {
				ma_decoder_seek_to_pcm_frame(&decoder, 0);
			}
			frames_remaining -= frames_read;
			std::cout << frames_remaining << " remaining." << std::endl;
		}

		std::cout << "Pointer to beggining: " << temp.data << "\n";
		ma_decoder_uninit(&decoder);
		return temp;
	}
	void disposeOfFile(audioInfoType& aInfo) noexcept {
		free(aInfo.data);
	}

	void loadFilesInCurrentDirectory(std::vector<audioInfoType>& arr) noexcept {
		for (const auto& a : std::filesystem::directory_iterator("audio/")) {
			if (
				a.path().extension() == ".mp3" ||
				a.path().extension() == ".wav" ||
				a.path().extension() == ".ogg"
				) {
				std::cout << "File found: " << a.path().string() << "\n";
				arr.push_back(loadFile(a.path().string()));
			}
		}
	}

	void cleanup(std::vector<audioInfoType>& arr) {
		for (auto& a : arr) {
			disposeOfFile(a);
		}
	}
}