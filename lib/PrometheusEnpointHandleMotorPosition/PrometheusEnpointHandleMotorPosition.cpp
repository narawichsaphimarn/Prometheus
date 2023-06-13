#include <PrometheusEnpointHandleMotorPosition.h>
#include <Arduino.h>

PrometheusEnpointHandleMotorPosition::PrometheusEnpointHandleMotorPosition(char *host, char *path, bool active)
{
    this->host = host;
    this->path = path;
    this->active = active;
}

void PrometheusEnpointHandleMotorPosition::setHost(char *s)
{
    this->host = s;
}

char *PrometheusEnpointHandleMotorPosition::getHost()
{
    return this->host;
}

void PrometheusEnpointHandleMotorPosition::setPath(char *s)
{
    this->path = s;
}

char *PrometheusEnpointHandleMotorPosition::getPath()
{
    return this->path;
}

void PrometheusEnpointHandleMotorPosition::setActive(bool s)
{
    this->active = s;
}

bool PrometheusEnpointHandleMotorPosition::getActive()
{
    return this->active;
}
