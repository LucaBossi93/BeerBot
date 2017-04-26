#include <i2cmaster.h>

void setup()
{
  Serial.begin(9600);
  Serial.println("Hello!");
  // Initialise the i2c bus
  i2c_init();
  Serial.println("Return from i2c_init");
  // Enable pullups
  PORTC = (1 << PORTC4) | (1 << PORTC5);
}

void loop()
{
  Serial.println("TEST 1");
  int dev = 0x5A << 1;
  int data_low = 0;
  int data_high = 0;
  int pec = 0;
  Serial.println("TEST 2");
  i2c_start_wait(dev + I2C_WRITE);
  Serial.println("TEST 3");
  i2c_write(0x07);

  Serial.println("TEST 4");
  i2c_rep_start(dev + I2C_READ);
  // Read 1 byte and then send ack
  data_low = i2c_readAck();
  // Read 1 byte and then send ack
  data_high = i2c_readAck();
  pec = i2c_readNak();
  i2c_stop();

  Serial.println("TEST 5");
  //This converts high and low bytes together and processes temperature, MSB is a error bit and is ignored for temps
  double tempFactor = 0.02; // 0.02 degrees per LSB
  double tempData = 0x0000;
  int frac;

  Serial.println("TEST 6");
  // This masks off the error bit of the high byte, then moves it left 8 bits and adds the low byte.
  tempData = (double)(((data_high & 0x007F) << 8) + data_low);
  tempData = (tempData * tempFactor) - 0.01;
  tempData = tempData - 273.15;
  // Print temp in degrees C to serial
  Serial.print((int)tempData);
  Serial.print(".");
  tempData = tempData - (int)tempData;
  frac = tempData * 100;
  Serial.println(frac);
  delay(100);
}

