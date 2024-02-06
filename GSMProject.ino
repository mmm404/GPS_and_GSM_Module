#include <DFRobot_sim808.h>
#include <SoftwareSerial.h>

#define MESSAGE_LENGTH 160
#define PIN_TX 3
#define PIN_RX 2

char message[MESSAGE_LENGTH];
int messageIndex = 0;
char MESSAGE[300];
char lat[12];
char lon[12];
char phone[16] = "+254799340764";

SoftwareSerial mySerial(PIN_TX, PIN_RX);
DFRobot_SIM808 sim808(&mySerial);

void setup()
{
  mySerial.begin(9600);
  Serial.begin(9600);
  mySerial.println("AT+CGNSPWR=1");

  while (!sim808.init())
  {
    Serial.println("Sim808 init error");
    delay(1000);
  }
  delay(3000);
}

void loop()
{
  messageIndex = sim808.isSMSunread();
  if (messageIndex > 0)
  {
    sim808.readSMS(messageIndex, message, MESSAGE_LENGTH, phone);
    Serial.println("Received SMS: ");
    Serial.println(message);

    if (strcmp(message, "location") == 0)
    {
      float la = sim808.GPSdata.lat;
      float lo = sim808.GPSdata.lon;

      dtostrf(la, 6, 2, lat);
      dtostrf(lo, 6, 2, lon);

      sprintf(MESSAGE, "\nhttp://maps.google.com/maps?q=%s,%s\n", lat, lon);

      mySerial.println("AT+CMGF=1");
      mySerial.print("AT+CMGS=\"");
      mySerial.print(phone);
      mySerial.println("\"");
      delay(500);
      mySerial.println(MESSAGE);
      mySerial.write((char)26);
      delay(500);

      Serial.println("Message sent");
    }
  }
  delay(5000); 
}
