#define UPDATE_FREQUENCY 10000

unsigned int lastPublish;

void setup() {
    Serial.begin(115200);
  
  Particle.subscribe("hook-response/wifi_scan");
  
  //{\"location\":{\"lat\":44.9490002,\"lng\":-93.2920491},\"accuracy\":63.3364459}
  //ook-response/wifi_scan/0","data":"{\"location\":{\"lat\":44.9490002,\"lng\":-93.2920491},\"accuracy\":63.3364459}","ttl":"60","published_at":"2016-09-22T22:05:38.800Z","coreid":"particle-internal"}

}

void loop() {
    unsigned int now = millis();
    if ((now - lastPublish) > UPDATE_FREQUENCY) {
        WifiScan();
        lastPublish = now;
    }
    
    delay(10);
}

void WifiScan() {
    Serial.println("starting scan");
    
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
        
        Serial.println(line);
        
        if (count < 5) {
            if (count > 0) {
                wifiList += ",";
            }
            
            wifiList += line;
            count++;
        }
        
        //{"name":"wifi_scan","data":"{\"list\":[{\"m\":\"70:5A:0F:F8:51:49\",\"s\":-60},{\"m\":\"6C:3B:E5:4E:7A:58\",\"s\":-77},{\"m\":\"52:6D:64:28:A2:C0\",\"s\":-70},{\"m\":\"B4:C7:99:B1:2F:F8\",\"s\":-65},{\"m\":\"B4:C7:99:B1:2F:F0\",\"s\":-66}]}","ttl":"60","published_at":"2016-09-22T21:56:48.130Z","coreid":"300040000a47343232363230"}
        
    }
    wifiList += "]}";
    
    if (count == 5) {
        // for simplicity right now, lets only do it if we see at least 5 APs.
        Particle.publish("wifi_scan", wifiList, PRIVATE);
    }
    
}

// For getting our location back
//
// https://location.services.mozilla.com/v1/geolocate?key=<API_KEY>
// {
//     "wifiAccessPoints": [
//         { "macAddress": "01:23:45:67:89:ab", "signalStrength": -51 }, 
//         { "macAddress": "01:23:45:67:89:cd" }
//     ]
// }


// for sending data to Mozilla
//
// https://location.services.mozilla.com/v2/geosubmit?key=<API_KEY>
// {
//     "items": [{
//         "timestamp": 1405602028568,
//         "wifiAccessPoints": [
//             {
//                 "macAddress": "01:23:45:67:89:ab",
//                 "age": 5000,
//                 "channel": 6,
//                 "frequency": 2437,
//                 "radioType": "802.11n",
//                 "signalToNoiseRatio": 13,
//                 "signalStrength": -77
//             },
//             {
//                 "macAddress": "23:45:67:89:ab:cd"
//             }
//         ]
// }]}



          

// {
//   "wifiAccessPoints": [
//     {
//       "macAddress": "{{list.0.m}}",
//       "signalStrength": "{{list.0.s}}"
//     },
//     {
//       "macAddress": "{{list.1.m}}",
//       "signalStrength": "{{list.1.s}}"
//     },
//     {
//       "macAddress": "{{list.2.m}}",
//       "signalStrength": "{{list.2.s}}"
//     }
//   ]
// }
          



