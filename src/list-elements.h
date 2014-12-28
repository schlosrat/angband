/**
 * \file list-elements.h
 * \brief Elements used in spells and other attacks.
 *
 * Fields:
 * name - text name 
 * desc - text description of attack
 * blind desc - text description of attack if blind
 * num - numerator for resistance
 * denom - denominator for resistance (random_value)
 * divisor - what to divide hp by to give breath damage
 * cap - breath damage cap
 * color - color of the effect
 */

/* name  		desc				blind desc			num denom           divisor	cap		color */
ELEM(ACID,		"acid",				"acid",				1,  RV(3,0,0,0),	3,		1600,	COLOUR_SLATE)
ELEM(ELEC,		"lightning",		"lightning",		1,  RV(3,0,0,0),	3,		1600,	COLOUR_BLUE)
ELEM(FIRE,		"fire",				"fire",				1,  RV(3,0,0,0),	3,		1600,	COLOUR_RED)
ELEM(COLD,		"cold",				"cold",				1,  RV(3,0,0,0),	3,		1600,	COLOUR_WHITE)
ELEM(POIS,		"poison",			"poison",			1,  RV(3,0,0,0),	3,		800,	COLOUR_GREEN)
ELEM(LIGHT,		"light",			"something",		4,  RV(6,1,6,0),	6,		400,	COLOUR_ORANGE)
ELEM(DARK,		"dark",				"something",		4,  RV(6,1,6,0),	6,		400,	COLOUR_L_DARK)
ELEM(SOUND,		"sound",			"noise",			5,  RV(6,1,6,0),	6,		500,	COLOUR_YELLOW)
ELEM(SHARD,		"shards",			"something sharp",	6,  RV(6,1,6,0),	6,		500,	COLOUR_UMBER)
ELEM(NEXUS,		"nexus",			"something strange",6,  RV(6,1,6,0),	6,		400,	COLOUR_L_RED)
ELEM(NETHER,	"nether",			"something cold",	6,  RV(6,1,6,0),	6,		550,	COLOUR_L_GREEN)
ELEM(CHAOS,		"chaos",			"something strange",6,	RV(6,1,6,0),	6,		500,	COLOUR_VIOLET)
ELEM(DISEN,		"disenchantment",	"something strange",6,  RV(6,1,6,0),	6,		500,	COLOUR_VIOLET)
ELEM(WATER,		"water",			"water",			0,	RV(0,0,0,0),	6,		0,		COLOUR_SLATE)
ELEM(ICE,		"ice",				"something sharp",	1,  RV(3,0,0,0),	6,		0,		COLOUR_WHITE)
ELEM(GRAVITY,	"gravity",			"something strange",0,	RV(0,0,0,0),	3,		200,	COLOUR_L_WHITE)
ELEM(INERTIA,	"inertia",			"something strange",0,	RV(0,0,0,0),	6,		200,	COLOUR_L_WHITE)
ELEM(FORCE,		"force",			"something hard",	0,	RV(0,0,0,0),	6,		200,	COLOUR_UMBER)
ELEM(TIME,		"time",				"something strange",0,	RV(0,0,0,0),	3,		150,	COLOUR_L_BLUE)
ELEM(PLASMA,	"plasma",			"something",		0,	RV(0,0,0,0),	6,		150,	COLOUR_RED)
ELEM(METEOR,	"a meteor",			"something",		0,	RV(0,0,0,0),	6,		0,		COLOUR_RED)
ELEM(MISSILE,	"a missile",		"something",		0,	RV(0,0,0,0),	6,		0,		COLOUR_VIOLET)
ELEM(MANA,		"mana",				"something",		0,	RV(0,0,0,0),	3,		1600,	COLOUR_L_DARK)
ELEM(HOLY_ORB,	"a holy orb",		"something",		0,	RV(0,0,0,0),	6,		0,		COLOUR_L_DARK)
ELEM(ARROW,		"an arrow",			"something sharp",	0,	RV(0,0,0,0),	6,		0,		COLOUR_WHITE)