#ifndef PrometheusWifiSetup_h
#define PrometheusWifiSetup_h
#include <Arduino.h>

class PrometheusWifiSetup
{
private:
    char *ssid;
    char *password;
    char *hostname;
public:
    PrometheusWifiSetup();

    PrometheusWifiSetup(char* ssid, char* password);

    void setSsid(char *s);

    char *getSsid();

    void setPassword(char *s);

    char *getPassword();

    void setHostname(char *s);

    char *getHostname();
};

#endif
