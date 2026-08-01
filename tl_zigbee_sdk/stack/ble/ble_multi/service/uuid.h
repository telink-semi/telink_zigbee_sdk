/********************************************************************************************************
 * @file    uuid.h
 *
 * @brief   This is the header file for BLE SDK
 *
 * @author  BLE GROUP
 * @date    06,2022
 *
 * @par     Copyright (c) 2022, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
 *
 *          Licensed under the Apache License, Version 2.0 (the "License");
 *          you may not use this file except in compliance with the License.
 *          You may obtain a copy of the License at
 *
 *              http://www.apache.org/licenses/LICENSE-2.0
 *
 *          Unless required by applicable law or agreed to in writing, software
 *          distributed under the License is distributed on an "AS IS" BASIS,
 *          WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *          See the License for the specific language governing permissions and
 *          limitations under the License.
 *
 *******************************************************************************************************/
#pragma once

#include "host/att/att_uuid.h"

/**
 *  @brief Definition for GATT Services UUID
 */
#define SERVICE_UUID_GENERIC_ACCESS                 0x1800 //Generic Access Service
#define SERVICE_UUID_GENERIC_ATTRIBUTE              0x1801 //Generic Attribute Service
#define SERVICE_UUID_IMMEDIATE_ALERT                0x1802 //Immediate Alert Service
#define SERVICE_UUID_LINK_LOSS                      0x1803 //Link Loss Service
#define SERVICE_UUID_TX_POWER                       0x1804 //Tx Power Service
#define SERVICE_UUID_CURRENT_TIME                   0x1805 //Current Time Service
#define SERVICE_UUID_REFERENCE_TIME_UPDATE          0x1806 //Reference Time Update Service
#define SERVICE_UUID_NEXT_DST_CHANGE                0x1807 //Next DST Change Service
#define SERVICE_UUID_GLUCOSE                        0x1808 //Glucose Service
#define SERVICE_UUID_HEALTH_THERMOMETER             0x1809 //Health Thermometer Service
#define SERVICE_UUID_DEVICE_INFORMATION             0x180A //Device Information Service
#define SERVICE_UUID_HEART_RATE                     0x180D //Heart Rate Service
#define SERVICE_UUID_PHONE_ALERT_STATUS             0x180E //Phone Alert Status Service
#define SERVICE_UUID_BATTERY                        0x180F //Battery Service
#define SERVICE_UUID_BLOOD_PRESSURE                 0x1810 //Blood Pressure Service
#define SERVICE_UUID_ALERT_NOTIFICATION             0x1811 //Alert Notification Service
#define SERVICE_UUID_HUMAN_INTERFACE_DEVICE         0x1812 //Human Interface Device Service
#define SERVICE_UUID_SCAN_PARAMETERS                0x1813 //Scan Parameters Service
#define SERVICE_UUID_RUNNING_SPEED_AND_CADENCE      0x1814 //Running Speed and Cadence Service
#define SERVICE_UUID_AUTOMATION_IO                  0x1815 //Automation IO Service
#define SERVICE_UUID_CYCLING_SPEED_AND_CADENCE      0x1816 //Cycling Speed and Cadence Service
#define SERVICE_UUID_CYCLING_POWER                  0x1818 //Cycling Power Service
#define SERVICE_UUID_LOCATION_AND_NAVIGATION        0x1819 //Location and Navigation Service
#define SERVICE_UUID_ENVIRONMENTAL_SENSING          0x181A //Environmental Sensing Service
#define SERVICE_UUID_BODY_COMPOSITION               0x181B //Body Composition Service
#define SERVICE_UUID_USER_DATA                      0x181C //User Data Service
#define SERVICE_UUID_WEIGHT_SCALE                   0x181D //Weight Scale Service
#define SERVICE_UUID_BOND_MANAGEMENT                0x181E //Bond Management Service
#define SERVICE_UUID_CONTINUOUS_GLUCOSE_MONITORING  0x181F //Continuous Glucose Monitoring Service
#define SERVICE_UUID_INTERNET_PROTOCOL_SUPPORT      0x1820 //Internet Protocol Support Service
#define SERVICE_UUID_INDOOR_POSITIONING             0x1821 //Indoor Positioning Service
#define SERVICE_UUID_PULSE_OXIMETER                 0x1822 //Pulse Oximeter Service
#define SERVICE_UUID_HTTP_PROXY                     0x1823 //HTTP Proxy Service
#define SERVICE_UUID_TRANSPORT_DISCOVERY            0x1824 //Transport Discovery Service
#define SERVICE_UUID_OBJECT_TRANSFER                0x1825 //Object Transfer Service
#define SERVICE_UUID_FITNESS_MACHINE                0x1826 //Fitness Machine Service
#define SERVICE_UUID_MESH_PROVISIONING              0x1827 //Mesh Provisioning Service
#define SERVICE_UUID_MESH_PROXY                     0x1828 //Mesh Proxy Service
#define SERVICE_UUID_RECONNECTION_CONFIGURATION     0x1829 //Reconnection Configuration Service
#define SERVICE_UUID_INSULIN_DELIVERY               0x183A //Insulin Delivery Service
#define SERVICE_UUID_BINARY_SENSOR                  0x183B //Binary Sensor Service
#define SERVICE_UUID_EMERGENCY_CONFIGURATION        0x183C //Emergency Configuration Service
#define SERVICE_UUID_AUTHORIZATION_CONTROL          0x183D //Authorization Control Service
#define SERVICE_UUID_PHYSICAL_ACTIVITY_MONITOR      0x183E //Physical Activity Monitor Service
#define SERVICE_UUID_ELAPSED_TIME                   0x183F //Elapsed Time Service
#define SERVICE_UUID_GENERIC_HEALTH_SENSOR          0x1840 //Generic Health Sensor Service
#define SERVICE_UUID_AUDIO_INPUT_CONTROL            0x1843 //Audio Input Control Service
#define SERVICE_UUID_VOLUME_CONTROL                 0x1844 //Volume Control Service
#define SERVICE_UUID_VOLUME_OFFSET_CONTROL          0x1845 //Volume Offset Control Service
#define SERVICE_UUID_COORDINATED_SET_IDENTIFICATION 0x1846 //Coordinated Set Identification Service
#define SERVICE_UUID_DEVICE_TIME                    0x1847 //Device Time Service
#define SERVICE_UUID_MEDIA_CONTROL                  0x1848 //Media Control Service
#define SERVICE_UUID_GENERIC_MEDIA_CONTROL          0x1849 //Generic Media Control Service
#define SERVICE_UUID_CONSTANT_TONE_EXTENSION        0x184A //Constant Tone Extension Service
#define SERVICE_UUID_TELEPHONE_BEARER               0x184B //Telephone Bearer Service
#define SERVICE_UUID_GENERIC_TELEPHONE_BEARER       0x184C //Generic Telephone Bearer Service
#define SERVICE_UUID_MICROPHONE_CONTROL             0x184D //Microphone Control Service
#define SERVICE_UUID_AUDIO_STREAM_CONTROL           0x184E //Audio Stream Control Service
#define SERVICE_UUID_BROADCAST_AUDIO_SCAN           0x184F //Broadcast Audio Scan Service
#define SERVICE_UUID_PUBLISHED_AUDIO_CAPABILITIES   0x1850 //Published Audio Capabilities Service
#define SERVICE_UUID_BASIC_AUDIO_ANNOUNCEMENT       0x1851 //Basic Audio Announcement Service
#define SERVICE_UUID_BROADCAST_AUDIO_ANNOUNCEMENT   0x1852 //Broadcast Audio Announcement Service
#define SERVICE_UUID_COMMON_AUDIO                   0x1853 //Common Audio Service
#define SERVICE_UUID_HEARING_ACCESS                 0x1854 //Hearing Access Service
#define SERVICE_UUID_TELEPHONY_AND_MEDIA_AUDIO      0x1855 //Telephony and Media Audio Service
#define SERVICE_UUID_PUBLIC_BROADCAST_ANNOUNCEMENT  0x1856 //Public Broadcast Announcement Service
#define SERVICE_UUID_ELECTRONIC_SHELF_LABEL         0x1857 //Electronic Shelf Label Service
#define SERVICE_UUID_GAMING_AUDIO                   0x1858 //Gaming Audio Service
#define SERVICE_UUID_MESH_PROXY_SOLICITATION        0x1859 //Mesh Proxy Solicitation Service
#define SERVICE_UUID_RANGING                        0x185B //Ranging Service
#define SERVICE_UUID_HID_ISO                        0x185C //HID ISO Service

/**
 *  @brief Definition for Declaration UUID
 */
#define DECLARATIONS_UUID_PRIMARY_SERVICE   0x2800 //Primary Service
#define DECLARATIONS_UUID_SECONDARY_SERVICE 0x2801 //Secondary Service
#define DECLARATIONS_UUID_INCLUDE           0x2802 //Include
#define DECLARATIONS_UUID_CHARACTERISTIC    0x2803 //Characteristic

/**
 * @brief Definition for Descriptor UUID
 */
#define DESCRIPTOR_UUID_CHARACTERISTIC_EXTENDED_PROPERTIES    0x2900 //Characteristic Extended Properties
#define DESCRIPTOR_UUID_CHARACTERISTIC_USER_DESCRIPTION       0x2901 //Characteristic User Description
#define DESCRIPTOR_UUID_CLIENT_CHARACTERISTIC_CONFIGURATION   0x2902 //Client Characteristic Configuration
#define DESCRIPTOR_UUID_SERVER_CHARACTERISTIC_CONFIGURATION   0x2903 //Server Characteristic Configuration
#define DESCRIPTOR_UUID_CHARACTERISTIC_PRESENTATION_FORMAT    0x2904 //Characteristic Presentation Format
#define DESCRIPTOR_UUID_CHARACTERISTIC_AGGREGATE_FORMAT       0x2905 //Characteristic Aggregate Format
#define DESCRIPTOR_UUID_VALID_RANGE                           0x2906 //Valid Range
#define DESCRIPTOR_UUID_EXTERNAL_REPORT_REFERENCE             0x2907 //External Report Reference
#define DESCRIPTOR_UUID_REPORT_REFERENCE                      0x2908 //Report Reference
#define DESCRIPTOR_UUID_NUMBER_OF_DIGITALS                    0x2909 //Number of Digitals
#define DESCRIPTOR_UUID_VALUE_TRIGGER_SETTING                 0x290A //Value Trigger Setting
#define DESCRIPTOR_UUID_ENVIRONMENTAL_SENSING_CONFIGURATION   0x290B //Environmental Sensing Configuration
#define DESCRIPTOR_UUID_ENVIRONMENTAL_SENSING_MEASUREMENT     0x290C //Environmental Sensing Measurement
#define DESCRIPTOR_UUID_ENVIRONMENTAL_SENSING_TRIGGER_SETTING 0x290D //Environmental Sensing Trigger Setting
#define DESCRIPTOR_UUID_TIME_TRIGGER_SETTING                  0x290E //Time Trigger Setting
#define DESCRIPTOR_UUID_COMPLETE_BR_EDR_TRANSPORT_BLOCK_DATA  0x290F //Complete BR-EDR Transport Block Data
#define DESCRIPTOR_UUID_OBSERVATION_SCHEDULE                  0x2910 //Observation Schedule
#define DESCRIPTOR_UUID_VALID_RANGE_AND_ACCURACY              0x2911 //Valid Range and Accuracy


/**
 * @brief Definition for Characteristic UUID
 */
#define CHARACTERISTIC_UUID_DEVICE_NAME                         0x2A00 //Device Name
#define CHARACTERISTIC_UUID_APPEARANCE                          0x2A01 //Appearance
#define CHARACTERISTIC_UUID_PERIPHERAL_PRIVACY_FLAG             0x2A02 //Peripheral Privacy Flag
#define CHARACTERISTIC_UUID_RECONNECTION_ADDRESS                0x2A03 //Reconnection Address
#define CHARACTERISTIC_UUID_PERIPHERAL_PREFERRED_CONN_PARAM     0x2A04 //Peripheral Preferred Connection Parameters
#define CHARACTERISTIC_UUID_SERVICE_CHANGED                     0x2A05 //Service Changed
#define CHARACTERISTIC_UUID_ALERT_LEVEL                         0x2A06 //Alert Level
#define CHARACTERISTIC_UUID_TX_POWER_LEVEL                      0x2A07 //Tx Power Level
#define CHARACTERISTIC_UUID_DATE_TIME                           0x2A08 //Date Time
#define CHARACTERISTIC_UUID_DAYOF_WEEK                          0x2A09 //Dayof Week
#define CHARACTERISTIC_UUID_DAY_DATE_TIME                       0x2A0A //Day Date Time
#define CHARACTERISTIC_UUID_EXACT_TIME_256                      0x2A0C //Exact Time 256
#define CHARACTERISTIC_UUID_DST_OFFSET                          0x2A0D //DST Offset
#define CHARACTERISTIC_UUID_TIME_ZONE                           0x2A0E //Time Zone
#define CHARACTERISTIC_UUID_LOCAL_TIME_INFORMATION              0x2A0F //Local Time Information
#define CHARACTERISTIC_UUID_TIME_WITH_DST                       0x2A11 //Time with DST
#define CHARACTERISTIC_UUID_TIME_ACCURACY                       0x2A12 //Time Accuracy
#define CHARACTERISTIC_UUID_TIME_SOURCE                         0x2A13 //Time Source
#define CHARACTERISTIC_UUID_REFERENCE_TIME_INFORMATION          0x2A14 //Reference Time Information
#define CHARACTERISTIC_UUID_TIME_UPDATE_CONTROL_POINT           0x2A16 //Time Update Control Point
#define CHARACTERISTIC_UUID_TIME_UPDATE_STATE                   0x2A17 //Time Update State
#define CHARACTERISTIC_UUID_GLUCOSE_MEASUREMENT                 0x2A18 //Glucose Measurement
#define CHARACTERISTIC_UUID_BATTERY_LEVEL                       0x2A19 //Battery Level
#define CHARACTERISTIC_UUID_BATTERY_POWER_STATE                 0x2A1A //Battery Power state
#define CHARACTERISTIC_UUID_TEMPERATURE_MEASUREMENT             0x2A1C //Temperature Measurement
#define CHARACTERISTIC_UUID_TEMPERATURE_TYPE                    0x2A1D //Temperature Type
#define CHARACTERISTIC_UUID_INTERMEDIATE_TEMPERATURE            0x2A1E //Intermediate Temperature
#define CHARACTERISTIC_UUID_MEASUREMENT_INTERVAL                0x2A21 //Measurement Interval
#define CHARACTERISTIC_UUID_BOOT_KEYBOARD_INPUT_REPORT          0x2A22 //Boot Keyboard Input Report
#define CHARACTERISTIC_UUID_SYSTEM_ID                           0x2A23 //System ID
#define CHARACTERISTIC_UUID_MODEL_NUMBER_STRING                 0x2A24 //Model Number String
#define CHARACTERISTIC_UUID_SERIAL_NUMBER_STRING                0x2A25 //Serial Number String
#define CHARACTERISTIC_UUID_FIRMWARE_REVISION_STRING            0x2A26 //Firmware Revision String
#define CHARACTERISTIC_UUID_HARDWARE_REVISION_STRING            0x2A27 //Hardware Revision String
#define CHARACTERISTIC_UUID_SOFTWARE_REVISION_STRING            0x2A28 //Software Revision String
#define CHARACTERISTIC_UUID_MANUFACTURER_NAME_STRING            0x2A29 //Manufacturer Name String
#define CHARACTERISTIC_UUID_IEEE11073_20601_DATA_LIST           0x2A2A //IEEE11073-20601 Regulatory Certification Data List
#define CHARACTERISTIC_UUID_CURRENT_TIME                        0x2A2B //Current Time
#define CHARACTERISTIC_UUID_MAGNETIC_DECLINATION                0x2A2C //Magnetic Declination
#define CHARACTERISTIC_UUID_SCAN_REFRESH                        0x2A31 //Scan Refresh
#define CHARACTERISTIC_UUID_BOOT_KEYBOARD_OUTPUT_REPORT         0x2A32 //Boot Keyboard Output Report
#define CHARACTERISTIC_UUID_BOOT_MOUSE_INPUT_REPORT             0x2A33 //Boot Mouse Input Report
#define CHARACTERISTIC_UUID_GLUCOSE_MEASUREMENT_CONTEXT         0x2A34 //Glucose Measurement Context
#define CHARACTERISTIC_UUID_BLOOD_PRESSURE_MEASUREMENT          0x2A35 //Blood Pressure Measurement
#define CHARACTERISTIC_UUID_INTERMEDIATE_CUFF_PRESSURE          0x2A36 //Intermediate Cuff Pressure
#define CHARACTERISTIC_UUID_HEART_RATE_MEASUREMENT              0x2A37 //Heart Rate Measurement
#define CHARACTERISTIC_UUID_BODY_SENSOR_LOCATION                0x2A38 //Body Sensor Location
#define CHARACTERISTIC_UUID_HEART_RATE_CONTROL_POINT            0x2A39 //Heart Rate Control Point
#define CHARACTERISTIC_UUID_ALERT_STATUS                        0x2A3F //Alert Status
#define CHARACTERISTIC_UUID_RINGER_CONTROL_POINT                0x2A40 //Ringer Control Point
#define CHARACTERISTIC_UUID_RINGER_SETTING                      0x2A41 //Ringer Setting
#define CHARACTERISTIC_UUID_ALERT_CATEGORY_ID_BIT_MASK          0x2A42 //Alert Category ID Bit Mask
#define CHARACTERISTIC_UUID_ALERT_CATEGORY_ID                   0x2A43 //Alert Category ID
#define CHARACTERISTIC_UUID_ALERT_NOTIFICATION_CONTROL_POINT    0x2A44 //Alert Notification Control Point
#define CHARACTERISTIC_UUID_UNREAD_ALERT_STATUS                 0x2A45 //Unread Alert Status
#define CHARACTERISTIC_UUID_NEW_ALERT                           0x2A46 //New Alert
#define CHARACTERISTIC_UUID_SUPPORTED_NEW_ALERT_CATEGORY        0x2A47 //Supported New Alert Category
#define CHARACTERISTIC_UUID_SUPPORTED_UNREAD_ALERT_CATEGORY     0x2A48 //Supported Unread Alert Category
#define CHARACTERISTIC_UUID_BLOOD_PRESSURE_FEATURE              0x2A49 //Blood Pressure Feature
#define CHARACTERISTIC_UUID_HID_INFORMATION                     0x2A4A //HID Information
#define CHARACTERISTIC_UUID_REPORT_MAP                          0x2A4B //Report Map
#define CHARACTERISTIC_UUID_HID_CONTROL_POINT                   0x2A4C //HID Control Point
#define CHARACTERISTIC_UUID_REPORT                              0x2A4D //Report
#define CHARACTERISTIC_UUID_PROTOCOL_MODE                       0x2A4E //Protocol Mode
#define CHARACTERISTIC_UUID_SCAN_INTERVAL_WINDOW                0x2A4F //Scan Interval Window
#define CHARACTERISTIC_UUID_PNP_ID                              0x2A50 //PnP ID
#define CHARACTERISTIC_UUID_GLUCOSE_FEATURE                     0x2A51 //Glucose Feature
#define CHARACTERISTIC_UUID_RECORD_ACCESS_CONTROL_POINT         0x2A52 //Record Access Control Point
#define CHARACTERISTIC_UUID_RSC_MEASUREMENT                     0x2A53 //RSC Measurement
#define CHARACTERISTIC_UUID_RSC_FEATURE                         0x2A54 //RSC Feature
#define CHARACTERISTIC_UUID_SC_CONTROL_POINT                    0x2A55 //SC Control Point
#define CHARACTERISTIC_UUID_AGGREGATE                           0x2A5A //Aggregate
#define CHARACTERISTIC_UUID_CSC_MEASUREMENT                     0x2A5B //CSC Measurement
#define CHARACTERISTIC_UUID_CSC_FEATURE                         0x2A5C //CSC Feature
#define CHARACTERISTIC_UUID_SENSOR_LOCATION                     0x2A5D //Sensor Location
#define CHARACTERISTIC_UUID_PLX_SPOT_CHECK_MEASUREMENT          0x2A5E //PLX Spot-Check Measurement
#define CHARACTERISTIC_UUID_PLX_CONTINUOUS_MEASUREMENT          0x2A5F //PLX Continuous Measurement
#define CHARACTERISTIC_UUID_PLX_FEATURES                        0x2A60 //PLX Features
#define CHARACTERISTIC_UUID_CYCLING_POWER_MEASUREMENT           0x2A63 //Cycling Power Measurement
#define CHARACTERISTIC_UUID_CYCLING_POWER_VECTOR                0x2A64 //Cycling Power Vector
#define CHARACTERISTIC_UUID_CYCLING_POWER_FEATURE               0x2A65 //Cycling Power Feature
#define CHARACTERISTIC_UUID_CYCLING_POWER_CONTROL_POINT         0x2A66 //Cycling Power Control Point
#define CHARACTERISTIC_UUID_LOCATION_AND_SPEED                  0x2A67 //Location and Speed
#define CHARACTERISTIC_UUID_NAVIGATION                          0x2A68 //Navigation
#define CHARACTERISTIC_UUID_POSITION_QUALITY                    0x2A69 //Position Quality
#define CHARACTERISTIC_UUID_LN_FEATURE                          0x2A6A //LN Feature
#define CHARACTERISTIC_UUID_LN_CONTROL_POINT                    0x2A6B //LN Control Point
#define CHARACTERISTIC_UUID_ELEVATION                           0x2A6C //Elevation
#define CHARACTERISTIC_UUID_PRESSURE                            0x2A6D //Pressure
#define CHARACTERISTIC_UUID_TEMPERATURE                         0x2A6E //Temperature
#define CHARACTERISTIC_UUID_HUMIDITY                            0x2A6F //Humidity
#define CHARACTERISTIC_UUID_TRUE_WIND_SPEED                     0x2A70 //True Wind Speed
#define CHARACTERISTIC_UUID_TRUE_WIND_DIRECTION                 0x2A71 //True Wind Direction
#define CHARACTERISTIC_UUID_APPARENT_WIND_SPEED                 0x2A72 //Apparent Wind Speed
#define CHARACTERISTIC_UUID_APPARENT_WIND_DIRECTION             0x2A73 //Apparent Wind Direction
#define CHARACTERISTIC_UUID_GUST_FACTOR                         0x2A74 //Gust Factor
#define CHARACTERISTIC_UUID_POLLEN_CONC                         0x2A75 //Pollen Concentration
#define CHARACTERISTIC_UUID_UV_INDEX                            0x2A76 //UV Index
#define CHARACTERISTIC_UUID_IRRADIANCE                          0x2A77 //Irradiance
#define CHARACTERISTIC_UUID_RAINFALL                            0x2A78 //Rainfall
#define CHARACTERISTIC_UUID_WIND_CHILL                          0x2A79 //Wind Chill
#define CHARACTERISTIC_UUID_HEAT_INDEX                          0x2A7A //Heat Index
#define CHARACTERISTIC_UUID_DEW_POINT                           0x2A7B //Dew Point
#define CHARACTERISTIC_UUID_DESCRIPTOR_VALUE_CHANGED            0x2A7D //Descriptor Value Changed
#define CHARACTERISTIC_UUID_AEROBIC_HEART_RATE_LOWER_LIMIT      0x2A7E //Aerobic Heart Rate Lower Limit
#define CHARACTERISTIC_UUID_AEROBIC_THRESHOLD                   0x2A7F //Aerobic Threshold
#define CHARACTERISTIC_UUID_AGE                                 0x2A80 //Age
#define CHARACTERISTIC_UUID_ANAEROBIC_HEART_RATE_LOWER_LIMIT    0x2A81 //Anaerobic Heart Rate Lower Limit
#define CHARACTERISTIC_UUID_ANAEROBIC_HEART_RATE_UPPER_LIMIT    0x2A82 //Anaerobic Heart Rate Upper Limit
#define CHARACTERISTIC_UUID_ANAEROBIC_THRESHOLD                 0x2A83 //Anaerobic Threshold
#define CHARACTERISTIC_UUID_AEROBIC_HEART_RATE_UPPER_LIMIT      0x2A84 //Aerobic Heart Rate Upper Limit
#define CHARACTERISTIC_UUID_DATE_OF_BIRTH                       0x2A85 //Date of Birth
#define CHARACTERISTIC_UUID_DATE_OF_THRESHOLD_ASSESSMENT        0x2A86 //Date of Threshold Assessment
#define CHARACTERISTIC_UUID_EMAIL_ADDRESS                       0x2A87 //Email Address
#define CHARACTERISTIC_UUID_FAT_BURN_HEART_RATE_LOWER_LIMIT     0x2A88 //Fat Burn Heart Rate Lower Limit
#define CHARACTERISTIC_UUID_FAT_BURN_HEART_RATE_UPPER_LIMIT     0x2A89 //Fat Burn Heart Rate Upper Limit
#define CHARACTERISTIC_UUID_FIRST_NAME                          0x2A8A //First Name
#define CHARACTERISTIC_UUID_FIVE_ZONE_HEART_RATE_LIMITS         0x2A8B //Five Zone Heart Rate Limits
#define CHARACTERISTIC_UUID_GENDER                              0x2A8C //Gender
#define CHARACTERISTIC_UUID_HEART_RATE_MAX                      0x2A8D //Heart Rate Max
#define CHARACTERISTIC_UUID_HEIGHT                              0x2A8E //Height
#define CHARACTERISTIC_UUID_HIP_CIRCUMFERENCE                   0x2A8F //Hip Circumference
#define CHARACTERISTIC_UUID_LAST_NAME                           0x2A90 //Last Name
#define CHARACTERISTIC_UUID_MAXIMUM_RECOMMENDED_HEART_RATE      0x2A91 //Maximum Recommended Heart Rate
#define CHARACTERISTIC_UUID_RESTING_HEART_RATE                  0x2A92 //Resting Heart Rate
#define CHARACTERISTIC_UUID_SPORT_TYPE                          0x2A93 //Sport Type for Aerobic and Anaerobic Thresholds
#define CHARACTERISTIC_UUID_THREE_ZONE_HEART_RATE_LIMITS        0x2A94 //Three Zone Heart Rate Limits
#define CHARACTERISTIC_UUID_TWO_ZONE_HEART_RATE_LIMITS          0x2A95 //Two Zone Heart Rate Limits
#define CHARACTERISTIC_UUID_VO2_MAX                             0x2A96 //VO2 Max
#define CHARACTERISTIC_UUID_WAIST_CIRCUMFERENCE                 0x2A97 //Waist Circumference
#define CHARACTERISTIC_UUID_WEIGHT                              0x2A98 //Weight
#define CHARACTERISTIC_UUID_DATABASE_CHANGE_INCREMENT           0x2A99 //Database Change Increment
#define CHARACTERISTIC_UUID_USER_INDEX                          0x2A9A //User Index
#define CHARACTERISTIC_UUID_BODY_COMPOSITION_FEATURE            0x2A9B //Body Composition Feature
#define CHARACTERISTIC_UUID_BODY_COMPOSITION_MEASUREMENT        0x2A9C //Body Composition Measurement
#define CHARACTERISTIC_UUID_WEIGHT_MEASUREMENT                  0x2A9D //Weight Measurement
#define CHARACTERISTIC_UUID_WEIGHT_SCALE_FEATURE                0x2A9E //Weight Scale Feature
#define CHARACTERISTIC_UUID_USER_CONTROL_POINT                  0x2A9F //User Control Point
#define CHARACTERISTIC_UUID_MAGNETIC_FLUX_DENSITY_2D            0x2AA0 //Magnetic Flux Density-2D
#define CHARACTERISTIC_UUID_MAGNETIC_FLUX_DENSITY_3D            0x2AA1 //Magnetic Flux Density-3D
#define CHARACTERISTIC_UUID_LANGUAGE                            0x2AA2 //Language
#define CHARACTERISTIC_UUID_BAROMETRIC_PRESSURE_TREND           0x2AA3 //Barometric Pressure Trend
#define CHARACTERISTIC_UUID_BOND_MANAGEMENT_CONTROL_POINT       0x2AA4 //Bond Management Control Point
#define CHARACTERISTIC_UUID_BOND_MANAGEMENT_FEATURE             0x2AA5 //Bond Management Feature
#define CHARACTERISTIC_UUID_CENTRAL_ADDRESS_RESOLUTION          0x2AA6 //Central Address Resolution
#define CHARACTERISTIC_UUID_CGM_MEASUREMENT                     0x2AA7 //CGM    Measurement
#define CHARACTERISTIC_UUID_CGM_FEATURE                         0x2AA8 //CGM    Feature
#define CHARACTERISTIC_UUID_CGM_STATUS                          0x2AA9 //CGM    Status
#define CHARACTERISTIC_UUID_CGM_SESSION_START_TIME              0x2AAA //CGM    Session Start Time
#define CHARACTERISTIC_UUID_CGM_SESSION_RUN_TIME                0x2AAB //CGM    Session Run Time
#define CHARACTERISTIC_UUID_CGM_SPECIFIC_OPS_CONTROL_POINT      0x2AAC //CGM    Specific Ops Control Point
#define CHARACTERISTIC_UUID_INDOOR_POSITIONING_CONFIGURATION    0x2AAD //Indoor Positioning Configuration
#define CHARACTERISTIC_UUID_LATITUDE                            0x2AAE //Latitude
#define CHARACTERISTIC_UUID_LONGITUDE                           0x2AAF //Longitude
#define CHARACTERISTIC_UUID_LOCAL_NORTH_COORDINATE              0x2AB0 //Local North Coordinate
#define CHARACTERISTIC_UUID_LOCAL_EAST_COORDINATE               0x2AB1 //Local East Coordinate
#define CHARACTERISTIC_UUID_FLOOR_NUMBER                        0x2AB2 //Floor Number
#define CHARACTERISTIC_UUID_ALTITUDE                            0x2AB3 //Altitude
#define CHARACTERISTIC_UUID_UNCERTAINTY                         0x2AB4 //Uncertainty
#define CHARACTERISTIC_UUID_LOCATION_NAME                       0x2AB5 //Location Name
#define CHARACTERISTIC_UUID_URI                                 0x2AB6 //URI
#define CHARACTERISTIC_UUID_HTTP_HEADERS                        0x2AB7 //HTTP Headers
#define CHARACTERISTIC_UUID_HTTP_STATUS_CODE                    0x2AB8 //HTTP Status Code
#define CHARACTERISTIC_UUID_HTTP_ENTITY_BODY                    0x2AB9 //HTTP Entity Body
#define CHARACTERISTIC_UUID_HTTP_CONTROL_POINT                  0x2ABA //HTTP Control Point
#define CHARACTERISTIC_UUID_HTTPS_SECURITY                      0x2ABB //HTTPS Security
#define CHARACTERISTIC_UUID_TDS_CONTROL_POINT                   0x2ABC //TDS Control Point
#define CHARACTERISTIC_UUID_OTS_FEATURE                         0x2ABD //OTS Feature
#define CHARACTERISTIC_UUID_OBJECT_NAME                         0x2ABE //Object Name
#define CHARACTERISTIC_UUID_OBJECT_TYPE                         0x2ABF //Object Type
#define CHARACTERISTIC_UUID_OBJECT_SIZE                         0x2AC0 //Object Size
#define CHARACTERISTIC_UUID_OBJECT_FIRST_CREATED                0x2AC1 //Object First-Created
#define CHARACTERISTIC_UUID_OBJECT_LAST_MODIFIED                0x2AC2 //Object Last-Modified
#define CHARACTERISTIC_UUID_OBJECT_ID                           0x2AC3 //Object ID
#define CHARACTERISTIC_UUID_OBJECT_PROPERTIES                   0x2AC4 //Object Properties
#define CHARACTERISTIC_UUID_OBJECT_ACTION_CONTROL_POINT         0x2AC5 //Object Action Control Point
#define CHARACTERISTIC_UUID_OBJECT_LIST_CONTROL_POINT           0x2AC6 //Object List Control Point
#define CHARACTERISTIC_UUID_OBJECT_LIST_FILTER                  0x2AC7 //Object List Filter
#define CHARACTERISTIC_UUID_OBJECT_CHANGED                      0x2AC8 //Object Changed
#define CHARACTERISTIC_UUID_RESOLVABLE_PRIVATE_ADDRESS_ONLY     0x2AC9 //Resolvable Private Address Only
#define CHARACTERISTIC_UUID_FITNESS_MACHINE_FEATURE             0x2ACC //Fitness Machine Feature
#define CHARACTERISTIC_UUID_TREADMILL_DATA                      0x2ACD //Treadmill Data
#define CHARACTERISTIC_UUID_CROSS_TRAINER_DATA                  0x2ACE //Cross Trainer Data
#define CHARACTERISTIC_UUID_STEP_CLIMBER_DATA                   0x2ACF //Step Climber Data
#define CHARACTERISTIC_UUID_STAIR_CLIMBER_DATA                  0x2AD0 //Stair Climber Data
#define CHARACTERISTIC_UUID_ROWER_DATA                          0x2AD1 //Rower Data
#define CHARACTERISTIC_UUID_INDOOR_BIKE_DATA                    0x2AD2 //Indoor Bike Data
#define CHARACTERISTIC_UUID_TRAINING_STATUS                     0x2AD3 //Training Status
#define CHARACTERISTIC_UUID_SUPPORTED_SPEED_RANGE               0x2AD4 //Supported Speed Range
#define CHARACTERISTIC_UUID_SUPPORTED_INCLINATION_RANGE         0x2AD5 //Supported Inclination Range
#define CHARACTERISTIC_UUID_SUPPORTED_RESISTANCE_LEVEL_RANGE    0x2AD6 //Supported Resistance Level Range
#define CHARACTERISTIC_UUID_SUPPORTED_HEART_RATE_RANGE          0x2AD7 //Supported Heart Rate Range
#define CHARACTERISTIC_UUID_SUPPORTED_POWER_RANGE               0x2AD8 //Supported Power Range
#define CHARACTERISTIC_UUID_FITNESS_MACHINE_CONTROL_POINT       0x2AD9 //Fitness Machine Control Point
#define CHARACTERISTIC_UUID_FITNESS_MACHINE_STATUS              0x2ADA //Fitness Machine Status
#define CHARACTERISTIC_UUID_MESH_PROVISIONING_DATA_IN           0x2ADB //Mesh Provisioning Data In
#define CHARACTERISTIC_UUID_MESH_PROVISIONING_DATA_OUT          0x2ADC //Mesh Provisioning Data Out
#define CHARACTERISTIC_UUID_MESH_PROXY_DATA_IN                  0x2ADD //Mesh Proxy Data In
#define CHARACTERISTIC_UUID_MESH_PROXY_DATA_OUT                 0x2ADE //Mesh Proxy Data Out
#define CHARACTERISTIC_UUID_AVERAGE_CURRENT                     0x2AE0 //Average Current
#define CHARACTERISTIC_UUID_AVERAGE_VOLTAGE                     0x2AE1 //Average Voltage
#define CHARACTERISTIC_UUID_BOOLEAN                             0x2AE2 //Boolean
#define CHARACTERISTIC_UUID_CHROMATIC_DIST_FROM_PLANCKIAN       0x2AE3 //Chromatic Distance from Planckian
#define CHARACTERISTIC_UUID_CHROMATICITY_COORDINATES            0x2AE4 //Chromaticity Coordinates
#define CHARACTERISTIC_UUID_CHROMATICITYIN_CCT_DUV_VALUES       0x2AE5 //Chromaticityin CCT and Duv Values
#define CHARACTERISTIC_UUID_CHROMATICITY_IN_CCT_DUV_VALUES      0x2AE5 //Chromaticityin CCT and Duv Values
#define CHARACTERISTIC_UUID_CHROMATICITY_TOLERANCE              0x2AE6 //Chromaticity Tolerance
#define CHARACTERISTIC_UUID_CIE13_3_1995_COLOR_INDEX            0x2AE7 //CIE13.3-1995 Color Rendering Index
#define CHARACTERISTIC_UUID_COEFFICIENT                         0x2AE8 //Coefficient
#define CHARACTERISTIC_UUID_CORRELATED_COLOR_TEMPERATURE        0x2AE9 //Correlated Color Temperature
#define CHARACTERISTIC_UUID_COUNT_16                            0x2AEA //Count 16
#define CHARACTERISTIC_UUID_COUNT_24                            0x2AEB //Count 24
#define CHARACTERISTIC_UUID_COUNTRY_CODE                        0x2AEC //Country Code
#define CHARACTERISTIC_UUID_DATE_UTC                            0x2AED //Date UTC
#define CHARACTERISTIC_UUID_ELECTRIC_CURRENT                    0x2AEE //Electric Current
#define CHARACTERISTIC_UUID_ELECTRIC_CURRENT_RANGE              0x2AEF //Electric Current Range
#define CHARACTERISTIC_UUID_ELECTRIC_CURRENT_SPECIFICATION      0x2AF0 //Electric Current Specification
#define CHARACTERISTIC_UUID_ELECTRIC_CURRENT_STATISTICS         0x2AF1 //Electric Current Statistics
#define CHARACTERISTIC_UUID_ENERGY                              0x2AF2 //Energy
#define CHARACTERISTIC_UUID_ENERGY_IN_A_PERIOD_OF_DAY           0x2AF3 //Energy in a Period of Day
#define CHARACTERISTIC_UUID_EVENT_STATISTICS                    0x2AF4 //Event Statistics
#define CHARACTERISTIC_UUID_FIXED_STRING_16                     0x2AF5 //Fixed String 16
#define CHARACTERISTIC_UUID_FIXED_STRING_24                     0x2AF6 //Fixed String 24
#define CHARACTERISTIC_UUID_FIXED_STRING_36                     0x2AF7 //Fixed String 36
#define CHARACTERISTIC_UUID_FIXED_STRING_8                      0x2AF8 //Fixed String 8
#define CHARACTERISTIC_UUID_GENERIC_LEVEL                       0x2AF9 //Generic Level
#define CHARACTERISTIC_UUID_GLOBAL_TRADE_ITEM_NUMBER            0x2AFA //Global Trade Item Number
#define CHARACTERISTIC_UUID_ILLUMINANCE                         0x2AFB //Illuminance
#define CHARACTERISTIC_UUID_LUMINOUS_EFFICACY                   0x2AFC //Luminous Efficacy
#define CHARACTERISTIC_UUID_LUMINOUS_ENERGY                     0x2AFD //Luminous Energy
#define CHARACTERISTIC_UUID_LUMINOUS_EXPOSURE                   0x2AFE //Luminous Exposure
#define CHARACTERISTIC_UUID_LUMINOUS_FLUX                       0x2AFF //Luminous Flux
#define CHARACTERISTIC_UUID_LUMINOUS_FLUX_RANGE                 0x2B00 //Luminous Flux Range
#define CHARACTERISTIC_UUID_LUMINOUS_INTENSITY                  0x2B01 //Luminous Intensity
#define CHARACTERISTIC_UUID_MASS_FLOW                           0x2B02 //Mass Flow
#define CHARACTERISTIC_UUID_PERCEIVED_LIGHTNESS                 0x2B03 //Perceived Lightness
#define CHARACTERISTIC_UUID_PERCENTAGE_8                        0x2B04 //Percentage_8
#define CHARACTERISTIC_UUID_POWER                               0x2B05 //Power
#define CHARACTERISTIC_UUID_POWER_SPECIFICATION                 0x2B06 //Power Specification
#define CHARACTERISTIC_UUID_RELATIVE_RUNTIME_CURRENT_RANGE      0x2B07 //Relative Runtime in a Current Range
#define CHARACTERISTIC_UUID_RELATIVE_RUNTIME_GENERIC_LEVEL      0x2B08 //Relative Runtime in a Generic Level Range
#define CHARACTERISTIC_UUID_RELATIVE_VALUE_VOLTAGE_RANGE        0x2B09 //Relative Value in a Voltage Range
#define CHARACTERISTIC_UUID_RELATIVE_VALUE_ILLUMINANCE          0x2B0A //Relative Value in an Illuminance Range
#define CHARACTERISTIC_UUID_RELATIVE_VALUE_PERIOD_OF_DAY        0x2B0B //Relative Value in a Period of Day
#define CHARACTERISTIC_UUID_RELATIVE_VALUE_TEMPERATURE_RANGE    0x2B0C //Relative Value in a Temperature Range
#define CHARACTERISTIC_UUID_TEMPERATURE_8                       0x2B0D //Temperature 8
#define CHARACTERISTIC_UUID_TEMPERATURE_8_IN_A_PERIOD_OF_DAY    0x2B0E //Temperature 8 in a Period of Day
#define CHARACTERISTIC_UUID_TEMPERATURE_8_STATISTICS            0x2B0F //Temperature 8 Statistics
#define CHARACTERISTIC_UUID_TEMPERATURE_RANGE                   0x2B10 //Temperature Range
#define CHARACTERISTIC_UUID_TEMPERATURE_STATISTICS              0x2B11 //Temperature Statistics
#define CHARACTERISTIC_UUID_TIME_DECIHOUR_8                     0x2B12 //Time Decihour 8
#define CHARACTERISTIC_UUID_TIME_EXPONENTIAL_8                  0x2B13 //Time Exponential 8
#define CHARACTERISTIC_UUID_TIME_HOUR_24                        0x2B14 //Time Hour 24
#define CHARACTERISTIC_UUID_TIME_MILLISECOND_24                 0x2B15 //Time Millisecond 24
#define CHARACTERISTIC_UUID_TIME_SECOND_16                      0x2B16 //Time Second1 6
#define CHARACTERISTIC_UUID_TIME_SECOND_8                       0x2B17 //Time Second 8
#define CHARACTERISTIC_UUID_VOLTAGE                             0x2B18 //Voltage
#define CHARACTERISTIC_UUID_VOLTAGE_SPECIFICATION               0x2B19 //Voltage Specification
#define CHARACTERISTIC_UUID_VOLTAGE_STATISTICS                  0x2B1A //Voltage Statistics
#define CHARACTERISTIC_UUID_VOLUME_FLOW                         0x2B1B //Volume Flow
#define CHARACTERISTIC_UUID_CHROMATICITY_COORDINATE             0x2B1C //Chromaticity Coordinate
#define CHARACTERISTIC_UUID_RC_FEATURE                          0x2B1D //RC Feature
#define CHARACTERISTIC_UUID_RC_SETTINGS                         0x2B1E //RC Settings
#define CHARACTERISTIC_UUID_RECONN_CFG_CONTROL_POINT            0x2B1F //Reconnection Configuration Control Point
#define CHARACTERISTIC_UUID_IDD_STATUS_CHANGED                  0x2B20 //IDD Status Changed
#define CHARACTERISTIC_UUID_IDD_STATUS                          0x2B21 //IDD Status
#define CHARACTERISTIC_UUID_IDD_ANNUNCIATION_STATUS             0x2B22 //IDD Annunciation Status
#define CHARACTERISTIC_UUID_IDD_FEATURES                        0x2B23 //IDD Features
#define CHARACTERISTIC_UUID_IDD_STATUS_READER_CONTROL_POINT     0x2B24 //IDD Status Reader Control Point
#define CHARACTERISTIC_UUID_IDD_COMMAND_CONTROL_POINT           0x2B25 //IDD Command Control Point
#define CHARACTERISTIC_UUID_IDD_COMMAND_DATA                    0x2B26 //IDD Command Data
#define CHARACTERISTIC_UUID_IDD_RECORD_ACCESS_CONTROL_POINT     0x2B27 //IDD Record Access Control Point
#define CHARACTERISTIC_UUID_IDD_HISTORY_DATA                    0x2B28 //IDD History Data
#define CHARACTERISTIC_UUID_CLIENT_SUPPORTED_FEATURES           0x2B29 //Client Supported Features
#define CHARACTERISTIC_UUID_DATABASE_HASH                       0x2B2A //Database Hash
#define CHARACTERISTIC_UUID_BSS_CONTROL_POINT                   0x2B2B //BSS Control Point
#define CHARACTERISTIC_UUID_BSS_RESPONSE                        0x2B2C //BSS Response
#define CHARACTERISTIC_UUID_EMERGENCY_ID                        0x2B2D //Emergency ID
#define CHARACTERISTIC_UUID_EMERGENCY_TEXT                      0x2B2E //Emergency Text
#define CHARACTERISTIC_UUID_ACS_STATUS                          0x2B2F //ACS Status
#define CHARACTERISTIC_UUID_ACS_DATA_IN                         0x2B30 //ACS Data In
#define CHARACTERISTIC_UUID_ACS_DATA_OUT_NOTIFY                 0x2B31 //ACS Data Out Notify
#define CHARACTERISTIC_UUID_ACS_DATA_OUT_INDICATE               0x2B32 //ACS Data Out Indicate
#define CHARACTERISTIC_UUID_ACS_CONTROL_POINT                   0x2B33 //ACS Control Point
#define CHARACTERISTIC_UUID_ENHANCED_BP_MEASUREMENT             0x2B34 //Enhanced Blood Pressure Measurement
#define CHARACTERISTIC_UUID_ENHANCED_INTERMEDIATE_CUFF_PRESSURE 0x2B35 //Enhanced Intermediate Cuff Pressure
#define CHARACTERISTIC_UUID_ENHANCED_INTERM_CUFF_PRESSURE       0x2B35 //Enhanced Intermediate Cuff Pressure
#define CHARACTERISTIC_UUID_BLOOD_PRESSURE_RECORD               0x2B36 //Blood Pressure Record
#define CHARACTERISTIC_UUID_REGISTERED_USER                     0x2B37 //Registered User
#define CHARACTERISTIC_UUID_BR_EDR_HANDOVER_DATA                0x2B38 //BR-EDR Handover Data
#define CHARACTERISTIC_UUID_BLUETOOTH_SIG_DATA                  0x2B39 //Bluetooth SIG Data
#define CHARACTERISTIC_UUID_SERVER_SUPPORTED_FEATURES           0x2B3A //Server Supported Features
#define CHARACTERISTIC_UUID_PHYSICAL_AM_FEATURES                0x2B3B //Physical Activity Monitor Features
#define CHARACTERISTIC_UUID_GENERAL_ACTIVITY_INST_DATA          0x2B3C //General Activity Instantaneous Data
#define CHARACTERISTIC_UUID_GENERAL_ACTIVITY_SUMMARY_DATA       0x2B3D //General Activity Summary Data
#define CHARACTERISTIC_UUID_CR_ACTIVITY_INSTANTANEOUS_DATA      0x2B3E //Cardio Respiratory Activity Instantaneous Data
#define CHARACTERISTIC_UUID_CR_ACTIVITY_SUMMARY_DATA            0x2B3F //Cardio Respiratory Activity Summary Data
#define CHARACTERISTIC_UUID_SC_ACTIVITY_SUMMARY_DATA            0x2B40 //Step Counter Activity Summary Data
#define CHARACTERISTIC_UUID_SLEEP_ACTIVITY_INST_DATA            0x2B41 //Sleep Activity Instantaneous Data
#define CHARACTERISTIC_UUID_SLEEP_ACTIVITY_SUMMARY_DATA         0x2B42 //Sleep Activity Summary Data
#define CHARACTERISTIC_UUID_PHYSICAL_AM_CONTROL_POINT           0x2B43 //Physical Activity Monitor Control Point
#define CHARACTERISTIC_UUID_ACTIVITY_CURRENT_SESSION            0x2B44 //Activity Current Session
#define CHARACTERISTIC_UUID_PHYSICAL_AS_DESCRIPTOR              0x2B45 //Physical Activity Session Descriptor
#define CHARACTERISTIC_UUID_PREFERRED_UNITS                     0x2B46 //Preferred Units
#define CHARACTERISTIC_UUID_HIGH_RESOLUTION_HEIGHT              0x2B47 //High Resolution Height
#define CHARACTERISTIC_UUID_MIDDLE_NAME                         0x2B48 //Middle Name
#define CHARACTERISTIC_UUID_STRIDE_LENGTH                       0x2B49 //Stride Length
#define CHARACTERISTIC_UUID_HANDEDNESS                          0x2B4A //Handedness
#define CHARACTERISTIC_UUID_DEVICE_WEARING_POSITION             0x2B4B //Device Wearing Position
#define CHARACTERISTIC_UUID_FOUR_ZONE_HEART_RATE_LIMITS         0x2B4C //Four Zone Heart Rate Limits
#define CHARACTERISTIC_UUID_HIGH_INTENSITY_EXERCISE_TH          0x2B4D //High Intensity Exercise Threshold
#define CHARACTERISTIC_UUID_ACTIVITY_GOAL                       0x2B4E //Activity Goal
#define CHARACTERISTIC_UUID_SEDENTARY_INTERVAL_NOTIFICATION     0x2B4F //Sedentary Interval Notification
#define CHARACTERISTIC_UUID_CALORIC_INTAKE                      0x2B50 //Caloric Intake
#define CHARACTERISTIC_UUID_TMAP_ROLE                           0x2B51 //TMAP Role
#define CHARACTERISTIC_UUID_AUDIO_INPUT_STATE                   0x2B77 //Audio Input State
#define CHARACTERISTIC_UUID_GAIN_SETTINGS_ATTRIBUTE             0x2B78 //Gain Settings Attribute
#define CHARACTERISTIC_UUID_AUDIO_INPUT_TYPE                    0x2B79 //Audio Input Type
#define CHARACTERISTIC_UUID_AICS_INPUT_STATUS                   0x2B7A //Audio Input Status
#define CHARACTERISTIC_UUID_AUDIO_INPUT_CONTROL_POINT           0x2B7B //Audio Input Control Point
#define CHARACTERISTIC_UUID_AUDIO_INPUT_DESCRIPTION             0x2B7C //Audio Input Description
#define CHARACTERISTIC_UUID_VOLUME_STATE                        0x2B7D //Volume State
#define CHARACTERISTIC_UUID_VOLUME_CONTROL_POINT                0x2B7E //Volume Control Point
#define CHARACTERISTIC_UUID_VOLUME_FLAGS                        0x2B7F //Volume Flags
#define CHARACTERISTIC_UUID_VOLUME_OFFSET_STATE                 0x2B80 //Volume Offset State
#define CHARACTERISTIC_UUID_AUDIO_LOCATION                      0x2B81 //Audio Location
#define CHARACTERISTIC_UUID_VOLUME_OFFSET_CONTROL_POINT         0x2B82 //Volume Offset Control Point
#define CHARACTERISTIC_UUID_AUDIO_OUTPUT_DESCRIPTION            0x2B83 //Audio Output Description
#define CHARACTERISTIC_UUID_SET_IDENTITY_RESOLVING_KEY          0x2B84 //Set Identity Resolving Key
#define CHARACTERISTIC_UUID_COORDINATED_SET_SIZE                0x2B85 //Coordinated Set Size
#define CHARACTERISTIC_UUID_SET_MEMBER_LOCK                     0x2B86 //Set Member Lock
#define CHARACTERISTIC_UUID_SET_MEMBER_RANK                     0x2B87 //Set Member Rank
#define CHARACTERISTIC_UUID_ENCRYPTED_DATA_KEY_MATERIAL         0x2B88 //Encrypted Data Key Material
#define CHARACTERISTIC_UUID_APPARENT_ENERGY_32                  0x2B89 //Apparent Energy 32
#define CHARACTERISTIC_UUID_APPARENT_POWER                      0x2B8A //Apparent Power
#define CHARACTERISTIC_UUID_LIVE_HEALTH_OBSERVATIONS            0x2B8B //Live Health Observations
#define CHARACTERISTIC_UUID_CO_CONC                             0x2B8C //CO\{}textsubscript{2} Concentration
#define CHARACTERISTIC_UUID_COSINE_OF_THE_ANGLE                 0x2B8D //Cosine of the Angle
#define CHARACTERISTIC_UUID_DEVICE_TIME_FEATURE                 0x2B8E //Device Time Feature
#define CHARACTERISTIC_UUID_DEVICE_TIME_PARAMETERS              0x2B8F //Device Time Parameters
#define CHARACTERISTIC_UUID_DEVICE_TIME                         0x2B90 //Device Time
#define CHARACTERISTIC_UUID_DEVICE_TIME_CONTROL_POINT           0x2B91 //Device Time Control Point
#define CHARACTERISTIC_UUID_TIME_CHANGE_LOG_DATA                0x2B92 //Time Change Log Data
#define CHARACTERISTIC_UUID_MEDIA_PLAYER_NAME                   0x2B93 //Media Player Name
#define CHARACTERISTIC_UUID_MEDIA_PLAYER_ICON_OBJECT_ID         0x2B94 //Media Player Icon Object ID
#define CHARACTERISTIC_UUID_MEDIA_PLAYER_ICON_URL               0x2B95 //Media Player Icon URL
#define CHARACTERISTIC_UUID_TRACK_CHANGED                       0x2B96 //Track Changed
#define CHARACTERISTIC_UUID_TRACK_TITLE                         0x2B97 //Track Title
#define CHARACTERISTIC_UUID_TRACK_DURATION                      0x2B98 //Track Duration
#define CHARACTERISTIC_UUID_TRACK_POSITION                      0x2B99 //Track Position
#define CHARACTERISTIC_UUID_PLAYBACK_SPEED                      0x2B9A //Playback Speed
#define CHARACTERISTIC_UUID_SEEKING_SPEED                       0x2B9B //Seeking Speed
#define CHARACTERISTIC_UUID_CURRENT_TRACK_SEGMENTS_OBJECT_ID    0x2B9C //Current Track Segments Object ID
#define CHARACTERISTIC_UUID_CURRENT_TRACK_OBJECT_ID             0x2B9D //Current Track Object ID
#define CHARACTERISTIC_UUID_NEXT_TRACK_OBJECT_ID                0x2B9E //Next Track Object ID
#define CHARACTERISTIC_UUID_PARENT_GROUP_OBJECT_ID              0x2B9F //Parent Group Object ID
#define CHARACTERISTIC_UUID_CURRENT_GROUP_OBJECT_ID             0x2BA0 //Current Group Object ID
#define CHARACTERISTIC_UUID_PLAYING_ORDER                       0x2BA1 //Playing Order
#define CHARACTERISTIC_UUID_PLAYING_ORDERS_SUPPORTED            0x2BA2 //Playing Orders Supported
#define CHARACTERISTIC_UUID_MEDIA_STATE                         0x2BA3 //Media State
#define CHARACTERISTIC_UUID_MEDIA_CONTROL_POINT                 0x2BA4 //Media Control Point
#define CHARACTERISTIC_UUID_MEDIA_CTRL_POINT_OP_SUPPORTED       0x2BA5 //Media Control Point Opcodes Supported
#define CHARACTERISTIC_UUID_SEARCH_RESULTS_OBJECT_ID            0x2BA6 //Search Results Object ID
#define CHARACTERISTIC_UUID_SEARCH_CONTROL_POINT                0x2BA7 //Search Control Point
#define CHARACTERISTIC_UUID_ENERGY_32                           0x2BA8 //Energy 32
#define CHARACTERISTIC_UUID_MEDIA_PLAYER_ICON_OBJECT_TYPE       0x2BA9 //Media Player Icon Object Type
#define CHARACTERISTIC_UUID_TRACK_SEGMENTS_OBJECT_TYPE          0x2BAA //Track Segments Object Type
#define CHARACTERISTIC_UUID_TRACK_OBJECT_TYPE                   0x2BAB //Track Object Type
#define CHARACTERISTIC_UUID_GROUP_OBJECT_TYPE                   0x2BAC //Group Object Type
#define CHARACTERISTIC_UUID_CONSTANT_TONE_EXTENSION_ENABLE      0x2BAD //Constant Tone Extension Enable
#define CHARACTERISTIC_UUID_ADVERTISING_CTE_MIN_LENGTH          0x2BAE //Advertising Constant Tone Extension Minimum Length
#define CHARACTERISTIC_UUID_ADVERTISING_CTE_MIN_TRANSMIT_CNT    0x2BAF //Advertising Constant Tone Extension Minimum Transmit Count
#define CHARACTERISTIC_UUID_ADVERTISING_CTE_TRAN_DURATION       0x2BB0 //Advertising Constant Tone Extension Transmit Duration
#define CHARACTERISTIC_UUID_ADVERTISING_CTE_INTERVAL            0x2BB1 //Advertising Constant Tone Extension Interval
#define CHARACTERISTIC_UUID_ADVERTISING_CTE_PHY                 0x2BB2 //Advertising Constant Tone Extension PHY
#define CHARACTERISTIC_UUID_BEARER_PROVIDER_NAME                0x2BB3 //Bearer Provider Name
#define CHARACTERISTIC_UUID_BEARER_UCI                          0x2BB4 //Bearer UCI
#define CHARACTERISTIC_UUID_BEARER_TECHNOLOGY                   0x2BB5 //Bearer Technology
#define CHARACTERISTIC_UUID_BEARER_URI_SCHEMES_SUPPRTED_LIST    0x2BB6 //Bearer URI Schemes Supported List
#define CHARACTERISTIC_UUID_BEARER_URI_SCHEMES_SUPPORTED_LIST   0x2BB6 //Bearer URI Schemes Supported List
#define CHARACTERISTIC_UUID_BEARER_SS                           0x2BB7 //Bearer Signal Strength
#define CHARACTERISTIC_UUID_BEARER_SS_REPORTING_INTERVAL        0x2BB8 //Bearer Signal Strength Reporting Interval
#define CHARACTERISTIC_UUID_BEARER_LIST_CURRENT_CALLS           0x2BB9 //Bearer List Current Calls
#define CHARACTERISTIC_UUID_CONTENT_CONTROL_ID                  0x2BBA //Content Control ID
#define CHARACTERISTIC_UUID_STATUS_FLAGS                        0x2BBB //Status Flags
#define CHARACTERISTIC_UUID_INCOMING_CALL_TARGET_BEARER_URI     0x2BBC //Incoming Call Target Bearer URI
#define CHARACTERISTIC_UUID_CALL_STATE                          0x2BBD //Call State
#define CHARACTERISTIC_UUID_CALL_CTRL_POINT                     0x2BBE //Call Control Point
#define CHARACTERISTIC_UUID_CALL_CTRL_POINT_OPTIONAL_OPCODES    0x2BBF //Call Control Point Optional Opcodes
#define CHARACTERISTIC_UUID_TERMINATION_REASON                  0x2BC0 //Termination Reason
#define CHARACTERISTIC_UUID_INCOMING_CALL                       0x2BC1 //Incoming Call
#define CHARACTERISTIC_UUID_CALL_FRIENDLY_NAME                  0x2BC2 //Call Friendly Name
#define CHARACTERISTIC_UUID_MUTE                                0x2BC3 //Mute
#define CHARACTERISTIC_UUID_SINK_ASE                            0x2BC4 //Sink ASE
#define CHARACTERISTIC_UUID_SOURCE_ASE                          0x2BC5 //Source ASE
#define CHARACTERISTIC_UUID_ASE_CONTROL_POINT                   0x2BC6 //ASE Control Point
#define CHARACTERISTIC_UUID_BAS_CONTROL_POINT                   0x2BC7 //Broadcast Audio Scan Control Point
#define CHARACTERISTIC_UUID_BROADCAST_RECEIVE_STATE             0x2BC8 //Broadcast Receive State
#define CHARACTERISTIC_UUID_SINK_PAC                            0x2BC9 //Sink PAC
#define CHARACTERISTIC_UUID_SINK_AUDIO_LOCATIONS                0x2BCA //Sink Audio Locations
#define CHARACTERISTIC_UUID_SOURCE_PAC                          0x2BCB //Source PAC
#define CHARACTERISTIC_UUID_SOURCE_AUDIO_LOCATIONS              0x2BCC //Source Audio Locations
#define CHARACTERISTIC_UUID_AVAILABLE_AUDIO_CONTEXTS            0x2BCD //Available Audio Contexts
#define CHARACTERISTIC_UUID_SUPPORTED_AUDIO_CONTEXTS            0x2BCE //Supported Audio Contexts
#define CHARACTERISTIC_UUID_AMMONIA_CONC                        0x2BCF //Ammonia Concentration
#define CHARACTERISTIC_UUID_CARBON_MONOXIDE_CONC                0x2BD0 //Carbon Monoxide Concentration
#define CHARACTERISTIC_UUID_METHANE_CONC                        0x2BD1 //Methane Concentration
#define CHARACTERISTIC_UUID_NITROGEN_DIOXIDE_CONC               0x2BD2 //Nitrogen Dioxide Concentration
#define CHARACTERISTIC_UUID_NON_METHANE_VOLATILE_OCS_CONC       0x2BD3 //Non-Methane Volatile Organic Compounds Concentration
#define CHARACTERISTIC_UUID_OZONE_CONC                          0x2BD4 //Ozone Concentration
#define CHARACTERISTIC_UUID_PARTICULATE_MATTER_PM1_CONC         0x2BD5 //Particulate Matter-PM1 Concentration
#define CHARACTERISTIC_UUID_PARTICULATE_MATTER_PM2_5_CONC       0x2BD6 //Particulate Matter-PM2.5 Concentration
#define CHARACTERISTIC_UUID_PARTICULATE_MATTER_PM10_CONC        0x2BD7 //Particulate Matter-PM10 Concentration
#define CHARACTERISTIC_UUID_SULFUR_DIOXIDE_CONC                 0x2BD8 //Sulfur Dioxide Concentration
#define CHARACTERISTIC_UUID_SULFUR_HEXAFLUORIDE_CONC            0x2BD9 //Sulfur Hexafluoride Concentration
#define CHARACTERISTIC_UUID_HEARING_AID_FEATURES                0x2BDA //Hearing Aid Features
#define CHARACTERISTIC_UUID_HEARING_AID_PRESET_CONTROL_POINT    0x2BDB //Hearing Aid Preset Control Point
#define CHARACTERISTIC_UUID_ACTIVE_PRESET_INDEX                 0x2BDC //Active Preset Index
#define CHARACTERISTIC_UUID_STORED_HEALTH_OBSERVATIONS          0x2BDD //Stored Health Observations
#define CHARACTERISTIC_UUID_FIXED_STRING_64                     0x2BDE //Fixed String 64
#define CHARACTERISTIC_UUID_HIGH_TEMPERATURE                    0x2BDF //High Temperature
#define CHARACTERISTIC_UUID_HIGH_VOLTAGE                        0x2BE0 //High Voltage
#define CHARACTERISTIC_UUID_LIGHT_DISTRIBUTION                  0x2BE1 //Light Distribution
#define CHARACTERISTIC_UUID_LIGHT_OUTPUT                        0x2BE2 //Light Output
#define CHARACTERISTIC_UUID_LIGHT_SOURCE_TYPE                   0x2BE3 //Light Source Type
#define CHARACTERISTIC_UUID_NOISE                               0x2BE4 //Noise
#define CHARACTERISTIC_UUID_RELATIVE_RUNTIME2                   0x2BE5 //Relative Runtime in a Correlated Color Temperature Range
#define CHARACTERISTIC_UUID_TIME_SECOND_32                      0x2BE6 //Time Second 32
#define CHARACTERISTIC_UUID_VOC_CONC                            0x2BE7 //VOC Concentration
#define CHARACTERISTIC_UUID_VOLTAGE_FREQUENCY                   0x2BE8 //Voltage Frequency
#define CHARACTERISTIC_UUID_BATTERY_CRITICAL_STATUS             0x2BE9 //Battery Critical Status
#define CHARACTERISTIC_UUID_BATTERY_HEALTH_STATUS               0x2BEA //Battery Health Status
#define CHARACTERISTIC_UUID_BATTERY_HEALTH_INFORMATION          0x2BEB //Battery Health Information
#define CHARACTERISTIC_UUID_BATTERY_INFORMATION                 0x2BEC //Battery Information
#define CHARACTERISTIC_UUID_BATTERY_LEVEL_STATUS                0x2BED //Battery Level Status
#define CHARACTERISTIC_UUID_BATTERY_TIME_STATUS                 0x2BEE //Battery Time Status
#define CHARACTERISTIC_UUID_ESTIMATED_SERVICE_DATE              0x2BEF //Estimated Service Date
#define CHARACTERISTIC_UUID_BATTERY_ENERGY_STATUS               0x2BF0 //Battery Energy Status
#define CHARACTERISTIC_UUID_OBSERVATION_SCHEDULE_CHANGED        0x2BF1 //Observation Schedule Changed
#define CHARACTERISTIC_UUID_CURRENT_ELAPSED_TIME                0x2BF2 //Current Elapsed Time
#define CHARACTERISTIC_UUID_HEALTH_SENSOR_FEATURES              0x2BF3 //Health Sensor Features
#define CHARACTERISTIC_UUID_GHS_CONTROL_POINT                   0x2BF4 //GHS Control Point
#define CHARACTERISTIC_UUID_LE_GATT_SECURITY_LEVELS             0x2BF5 //LE GATT Security Levels
#define CHARACTERISTIC_UUID_ESL_ADDRESS                         0x2BF6 //ESL Address
#define CHARACTERISTIC_UUID_AP_SYNC_KEY_MATERIAL                0x2BF7 //AP Sync Key Material
#define CHARACTERISTIC_UUID_ESL_RESPONSE_KEY_MATERIAL           0x2BF8 //ESL Response Key Material
#define CHARACTERISTIC_UUID_ESL_CURRENT_ABSOLUTE_TIME           0x2BF9 //ESL Current Absolute Time
#define CHARACTERISTIC_UUID_ESL_DISPLAY_INFORMATION             0x2BFA //ESL Display Information
#define CHARACTERISTIC_UUID_ESL_IMAGE_INFORMATION               0x2BFB //ESL Image Information
#define CHARACTERISTIC_UUID_ESL_SENSOR_INFORMATION              0x2BFC //ESL Sensor Information
#define CHARACTERISTIC_UUID_ESL_LED_INFORMATION                 0x2BFD //ESL LED Information
#define CHARACTERISTIC_UUID_ESL_CONTROL_POINT                   0x2BFE //ESL Control Point
#define CHARACTERISTIC_UUID_UDI_FOR_MEDICAL_DEVICES             0x2BFF //UDI for Medical Devices
#define CHARACTERISTIC_UUID_GMAP_ROLE                           0x2C00 //GMAP Role
#define CHARACTERISTIC_UUID_UGG_FEATURES                        0x2C01 //UGG Features
#define CHARACTERISTIC_UUID_UGT_FEATURES                        0x2C02 //UGT Features
#define CHARACTERISTIC_UUID_BGS_FEATURES                        0x2C03 //BGS Features
#define CHARACTERISTIC_UUID_BGR_FEATURES                        0x2C04 //BGR Features
#define CHARACTERISTIC_UUID_RAS_FEATURE                         0x2C14 //RAS feature
#define CHARACTERISTIC_UUID_REAL_TIME_PROCEDURE_DATA            0x2C15 //Real Time Procedure Data
#define CHARACTERISTIC_UUID_ON_DEMAND_PROCEDURE_DATA            0x2C16 //On Demand Procedure Data
#define CHARACTERISTIC_UUID_CONTROL_POINT                       0x2C17 //Control Point
#define CHARACTERISTIC_UUID_RANGING_DATA_READY                  0x2C18 //Ranging Data Ready
#define CHARACTERISTIC_UUID_RANGING_DATA_OVERWRITTEN            0x2C19 //Ranging Data Overwritten
#define CHARACTERISTIC_UUID_HID_ISO_PROPERTIES                  0x2C23 //Ultra Low Latency HID Properties
#define CHARACTERISTIC_UUID_LE_HID_OPERATION_MODE               0x2C24 //LE HID Operation mode
//TODO: Coordinated Set Name Characteristic has not been confirmed yet.
#define CHARACTERISTIC_UUID_COORDINATED_SET_NAME 0x7FE6 //Coordinated Set Name

///////////////////////
//old UUID define
//////////////////////
/**
 *  @brief  GATT 16 bit UUID definition
 */
#define GATT_UUID_PRIMARY_SERVICE     0x2800 //!< Primary Service
#define GATT_UUID_SECONDARY_SERVICE   0x2801 //!< Secondary Service
#define GATT_UUID_INCLUDE             0x2802 //!< Include
#define GATT_UUID_CHARACTER           0x2803 //!< Characteristic
#define GATT_UUID_CHAR_EXT_PROPS      0x2900 //!< Characteristic Extended Properties
#define GATT_UUID_CHAR_USER_DESC      0x2901 //!< Characteristic User Description
#define GATT_UUID_CLIENT_CHAR_CFG     0x2902 //!< Client Characteristic Configuration
#define GATT_UUID_SERVER_CHAR_CFG     0x2903 //!< Server Characteristic Configuration
#define GATT_UUID_CHAR_PRESENT_FORMAT 0x2904 //!< Characteristic Present Format
#define GATT_UUID_CHAR_AGG_FORMAT     0x2905 //!< Characteristic Aggregate Format
#define GATT_UUID_VALID_RANGE         0x2906 //!< Valid Range
#define GATT_UUID_EXT_REPORT_REF      0x2907 //!< External Report Reference
#define GATT_UUID_REPORT_REF          0x2908 //!< Report Reference

#define GATT_UUID_DEVICE_NAME         0x2a00 //!< Report Reference
#define GATT_UUID_APPEARANCE          0x2a01
#define GATT_UUID_PERI_CONN_PARAM     0x2a04
#define GATT_UUID_SERVICE_CHANGE      0x2a05
#define GATT_UUID_BATTERY_LEVEL       0x2A19
#define GATT_UUID_FIRMWARE_VER        0x2a26 //!<Firmware Version
#define GATT_UUID_CENTRAL_ADDR_RES    0x2AA6
#define GATT_UUID_RAP_ONLY            0x2AC9

/**
 * @brief   hids_uuid Hids Characteristic UUID
 */
#define CHARACTERISTIC_UUID_HID_BOOT_KEY_INPUT   0x2A22 //!< HID Boot Keyboard Input Report
#define CHARACTERISTIC_UUID_HID_BOOT_KEY_OUTPUT  0x2A32 //!< HID Boot Keyboard Output Report
#define CHARACTERISTIC_UUID_HID_BOOT_MOUSE_INPUT 0x2A33 //!< HID Boot Mouse Input Report
#define CHARACTERISTIC_UUID_HID_INFORMATION      0x2A4A //!< HID Information
#define CHARACTERISTIC_UUID_HID_REPORT_MAP       0x2A4B //!< HID Report Map
#define CHARACTERISTIC_UUID_HID_CONTROL_POINT    0x2A4C //!< HID Control Point
#define CHARACTERISTIC_UUID_HID_REPORT           0x2A4D //!< HID Report
#define CHARACTERISTIC_UUID_HID_PROTOCOL_MODE    0x2A4E //!< HID Protocol Mode


////////////////////////////////////
// Telink Service
////////////////////////////////////
#define WRAPPING_BRACES(__DATA__)     {__DATA__}
#define TELINK_SPP_UUID_SERVICE       0x10, 0x19, 0x0d, 0x0c, 0x0b, 0x0a, 0x09, 0x08, 0x07, 0x06, 0x05, 0x04, 0x03, 0x02, 0x01, 0x00 //!< TELINK_SPP service
#define TELINK_AUDIO_UUID_SERVICE     0x11, 0x19, 0x0d, 0x0c, 0x0b, 0x0a, 0x09, 0x08, 0x07, 0x06, 0x05, 0x04, 0x03, 0x02, 0x01, 0x00 //!< TELINK_SPP service
#define TELINK_OTA_UUID_SERVICE       0x12, 0x19, 0x0d, 0x0c, 0x0b, 0x0a, 0x09, 0x08, 0x07, 0x06, 0x05, 0x04, 0x03, 0x02, 0x01, 0x00 //!< TELINK_OTA service
#define TELINK_MESH_UUID_SERVICE      0x20, 0x19, 0x0d, 0x0c, 0x0b, 0x0a, 0x09, 0x08, 0x07, 0x06, 0x05, 0x04, 0x03, 0x02, 0x01, 0x00 //!< TELINK_SPP service
#define TELINK_MESH_LT_UUID_SERVICE   0x21, 0x19, 0x0d, 0x0c, 0x0b, 0x0a, 0x09, 0x08, 0x07, 0x06, 0x05, 0x04, 0x03, 0x02, 0x01, 0x00 //!< TELINK_SPP service

#define TELINK_SPP_DATA_SERVER2CLIENT 0x10, 0x2B, 0x0d, 0x0c, 0x0b, 0x0a, 0x09, 0x08, 0x07, 0x06, 0x05, 0x04, 0x03, 0x02, 0x01, 0x00 //!< TELINK_SPP data from server to client
#define TELINK_SPP_DATA_CLIENT2SERVER 0x11, 0x2B, 0x0d, 0x0c, 0x0b, 0x0a, 0x09, 0x08, 0x07, 0x06, 0x05, 0x04, 0x03, 0x02, 0x01, 0x00 //!< TELINK_SPP data from client to server
#define TELINK_SPP_DATA_OTA           0x12, 0x2B, 0x0d, 0x0c, 0x0b, 0x0a, 0x09, 0x08, 0x07, 0x06, 0x05, 0x04, 0x03, 0x02, 0x01, 0x00 //!< TELINK_SPP data for ota
#define TELINK_SPP_DATA_PAIR          0x13, 0x2B, 0x0d, 0x0c, 0x0b, 0x0a, 0x09, 0x08, 0x07, 0x06, 0x05, 0x04, 0x03, 0x02, 0x01, 0x00 //!< TELINK_SPP data for ota
#define TELINK_SPP_DATA_DEFINE        0x14, 0x2B, 0x0d, 0x0c, 0x0b, 0x0a, 0x09, 0x08, 0x07, 0x06, 0x05, 0x04, 0x03, 0x02, 0x01, 0x00 //!< TELINK_SPP data for userdefine data


#define TELINK_MIC_DATA               0x18, 0x2B, 0x0d, 0x0c, 0x0b, 0x0a, 0x09, 0x08, 0x07, 0x06, 0x05, 0x04, 0x03, 0x02, 0x01, 0x00 //!< TELINK_SPP data from server to client
#define TELINK_SPEAKER_DATA           0x19, 0x2B, 0x0d, 0x0c, 0x0b, 0x0a, 0x09, 0x08, 0x07, 0x06, 0x05, 0x04, 0x03, 0x02, 0x01, 0x00 //!< TELINK_SPP data from client to server


#define AUDIO_GOOGLE_SERVICE_UUID     0x64, 0xB6, 0x17, 0xF6, 0x01, 0xAF, 0x7D, 0xBC, 0x05, 0x4F, 0x21, 0x5A, 0x01, 0x00, 0x5E, 0xAB
#define AUDIO_GOOGL_TX_CHAR_UUID      0x64, 0xB6, 0x17, 0xF6, 0x01, 0xAF, 0x7D, 0xBC, 0x05, 0x4F, 0x21, 0x5A, 0x02, 0x00, 0x5E, 0xAB
#define AUDIO_GOOGL_RX_CHAR_UUID      0x64, 0xB6, 0x17, 0xF6, 0x01, 0xAF, 0x7D, 0xBC, 0x05, 0x4F, 0x21, 0x5A, 0x03, 0x00, 0x5E, 0xAB
#define AUDIO_GOOGL_CTL_CHAR_UUID     0x64, 0xB6, 0x17, 0xF6, 0x01, 0xAF, 0x7D, 0xBC, 0x05, 0x4F, 0x21, 0x5A, 0x04, 0x00, 0x5E, 0xAB

// google defined 128bit uuid for special ranging setting
#define CHARACTERISTIC_UUID_SRS_READ_CAP 0xF1, 0x35, 0x9C, 0x5F, 0x80, 0x00, 0x00, 0x80, 0x00, 0x10, 0x00, 0x00, 0x01, 0x8F, 0x00, 0x00
#define CHARACTERISTIC_UUID_SRS_SEND_CMD 0xF1, 0x35, 0x9C, 0x5F, 0x80, 0x00, 0x00, 0x80, 0x00, 0x10, 0x00, 0x00, 0x02, 0x8F, 0x00, 0x00

extern const unsigned char serviceGenericAccessUuid[ATT_16_UUID_LEN];
extern const unsigned char serviceGenericAttributeUuid[ATT_16_UUID_LEN];
extern const unsigned char serviceImmediateAlertUuid[ATT_16_UUID_LEN];
extern const unsigned char serviceLinkLossUuid[ATT_16_UUID_LEN];
extern const unsigned char serviceTxPowerUuid[ATT_16_UUID_LEN];
extern const unsigned char serviceCurrentTimeUuid[ATT_16_UUID_LEN];
extern const unsigned char serviceReferenceTimeUpdateUuid[ATT_16_UUID_LEN];
extern const unsigned char serviceNextDstChangeUuid[ATT_16_UUID_LEN];
extern const unsigned char serviceGlucoseUuid[ATT_16_UUID_LEN];
extern const unsigned char serviceHealthThermometerUuid[ATT_16_UUID_LEN];
extern const unsigned char serviceDeviceInformationUuid[ATT_16_UUID_LEN];
extern const unsigned char serviceHeartRateUuid[ATT_16_UUID_LEN];
extern const unsigned char servicePhoneAlertStatusUuid[ATT_16_UUID_LEN];
extern const unsigned char serviceBatteryUuid[ATT_16_UUID_LEN];
extern const unsigned char serviceBloodPressureUuid[ATT_16_UUID_LEN];
extern const unsigned char serviceAlertNotificationUuid[ATT_16_UUID_LEN];
extern const unsigned char serviceHumanInterfaceDeviceUuid[ATT_16_UUID_LEN];
extern const unsigned char serviceScanParametersUuid[ATT_16_UUID_LEN];
extern const unsigned char serviceRunningSpeedAndCadenceUuid[ATT_16_UUID_LEN];
extern const unsigned char serviceAutomationIoUuid[ATT_16_UUID_LEN];
extern const unsigned char serviceCyclingSpeedAndCadenceUuid[ATT_16_UUID_LEN];
extern const unsigned char serviceCyclingPowerUuid[ATT_16_UUID_LEN];
extern const unsigned char serviceLocationAndNavigationUuid[ATT_16_UUID_LEN];
extern const unsigned char serviceEnvironmentalSensingUuid[ATT_16_UUID_LEN];
extern const unsigned char serviceBodyCompositionUuid[ATT_16_UUID_LEN];
extern const unsigned char serviceUserDataUuid[ATT_16_UUID_LEN];
extern const unsigned char serviceWeightScaleUuid[ATT_16_UUID_LEN];
extern const unsigned char serviceBondManagementUuid[ATT_16_UUID_LEN];
extern const unsigned char serviceContinuousGlucoseMonitoringUuid[ATT_16_UUID_LEN];
extern const unsigned char serviceInternetProtocolSupportUuid[ATT_16_UUID_LEN];
extern const unsigned char serviceIndoorPositioningUuid[ATT_16_UUID_LEN];
extern const unsigned char servicePulseOximeterUuid[ATT_16_UUID_LEN];
extern const unsigned char serviceHttpProxyUuid[ATT_16_UUID_LEN];
extern const unsigned char serviceTransportDiscoveryUuid[ATT_16_UUID_LEN];
extern const unsigned char serviceObjectTransferUuid[ATT_16_UUID_LEN];
extern const unsigned char serviceFitnessMachineUuid[ATT_16_UUID_LEN];
extern const unsigned char serviceMeshProvisioningUuid[ATT_16_UUID_LEN];
extern const unsigned char serviceMeshProxyUuid[ATT_16_UUID_LEN];
extern const unsigned char serviceReconnectionConfigurationUuid[ATT_16_UUID_LEN];
extern const unsigned char serviceInsulinDeliveryUuid[ATT_16_UUID_LEN];
extern const unsigned char serviceBinarySensorUuid[ATT_16_UUID_LEN];
extern const unsigned char serviceEmergencyConfigurationUuid[ATT_16_UUID_LEN];
extern const unsigned char serviceAuthorizationControlUuid[ATT_16_UUID_LEN];
extern const unsigned char servicePhysicalActivityMonitorUuid[ATT_16_UUID_LEN];
extern const unsigned char serviceElapsedTimeUuid[ATT_16_UUID_LEN];
extern const unsigned char serviceGenericHealthSensorUuid[ATT_16_UUID_LEN];
extern const unsigned char serviceAudioInputControlUuid[ATT_16_UUID_LEN];
extern const unsigned char serviceVolumeControlUuid[ATT_16_UUID_LEN];
extern const unsigned char serviceVolumeOffsetControlUuid[ATT_16_UUID_LEN];
extern const unsigned char serviceCoordinatedSetIdentificationUuid[ATT_16_UUID_LEN];
extern const unsigned char serviceDeviceTimeUuid[ATT_16_UUID_LEN];
extern const unsigned char serviceMediaControlUuid[ATT_16_UUID_LEN];
extern const unsigned char serviceGenericMediaControlUuid[ATT_16_UUID_LEN];
extern const unsigned char serviceConstantToneExtensionUuid[ATT_16_UUID_LEN];
extern const unsigned char serviceTelephoneBearerUuid[ATT_16_UUID_LEN];
extern const unsigned char serviceGenericTelephoneBearerUuid[ATT_16_UUID_LEN];
extern const unsigned char serviceMicrophoneControlUuid[ATT_16_UUID_LEN];
extern const unsigned char serviceAudioStreamControlUuid[ATT_16_UUID_LEN];
extern const unsigned char serviceBroadcastAudioScanUuid[ATT_16_UUID_LEN];
extern const unsigned char servicePublishedAudioCapabilitiesUuid[ATT_16_UUID_LEN];
extern const unsigned char serviceBasicAudioAnnouncementUuid[ATT_16_UUID_LEN];
extern const unsigned char serviceBroadcastAudioAnnouncementUuid[ATT_16_UUID_LEN];
extern const unsigned char serviceCommonAudioUuid[ATT_16_UUID_LEN];
extern const unsigned char serviceHearingAccessUuid[ATT_16_UUID_LEN];
extern const unsigned char serviceTelephonyAndMediaAudioUuid[ATT_16_UUID_LEN];
extern const unsigned char servicePublicBroadcastAnnouncementUuid[ATT_16_UUID_LEN];
extern const unsigned char serviceElectronicShelfLabelUuid[ATT_16_UUID_LEN];
extern const unsigned char serviceGamingAudioUuid[ATT_16_UUID_LEN];
extern const unsigned char serviceMeshProxySolicitationUuid[ATT_16_UUID_LEN];
//TODO: Ranging profile has not been confirmed yet. by junhui.hu
extern const unsigned char serviceRangingUuid[ATT_16_UUID_LEN];
//TODO: Ultra Low Latency HID service has not been confirmed yet. by qihang.mou
extern const unsigned char serviceHidIsoUuid[ATT_16_UUID_LEN];

extern const unsigned char declarationsPrimaryServiceUuid[ATT_16_UUID_LEN];
extern const unsigned char declarationsSecondaryServiceUuid[ATT_16_UUID_LEN];
extern const unsigned char declarationsIncludeUuid[ATT_16_UUID_LEN];
extern const unsigned char declarationsCharacteristicUuid[ATT_16_UUID_LEN];
extern const unsigned char descriptorCharacteristicExtendedPropertiesUuid[ATT_16_UUID_LEN];
extern const unsigned char descriptorCharacteristicUserDescriptionUuid[ATT_16_UUID_LEN];
extern const unsigned char descriptorClientCharacteristicConfigurationUuid[ATT_16_UUID_LEN];
extern const unsigned char descriptorServerCharacteristicConfigurationUuid[ATT_16_UUID_LEN];
extern const unsigned char descriptorCharacteristicPresentationFormatUuid[ATT_16_UUID_LEN];
extern const unsigned char descriptorCharacteristicAggregateFormatUuid[ATT_16_UUID_LEN];
extern const unsigned char descriptorValidRangeUuid[ATT_16_UUID_LEN];
extern const unsigned char descriptorExternalReportReferenceUuid[ATT_16_UUID_LEN];
extern const unsigned char descriptorReportReferenceUuid[ATT_16_UUID_LEN];
extern const unsigned char descriptorNumberOfDigitalsUuid[ATT_16_UUID_LEN];
extern const unsigned char descriptorValueTriggerSettingUuid[ATT_16_UUID_LEN];
extern const unsigned char descriptorEnvironmentalSensingConfigurationUuid[ATT_16_UUID_LEN];
extern const unsigned char descriptorEnvironmentalSensingMeasurementUuid[ATT_16_UUID_LEN];
extern const unsigned char descriptorEnvironmentalSensingTriggerSettingUuid[ATT_16_UUID_LEN];
extern const unsigned char descriptorTimeTriggerSettingUuid[ATT_16_UUID_LEN];
extern const unsigned char descriptorCompleteBrEdrTransportBlockDataUuid[ATT_16_UUID_LEN];
extern const unsigned char descriptorObservationScheduleUuid[ATT_16_UUID_LEN];
extern const unsigned char descriptorValidRangeAndAccuracyUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicDeviceNameUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicAppearanceUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicPeripheralPrivacyFlagUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicReconnectionAddressUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicPeripheralPreferredConnParamUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicServiceChangedUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicAlertLevelUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicTxPowerLevelUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicDateTimeUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicDayofWeekUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicDayDateTimeUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicExactTime256Uuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicDstOffsetUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicTimeZoneUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicLocalTimeInformationUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicTimeWithDstUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicTimeAccuracyUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicTimeSourceUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicReferenceTimeInformationUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicTimeUpdateControlPointUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicTimeUpdateStateUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicGlucoseMeasurementUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicBatteryLevelUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicBatteryPowerStateUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicTemperatureMeasurementUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicTemperatureTypeUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicIntermediateTemperatureUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicMeasurementIntervalUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicBootKeyboardInputReportUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicSystemIdUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicModelNumberStringUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicSerialNumberStringUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicFirmwareRevisionStringUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicHardwareRevisionStringUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicSoftwareRevisionStringUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicManufacturerNameStringUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicIEEE_11073_20601DataListUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicCurrentTimeUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicMagneticDeclinationUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicScanRefreshUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicBootKeyboardOutputReportUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicBootMouseInputReportUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicGlucoseMeasurementContextUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicBloodPressureMeasurementUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicIntermediateCuffPressureUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicHeartRateMeasurementUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicBodySensorLocationUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicHeartRateControlPointUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicAlertStatusUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicRingerControlPointUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicRingerSettingUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicAlertCategoryIdBitMaskUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicAlertCategoryIdUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicAlertNotificationControlPointUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicUnreadAlertStatusUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicNewAlertUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicSupportedNewAlertCategoryUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicSupportedUnreadAlertCategoryUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicBloodPressureFeatureUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicHidInformationUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicReportMapUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicHidControlPointUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicReportUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicProtocolModeUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicScanIntervalWindowUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicPnpIdUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicGlucoseFeatureUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicRecordAccessControlPointUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicRscMeasurementUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicRscFeatureUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicScControlPointUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicAggregateUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicCscMeasurementUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicCscFeatureUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicSensorLocationUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicPlxSpotCheckMeasurementUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicPlxContinuousMeasurementUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicPlxFeaturesUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicCyclingPowerMeasurementUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicCyclingPowerVectorUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicCyclingPowerFeatureUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicCyclingPowerControlPointUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicLocationAndSpeedUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicNavigationUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicPositionQualityUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicLnFeatureUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicLnControlPointUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicElevationUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicPressureUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicTemperatureUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicHumidityUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicTrueWindSpeedUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicTrueWindDirectionUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicApparentWindSpeedUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicApparentWindDirectionUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicGustFactorUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicPollenConcUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicUvIndexUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicIrradianceUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicRainfallUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicWindChillUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicHeatIndexUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicDewPointUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicDescriptorValueChangedUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicAerobicHeartRateLowerLimitUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicAerobicThresholdUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicAgeUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicAnaerobicHeartRateLowerLimitUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicAnaerobicHeartRateUpperLimitUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicAnaerobicThresholdUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicAerobicHeartRateUpperLimitUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicDateOfBirthUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicDateOfThresholdAssessmentUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicEmailAddressUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicFatBurnHeartRateLowerLimitUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicFatBurnHeartRateUpperLimitUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicFirstNameUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicFiveZoneHeartRateLimitsUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicGenderUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicHeartRateMaxUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicHeightUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicHipCircumferenceUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicLastNameUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicMaximumRecommendedHeartRateUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicRestingHeartRateUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicSportTypeUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicThreeZoneHeartRateLimitsUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicTwoZoneHeartRateLimitsUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicVo2MaxUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicWaistCircumferenceUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicWeightUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicDatabaseChangeIncrementUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicUserIndexUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicBodyCompositionFeatureUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicBodyCompositionMeasurementUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicWeightMeasurementUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicWeightScaleFeatureUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicUserControlPointUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicMagneticFluxDensity2dUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicMagneticFluxDensity3dUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicLanguageUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicBarometricPressureTrendUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicBondManagementControlPointUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicBondManagementFeatureUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicCentralAddressResolutionUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicCgmMeasurementUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicCgmFeatureUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicCgmStatusUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicCgmSessionStartTimeUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicCgmSessionRunTimeUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicCgmSpecificOpsControlPointUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicIndoorPositioningConfigurationUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicLatitudeUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicLongitudeUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicLocalNorthCoordinateUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicLocalEastCoordinateUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicFloorNumberUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicAltitudeUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicUncertaintyUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicLocationNameUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicUriUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicHttpHeadersUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicHttpStatusCodeUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicHttpEntityBodyUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicHttpControlPointUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicHttpsSecurityUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicTdsControlPointUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicOtsFeatureUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicObjectNameUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicObjectTypeUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicObjectSizeUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicObjectFirstCreatedUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicObjectLastModifiedUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicObjectIdUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicObjectPropertiesUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicObjectActionControlPointUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicObjectListControlPointUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicObjectListFilterUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicObjectChangedUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicResolvablePrivateAddressOnlyUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicFitnessMachineFeatureUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicTreadmillDataUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicCrossTrainerDataUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicStepClimberDataUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicStairClimberDataUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicRowerDataUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicIndoorBikeDataUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicTrainingStatusUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicSupportedSpeedRangeUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicSupportedInclinationRangeUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicSupportedResistanceLevelRangeUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicSupportedHeartRateRangeUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicSupportedPowerRangeUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicFitnessMachineControlPointUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicFitnessMachineStatusUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicMeshProvisioningDataInUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicMeshProvisioningDataOutUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicMeshProxyDataInUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicMeshProxyDataOutUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicAverageCurrentUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicAverageVoltageUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicBooleanUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicChromaticDistFromPlanckianUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicChromaticityCoordinatesUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicChromaticityinCctDuvValuesUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicChromaticityToleranceUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicCie1331995ColorIndexUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicCoefficientUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicCorrelatedColorTemperatureUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicCount16Uuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicCount24Uuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicCountryCodeUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicDateUtcUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicElectricCurrentUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicElectricCurrentRangeUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicElectricCurrentSpecificationUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicElectricCurrentStatisticsUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicEnergyUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicEnergyInAPeriodOfDayUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicEventStatisticsUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicFixedString16Uuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicFixedString24Uuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicFixedString36Uuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicFixedString8Uuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicGenericLevelUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicGlobalTradeItemNumberUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicIlluminanceUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicLuminousEfficacyUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicLuminousEnergyUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicLuminousExposureUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicLuminousFluxUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicLuminousFluxRangeUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicLuminousIntensityUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicMassFlowUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicPerceivedLightnessUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicPercentage8Uuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicPowerUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicPowerSpecificationUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicRelativeRuntimeCurrentRangeUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicRelativeRuntimeGenericLevelUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicRelativeValueVoltageRangeUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicRelativeValueIlluminanceUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicRelativeValuePeriodOfDayUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicRelativeValueTemperatureRangeUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicTemperature8Uuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicTemperature8InAPeriodOfDayUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicTemperature8StatisticsUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicTemperatureRangeUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicTemperatureStatisticsUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicTimeDecihour8Uuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicTimeExponential8Uuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicTimeHour24Uuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicTimeMillisecond24Uuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicTimeSecond16Uuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicTimeSecond8Uuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicVoltageUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicVoltageSpecificationUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicVoltageStatisticsUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicVolumeFlowUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicChromaticityCoordinateUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicRcFeatureUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicRcSettingsUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicReconnCfgControlPointUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicIddStatusChangedUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicIddStatusUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicIddAnnunciationStatusUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicIddFeaturesUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicIddStatusReaderControlPointUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicIddCommandControlPointUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicIddCommandDataUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicIddRecordAccessControlPointUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicIddHistoryDataUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicClientSupportedFeaturesUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicDatabaseHashUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicBssControlPointUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicBssResponseUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicEmergencyIdUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicEmergencyTextUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicAcsStatusUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicAcsDataInUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicAcsDataOutNotifyUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicAcsDataOutIndicateUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicAcsControlPointUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicEnhancedBpMeasurementUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicEnhancedIntermediateCuffPressureUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicBloodPressureRecordUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicRegisteredUserUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicBrEdrHandoverDataUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicBluetoothSigDataUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicServerSupportedFeaturesUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicPhysicalAmFeaturesUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicGeneralActivityInstDataUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicGeneralActivitySummaryDataUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicCrActivityInstantaneousDataUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicCrActivitySummaryDataUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicScActivitySummaryDataUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicSleepActivityInstDataUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicSleepActivitySummaryDataUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicPhysicalAmControlPointUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicActivityCurrentSessionUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicPhysicalAsDescriptorUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicPreferredUnitsUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicHighResolutionHeightUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicMiddleNameUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicStrideLengthUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicHandednessUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicDeviceWearingPositionUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicFourZoneHeartRateLimitsUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicHighIntensityExerciseThUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicActivityGoalUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicSedentaryIntervalNotificationUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicCaloricIntakeUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicTmapRoleUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicAudioInputStateUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicGainSettingsAttributeUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicAudioInputTypeUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicAudioInputStatusUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicAudioInputControlPointUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicAudioInputDescriptionUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicVolumeStateUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicVolumeControlPointUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicVolumeFlagsUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicVolumeOffsetStateUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicAudioLocationUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicVolumeOffsetControlPointUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicAudioOutputDescriptionUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicSetIdentityResolvingKeyUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicCoordinatedSetSizeUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicSetMemberLockUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicSetMemberRankUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicEncryptedDataKeyMaterialUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicApparentEnergy32Uuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicApparentPowerUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicLiveHealthObservationsUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicCoConcUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicCosineOfTheAngleUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicDeviceTimeFeatureUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicDeviceTimeParametersUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicDeviceTimeUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicDeviceTimeControlPointUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicTimeChangeLogDataUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicMediaPlayerNameUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicMediaPlayerIconObjectIdUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicMediaPlayerIconUrlUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicTrackChangedUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicTrackTitleUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicTrackDurationUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicTrackPositionUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicPlaybackSpeedUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicSeekingSpeedUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicCurrentTrackSegmentsObjectIdUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicCurrentTrackObjectIdUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicNextTrackObjectIdUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicParentGroupObjectIdUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicCurrentGroupObjectIdUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicPlayingOrderUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicPlayingOrdersSupportedUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicMediaStateUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicMediaControlPointUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicMediaCtrlPointOpSupportedUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicSearchResultsObjectIdUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicSearchControlPointUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicEnergy32Uuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicMediaPlayerIconObjectTypeUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicTrackSegmentsObjectTypeUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicTrackObjectTypeUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicGroupObjectTypeUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicConstantToneExtensionEnableUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicAdvertisingCteMinLengthUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicAdvertisingCteMinTransmitCntUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicAdvertisingCteTranDurationUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicAdvertisingCteIntervalUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicAdvertisingCtePhyUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicBearerProviderNameUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicBearerUciUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicBearerTechnologyUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicBearerUriSchemesSuppListUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicBearerSsUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicBearerSsReportingIntervalUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicBearerListCurrentCallsUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicContentControlIdUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicStatusFlagsUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicIncomingCallTargetBearerUriUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicCallStateUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicCallCtrlPointUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicCallCtrlPointOptionalOpcodesUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicTerminationReasonUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicIncomingCallUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicCallFriendlyNameUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicMuteUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicSinkAseUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicSourceAseUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicAseControlPointUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicBasControlPointUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicBroadcastReceiveStateUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicSinkPacUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicSinkAudioLocationsUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicSourcePacUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicSourceAudioLocationsUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicAvailableAudioContextsUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicSupportedAudioContextsUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicAmmoniaConcUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicCarbonMonoxideConcUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicMethaneConcUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicNitrogenDioxideConcUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicNonMethaneVolatileOcsConcUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicOzoneConcUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicParticulateMatterPm1ConcUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicParticulateMatterPm25ConcUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicParticulateMatterPm10ConcUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicSulfurDioxideConcUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicSulfurHexafluorideConcUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicHearingAidFeaturesUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicHearingAidPresetControlPointUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicActivePresetIndexUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicStoredHealthObservationsUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicFixedString64Uuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicHighTemperatureUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicHighVoltageUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicLightDistributionUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicLightOutputUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicLightSourceTypeUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicNoiseUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicRelativeRuntime2Uuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicTimeSecond32Uuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicVocConcUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicVoltageFrequencyUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicBatteryCriticalStatusUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicBatteryHealthStatusUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicBatteryHealthInformationUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicBatteryInformationUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicBatteryLevelStatusUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicBatteryTimeStatusUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicEstimatedServiceDateUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicBatteryEnergyStatusUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicObservationScheduleChangedUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicCurrentElapsedTimeUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicHealthSensorFeaturesUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicGhsControlPointUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicLeGattSecurityLevelsUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicEslAddressUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicApSyncKeyMaterialUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicEslResponseKeyMaterialUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicEslCurrentAbsoluteTimeUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicEslDisplayInformationUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicEslImageInformationUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicEslSensorInformationUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicEslLedInformationUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicEslControlPointUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicUdiForMedicalDevicesUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicGmapRoleUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicUggFeaturesUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicUgtFeaturesUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicBgsFeaturesUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicBgrFeaturesUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicRasFeatureUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicRealTimeProcedureDataUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicOnDemandProcedureDataUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicControlPointUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicRangingDataReadyUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicRangingDataOverwrittenUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicGoogleSrsReadCapUuid[ATT_128_UUID_LEN];
extern const unsigned char characteristicGoogleSrsSendCmdUuid[ATT_128_UUID_LEN];
//TODO: Ultra Low Latency HID service has not been confirmed yet. by qihang.mou
extern const unsigned char characteristicHidIsoPropertiesUuid[ATT_16_UUID_LEN];
extern const unsigned char characteristicLeHidOperationModeUuid[ATT_16_UUID_LEN];


/***********************for zigbee direct start***************************/
#define SERVICE_UUID_COMMISSIONING                  0xFFF7

#define SERVICE_BASE_UUID                           0xFB,0x34,0x9B,0x5F,0x80,0x00,0x00,0x80,0x00,0x10,0x00,0x00,0x00,0x00,0x00,0x00

#define CHARACTERISTIC_UUID_FORM_NETWORK            0x61,0x3A,0x33,0x27,0x1C,0x49,0x63,0xB1,0x1C,0x42,0x01,0x00,0x7D,0x37,0x72,0x70
#define CHARACTERISTIC_UUID_JOIN_NETWORK            0x61,0x3A,0x33,0x27,0x1C,0x49,0x63,0xB1,0x1C,0x42,0x02,0x00,0x7D,0x37,0x72,0x70
#define CHARACTERISTIC_UUID_PERMIT_JOINING          0x61,0x3A,0x33,0x27,0x1C,0x49,0x63,0xB1,0x1C,0x42,0x03,0x00,0x7D,0x37,0x72,0x70
#define CHARACTERISTIC_UUID_LEAVE_NETWORK           0x61,0x3A,0x33,0x27,0x1C,0x49,0x63,0xB1,0x1C,0x42,0x04,0x00,0x7D,0x37,0x72,0x70
#define CHARACTERISTIC_UUID_COMMISSIONING_STATUS    0x61,0x3A,0x33,0x27,0x1C,0x49,0x63,0xB1,0x1C,0x42,0x05,0x00,0x7D,0x37,0x72,0x70
#define CHARACTERISTIC_UUID_MANAGE_JOINERS          0x61,0x3A,0x33,0x27,0x1C,0x49,0x63,0xB1,0x1C,0x42,0x06,0x00,0x7D,0x37,0x72,0x70
#define CHARACTERISTIC_UUID_IDENTIFY                0x61,0x3A,0x33,0x27,0x1C,0x49,0x63,0xB1,0x1C,0x42,0x07,0x00,0x7D,0x37,0x72,0x70
#define CHARACTERISTIC_UUID_FINDING_BINDING         0x61,0x3A,0x33,0x27,0x1C,0x49,0x63,0xB1,0x1C,0x42,0x08,0x00,0x7D,0x37,0x72,0x70


#define ZBD_SEC_SERVICE_UUID_SERVICE                0xE3,0x29,0xB4,0x99,0x02,0x6D,0xE9,0xBF,0x81,0x44,0x00,0x00,0xF4,0x4A,0x14,0x29

/*ZVD should support the first and third methods, and the ZDD can support at least one(the first one now).*/
#define CHARACTERISTIC_UUID_ZBD_SS_SPEKE_AES128     0xE3,0x29,0xB4,0x99,0x02,0x6D,0xE9,0xBF,0x81,0x44,0x01,0x00,0xF4,0x4A,0x14,0x29
#define CHARACTERISTIC_UUID_ZBD_SS_SPEKE_SHA256     0xE3,0x29,0xB4,0x99,0x02,0x6D,0xE9,0xBF,0x81,0x44,0x02,0x00,0xF4,0x4A,0x14,0x29
#define CHARACTERISTIC_UUID_ZBD_SS_ECDHE_SHA256     0xE3,0x29,0xB4,0x99,0x02,0x6D,0xE9,0xBF,0x81,0x44,0x03,0x00,0xF4,0x4A,0x14,0x29

#define ZBD_TUNNEL_UUID_SERVICE                     0x3F,0x31,0xD5,0x8B,0x37,0xB2,0x20,0x81,0xF4,0x45,0x00,0x00,0xFD,0x78,0xD1,0x8B
#define CHARACTERISTIC_UUID_ZDTS_NPDU               0x3F,0x31,0xD5,0x8B,0x37,0xB2,0x20,0x81,0xF4,0x45,0x01,0x00,0xFD,0x78,0xD1,0x8B
/***********************for zigbee direct end***************************/
