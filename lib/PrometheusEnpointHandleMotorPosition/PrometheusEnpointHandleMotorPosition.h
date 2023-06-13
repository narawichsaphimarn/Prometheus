#ifndef PrometheusEnpointHandleMotorPosition_h
#define PrometheusEnpointHandleMotorPosition_h
#include <Arduino.h>

class PrometheusEnpointHandleMotorPosition
{
private:
    char *host;
    char *path;
    bool active;
public:
    PrometheusEnpointHandleMotorPosition(char *host, char *path, bool active);

    void setHost(char *s);

    char *getHost();

    void setPath(char *s);

    char *getPath();

    void setActive(bool s);

    bool getActive();
};

#endif
