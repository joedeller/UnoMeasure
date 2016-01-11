#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Max72xxPanel.h>
#include <NewPing.h>

int pinCS = 10; // Attach CS to this pin, DIN to MOSI and CLK to SCK (cf http://arduino.cc/en/Reference/SPI )
int numberOfHorizontalDisplays = 2;
int numberOfVerticalDisplays = 1;

Max72xxPanel matrix = Max72xxPanel(pinCS, numberOfHorizontalDisplays, numberOfVerticalDisplays);

#define TRIGGER_PIN  9  // Arduino pin tied to trigger pin on ping sensor.
#define ECHO_PIN     8  // Arduino pin tied to echo pin on ping sensor.
#define MAX_DISTANCE 99 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.

unsigned int pingSpeed = 50; // How frequently are we going to send out a ping (in milliseconds). 50ms would be 20 times a second.
unsigned long pingTimer;     // Holds the next ping time.
unsigned int cm;
unsigned int lastcm;

int wait = 30; // In milliseconds

int spacer = 1;
int width = 5 + spacer; // The font width is 5 pixels

void setup() {

  Serial.begin(9600); // Open serial monitor at 9600 baud to see ping results.
  pingTimer = millis(); // Start now.
  matrix.setIntensity(7); // Use a value between 0 and 15 for brightness

  // Adjust to your own needs
  //  matrix.setPosition(0, 0, 0); // The first display is at <0, 0>
  //  matrix.setPosition(1, 1, 0); // The second display is at <1, 0>
  //  matrix.setPosition(2, 2, 0); // The third display is at <2, 0>
  //  matrix.setPosition(3, 3, 0); // And the last display is at <3, 0>
  //  ...
  //  matrix.setRotation(0, 2);    // The first display is position upside down
  //  matrix.setRotation(3, 2);    // The same hold for the last display
}

void loop() {
  String reading ;
  String lastreading="  ";
  int lastcm =0;
  int dist = 0;
  while (true)
  {   
  sonar.ping_timer(echoCheck);
  dist = (sonar.ping_median(10)/ US_ROUNDTRIP_CM);
  
  if (dist<10)
  {
    reading = "0" + String(dist);
  }
  else
  {
    reading = String(dist);
  }  
  if (reading != lastreading)
  {
    lastreading = reading;
    matrix.fillScreen(LOW);
    int x = 0;
    int y = (matrix.height() - 8) / 2; // center the text vertically
    
        matrix.drawChar(x, y, reading[0], HIGH, LOW, 1);
        matrix.drawChar(x+ 8, y, reading[1], HIGH, LOW, 1);
    
    matrix.write(); // Send bitmap to display
  }
    delay(wait);
  }

}


void echoCheck() { // Timer2 interrupt calls this function every 24uS where you can check the ping status.
  // Don't do anything here!
  
  if (sonar.check_timer()){
    cm = sonar.ping_result / US_ROUNDTRIP_CM;
    

}
// Don't do anything here!
}
