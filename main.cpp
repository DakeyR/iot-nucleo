#include "mbed.h"
#include "DHT.h"
#include "ultrasonic.h"
#include "TSL2591.h"

//------------------------------------
// Hyperterminal configuration
// 9600 bauds, 8-bit data, no parity
//------------------------------------

Serial sigfox(D8, D2);
Serial pc(SERIAL_TX, SERIAL_RX);

DigitalOut myled(LED1);

DHT sensor(D9, DHT11);

I2C i2c1(I2C_SDA, I2C_SCL);
TSL2591 light(i2c1, TSL2591_ADDR);


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
        light.getALS();
        light.calcLux();

        pc.printf("light.full: %d \t light.ir: %d \t light.visible: %d \t light.lux: %d\n",light.full,light.ir,(int16_t)light.visible,light.lux);
        wait(10);
        mu.checkDistance();     //call checkDistance() as much as possible, as this is where
        int err = sensor.readData();
        if (err == 0) {
            temp = (int) sensor.ReadTemperature(CELCIUS);
            humid = (int) sensor.ReadHumidity();
            sigfox.printf("AT$SS=%02x %02x %04x %02x\n", temp, humid, light.visible, mu.getCurrentDistance());
            pc.printf("AT$SS=%02x %02x %04x %02x\n", temp, humid, light.visible, mu.getCurrentDistance());
            pc.printf("temp: %d, humid: %d \r\n", temp, humid);
            myled = 0;
        }
        else {
            myled = 1;
            pc.printf("Error when reading temperature/humidity: %d\r\n", err);
        }
        

        //wait(60);

    }
}
