#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>

LiquidCrystal_I2C lcd(0x27/*LCD Address*/, 16/*LCD Columns*/, 2/*LCD Rows*/);
DHT dht(2/*DHT Connection Pin*/, DHT11/*Type of DHT sensor*/);

byte degreesC[8] = {
  B11000,
  B11000,
  B00000,
  B01111,
  B01000,
  B01000,
  B01111,
};

boolean activated = true;
boolean firstActivationRefresh = true;
double activationTime = millis();
double buttonPressTime = millis();
double refreshTime = millis();
int currentDisplay = 1;
int latchOnTime = 30000;

void setup(){
  
  pinMode(7, INPUT);
  pinMode(7, INPUT);
  
  //initialise temperature/humidity sensor
  dht.begin();
  
  // initialize the LCD
  lcd.begin();

  lcd.createChar(0, degreesC);
  
  
  // Opening message sequence
  lcd.backlight();
  lcd.setCursor(16,0);
  lcd.print("Welcome!");
  for(int i = 0; i < 24; i++){
    lcd.scrollDisplayLeft();
    delay(350);
  }
  lcd.clear();
  lcd.noBacklight();
}

void loop(){
  if(digitalRead(7) == HIGH && millis() - buttonPressTime >= 500){
    buttonPressTime = millis(); 
    if(!activated){
      activationTime = millis();
      activated = true;
      firstActivationRefresh = true;
    }else if(activated) {
      activated = false;
    }
   
  } else if(millis() - activationTime >= latchOnTime) {
    activated = false;
  }
  
  if(digitalRead(8) == HIGH && millis() - buttonPressTime >= 500){
    buttonPressTime = millis();
    switchCurrentDisplay();
    
  }
  
  if(!activated){
    lcd.noBacklight();
    lcd.clear();
  } 
  
  if(activated && (firstActivationRefresh || millis() - refreshTime >= 5000)){
      refreshTime = millis();
      lcd.backlight();
      float humidity, temperatureC, temperatureF;
      readValues(&humidity, &temperatureC, &temperatureF);
    
      // Check if any reads failed and exit early (to try again).
      if (isnan(humidity) || isnan(temperatureC) || isnan(temperatureF)) {
        lcd.clear();
        lcd.print("Error");
        return;
      }
    
    printValues(humidity, temperatureC, temperatureF);
    firstActivationRefresh = false;

  }
  
}

void readValues(float *humidity, float *temperatureC, float *temperatureF){
  *humidity = dht.readHumidity();
  *temperatureC = dht.readTemperature();
  *temperatureF = dht.readTemperature(true);
}

void printValues(float humidity, float temperatureC, float temperatureF){
  lcd.clear();
  if(currentDisplay == 1){
    lcd.print("Current Temp:");
    lcd.setCursor(5,1);
    lcd.print(temperatureC);
    lcd.write(byte(0));
  } else if(currentDisplay == 2){
    lcd.print("Current Humid:");
    lcd.setCursor(5,1);
    lcd.print(humidity);
    lcd.print("%");
  }else if(currentDisplay == 3){
    lcd.print("Heat Index:");
    lcd.setCursor(5,1);
    lcd.print(dht.convertFtoC(dht.computeHeatIndex(temperatureF, humidity)));
    lcd.write(byte(0));
  } else {
    lcd.clear();
    lcd.print("Error");
  }
  
}

void switchCurrentDisplay(){
  if(currentDisplay == 3){
    currentDisplay = 1;
  } else {
    currentDisplay++;
  }
  firstActivationRefresh = true;
}
