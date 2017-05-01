#include <SharpIR.h>

#define ir A0
#define ir2 A1
#define model 1080
// ir: the pin where your sensor is attached
// model: an int that determines your sensor:  1080 for GP2Y0A21Y
//                                            20150 for GP2Y0A02Y
//                                            (working distance range according to the datasheets)

SharpIR SharpIRd(ir, model);
SharpIR SharpIRs(ir2, model);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  delay(500);

  unsigned long pepe1 = millis(); // takes the time before the loop on the library begins

  int dis = SharpIRd.distance(); // this returns the distance to the object you're measuring
  int dus = SharpIRs.distance(); // this returns the distance to the object you're measuring

  Serial.print("Mean distance: ");  // returns it to the serial monitor
  Serial.print(dis);
  Serial.print(", ");
  Serial.println(dus);
  unsigned long pepe2 = millis() - pepe1; // the following gives you the time taken to get the measurement
  // Serial.print("Time taken (ms): ");
  // Serial.println(pepe2);

}
