/* See LICENSE file for copyright and license details. */

/* appearance */
static const unsigned int borderpx  = 3;        /* border pixel of windows */
static const unsigned int snap      = 8;       /* snap pixel */
static const unsigned int systraypinning = 1;   /* 0: sloppy systray follows selected monitor, >0: pin systray to monitor X */
static const unsigned int systrayonleft = 1;    /* 0: systray in the right corner, >0: systray on left of status text */
static const unsigned int systrayspacing = 0;   /* systray spacing */
static const int systraypinningfailfirst = 1;   /* 1: if pinning fails, display systray on the first monitor, False: display systray on the last monitor*/
static const int showsystray        = 1;        /* 0 means no systray */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char font[]            = { "monospace:size=9" };
static const char *fonts[]          = { "mono:size=9" };
static const char dmenufont[]       = "monospace:size=9";
static const char col_gray1[]       = "#000000";
static const char col_gray2[]       = "#444444";
static const char col_gray3[]       = "#bbbbbb";
static const char col_gray4[]       = "#eeeeee";
static const char col_cyan[]        = "#005577";
static const char col_silver[]      = "#a1a1a1";
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_gray3, col_gray1, col_gray2 },
	[SchemeSel]  = { col_gray1, col_silver, col_silver },
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6"};

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   monitor */
	{ "Gimp",     NULL,       NULL,       0,            1,           -1 },
	{ "Firefox",  NULL,       NULL,       1 << 8,       0,           -1 },
    {  "qutebrowser",     "floating",        NULL,  0,     1,  -1  },
    {  "floating",     NULL,        NULL,  0,     1,  -1  },
    {  "float",        NULL,        NULL,  0,     1,  -1  },
    {  "mpv",          "float",     NULL,  0,     1,  -1  },
    {  "mpv",          "floating",  NULL,  0,     1,  -1  },
    {  "feh",          "float",     NULL,  0,     1,  -1  },
    {  "zoom",         NULL,        NULL,  0,     1,  -1  },
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
};

/* key definitions */
#define MODKEY Mod4Mask
#define AltMask Mod1Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, NULL };
static const char *dmenulauncher[] = { "dmenu_launcher", "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_silver, "-sf", col_gray1, NULL };
static const char *dmenuoneliners[] = { "dmenu_oneliners", "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_silver, "-sf", col_gray1, NULL };
static const char *dmenubrowserseach[] = { "dmenu_browsersearch", "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_silver, "-sf", col_gray1, NULL };
static const char *dmenubookmarks[] = { "dmenu_bookmark", "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_silver, "-sf", col_gray1, NULL };
static const char *dmenuopenfile[] = { "dmenu_openfile", "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_silver, "-sf", col_gray1, NULL };
static const char *dmenubibtex[] = { "dmenu_bibtex", "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_silver, "-sf", col_gray1, NULL };
static const char *dmenussh[] = { "dmenu_ssh", "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_silver, "-sf", col_gray1, NULL };
static const char *dmenusysact[] = { "dmenu_sysact", "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_silver, "-sf", col_gray1, NULL };
static const char *dmenucharmap[] = { "dmenu_charmap", "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_silver, "-sf", col_gray1, NULL };
static const char *dmenuclipcat[] = { "/usr/bin/clipcat-menu", "--dmenu-extra-arguments", "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_silver, "-sf", col_gray1, NULL };
static const char *termcmd[]  = { "st", NULL };
static const char *termcmdfloat[]  = { "st","-c","floating","-g","100x22+0-200", NULL };

#include <X11/XF86keysym.h>
static Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_a,      spawn,          {.v = dmenubookmarks } },
	{ MODKEY,                       XK_s,      spawn,          {.v = dmenubrowserseach } },
	{ MODKEY,                       XK_d,      spawn,          {.v = dmenulauncher } },
	{ MODKEY|ShiftMask,             XK_d,      spawn,          {.v = dmenuoneliners } },
	{ MODKEY,                       XK_p,      spawn,          {.v = dmenuopenfile } },
	{ MODKEY|ShiftMask,             XK_p,      spawn,          {.v = dmenubibtex } },
	{ MODKEY,                       XK_End,    spawn,          {.v = dmenusysact } },
	{ MODKEY|ShiftMask,             XK_k,      spawn,          {.v = dmenucharmap } },
	{ MODKEY,             XK_c,      spawn,          SHCMD("clipcat-menu --dmenu-menu-length=5") },
	{ MODKEY,                       XK_Return, spawn,          {.v = termcmd } },
	{ ControlMask|AltMask,          XK_t,      spawn,          {.v = termcmd } },
	{ MODKEY|ShiftMask,             XK_Return, spawn,          {.v = termcmdfloat } },
	{ MODKEY,              XK_b,      togglebar,      {0} },
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_m,      incnmaster,     {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_m,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
    { MODKEY,                       XK_n,      spawn,          SHCMD("dunstctl close") },
	{ MODKEY,                       XK_space,  zoom,           {0} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY|ShiftMask,             XK_q,      killclient,     {0} },
    { MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
    { MODKEY,                       XK_y,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_f,      togglefloating, {0} },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
	{ MODKEY|ShiftMask,             XK_r,      quit,           {1} },
    // Audio
	{ 0, XF86XK_AudioMute,	    	spawn,		SHCMD("pamixer -t") },
	{ 0, XF86XK_AudioRaiseVolume,	spawn,		SHCMD("change-volume +5 unmute") },
	{ 0, XF86XK_AudioLowerVolume,	spawn,		SHCMD("change-volume -5 unmute") },
	{ 0, XF86XK_AudioPrev,		    spawn,		SHCMD("mpc prev") },
	{ 0, XF86XK_AudioNext,		    spawn,		SHCMD("mpc next") },
	{ 0, XF86XK_AudioPause,		    spawn,		SHCMD("mpc toggle") },
	{ 0, XF86XK_AudioPlay,		    spawn,		SHCMD("mpc toggle") },
	{ 0, XF86XK_AudioStop,		    spawn,		SHCMD("mpc stop") },
	{ 0, XF86XK_AudioRewind,	    spawn,		SHCMD("mpc seek -10") },
	{ 0, XF86XK_AudioForward,	    spawn,		SHCMD("mpc seek +10") },
	{ 0, XF86XK_AudioMicMute,	    spawn,		SHCMD("pactl set-source-mute @DEFAULT_SOURCE@ toggle") },
    // Brightness
	{ 0, XF86XK_MonBrightnessUp,	spawn,		SHCMD("change-screen-brightness -inc 5") },
	{ 0, XF86XK_MonBrightnessDown,	spawn,		SHCMD("change-screen-brightness -dec 5") },
    // System
	{ 0, XF86XK_PowerOff,		    spawn,		SHCMD("sysact") },
	{ 0,                            XK_Print,	spawn,	   SHCMD("flameshot gui") },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static const Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
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
