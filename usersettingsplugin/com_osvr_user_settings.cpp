/** @file
    @brief User settings plugin

    @date 2015

    @author
    Razer, LLC.
    <http://razerzone.com>
*/

// Copyright 2015 Razer, LLC.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

// Internal Includes
#include <osvr/PluginKit/PluginKit.h>
#include <osvr/PluginKit/AnalogInterfaceC.h>

// Generated JSON header file
#include "com_osvr_user_settings_json.h"

// Library/third-party includes
// - none

// Standard includes
#include <iostream>
#include <fstream>

#include "../osvruser.h"

// Anonymous namespace to avoid symbol collision
namespace {

class AnalogSyncDevice {
  public:
    AnalogSyncDevice(OSVR_PluginRegContext ctx) : m_myVal(0) {

		std::ifstream file_id;
		file_id.open("C:/ProgramData/OSVR/osvr_user_settings.json");

		Json::Reader reader;
		Json::Value value;
		if (!reader.parse(file_id, value)){
			std::cout << "Couldn't open save file, creating file.";
			// new file just has default values
			//m_osvrUser.saveConfigFile(filename);
		}
		else{
			m_osvrUser.read(value);
		}

		/// Create the initialization options
        OSVR_DeviceInitOptions opts = osvrDeviceCreateInitOptions(ctx);

        /// Indicate that we'll want 3 analog channels.
        osvrDeviceAnalogConfigure(opts, &m_analog, 4);

        /// Create the sync device token with the options
        m_dev.initSync(ctx, "UserSettings", opts);

        /// Send JSON descriptor
        m_dev.sendJsonDescriptor(
            com_osvr_user_settings_json);

        /// Register update callback
        m_dev.registerUpdateCallback(this);
    }

    OSVR_ReturnCode update() {

		OSVR_AnalogState values[4];

		values[0] = m_osvrUser.pupilDistance(OS) + m_osvrUser.pupilDistance(OD);
		values[1] = m_osvrUser.standingEyeHeight();
		values[2] = m_osvrUser.seatedEyeHeight();
		values[3] = rand();
		osvrDeviceAnalogSetValues(m_dev, m_analog, values, 4);

		return OSVR_RETURN_SUCCESS;
    }

  private:
    osvr::pluginkit::DeviceToken m_dev;
    OSVR_AnalogDeviceInterface m_analog;
	double m_myVal;
	OSVRUser m_osvrUser;
	bool m_initialized = false;
};

class HardwareDetection {
  public:
    HardwareDetection() : m_found(false) {}
    OSVR_ReturnCode operator()(OSVR_PluginRegContext ctx) {

        std::cout << "UserSettings: plugin instantiated" << std::endl;
        if (!m_found) {
            std::cout << "UserSettings: Reading settings file" << std::endl;
            m_found = true;

            /// Create our device object
            osvr::pluginkit::registerObjectForDeletion(
                ctx, new AnalogSyncDevice(ctx));
        }
        return OSVR_RETURN_SUCCESS;
    }

  private:
    /// @brief Have we found our device yet? (this limits the plugin to one
    /// instance)
    bool m_found;
};
} // namespace

OSVR_PLUGIN(com_osvr_user_settings) {
    osvr::pluginkit::PluginContext context(ctx);

    /// Register a detection callback function object.
    context.registerHardwareDetectCallback(new HardwareDetection());

    return OSVR_RETURN_SUCCESS;
}
