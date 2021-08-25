/*
 * Copyright (c) 2019 - 2021 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef ZEPHYR_INCLUDE_BLUETOOTH_HOST_AUDIO_MEDIA_PROXY_H_
#define ZEPHYR_INCLUDE_BLUETOOTH_HOST_AUDIO_MEDIA_PROXY_H_

/** @brief Media proxy module
 *
 * The media proxy module is the connection point between media player
 * instances and media controllers.
 *
 * A media player has (access to) media content and knows how to
 * navigate and play this content. A media controller reads or gets
 * information from a player and controls the player by setting player
 * parameters and giving the player commands.
 *
 * The media proxy module allows media player implementations to make
 * themselves available to media controllers. And it allows
 * controllers to access, and get updates from, any player.
 *
 * The media proxy module allows both local and remote control of
 * local player instances: A media controller may be a local
 * application, or it may be a Media Control Service relaying requests
 * from a remote Media Control Client. There may be either local or
 * remote control, or both, or even multiple instances of each.
 *
 * [Experimental] Users should note that the APIs can change
 * as a part of ongoing development.
 */

#include <stdbool.h>
#include <zephyr/types.h>
#include <bluetooth/bluetooth.h>

#include "mcs.h"

#ifdef __cplusplus
extern "C" {
#endif


/**
 * @brief Media player command
 */
struct mpl_cmd_t {
	uint8_t  opcode;
	bool  use_param;
	int32_t param;
};

/**
 * @brief Media command notification
 */
struct mpl_cmd_ntf_t {
	uint8_t requested_opcode;
	uint8_t result_code;
};

/**
 * @brief Search control item
 */
struct mpl_sci_t {
	uint8_t len;                     /**< Length of type and parameter */
	uint8_t type;                    /**< MEDIA_PROXY_SEARCH_TYPE_<...> */
	char    param[SEARCH_PARAM_MAX]; /**< Search parameter */
};

/**
 * @brief Search
 */
struct mpl_search_t {
	uint8_t len;
	char    search[SEARCH_LEN_MAX]; /* Concatenated search control items */
};	                                /* - (type, length, param) */


/* PUBLIC API FOR CONTROLLERS */

/**
 * @brief Playback speed parameters
 *
 * All values from -128 to 127 allowed, only some defined
 */
#define MEDIA_PROXY_PLAYBACK_SPEED_MIN     -128
#define MEDIA_PROXY_PLAYBACK_SPEED_QUARTER -128
#define MEDIA_PROXY_PLAYBACK_SPEED_HALF     -64
#define MEDIA_PROXY_PLAYBACK_SPEED_UNITY      0
#define MEDIA_PROXY_PLAYBACK_SPEED_DOUBLE    64
#define MEDIA_PROXY_PLAYBACK_SPEED_MAX      127

/**
 * @brief Seeking speed factors
 *
 * The allowed values for seeking speed are the range -64 to -4
 * (endpoints included), the value 0, and the range 4 to 64
 * (endpoints included).
 */
#define MEDIA_PROXY_SEEKING_SPEED_FACTOR_MAX  64
#define MEDIA_PROXY_SEEKING_SPEED_FACTOR_MIN   4
#define MEDIA_PROXY_SEEKING_SPEED_FACTOR_ZERO  0

/**
 * @brief Playing orders
 */
#define MEDIA_PROXY_PLAYING_ORDER_SINGLE_ONCE    0x01
#define MEDIA_PROXY_PLAYING_ORDER_SINGLE_REPEAT  0x02
#define MEDIA_PROXY_PLAYING_ORDER_INORDER_ONCE   0x03
#define MEDIA_PROXY_PLAYING_ORDER_INORDER_REPEAT 0x04
#define MEDIA_PROXY_PLAYING_ORDER_OLDEST_ONCE    0x05
#define MEDIA_PROXY_PLAYING_ORDER_OLDEST_REPEAT  0x06
#define MEDIA_PROXY_PLAYING_ORDER_NEWEST_ONCE    0x07
#define MEDIA_PROXY_PLAYING_ORDER_NEWEST_REPEAT  0x08
#define MEDIA_PROXY_PLAYING_ORDER_SHUFFLE_ONCE   0x09
#define MEDIA_PROXY_PLAYING_ORDER_SHUFFLE_REPEAT 0x0a

/**
 * @brief Playing orders supported
 *
 * A bitmap, in the same order as the playing orders above.
 * Note that playing order 1 corresponds to bit 0, and so on.
 */
#define MEDIA_PROXY_PLAYING_ORDERS_SUPPORTED_SINGLE_ONCE    BIT(0)
#define MEDIA_PROXY_PLAYING_ORDERS_SUPPORTED_SINGLE_REPEAT  BIT(1)
#define MEDIA_PROXY_PLAYING_ORDERS_SUPPORTED_INORDER_ONCE   BIT(2)
#define MEDIA_PROXY_PLAYING_ORDERS_SUPPORTED_INORDER_REPEAT BIT(3)
#define MEDIA_PROXY_PLAYING_ORDERS_SUPPORTED_OLDEST_ONCE    BIT(4)
#define MEDIA_PROXY_PLAYING_ORDERS_SUPPORTED_OLDEST_REPEAT  BIT(5)
#define MEDIA_PROXY_PLAYING_ORDERS_SUPPORTED_NEWEST_ONCE    BIT(6)
#define MEDIA_PROXY_PLAYING_ORDERS_SUPPORTED_NEWEST_REPEAT  BIT(7)
#define MEDIA_PROXY_PLAYING_ORDERS_SUPPORTED_SHUFFLE_ONCE   BIT(8)
#define MEDIA_PROXY_PLAYING_ORDERS_SUPPORTED_SHUFFLE_REPEAT BIT(9)

/**
 * @brief Media player states
 */
#define MEDIA_PROXY_STATE_INACTIVE 0x00
#define MEDIA_PROXY_STATE_PLAYING  0x01
#define MEDIA_PROXY_STATE_PAUSED   0x02
#define MEDIA_PROXY_STATE_SEEKING  0x03
#define MEDIA_PROXY_STATE_LAST     0x04

/**
 * @brief Media player command opcodes
 */
#define MEDIA_PROXY_OP_PLAY          0x01
#define MEDIA_PROXY_OP_PAUSE         0x02
#define MEDIA_PROXY_OP_FAST_REWIND   0x03
#define MEDIA_PROXY_OP_FAST_FORWARD  0x04
#define MEDIA_PROXY_OP_STOP          0x05

#define MEDIA_PROXY_OP_MOVE_RELATIVE 0x10

#define MEDIA_PROXY_OP_PREV_SEGMENT  0x20
#define MEDIA_PROXY_OP_NEXT_SEGMENT  0x21
#define MEDIA_PROXY_OP_FIRST_SEGMENT 0x22
#define MEDIA_PROXY_OP_LAST_SEGMENT  0x23
#define MEDIA_PROXY_OP_GOTO_SEGMENT  0x24

#define MEDIA_PROXY_OP_PREV_TRACK    0x30
#define MEDIA_PROXY_OP_NEXT_TRACK    0x31
#define MEDIA_PROXY_OP_FIRST_TRACK   0x32
#define MEDIA_PROXY_OP_LAST_TRACK    0x33
#define MEDIA_PROXY_OP_GOTO_TRACK    0x34

#define MEDIA_PROXY_OP_PREV_GROUP    0x40
#define MEDIA_PROXY_OP_NEXT_GROUP    0x41
#define MEDIA_PROXY_OP_FIRST_GROUP   0x42
#define MEDIA_PROXY_OP_LAST_GROUP    0x43
#define MEDIA_PROXY_OP_GOTO_GROUP    0x44

/**
 * @brief Media player supported opcodes length
 */
#define MEDIA_PROXY_OPCODES_SUPPORTED_LEN 4

/**
 * @brief Media player supported command opcodes
 */
#define MEDIA_PROXY_OP_SUP_PLAY          BIT(0)
#define MEDIA_PROXY_OP_SUP_PAUSE         BIT(1)
#define MEDIA_PROXY_OP_SUP_FAST_REWIND   BIT(2)
#define MEDIA_PROXY_OP_SUP_FAST_FORWARD  BIT(3)
#define MEDIA_PROXY_OP_SUP_STOP          BIT(4)

#define MEDIA_PROXY_OP_SUP_MOVE_RELATIVE BIT(5)

#define MEDIA_PROXY_OP_SUP_PREV_SEGMENT  BIT(6)
#define MEDIA_PROXY_OP_SUP_NEXT_SEGMENT  BIT(7)
#define MEDIA_PROXY_OP_SUP_FIRST_SEGMENT BIT(8)
#define MEDIA_PROXY_OP_SUP_LAST_SEGMENT  BIT(9)
#define MEDIA_PROXY_OP_SUP_GOTO_SEGMENT  BIT(10)

#define MEDIA_PROXY_OP_SUP_PREV_TRACK    BIT(11)
#define MEDIA_PROXY_OP_SUP_NEXT_TRACK    BIT(12)
#define MEDIA_PROXY_OP_SUP_FIRST_TRACK   BIT(13)
#define MEDIA_PROXY_OP_SUP_LAST_TRACK    BIT(14)
#define MEDIA_PROXY_OP_SUP_GOTO_TRACK    BIT(15)

#define MEDIA_PROXY_OP_SUP_PREV_GROUP    BIT(16)
#define MEDIA_PROXY_OP_SUP_NEXT_GROUP    BIT(17)
#define MEDIA_PROXY_OP_SUP_FIRST_GROUP   BIT(18)
#define MEDIA_PROXY_OP_SUP_LAST_GROUP    BIT(19)
#define MEDIA_PROXY_OP_SUP_GOTO_GROUP    BIT(20)

/**
 * @brief Media player command result codes
 */
#define MEDIA_PROXY_CMD_SUCCESS             0x01
#define MEDIA_PROXY_CMD_NOT_SUPPORTED       0x02
#define MEDIA_PROXY_CMD_PLAYER_INACTIVE     0x03
#define MEDIA_PROXY_CMD_CANNOT_BE_COMPLETED 0x04

/**
 * @brief Search operation type values
 */
#define MEDIA_PROXY_SEARCH_TYPE_TRACK_NAME    0x01
#define MEDIA_PROXY_SEARCH_TYPE_ARTIST_NAME   0x02
#define MEDIA_PROXY_SEARCH_TYPE_ALBUM_NAME    0x03
#define MEDIA_PROXY_SEARCH_TYPE_GROUP_NAME    0x04
#define MEDIA_PROXY_SEARCH_TYPE_EARLIEST_YEAR 0x05
#define MEDIA_PROXY_SEARCH_TYPE_LATEST_YEAR   0x06
#define MEDIA_PROXY_SEARCH_TYPE_GENRE         0x07
#define MEDIA_PROXY_SEARCH_TYPE_ONLY_TRACKS   0x08
#define MEDIA_PROXY_SEARCH_TYPE_ONLY_GROUPS   0x09

/**
 * @brief Search operation result codes
 */
#define MEDIA_PROXY_SEARCH_SUCCESS  0x01
#define MEDIA_PROXY_SEARCH_FAILURE  0x02


/**
 * @brief Opaque media player instance
 */
struct media_player;

/** @brief Callbacks to a controller, from the media proxy
 *
 *  Given by a controller when registering
 */
struct media_proxy_ctrl_cbs {

	/**
	 * @brief Media Player Instance callback
	 *
	 * Called when the local Media Player instance is registered or read (TODO).
	 * Also called if the local player instance is already registered when
	 * the controller is registered.
	 * Provides the controller with the pointer to the local player instance.
	 *
	 * @param player   Media player instance pointer
	 * @param err      Error value. 0 on success, or errno on negative value.
	 */
	void (*local_player_instance)(struct media_player *player, int err);

	/**
	 * @brief Discover Player Instance callback
	 *
	 * Called when a remote player instance has been discovered.
	 * The instance has been discovered, and will be accessed, using Bluetooth,
	 * via media control client and a remote media control service.
	 *
	 * @param player   Instance pointer to the remote player
	 * @param err      Error value. 0 on success, GATT error on positive value
	 *                 or errno on negative value.
	 */
	void (*discover_player)(struct media_player *player, int err);

	/**
	 * @brief Media Player Name callback
	 *
	 * Called when the Media Player Name is read or changed
	 * See also media_proxy_ctrl_name_get()
	 *
	 * @param player   Media player instance pointer
	 * @param err      Error value. 0 on success, GATT error on positive value
	 *                 or errno on negative value.
	 * @param name	   The name of the media player
	 */
	void (*player_name)(struct media_player *player, int err, const char *name);

	/**
	 * @brief Media Player Icon Object ID callback
	 *
	 * Called when the Media Player Icon Object ID is read
	 * See also media_proxy_ctrl_icon_id_get()
	 *
	 * @param player   Media player instance pointer
	 * @param err      Error value. 0 on success, GATT error on positive value
	 *                 or errno on negative value.
	 * @param id	   The ID of the Icon object in the Object Transfer Service (48 bits)
	 */
	void (*icon_id)(struct media_player *player, int err, uint64_t id);

	/**
	 * @brief Media Player Icon URL callback
	 *
	 * Called when the Media Player Icon URL is read
	 * See also media_proxy_ctrl_icon_url_get()
	 *
	 * @param player   Media player instance pointer
	 * @param err      Error value. 0 on success, GATT error on positive value
	 *                 or errno on negative value.
	 * @param url	   The URL of the icon
	 */
	void (*icon_url)(struct media_player *player, int err, const char *url);

	/**
	 * @brief Track changed callback
	 *
	 * Called when the Current Track is changed
	 *
	 * @param player   Media player instance pointer
	 * @param err      Error value. 0 on success, GATT error on positive value
	 *                 or errno on negative value.
	 */
	void (*track_changed)(struct media_player *player, int err);

	/**
	 * @brief Track Title callback
	 *
	 * Called when the Track Title is read or changed
	 * See also media_proxy_ctrl_track_title_get()
	 *
	 * @param player   Media player instance pointer
	 * @param err      Error value. 0 on success, GATT error on positive value
	 *                 or errno on negative value.
	 * @param title	   The title of the current track
	 */
	void (*track_title)(struct media_player *player, int err, const char *title);

	/**
	 * @brief Track Duration callback
	 *
	 * Called when the Track Duration is read or changed
	 * Seel also media_proxy_ctrl_track_duration_get()
	 *
	 * @param player     Media player instance pointer
	 * @param err        Error value. 0 on success, GATT error on positive value
	 *                   or errno on negative value.
	 * @param duration   The duration of the current track
	 */
	void (*track_duration)(struct media_player *player, int err, int32_t duration);

	/**
	 * @brief Track Position callback
	 *
	 * Called when the Track Position is read or changed
	 * See also media_proxy_ctrl_track_position_get() and
	 * media_proxy_ctrl_track_position_set()
	 *
	 * @param player     Media player instance pointer
	 * @param err        Error value. 0 on success, GATT error on positive value
	 *                   or errno on negative value.
	 * @param position   The player's position in the track
	 */
	void (*track_position)(struct media_player *player, int err, int32_t position);

	/**
	 * @brief Playback Speed callback
	 *
	 * Called when the Playback Speed is read or changed
	 * See also media_proxy_ctrl_playback_speed_get() and
	 * media_proxy_ctrl_playback_speed_set()
	 *
	 * @param player   Media player instance pointer
	 * @param err      Error value. 0 on success, GATT error on positive value
	 *                 or errno on negative value.
	 * @param speed    The playback speed parameter
	 */
	void (*playback_speed)(struct media_player *player, int err, int8_t speed);

	/**
	 * @brief Seeking Speed callback
	 *
	 * Called when the Seeking Speed is read or changed
	 * See also media_proxy_ctrl_seeking_speed_get()
	 *
	 * @param player   Media player instance pointer
	 * @param err      Error value. 0 on success, GATT error on positive value
	 *                 or errno on negative value.
	 * @param speed    The seeking speed factor
	 */
	void (*seeking_speed)(struct media_player *player, int err, int8_t speed);

	/**
	 * @brief Track Segments Object ID callback
	 *
	 * Called when the Track Segments Object ID is read
	 * See also media_proxy_ctrl_track_segments_id_get()
	 *
	 * @param player   Media player instance pointer
	 * @param err      Error value. 0 on success, GATT error on positive value
	 *                 or errno on negative value.
	 * @param id	   The ID of the track segments object in Object Transfer Service (48 bits)
	 */
	void (*track_segments_id)(struct media_player *player, int err, uint64_t id);

	/**
	 * @brief Current Track Object ID callback
	 *
	 * Called when the Current Track Object ID is read or changed
	 * See also media_proxy_ctrl_current_track_id_get() and
	 * media_proxy_ctrl_current_track_id_set()
	 *
	 * @param player   Media player instance pointer
	 * @param err      Error value. 0 on success, GATT error on positive value
	 *                 or errno on negative value.
	 * @param id	   The ID of the current track object in Object Transfer Service (48 bits)
	 */
	void (*current_track_id)(struct media_player *player, int err, uint64_t id);

	/**
	 * @brief Next Track Object ID callback
	 *
	 * Called when the Next Track Object ID is read or changed
	 * See also media_proxy_ctrl_next_track_id_get() and
	 * media_proxy_ctrl_next_track_id_set()
	 *
	 * @param player   Media player instance pointer
	 * @param err      Error value. 0 on success, GATT error on positive value
	 *                 or errno on negative value.
	 * @param id	   The ID of the next track object in Object Transfer Service (48 bits)
	 */
	void (*next_track_id)(struct media_player *player, int err, uint64_t id);

	/**
	 * @brief Current Group Object ID callback
	 *
	 * Called when the Current Group Object ID is read or changed
	 * See also media_proxy_ctrl_current_group_id_get() and
	 * media_proxy_ctrl_current_group_id_set()
	 *
	 * @param player   Media player instance pointer
	 * @param err      Error value. 0 on success, GATT error on positive value
	 *                 or errno on negative value.
	 * @param id	   The ID of the current group object in Object Transfer Service (48 bits)
	 */
	void (*current_group_id)(struct media_player *player, int err, uint64_t id);

	/**
	 * @brief Parent Group Object ID callback
	 *
	 * Called when the Parent Group Object ID is read or changed
	 * See also media_proxy_ctrl_parent_group_id_get()
	 *
	 * @param player   Media player instance pointer
	 * @param err      Error value. 0 on success, GATT error on positive value
	 *                 or errno on negative value.
	 * @param id	   The ID of the parent group object in Object Transfer Service (48 bits)
	 */
	void (*parent_group_id)(struct media_player *player, int err, uint64_t id);

	/**
	 * @brief Playing Order callback
	 *
	 * Called when the Playing Order is read or changed
	 * See also media_proxy_ctrl_playing_order_get() and
	 * media_proxy_ctrl_playing_order_set()
	 *
	 * @param player   Media player instance pointer
	 * @param err      Error value. 0 on success, GATT error on positive value
	 *                 or errno on negative value.
	 * @param order    The playing order
	 */
	void (*playing_order)(struct media_player *player, int err, uint8_t order);

	/**
	 * @brief Playing Orders Supported callback
	 *
	 * Called when the Playing Orders Supported is read
	 * See also media_proxy_ctrl_playing_orders_supported_get()
	 *
	 * @param player   Media player instance pointer
	 * @param err      Error value. 0 on success, GATT error on positive value
	 *                 or errno on negative value.
	 * @param orders   The playing orders supported
	 */
	void (*playing_orders_supported)(struct media_player *player, int err, uint16_t orders);

	/**
	 * @brief Media State callback
	 *
	 * Called when the Media State is read or changed
	 * See also media_proxy_ctrl_media_state_get() and
	 * media_proxy_ctrl_command_send()
	 *
	 * @param player   Media player instance pointer
	 * @param err      Error value. 0 on success, GATT error on positive value
	 *                 or errno on negative value.
	 * @param state    The media player state
	 */
	void (*media_state)(struct media_player *player, int err, uint8_t state);

	/**
	 * @brief Command callback
	 *
	 * Called when a command has been sent, to give the result of the
	 * command
	 * See also media_proxy_ctrl_command_send()
	 *
	 * @param player   Media player instance pointer
	 * @param err      Error value. 0 on success, GATT error on positive value
	 *                 or errno on negative value.
	 * @param cmd_ntf  The result of the command
	 */
	void (*command)(struct media_player *player, int err, struct mpl_cmd_ntf_t cmd_ntf);

	/**
	 * @brief Commands supported callback
	 *
	 * Called when the Commands Supported is read or changed
	 * See also media_proxy_ctrl_commands_supported_get()
	 *
	 * @param player       Media player instance pointer
	 * @param err          Error value. 0 on success, GATT error on positive value
	 *                     or errno on negative value.
	 * @param opcodes      The supported command opcodes (bitmap)
	 */
	void (*commands_supported)(struct media_player *player, int err, uint32_t opcodes);

	/**
	 * @brief Search callback
	 *
	 * Called when a search operation has been set, to give the result
	 * of the operation
	 * See also media_proxy_ctrl_search_set()
	 *
	 * @param player        Media player instance pointer
	 * @param err           Error value. 0 on success, GATT error on positive value
	 *                      or errno on negative value.
	 * @param result_code   The result of the operation
	 */
	void (*search)(struct media_player *player, int err, uint8_t result_code);

	/**
	 * @brief Search Results Object ID callback
	 * See also media_proxy_ctrl_search_results_id_get()
	 *
	 * Called when the Search Results Object ID is read or changed
	 *
	 * @param player   Media player instance pointer
	 * @param err      Error value. 0 on success, GATT error on positive value
	 *                 or errno on negative value.
	 * @param id	   The ID of the search results object in Object Transfer Service (48 bits)
	 */
	void (*search_results_id)(struct media_player *player, int err, uint64_t id);

	/**
	 * @brief Content Control ID callback
	 *
	 * Called when the Content Control ID is read
	 * See also media_proxy_ctrl_content_ctrl_id_get()
	 *
	 * @param player   Media player instance pointer
	 * @param err      Error value. 0 on success, GATT error on positive value
	 *                 or errno on negative value.
	 * @param ccid     The content control ID
	 */
	void (*content_ctrl_id)(struct media_player *player, int err, uint8_t ccid);
};

/**
 * @brief Register a controller with the media_proxy
 *
 * @param ctrl_cbs   Callbacks to the controller
 *
 * @return 0 if success, errno on failure
 */
int media_proxy_ctrl_register(struct media_proxy_ctrl_cbs *ctrl_cbs);

/**
 * @brief Discover a remote media player
 *
 * Discover a remote media player instance.
 * The remote player instance will be discovered, and accessed, using Bluetooth,
 * via the media control client and a remote media control service.
 * This call will start a GATT discovery of the Media Control Service on the peer,
 * and setup handles and subscriptions.
 *
 * This shall be called once before any other actions can be executed for the
 * remote player. The remote player instance will be returned in the
 * discover_player() callback.
 *
 * @param conn   The connection to do discovery for
 *
 * @return 0 if success, errno on failure
 */
int media_proxy_ctrl_discover_player(struct bt_conn *conn);

/**
 * @brief Read Media Player Name
 *
 * @param player   Media player instance pointer
 *
 * @return 0 if success, errno on failure.
 */
int media_proxy_ctrl_player_name_get(struct media_player *player);

/**
 * @brief Read Icon Object ID
 *
 * Get an ID (48 bit) that can be used to retrieve the Icon
 * Object from an Object Transfer Service
 *
 * See the Media Control Service spec v1.0 sections 3.2 and
 * 4.1 for a description of the Icon Object.
 *
 * Requires Object Transfer Service
 *
 * @param player   Media player instance pointer
 *
 * @return 0 if success, errno on failure.
 */
int media_proxy_ctrl_icon_id_get(struct media_player *player);

/**
 * @brief Read Icon URL
 *
 * Get a URL to the media player's icon.
 *
 * @param player   Media player instance pointer
 */
int media_proxy_ctrl_icon_url_get(struct media_player *player);

/**
 * @brief Read Track Title
 *
 * @param player   Media player instance pointer
 *
 * @return 0 if success, errno on failure.
 */
int media_proxy_ctrl_track_title_get(struct media_player *player);

/**
 * @brief Read Track Duration
 *
 * The duration of a track is measured in hundredths of a
 * second.
 *
 * @param player   Media player instance pointer
 *
 * @return 0 if success, errno on failure.
 */
int media_proxy_ctrl_track_duration_get(struct media_player *player);

/**
 * @brief Read Track Position
 *
 * The position of the player (the playing position) is
 * measured in hundredths of a second from the beginning of
 * the track
 *
 * @param player   Media player instance pointer
 *
 * @return 0 if success, errno on failure.
 */
int media_proxy_ctrl_track_position_get(struct media_player *player);

/**
 * @brief Set Track Position
 *
 * Set the playing position of the media player in the current
 * track. The position is given in in hundredths of a second,
 * from the beginning of the track of the track for positive
 * values, and (backwards) from the end of the track for
 * negative values.
 *
 * @param player     Media player instance pointer
 * @param position   The track position to set
 *
 * @return 0 if success, errno on failure.
 */
int media_proxy_ctrl_track_position_set(struct media_player *player, int32_t position);

/**
 * @brief Get Playback Speed
 *
 * The playback speed parameter is related to the actual
 * playback speed as follows:
 * actual playback speed = 2^(speed_parameter/64)
 *
 * A speed parameter of 0 corresponds to unity speed playback
 * (i.e. playback at "normal" speed). A speed parameter of
 * -128 corresponds to playback at one fourth of normal speed,
 * 127 corresponds to playback at almost four times the normal
 * speed.
 *
 * @param player   Media player instance pointer
 *
 * @return 0 if success, errno on failure.
 */
int media_proxy_ctrl_playback_speed_get(struct media_player *player);

/**
 * @brief Set Playback Speed
 *
 * See the playback_speed_get() function for an explanation of
 * the playback speed parameter.
 *
 * Note that the media player may not support all possible
 * values of the playback speed parameter. If the value given
 * is not supported, and is higher than the current value, the
 * player should set the playback speed to the next higher
 * supported value. (And correspondingly to the next lower
 * supported value for given values lower than the current
 * value.)
 *
 * @param player   Media player instance pointer
 * @param speed    The playback speed parameter to set
 *
 * @return 0 if success, errno on failure.
 */
int media_proxy_ctrl_playback_speed_set(struct media_player *player, int8_t speed);

/**
 * @brief Get Seeking Speed
 *
 * The seeking speed gives the speed with which the player is
 * seeking. It is a factor, relative to real-time playback
 * speed - a factor four means seeking happens at four times
 * the real-time playback speed. Positive values are for
 * forward seeking, negative values for backwards seeking.
 *
 * The seeking speed is not setable - a non-zero seeking speed
 * is the result of "fast rewind" of "fast forward" commands.
 *
 * @param player   Media player instance pointer
 *
 * @return 0 if success, errno on failure.
 */
int media_proxy_ctrl_seeking_speed_get(struct media_player *player);

/**
 * @brief Read Current Track Segments Object ID
 *
 * Get an ID (48 bit) that can be used to retrieve the Current
 * Track Segments Object from an Object Transfer Service
 *
 * See the Media Control Service spec v1.0 sections 3.10 and
 * 4.2 for a description of the Track Segments Object.
 *
 * Requires Object Transfer Service
 *
 * @param player   Media player instance pointer
 *
 * @return 0 if success, errno on failure.
 */
int media_proxy_ctrl_track_segments_id_get(struct media_player *player);

/**
 * @brief Read Current Track Object ID
 *
 * Get an ID (48 bit) that can be used to retrieve the Current
 * Track Object from an Object Transfer Service
 *
 * See the Media Control Service spec v1.0 sections 3.11 and
 * 4.3 for a description of the Current Track Object.
 *
 * Requires Object Transfer Service
 *
 * @param player   Media player instance pointer
 *
 * @return 0 if success, errno on failure.
 */
int media_proxy_ctrl_current_track_id_get(struct media_player *player);

/**
 * @brief Set Current Track Object ID
 *
 * Change the player's current track to the track given by the ID.
 * (Behaves similarly to the goto track command.)
 *
 * Requires Object Transfer Service
 *
 * @param player   Media player instance pointer
 * @param id	   The ID of a track object
 *
 * @return 0 if success, errno on failure.
 */
int media_proxy_ctrl_current_track_id_set(struct media_player *player, uint64_t id);

/**
 * @brief Read Next Track Object ID
 *
 * Get an ID (48 bit) that can be used to retrieve the Next
 * Track Object from an Object Transfer Service
 *
 * Requires Object Transfer Service
 *
 * @param player   Media player instance pointer
 *
 * @return 0 if success, errno on failure.
 */
int media_proxy_ctrl_next_track_id_get(struct media_player *player);

/**
 * @brief Set Next Track Object ID
 *
 * Change the player's next track to the track given by the ID.
 *
 * Requires Object Transfer Service
 *
 * @param player   Media player instance pointer
 * @param id       The ID of a track object
 *
 * @return 0 if success, errno on failure.
 */
int media_proxy_ctrl_next_track_id_set(struct media_player *player, uint64_t id);

/**
 * @brief Read Current Group Object ID
 *
 * Get an ID (48 bit) that can be used to retrieve the Current
 * Track Object from an Object Transfer Service
 *
 * See the Media Control Service spec v1.0 sections 3.14 and
 * 4.4 for a description of the Current Group Object.
 *
 * Requires Object Transfer Service
 *
 * @param player   Media player instance pointer
 *
 * @return 0 if success, errno on failure.
 */
int media_proxy_ctrl_current_group_id_get(struct media_player *player);

/**
 * @brief Set Current Group Object ID
 *
 * Change the player's current group to the group given by the
 * ID, and the current track to the first track in that group.
 *
 * Requires Object Transfer Service
 *
 * @param player   Media player instance pointer
 * @param id	   The ID of a group object
 *
 * @return 0 if success, errno on failure.
 */
int media_proxy_ctrl_current_group_id_set(struct media_player *player, uint64_t id);

/**
 * @brief Read Parent Group Object ID
 *
 * Get an ID (48 bit) that can be used to retrieve the Parent
 * Track Object from an Object Transfer Service
 *
 * The parent group is the parent of the current group.
 *
 * See the Media Control Service spec v1.0 sections 3.14 and
 * 4.4 for a description of the Current Track Object.
 *
 * Requires Object Transfer Service
 *
 * @param player   Media player instance pointer
 *
 * @return 0 if success, errno on failure.
 */
int media_proxy_ctrl_parent_group_id_get(struct media_player *player);

/**
 * @brief Read Playing Order
 *
 * @param player   Media player instance pointer
 *
 * @return 0 if success, errno on failure.
 */
int media_proxy_ctrl_playing_order_get(struct media_player *player);

/**
 * @brief Set Playing Order
 *
 * Set the media player's playing order
 *
 * @param player   Media player instance pointer
 * @param order	   The playing order to set
 *
 * @return 0 if success, errno on failure.
 */
int media_proxy_ctrl_playing_order_set(struct media_player *player, uint8_t order);

/**
 * @brief Read Playing Orders Supported
 *
 * Read a bitmap containing the media player's supported
 * playing orders.
 *
 * @param player   Media player instance pointer
 *
 * @return 0 if success, errno on failure.
 */
int media_proxy_ctrl_playing_orders_supported_get(struct media_player *player);

/**
 * @brief Read Media State
 *
 * Read the media player's state
 *
 * @param player   Media player instance pointer
 *
 * @return 0 if success, errno on failure.
 */
int media_proxy_ctrl_media_state_get(struct media_player *player);

/**
 * @brief Send Command
 *
 * Send a command to the media player.
 * Commands may cause the media player to change its state
 *
 * @param player      Media player instance pointer
 * @param command     The command to send
 *
 * @return 0 if success, errno on failure.
 */
int media_proxy_ctrl_command_send(struct media_player *player, struct mpl_cmd_t command);

/**
 * @brief Read Commands Supported
 *
 * Read a bitmap containing the media player's supported
 * command opcodes.
 *
 * @param player   Media player instance pointer
 *
 * @return 0 if success, errno on failure.
 */
int media_proxy_ctrl_commands_supported_get(struct media_player *player);

/**
 * @brief Set Search
 *
 * Write a search to the media player.
 * If the search is successful, the search result will be available as an object
 * in the Object Transfer Service.
 *
 * Requires Object Transfer Service
 *
 * @param player   Media player instance pointer
 * @param search   The search to write
 *
 * @return 0 if success, errno on failure.
 */
int media_proxy_ctrl_search_set(struct media_player *player, struct mpl_search_t search);

/**
 * @brief Read Search Results Object ID
 *
 * Get an ID (48 bit) that can be used to retrieve the Search
 * Results Object from an Object Transfer Service
 *
 * The search results object is a group object.
 * The search results object only exists if a successful
 * search operation has been done.
 *
 * Requires Object Transfer Service
 *
 * @param player   Media player instance pointer
 *
 * @return 0 if success, errno on failure.
 */
int media_proxy_ctrl_search_results_id_get(struct media_player *player);

/**
 * @brief Read Content Control ID
 *
 * The content control ID identifies a content control service
 * on a device, and links it to the corresponding audio
 * stream.
 *
 * @param player   Media player instance pointer
 *
 * @return 0 if success, errno on failure.
 */
uint8_t media_proxy_ctrl_content_ctrl_id_get(struct media_player *player);


/* PUBLIC API FOR PLAYERS */

/** @brief Availalable calls in a player, that the media proxy can call
 *
 * Given by a player when registering.
 */
struct media_proxy_pl_calls {

	/**
	 * @brief Read Media Player Name
	 *
	 * @return The name of the media player
	 */
	const char *(*player_name_get)(void);

	/**
	 * @brief Read Icon Object ID
	 *
	 * Get an ID (48 bit) that can be used to retrieve the Icon
	 * Object from an Object Transfer Service
	 *
	 * See the Media Control Service spec v1.0 sections 3.2 and
	 * 4.1 for a description of the Icon Object.
	 *
	 * @return The Icon Object ID
	 */
	uint64_t (*icon_id_get)(void);

	/**
	 * @brief Read Icon URL
	 *
	 * Get a URL to the media player's icon.
	 *
	 * @return The URL of the Icon
	 */
	const char *(*icon_url_get)(void);

	/**
	 * @brief Read Track Title
	 *
	 * @return The title of the current track
	 */
	const char *(*track_title_get)(void);

	/**
	 * @brief Read Track Duration
	 *
	 * The duration of a track is measured in hundredths of a
	 * second.
	 *
	 * @return The duration of the current track
	 */
	int32_t (*track_duration_get)(void);

	/**
	 * @brief Read Track Position
	 *
	 * The position of the player (the playing position) is
	 * measured in hundredths of a second from the beginning of
	 * the track
	 *
	 * @return The position of the player in the current track
	 */
	int32_t (*track_position_get)(void);

	/**
	 * @brief Set Track Position
	 *
	 * Set the playing position of the media player in the current
	 * track. The position is given in in hundredths of a second,
	 * from the beginning of the track of the track for positive
	 * values, and (backwards) from the end of the track for
	 * negative values.
	 *
	 * @param position	The player position to set
	 */
	void (*track_position_set)(int32_t position);

	/**
	 * @brief Get Playback Speed
	 *
	 * The playback speed parameter is related to the actual
	 * playback speed as follows:
	 * actual playback speed = 2^(speed_parameter/64)
	 *
	 * A speed parameter of 0 corresponds to unity speed playback
	 * (i.e. playback at "normal" speed). A speed parameter of
	 * -128 corresponds to playback at one fourth of normal speed,
	 * 127 corresponds to playback at almost four times the normal
	 * speed.
	 *
	 * @return The playback speed parameter
	 */
	int8_t (*playback_speed_get)(void);

	/**
	 * @brief Set Playback Speed
	 *
	 * See the playback_speed_get() function for an explanation of
	 * the playback speed parameter.
	 *
	 * Note that the media player may not support all possible
	 * values of the playback speed parameter. If the value given
	 * is not supported, and is higher than the current value, the
	 * player should set the playback speed to the next higher
	 * supported value. (And correspondingly to the next lower
	 * supported value for given values lower than the current
	 * value.)
	 *
	 * @param speed The playback speed parameter to set
	 */
	void (*playback_speed_set)(int8_t speed);

	/**
	 * @brief Get Seeking Speed
	 *
	 * The seeking speed gives the speed with which the player is
	 * seeking. It is a factor, relative to real-time playback
	 * speed - a factor four means seeking happens at four times
	 * the real-time playback speed. Positive values are for
	 * forward seeking, negative values for backwards seeking.
	 *
	 * The seeking speed is not setable - a non-zero seeking speed
	 * is the result of "fast rewind" of "fast forward" commands.
	 *
	 * @return The seeking speed factor
	 */
	int8_t (*seeking_speed_get)(void);

	/**
	 * @brief Read Current Track Segments Object ID
	 *
	 * Get an ID (48 bit) that can be used to retrieve the Current
	 * Track Segments Object from an Object Transfer Service
	 *
	 * See the Media Control Service spec v1.0 sections 3.10 and
	 * 4.2 for a description of the Track Segments Object.
	 *
	 * @return Current The Track Segments Object ID
	 */
	uint64_t (*track_segments_id_get)(void);

	/**
	 * @brief Read Current Track Object ID
	 *
	 * Get an ID (48 bit) that can be used to retrieve the Current
	 * Track Object from an Object Transfer Service
	 *
	 * See the Media Control Service spec v1.0 sections 3.11 and
	 * 4.3 for a description of the Current Track Object.
	 *
	 * @return The Current Track Object ID
	 */
	uint64_t (*current_track_id_get)(void);

	/**
	 * @brief Set Current Track Object ID
	 *
	 * Change the player's current track to the track given by the ID.
	 * (Behaves similarly to the goto track command.)
	 *
	 * @param id	The ID of a track object
	 */
	void (*current_track_id_set)(uint64_t id);

	/**
	 * @brief Read Next Track Object ID
	 *
	 * Get an ID (48 bit) that can be used to retrieve the Next
	 * Track Object from an Object Transfer Service
	 *
	 * @return The Next Track Object ID
	 */
	uint64_t (*next_track_id_get)(void);

	/**
	 * @brief Set Next Track Object ID
	 *
	 * Change the player's next track to the track given by the ID.
	 *
	 * @param id	The ID of a track object
	 */
	void (*next_track_id_set)(uint64_t id);

	/**
	 * @brief Read Current Group Object ID
	 *
	 * Get an ID (48 bit) that can be used to retrieve the Current
	 * Track Object from an Object Transfer Service
	 *
	 * See the Media Control Service spec v1.0 sections 3.14 and
	 * 4.4 for a description of the Current Group Object.
	 *
	 * @return The Current Group Object ID
	 */
	uint64_t (*current_group_id_get)(void);

	/**
	 * @brief Set Current Group Object ID
	 *
	 * Change the player's current group to the group given by the
	 * ID, and the current track to the first track in that group.
	 *
	 * @param id	The ID of a group object
	 */
	void (*current_group_id_set)(uint64_t id);

	/**
	 * @brief Read Parent Group Object ID
	 *
	 * Get an ID (48 bit) that can be used to retrieve the Parent
	 * Track Object from an Object Transfer Service
	 *
	 * The parent group is the parent of the current group.
	 *
	 * See the Media Control Service spec v1.0 sections 3.14 and
	 * 4.4 for a description of the Current Track Object.
	 *
	 * @return The Current Group Object ID
	 */
	uint64_t (*parent_group_id_get)(void);

	/**
	 * @brief Read Playing Order
	 *
	 * return The media player's current playing order
	 */
	uint8_t (*playing_order_get)(void);

	/**
	 * @brief Set Playing Order
	 *
	 * Set the media player's playing order
	 * See the Media Control Service spec, or the
	 * BT_MCS_PLAYING_ORDER_* defines in the mcs.h file.
	 *
	 * @param order	The playing order to set
	 */
	void (*playing_order_set)(uint8_t order);

	/**
	 * @brief Read Playing Orders Supported
	 *
	 * Read a bitmap containing the media player's supported
	 * playing orders.
	 * See the Media Control Service spec, or the
	 * BT_MCS_PLAYING_ORDERS_SUPPORTED_* defines in the mcs.h
	 * file.
	 *
	 * @return The media player's supported playing orders
	 */
	uint16_t (*playing_orders_supported_get)(void);

	/**
	 * @brief Read Media State
	 *
	 * Read the media player's state
	 * See the Media Control Service spec, or the
	 * BT_MCS_MEDIA_STATE_* defines in the mcs.h file.
	 *
	 * @return The media player's state
	 */
	uint8_t (*media_state_get)(void);

	/**
	 * @brief Send Command
	 *
	 * Send a command to the media player.
	 * For command opcodes (play, pause, ... - see the Media Control
	 * Service spec, or the BT_MCS_OPC_* defines in the mcs.h
	 * file.
	 *
	 * @param command	The command to send
	 */
	void (*command_send)(struct mpl_cmd_t command);

	/**
	 * @brief Read Commands Supported
	 *
	 * Read a bitmap containing the media player's supported
	 * command opcodes..
	 * See the Media Control Service spec, or the
	 * BT_MCS_OPC_SUP_* defines in the mcs.h file.
	 *
	 * @return The media player's supported command opcodes
	 */
	uint32_t (*commands_supported_get)(void);

	/**
	 * @brief Set Search
	 *
	 * Write a search to the media player.
	 * (For the formatting of a search, see the Media Control
	 * Service spec and the mcs.h file.)
	 *
	 * @param search	The search to write
	 */
	void (*search_set)(struct mpl_search_t search);

	/**
	 * @brief Read Search Results Object ID
	 *
	 * Get an ID (48 bit) that can be used to retrieve the Search
	 * Results Object from an Object Transfer Service
	 *
	 * The search results object is a group object.
	 * The search results object only exists if a successful
	 * search operation has been done.
	 *
	 * @return The Search Results Object ID
	 */
	uint64_t (*search_results_id_get)(void);

	/**
	 * @brief Read Content Control ID
	 *
	 * The content control ID identifies a content control service
	 * on a device, and links it to the corresponding audio
	 * stream.
	 *
	 * @return The content control ID for the media player
	 */
	uint8_t (*content_ctrl_id_get)(void);
};

/**
 * @brief Register a player with the media proxy
 *
 * Register a player with the media proxy module, for use by media
 * controllers.
 *
 * The media proxy may call any non-NULL function pointers in the
 * supplied media_proxy_pl_calls structure.
 *
 * @param pl_calls	Function pointers to the media player's calls
 *
 * @return 0 if success, errno on failure
 */
int media_proxy_pl_register(struct media_proxy_pl_calls *pl_calls);

/* Initialize player - TODO: Move to player header file */
int media_proxy_pl_init(void);

/* TODO: Find best location for this call, and move this one also */
struct bt_ots *bt_mcs_get_ots(void);

/* Callbacks from the player to the media proxy */

/**
 * @brief Track changed callback
 *
 * To be called when the player's current track is changed
 */
void media_proxy_pl_track_changed_cb(void);

/**
 * @brief Track title callback
 *
 * To be called when the player's current track is changed
 *
 * @param title	The title of the track
 */
void media_proxy_pl_track_title_cb(char *title);

/**
 * @brief Track duration callback
 *
 * To be called when the current track's duration is changed (e.g. due
 * to a track change)
 *
 * The track duration is given in hundredths of a second.
 *
 * @param duration	The track duration
 */
void media_proxy_pl_track_duration_cb(int32_t duration);

/**
 * @brief Track position callback
 *
 * To be called when the media player's position in the track is
 * changed, or when the player is paused or similar.
 *
 * Exception: This callback should not be called when the position
 * changes during regular playback, i.e. while the player is playing
 * and playback happens at a constant speed.
 *
 * The track position is given in hundredths of a second from the
 * start of the track.
 *
 *  @param position	The media player's position in the track
 */
void media_proxy_pl_track_position_cb(int32_t position);

/**
 * @brief Playback speed callback
 *
 * To be called when the playback speed is changed.
 *
 * @param speed	The playback speed parameter
 */
void media_proxy_pl_playback_speed_cb(int8_t speed);

/**
 * @brief Seeking speed callback
 *
 * To be called when the seeking speed is changed.
 *
 * @param speed	The seeking speed factor
 */
void media_proxy_pl_seeking_speed_cb(int8_t speed);

/**
 * @brief Current track object ID callback
 *
 * To be called when the ID of the current track is changed (e.g. due
 * to a track change).
 *
 * @param speed	The ID of the current track object in the OTS
 */
void media_proxy_pl_current_track_id_cb(uint64_t id);

/**
 * @brief Next track object ID callback
 *
 * To be called when the ID of the current track is changes
 *
 * @param speed	The ID of the next track object in the OTS
 */
void media_proxy_pl_next_track_id_cb(uint64_t id);

/**
 * @brief Current group object ID callback
 *
 * To be called when the ID of the current group is changed
 *
 * @param speed	The ID of the current group object in the OTS
 */
void media_proxy_pl_current_group_id_cb(uint64_t id);

/**
 * @brief Parent group object ID callback
 *
 * To be called when the ID of the parent group is changed
 *
 * @param speed	The ID of the parent group object in the OTS
 */
void media_proxy_pl_parent_group_id_cb(uint64_t id);

/**
 * @brief Playing order callback
 *
 * To be called when the playing order is changed
 *
 * @param order	The playing order
 */
void media_proxy_pl_playing_order_cb(uint8_t order);

/**
 * @brief Media state callback
 *
 * To be called when the media state is changed
 *
 * @param state	The media player's state
 */
void media_proxy_pl_media_state_cb(uint8_t state);

/**
 * @brief Command callback
 *
 * To be called when a command has been sent, to notify whether the
 * command was successfully performed or not.
 * See the Media Control Service spec, or the BT_MCS_OPC_NTF_*
 * defines in the mcs.h file.
 *
 * @param cmd_ntf	The result of the command
 */
void media_proxy_pl_command_cb(struct mpl_cmd_ntf_t cmd_ntf);

/**
 * @brief Commands supported callback
 *
 * To be called when the set of commands supported is changed
 *
 * @param opcodes   The supported commands opcodes
 */
void media_proxy_pl_commands_supported_cb(uint32_t opcodes);

/**
 * @brief Search callback
 *
 * To be called when a search has been set to notify whether the
 * search was successfully performed or not.
 * See the Media Control Service spec, or the BT_MCS_SCP_NTF_*
 * defines in the mcs.h file.
 *
 * The actual results of the search, if successful, can be found in
 * the search results object.
 *
 * @param result_code	The result (success or failure) of the search
 */
void media_proxy_pl_search_cb(uint8_t result_code);

/**
 * @brief Search Results object ID callback
 *
 * To be called when the ID of the search results is changed
 * (typically as the result of a new successful search).
 *
 * @param id	The ID of the search results object in the OTS
 */
void media_proxy_pl_search_results_id_cb(uint64_t id);

#ifdef __cplusplus
}
#endif

#endif /* ZEPHYR_INCLUDE_BLUETOOTH_HOST_AUDIO_MEDIA_PROXY_H_ */
