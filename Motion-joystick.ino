// For Arduino Leonardo boards

#include "Joystick.h"
#include "Wire.h"
#include "I2Cdev.h"
#include "MPU6050_6Axis_MotionApps20.h"

MPU6050 mpu;
bool dmpReady=false;  // set true if DMP init was successful
uint8_t mpuIntStatus;   // holds actual interrupt status byte from MPU
uint8_t devStatus;      // return status after each device operation (0 = success, !0 = error)
uint16_t packetSize;    // expected DMP packet size (default is 42 bytes)
uint16_t fifoCount;     // count of all bytes currently in FIFO
uint8_t fifoBuffer[64]; // FIFO storage buffer
Quaternion q;           // [w, x, y, z]         quaternion container
VectorInt16 aa;         // [x, y, z]            accel sensor measurements
VectorInt16 aaReal;     // [x, y, z]            gravity-free accel sensor measurements
VectorInt16 aaWorld;    // [x, y, z]            world-frame accel sensor measurements
VectorFloat gravity;    // [x, y, z]            gravity vector
float euler[3];         // [psi, theta, phi]    Euler angle container
float ypr[3];           // [yaw, pitch, roll]   yaw/pitch/roll container and gravity vector
volatile bool mpuInterrupt=false;     // indicates whether MPU interrupt pin has gone high

bool ftime=true,butpressed=false,filtering=false,firstfilter=true;
float y,p,r,ly,lp,lr,sy,sp,sr,alpha=1;
int ls=1;
int gmode=1; // 0=Full YPR  1=Screen YPR
long stimer;

void dmpDataReady()
{
  mpuInterrupt=true;
}

void setup()
{
  Wire.begin();
  Serial.begin(115200);
  Joystick.begin(false);
  pinMode(7,INPUT);
  pinMode(10,INPUT_PULLUP);
  mpu.initialize();
  devStatus=mpu.dmpInitialize();
  mpu.setXGyroOffset(0);
  mpu.setYGyroOffset(0);
  mpu.setZGyroOffset(0);
  mpu.setZAccelOffset(697);
  if(devStatus==0)
  {
    mpu.setDMPEnabled(true);
    attachInterrupt(digitalPinToInterrupt(7),dmpDataReady,RISING);
    mpuIntStatus=mpu.getIntStatus();
    dmpReady=true;
    packetSize=mpu.dmpGetFIFOPacketSize();
  }
}

void loop()
{
  int xa,ya,za,st;
  if(!dmpReady) return;
  while(!mpuInterrupt && fifoCount<packetSize){}
  mpuInterrupt=false;
  mpuIntStatus=mpu.getIntStatus();
  fifoCount=mpu.getFIFOCount();
  if((mpuIntStatus&0x10)||fifoCount==1024)
  {
    mpu.resetFIFO();
  }
  else if(mpuIntStatus&0x02)
  {
    while(fifoCount<packetSize) fifoCount=mpu.getFIFOCount();
    mpu.getFIFOBytes(fifoBuffer,packetSize);
    fifoCount-=packetSize;
    mpu.dmpGetQuaternion(&q,fifoBuffer);
    mpu.dmpGetGravity(&gravity,&q);
    mpu.dmpGetYawPitchRoll(ypr,&q,&gravity);
    y=ypr[0];
    p=ypr[1];
    r=ypr[2];
    dofiltering();
    st=digitalRead(10);
    if(st==LOW)
    {
      if(ls==HIGH)
      {
        stimer=millis();
        if(butpressed==false)
        {
          Joystick.setButton(0,1);
          butpressed=true;
        }
      }
    }
    else
    {
      if(ls==LOW)
      {
        if((millis()-stimer)>3000) setcentre();
        if(butpressed==true)
        {
          Joystick.setButton(0,0);
          butpressed=false;
        }

      }
    }
    ls=st;
    if(ftime==true)
    {
      setcentre();
      ftime=false;
    }
    else
    {
      xa=aminus(y,sy)*1000;
      ya=aminus(p,sp)*1000;
      za=aminus(r,sr)*1000;
      switch(gmode)
      {
        case 0: xa=map(constrain(xa,-3142,3142),-3142,3142,-127,127);
                ya=map(constrain(ya,-3142,3142),-3142,3142,127,-127);
                za=map(constrain(za,-3142,3142),-3142,3142,-127,127);
                break;
        case 1: xa=map(constrain(xa,-400,400),-400,400,-127,127);
                ya=map(constrain(ya,-400,400),-400,400,127,-127);
                za=map(constrain(za,-400,400),-400,400,-127,127);
                break;
      }
      Joystick.setXAxis(xa);
      Joystick.setYAxis(ya);
      Joystick.setZAxis(za);
      Joystick.sendState();
    } 
  }
  if(Serial.available()>0)
  {
    char b=Serial.read();
    switch(b)
    {
      case 'v': Serial.println("dhhtracker"); break;
      case 'r': setcentre(); break;
      case 'a': gmode=0; break;
      case 'b': gmode=1; break;
      case '0': filtering=false; break;
      case '1': alpha=0.300; filtering=true; firstfilter=true; break;
      case '2': alpha=0.267; filtering=true; firstfilter=true; break;
      case '3': alpha=0.233; filtering=true; firstfilter=true; break;
      case '4': alpha=0.200; filtering=true; firstfilter=true; break;
      case '5': alpha=0.167; filtering=true; firstfilter=true; break;
      case '6': alpha=0.133; filtering=true; firstfilter=true; break;
      case '7': alpha=0.100; filtering=true; firstfilter=true; break;
      case '8': alpha=0.067; filtering=true; firstfilter=true; break;
      case '9': alpha=0.033; filtering=true; firstfilter=true; break;
      case 'z': sy=0; sp=0; sr=0; break;
    }
  }
}

void dofiltering()
{
  if(firstfilter==true)
  {
    ly=y; lp=p; lr=r;
    firstfilter=false;
  }
  else
  {
    y=(alpha*y)+((1-alpha)*ly);
    p=(alpha*p)+((1-alpha)*lp);
    r=(alpha*r)+((1-alpha)*lr);
    ly=y; lp=p; lr=r;
  }
}

float aminus(float a,float b)
{
  float res;
  res=(a+10000)-(b+10000);
  return res;
}

void setcentre()
{
  sy=ypr[0];
  sp=ypr[1];
  sr=ypr[2];
}


