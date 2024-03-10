#pragma once
#include "Audio.hpp"

namespace Bot {
	extern const char* Token;

	void Join(const dpp::slashcommand_t& event) noexcept;
	void Leave(const dpp::slashcommand_t& event) noexcept;
	void Play(std::vector<Audio::audioInfoType>& arr, const dpp::slashcommand_t& event) noexcept;
	void Stop(const dpp::slashcommand_t& event) noexcept;
	void List(std::vector<Audio::audioInfoType>& arr, const dpp::slashcommand_t& event) noexcept;
	void Load(std::vector<Audio::audioInfoType>& arr, const dpp::slashcommand_t& event) noexcept;
}