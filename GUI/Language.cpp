// Language.cpp: implementation of the CLanguage class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Language.h"
#include "IniFile.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__ = ini.GetProfileString(_T(""), _T(""), _T(""));
#define new DEBUG_NEW
#endif


void CLanguage::LoadLanguage(LPCTSTR lpFile)
{
	if (lpFile == NULL || m_sPrevLangFile.CompareNoCase(lpFile) == 0)
		return;

	m_sPrevLangFile = lpFile;

	CIniFile ini(lpFile);

	//[Common]
	COMMON_OK = ini.GetProfileString(_T("Common"), _T("ok"), _T(""));
	COMMON_CANCEL = ini.GetProfileString(_T("Common"), _T("cancel"), _T(""));
	COMMON_YES = ini.GetProfileString(_T("Common"), _T("yes"), _T(""));
	COMMON_NO = ini.GetProfileString(_T("Common"), _T("no"), _T(""));
	COMMON_HELP = ini.GetProfileString(_T("Common"), _T("help"), _T(""));
	COMMON_CLOSE = ini.GetProfileString(_T("Common"), _T("close"), _T(""));
	COMMON_CLOSE_APP = ini.GetProfileString(_T("Common"), _T("close app"), _T(""));
	COMMON_SIG_SETTINGS = ini.GetProfileString(_T("Common"), _T("sig settings"), _T(""));
	COMMON_APPLY_SETTINGS_LATER = ini.GetProfileString(_T("Common"), _T("apply settings later"), _T(""));
	COMMON_NEW = ini.GetProfileString(_T("Common"), _T("new"), _T(""));
	COMMON_EDIT = ini.GetProfileString(_T("Common"), _T("edit"), _T(""));
	COMMON_REFRESH = ini.GetProfileString(_T("Common"), _T("refresh"), _T(""));
	COMMON_REMOVE = ini.GetProfileString(_T("Common"), _T("remove"), _T(""));
	COMMON_MS = ini.GetProfileString(_T("Common"), _T("millisecond"), _T(""));

	//[GUI]
	GUI_HELP_TOPIC = ini.GetProfileString(_T("GUI"), _T("help topic"), _T(""));
	GUI_ABOUT = ini.GetProfileString(_T("GUI"), _T(""), _T("about"));
	GUI_MULE_ENABLED = ini.GetProfileString(_T("GUI"), _T("mule enabled"), _T(""));
	GUI_MULE_DISABLED = ini.GetProfileString(_T("GUI"), _T("mule disabled"), _T(""));
	GUI_START = ini.GetProfileString(_T("GUI"), _T("start"), _T(""));
	GUI_PAUSE = ini.GetProfileString(_T("GUI"), _T("pause"), _T(""));
	GUI_RESUME = ini.GetProfileString(_T("GUI"), _T("resume"), _T(""));
	GUI_STOP = ini.GetProfileString(_T("GUI"), _T("stop"), _T(""));
	GUI_SETTINGS = ini.GetProfileString(_T("GUI"), _T("settings"), _T(""));
	GUI_SHOW_D2 = ini.GetProfileString(_T("GUI"), _T("activate d2 wnd"), _T(""));
	GUI_CLOSE_SIG = ini.GetProfileString(_T("GUI"), _T("close sig"), _T(""));
	GUI_SM_FAIL = ini.GetProfileString(_T("GUI"), _T("shared mem fail"), _T(""));
	GUI_INIT_FAIL = ini.GetProfileString(_T("GUI"), _T("init fail"), _T(""));

	//[Label Info]
	LBL_READY = ini.GetProfileString(_T("Label Info"), _T("ready"), _T(""));
	LBL_CREATE_SUCCESS = ini.GetProfileString(_T("Label Info"), _T("game create success"), _T(""));
	LBL_SIG_UNLOADED = ini.GetProfileString(_T("Label Info"), _T("sig unloaded"), _T(""));
	LBL_GAME_DROPPED = ini.GetProfileString(_T("Label Info"), _T("game dropped"), _T(""));
	LBL_CLOSING_D2 = ini.GetProfileString(_T("Label Info"), _T("closing d2"), _T(""));
	LBL_lAUNCH_IN_X = ini.GetProfileString(_T("Label Info"), _T("launching d2 in x"), _T(""));
	LBL_PREPARE_TO_LAUNCH = ini.GetProfileString(_T("Label Info"), _T("prepare to launch d2"), _T(""));
	LBL_LAUNCHING_D2 = ini.GetProfileString(_T("Label Info"), _T("launching d2"), _T(""));
	LBL_D2_FAIL = ini.GetProfileString(_T("Label Info"), _T("launch d2 failed"), _T(""));
	LBL_CHECK_D2 = ini.GetProfileString(_T("Label Info"), _T("check d2"), _T(""));
	LBL_SIG_LOAD_FAIL = ini.GetProfileString(_T("Label Info"), _T("sig load failed"), _T(""));
	LBL_CONNECT_FAIL = ini.GetProfileString(_T("Label Info"), _T("connect fail"), _T(""));
	LBL_LOGGING_IN = ini.GetProfileString(_T("Label Info"), _T("logging in"), _T(""));
	LBL_NULL_PASSWORD = ini.GetProfileString(_T("Label Info"), _T("null password"), _T(""));
	LBL_INVALID_PASSWORD = ini.GetProfileString(_T("Label Info"), _T("invalid password"), _T(""));
	LBL_CHECK_LOGIN = ini.GetProfileString(_T("Label Info"), _T("check login"), _T(""));
	LBL_LOGIN_SUCCESS = ini.GetProfileString(_T("Label Info"), _T("login success"), _T(""));
	LBL_LOGIN_FAIL = ini.GetProfileString(_T("Label Info"), _T("login failed"), _T(""));
	LBL_WAIT_CHAR_LIST = ini.GetProfileString(_T("Label Info"), _T("wait for char list"), _T(""));
	LBL_CHAR_LIST_SUCCESS = ini.GetProfileString(_T("Label Info"), _T("char list success"), _T(""));
	LBL_CHAR_LIST_FAIL = ini.GetProfileString(_T("Label Info"), _T("char list failed"), _T(""));
	LBL_INVALID_CHAR_POS = ini.GetProfileString(_T("Label Info"), _T("invalid char position"), _T(""));
	LBL_INVALID_USER_DATA = ini.GetProfileString(_T("Label Info"), _T("invalid user data"), _T(""));
	LBL_SEL_CHAR = ini.GetProfileString(_T("Label Info"), _T("select char"), _T(""));
	LBL_CHECK_CHAR_SEL = ini.GetProfileString(_T("Label Info"), _T("check char select"), _T(""));
	LBL_CHAR_SEL_FAIL = ini.GetProfileString(_T("Label Info"), _T("select char failed"), _T(""));
	LBL_ENTER_LOBBY = ini.GetProfileString(_T("Label Info"), _T("enter lobby"), _T(""));
	LBL_NOT_IN_LOBBY = ini.GetProfileString(_T("Label Info"), _T("char not in lobby"), _T(""));
	LBL_CREATING_GAME = ini.GetProfileString(_T("Label Info"), _T("creating game"), _T(""));
	LBL_CHECK_GAME = ini.GetProfileString(_T("Label Info"), _T("check game"), _T(""));
	LBL_CREATE_FAIL = ini.GetProfileString(_T("Label Info"), _T("create failed"), _T(""));
	LBL_NAME_INVALID = ini.GetProfileString(_T("Label Info"), _T("game name invalid"), _T(""));
	LBL_NAME_EXISTS = ini.GetProfileString(_T("Label Info"), _T("game name exists"), _T(""));
	LBL_CREATE_RETRY = ini.GetProfileString(_T("Label Info"), _T("create game retry"), _T(""));
	LBL_SKIP_CHAR = ini.GetProfileString(_T("Label Info"), _T("create fail skip char"), _T(""));
	LBL_EXIT_GAME = ini.GetProfileString(_T("Label Info"), _T("exiting game"), _T(""));
	LBL_CHECK_EXIT = ini.GetProfileString(_T("Label Info"), _T("check exit game"), _T(""));
	LBL_READY_FOR_NEXT_CHAR = ini.GetProfileString(_T("Label Info"), _T("ready for next char"), _T(""));
	LBL_READY_FOR_NEXT_ACCOUNT = ini.GetProfileString(_T("Label Info"), _T("ready for next account"), _T(""));
	LBL_RECREATE = ini.GetProfileString(_T("Label Info"), _T("recreating game"), _T(""));
	LBL_EXIT_LOBBY = ini.GetProfileString(_T("Label Info"), _T("exiting lobby"), _T(""));
	LBL_EXIT_CHAR_LIST = ini.GetProfileString(_T("Label Info"), _T("exiting char select"), _T(""));
	LBL_RECONNECT = ini.GetProfileString(_T("Label Info"), _T("reconnect"), _T(""));
	LBL_CLEAN_ERRORS = ini.GetProfileString(_T("Label Info"), _T("cleaning error"), _T(""));
	LBL_CLIENT_CRASH = ini.GetProfileString(_T("Label Info"), _T("client crashed"), _T(""));
	LBL_NO_MULE = ini.GetProfileString(_T("Label Info"), _T("no mule data"), _T(""));
	LBL_MULE_STARTED = ini.GetProfileString(_T("Label Info"), _T("mule mode started"), _T(""));
	LBL_NORMAL_STARTED = ini.GetProfileString(_T("Label Info"), _T("normal mode started"), _T(""));
	LBL_INIT_SIG = ini.GetProfileString(_T("Label Info"), _T("initializing sig"), _T(""));
	LBL_FILE_MISSING = ini.GetProfileString(_T("Label Info"), _T("file missing"), _T(""));
	LBL_MULE_FINISHED = ini.GetProfileString(_T("Label Info"), _T("mule mode finished"), _T(""));

	//[Messages]
	MSG_INVALID_CHAR_POS = ini.GetProfileString(_T("Messages"), _T("invalid char position"), _T(""));
	MSG_EXIT_CONFIRM = ini.GetProfileString(_T("Messages"), _T("exit confirm"), _T(""));
	MSG_CANNOT_RUN_MULE = ini.GetProfileString(_T("Messages"), _T("cannot run mule"), _T(""));
	MSG_PASSWORD_MISMATCH = ini.GetProfileString(_T("Messages"), _T("password mismatch"), _T(""));
	MSG_NULL_INPUT = ini.GetProfileString(_T("Messages"), _T("null input"), _T(""));
	MSG_INVALID_INPUT = ini.GetProfileString(_T("Messages"), _T("invalid input"), _T(""));
	MSG_INSUFFICIENT_LINES = ini.GetProfileString(_T("Messages"), _T("insufficient lines"), _T(""));
	MSG_WARN = ini.GetProfileString(_T("Messages"), _T("warn"), _T(""));

	//[General page]
	GENERAL_GENERAL = ini.GetProfileString(_T("General page"), _T("general"), _T(""));
	GENERAL_LOAD_D2 = ini.GetProfileString(_T("General page"), _T("load d2"), _T(""));
	GENERAL_D2_PATH = ini.GetProfileString(_T("General page"), _T("d2 path"), _T(""));
	GENERAL_BROWSE = ini.GetProfileString(_T("General page"), _T("browse"), _T(""));
	GENERAL_PARAMS = ini.GetProfileString(_T("General page"), _T("parameters"), _T(""));
	GENERAL_W = ini.GetProfileString(_T("General page"), _T("w"), _T(""));
	GENERAL_SKIPTOBNET = ini.GetProfileString(_T("General page"), _T("skiptobnet"), _T(""));
	GENERAL_NOPLUGIN = ini.GetProfileString(_T("General page"), _T("noplugin"), _T(""));
	GENERAL_RES800 = ini.GetProfileString(_T("General page"), _T("res800"), _T(""));
	GENERAL_LQ = ini.GetProfileString(_T("General page"), _T("lq"), _T(""));
	GENERAL_NS = ini.GetProfileString(_T("General page"), _T("ns"), _T(""));
	GENERAL_LOCALE = ini.GetProfileString(_T("General page"), _T("locale"), _T(""));
	GENERAL_MPQ = ini.GetProfileString(_T("General page"), _T("mpq"), _T(""));
	GENERAL_MIN_D2 = ini.GetProfileString(_T("General page"), _T("minimize d2"), _T(""));
	GENERAL_NO_PLUGIN_WARN = ini.GetProfileString(_T("General page"), _T("no plugin warn"), _T(""));

	//[Login page]
	LOGIN_REALM = ini.GetProfileString(_T("Login page"), _T("realm"), _T(""));
	LOGIN_ACCOUNT = ini.GetProfileString(_T("Login page"), _T("account"), _T(""));
	LOGIN_ACCOUNT_NAME = ini.GetProfileString(_T("Login page"), _T("account name"), _T(""));
	LOGIN_PASSWORD = ini.GetProfileString(_T("Login page"), _T("password"), _T(""));
	LOGIN_REPEAT_PASSWORD = ini.GetProfileString(_T("Login page"), _T("repeat password"), _T(""));
	LOGIN_CHAR_POS = ini.GetProfileString(_T("Login page"), _T("char position"), _T(""));
	LOGIN_CHAR1 = ini.GetProfileString(_T("Login page"), _T("char 1"), _T(""));
	LOGIN_CHAR2 = ini.GetProfileString(_T("Login page"), _T("char 2"), _T(""));
	LOGIN_CHAR3 = ini.GetProfileString(_T("Login page"), _T("char 3"), _T(""));
	LOGIN_CHAR4 = ini.GetProfileString(_T("Login page"), _T("char 4"), _T(""));
	LOGIN_CHAR5 = ini.GetProfileString(_T("Login page"), _T("char 5"), _T(""));
	LOGIN_CHAR6 = ini.GetProfileString(_T("Login page"), _T("char 6"), _T(""));
	LOGIN_CHAR7 = ini.GetProfileString(_T("Login page"), _T("char 7"), _T(""));
	LOGIN_CHAR8 = ini.GetProfileString(_T("Login page"), _T("char 8"), _T(""));

	//[Create Page]
	CREATE_CHANNEL = ini.GetProfileString(_T("Create Page"), _T("channel"), _T(""));
	CREATE_GAME_CREATE = ini.GetProfileString(_T("Create Page"), _T("game create"), _T(""));
	CREATE_USE_RANDOM = ini.GetProfileString(_T("Create Page"), _T("use random"), _T(""));
	CREATE_USE_PREFIX = ini.GetProfileString(_T("Create Page"), _T("use prefix"), _T(""));
	CREATE_USE_DIC = ini.GetProfileString(_T("Create Page"), _T("use dictionary"), _T(""));
	CREATE_EDIT_DIC = ini.GetProfileString(_T("Create Page"), _T("edit dictionary"), _T(""));
	CREATE_PASSWORD_LEN = ini.GetProfileString(_T("Create Page"), _T("password length"), _T(""));
	CREATE_RETRY_TIMES = ini.GetProfileString(_T("Create Page"), _T("retry times"), _T(""));
	CREATE_DIFFICULTY = ini.GetProfileString(_T("Create Page"), _T("difficulty"), _T(""));
	CREATE_CONTROL = ini.GetProfileString(_T("Create Page"), _T("control"), _T(""));
	CREATE_USE_AI = ini.GetProfileString(_T("Create Page"), _T("use anti idle"), _T(""));
	CREATE_INGAME_LIMIT = ini.GetProfileString(_T("Create Page"), _T("in game limit"), _T(""));

	//[Delay page]
	DELAY_FRM_MAX_DELAY = ini.GetProfileString(_T("Delay page"), _T("frame max delay"), _T(""));
	DELAY_LAUNCH_DELAY = ini.GetProfileString(_T("Delay page"), _T("launch delay"), _T(""));
	DELAY_LOGIN_DELAY = ini.GetProfileString(_T("Delay page"), _T("login delay"), _T(""));
	DELAY_CHARSEL_DELAY = ini.GetProfileString(_T("Delay page"), _T("char sel delay"), _T(""));
	DELAY_CREATE_DELAY = ini.GetProfileString(_T("Delay page"), _T("create delay"), _T(""));
	DELAY_EXIT_DELAY = ini.GetProfileString(_T("Delay page"), _T("exit delay"), _T(""));
	DELAY_MAX_DELAY_BTN = ini.GetProfileString(_T("Delay page"), _T("max delay button"), _T(""));
	DELAY_FRM_MIN_DELAY = ini.GetProfileString(_T("Delay page"), _T("frame min delay"), _T(""));
	DELAY_CONNECT_DELAY = ini.GetProfileString(_T("Delay page"), _T("connect delay"), _T(""));
	DELAY_BTN_DELAY = ini.GetProfileString(_T("Delay page"), _T("button delay"), _T(""));
	DELAY_ERROR_DELAY = ini.GetProfileString(_T("Delay page"), _T("error delay"), _T(""));
	DELAY_MIN_DELAY_BTN = ini.GetProfileString(_T("Delay page"), _T("min delay button"), _T(""));
	DELAY_RESTORE_DEFAULT = ini.GetProfileString(_T("Delay page"), _T("restore default"), _T(""));
	DELAY_TOOLTIP_BTN = ini.GetProfileString(_T("Delay page"), _T("tooltip delay desc"), _T(""));
	DELAY_TOOLTIP_DEFAULT = ini.GetProfileString(_T("Delay page"), _T("tooltip default"), _T(""));
	DELAY_MAX_DELAY = ini.GetProfileString(_T("Delay page"), _T("max delay"), _T(""));
	DELAY_MAX_DELAY_DESC = ini.GetProfileString(_T("Delay page"), _T("max delay desc"), _T(""));
	DELAY_MIN_DELAY_DESC = ini.GetProfileString(_T("Delay page"), _T("min delay desc"), _T(""));

	//[Mule Page]
	MULE_LIST = ini.GetProfileString(_T("Mule Page"), _T("mule list"), _T(""));
	MULE_STAY_LEN = ini.GetProfileString(_T("Mule Page"), _T("stay length"), _T(""));
	MULE_ADD = ini.GetProfileString(_T("Mule Page"), _T("add account"), _T(""));
	MULE_EDIT = ini.GetProfileString(_T("Mule Page"), _T("edit account"), _T(""));
	MULE_REMOVE = ini.GetProfileString(_T("Mule Page"), _T("remove account"), _T(""));
	MULE_MOVE_UP = ini.GetProfileString(_T("Mule Page"), _T("move up"), _T(""));
	MULE_mOVE_DOWN = ini.GetProfileString(_T("Mule Page"), _T("move down"), _T(""));
	MULE_TOOLTIP_LIST = ini.GetProfileString(_T("Mule Page"), _T("list tooltip"), _T(""));

	//[Hacks Page]
	HACKS_LIST = ini.GetProfileString(_T("Hacks Page"), _T("hack list"), _T(""));

	//[D2JSP Page]
	D2JSP_LOAD = ini.GetProfileString(_T("D2JSP Page"), _T("load d2jsp"), _T(""));
	D2JSP_NONE = ini.GetProfileString(_T("D2JSP Page"), _T("none"), _T(""));
	D2JSP_LOAD_DESC = ini.GetProfileString(_T("D2JSP Page"), _T("load d2jsp desc"), _T(""));
	D2JSP_LOAD_SCRIPTS = ini.GetProfileString(_T("D2JSP Page"), _T("load scripts"), _T(""));
	D2JSP_AUTO = ini.GetProfileString(_T("D2JSP Page"), _T("auto mode"), _T(""));
	D2JSP_MANUAL = ini.GetProfileString(_T("D2JSP Page"), _T("manual mode"), _T(""));
	D2JSP_LIST = ini.GetProfileString(_T("D2JSP Page"), _T("scripts list"), _T(""));

	//[About]
	ABOUT_COPYRIGHT = ini.GetProfileString(_T("About"), _T("copyright"), _T(""));
}
