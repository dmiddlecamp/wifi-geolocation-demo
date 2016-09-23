
#include "application.h"
#include "WifiLocate.h"

//class WifiLocate {
//    void locate();
//    void setCallback(location_callback callback);
//
//    protected:
//        location_callback myCallback = NULL;
//}

void WifiLocate::init(const char *topicName) {
    if (topicName != NULL) {
        strncpy(this->myTopicName, topicName, sizeof(this->myTopicName));
    }

    //Particle.subscribe("hook-response/" + String(this->myTopicName), this->onHookResponse, MY_DEVICES);
    Particle.subscribe("hook-response/" + String(this->myTopicName), &WifiLocate::onHookResponse, this, MY_DEVICES);
}

void WifiLocate::locate() {
    WifiScan();
}

void WifiLocate::setCallback(location_callback callback) {
    this->myCallback = callback;
}

void WifiLocate::WifiScan() {
    int count = 0;
    String wifiList = "{\"list\":[";

    WiFiAccessPoint aps[20];
    int found = WiFi.scan(aps, 20);
    for (int i=0; i<found; i++) {
        WiFiAccessPoint& ap = aps[i];

        //{ "macAddress": "01:23:45:67:89:ab", "signalStrength": -51 },
        String line = String::format("{\"m\":\"%02X:%02X:%02X:%02X:%02X:%02X\",\"s\":%d}",
            ap.bssid[0], ap.bssid[1], ap.bssid[2], ap.bssid[3], ap.bssid[4], ap.bssid[5],
            ap.rssi
        );

        //Serial.println(line);

        if (count < 5) {
            if (count > 0) {
                wifiList += ",";
            }

            wifiList += line;
            count++;
        }
    }
    wifiList += "]}";

    if (count == 5) {
        // for simplicity right now, lets only do it if we see at least 5 APs.
        Particle.publish(String(this->myTopicName), wifiList, PRIVATE);
    }
}


// data should be "lat,lng,accuracy"
void WifiLocate::onHookResponse(const char *event, const char *data) {
    if (strcmp(data, "{}") == 0) {
        //Serial.println("got a contribute link back");
        return;
    }

    //Serial.println("got location back " + String(data));


    char one[32], two[32], three[32];
    float latitude, longitude, altitude;
    //sscanf(someString, "%f,%f,%f", &latitude, &longitude, &altitude);
    sscanf(data, "%[^,],%[^,],%[^,]", one, two, three);

    latitude = String(one).toFloat();
    longitude = String(two).toFloat();
    altitude = String(three).toFloat();

    if (latitude ==  0) {
        //Serial.println("nevermind");
        return;
    }

    if ( this->myCallback != NULL) {
        this->myCallback(latitude, longitude, altitude);
    }
}

