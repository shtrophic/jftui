#include "json_parser.h"

////////// GLOBALS //////////
extern jf_options g_options;
/////////////////////////////


////////// STATIC VARIABLES //////////
static char s_error_buffer[JF_PARSER_ERROR_BUFFER_SIZE];
//////////////////////////////////////


////////// STATIC FUNCTIONS //////////
static int sax_items_start_map(void *ctx);
static int sax_items_end_map(void *ctx);
static int sax_items_map_key(void *ctx, const unsigned char *key, size_t key_len);
static int sax_items_start_array(void *ctx);
static int sax_items_end_array(void *ctx);
static int sax_items_string(void *ctx, const unsigned char *string, size_t strins_len);
static int sax_items_number(void *ctx, const char *string, size_t strins_len);

// Function: jf_yajl_parser_new
//
// Allocates a new yajl parser instance, registering callbacks and context and setting yajl_allow_multiple_values to let it digest multiple JSON messages in a row.
// Failures are considered catastrophic. Will set thread buffer state to JF_THREAD_BUFFER_STATE_PARSER_DEAD and put an error message in the thread buffer data buffer.
//
// Parameters:
// 	- callbacks: Pointer to callbacks struct to register.
// 	- context: Pointer to json parser context to register.
//
// Returns:
// 	The yajl_handle of the new parser on success, NULL on failure.
static yajl_handle jf_yajl_parser_new(yajl_callbacks *callbacks, jf_sax_context *context);

static void jf_sax_context_init(jf_sax_context *context, jf_thread_buffer *tb);
static void jf_sax_context_current_item_clear(jf_sax_context *context);
static bool jf_sax_context_current_item_copy(jf_sax_context *context);
//////////////////////////////////////


////////// SAX PARSER CALLBACKS //////////
static int sax_items_start_map(void *ctx)
{
	jf_sax_context *context = (jf_sax_context *)(ctx);
	switch (context->parser_state) {
		case JF_SAX_IDLE:
			context->tb->item_count = 0;
			jf_sax_context_current_item_clear(context);
			jf_disk_refresh();
			context->parser_state = JF_SAX_IN_QUERYRESULT_MAP;
			break;
		case JF_SAX_IN_LATEST_ARRAY:
			context->latest_array = true;
			context->parser_state = JF_SAX_IN_ITEM_MAP;
			break;
		case JF_SAX_IN_ITEMS_ARRAY:
			context->parser_state = JF_SAX_IN_ITEM_MAP;
			break;
		case JF_SAX_IN_USERDATA_VALUE:
			context->parser_state = JF_SAX_IN_USERDATA_MAP;
			break;
		case JF_SAX_IN_ITEM_MAP:
			context->state_to_resume = JF_SAX_IN_ITEM_MAP;
			context->parser_state = JF_SAX_IGNORE;
			context->maps_ignoring = 1;
			break;
		case JF_SAX_IGNORE:
			context->maps_ignoring++;
			break;
	}
	return 1;
}


static int sax_items_end_map(void *ctx)
{
	jf_sax_context *context = (jf_sax_context *)(ctx);
	switch (context->parser_state) {
		case JF_SAX_IN_QUERYRESULT_MAP:
			context->parser_state = JF_SAX_IDLE;
			break;
		case JF_SAX_IN_ITEMS_VALUE:
			context->parser_state = JF_SAX_IN_QUERYRESULT_MAP;
			break;
		case JF_SAX_IN_USERDATA_MAP:
			context->parser_state = JF_SAX_IN_ITEM_MAP;
			break;
		case JF_SAX_IN_ITEM_MAP:
			context->tb->item_count++;
			jf_growing_buffer_empty(context->current_item_display_name);
			switch (context->current_item_type) {
				case JF_ITEM_TYPE_AUDIO:
				case JF_ITEM_TYPE_AUDIOBOOK:
					JF_SAX_PRINT_LEADER("T");
					if (context->tb->promiscuous_context) {
						JF_SAX_TRY_APPEND_NAME("", artist, " - ");
						JF_SAX_TRY_APPEND_NAME("", album, " - ");
					}
					jf_growing_buffer_append(context->current_item_display_name,
							context->name, context->name_len);
					break;
				case JF_ITEM_TYPE_ALBUM:
					JF_SAX_PRINT_LEADER("D");
					if (context->tb->promiscuous_context) {
						JF_SAX_TRY_APPEND_NAME("", artist, " - ");
					}
					jf_growing_buffer_append(context->current_item_display_name,
							context->name, context->name_len);
					JF_SAX_TRY_APPEND_NAME(" (", year, ")");
					break;
				case JF_ITEM_TYPE_EPISODE:
					JF_SAX_PRINT_LEADER("V");
					if (context->tb->promiscuous_context) {
						JF_SAX_TRY_APPEND_NAME("", series, " - ");
						JF_SAX_TRY_APPEND_NAME("S", parent_index, "");
						JF_SAX_TRY_APPEND_NAME("E", index, " ");
					}
					jf_growing_buffer_append(context->current_item_display_name,
							context->name, context->name_len);
					break;
				case JF_ITEM_TYPE_SEASON:
					JF_SAX_PRINT_LEADER("D");
					if (context->tb->promiscuous_context) {
						JF_SAX_TRY_APPEND_NAME("", series, " - ");
					}
					jf_growing_buffer_append(context->current_item_display_name,
							context->name, context->name_len);
					break;
				case JF_ITEM_TYPE_MOVIE:
					JF_SAX_PRINT_LEADER("V");
					jf_growing_buffer_append(context->current_item_display_name,
							context->name, context->name_len);
					JF_SAX_TRY_APPEND_NAME(" (", year, ")");
					break;
				case JF_ITEM_TYPE_ARTIST:
				case JF_ITEM_TYPE_SERIES:
				case JF_ITEM_TYPE_PLAYLIST:
				case JF_ITEM_TYPE_FOLDER:
				case JF_ITEM_TYPE_COLLECTION:
				case JF_ITEM_TYPE_COLLECTION_MUSIC:
				case JF_ITEM_TYPE_COLLECTION_SERIES:
				case JF_ITEM_TYPE_COLLECTION_MOVIES:
				case JF_ITEM_TYPE_USER_VIEW:
					JF_SAX_PRINT_LEADER("D");
					jf_growing_buffer_append(context->current_item_display_name,
							context->name, context->name_len);
					break;
			}

			// FINALIZE PRINT
			jf_growing_buffer_append(context->current_item_display_name, "", 1);
			printf("%s\n", context->current_item_display_name->buf);

			// SAVE AND CLEAR ITEM
			jf_menu_item *item = jf_menu_item_new(context->current_item_type, NULL,
					(const char*)context->id, context->current_item_display_name->buf,
					context->ticks);
			jf_disk_payload_add_item(item);
			jf_menu_item_free(item);
			jf_sax_context_current_item_clear(context);

			if (context->latest_array) {
				context->parser_state = JF_SAX_IN_LATEST_ARRAY;
				context->latest_array = false;
			} else {
				context->parser_state = JF_SAX_IN_ITEMS_ARRAY;
			}
			break;
		case JF_SAX_IGNORE:
			context->maps_ignoring--;
			if (context->maps_ignoring == 0 && context->arrays_ignoring == 0) {
				context->parser_state = context->state_to_resume;
				context->state_to_resume = JF_SAX_NO_STATE;
			}
	}
	return 1;
}


static int sax_items_map_key(void *ctx, const unsigned char *key, size_t key_len)
{
	jf_sax_context *context = (jf_sax_context *)(ctx);
	switch (context->parser_state) {
		case JF_SAX_IN_QUERYRESULT_MAP:
			if (JF_SAX_KEY_IS("Items")) {
				context->parser_state = JF_SAX_IN_ITEMS_VALUE;
			}
			break;
		case JF_SAX_IN_ITEM_MAP:
			if (JF_SAX_KEY_IS("Name")) {
				context->parser_state = JF_SAX_IN_ITEM_NAME_VALUE;
			} else if (JF_SAX_KEY_IS("Type")) {
				context->parser_state = JF_SAX_IN_ITEM_TYPE_VALUE;
			} else if (JF_SAX_KEY_IS("CollectionType")) {
				context->parser_state = JF_SAX_IN_ITEM_COLLECTION_TYPE_VALUE;
			} else if (JF_SAX_KEY_IS("Id")) {
				context->parser_state = JF_SAX_IN_ITEM_ID_VALUE;
			} else if (JF_SAX_KEY_IS("Artists")) {
				context->parser_state = JF_SAX_IN_ITEM_ARTISTS_VALUE;
			} else if (JF_SAX_KEY_IS("Album")) {
				context->parser_state = JF_SAX_IN_ITEM_ALBUM_VALUE;
			} else if (JF_SAX_KEY_IS("SeriesName")) {
				context->parser_state = JF_SAX_IN_ITEM_SERIES_VALUE;
			} else if (JF_SAX_KEY_IS("ProductionYear")) {
				context->parser_state = JF_SAX_IN_ITEM_YEAR_VALUE;
			} else if (JF_SAX_KEY_IS("IndexNumber")) {
				context->parser_state = JF_SAX_IN_ITEM_INDEX_VALUE;
			} else if (JF_SAX_KEY_IS("ParentIndexNumber")) {
				context->parser_state = JF_SAX_IN_ITEM_PARENT_INDEX_VALUE;
			} else if (JF_SAX_KEY_IS("UserData")) {
				context->parser_state = JF_SAX_IN_USERDATA_VALUE;
			}
			break;
		case JF_SAX_IN_USERDATA_MAP:
			if (JF_SAX_KEY_IS("PlaybackPositionTicks")) {
				context->parser_state = JF_SAX_IN_USERDATA_TICKS_VALUE;
			}
	}
	return 1;
}

static int sax_items_start_array(void *ctx)
{
	jf_sax_context *context = (jf_sax_context *)(ctx);
	switch (context->parser_state) {
		case JF_SAX_IDLE:
			context->parser_state = JF_SAX_IN_LATEST_ARRAY;
			context->tb->item_count = 0;
			jf_sax_context_current_item_clear(context);
			break;
		case JF_SAX_IN_ITEMS_VALUE:
			context->parser_state = JF_SAX_IN_ITEMS_ARRAY;
			break;
		case JF_SAX_IN_ITEM_ARTISTS_VALUE:
			context->parser_state = JF_SAX_IN_ITEM_ARTISTS_ARRAY;
			break;
		case JF_SAX_IN_ITEM_MAP:
			context->parser_state = JF_SAX_IGNORE;
			context->state_to_resume = JF_SAX_IN_ITEM_MAP;
			context->arrays_ignoring = 1;
			break;
		case JF_SAX_IGNORE:
			context->arrays_ignoring++;
			break;
	}
	return 1;
}


static int sax_items_end_array(void *ctx)
{
	jf_sax_context *context = (jf_sax_context *)(ctx);
	switch (context->parser_state) {
		case JF_SAX_IN_LATEST_ARRAY:
			context->parser_state = JF_SAX_IDLE;
			break;
		case JF_SAX_IN_ITEMS_ARRAY:
			context->parser_state = JF_SAX_IN_QUERYRESULT_MAP;
			break;
		case JF_SAX_IN_ITEM_ARTISTS_ARRAY:
			context->parser_state = JF_SAX_IN_ITEM_MAP;
			break;
		case JF_SAX_IGNORE:
			context->arrays_ignoring--;
			if (context->arrays_ignoring == 0 && context->maps_ignoring == 0) {
				context->parser_state = context->state_to_resume;
				context->state_to_resume = JF_SAX_NO_STATE;
			}
			break;
	}
	return 1;
}


static int sax_items_string(void *ctx, const unsigned char *string, size_t string_len)
{
	jf_sax_context *context = (jf_sax_context *)(ctx);
	switch (context->parser_state) {
		case JF_SAX_IN_ITEM_NAME_VALUE:
			JF_SAX_ITEM_FILL(name);
			context->parser_state = JF_SAX_IN_ITEM_MAP;
			break;
		case JF_SAX_IN_ITEM_TYPE_VALUE:
			if (JF_SAX_STRING_IS("CollectionFolder")
					&& context->current_item_type == JF_ITEM_TYPE_NONE) {
				// don't overwrite if we already got more specific information
				context->current_item_type = JF_ITEM_TYPE_COLLECTION;
			} else if (JF_SAX_STRING_IS("Folder") || JF_SAX_STRING_IS("UserView")
					|| JF_SAX_STRING_IS("Playlist") || JF_SAX_STRING_IS("PlaylistsFolder")) {
				context->current_item_type = JF_ITEM_TYPE_FOLDER;
			} else if (JF_SAX_STRING_IS("Audio")) {
				context->current_item_type = JF_ITEM_TYPE_AUDIO;
			} else if (JF_SAX_STRING_IS("Artist") || JF_SAX_STRING_IS("MusicArtist")) {
				context->current_item_type = JF_ITEM_TYPE_ARTIST;
			} else if (JF_SAX_STRING_IS("MusicAlbum")) {
				context->current_item_type = JF_ITEM_TYPE_ALBUM;
			} else if (JF_SAX_STRING_IS("Episode")) {
				context->current_item_type = JF_ITEM_TYPE_EPISODE;
			} else if (JF_SAX_STRING_IS("Season")) {
				context->current_item_type = JF_ITEM_TYPE_SEASON;
			} else if (JF_SAX_STRING_IS("Series")) {
				context->current_item_type = JF_ITEM_TYPE_SERIES;
			} else if (JF_SAX_STRING_IS("Movie")) {
				context->current_item_type = JF_ITEM_TYPE_MOVIE;
			} else if (JF_SAX_STRING_IS("AudioBook")) {
				context->current_item_type = JF_ITEM_TYPE_AUDIOBOOK;
			}
			context->parser_state = JF_SAX_IN_ITEM_MAP;
			break;
		case JF_SAX_IN_ITEM_COLLECTION_TYPE_VALUE:
			if (JF_SAX_STRING_IS("music")) {
				context->current_item_type = JF_ITEM_TYPE_COLLECTION_MUSIC;
			} else if (JF_SAX_STRING_IS("tvshows")) {
				context->current_item_type = JF_ITEM_TYPE_COLLECTION_SERIES;
			} else if (JF_SAX_STRING_IS("movies") || JF_SAX_STRING_IS("homevideos")
					|| JF_SAX_STRING_IS("musicvideos")) {
				context->current_item_type = JF_ITEM_TYPE_COLLECTION_MOVIES;
			} else if (JF_SAX_STRING_IS("folders")) {
				context->current_item_type = JF_ITEM_TYPE_FOLDER;
			}
			context->parser_state = JF_SAX_IN_ITEM_MAP;
			break;
		case JF_SAX_IN_ITEM_ID_VALUE:
			JF_SAX_ITEM_FILL(id);
			context->parser_state = JF_SAX_IN_ITEM_MAP;
			break;
		case JF_SAX_IN_ITEM_ARTISTS_ARRAY:
			// TODO we're effectively keeping only the last one of the list: review how reasonable this is
			JF_SAX_ITEM_FILL(artist);
			break;
		case JF_SAX_IN_ITEM_ALBUM_VALUE:
			JF_SAX_ITEM_FILL(album);
			context->parser_state = JF_SAX_IN_ITEM_MAP;
			break;
		case JF_SAX_IN_ITEM_SERIES_VALUE:
			JF_SAX_ITEM_FILL(series);
			context->parser_state = JF_SAX_IN_ITEM_MAP;
			break;
	}
	return 1;
}


static int sax_items_number(void *ctx, const char *string, size_t string_len)
{
	jf_sax_context *context = (jf_sax_context *)(ctx);
	switch (context->parser_state) {
		case JF_SAX_IN_USERDATA_TICKS_VALUE:
			context->ticks = strtoll(string, NULL, 10);
			context->parser_state = JF_SAX_IN_USERDATA_MAP;
			break;
		case JF_SAX_IN_ITEM_YEAR_VALUE:
			JF_SAX_ITEM_FILL(year);
			context->parser_state = JF_SAX_IN_ITEM_MAP;
			break;
		case JF_SAX_IN_ITEM_INDEX_VALUE:
			JF_SAX_ITEM_FILL(index);
			context->parser_state = JF_SAX_IN_ITEM_MAP;
			break;
		case JF_SAX_IN_ITEM_PARENT_INDEX_VALUE:
			JF_SAX_ITEM_FILL(parent_index);
			context->parser_state = JF_SAX_IN_ITEM_MAP;
			break;
	}
	return 1;
}
//////////////////////////////////////////


static yajl_handle jf_yajl_parser_new(yajl_callbacks *callbacks, jf_sax_context *context)
{
	yajl_handle parser;

	if ((parser = yajl_alloc(callbacks, NULL, (void *)(context))) == NULL) {
		context->tb->state = JF_THREAD_BUFFER_STATE_PARSER_DEAD;
		strcpy(context->tb->data, "sax parser yajl_alloc failed");
		return NULL;
	}

	// allow persistent parser to digest many JSON objects
	if (yajl_config(parser, yajl_allow_multiple_values, 1) == 0) {
		yajl_free(parser);
		context->tb->state = JF_THREAD_BUFFER_STATE_PARSER_DEAD;
		strcpy(context->tb->data, "sax parser could not allow_multiple_values");
		return NULL;
	}

	return parser;
}


static void jf_sax_context_init(jf_sax_context *context, jf_thread_buffer *tb)
{
	context->parser_state = JF_SAX_IDLE;
	context->state_to_resume = JF_SAX_NO_STATE;
	context->maps_ignoring = 0;
	context->arrays_ignoring = 0;
	context->latest_array = false;
	context->tb = tb;
	context->current_item_type = JF_ITEM_TYPE_NONE;
	context->copy_buffer = NULL;
	context->current_item_display_name = jf_growing_buffer_new(0);
	context->name = context->id = context->artist = context->album = NULL;
	context->series = context->year = context->index = context->parent_index = NULL;
	context->name_len = context->id_len = context->artist_len = 0;
	context->album_len = context->series_len = context->year_len = 0;
	context->index_len = context->parent_index_len = 0;
	context->ticks = 0;
}


static void jf_sax_context_current_item_clear(jf_sax_context *context)
{
	context->current_item_type = JF_ITEM_TYPE_NONE;
	context->name_len = 0;
	context->id_len = 0;
	context->artist_len = 0;
	context->album_len = 0;
	context->series_len = 0;
	context->year_len = 0;
	context->index_len = 0;
	context->parent_index_len = 0;

	free(context->copy_buffer);
	context->copy_buffer = NULL;
}


static bool jf_sax_context_current_item_copy(jf_sax_context *context)
{
	// allocate a contiguous buffer containing the copied values
	// then update the context pointers to point within it
	size_t item_size;
	size_t used = 0;
	item_size = (size_t)(context->name_len + context->id_len + context->artist_len
		+ context->album_len + context->series_len + context->year_len +
		context->index_len + context->parent_index_len);
	if ((context->copy_buffer = malloc(item_size)) != NULL) {
		JF_SAX_CONTEXT_COPY(name);
		JF_SAX_CONTEXT_COPY(id);
		JF_SAX_CONTEXT_COPY(artist);
		JF_SAX_CONTEXT_COPY(album);
		JF_SAX_CONTEXT_COPY(series);
		JF_SAX_CONTEXT_COPY(year);
		JF_SAX_CONTEXT_COPY(index);
		JF_SAX_CONTEXT_COPY(parent_index);
		return true;
	}
	return false;
}


// NB all data created by the thread itself is allocated on the stack,
// so it is safe to detach it
void *jf_sax_parser_thread(void *arg)
{
	jf_sax_context context;
	yajl_status status;
	yajl_handle parser;
	yajl_callbacks callbacks = {
		.yajl_null = NULL,
		.yajl_boolean = NULL,
		.yajl_integer = NULL,
		.yajl_double = NULL,
		.yajl_number = sax_items_number,
		.yajl_string = sax_items_string,
		.yajl_start_map = sax_items_start_map,
		.yajl_map_key = sax_items_map_key,
		.yajl_end_map = sax_items_end_map,
		.yajl_start_array = sax_items_start_array,
		.yajl_end_array = sax_items_end_array
	};

	jf_sax_context_init(&context, (jf_thread_buffer *)arg);

	if ((parser = jf_yajl_parser_new(&callbacks, &context)) == NULL) {
		return NULL;
	}

	pthread_mutex_lock(&context.tb->mut);
	while (true) {
		while (context.tb->state != JF_THREAD_BUFFER_STATE_PENDING_DATA) {
			pthread_cond_wait(&context.tb->cv_no_data, &context.tb->mut);
		}
		if ((status = yajl_parse(parser, (unsigned char*)context.tb->data, context.tb->used)) != yajl_status_ok) {
			unsigned char *error_str = yajl_get_error(parser, 1, (unsigned char*)context.tb->data, context.tb->used);
			strcpy(context.tb->data, "yajl_parse error: ");
			strncat(context.tb->data, (char *)error_str, JF_PARSER_ERROR_BUFFER_SIZE - strlen(context.tb->data));
			context.tb->state = JF_THREAD_BUFFER_STATE_PARSER_ERROR;
			pthread_mutex_unlock(&context.tb->mut);
			yajl_free_error(parser, error_str);
			// the parser never recovers after an error; we must free and reallocate it
			yajl_free(parser);
			if ((parser = jf_yajl_parser_new(&callbacks, &context)) == NULL) {
				return NULL;
			}
		} else if (context.parser_state == JF_SAX_IDLE) {
			// JSON fully parsed
			yajl_complete_parse(parser);
			context.tb->state = JF_THREAD_BUFFER_STATE_CLEAR;
		} else if (context.copy_buffer == NULL) {
			// we've still more to go, so we populate the copy buffer to not lose data
			// but if it is already filled from last time, filling it again would be unnecessary
			// and lead to a memory leak
			context.tb->state = JF_THREAD_BUFFER_STATE_AWAITING_DATA;
			if (! jf_sax_context_current_item_copy(&context)) {
				context.tb->state = JF_THREAD_BUFFER_STATE_PARSER_ERROR;
				strcpy(context.tb->data, "jf_sax_context_current_item_copy malloc fail");
				// we need to reset the parser
				yajl_free(parser);
				if ((parser = jf_yajl_parser_new(&callbacks, &context)) == NULL) {
					return NULL;
				}
			}
		} else {
			context.tb->state = JF_THREAD_BUFFER_STATE_AWAITING_DATA;
		}
		
		context.tb->used = 0;
		pthread_cond_signal(&context.tb->cv_has_data);
	}
}


char *jf_json_error_string()
{
	return s_error_buffer;
}


bool jf_json_parse_login_response(const char *payload)
{
	yajl_val parsed;
	const char *userid_selector[3] = { "User", "Id", NULL };
	const char *token_selector[2] = { "AccessToken", NULL };
	char *userid;
	char *token;

	s_error_buffer[0] = '\0';
	if ((parsed = yajl_tree_parse(payload, s_error_buffer, JF_PARSER_ERROR_BUFFER_SIZE)) == NULL) {
		if (s_error_buffer[0] == '\0') {
			strcpy(s_error_buffer, "yajl_tree_parse unknown error");
		}
		return false;
	}
	// NB macros propagate NULL
	userid = YAJL_GET_STRING(yajl_tree_get(parsed, userid_selector, yajl_t_string));
	token = YAJL_GET_STRING(yajl_tree_get(parsed, token_selector, yajl_t_string));
	if (userid != NULL && token != NULL) {
		g_options.userid = strdup(userid);
		g_options.token = strdup(token);
		yajl_tree_free(parsed);
		return true;
	} else {
		yajl_tree_free(parsed);
		return false;
	}
}


char *jf_json_make_login_request(const char *username, const char *password)
{
	yajl_gen gen;
	char *json = NULL;
	size_t json_len;

	if ((gen = yajl_gen_alloc(NULL)) == NULL) return (char *)NULL;
	JF_GEN_BAD_JUMP_OUT(yajl_gen_map_open(gen));
	JF_GEN_BAD_JUMP_OUT(yajl_gen_string(gen, (const unsigned char *)"Username", JF_STATIC_STRLEN("Username")));
	JF_GEN_BAD_JUMP_OUT(yajl_gen_string(gen, (const unsigned char *)username, strlen(username)));
	JF_GEN_BAD_JUMP_OUT(yajl_gen_string(gen, (const unsigned char *)"Pw", JF_STATIC_STRLEN("Pw")));
	JF_GEN_BAD_JUMP_OUT(yajl_gen_string(gen, (const unsigned char *)password, strlen(password)));
	JF_GEN_BAD_JUMP_OUT(yajl_gen_map_close(gen));
	JF_GEN_BAD_JUMP_OUT(yajl_gen_get_buf(gen, (const unsigned char **)&json, &json_len));
	json = strndup(json, json_len);

out:
	yajl_gen_free(gen);
	return (char *)json;
}
