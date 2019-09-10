///////////////////////////////////
#define _POSIX_C_SOURCE 200809L  //
///////////////////////////////////


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <locale.h>
#include <mpv/client.h>

#include "shared.h"
#include "network.h"
#include "json_parser.h"
#include "config.h"
#include "disk_io.h"


#define JF_MPV_ERROR_FATAL(status)													\
do {																				\
	if (status < 0) {																\
		fprintf(stderr, "FATAL: mpv API error: %s\n", mpv_error_string(status));	\
		mpv_terminate_destroy(g_mpv_ctx);											\
		exit(EXIT_FAILURE);															\
	}																				\
} while (false);


////////// GLOBALS //////////
jf_options g_options;
jf_global_state g_state;
mpv_handle *g_mpv_ctx = NULL;
/////////////////////////////


static mpv_handle *jf_mpv_context_new(void);


mpv_handle *jf_mpv_context_new()
{
	mpv_handle *ctx;
	int mpv_flag_yes = 1;
	char *x_emby_token;

	if ((ctx = mpv_create()) == NULL) {
		fprintf(stderr, "FATAL: failed to create mpv context.\n");
		return NULL;
	}
	JF_MPV_ERROR_FATAL(mpv_set_option(ctx, "config", MPV_FORMAT_FLAG, &mpv_flag_yes));
	JF_MPV_ERROR_FATAL(mpv_set_option(ctx, "osc", MPV_FORMAT_FLAG, &mpv_flag_yes));
	JF_MPV_ERROR_FATAL(mpv_set_option(ctx, "input-default-bindings", MPV_FORMAT_FLAG, &mpv_flag_yes));
	JF_MPV_ERROR_FATAL(mpv_set_option(ctx, "input-vo-keyboard", MPV_FORMAT_FLAG, &mpv_flag_yes));
	JF_MPV_ERROR_FATAL(mpv_set_option(ctx, "input-terminal", MPV_FORMAT_FLAG, &mpv_flag_yes));
	JF_MPV_ERROR_FATAL(mpv_set_option(ctx, "terminal", MPV_FORMAT_FLAG, &mpv_flag_yes));
	if ((x_emby_token = jf_concat(2, "x-emby-token: ", g_options.token)) == NULL) {
		fprintf(stderr, "FATAL: jf_concat for x-emby-token header field for mpv requests returned NULL.\n");
	}
	JF_MPV_ERROR_FATAL(mpv_set_option_string(ctx, "http-header-fields", x_emby_token));
	free(x_emby_token);
	JF_MPV_ERROR_FATAL(mpv_observe_property(ctx, 0, "time-pos", MPV_FORMAT_INT64));

	JF_MPV_ERROR_FATAL(mpv_initialize(ctx));

	return ctx;
}


int main(int argc, char *argv[])
{
	// VARIABLES
	char *config_path;
	mpv_event *event;
	int mpv_flag_yes = 1, mpv_flag_no = 0;

	// LIBMPV VERSION CHECK
	// required for "osc" option
	if (mpv_client_api_version() < MPV_MAKE_VERSION(1,23)) {
		fprintf(stderr, "FATAL: found libmpv version %lu.%lu, but 1.23 or greater is required.\n", mpv_client_api_version() >> 16, mpv_client_api_version() & 0xFFFF);
		exit(EXIT_FAILURE);
	}
	///////////////////////


	// VARIABLES INIT
	jf_options_init();
	atexit(jf_options_clear);
	jf_menu_init();
	atexit(jf_menu_clear);
	if (! jf_global_state_init()) {
		exit(EXIT_FAILURE);
	}
	atexit(jf_global_state_clear);
	jf_disk_init();
	atexit(jf_disk_clear);
	/////////////////
	
	
	// TODO command line arguments
	

	// SETUP NETWORK
	// NB the network unit keeps a reference to options, not a copy. Keep it live till cleanup!
	if (! jf_network_pre_init()) {
		fprintf(stderr, "FATAL: could not initialize network context.\n");
		exit(EXIT_FAILURE);
	}
	atexit(jf_network_clear);
	////////////////
	

	// READ AND PARSE CONFIGURATION FILE
	// apply config directory location default unless there was user override
	if (g_state.config_dir == NULL) {
		if ((g_state.config_dir = jf_config_get_default_dir()) == NULL) {
			JF_STATIC_PRINT_ERROR("FATAL: could not acquire configuration directory location. $HOME could not be read and --config-dir was not passed.\n");
			exit(EXIT_FAILURE);
		}
	}

	// get location of config file
	if ((config_path = jf_concat(2, g_state.config_dir, "/options")) == NULL) {
		JF_STATIC_PRINT_ERROR("FATAL: could not compute config_path.\n");
		exit(EXIT_FAILURE);
	}

	// check config file exists
	errno = 0;
	if (access(config_path, F_OK) == 0) {
		// it's there: read it
		if (! jf_config_read(config_path)) {
			free(config_path);
			exit(EXIT_FAILURE);
		}
		// if server, userid or token are missing (may happen if the config file was edited badly)
		if (g_options.server == NULL || g_options.userid == NULL || g_options.token == NULL) {
			jf_user_config();
		}
	} else if (errno == ENOENT || errno == ENOTDIR) {
		// it's not there
		jf_user_config();
	} else {
		int access_errno = errno;
		JF_STATIC_PRINT_ERROR("FATAL access for config file at location ");
		write(2, config_path, strlen(config_path));
		JF_STATIC_PRINT_ERROR(": ");
		write(2, strerror(access_errno), strlen(strerror(access_errno)));
		JF_STATIC_PRINT_ERROR("\n");
		free(config_path);
		exit(EXIT_FAILURE);
	}
	////////////////////////////////////
	
	// DOUBLE CHECK AND FINALIZE NETWORK CONFIG
	if (! jf_network_refresh()) {
		free(config_path);
		exit(EXIT_FAILURE);
	}
	// TODO ping server (and get name)
	g_state.server_name = strdup("TEST SERVER"); // placeholder; we will somehow need to send it to the menu TU
	// TODO check token still valid, prompt relogin otherwise
	
	// COMMIT CONFIG TO DISK
	jf_config_write(config_path);
	free(config_path);
	//////////////////////////////////////////


	// SETUP MPV
	if (setlocale(LC_NUMERIC, "C") == NULL) {
		fprintf(stderr, "WARNING: could not set numeric locale to sane standard. mpv might refuse to work.\n");
	}

	if ((g_mpv_ctx = jf_mpv_context_new()) == NULL) {
		exit(EXIT_FAILURE);
	}
	atexit(jf_mpv_clear);
	////////////


	////////// MAIN LOOP //////////
	while (true) {
		switch (g_state.state) {
			// HANDLE SHUTDOWN
			case JF_STATE_USER_QUIT:
				exit(EXIT_SUCCESS);
				break;
			case JF_STATE_FAIL:
				exit(EXIT_FAILURE);
				break;
			// RUNTIME: READ AND PROCESS EVENTS
			default:
				event = mpv_wait_event(g_mpv_ctx, -1);
				printf("DEBUG: event: %s\n", mpv_event_name(event->event_id));
				switch (event->event_id) {
					case MPV_EVENT_CLIENT_MESSAGE:
						// playlist controls
						if (((mpv_event_client_message *)event->data)->num_args > 0) {
							if (strcmp(((mpv_event_client_message *)event->data)->args[0], "jftui-playlist-next") == 0) {
								jf_menu_playlist_forward();
							} else if (strcmp(((mpv_event_client_message *)event->data)->args[0], "jftui-playlist-prev") == 0) {
								jf_menu_playlist_backward();
							}
						}
						break;
					case MPV_EVENT_END_FILE:
						if (((mpv_event_end_file *)event->data)->reason == MPV_END_FILE_REASON_EOF) {
							if (jf_menu_playlist_forward()) {
								g_state.state = JF_STATE_PLAYBACK_NAVIGATING;
							}
						}
						break;
					case MPV_EVENT_SEEK:
						if (g_state.state == JF_STATE_PLAYBACK_START_MARK) {
							mpv_set_property_string(g_mpv_ctx, "start", "none");
							g_state.state = JF_STATE_PLAYBACK;
						}
						break;
					case MPV_EVENT_PROPERTY_CHANGE:
						if (strcmp("time-pos", ((mpv_event_property *)event->data)->name) != 0) break;
						if (((mpv_event_property *)event->data)->format == MPV_FORMAT_NONE) break;
						// event valid, check if need to update the server
						int64_t playback_ticks = JF_SECS_TO_TICKS(*(int64_t *)((mpv_event_property *)event->data)->data);
						if (llabs(playback_ticks - g_state.currently_playing.playback_ticks) < JF_SECS_TO_TICKS(10)) break;
						if (playback_ticks < 0.05 * g_state.currently_playing.runtime_ticks) break;
						// close to end, mark played unless last update already did so
						if (playback_ticks > 0.95 * g_state.currently_playing.runtime_ticks
								&& g_state.currently_playing.playback_ticks < 0.95 * g_state.currently_playing.runtime_ticks) {
							if (jf_menu_mark_played(&g_state.currently_playing)) {
								g_state.currently_playing.playback_ticks = playback_ticks;
							}
							printf("DEBUG: marked as played.\n");
							break;
						}
						// good for update
						char *post = jf_json_generate_progress_post(g_state.currently_playing.id, playback_ticks);
						jf_reply *reply = jf_request("/sessions/playing/progress", JF_REQUEST_IN_MEMORY, post);
						free(post);
						if (reply == NULL) {
							fprintf(stderr, "Error: progress update jf_request returned NULL.\n");
							break;
						}
						if (JF_REPLY_PTR_HAS_ERROR(reply)) {
							fprintf(stderr, "Error: progress update jf_request: %s.\n", jf_reply_error_string(reply));
						} else {
							g_state.currently_playing.playback_ticks = playback_ticks;
							printf("DEBUG: successful progress update\n");
						}
						jf_reply_free(reply);
						break;
					case MPV_EVENT_IDLE:
						if (g_state.state == JF_STATE_PLAYBACK_NAVIGATING) {
							// digest idle event while we move to the next track
							g_state.state = JF_STATE_PLAYBACK;
						} else {
							// go into UI mode
							g_state.state = JF_STATE_MENU_UI;
							JF_MPV_ERROR_FATAL(mpv_set_property(g_mpv_ctx, "terminal", MPV_FORMAT_FLAG, &mpv_flag_no));
							while (g_state.state == JF_STATE_MENU_UI) jf_menu_ui();
							JF_MPV_ERROR_FATAL(mpv_set_property(g_mpv_ctx, "terminal", MPV_FORMAT_FLAG, &mpv_flag_yes));
						}
						break;
					case MPV_EVENT_SHUTDOWN:
						// it is unfortunate, but the cleanest way to handle this case
						// (which is when mpv receives a "quit" command)
						// is to comply and create a new context
						mpv_terminate_destroy(g_mpv_ctx);
						if ((g_mpv_ctx = jf_mpv_context_new()) == NULL) {
							exit(EXIT_FAILURE);
						}
						break;
					default:
						// no-op on everything else
						break;
				}
		}
	}
	///////////////////////////////


	// never reached
	exit(EXIT_SUCCESS);
}
