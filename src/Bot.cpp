#include "Bot.hpp"

namespace Bot {
	const char* Token = "TODO";
	
	void Join(const dpp::slashcommand_t& event) noexcept {
		dpp::guild* g = dpp::find_guild(event.command.guild_id);

		if (!g->connect_member_voice(event.command.get_issuing_user().id)) {
			event.reply("You don't seem to be in a voice channel!");
			return;
		}

		event.reply(dpp::message("Joined your channel!").set_flags(dpp::m_ephemeral));
	}
	void Leave(const dpp::slashcommand_t& event) noexcept {
		dpp::voiceconn* v = event.from->get_voice(event.command.guild_id);
		if (!v || !v->voiceclient || !v->voiceclient->is_ready()) {
			event.reply("Please add this bot to a voice channel first (/join)."); return;
		}

		v->voiceclient->send_close_packet();

		event.reply(dpp::message("Bot has left the channel.").set_flags(dpp::m_ephemeral));
	}
	void Play(std::vector<Audio::audioInfoType>& arr, const dpp::slashcommand_t& event) noexcept {
		dpp::voiceconn* v = event.from->get_voice(event.command.guild_id);
		if (!v || !v->voiceclient || !v->voiceclient->is_ready()) {
			event.reply("Please add this bot to a voice channel first (/join)."); return;
		}

		uint64_t musicId = std::get<int64_t>(event.get_parameter("fileid"));

		event.reply(dpp::message("Played the audio file.").set_flags(dpp::m_ephemeral));
		
		std::cout << "Play command: Id " << musicId << "\n";
		v->voiceclient->stop_audio();
		v->voiceclient->send_audio_raw((uint16_t*)(arr[musicId].data), (arr[musicId].bytesize));
	}
	void Stop(const dpp::slashcommand_t& event) noexcept {
		dpp::voiceconn* v = event.from->get_voice(event.command.guild_id);
		if (!v || !v->voiceclient || !v->voiceclient->is_ready()) {
			event.reply("Please add this bot to a voice channel."); return;
		}

		v->voiceclient->stop_audio();

		event.reply(dpp::message("Stopped all audio.").set_flags(dpp::m_ephemeral));
	}
	void List(std::vector<Audio::audioInfoType>& arr, const dpp::slashcommand_t& event) noexcept {
		std::string text;
		text.append("List of available music:\n");
		for (uint64_t i = 0; i < arr.size(); i++) {
			text.append(std::to_string(i)).append("): ").append(arr[i].filename).push_back('\n');
		}

		std::cout << text << "\n";
		event.reply(dpp::message(text).set_flags(dpp::m_ephemeral));
	}
	void Load(std::vector<Audio::audioInfoType>& arr, const dpp::slashcommand_t& event) noexcept {
		std::string filename = std::get<std::string>(event.get_parameter("filename"));

		Audio::audioInfoType temp = Audio::loadFile(filename);

		if (temp.data == nullptr) {
			event.reply(dpp::message("Failed to load " + filename).set_flags(dpp::m_ephemeral));
			return;
		}

		arr.push_back(temp);

		std::cout << "Loaded: " << filename << "\n";
		event.reply(dpp::message("Loaded " + filename).set_flags(dpp::m_ephemeral));
	}
}