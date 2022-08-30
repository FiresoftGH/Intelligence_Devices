#include "MPU6050.h"

MPU6050 mpu;
int16_t ax, ay, az;
int16_t gx, gy, gz;
int valx, valy,  valz;

void setup ()
{

  Serial.begin(9600);
  Serial.println("Initialize MPU");
  mpu.initialize();
  Serial.println(mpu.testConnection() ? "Connected" : "Connection failed");

}

void loop ()
{

  mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
  valx = map(ax, -17000, 17000, -90, 90);
  valy = map(ay, -17000, 17000, -90, 90);
  valz = map(az, -17000, 17000, -90, 90);
  Serial.println("axis x, y, z = "+String(valx) + ":"+String(valy) + ":"+String(valz));
  delay(500);

}
