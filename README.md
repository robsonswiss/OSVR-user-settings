# OSVR-user-settings
Utility to help users set up their configuration parameters

The application by default reads and writes a file called osvr_user_settings.json.

The schema is documented in the file user_schema.json.
An example user config file is in the file sample_user.json.


The application is built using QT and relies on the jsoncpp libraries.

To deploy, requires bundling with a few QT libraries:
Qt5Core.dll
Qt5Gui.dll
Qt5Widgets.dll
icudt53.dll
icuin53.dll
icuuc53.dll
libstdc++-6.dll
libwinpthread-1.dll

List of improvements:
- change external program invocations to use an XML config file to promote cross platform compatibility
- integrate device parser application