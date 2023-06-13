#include <PrometheusMyStepper.h>
#include <Arduino.h>

PrometheusMyStepper::PrometheusMyStepper() {}

PrometheusMyStepper::PrometheusMyStepper(int m, int a, int b, int c, int d)
{
    Serial.printf("Detail : MODE(%d) IN1(%d) IN2(%d) IN3(%d) IN4(%d)\n", mode, in1, in2, in3, in4);
    this->mode = m;
    this->in1 = a;
    this->in2 = b;
    this->in3 = c;
    this->in4 = d;
    pinMode(this->in1, OUTPUT);
    pinMode(this->in2, OUTPUT);
    pinMode(this->in3, OUTPUT);
    pinMode(this->in4, OUTPUT);
}

void PrometheusMyStepper::setupPort()
{
    pinMode(this->in1, OUTPUT);
    pinMode(this->in2, OUTPUT);
    pinMode(this->in3, OUTPUT);
    pinMode(this->in4, OUTPUT);
}

void PrometheusMyStepper::clearPort()
{
    digitalWrite(this->in1, 0);
    digitalWrite(this->in2, 0);
    digitalWrite(this->in3, 0);
    digitalWrite(this->in4, 0);
}

void PrometheusMyStepper::setStep(long s)
{
    this->step = s;
}

long PrometheusMyStepper::getStep()
{
    return this->step;
}

void PrometheusMyStepper::setCurrentPosition(long s)
{
    this->currentPosition = s;
}

long PrometheusMyStepper::getCurrentPosition()
{
    return this->currentPosition;
}

void PrometheusMyStepper::setMoveTo(long s)
{
    this->moveTo = s;
}

long PrometheusMyStepper::getMoveTo()
{
    return this->moveTo;
}

void PrometheusMyStepper::setSpeedMotor(int s)
{
    this->speedMotor = s;
}

int PrometheusMyStepper::getSpeedMotor()
{
    return this->speedMotor;
}

int PrometheusMyStepper::getDistantPosition()
{
    return this->currentPosition - this->moveTo;
}

void PrometheusMyStepper::setName(char *s)
{
    this->name = s;
}

char *PrometheusMyStepper::getName()
{
    return this->name;
}

void PrometheusMyStepper::setPort1(int s)
{
    this->in1 = s;
}

int PrometheusMyStepper::getPort1()
{
    return this->in1;
}

void PrometheusMyStepper::setPort2(int s)
{
    this->in2 = s;
}

int PrometheusMyStepper::getPort2()
{
    return this->in2;
}

void PrometheusMyStepper::setPort3(int s)
{
    this->in3 = s;
}

int PrometheusMyStepper::getPort3()
{
    return this->in3;
}

void PrometheusMyStepper::setPort4(int s)
{
    this->in4 = s;
}

int PrometheusMyStepper::getPort4()
{
    return this->in4;
}

void PrometheusMyStepper::setMode(int s)
{
    this->mode = s;
}

int PrometheusMyStepper::getMode()
{
    return this->mode;
}

void PrometheusMyStepper::setIndex(int s)
{
    this->index = s;
}

int PrometheusMyStepper::getIndex()
{
    return this->index;
}

void PrometheusMyStepper::upCurrentPosition()
{
    this->currentPosition += 1;
}

void PrometheusMyStepper::downCurrentPosition()
{
    this->currentPosition -= 1;
}

void PrometheusMyStepper::setMotorNumber(int s)
{
    this->motorNumber = s;
}

int PrometheusMyStepper::getMotorNumber()
{
    return this->motorNumber;
}
