#ifndef PrometheusMyStepper_h
#define PrometheusMyStepper_h
#include <Arduino.h>

class PrometheusMyStepper
{
private:
    long step;
    long currentPosition;
    long moveTo;
    int mode;
    int speedMotor;
    int in1;
    int in2;
    int in3;
    int in4;
    char *name;
    int index;
    int motorNumber;

public:
    PrometheusMyStepper();

    PrometheusMyStepper(int m, int a, int b, int c, int d);

    void setupPort();

    void clearPort();

    void setStep(long s);

    long getStep();

    void setCurrentPosition(long s);

    long getCurrentPosition();

    void setMoveTo(long s);

    long getMoveTo();

    void setSpeedMotor(int s);

    int getSpeedMotor();

    int getDistantPosition();

    void setName(char *s);

    char *getName();

    void setPort1(int s);

    int getPort1();

    void setPort2(int s);

    int getPort2();

    void setPort3(int s);

    int getPort3();

    void setPort4(int s);

    int getPort4();

    void setMode(int s);

    int getMode();

    void setIndex(int s);

    int getIndex();

    void upCurrentPosition();

    void downCurrentPosition();

    void setMotorNumber(int s);

    int getMotorNumber();
};

#endif // PrometheusMyStepper_h
