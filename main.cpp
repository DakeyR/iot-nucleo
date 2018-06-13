#include "mbed.h"
#include "DHT.h"
#include "ultrasonic.h"

//------------------------------------
// Hyperterminal configuration
// 9600 bauds, 8-bit data, no parity
//------------------------------------

Serial sigfox(D8, D2);
Serial pc(SERIAL_TX, SERIAL_RX);

DigitalOut myled(LED1);

DHT sensor(D9, DHT11);

void dist(int distance)
{
    //put code here to happen when the distance is changed
    pc.printf("Distance changed to %dmm\r\n", distance);
}
 
ultrasonic mu(D12, D11, .1, 1, &dist);    //Set the trigger pin to D12 and the echo pin to D11
                                        //have updates every .1 seconds and a timeout after 1
                                        //second, and call dist when the distance changes
 
int main()
{
    int temp = 0;
    int humid = 0;
    mu.startUpdates();//start mesuring the distance
    while(1) {
        wait(10);
        mu.checkDistance();     //call checkDistance() as much as possible, as this is where
        int err = sensor.readData();
        if (err == 0) {
            temp = (int) sensor.ReadTemperature(CELCIUS);
            humid = (int) sensor.ReadHumidity();
            //sigfox.printf("AT$SS=%02x %02x\n", temp, humid);
            pc.printf("temp: %d, humid: %d (AT$SS=%02x %02x\r)\r\n", temp, humid, temp, humid);
            myled = 0;
        }
        else {
            myled = 1;
            pc.printf("Error when reading temperature/humidity: %d\r\n", err);
        }
        

        //wait(60);

    }
}
