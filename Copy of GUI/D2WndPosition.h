// Special positions on Diablo II Window.

// Note: All positions are related to window client area, frame border
// border and window title bar are not taken into account.

#ifndef __D2WNDPOSITION_H__
#define __D2WNDPOSITION_H__

// "Battle.net" button
const POINT BNET_BUTTON = { 400, 350 };
const POINT CANCEL_CONNECT = { 401, 394 };
const POINT APPLY_LOGIN = { 400, 470 };

// "Account name" box
const POINT ACCOUNTNAME_BOX = { 393, 333 };

// the character selection screen
const POINT CHAR_POSITIONS[8] = 
{
	{ 58, 130 }, { 558, 130 },
	{ 58, 225 }, { 558, 225 },
	{ 58, 325 }, { 558, 325 },
	{ 58, 415 }, { 558, 415 }
};

const POINT APPLY_CHARSEL = { 695, 555 };
const POINT CLEAN_DLG = { 399, 323 };

// chat input box
const POINT CHAT_INPUT = { 39, 431 };
const POINT APPLY_CHAT = { 68, 463 };

// the "create" button
const POINT CREATE_BUTTON = { 635, 460 };
const POINT JOIN_BUTTON = { 695, 460 };
const POINT APPLY_CREATE = { 731, 418 };

// the "Game name" box for create
const POINT CREATENAME_BOX = { 512, 152 };

// the "Game name" box for join
const POINT JOINNAME_BOX = { 502, 137 };

// the cancel queue button
const POINT CANCEL_QUEUE = { 510, 417 }; 

// player number limit box
const POINT PLAYERNUMBER_BOX = { 663, 299 };

// difficulty selection buttons
const POINT DIFFICULTIES[3] = 
{
	{ 438, 374 },  // normal
	{ 564, 374 },  // nightmare
	{ 707, 374 }   // hell
};

#endif