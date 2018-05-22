#include "mbed.h"
#include "DHT.h"

//------------------------------------
// Hyperterminal configuration
// 9600 bauds, 8-bit data, no parity
//------------------------------------

Serial sigfox(D8, D2);
Serial pc(SERIAL_TX, SERIAL_RX);

DigitalOut myled(LED1);

DHT sensor(A0,SEN11301P);

int main()
{
    int temp = 0;
    int humid = 0;
    
    while(1) {
        wait(10);
        int err = sensor.readData();
        if (err == 0) {
            temp = (int) sensor.ReadTemperature(CELCIUS);
            humid = (int) sensor.ReadHumidity();
            sigfox.printf("AT$SS=%02x %02x\n", temp, humid);
            pc.printf("AT$SS=%02x %02x\r\n", temp, humid);
            myled = 0;
        }
        else {
            myled = 1;
            pc.printf("Error when reading temperature/humidity\r\n");
        }

        wait(60);

    }
}
