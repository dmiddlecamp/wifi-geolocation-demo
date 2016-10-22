#include "WifiLocate.h"

#define UPDATE_FREQUENCY 2 * 60000

unsigned int lastUpdate;


WifiLocate locator;

void setup() {
    Serial.begin(115200);

    Serial.println("testing the library!");

    locator.init("wifi_scan");
    locator.setCallback(onLocation);
}

void loop() {
    unsigned int now = millis();
    if ((now - lastUpdate) > UPDATE_FREQUENCY) {

        // lets go!
        locator.locate();

        lastUpdate = now;
    }

    delay(10);
}


void onLocation(float latitude, float longitude, float altitude) {

    Serial.println(String::format("Got location back! %f, %f, %f ", latitude, longitude, altitude));
    Serial.println(String::format("You're on a map! https://www.google.com/maps/place/%f,%f", latitude, longitude));

    String dataJson = String("{")
        + "\"c_lat\":" + String::format("%f", latitude)
        + ",\"c_lng\":" + String::format("%f", longitude)
        + ",\"c_unc\":" + String::format("%f", altitude)
        + "}";

    Particle.publish("trk/loc", dataJson, 60, PRIVATE);
}