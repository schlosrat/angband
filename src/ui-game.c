/**
 * \file ui-game.c
 * \brief Game management for the traditional text UI
 *
 * Copyright (c) 1997 Ben Harrison, James E. Wilson, Robert A. Koeneke
 * Copyright (c) 2015 Nick McConnell
 *
 * This work is free software; you can redistribute it and/or modify it
 * under the terms of either:
 *
 * a) the GNU General Public License as published by the Free Software
 *    Foundation, version 2, or
 *
 * b) the "Angband licence":
 *    This software may be copied and distributed for educational, research,
 *    and not for profit purposes provided that this copyright and statement
 *    are included in all such copies.  Other copyrights may also apply.
 */

#include "angband.h"
#include "cmds.h"
#include "dungeon.h"
#include "keymap.h"
#include "obj-util.h"
#include "player-attack.h"
#include "player-path.h"
#include "player-util.h"
#include "ui-command.h"
#include "ui-context.h"
#include "ui-display.h"
#include "ui-help.h"
#include "ui-input.h"
#include "ui-knowledge.h"
#include "ui-map.h"
#include "ui-obj.h"
#include "ui-object.h"
#include "ui-player.h"
#include "ui-prefs.h"
#include "ui-spell.h"
#include "ui-store.h"
#include "ui-target.h"
#include "ui.h"


/**
 * Here are lists of commands, stored in this format so that they can be
 * easily maniuplated for e.g. help displays, or if a port wants to provide a
 * native menu containing a command list.
 *
 * Consider a two-paned layout for the command menus. XXX
 */

/**
 * Item commands
 */
struct cmd_info cmd_item[] =
{
	{ "Inscribe an object", { '{' }, CMD_INSCRIBE },
	{ "Uninscribe an object", { '}' }, CMD_UNINSCRIBE },
	{ "Wear/wield an item", { 'w' }, CMD_WIELD },
	{ "Take off/unwield an item", { 't', 'T'}, CMD_TAKEOFF },
	{ "Examine an item", { 'I' }, CMD_NULL, textui_obj_examine },
	{ "Drop an item", { 'd' }, CMD_DROP },
	{ "Fire your missile weapon", { 'f', 't' }, CMD_FIRE, NULL, player_can_fire_prereq },
	{ "Use a staff", { 'u', 'Z' }, CMD_USE_STAFF },
	{ "Aim a wand", {'a', 'z'}, CMD_USE_WAND },
	{ "Zap a rod", {'z', 'a'}, CMD_USE_ROD },
	{ "Activate an object", {'A' }, CMD_ACTIVATE },
	{ "Eat some food", { 'E' }, CMD_EAT },
	{ "Quaff a potion", { 'q' }, CMD_QUAFF },
	{ "Read a scroll", { 'r' }, CMD_READ_SCROLL, NULL, player_can_read_prereq },
	{ "Fuel your light source", { 'F' }, CMD_REFILL, NULL, player_can_refuel_prereq },
	{ "Use an item", { 'U', 'X' }, CMD_USE }
};

/**
 * General actions
 */
struct cmd_info cmd_action[] =
{
	{ "Search for traps/doors", { 's' }, CMD_SEARCH },
	{ "Disarm a trap or chest", { 'D' }, CMD_DISARM },
	{ "Rest for a while", { 'R' }, CMD_NULL, textui_cmd_rest },
	{ "Look around", { 'l', 'x' }, CMD_NULL, do_cmd_look },
	{ "Target monster or location", { '*' }, CMD_NULL, textui_target },
	{ "Target closest monster", { '\'' }, CMD_NULL, textui_target_closest },
	{ "Dig a tunnel", { 'T', KTRL('T') }, CMD_TUNNEL },
	{ "Go up staircase", {'<' }, CMD_GO_UP },
	{ "Go down staircase", { '>' }, CMD_GO_DOWN },
	{ "Toggle search mode", { 'S', '#' }, CMD_TOGGLE_SEARCH },
	{ "Open a door or a chest", { 'o' }, CMD_OPEN },
	{ "Close a door", { 'c' }, CMD_CLOSE },
	{ "Fire at nearest target", { 'h', KC_TAB }, CMD_NULL, do_cmd_fire_at_nearest },
	{ "Throw an item", { 'v' }, CMD_THROW },
	{ "Walk into a trap", { 'W', '-' }, CMD_JUMP, NULL },
};

/**
 * Item management commands
 */
struct cmd_info cmd_item_manage[] =
{
	{ "Display equipment listing", { 'e' }, CMD_NULL, do_cmd_equip },
	{ "Display inventory listing", { 'i' }, CMD_NULL, do_cmd_inven },
	{ "Pick up objects", { 'g' }, CMD_PICKUP, NULL },
	{ "Ignore an item", { 'k', KTRL('D') }, CMD_IGNORE, textui_cmd_ignore },	
};

/**
 * Information access commands
 */
struct cmd_info cmd_info[] =
{
	{ "Browse a book", { 'b', 'P' }, CMD_BROWSE_SPELL, textui_spell_browse },
	{ "Gain new spells", { 'G' }, CMD_STUDY, NULL, player_can_study_prereq },
	{ "Cast a spell", { 'm' }, CMD_CAST, NULL, player_can_cast_prereq },
	{ "Cast a spell", { 'p' }, CMD_CAST, NULL, player_can_cast_prereq },
	{ "Full dungeon map", { 'M' }, CMD_NULL, do_cmd_view_map },
	{ "Toggle ignoring of items", { 'K', 'O' }, CMD_NULL, textui_cmd_toggle_ignore },
	{ "Display visible item list", { ']' }, CMD_NULL, do_cmd_itemlist },
	{ "Display visible monster list", { '[' }, CMD_NULL, do_cmd_monlist },
	{ "Locate player on map", { 'L', 'W' }, CMD_NULL, do_cmd_locate },
	{ "Help", { '?' }, CMD_NULL, do_cmd_help },
	{ "Identify symbol", { '/' }, CMD_NULL, do_cmd_query_symbol },
	{ "Character description", { 'C' }, CMD_NULL, do_cmd_change_name },
	{ "Check knowledge", { '~' }, CMD_NULL, textui_browse_knowledge },
	{ "Repeat level feeling", { KTRL('F') }, CMD_NULL, do_cmd_feeling },
	{ "Show previous message", { KTRL('O') }, CMD_NULL, do_cmd_message_one },
	{ "Show previous messages", { KTRL('P') }, CMD_NULL, do_cmd_messages }
};

/**
 * Utility/assorted commands
 */
struct cmd_info cmd_util[] =
{
	{ "Interact with options", { '=' }, CMD_NULL, do_cmd_xxx_options },

	{ "Save and don't quit", { KTRL('S') }, CMD_NULL, save_game },
	{ "Save and quit", { KTRL('X') }, CMD_NULL, textui_quit },
	{ "Quit (commit suicide)", { 'Q' }, CMD_NULL, textui_cmd_suicide },
	{ "Redraw the screen", { KTRL('R') }, CMD_NULL, do_cmd_redraw },

	{ "Load \"screen dump\"", { '(' }, CMD_NULL, do_cmd_load_screen },
	{ "Save \"screen dump\"", { ')' }, CMD_NULL, do_cmd_save_screen }
};

/**
 * Commands that shouldn't be shown to the user
 */
struct cmd_info cmd_hidden[] =
{
	{ "Take notes", { ':' }, CMD_NULL, do_cmd_note },
	{ "Version info", { 'V' }, CMD_NULL, do_cmd_version },
	{ "Load a single pref line", { '"' }, CMD_NULL, do_cmd_pref },
	{ "Enter a store", { '_' }, CMD_NULL, textui_enter_store },
	{ "Toggle windows", { KTRL('E') }, CMD_NULL, toggle_inven_equip }, /* XXX */
	{ "Alter a grid", { '+' }, CMD_ALTER, NULL },
	{ "Walk", { ';' }, CMD_WALK, NULL },
	{ "Start running", { '.', ',' }, CMD_RUN, NULL },
	{ "Stand still", { ',', '.' }, CMD_HOLD, NULL },
	{ "Center map", { KTRL('L'), '@' }, CMD_NULL, do_cmd_center_map },
	{ "Toggle wizard mode", { KTRL('W') }, CMD_NULL, do_cmd_wizard },
	{ "Repeat previous command", { 'n', KTRL('V') }, CMD_REPEAT, NULL },
	{ "Do autopickup", { KTRL('G') }, CMD_AUTOPICKUP, NULL },
	{ "Debug mode commands", { KTRL('A') }, CMD_NULL, do_cmd_try_debug },
};

/**
 * List of command lists */
struct command_list cmds_all[] =
{
	{ "Items",           cmd_item,        N_ELEMENTS(cmd_item) },
	{ "Action commands", cmd_action,      N_ELEMENTS(cmd_action) },
	{ "Manage items",    cmd_item_manage, N_ELEMENTS(cmd_item_manage) },
	{ "Information",     cmd_info,        N_ELEMENTS(cmd_info) },
	{ "Utility",         cmd_util,        N_ELEMENTS(cmd_util) },
	{ "Hidden",          cmd_hidden,      N_ELEMENTS(cmd_hidden) },
	{ 0 }
};



/*** Exported functions ***/

#define KEYMAP_MAX 2

/* List indexed by char */
static struct cmd_info *converted_list[KEYMAP_MAX][UCHAR_MAX+1];


/**
 * Initialise the command list.
 */
void cmd_init(void)
{
	size_t i, j;

	memset(converted_list, 0, sizeof(converted_list));

	/* Go through all generic commands (-1 for NULL end entry) */
	for (j = 0; j < N_ELEMENTS(cmds_all) - 1; j++)
	{
		struct cmd_info *commands = cmds_all[j].list;

		/* Fill everything in */
		for (i = 0; i < cmds_all[j].len; i++) {
			/* If a roguelike key isn't set, use default */
			if (!commands[i].key[1])
				commands[i].key[1] = commands[i].key[0];

			converted_list[0][commands[i].key[0]] = &commands[i];
			converted_list[1][commands[i].key[1]] = &commands[i];
		}
	}
}

unsigned char cmd_lookup_key(cmd_code lookup_cmd, int mode)
{
	unsigned int i;

	assert(mode == KEYMAP_MODE_ROGUE || mode == KEYMAP_MODE_ORIG);

	for (i = 0; i < N_ELEMENTS(converted_list[mode]); i++) {
		struct cmd_info *cmd = converted_list[mode][i];

		if (cmd && cmd->cmd == lookup_cmd)
			return cmd->key[mode];
	}

	return 0;
}

unsigned char cmd_lookup_key_unktrl(cmd_code lookup_cmd, int mode)
{
	unsigned char c = cmd_lookup_key(lookup_cmd, mode);

	if (c < 0x20)
		c = UN_KTRL(c);

	return c;
}

cmd_code cmd_lookup(unsigned char key, int mode)
{
	assert(mode == KEYMAP_MODE_ROGUE || mode == KEYMAP_MODE_ORIG);

	if (!converted_list[mode][key])
		return CMD_NULL;

	return converted_list[mode][key]->cmd;
}


/**
 * Parse and execute the current command
 * Give "Warning" on illegal commands.
 */
void textui_process_command(void)
{
	int count = 0;
	bool done = TRUE;
	ui_event e = textui_get_command(&count);
	struct cmd_info *cmd = NULL;
	unsigned char key = '\0';
	int mode = OPT(rogue_like_commands) ? KEYMAP_MODE_ROGUE : KEYMAP_MODE_ORIG;

	switch (e.type) {
		case EVT_RESIZE: do_cmd_redraw(); return;
		case EVT_MOUSE: textui_process_click(e); return;
		case EVT_BUTTON:
		case EVT_KBRD: done = textui_process_key(e.key, &key, count); break;
		default: ;
	}

	/* Null command */
	if (!key && done)
		return;

	if (key == KC_ENTER) {
		/* Use command menus */
		cmd = textui_action_menu_choose();
	} else {
		/* Command key */
		cmd = converted_list[mode][key];
	}

	if (cmd && done) {
		/* Confirm for worn equipment inscriptions, check command prereqs */
		if (!key_confirm_command(key) || (cmd->prereq && !cmd->prereq()))
			cmd = NULL;

		/* Split on type of command */
		if (cmd->hook)
			/* UI command */
			cmd->hook();
		else if (cmd->cmd)
			/* Game command */
			cmdq_push_repeat(cmd->cmd, count);
	} else
		/* Error */
		do_cmd_unknown();
}

errr textui_get_cmd(cmd_context context)
{
	if (context == CMD_GAME)
		textui_process_command();

	/* If we've reached here, we haven't got a command. */
	return 1;
}

