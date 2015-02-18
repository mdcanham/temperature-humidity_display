#include <LiquidCrystal_I2C.h>
#include <DHT.h>

#define DHTPIN 2     // what pin we're connected to
#define DHTTYPE DHT11   // DHT 11 

DHT dht(DHTPIN, DHTTYPE);

void setup(){
  dht.begin();
}

void loop(){

}
