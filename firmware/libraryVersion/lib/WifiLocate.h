
#include "application.h"

typedef void (*location_callback)(float latitude, float longitude, float altitude);


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



/**
 *
 * This class performs a WiFiScan, and publishes the results
 * It's counting on a webhook to perform a geolocation, and return the coordinates
 * which we then parse, and pass to your callback.
 *
**/
class WifiLocate {
    public:

        void init(const char *topicName);
        void locate();
        void setCallback(location_callback callback);

    protected:
        // my vars
        char myTopicName[62] = "wifi_scan_lib";

        // my internals
        void WifiScan();
        void onHookResponse(const char *topic, const char *data);

        // my callbacks
        location_callback myCallback = NULL;
};
