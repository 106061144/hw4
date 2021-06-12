#include "mbed.h"
#include "bbcar.h"
#include "mbed_rpc.h"

BufferedSerial pc(USBTX,USBRX); //tx,rx
BufferedSerial uart(D1,D0); //tx,rx
Ticker servo_ticker;
DigitalInOut ping(D11);
Timer t;
PwmOut pin5(D5), pin6(D6);
DigitalInOut pin10(D10);

BBCar car(pin5, pin6, servo_ticker);
Thread T;
Thread ping_t;
float angle=0.0;
float dis=0.0;

int dir = 0;
int Count = 0;
int step=0;
float p1=36.0;
float p2=55.0;

void ping_thread(){
    float val;
    ping.output();
    ping = 0; wait_us(200);
    ping = 1; wait_us(5);
    ping = 0; wait_us(5);

    ping.input();
    while(ping.read() == 0);
    t.start();
    while(ping.read() == 1);
    val = t.read();
    dis = val*17700.4f;
    t.stop();
    t.reset();
}


void car_thread(){
    if(dis<p1){
        car.goStraight(-150);
        ThisThread::sleep_for(100ms);
        car.stop();
        ThisThread::sleep_for(100ms);
        return;
    }
    if(step==1){
        if(dir==1){
            step=0;
            car.turn(100,-0.4);
            ThisThread::sleep_for(180ms);
            return;
        }else if(dir==2){
            step=0;
            car.turn(100,0.4);
            ThisThread::sleep_for(180ms);
            return;
        }
    }else if(step==0){
        if((angle<358.0) && (angle>180.0)){ //need to turn right
            dir=1;
            step=1;
            car.turn(-60,-0.01);
            ThisThread::sleep_for(100ms);
            return;
        }else if((angle>2.0) && (angle<180.0)){ //need to turn left 
            dir=2;
            step=1;
            car.turn(-60, 0.01);
            ThisThread::sleep_for(100ms);
            return;
        }else {
            car.stop();
            ThisThread::sleep_for(200ms);
            return;
        }   
    }                                                
    /*
    if(angle>=0.0 && angle<=0.0){
        Count+=1;
        if(Count==10){
            Count=0;
            if(dir==1){
                car.turn(100,0.01);
                ThisThread::sleep_for(100ms);
            }else if(dir==2){
                car.turn(100,-0.01);
                ThisThread::sleep_for(100ms);
            }
            car.stop();
            ThisThread::sleep_for(50ms);
        }
        return;
    }
    if((angle<358.0) && (angle>180.0)){ //need to turn right
        dir=1;
        car.turn(100,-0.8);
        ThisThread::sleep_for(100ms);
        car.stop();
        ThisThread::sleep_for(100ms);
        return;
    }else if((angle>2.0) && (angle<180.0)){ //need to turn left 
        dir=2;
        car.turn(100,0.8);
        ThisThread::sleep_for(100ms);
        car.stop();
        ThisThread::sleep_for(100ms);
        return;
    }else {
        car.stop();
        ThisThread::sleep_for(200ms);
        return;
    }
    */

    if(dis>p2){
        car.goStraight(50);
        ThisThread::sleep_for(100ms);
        car.stop();
        ThisThread::sleep_for(100ms);
        return;
    }
}

int num = 0;
char temp[128];
int main(){
   uart.set_baud(9600);
   pc.set_baud(9600);
   EventQueue queue(32 * EVENTS_EVENT_SIZE);
   EventQueue event_queue(32 * EVENTS_EVENT_SIZE);
   T.start(callback(&queue, &EventQueue::dispatch_forever));
   ping_t.start(callback(&event_queue, &EventQueue::dispatch_forever));
   Ticker ticker;
   Ticker ping_ticker;
   ticker.attach(queue.event(&car_thread), 1s);
   ping_ticker.attach(event_queue.event(&ping_thread), 200ms);
   while(1){
        if(uart.readable()){
            char recv[1];
            uart.read(recv, sizeof(recv));
            temp[num] = recv[0];
            
            if(recv[0] == 'F'){
                temp[num] = '0';
                angle = atof(temp);
                printf("angle:%f ",angle);
                printf("distance:%f\n",dis);
                printf("step:%d\n",step);
            }
            
            num++;
            if(recv[0] == '\n'){
                num=0;
                memset(temp, 0, 128);
            }
        }
        
   }
}