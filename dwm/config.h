/* See LICENSE file for copyright and license details. */
#include <X11/XF86keysym.h>

/* Constants */
#define TERMINAL "st"
#define TERMCLASS "st"
#define BROWSER "qutebrowser"

/* appearance */
/* static const char *light_up[] = {"/usr/bin/brightnessctl", "set", "5%+", NULL};
static const char *light_down[] = {"/usr/bin/brightnessctl", "set", "5%-", NULL}; */
static const char *upvol[] = { "/usr/bin/amixer", "set", "Master", "5%+", NULL };
static const char *downvol[] = { "/usr/bin/amixer", "set", "Master", "5%-", NULL };
static const char *mutevol[] = { "/usr/bin/amixer", "set", "Master", "toggle", NULL };
static const unsigned int borderpx  = 3;        /* border pixel of windows */
static const unsigned int gappx     = 5;        /* gaps between windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]          = { "monospace:size=12","fontawesome=12" };
static const char dmenufont[]       = "monospace:size=12";
static const char col_gray1[]       = "#222222"; /** norm background color */
static const char col_gray2[]       = "#444444"; /** norm border color */
static const char col_gray3[]       = "#bbbbbb"; /** norm foreground color or font color */
static const char col_gray4[]       = "#222222"; /** selected foreground color or font color original col : #eeeeee */
static const char col_cyan[]        = "#86eb34"; /** selected background color, original col : #005577 */
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_gray3, col_gray1, col_gray2 },
	[SchemeSel]  = { col_gray4, col_cyan,  col_cyan  },
};
static const XPoint stickyicon[]    = { {0,0}, {4,0}, {4,8}, {2,6}, {0,8}, {0,0} }; /* represents the icon as an array of vertices */
static const XPoint stickyiconbb    = {4,8};	/* defines the bottom right corner of the polygon's bounding box (speeds up scaling) */

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   monitor */
	{ "Gimp",     NULL,       NULL,       0,            1,           -1 },
	{ "Firefox",  NULL,       NULL,       1 << 8,       0,           -1 },
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
	{ "[D]",      deck },
	{ "TTT",      bstack },
	{ "===",      bstackhoriz },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run_history", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_cyan, "-sf", col_gray4, NULL };
static const char *termcmd[]  = { "st", NULL };


static Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                  	  XK_F11, 	 spawn, 				 SHCMD("brightnessctl set 5%+")},
	{ MODKEY|ShiftMask,        	  XK_F11, 	 spawn, 				 SHCMD("brightnessctl set 5%-")},
	{ 0,		  XF86XK_MonBrightnessUp, 	 spawn,				 	 SHCMD("brightnessctl set 5%+")},
	{ 0,		XF86XK_MonBrightnessDown,	 spawn,		 		 	 SHCMD("brightnessctl set 5%-")},
	{ 0,        XF86XK_AudioLowerVolume, 	 spawn, 				 {.v = downvol } },
	{ 0,  		       XF86XK_AudioMute,	 spawn, 				 {.v = mutevol } },
	{ 0,        XF86XK_AudioRaiseVolume, 	 spawn, 				 {.v = upvol   } },
	{ MODKEY,                     XK_F9,	 spawn, 				 {.v = downvol } },
	{ MODKEY|ShiftMask,           XK_F9,  	 spawn, 				 {.v = mutevol } },
	{ MODKEY,                     XK_F10, 	 spawn, 				 {.v = upvol   } },
	/** { MODKEY|ShiftMask,         XK_F11, 	 spawn, 				 SHCMD(TERMINAL " brightnessctl set 5%-")}, */
	{ MODKEY|ShiftMask,           XK_w,		 spawn, 				 SHCMD("qutebrowser")},
	{ MODKEY,      			      XK_w,		 spawn, 				 SHCMD("firefox")},
	{ MODKEY,             XK_backslash,	 	 spawn, 				 SHCMD("clipmenu")},
	{ MODKEY,                  	  XK_e,		 spawn, 				 SHCMD(TERMINAL " -e ranger")},
	{ MODKEY|ShiftMask,           XK_e, 	 spawn, 				 SHCMD("thunar")},
	{ MODKEY,                  	  XK_Escape, spawn, 				 SHCMD("slock")},
	{ MODKEY|ShiftMask,        	  XK_Escape, spawn, 				 SHCMD("sudo systemctl suspend; slock")},
	{ 0,						  XK_Print,	 spawn,				 	 SHCMD("ksnip -s")},
	{ MODKEY,                       XK_p,      spawn, 	          {.v = dmenucmd } },
	{ MODKEY|ShiftMask,             XK_p,      spawn,	          SHCMD("linux-config-file")}, 
	{ MODKEY,					    XK_Return, spawn,      	  	 SHCMD(TERMINAL " -e fish") },
	{ MODKEY|ShiftMask,             XK_Return, spawn,          	 {.v = termcmd } },
	{ MODKEY,                       XK_b,      togglebar,      	 {0} },
	{ MODKEY,                       XK_j,      focusstack,     	 {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     	 {.i = -1 } },
	{ MODKEY,                       XK_i,      incnmaster,     	 {.i = +1 } },
	{ MODKEY,                       XK_d,      incnmaster,     	 {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       	 {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       	 {.f = +0.05} },
	{ MODKEY,			              XK_c,		 zoom,           	 {0} },
	{ MODKEY,                       XK_Tab,    view,           	 {0} },
	{ MODKEY,			              XK_q,      killclient,     	 {0} },
/**{ MODKEY,                       XK_comma,  scratchpad_show,  {.i = 1} },
	{ MODKEY,                       XK_period, scratchpad_show,  {.i = 2} }, */
	{ MODKEY,                       XK_slash,  scratchpad_show,  {.i = 3} },
/**{ MODKEY|ShiftMask,             XK_comma,  scratchpad_hide,  {.i = 1} },
	{ MODKEY|ShiftMask,             XK_period, scratchpad_hide,  {.i = 2} }, */
	{ MODKEY|ShiftMask,             XK_slash,  scratchpad_hide,  {.i = 3} },
/**{ MODKEY|ShiftMask,        XK_apostrophe,scratchpad_remove,       {0} }, */

	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_f,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_r,      setlayout,      {.v = &layouts[3]} },
	{ MODKEY,                       XK_u,      setlayout,      {.v = &layouts[4]} },
	{ MODKEY,                       XK_o,      setlayout,      {.v = &layouts[5]} },
	{ MODKEY,                       XK_space,  setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
	{ MODKEY,                       XK_s,      togglesticky,   {0} },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
	{ MODKEY,                       XK_minus,  setgaps,        {.i = -1 } },
	{ MODKEY,                       XK_equal,  setgaps,        {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_equal,  setgaps,        {.i = 0  } },
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
	{ MODKEY|ShiftMask,	           XK_F12,    quit,           {0} },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

