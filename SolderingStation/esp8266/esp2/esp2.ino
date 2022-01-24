#include <ESP8266WiFi.h> 
#include <WiFiClient.h> 
#include <ESP8266WebServer.h> 

const char* ssid = "skyNet"; // Указываем имя существующей точки доступа
const char* password = "Orient78"; // Указываем пароль существующей точки доступа

ESP8266WebServer server(80);

uint8_t LED1pin = 2;
bool LED1status = LOW;

void handleRoot() { // Обработчик запроса клиента по корневому адресу
  // Отправляем клиенту
  server.send(200, "text/plain", "Привет от ESP8266!");
}

void handleNotFound() { // Обрабатываем небезызвестную ошибку 404
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}

void setup(void) {
  Serial.begin(115200);
  pinMode(LED1pin, OUTPUT);

  WiFi.mode(WIFI_STA); // Устанавливаем Wi-Fi модуль в режим клиента (STA)
  WiFi.begin(ssid, password); // Устанавливаем ssid и пароль от сети, подключаемся
  
  while (WiFi.status() != WL_CONNECTED) { // Ожидаем подключения к Wi-Fi
    delay(500);
    Serial.print(".");
  }
  
  // Выводим информацию о подключении
  Serial.println("");
  Serial.print("Подключено к ");
  Serial.println(ssid);
  Serial.print("IP адрес: ");
  Serial.println(WiFi.localIP());

  // Устанавливаем обработчики. Можно сделать двумя способами:
  server.on("/", handle_OnConnect);
  server.on("/led1on", handle_led1on);
  server.on("/led1off", handle_led1off);
  server.onNotFound(handleNotFound); // Вызывается, когда обработчик не назначен

  // Запускаем сервер
  server.begin();
  Serial.println("HTTP-сервер запущен");
}

void loop(void) {
  server.handleClient();
  if(LED1status)
    digitalWrite(LED1pin, HIGH);
  else
    digitalWrite(LED1pin, LOW);
}

void handle_OnConnect() 
{ 
  Serial.print("GPIO7 Status: ");
  if(LED1status)
    Serial.print("ON");
  else
    Serial.print("OFF");

  Serial.println("");
  server.send(200, "text/html", SendHTML(LED1status)); 
}
void handle_led1on() 
{
  LED1status = HIGH;
  Serial.println("GPIO7 Status: ON");
  server.send(200, "text/html", SendHTML(true)); 
}
void handle_led1off() 
{
  LED1status = LOW;
  Serial.println("GPIO7 Status: OFF");
  server.send(200, "text/html", SendHTML(false)); 
}
String SendHTML(uint8_t led1stat)
{
  String ptr = "<!DOCTYPE html> <html lang=\"ru-RU\">\n";
  ptr +="<head><meta charset=\"UTF-8\" name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr +="<title>LED Control</title>\n";
  ptr +="<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
  ptr +="body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;} h3 {color: #444444;margin-bottom: 50px;}\n";
  ptr +=".button {display: block;width: 80px;background-color: #1abc9c;border: none;color: white;padding: 13px 30px;text-decoration: none;font-size: 25px;margin: 0px auto 35px;cursor: pointer;border-radius: 4px;}\n";
  ptr +=".button-on {background-color: #1abc9c;}\n";
  ptr +=".button-on:active {background-color: #16a085;}\n";
  ptr +=".button-off {background-color: #34495e;}\n";
  ptr +=".button-off:active {background-color: #2c3e50;}\n";
  ptr +="p {font-size: 14px;color: #888;margin-bottom: 10px;}\n";
  ptr +="</style>\n";
  ptr +="</head>\n";
  ptr +="<body>\n";
  ptr +="<h1>ESP8266 Web Server</h1>\n";
  ptr +="<h3>Используется в режиме точки доступа</h3>\n";
  
  if(led1stat)
    ptr +="<p>LED1 Status: ON</p><a class=\"button button-off\" href=\"/led1off\">OFF</a>\n";
  else
    ptr +="<p>LED1 Status: OFF</p><a class=\"button button-on\" href=\"/led1on\">ON</a>\n";

  ptr +="</body>\n";
  ptr +="</html>\n";
  return ptr;
}
