// Using UART2 for reading the data from the smart meter, if you use the UART0 you can not upload new code to the board cause its blocked while reading from the meter!
int uart2_rx_gpio = 16;
int uart2_tx_gpio = 17;

// int data_request_gpio = 26;
<<<<<<< HEAD
int DEBUG_LED_WIFI_GPIO = 21;
int DATA_REQUEST_SM1_GPIO = 100;
int DATA_REQUEST_SM2_GPIO = 100;
int DEBUG_LED_SM1_GPIO = 200;
int DEBUG_LED_SM2_GPIO = 200;

char ARDUINO_OTA_HOSTNAME[] = "ESP_Smartmeter";



// IP - Network settings of your ESP
// IPAddress local_IP(10, 0, 0, 123); // Set your Static IP address 
// IPAddress gateway(10, 0, 0, 1);   // Set your Gateway IP address
// IPAddress subnet(255, 255, 255, 0); // Set your Subnet IP address
// IPAddress primaryDNS(10, 0, 0, 1);  
// IPAddress secondaryDNS(8, 8, 4, 4); 

//char mqtt_server[] = "test.mosquitto.org"; //Test mqttbroker
//const char* mqtt_server = "test.mosquitto.org";


// NTP Settings
char ntp_server[] = "pool.ntp.org";
const long gmtOffset_sec = 3600;
const int daylightOffset_sec = 3600;

const byte start_byte = 0x7E; 
const byte stop_byte = 0x7E;

const int max_wait_time_for_reading_data = 1100;
int delay_before_reading_data = 1000;

const int message_length = 123;
byte message[message_length];
byte buffer[90];

bool use_test_data = false;
byte testData[123] = {0x7e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
                      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
                      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
                      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
                      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
                      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
                      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
                      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
                      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
                      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
                      0x00, 0x00, 0x7e};
