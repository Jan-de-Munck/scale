#include <ESPAsyncWebServer.h>
#include <LittleFS.h>
#include <HX711_ADC.h>
#include <EEPROM.h>

#include "config.h"

AsyncWebServer server(80);
int force = 0;
char forceString[4] = "000";

const int HX711_dout = 4; //change to whichever pins (arduino) your using
const int HX711_sck = 5;

HX711_ADC LoadCell(HX711_dout, HX711_sck);

IPAddress local_IP(local_ip);	//from config.h
IPAddress gateway(gateway_ip); 
IPAddress subnet(subnet_ip);

void setup(){
	Serial.begin(115200);

	if (!LittleFS.begin()){
		Serial.println("\nAn Error has occurred while mounting LittleFS");
		return;
	}

	if (!WiFi.config(local_IP, gateway, subnet)) {
		Serial.println("STA Failed to configure");
	}

	WiFi.mode(WIFI_STA);
	WiFi.begin(WIFI_SSID, WIFI_PASSWORD); //SSID and password from config.h

	Serial.print("");
	Serial.print("\nConnecting");
	while (WiFi.status() != WL_CONNECTED) {
		delay(500);
		Serial.print(".");
	}

	Serial.print("\nConnected to: ");
	Serial.println(WIFI_SSID);
	Serial.print("IP address: ");
	Serial.println(WiFi.localIP());

	server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
		request->send(LittleFS, "/index.html");
	});

	server.on("/force", HTTP_GET, [](AsyncWebServerRequest* request){
		request->send_P(200, "text/plain", forceString);
	});

	server.begin();

	// Run the example calibration that comes with HX711_ADC.h to get the calibration in EEPROM
	float calibrationValue;
	const int calVal_eepromAdress = 0;
	EEPROM.begin(512);
	EEPROM.get(calVal_eepromAdress, calibrationValue);

	LoadCell.begin();
	LoadCell.start(2000, true);
	if (LoadCell.getTareTimeoutFlag()) {
		Serial.println("Timeout, check MCU>HX711 wiring and pin designations");
	}
	else {
		LoadCell.setCalFactor(calibrationValue);
		Serial.println("Startup is complete");
	}

	while (!LoadCell.update());
	Serial.print("Calibration value: ");
	Serial.println(LoadCell.getCalFactor());
	Serial.print("HX711 measured conversion time ms: ");
	Serial.println(LoadCell.getConversionTime());
	Serial.print("HX711 measured sampling rate Hz: ");
	Serial.println(LoadCell.getSPS());
	Serial.print("HX711 measured settlingtime ms: ");
	Serial.println(LoadCell.getSettlingTime());
	if (LoadCell.getSPS() < 7) {
		Serial.println("!!Sampling rate is lower than specification, check MCU>HX711 wiring and pin designations");
	}
	else if (LoadCell.getSPS() > 100) {
		Serial.println("!!Sampling rate is higher than specification, check MCU>HX711 wiring and pin designations");
	}

}

void loop(){
	if (LoadCell.update()) {
		force = LoadCell.getData();
		itoa(force, forceString, 10);
	}
}
