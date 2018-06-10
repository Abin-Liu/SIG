autod2jsp README

Simple setup instructions:

1) Don't edit the autod2jsp.ini file directly, unless you need to delete a line
	Use the autod2jsp Add/Edit controls to add/edit new d2jsp instances.
2) Always copy the autostarter.d2j included with these zip files over your existing starter.d2j for it to fully work
3) You can easily use a single d2jsp folder now, by correctly configuring autod2jsp.exe to start the right script(s)
4) Make sure your pathes and file names are 100% correct, even a single mistake will cause autod2jsp to not work.

5) Enjoy ;)

--njaguar


Versioning information:

MAJOR.MINOR.SUB
eg: version 0.1.2

Sub versions will include bug fixes, and minor feature enhancements
Minor versions will include config file changes and/or major enhancements
Major versions will be final release versions



HOW TO USE LOG LIST WINDOW:
Call this function (or similiar):

function nj_logItemText( textStr ) {
	sendDDE(1,"autod2jsp","log","",getMyPID() + "," + D2JSP_UTIL_DATE_ToSmallDate( new Date() ) + "," + String(textStr).replace(/\xff+(c\d)/g, ""));
}

Replace the D2JSP_UTIL_DATE_ToSmallDate() function call with your own date formatting function.


Todo:
 - If you attempt to start a 2nd d2jsp, check titles to make sure they are unique, if not, error message
 - popup file select for script to run
 - allow the ability to specify which starter to use
 - add skiptobnet option


HISTORY:
v0.2.3 -
 - Fixed a bug with autostart, works correctly now


v0.2.2 -
 - autod2jsp now starts the script autostarter.d2j and stops starter.d2j (if it is running)
 - cdkey checkbox cannot be checked without first selecting d2loader option
 - relabeled the auto start d2jsp checkbox
 

v0.2.1 -
 - Adding or Editting now prompts you to be sure to save the first time you add or edit
 - Saving now prompts you to let you know you successfully saved
 - Reworked the filename generator for the ini file (reported bugs)


v0.2.0 -
 - fixed a couple bugs
 - now reads from an ini file (to prevent the need to keep dumping the config file for each change)
 - Removed D2App variable, now use the d2path to select the d2 exe file
 - d2jspPath now must include the d2jsp.exe
 - Fixed a bug in the attacher sequence, the -install was out of order
 

v0.1.5 -
 - fixed old reintroduced bugs in the log list


v0.1.4 -
 - fixed a log dde error where it would truncate the log message if there was a comma in it
 - setting list is now single selection, instead of multiple
 - password field is now a password box (shows ****)
 - freeze threshold is now forced to a number box
 

v0.1.3 -
 - ping timer starts as soon as it attachs d2jsp to catch frozen d2's on attach


v0.1.2 -
 - fixed a bug where it would try to attach d2jsp more than once to a thread in some conditions


v0.1.1 -
 - fixed a log -> list output bug, date wasnt getting inserted
 

v0.1.0 -
 - there is now a 1 second delay between when autod2jsp finds the d2 (started) window and when it attaches d2jsp
   this was causing crashes when trying to attach too fast
 - added setting name, and renamed Title columns to show this instead, this is for labeling of d2 settings
 - straightened the settings dialog controls (made it look better)
 - added game type
 - added game name
 - added game pass
 - added random game name
 - added random game pass
 - if you set freeze threshold to 0, it will disable it
 - setting name will now update without requiring a restart
 - Kill button now kills the d2 selected


v0.0.7 -
 - any dde command/request issued from d2jsp will reset the idle counter
 

v0.0.6 -
 - window title, d2 exe, and cdkey.mpq disable/enable based upon d2loader checkbox state
 - cdkey.mpq is disabled/enabled based upon cdkey.mpq checkbox state (and d2loader)


v0.0.5 -
 - Fixed bug with freeze timeout delay
 (first history docs)


More to come ;)

Please report any bugs/failures

Ideas are welcome, but this is very alpha, so there are tons of things already planned. :)

--njaguar
