#include <PrometheusWifiSetup.h>
#include <Arduino.h>

PrometheusWifiSetup::PrometheusWifiSetup() {}

PrometheusWifiSetup::PrometheusWifiSetup(char* ssid, char* password)
{
    this->ssid = ssid;
    this->password = password;
}

void PrometheusWifiSetup::setSsid(char *s)
{
    this->ssid = s;
}

char *PrometheusWifiSetup::getSsid()
{
    return this->ssid;
}

void PrometheusWifiSetup::setPassword(char *s)
{
    this->password = s;
}

char *PrometheusWifiSetup::getPassword()
{
    return this->password;
}

void PrometheusWifiSetup::setHostname(char *s)
{
    this->hostname = s;
}

char *PrometheusWifiSetup::getHostname()
{
    this->hostname;
}
