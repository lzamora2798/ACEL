
#include <SPI.h>
#include <Wire.h>

#define MPU 0x68  // Direccion I2C del MPU-6050


double AcX,AcY,AcZ;
int Pitch, Roll;

double valorX,valorY;
void init_MPU(){
  Wire.begin();
  Wire.beginTransmission(MPU);
  Wire.write(0x6B);  // PWR_MGMT_1 register
  Wire.write(0);     // Seteamos a cero (wakes up - Despertamos el MPU-6050)
  Wire.endTransmission(true);
  delay(1000);
}

void FunctionsMPU(){
  Wire.beginTransmission(MPU);
  Wire.write(0x3B);  // Empezamos con el registro 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(MPU,6,true);  // requerimos un total de 6 registers
  AcX=Wire.read()<<8|Wire.read();  // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)     
  AcY=Wire.read()<<8|Wire.read();  // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
  AcZ=Wire.read()<<8|Wire.read();  // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
  valorX = FunctionsPitchRoll(AcX, AcZ, AcY);  
  valorY = FunctionsPitchRoll(AcY, AcX, AcZ); 
}

double FunctionsPitchRoll(double A, double B, double C){
  double DatoA, DatoB, Value;
  DatoA = A;
  DatoB = (B*B) + (C*C);
  DatoB = sqrt(DatoB);
  
  Value = atan2(DatoA, DatoB);
  Value = Value * 180/3.14;
  
  return (int)Value;
}

void setup(){
  Serial.begin(9600);
  init_MPU(); // Inicializamos el MPU6050
}

void loop() {
  // put your main code here, to run repeatedly:
  FunctionsMPU();
  
  Serial.print(valorX);
  Serial.print(" ");
  Serial.println(valorY);
  delay(1000);
}
