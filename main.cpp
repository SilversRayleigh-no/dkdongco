/*
 * date: 21/07/2022
 * from: Lê Ngọc Hải
 * a hiểu cái gì a comment hết lại rồi
*/

#include <ESP8266WiFi.h>  // để kết nối wifi của ESP8266
#include <ESP8266WebServer.h>  // để chạy web server

/* define các chân để điều khiển, nó hiểu theo GPIOx, x là default
 * nếu chỉ có 1 chân cho 1 động cơ thì k thể đảo chiều động cơ - vì sao chắc ae hiểu bà nó rồi
 * do đó cần 2 chân cho việc rẽ, 1 lùi 1 tiến nhanh hơn là 1 đứng im 1 tiến
*/
// động cơ phải và phải, 1 trong 2 thằng cần là chân analog để phát xung PWM, nếu k thì phải có 1 chân ENx để băm xung. Chân ENx là chân cấp nguồn nhỉ ?
// #define ENA something
// #define ENB something
#define IN1_RIGHT something
#define IN2_RIGHT something
#define IN3_LEFT  something
#define IN4_LEFT  something

// name and password wifi
const char* nameWifi = "coffeeteam";
const char* password = "coffeeteam";

// create 1 thằng web serve
ESP8266WebServer server; // port default của nó creat trong constuctor là 80 sẵn rồi

// trước tiên khi 1 sketch chạy thì nó chạy thằng setup trước, coi như là bước khởi tạo
void setup () {
    // set in out cho các chân
    // pinMode(ENA, OUT);
    // pinMode(ENB, OUT);
    pinMode(IN1_RIGHT, OUT);
    pinMode(IN2_RIGHT, OUT);
    pinMode(IN3_LEFT, OUT);
    pinMode(IN4_LEFT, OUT);

    // Serial là 1 element trong Adruino IDE, để communication data từ bo mạch với pc qua usb
    Serial.begin(115200); // start communication và 115200 chính là tốc độ truyền cho cái việc communication data đấy
    createAccessPointAsWifi();
}

// soft-AP: là access point nhưng méo thể connect có dây với mạng, max cho 5 thằng connect
void createAccessPointAsWifi () {
    Serial.print("start createAccessPointAsWifi\n");
    // set mode : thằng esp32 nó có thể là trạm, có thể là access point, hoặc cả 2, ở đây mình set nó là access point
    WiFi.mode(WIFI_AP); // WIFI_STA - trạm - connect to accesspoint, WIFI_AP - accesspoint - cho trạm connect tới, WIFI_STA_AP - là cả 2 connect to access point
    WiFi.softAP(nameWifi, password);  // create softAP
    Serial.print("ip address of softAP: %s\n", WiFi.softAPIP().toString.c_str());


    // start web-serve, dùng lambda đi
    server.on("/", [](){
        server.send( 200, "text/html", "" );
    }); // kiểu như trang chủ ấy, sau / gì cả, ví dụ http://github.com thì ok, nhưng http://github.com/doan thì oẳng vì k chỉ định

    server.onNotFound([](){
        server.send(404, "text/html", "404: Not found");
    }); // khi uri k được chỉ định với server.on thì chạy vào thằng này thôi
    server.begin(); // thực sự bắt đầu start server ở đây
}

//void HTTP_handleRoot () {
//  server.send( 200, "text/html", "" ); // send http status 200 - ok
//}
//void HTTP_handleNotFound () {
//  server.send(404, "text/html", "404: Not found"); // send http status 400 - not found
//}

// sau đó chạy thằng loop, như thăng main trong cpp
void loop () {
    server.handleClient(); // listen http request từ client

    // nhận giá trị từ app mobile
    robotMoving();
    delay(1000); // 1s
}

/* xem mình đấu chân âm dương vào module như thế nào, U = 5v là max thì 0 < HIGH - x  < 5 thì nó sẽ chậm hơn
nếu mà LOW -x < 0 thì đaỏ mẹ chiều */

void up (int speed = 255){
    // cùng tiến
    digitalWrite(IN1_RIGHT, HIGH);
    analogWrite(IN2_RIGHT, 255 - speed); // if is chân pwm

    digitalWrite(IN3_LEFT, HIGH);
    analogWrite(IN4_LEFT, 255 - speed); // if is chân pwm
}

void down (int speed = 255){
    // cùng lùi
    digitalWrite(IN1_RIGHT, LOW);
    analogWrite(IN2_RIGHT, speed); // if is chân pwm

    digitalWrite(IN3_LEFT, LOW);
    analogWrite(IN4_LEFT, speed); // if is chân pwm
}

void turnleft(int speed = 255){
    // trái lùi phải tiến
    digitalWrite(IN1_RIGHT, HIGH);
    analogWrite(IN2_RIGHT, 255-speed); // if is chân pwm

    digitalWrite(IN3_LEFT, LOW);
    analogWrite(IN4_LEFT, speed); // if is chân pwm
}

void turnright(int speed = 255){
    // trái tiến phải lùi
    digitalWrite(IN1_RIGHT, LOW);
    analogWrite(IN2_RIGHT, speed); // if is chân pwm

    digitalWrite(IN3_LEFT, HIGH);
    analogWrite(IN4_LEFT, 255-speed); // if is chân pwm
}

void robotMoving(){

}
