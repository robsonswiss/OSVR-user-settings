# OSVR-user-settings
A simple QT based utility to help users set up their configuration parameters

The application by default reads and writes a file called osvr_user_settings.json.

The schema is documented in the file user_schema.json.

An example user config file is in the file osvr_user_settings.json. This file gets read/written to the /ProgramData/OSVR directory on Windows platforms.

The application is built using QT and relies on the jsoncpp libraries.

##To deploy, requires bundling with a few QT libraries:
- Qt5Core.dll
- Qt5Gui.dll
- Qt5Widgets.dll
- icudt53.dll
- icuin53.dll
- icuuc53.dll
- libstdc++-6.dll
- libwinpthread-1.dll

##List of improvements:
- change external program invocations to use an XML config file to promote cross platform compatibility
- integrate device parser application

##Utilities pane provides launching of:
- direct mode enable/disable
- reset yaw application

The method to launch these involves a bat file which should contain the actual locations of the exes. 

##Things on the todo list:
- improve the mechanism for pointing to executables
- remove jsoncpp
- continuous integration
- make an xml config file to point to the executables for demo and reset_yaw.
- integrate osvr_print.exe

##To consume any of these settings, you must have a few other components in place:
###com_osvr_user_settings.dll
	- this is a server side plugin that is able to read the settings in the user settings file
	- this file must be installed in the osvr-plugins-0 directory of the server binary executable
	- the file is read at server startup: you must stop and start the server to pick up any new values
###osvr_server_config.json
	- this is the server config file
	- for convenience, a properly edited version is distributed with the plugin
	- it must be edited to include the user settings plugin.
		- adds plugin entry and some aliases
		
`"plugins": [
        "com_osvr_user_settings"
    ],
    "aliases": [
        {
            "/me/IPD": "/com_osvr_user_settings/UserSettings/analog/0",
            "/me/StandingHeight": "/com_osvr_user_settings/UserSettings/analog/1",
            "/me/SeatedHeight": "/com_osvr_user_settings/UserSettings/analog/2"
        }
    ]
`
	