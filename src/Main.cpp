#include "Bot.hpp"

int main() {
	std::vector<Audio::audioInfoType> audios;
	loadFilesInCurrentDirectory(audios);

	dpp::cluster bot(Bot::Token);
	bot.on_log(dpp::utility::cout_logger());

	bot.on_slashcommand([&bot, &audios](const dpp::slashcommand_t& event) {
		if (event.command.get_command_name() == "join") { Bot::Join(event);	}
		else if (event.command.get_command_name() == "leave") { Bot::Leave(event); }
		else if (event.command.get_command_name() == "play") { Bot::Play(audios, event); }
		else if (event.command.get_command_name() == "stop") { Bot::Stop(event); }
		else if (event.command.get_command_name() == "list") { Bot::List(audios, event); }
		else if (event.command.get_command_name() == "load") { Bot::Load(audios, event); }
	});

	bot.on_ready([&bot](const dpp::ready_t& event) {
		if (dpp::run_once<struct register_bot_commands>()) {
			bot.set_presence(dpp::presence(dpp::ps_online, dpp::at_custom, "Playing music!"));

			dpp::slashcommand joincommand("join", "Joins the channel you are in.", bot.me.id);
			dpp::slashcommand leavecommand("leave", "Lists available audio files.", bot.me.id);
			dpp::slashcommand playcommand("play", "Plays an audio file.", bot.me.id);
			playcommand.add_option(dpp::command_option(dpp::co_integer, "fileid", "File ID to play. Get ID by using /list command.", true));
			dpp::slashcommand stopcommand("stop", "Stops playing the audio file.", bot.me.id);
			dpp::slashcommand listcommand("list", "Lists available audio files.", bot.me.id);
			dpp::slashcommand loadcommand("load", "Loads another file.", bot.me.id);
			loadcommand.add_option(dpp::command_option(dpp::co_string, "filename", "Name of file to load.", true));

			bot.global_bulk_command_create({joincommand, leavecommand, playcommand, stopcommand, listcommand, loadcommand});
		}
		});

	bot.start(dpp::st_wait);

	Audio::cleanup(audios);

	return 0;
}