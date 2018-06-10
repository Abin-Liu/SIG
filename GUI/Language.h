// Language.h: interface for the CLanguage class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LANGUAGE_H__99A2EC1B_260E_4828_88B4_71D9BA060AB6__INCLUDED_)
#define AFX_LANGUAGE_H__99A2EC1B_260E_4828_88B4_71D9BA060AB6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CLanguage  
{
public:
	void LoadLanguage(LPCTSTR lpFile);

	//[Common]
	CString COMMON_OK;
	CString COMMON_CANCEL;
	CString COMMON_YES;
	CString COMMON_NO;
	CString COMMON_HELP;
	CString COMMON_CLOSE;
	CString COMMON_CLOSE_APP;
	CString COMMON_SIG_SETTINGS;
	CString COMMON_APPLY_SETTINGS_LATER;
	CString COMMON_NEW;
	CString COMMON_EDIT;
	CString COMMON_REFRESH;
	CString COMMON_REMOVE;
	CString COMMON_MS;

	//[GUI]
	CString GUI_HELP_TOPIC;
	CString GUI_ABOUT;
	CString GUI_MULE_ENABLED;
	CString GUI_MULE_DISABLED;
	CString GUI_START;
	CString GUI_PAUSE;
	CString GUI_RESUME;
	CString GUI_STOP;
	CString GUI_SETTINGS;
	CString GUI_SHOW_D2;
	CString GUI_CLOSE_SIG;
	CString GUI_SM_FAIL;
	CString GUI_INIT_FAIL;

	//[Label Info]
	CString LBL_READY;
	CString LBL_CREATE_SUCCESS;
	CString LBL_SIG_UNLOADED;
	CString LBL_GAME_DROPPED;
	CString LBL_CLOSING_D2;
	CString LBL_lAUNCH_IN_X;
	CString LBL_PREPARE_TO_LAUNCH;
	CString LBL_LAUNCHING_D2;
	CString LBL_D2_FAIL;
	CString LBL_CHECK_D2;
	CString LBL_SIG_LOAD_FAIL;
	CString LBL_CONNECT_FAIL;
	CString LBL_LOGGING_IN;
	CString LBL_NULL_PASSWORD;
	CString LBL_INVALID_PASSWORD;
	CString LBL_CHECK_LOGIN;
	CString LBL_LOGIN_SUCCESS;
	CString LBL_LOGIN_FAIL;
	CString LBL_WAIT_CHAR_LIST;
	CString LBL_CHAR_LIST_SUCCESS;
	CString LBL_CHAR_LIST_FAIL;
	CString LBL_INVALID_CHAR_POS;
	CString LBL_INVALID_USER_DATA;
	CString LBL_SEL_CHAR;
	CString LBL_CHECK_CHAR_SEL;
	CString LBL_CHAR_SEL_FAIL;
	CString LBL_ENTER_LOBBY;
	CString LBL_NOT_IN_LOBBY;
	CString LBL_CREATING_GAME;
	CString LBL_CHECK_GAME;
	CString LBL_CREATE_FAIL;
	CString LBL_NAME_INVALID;
	CString LBL_NAME_EXISTS;
	CString LBL_CREATE_RETRY;
	CString LBL_SKIP_CHAR;
	CString LBL_EXIT_GAME;
	CString LBL_CHECK_EXIT;
	CString LBL_READY_FOR_NEXT_CHAR;
	CString LBL_READY_FOR_NEXT_ACCOUNT;
	CString LBL_RECREATE;
	CString LBL_EXIT_LOBBY;
	CString LBL_EXIT_CHAR_LIST;
	CString LBL_RECONNECT;
	CString LBL_CLEAN_ERRORS;
	CString LBL_CLIENT_CRASH;
	CString LBL_NO_MULE;
	CString LBL_MULE_STARTED;
	CString LBL_NORMAL_STARTED;
	CString LBL_INIT_SIG;
	CString LBL_FILE_MISSING;
	CString LBL_MULE_FINISHED;

	//[Messages]
	CString MSG_INVALID_CHAR_POS;
	CString MSG_EXIT_CONFIRM;
	CString MSG_CANNOT_RUN_MULE;
	CString MSG_PASSWORD_MISMATCH;
	CString MSG_NULL_INPUT;
	CString MSG_INVALID_INPUT;
	CString MSG_INSUFFICIENT_LINES;
	CString MSG_WARN;

	//[General page]
	CString GENERAL_GENERAL;
	CString GENERAL_LOAD_D2;
	CString GENERAL_D2_PATH;
	CString GENERAL_BROWSE;
	CString GENERAL_PARAMS;
	CString GENERAL_W;
	CString GENERAL_SKIPTOBNET;
	CString GENERAL_NOPLUGIN;
	CString GENERAL_RES800;
	CString GENERAL_LQ;
	CString GENERAL_NS;
	CString GENERAL_LOCALE;
	CString GENERAL_MPQ;
	CString GENERAL_MIN_D2;
	CString GENERAL_NO_PLUGIN_WARN;

	//[Login page]
	CString LOGIN_REALM;
	CString LOGIN_ACCOUNT;
	CString LOGIN_ACCOUNT_NAME;
	CString LOGIN_PASSWORD;
	CString LOGIN_REPEAT_PASSWORD;
	CString LOGIN_CHAR_POS;
	CString LOGIN_CHAR1;
	CString LOGIN_CHAR2;
	CString LOGIN_CHAR3;
	CString LOGIN_CHAR4;
	CString LOGIN_CHAR5;
	CString LOGIN_CHAR6;
	CString LOGIN_CHAR7;
	CString LOGIN_CHAR8;

	//[Create Page]
	CString CREATE_CHANNEL;
	CString CREATE_GAME_CREATE;
	CString CREATE_USE_RANDOM;
	CString CREATE_USE_PREFIX;
	CString CREATE_USE_DIC;
	CString CREATE_EDIT_DIC;
	CString CREATE_PASSWORD_LEN;
	CString CREATE_RETRY_TIMES;
	CString CREATE_DIFFICULTY;
	CString CREATE_CONTROL;
	CString CREATE_USE_AI;
	CString CREATE_INGAME_LIMIT;

	//[Delay page]
	CString DELAY_FRM_MAX_DELAY;
	CString DELAY_LAUNCH_DELAY;
	CString DELAY_LOGIN_DELAY;
	CString DELAY_CHARSEL_DELAY;
	CString DELAY_CREATE_DELAY;
	CString DELAY_EXIT_DELAY;
	CString DELAY_MAX_DELAY_BTN;
	CString DELAY_FRM_MIN_DELAY;
	CString DELAY_CONNECT_DELAY;
	CString DELAY_BTN_DELAY;
	CString DELAY_ERROR_DELAY;
	CString DELAY_MIN_DELAY_BTN;
	CString DELAY_RESTORE_DEFAULT;
	CString DELAY_TOOLTIP_BTN;
	CString DELAY_TOOLTIP_DEFAULT;
	CString DELAY_MAX_DELAY;
	CString DELAY_MAX_DELAY_DESC;
	CString DELAY_MIN_DELAY_DESC;

	//[Mule Page]
	CString MULE_LIST;
	CString MULE_STAY_LEN;
	CString MULE_ADD;
	CString MULE_EDIT;
	CString MULE_REMOVE;
	CString MULE_MOVE_UP;
	CString MULE_mOVE_DOWN;
	CString MULE_TOOLTIP_LIST;

	//[Hacks Page]
	CString HACKS_LIST;

	//[D2JSP Page]
	CString D2JSP_LOAD;
	CString D2JSP_NONE;
	CString D2JSP_LOAD_DESC;
	CString D2JSP_LOAD_SCRIPTS;
	CString D2JSP_AUTO;
	CString D2JSP_MANUAL;
	CString D2JSP_LIST;

	//[About]
	CString ABOUT_COPYRIGHT;

private:
	CString m_sPrevLangFile;	
};

#endif // !defined(AFX_LANGUAGE_H__99A2EC1B_260E_4828_88B4_71D9BA060AB6__INCLUDED_)
