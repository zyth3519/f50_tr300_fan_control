#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>

WiFiClient wifiClient;

// 替换为你的wifi名称和密码
const char *ssid = "Cudy";            // wifi名称
const char *password = ""; // wifi密码

// 定义输出引脚连接
const int FANF50 = 5;    // esp8266的IO口5,既D1
const int FANTR3000 = 2; // esp8266的IO口2,既D4

const int OFF = HIGH;
const int ON = LOW;

// 温度服务器地址
const String BASEURL = "http://192.168.1.1:8088/api/";
const String F50 = "f50";
const String TR3000 = "tr3000";

// 发送http请求来获取温度
float getTemp(String type)
{
  HTTPClient http;
  http.begin(wifiClient, BASEURL + type);
  int httpCode = http.GET();
  if (httpCode > 0)
  {
    String res = http.getString();
    // try
    // {

    return atof(res.c_str());
    // }
    // catch (const std::exception &e)
    // {
    //   http.end();
    //   return 0.0;
    // }
  }
  http.end();
  return 0.0;
}

void setup()
{
  Serial.begin(9600);
  // 设置引脚为输出模式
  pinMode(FANF50, OUTPUT);
  pinMode(FANTR3000, OUTPUT);

  // 关闭风扇
  digitalWrite(FANF50, OFF);
  digitalWrite(FANTR3000, OFF);

  // 连接wifi
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
}
void loop()
{
  // 获取温度
  float f50Temp = getTemp(F50);
  float tr3000Temp = getTemp(TR3000);
  Serial.print("f50: ");
  Serial.println(f50Temp);
  Serial.print("tr3000: ");
  Serial.println(tr3000Temp);


  // 根据温度控制风扇开关
  if (tr3000Temp >= 60)
  {
    digitalWrite(FANTR3000, ON);
  } else if (tr3000Temp < 50) {
    digitalWrite(FANTR3000, OFF);
  }
  if (f50Temp > 50)
  {
    digitalWrite(FANF50, ON);
  } else if (f50Temp <= 40)
  {
    digitalWrite(FANF50, OFF);
  }
  delay(1000 * 5);
}
