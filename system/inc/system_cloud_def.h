/**
 ******************************************************************************
  Copyright (c) 2013-2014 IntoRobot Team.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation, either
  version 3 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, see <http://www.gnu.org/licenses/>.
  ******************************************************************************
*/

#ifndef SYSTEM_CLOUD_DEF_H_
#define SYSTEM_CLOUD_DEF_H_


#define INTOROBOT_API_VER             "v1"

#define INTOROBOT_OTA_UPDATE_URL       "/v1/bins/"
#define INTOROBOT_SUBSYSTEM_UPDATE_URL "/downloads/neutron/"

#define INTOROBOT_UPDATE_DOMAIN        "www.intorobot.com"
#define INTOROBOT_SERVER_DOMAIN        "iot.intorobot.com"

#define INTOROBOT_SERVER_PORT          1883
#define INTOROBOT_SERVER_TLS_PORT      8883

//maqtt will
#define INTOROBOT_MQTT_WILL_TOPIC      "platform/default/info/online"
#define INTOROBOT_MQTT_WILL_QOS        0
#define INTOROBOT_MQTT_WILL_RETAIN     true
#define INTOROBOT_MQTT_WILL_MESSAGE    "{\"key\": \"offline\"}"

//device version
#define INTOROBOT_MQTT_VERSION_TOPIC   "firmware/default/info/version"
#define INTOROBOT_MQTT_VERSION_QOS     0
#define INTOROBOT_MQTT_VERSION_RETAIN  true

//notify response
#define INTOROBOT_MQTT_RESPONSE_TOPIC   "platform/default/info/notify"
#define INTOROBOT_MQTT_RESPONSE_QOS     0
#define INTOROBOT_MQTT_RESPONSE_RETAIN  false

#define INTOROBOT_MQTT_RESPONSE_OTA_READY            "10"  //online ota ready
#define INTOROBOT_MQTT_RESPONSE_OTA_DOWN_FAIL        "11"  //online ota down failed
#define INTOROBOT_MQTT_RESPONSE_OTA_DOWN_SUCC        "12"  //online ota success
#define INTOROBOT_MQTT_RESPONSE_OTA_UPDATE_FAIL      "13"  //online ota update failed
#define INTOROBOT_MQTT_RESPONSE_OTA_UPDATE_SUCC      "14"  //online ota update success
#define INTOROBOT_MQTT_RESPONSE_OTA_DOWN_SUCC_EXIT   "15"  //online ota down success and exit
#define INTOROBOT_MQTT_RESPONSE_OTA_TYPEEEOR         "16"  //online ota board type error

#define INTOROBOT_MQTT_RESPONSE_TOKEN_READY          "20"  //ready token update
#define INTOROBOT_MQTT_RESPONSE_TOKEN_UPDATE_FAIL    "21"  //token update failed
#define INTOROBOT_MQTT_RESPONSE_TOKEN_UPDATE_SUCC    "22"  //token update success

#define INTOROBOT_MQTT_RESPONSE_RESET_READY          "30"  //ready system reset
#define INTOROBOT_MQTT_RESPONSE_RESET_FAIL           "31"  //system reset failed
#define INTOROBOT_MQTT_RESPONSE_RESET_SUCC           "32"  //system reset success

//notify json response
#define INTOROBOT_MQTT_RESPONSE_JSON_TOPIC   "platform/default/info/json"
#define INTOROBOT_MQTT_RESPONSE_JSON_QOS     0
#define INTOROBOT_MQTT_RESPONSE_JSON_RETAIN  false

#define INTOROBOT_MQTT_RESPONSE_SUBSYS_READY           "10"  //subsys ready down
#define INTOROBOT_MQTT_RESPONSE_SUBSYS_DOWN_FAIL       "11"  //subsys down failed
#define INTOROBOT_MQTT_RESPONSE_SUBSYS_DOWN_SUCC       "12"  //subsys down success
#define INTOROBOT_MQTT_RESPONSE_SUBSYS_UPDATE_FAIL     "13"  //subsys update failed
#define INTOROBOT_MQTT_RESPONSE_SUBSYS_UPDATE_SUCC     "14"  //subsys update success
#define INTOROBOT_MQTT_RESPONSE_SUBSYS_UPDATE_PROCESS  "15"  //subsys update process
#define INTOROBOT_MQTT_RESPONSE_SUBSYS_TYPEEEOR        "16"  //subsys update board type error


//device debug info
#define INTOROBOT_MQTT_SEND_DEBUG_TOPIC    "firmware/default/info/debug"
#define INTOROBOT_MQTT_SEND_DEBUG_QOS      0


/*subscribe  topic*/
#define INTOROBOT_MQTT_SUB_UPDATE_TOPIC    "firmware/default/action/flash"
#define INTOROBOT_MQTT_SUB_UPDATE_QOS      0

#define INTOROBOT_MQTT_SUB_JSON_UPDATE_TOPIC   "platform/default/action/json"
#define INTOROBOT_MQTT_SUB_JSON_UPDATE_QOS     0

#define INTOROBOT_MQTT_SUB_SYNC_TIME_TOPIC     "platform/default/info/time"
#define INTOROBOT_MQTT_SUB_SYNC_TIME_QOS       0

#define INTOROBOT_MQTT_SUB_CHANGE_TOKEN_TOPIC   "token/default/action/flash"
#define INTOROBOT_MQTT_SUB_CHANGE_TOKEN_QOS     0

#define INTOROBOT_MQTT_SUB_RESET_TOPIC   "firmware/default/action/reset"
#define INTOROBOT_MQTT_SUB_RESET_QOS     0

#define INTOROBOT_MQTT_SUB_RECEIVE_DEBUG_TOPIC   "firmware/default/action/debug"
#define INTOROBOT_MQTT_SUB_RECEIVE_DEBUG_QOS     0


#endif	/*SYSTEM_CLOUD_DEF_H_ */