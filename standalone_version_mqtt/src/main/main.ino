//this project was released as esphome_im350 / standalone_version to read serial data stream from im350 smartmeter
//the decryption is done by the esp and decrypted data is output via USB-UART and via TelnetStream to 
//used this script as base for further coding
//changes 22.03.2023 by ingmarsretro
//ip address of ESP is to static ip ->  take a look in settings.h
//included PubSubclient.h for mqtt - services
//output ESP RSSI & counter P_in, P_out, Q_in, Q_out, actual power in and out as MQTT topic

#include <Arduino.h>
#include <ArduinoOTA.h>
#include <TelnetStream.h>
#include <Crypto.h>
#include <AES.h>
#include <GCM.h> // Crypto, Rhys Weatherley
#include <WiFi.h>
#include <PubSubClient.h> // PubSubClient
#include "time.h"
// #include <WiFiNINA.h> // WiFiNINA
//#include <ArduinoMqttClient.h> // AruinoMqttClient


#include "secrets.h"
#include "settings.h"
#include "crypto_lib.h"
// #include "meter_power.h"
// #include "parsePower.h"

// MQTT settings
// WiFiClient wifiClient;
// MqttClient mqttClient(wifiClient);
WiFiClient espClient;
PubSubClient client(espClient);

// NTP Settings
struct tm ntpTime;
uint8_t current_time_day;
uint8_t current_time_month;
uint16_t current_time_year;

// message date variables
uint16_t message_year;
uint8_t message_month;
uint8_t message_day;
uint8_t message_hour;
uint8_t message_minute;
uint8_t message_second;

// reading variables
uint32_t counter_reading_p_in;
uint32_t counter_reading_p_out;
uint32_t counter_reading_q_in;
uint32_t counter_reading_q_out;
uint32_t current_power_usage_in;
uint32_t current_power_usage_out;


char RSSIstr[8];

// Vector_GCM Vector_SM;

boolean getLocalTime()
{
  if(!getLocalTime(&ntpTime)){
    Serial.println("Failed to obtain time");
    return false;
  }
  else {
    current_time_day = ntpTime.tm_mday;
    current_time_month = ntpTime.tm_mon + 1; // Month is 0 - 11, add 1 to get a jan-dec 1-12 concept
    current_time_year = ntpTime.tm_year + 1900; // Year is # years since 1900
    return true;
  }
}

void printLocalTime() {
  if (getLocalTime()) {
    Serial.println(&ntpTime, "%A, %B %d %Y %H:%M:%S");  
    TelnetStream.println(&ntpTime, "%A, %B %d %Y %H:%M:%S");  
  }
}

// void print_byte_in_hex(char X) {

//    if (X < 16) {Serial.print("0");}

//    Serial.print(X, HEX);

// }


// void print_array(byte array[], unsigned int len)
// {
//   char text_buffer[len];

//   for (unsigned int i = 0; i < len; i++)
//   {
//       byte nib1 = (array[i] >> 4) & 0x0F;
//       byte nib2 = (array[i] >> 0) & 0x0F;
//       text_buffer[i*2+0] = nib1  < 0xA ? '0' + nib1  : 'A' + nib1  - 0xA;
//       text_buffer[i*2+1] = nib2  < 0xA ? '0' + nib2  : 'A' + nib2  - 0xA;
//   }
//   text_buffer[len*2] = '\0';
//   for (unsigned int i = 0; i < len; i++) {
//     Serial.print(text_buffer[i]);
//     TelnetStream.print(text_buffer[i]);
//   }
// }

// uint32_t byteToUInt32(byte array[], unsigned int startByte)
// {
//     // https://stackoverflow.com/questions/12240299/convert-bytes-to-int-uint-in-c
//     // convert 4 bytes to uint32
//     uint32_t result;
//     result = (uint32_t) array[startByte] << 24;
//     result |=  (uint32_t) array[startByte+1] << 16;
//     result |= (uint32_t) array[startByte+2] << 8;
//     result |= (uint32_t) array[startByte+3];
//
//     return result;
// }


// void parse_timestamp(byte array[]) {
//   message_year = (array[6] << 8) + (array[7]);
//   message_month = array[8];
//   message_day = array[9];
//   message_hour = array[11];
//   message_minute = array[12];
//   message_second = array[13];
// }


// Why the hell we do that?
// used to see if encrypted data are correct...
// bool validate_message_date() {
//   Serial.println();
//   // compare date from ntp and message should be the same day!
//   Serial.println();
//   TelnetStream.println();
//   Serial.println("======DEBUG=======");
//   TelnetStream.println("======DEBUG=======");
//   Serial.printf("DATE FROM NTP: %02d-%02d-%02d", current_time_year, current_time_month, current_time_day);
//   TelnetStream.printf("DATE FROM NTP: %02d-%02d-%02d", current_time_year, current_time_month, current_time_day);
//   Serial.println();
//   TelnetStream.println();
//   Serial.printf("DATE FROM MESSAGE: %02d-%02d-%02d", message_year, message_month, message_day);
//   TelnetStream.printf("DATE FROM MESSAGE: %02d-%02d-%02d", message_year, message_month, message_day);
//   Serial.println();
//   TelnetStream.println();

//   if (current_time_year == message_year and current_time_month == message_month and current_time_day == message_day){
//     Serial.printf("Message Date is VALID!, ntp_date: %02d-%02d-%02d == message_date: %02d-%02d-%02d\n", current_time_year, current_time_month, current_time_day, message_year, message_month, message_day);
//     TelnetStream.printf("Message Date is VALID!, ntp_date: %02d-%02d-%02d == message_date: %02d-%02d-%02d\n", current_time_year, current_time_month, current_time_day, message_year, message_month, message_day);
//     Serial.println("======DEBUG=======");
//     TelnetStream.println("======DEBUG=======");
//     Serial.println();
//     TelnetStream.println();
//     return true;
//   }
//   else {
//     Serial.printf("Message Date is INVALID!, ntp_date: %02d-%02d-%02d !=  message_date: %02d-%02d-%02d\n", current_time_year, current_time_month, current_time_day, message_year, message_month, message_day);
//     TelnetStream.printf("Message Date is INVALID!, ntp_date: %02d-%02d-%02d !=  message_date: %02d-%02d-%02d\n", current_time_year, current_time_month, current_time_day, message_year, message_month, message_day);
//     Serial.println("======DEBUG=======");
//     TelnetStream.println("======DEBUG=======");
//     // Serial.println();
//     //TelnetStream.println();
//     return false;
//   }
// }

int readSerial(int data_request_gpio) {

    unsigned long time_start_reading;
    unsigned long time_read_attempt;
    unsigned long UART_TIMEOUT = 10000;


    Serial.println("Try to read data from serial port.");
    TelnetStream.println("Try to read data from serial port.");
    Serial.println("Set Request Pin");
    TelnetStream.println("Set Request Pin");
    // set request pin to get data

    digitalWrite(data_request_gpio, HIGH);
    time_start_reading = millis();

    while (true) {
        time_read_attempt = millis();
        if ((time_read_attempt - time_start_reading) > UART_TIMEOUT) {
            Serial.println("Timeout UART, no data received");
            TelnetStream.println("Timeout UART, no data received");
            return false;
        }
        if (Serial2.available()) {
            Serial.println("Serial2 is available");
            TelnetStream.println("Serial2 is available");

            // turn on led to show data is available
            int return_byte;
            memset(buffer, 0, sizeof(buffer));
            int count = 0;

            while (Serial2.available())
            {
                // DO SOMETHING WITH message
                //Serial2.readBytes(message, message_length);
                // int len = Serial2.readBytesUntil('\n', message, message_length);
                return_byte = Serial2.read();

                if (return_byte < 16) {
                    Serial.print("0");
                    TelnetStream.print("0");
                }
                Serial.print(return_byte, HEX);
                TelnetStream.print(return_byte, HEX);
                buffer[count] = return_byte;
                count++;
            }

            Serial.println();
            digitalWrite(data_request_gpio, LOW);
        }

    }

    Serial.println("Done with reading from from serial port.");
    TelnetStream.println("Done with reading from from serial port.");

    return true;
}




// void printBytesToHex(byte array[], unsigned int len) {
//   for (unsigned int i = 0; i < len; i++) {
//     Serial.print(message[i], HEX);
//     TelnetStream.print(message[i], HEX);
//   }
//   Serial.print("\n");
//   TelnetStream.print("\n");
// }


void setup() {

    // disable bluetooth for security and power saving
    btStop();

    // Init serial
    Serial.begin(115200);
    Serial2.begin(115200, SERIAL_8N1, uart2_rx_gpio, uart2_tx_gpio);

    // Init pins
    pinMode(DATA_REQUEST_SM1_GPIO, OUTPUT);
    pinMode(DATA_REQUEST_SM2_GPIO, OUTPUT);
    pinMode(DEBUG_LED_WIFI_GPIO, OUTPUT);
    pinMode(DEBUG_LED_SM1_GPIO, OUTPUT);
    pinMode(DEBUG_LED_SM2_GPIO, OUTPUT);

    // test all LEDs
    digitalWrite(DEBUG_LED_WIFI_GPIO, HIGH);
    digitalWrite(DEBUG_LED_SM1_GPIO, HIGH);
    digitalWrite(DEBUG_LED_SM2_GPIO, HIGH);
    delay(1000);
    digitalWrite(DEBUG_LED_WIFI_GPIO, LOW);
    digitalWrite(DEBUG_LED_SM1_GPIO, LOW);
    digitalWrite(DEBUG_LED_SM2_GPIO, LOW);
    
    //connect to WiFi
    // Configures static IP address
       // if (!WiFi.config(local_IP, gateway, subnet, primaryDNS)) {
      //Serial.println("STA Failed to configure");
      //}


    // Connect to WiFi
    Serial.printf("Connecting to %s ", WIFI_SSID);
    WiFi.mode(WIFI_STA);   //station mode: the ESP32 connects to an access point
    // WiFi.setTxPower(WIFI_POWER_5dBm);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    digitalWrite(DEBUG_LED_WIFI_GPIO, HIGH);
    delay(2000);
    

    while (WiFi.waitForConnectResult() != WL_CONNECTED) {
      Serial.println("Connection Failed! Rebooting...");
      digitalWrite(DEBUG_LED_WIFI_GPIO, LOW);
      delay(200);
      digitalWrite(DEBUG_LED_WIFI_GPIO, HIGH);
      delay(5000);
      ESP.restart();
    }


    // init and get the time
    configTime(gmtOffset_sec, daylightOffset_sec, ntp_server);
    getLocalTime();
    printLocalTime();


    // mqtt client
    client.setServer(MQTT_SERVER, MQTT_SERVER_PORT); // does not boot with 5.3V from the smart meter
    //client.setCallback(callback_function);  // function for receiving messages


    // Arduino Over The Air flashing
    ArduinoOTA.setPort(3232); // Port defaults to 3232
    ArduinoOTA.setHostname(ARDUINO_OTA_HOSTNAME); // Hostname defaults to esp3232-[MAC]
    ArduinoOTA.setPassword(ARDUINO_OTA_PASS); // No authentication by default, 
    // Password can be set with it's md5 value as well
    // MD5(admin) = 21232f297a57a5a743894a0e4a801fc3
    // ArduinoOTA.setPasswordHash("21232f297a57a5a743894a0e4a801fc3");
    
    ArduinoOTA
        .onStart([]() {
                String type;
                if (ArduinoOTA.getCommand() == U_FLASH)
                type = "sketch";
                else // U_SPIFFS
                type = "filesystem";

                // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
                Serial.println("Start updating " + type);
                })
    .onEnd([]() {
            Serial.println("\nEnd");
            })
    .onProgress([](unsigned int progress, unsigned int total) {
            Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
            })
    .onError([](ota_error_t error) {
            Serial.printf("Error[%u]: ", error);
            if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
            else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
            else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
            else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
            else if (error == OTA_END_ERROR) Serial.println("End Failed");
            });

    // init ArduinoOTA
    ArduinoOTA.begin();

    // Telnet Stream for debugging without USB
    TelnetStream.begin();

    Serial.println("Wifi ready.");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
    TelnetStream.println("Wifi ready.");
    TelnetStream.print("IP address: ");
    TelnetStream.println(WiFi.localIP());


}

void reconnect() {
    digitalWrite(DEBUG_LED_WIFI_GPIO, LOW);
    // test if WiFi is connected
    if (WiFi.waitForConnectResult() != WL_CONNECTED) {
        Serial.println("Connection Failed! Rebooting...");
        TelnetStream.println("Connection Failed! Rebooting...");
        delay(5000);
        ESP.restart();
    }

    // try to reconnect mqtt
    while (!client.connected()) {
        Serial.print("Attempting MQTT connection...");
        TelnetStream.print("Attempting MQTT connection...");
        // Attempt to connect
        if (client.connect(MQTT_CLIENT_NAME, MQTT_USER, MQTT_PASS)) {
            Serial.println("MQTT connected.");
            TelnetStream.println("MQTT connected.");
            client.subscribe("homeassistant/esp32/output");
        } else {
            Serial.print("failed, rc=");
            Serial.print(client.state());
            Serial.println(", try again in 5 seconds");
            TelnetStream.print("failed, rc=");
            TelnetStream.print(client.state());
            TelnetStream.println(", try again in 5 seconds");
            // Wait 5 seconds before retrying
            delay(5000);
        }
    }
}


void loop() {

    // check WiFI/MQTT connection
    if (!client.connected()) {
       reconnect();
    }

    // for updates
    ArduinoOTA.handle();

    //init and get the time
    configTime(gmtOffset_sec, daylightOffset_sec, ntp_server);
    getLocalTime();
    printLocalTime();

    // Send RSSI of MQTT Server, so ESP is up
    dtostrf(WiFi.RSSI(), 1, 2, RSSIstr);
    client.publish("homeassistant/esp", RSSIstr);
    Serial.printf("RSSI: %d dBm\n", WiFi.RSSI());
    TelnetStream.printf("RSSI: %d dBm\n", WiFi.RSSI());
    Serial.println(WiFi.BSSIDstr());
    TelnetStream.println(WiFi.BSSIDstr());


    // read data
    // readSerial(DATA_REQUEST_SM1_GPIO);
    int ct_length = decryptBuffer(ENCRYPTION_KEY_SM1, AUTH_KEY_SM1);
    for (int i = 0; i < ct_length; i++) {
        Serial.write(plaintext[i]);
        
    }
    Serial.println();




    // parsePower3();
    //     readBuffer (pin_numer)
    //     decrypt (key, auth_key)
    //     parse message (structure)
    //     send mqtt (topics)
    //
    // request_pin
    // encryption_key
    // auth_key
    // parse_structure
    // 
    // readPower
    //
    //
    //
    // readBuffer(DATA_REQUEST_SM2_GPIO);
    // parsePower1();
    //
    // readBuffer
    //     set pin
    //     read serial
    //
    // parsePower



      // always init in 
      // init_vector(&Vector_SM,"Vector_SM",sm_decryption_key); 
      // decrypt_text(&Vector_SM);
      //



    digitalWrite(DEBUG_LED_WIFI_GPIO, LOW);
    delay(1000);
    digitalWrite(DEBUG_LED_WIFI_GPIO, HIGH);

    Serial.println("waiting 1 second...");
    TelnetStream.println("waiting 1 second...");

    delay(READOUT_INTERVALL);


}
