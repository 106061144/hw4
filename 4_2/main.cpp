#include"mbed.h"
#include "bbcar.h"
#include "mbed_rpc.h"

BufferedSerial pc(USBTX,USBRX); //tx,rx
BufferedSerial uart(D1,D0); //tx,rx
Ticker servo_ticker;
PwmOut pin5(D5), pin6(D6);
BBCar car(pin5, pin6, servo_ticker);
Thread t;
float angle=-40.0;

void car_thread(){
    /*
    //printf("angle=%f", angle);
    if(angle>10.0){
        //car.stop();
        car.turn(50,-0.3);
        ThisThread::sleep_for(100ms);
    }else if(angle<(-10.0)){
        //car.stop();
        car.turn(50,0.3);
        ThisThread::sleep_for(100ms);
    }else {
        //car.stop();
        car.goStraight(50);
        ThisThread::sleep_for(100ms);
    }
    car.stop();
    */
}

int num = 0;
char temp[256];
int main(){
   uart.set_baud(9600);
   EventQueue queue(32 * EVENTS_EVENT_SIZE);
   t.start(callback(&queue, &EventQueue::dispatch_forever));
   Ticker ticker;
   ticker.attach(queue.event(&car_thread), 200ms);
   while(1){
        if(uart.readable()){
            char recv[1];
            uart.read(recv, sizeof(recv));
            temp[num] = recv[0];
            num++;
            
            if(recv[0] == '\n'){
                num=0;
                angle = atof(temp);
                printf("angle:%f\n",angle);
            }
            
        }
   }
}