#include "mbed.h"
#include "bbcar.h"
#include "mbed_rpc.h"

Ticker servo_ticker;
PwmOut pin5(D5), pin6(D6);
BufferedSerial xbee(D1, D0);
BBCar car(pin5, pin6, servo_ticker);

void MOVEControl(Arguments *in, Reply *out);
RPCFunction rpcMOVE(&MOVEControl, "MOVE");
int x, y, z, k;

int main() {
   char buf[256], outbuf[256];
   FILE *devin = fdopen(&xbee, "r");
   FILE *devout = fdopen(&xbee, "w");
   while (1) {
      memset(buf, 0, 256);
      for( int i = 0; ; i++ ) {
         char recv = fgetc(devin);
         if(recv == '\n') {
            printf("\r\n");
            break;
         }
         buf[i] = fputc(recv, devout);
      }
   RPC::call(buf, outbuf);
   printf("%s\r\n", outbuf);
   }
}

void MOVEControl (Arguments *in, Reply *out)   {
    bool success = true;

    // In this scenario, when using RPC delimit the two arguments with a space.
    x = in->getArg<int>();
    y = in->getArg<int>();
    z = in->getArg<int>();
    //k = in->getArg<double>();
   
    // Have code here to call another RPC function to wake up specific led or close it.
    //char buffer[200], outbuf[256];
    //char strings[20];
    //int d1 = 10*x+y;
    //int d2 = 10*z+k;
    //printf("%d", z);

    if(z==1){//west
      car.goStraight(-100); // the lenth of BBcar
      ThisThread::sleep_for(450ms);
      car.stop();
      
      int i=0;
      
      while(i<x){
         i++;
         car.goStraight(-100);
         ThisThread::sleep_for(72ms);
         car.stop();
      }
      ThisThread::sleep_for(1000ms);
      
      car.turn(-100,-0.001);
      ThisThread::sleep_for(1100ms);
      car.stop();
      
      i=0;

      while(i<y){
         i++;
         car.goStraight(-100);
         ThisThread::sleep_for(72ms);
         car.stop();
      }
      car.goStraight(-100); // the lenth of BBcar
      ThisThread::sleep_for(300ms);
      car.stop();

    }else if(z==2){//south
      car.goStraight(-100); // the lenth of BBcar
      ThisThread::sleep_for(0ms);
      car.stop();
      
      int i=0;
      while(i<y){
         i++;
         car.goStraight(-100);
         ThisThread::sleep_for(72ms);
         car.stop();
      }
    }else if(z==3){//east
      car.goStraight(-100); // the lenth of BBcar
      ThisThread::sleep_for(430ms);
      car.stop();
      
      int i=0;
      
      while(i<x){
         i++;
         car.goStraight(-100);
         ThisThread::sleep_for(72ms);
         car.stop();
      }
      ThisThread::sleep_for(1000ms);
      
      car.turn(-100,0.001);
      ThisThread::sleep_for(1000ms);
      car.stop();
      
      i=0;

      while(i<y){
         i++;
         car.goStraight(-100);
         ThisThread::sleep_for(72ms);
         car.stop();
      }
      car.goStraight(-100); // the lenth of BBcar
      ThisThread::sleep_for(400ms);
      car.stop();
    }else{
      car.goStraight(-100);
      ThisThread::sleep_for(72ms);
    }
    
}
