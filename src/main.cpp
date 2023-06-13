#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <ESPmDNS.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>
#include <string>
#include <HTTPClient.h>
#include <PrometheusMyStepper.h>
#include <PrometheusWifiSetup.h>
#include <PrometheusEnpointHandleMotorPosition.h>

#define MOTOR1_IN1 23
#define MOTOR1_IN2 22
#define MOTOR1_IN3 21
#define MOTOR1_IN4 19

#define MOTOR2_IN1 18
#define MOTOR2_IN2 17
#define MOTOR2_IN3 16
#define MOTOR2_IN4 4

#define LED 2

TaskHandle_t Task1;
TaskHandle_t Task2;
TaskHandle_t TaskInformation;

AsyncWebServer server(80);

static int fullStep[4][4] = {
    {HIGH, LOW, LOW, HIGH},
    {HIGH, HIGH, LOW, LOW},
    {LOW, HIGH, HIGH, LOW},
    {LOW, LOW, HIGH, HIGH}};
static int halfStep[8][4] = {
    {HIGH, LOW, LOW, HIGH},
    {HIGH, LOW, LOW, LOW},
    {HIGH, HIGH, LOW, LOW},
    {LOW, HIGH, LOW, LOW},
    {LOW, HIGH, HIGH, LOW},
    {LOW, LOW, HIGH, LOW},
    {LOW, LOW, HIGH, HIGH},
    {LOW, LOW, LOW, HIGH}};

static PrometheusMyStepper stepper1(1, MOTOR1_IN1, MOTOR1_IN2, MOTOR1_IN3, MOTOR1_IN4);
static PrometheusMyStepper stepper2(1, MOTOR2_IN1, MOTOR2_IN2, MOTOR2_IN3, MOTOR2_IN4);
static PrometheusWifiSetup wifiSetup("NS", "-Naras-CPE290821-");
static PrometheusEnpointHandleMotorPosition enpointHandleMotorPosition("http://DEADLY-MOUSE", "/prometheus-esp-information-processor-api/api/v1/information", false);

unsigned long notiPeriod = 500;
unsigned long lastTimeNoti = 0;
unsigned long wifiPeriod = 1000;
unsigned long lastTimeWifi = 0;
unsigned long ledWifiPeriod = 5000;
unsigned long lastTimeLedWifi = 0;

void motor(void *pvParameters);
String mapJsonHandleMotorPosition();
void motorInformation(void *pvParameters);
void handleMotor(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total);
DynamicJsonDocument mapJsonObject(uint8_t *data);
void handleCancleThreadRunning(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total);
void changeHostPathEnpointHandlePosition(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total);
void getMotorValue(AsyncWebServerRequest *request);
void handleSuspends(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total);
void handleResume(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total);
void handleChangeMode(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total);
void handleChangePort(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total);
void handleRoot(AsyncWebServerRequest *request);
void handleNotFound(AsyncWebServerRequest *request);
void handleStatus(AsyncWebServerRequest *request);

typedef struct Data_t
{
  PrometheusMyStepper *stepper;
} GenericData_t;

void motor(void *pvParameters)
{
  GenericData_t *data = (GenericData_t *)pvParameters;
  // PrometheusMyStepper *stepper = data->stepper;
  Serial.printf("Print from thread(%d) in value %d and position(%d)....\n", xPortGetCoreID(), data->stepper->getMotorNumber(), data->stepper->getCurrentPosition());
  for (;;)
  {
    if (data->stepper->getMoveTo() > data->stepper->getCurrentPosition())
    {
      if (data->stepper->getMode() == 1)
      {
        for (int i = 0; i < 4; i++)
        {
          if (data->stepper->getCurrentPosition() != data->stepper->getMoveTo())
          {
            digitalWrite(data->stepper->getPort1(), fullStep[i][0]);
            digitalWrite(data->stepper->getPort2(), fullStep[i][1]);
            digitalWrite(data->stepper->getPort3(), fullStep[i][2]);
            digitalWrite(data->stepper->getPort4(), fullStep[i][3]);
            data->stepper->upCurrentPosition();
            Serial.printf("Detail : CORE(%d) currentPosition(%d) MOTOR(%d) SIZE(%d) B1(%d) B2(%d) B3(%d) B4(%d)\n",
                          xPortGetCoreID(),
                          data->stepper->getCurrentPosition(),
                          data->stepper->getMotorNumber(),
                          data->stepper->getMode() == 1 ? 4 : 8,
                          data->stepper->getMode() == 1 ? fullStep[i][0] : halfStep[i][0],
                          data->stepper->getMode() == 1 ? fullStep[i][1] : halfStep[i][1],
                          data->stepper->getMode() == 1 ? fullStep[i][2] : halfStep[i][2],
                          data->stepper->getMode() == 1 ? fullStep[i][3] : halfStep[i][3]);
            // vTaskDelay(speedMotor / portTICK_PERIOD_MS);
          }
          else
          {
            break;
          }
        }
      }
      else if (data->stepper->getMode() == 2)
      {
        for (int i = 0; i < 8; i++)
        {
          if (data->stepper->getCurrentPosition() != data->stepper->getMoveTo())
          {
            digitalWrite(data->stepper->getPort1(), halfStep[i][0]);
            digitalWrite(data->stepper->getPort2(), halfStep[i][1]);
            digitalWrite(data->stepper->getPort3(), halfStep[i][2]);
            digitalWrite(data->stepper->getPort4(), halfStep[i][3]);
            data->stepper->upCurrentPosition();
            Serial.printf("Detail : CORE(%d) currentPosition(%d) MOTOR(%d) SIZE(%d) B1(%d) B2(%d) B3(%d) B4(%d)\n",
                          xPortGetCoreID(),
                          data->stepper->getCurrentPosition(),
                          data->stepper->getMotorNumber(),
                          data->stepper->getMode() == 1 ? 4 : 8,
                          data->stepper->getMode() == 1 ? fullStep[i][0] : halfStep[i][0],
                          data->stepper->getMode() == 1 ? fullStep[i][1] : halfStep[i][1],
                          data->stepper->getMode() == 1 ? fullStep[i][2] : halfStep[i][2],
                          data->stepper->getMode() == 1 ? fullStep[i][3] : halfStep[i][3]);
            // vTaskDelay(speedMotor / portTICK_PERIOD_MS);
          }
          else
          {
            break;
          }
        }
      }
    }
    else if (data->stepper->getMoveTo() < data->stepper->getCurrentPosition())
    {
      for (int i = data->stepper->getCurrentPosition(); i > data->stepper->getMoveTo(); i--)
      {
        if (data->stepper->getMode() == 1)
        {
          for (int i = 4 - 1; i >= 0; i--)
          {
            if (data->stepper->getCurrentPosition() != data->stepper->getMoveTo())
            {
              digitalWrite(data->stepper->getPort1(), fullStep[i][0]);
              digitalWrite(data->stepper->getPort2(), fullStep[i][1]);
              digitalWrite(data->stepper->getPort3(), fullStep[i][2]);
              digitalWrite(data->stepper->getPort4(), fullStep[i][3]);
              data->stepper->downCurrentPosition();
              Serial.printf("Detail : CORE(%d) currentPosition(%d) MOTOR(%d) SIZE(%d) B1(%d) B2(%d) B3(%d) B4(%d)\n",
                            xPortGetCoreID(),
                            data->stepper->getCurrentPosition(),
                            data->stepper->getMotorNumber(),
                            data->stepper->getMode() == 1 ? 4 : 8,
                            data->stepper->getMode() == 1 ? fullStep[i][0] : halfStep[i][0],
                            data->stepper->getMode() == 1 ? fullStep[i][1] : halfStep[i][1],
                            data->stepper->getMode() == 1 ? fullStep[i][2] : halfStep[i][2],
                            data->stepper->getMode() == 1 ? fullStep[i][3] : halfStep[i][3]);
              // vTaskDelay(speedMotor / portTICK_PERIOD_MS);
            }
            else
            {
              break;
            }
          }
        }
        else if (data->stepper->getMode() == 2)
        {
          for (int i = 8 - 1; i >= 0; i--)
          {
            if (data->stepper->getCurrentPosition() != data->stepper->getMoveTo())
            {
              digitalWrite(data->stepper->getPort1(), halfStep[i][0]);
              digitalWrite(data->stepper->getPort2(), halfStep[i][1]);
              digitalWrite(data->stepper->getPort3(), halfStep[i][2]);
              digitalWrite(data->stepper->getPort4(), halfStep[i][3]);
              data->stepper->downCurrentPosition();
              Serial.printf("Detail : CORE(%d) currentPosition(%d) MOTOR(%d) SIZE(%d) B1(%d) B2(%d) B3(%d) B4(%d)\n",
                            xPortGetCoreID(),
                            data->stepper->getCurrentPosition(),
                            data->stepper->getMotorNumber(),
                            data->stepper->getMode() == 1 ? 4 : 8,
                            data->stepper->getMode() == 1 ? fullStep[i][0] : halfStep[i][0],
                            data->stepper->getMode() == 1 ? fullStep[i][1] : halfStep[i][1],
                            data->stepper->getMode() == 1 ? fullStep[i][2] : halfStep[i][2],
                            data->stepper->getMode() == 1 ? fullStep[i][3] : halfStep[i][3]);
              // vTaskDelay(speedMotor / portTICK_PERIOD_MS);
            }
            else
            {
              break;
            }
          }
        }
      }
    }
    else
    {
      break;
    }
  }
  digitalWrite(data->stepper->getPort1(), LOW);
  digitalWrite(data->stepper->getPort2(), LOW);
  digitalWrite(data->stepper->getPort3(), LOW);
  digitalWrite(data->stepper->getPort4(), LOW);
  switch (data->stepper->getMotorNumber())
  {
  case 1:
    vTaskDelete(Task1);
    break;
  case 2:
    vTaskDelete(Task2);
    break;
  }
}

void setup()
{
  Serial.begin(115200);
  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);

  WiFi.mode(WIFI_STA);
  WiFi.config(INADDR_NONE, INADDR_NONE, INADDR_NONE, INADDR_NONE);
  WiFi.begin(wifiSetup.getSsid(), wifiSetup.getPassword());
  Serial.println("");
  while (WiFi.status() != WL_CONNECTED)
  {
    if (millis() - lastTimeWifi >= wifiPeriod)
    {
      lastTimeWifi = millis();
      Serial.print(".");
    }
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(wifiSetup.getSsid());
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("esp32"))
  {
    Serial.println("MDNS responder started");
  }

  server.on("/", HTTP_GET, handleRoot);
  server.on("/status", HTTP_POST, handleStatus);
  server.on(
      "/handle/motor", HTTP_POST, [](AsyncWebServerRequest *request) {}, NULL, handleMotor);
  server.on(
      "/handle/cancle", HTTP_POST, [](AsyncWebServerRequest *request) {}, NULL, handleCancleThreadRunning);
  server.on(
      "/handle/suspend", HTTP_POST, [](AsyncWebServerRequest *request) {}, NULL, handleSuspends);
  server.on(
      "/handle/resume", HTTP_POST, [](AsyncWebServerRequest *request) {}, NULL, handleResume);
  server.on(
      "/handle/change-mode", HTTP_POST, [](AsyncWebServerRequest *request) {}, NULL, handleChangeMode);
  server.on(
      "/handle/change-port", HTTP_POST, [](AsyncWebServerRequest *request) {}, NULL, handleChangePort);
  server.on(
      "/handle/change-enpoint/position", HTTP_POST, [](AsyncWebServerRequest *request) {}, NULL, changeHostPathEnpointHandlePosition);
  server.on("/handle/inquiry", HTTP_GET, getMotorValue);
  server.onNotFound(handleNotFound);
  server.begin();
  Serial.println("HTTP server started");
}

void loop()
{
  // put your main code here, to run repeatedly:
  if ((millis() - lastTimeLedWifi) >= ledWifiPeriod && WiFi.status() == WL_CONNECTED)
  {
    lastTimeLedWifi = millis();
    digitalWrite(LED, !digitalRead(LED));
  }
}

DynamicJsonDocument mapJsonObject(uint8_t *data)
{
  DynamicJsonDocument doc(2048);
  deserializeJson(doc, String((char *)data));
  return doc;
}

String mapJsonHandleMotorPosition()
{
  // const int capacity = JSON_ARRAY_SIZE(10) + 3 * JSON_OBJECT_SIZE(10);
  DynamicJsonDocument doc(2048);
  JsonArray array = doc.to<JsonArray>();
  JsonObject data1 = doc.createNestedObject();
  data1["current"] = stepper1.getCurrentPosition();
  std::string data1S1 = std::to_string(stepper1.getPort1());
  std::string data1S2 = std::to_string(stepper1.getPort2());
  std::string data1S3 = std::to_string(stepper1.getPort3());
  std::string data1S4 = std::to_string(stepper1.getPort4());
  data1["port"] = data1S1 + "," + data1S2 + "," + data1S3 + "," + data1S4;
  data1["name"] = stepper1.getName();
  data1["to_position"] = stepper1.getMoveTo();
  data1["mode"] = stepper1.getMode();
  data1["motor_id"] = 1;
  data1["ip_address"] = WiFi.localIP();

  JsonObject data2 = doc.createNestedObject();
  data2["current"] = stepper2.getCurrentPosition();
  std::string data2S1 = std::to_string(stepper1.getPort1());
  std::string data2S2 = std::to_string(stepper1.getPort2());
  std::string data2S3 = std::to_string(stepper1.getPort3());
  std::string data2S4 = std::to_string(stepper1.getPort4());
  data2["port"] = data2S1 + "," + data2S2 + "," + data2S3 + "," + data2S4;
  data2["name"] = stepper2.getName();
  data2["to_position"] = stepper2.getMoveTo();
  data2["mode"] = stepper2.getMode();
  data2["motor_id"] = 2;
  data2["ip_address"] = WiFi.localIP();
  array.add(data1);
  array.add(data2);
  String body;
  serializeJson(doc, body);
  return body;
}

void motorInformation(void *pvParameters)
{
  (void *)pvParameters;
  const char *host = enpointHandleMotorPosition.getHost();
  const char *path = enpointHandleMotorPosition.getPath();
  String pathStr = String(path);
  String hostStr = String(host);
  hostStr.concat(pathStr);
  try
  {
    HTTPClient http;
    http.begin(hostStr);
    http.addHeader("Content-Type", "application/json");
    String body = mapJsonHandleMotorPosition();
    Serial.println("body : " + body);
    int httpResponseCode = http.POST(body);
    if (httpResponseCode < 0)
    {
      Serial.print("Error on sending POST: ");
      Serial.println(httpResponseCode);
    }
    http.end();
    // vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
  catch (...)
  {
    Serial.println("Error step noti information");
  }
  vTaskDelete(TaskInformation);
}

void handleMotor(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total)
{
  DynamicJsonDocument root = mapJsonObject(data);
  Serial.printf("Running on thread : %d\n", xPortGetCoreID());
  PrometheusMyStepper *stepper;
  try
  {
    if (!root["positions"].isNull())
    {
      JsonObject jo = root["positions"].as<JsonObject>();
      if (jo["index"].as<int>() == 1)
      {
        Task1 = xTaskGetHandle("MotorI");
        if (Task1 != NULL)
        {
          vTaskDelete(Task1);
        }
        stepper1.setMoveTo(jo["value"].as<long>());
        if (!jo["speed"].isNull())
        {
          stepper1.setSpeedMotor(jo["speed"].as<int>());
        }
        stepper = &stepper1;
        GenericData_t dataT = {stepper};
        xTaskCreatePinnedToCore(motor, stepper1.getName(), 4096, (void *)&dataT, 20, &Task1, 0);
      }
      else if (jo["index"].as<int>() == 2)
      {
        Task2 = xTaskGetHandle("MotorII");
        if (Task2 != NULL)
        {
          vTaskDelete(Task2);
        }
        stepper2.setMoveTo(jo["value"].as<long>());
        if (!jo["speed"].isNull())
        {
          stepper2.setSpeedMotor(jo["speed"].as<int>());
        }
        stepper = &stepper2;
        GenericData_t dataT = {stepper};
        xTaskCreatePinnedToCore(motor, stepper2.getName(), 4096, (void *)&dataT, 20, &Task2, 1);
      }
      Serial.printf("Index : %d Value : %d\n", jo["index"].as<int>(), jo["value"].as<long>());
    }
    request->send(200, "application/json", "{\"status\":\"success\"}");
  }
  catch (...)
  {
    request->send(500, "application/json", "{\"status\":\"error\"}");
  }
}

void handleCancleThreadRunning(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total)
{
  DynamicJsonDocument root = mapJsonObject(data);
  try
  {
    if (!root["motor"].isNull())
    {
      Serial.printf("Motor: %d\n", root["motor"].as<int>());
      if (root["motor"].as<int>() == 0)
      {
        Task1 = xTaskGetHandle("MotorI");
        if (Task1 != NULL)
        {
          vTaskDelete(Task1);
        }
        Task2 = xTaskGetHandle("MotorII");
        if (Task2 != NULL)
        {
          delay(1000);
          vTaskDelete(Task2);
        }
      }
      else if (root["motor"].as<int>() == 1)
      {
        Task1 = xTaskGetHandle("MotorI");
        if (Task1 != NULL)
        {
          vTaskDelete(Task1);
        }
      }
      else if (root["motor"].as<int>() == 2)
      {
        Task2 = xTaskGetHandle("MotorII");
        if (Task2 != NULL)
        {
          vTaskDelete(Task2);
        }
      }
      else
      {
        Serial.println("Motor number is not found.");
      }
    }
    request->send(200, "application/json", "{\"status\":\"success\"}");
  }
  catch (...)
  {
    request->send(500, "application/json", "{\"status\":\"error\"}");
  }
}

void changeHostPathEnpointHandlePosition(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total)
{
  DynamicJsonDocument root = mapJsonObject(data);
  try
  {
    if (!root["host"].isNull())
    {
      const char *host = root["host"];
      char *hostRaw = new char[strlen(host) + 1];
      strcat(hostRaw, host);
      enpointHandleMotorPosition.setHost(hostRaw);
    }
    if (!root["path"].isNull())
    {
      const char *path = root["path"];
      char *pathRaw = new char[strlen(path) + 1];
      strcat(pathRaw, path);
      enpointHandleMotorPosition.setPath(pathRaw);
    }
    if (!root["active"].isNull())
    {
      bool path = root["active"].as<bool>();
      enpointHandleMotorPosition.setActive(path);
    }
    request->send(200, "application/json", "{\"status\":\"success\"}");
  }
  catch (...)
  {
    request->send(500, "application/json", "{\"status\":\"error\"}");
  }
}

void getMotorValue(AsyncWebServerRequest *request)
{
  int paramsNr = request->params();
  const int capacity = JSON_ARRAY_SIZE(2) + 4 * JSON_OBJECT_SIZE(2);
  StaticJsonDocument<capacity> doc;
  JsonObject data = doc.createNestedObject("data");
  PrometheusMyStepper stepper;
  try
  {
    for (int i = 0; i < paramsNr; i++)
    {
      AsyncWebParameter *p = request->getParam(i);
      Serial.print("Param name: ");
      Serial.println(p->name());
      Serial.print("Param value: ");
      Serial.println(p->value());
      Serial.println("------");
      if (p->name().equals("motor"))
      {
        const long motorNumber = p->value().toInt();
        if (motorNumber == 1)
        {
          stepper = stepper1;
        }
        else if (motorNumber == 2)
        {
          stepper = stepper2;
        }
      }
      data["currentPosition"] = stepper.getCurrentPosition();
      data["port1"] = stepper.getPort1();
      data["port2"] = stepper.getPort2();
      data["port3"] = stepper.getPort3();
      data["port4"] = stepper.getPort4();
      data["name"] = stepper.getName();
      data["moveToPosition"] = stepper.getMoveTo();
      data["mode"] = stepper.getMode();
      AsyncResponseStream *response = request->beginResponseStream("application/json");
      serializeJson(doc, *response);
      request->send(response);
    }
  }
  catch (...)
  {
    request->send(500, "application/json", "{\"status\":\"error\"}");
  }
}

void handleSuspends(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total)
{
  DynamicJsonDocument root = mapJsonObject(data);
  try
  {
    if (!root["motor"].isNull())
    {
      Serial.printf("Motor: %d\n", root["motor"].as<int>());
      if (root["motor"].as<int>() == 0)
      {
        Task1 = xTaskGetHandle("MotorI");
        if (Task1 != NULL)
        {
          vTaskSuspend(Task1);
        }
        Task2 = xTaskGetHandle("MotorII");
        if (Task2 != NULL)
        {
          vTaskSuspend(Task2);
        }
      }
      else if (root["motor"].as<int>() == 1)
      {
        Task1 = xTaskGetHandle("MotorI");
        if (Task1 != NULL)
        {
          vTaskSuspend(Task1);
        }
      }
      else if (root["motor"].as<int>() == 2)
      {
        Task2 = xTaskGetHandle("MotorII");
        if (Task2 != NULL)
        {
          vTaskSuspend(Task2);
        }
      }
      else
      {
        Serial.println("Motor number is not found.");
      }
    }
    request->send(200, "application/json", "{\"status\":\"success\"}");
  }
  catch (...)
  {
    request->send(500, "application/json", "{\"status\":\"error\"}");
  }
}

void handleResume(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total)
{
  DynamicJsonDocument root = mapJsonObject(data);
  try
  {
    if (!root["motor"].isNull())
    {
      Serial.printf("Motor: %d\n", root["motor"].as<int>());
      if (root["motor"].as<int>() == 0)
      {
        Task1 = xTaskGetHandle("MotorI");
        if (Task1 != NULL)
        {
          vTaskResume(Task1);
        }
        Task2 = xTaskGetHandle("MotorII");
        if (Task2 != NULL)
        {
          vTaskResume(Task2);
        }
      }
      else if (root["motor"].as<int>() == 1)
      {
        Task1 = xTaskGetHandle("MotorI");
        if (Task1 != NULL)
        {
          vTaskResume(Task1);
        }
      }
      else if (root["motor"].as<int>() == 2)
      {
        Task2 = xTaskGetHandle("MotorII");
        if (Task2 != NULL)
        {
          vTaskResume(Task2);
        }
      }
      else
      {
        Serial.println("Motor number is not found.");
      }
    }
    request->send(200, "application/json", "{\"status\":\"success\"}");
  }
  catch (...)
  {
    request->send(500, "application/json", "{\"status\":\"error\"}");
  }
}

void handleChangeMode(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total)
{
  DynamicJsonDocument root = mapJsonObject(data);
  try
  {
    if (!root["motor"].isNull())
    {
      Serial.printf("Motor: %d\n", root["motor"].as<int>());
      if (root["motor"].as<int>() == 1)
      {
        if (!root["mode"].isNull())
        {
          stepper1.setMode(root["mode"].as<int>());
        }
      }
      else if (root["motor"].as<int>() == 2)
      {
        if (!root["mode"].isNull())
        {
          stepper2.setMode(root["mode"].as<int>());
        }
      }
      else
      {
        Serial.println("Motor number is not found.");
      }
    }
    request->send(200, "application/json", "{\"status\":\"success\"}");
  }
  catch (...)
  {
    request->send(500, "application/json", "{\"status\":\"error\"}");
  }
}

void handleChangePort(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total)
{
  DynamicJsonDocument root = mapJsonObject(data);
  try
  {
    if (!root["motor"].isNull())
    {
      Serial.printf("Motor: %d\n", root["motor"].as<int>());
      if (root["motor"].as<int>() == 1)
      {
        if (!root["port1"].isNull())
        {
          stepper1.setPort1(root["port1"].as<int>());
        }
        if (!root["port2"].isNull())
        {
          stepper1.setPort2(root["port2"].as<int>());
        }
        stepper1.setupPort();
      }
      else if (root["motor"].as<int>() == 2)
      {
        if (!root["port1"].isNull())
        {
          stepper2.setPort1(root["port1"].as<int>());
        }
        if (!root["port2"].isNull())
        {
          stepper2.setPort2(root["port2"].as<int>());
        }
        stepper2.setupPort();
      }
      else
      {
        Serial.println("Motor number is not found.");
      }
    }
    request->send(200, "application/json", "{\"status\":\"success\"}");
  }
  catch (...)
  {
    request->send(500, "application/json", "{\"status\":\"error\"}");
  }
}

void handleRoot(AsyncWebServerRequest *request)
{
  digitalWrite(LED, 1);
  char temp[400];
  int sec = millis() / 1000;
  int min = sec / 60;
  int hr = min / 60;

  snprintf(temp, 400,

           "<html>\
  <head>\
    <meta http-equiv='refresh' content='5'/>\
    <title>ESP32 Demo</title>\
    <style>\
      body { background-color: #cccccc; font-family: Arial, Helvetica, Sans-Serif; Color: #000088; }\
    </style>\
  </head>\
  <body>\
    <h1>Hello from ESP32!</h1>\
    <p>Uptime: %02d:%02d:%02d</p>\
  </body>\
</html>",

           hr, min % 60, sec % 60);
  request->send(200, "text/html", temp);
  digitalWrite(LED, 0);
}

void handleNotFound(AsyncWebServerRequest *request)
{
  digitalWrite(LED, 1);
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += request->url();
  message += "\nMethod: ";
  message += (request->method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += request->args();
  message += "\n";

  for (uint8_t i = 0; i < request->args(); i++)
  {
    message += " " + request->argName(i) + ": " + request->arg(i) + "\n";
  }

  request->send(404, "text/plain", message);
  digitalWrite(LED, 0);
}

void handleStatus(AsyncWebServerRequest *request)
{
  for (int i = 0; i < 3; i++)
  {
    digitalWrite(LED, 1);
    delay(500);
    digitalWrite(LED, 0);
    delay(500);
  }
  request->send(200, "text/plain", "OK");
}