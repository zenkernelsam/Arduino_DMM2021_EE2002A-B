//Group 16 DMM Arduino Project Program Code [22/11/2021]
//The program achieves the following challenges:
//1. Measure DC voltage higher than 5V (The system can measure up to 50V)
//2. Can show mA and mV instead of A and V when the measurement is small
//3. Can show Kohm and ohm in auto-range

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DigiPotX9Cxxx.h> //For resistance testor
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
// The pins for I2C are defined by the Wire-library. 
// On an arduino UNO:       A4(SDA), A5(SCL)
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
#define NUMFLAKES     10 // Number of snowflakes in the animation example


//Added by Cisco

//Menu startup varialbles
int i  = 10; // this is the y-position of the selection sign ">" ranging from 10-30
int b = 10; //b ranges from 0-40, indicating 4 lines (4 function types)
int page = 1; // page as default
int x = 0; int y = 0;  // randomly named variables in order to control button pressed or not

//Current sensor testor variables
int analogPin = A2; // Current sensor output protocol
//int sensitivity_value = 10;
//int _sensitivity;

//Voltage sensor testor variables
float voltage_startup; //voltage test value
//Resistance testor variables
DigiPot pot(2,3,4);      // define an object of X9C104  
//float var_voltage;
float unknown_voltage;
//float var_resistance;
float unknown_resistance;
//Array Resistance testor
                 // 0         1           2         3        4         5        6         7 
int   pot_SW[] =  {30       , 25       ,20       , 15      , 9      , 3,         2      , 1 };
float res_val[] = {105289.37, 87753.27 ,70369.85 , 53361.86, 31681.3, 10513.42, 7039.14 , 3553.47 }; //Method done by Dieter
//Resistance testor variable
char *pPUnit=" Ohm";
float Rx;

//Resistance Function
 float cal_res(float voltRead, float pos_res)
{
    Rx=(voltRead * pos_res)/(5- voltRead);
    delay(20);

   //display.clearDisplay();
  //Serial.print("Rx:");  

  if(Rx > 1e6)
  {
      pPUnit = " MOhm";
      Rx = Rx / 1e6;
  }
  else if(Rx > 1e3)
  {
      pPUnit = " KOhm";
      Rx = Rx / 1e3;    
  }
  else if(Rx>1)
  {
      Rx = Rx;  
  }

    return Rx; //return Rx has some problem
}

void selectmenu()
{
  int y = digitalRead(5); //Digital Port 5
  if (y==1 && b==10)
  page = 3; //We select voltage measure//
  if(y==1 && b==20)
  page = 4;
  if(y==1 && b==30)
  page = 5;
  if(y==1 && b==40)
  page=6;

}
  


void _scroll()  //Down function. When button pressed selection sign ">" will move downwards.
{ 
  int x = digitalRead(6); //Digital Port 6
  if (x == 1)
  {
    
    i = i + 10; //y-position moves downwards
    b = b + 10; //b value increases to tell selectmenu() which test users are pointing to
    x = 0; //Reset the x to 0
    
  }

    if(i>20)
  {
    i=10; //Since ">" can only move in two lines, so it needs to move upwards to the first line after it exceeds i = 20 
  }

  if(b>40)
  {
    b=10; //Since there are only four functions, integer b needs to reset to 10 after it exceeds b = 40.
  }
  
  if (b == 10) //The belowing four conditional statements arrange the relationship between integer b and integer page. So the screen can display the page and information correctly
  {
    page = 1;
  }

  if (b == 20)
  {
    page = 1;
  }
  
  
  if (b == 30)
  {
  page = 2;
  i = 10; //b = 30 is pointing to the function of resistance test, which is the first line of page 2. So page = 2 and i = 10. Integer i have to reset as here i is dependent of b.
  }
  
  if (b == 40)
  {
  page = 2;
  }
  
  display.display(); 

  }


int returnmenu() //Press the button and return back to the main menu.
{
  
  //Let me adjust by just using 2 buttons
  int z = digitalRead(5);
  if(z==1)
  {
    page = 1;
    b = 10;
    i = 10;
    z = 0;
    return 1; //Return a value back to while(1) and let the loop breaks itself
  }
}



//Ended adding by Cisco//

void setup() {
  Serial.begin(9600);
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  //display.setCursor(10, 10);  
  //display.println(F("GP 16 - Starting DMM..."));
  //display.display();
  delay(2000); // Pause for 2 seconds
  // Clear the buffer
  display.clearDisplay();
  delay(2000);
  // display.display() is NOT necessary after every single drawing command,
  // unless that's what you want...rather, you can batch up a bunch of
  // drawing operations and then update the screen all at once by calling
  // display.display(). These examples demonstrate both approaches...
   display.setTextColor(SSD1306_WHITE); // Draw white text
   display.cp437(true);         // Use full 256 char 'Code Page 437' font
}

void loop() {
  int x = digitalRead(2);
  int y = digitalRead(3);
  int z = digitalRead(4);


  while(page == 1) {

    display.clearDisplay();
    display.setCursor(10, 0);     // Start at top-left corner
    display.setTextSize(0.5);
    display.print("Group 16");  // to show string
    display.setCursor(10, 10);  
    display.println(F("VOLTAGE MEASURE"));
    display.setCursor(10, 20);  
    display.println(F("CURRENT MEASURE"));
    display.setCursor(0, i); 
    display.println(">"); 
    _scroll();
    selectmenu();
    display.display();
    delay(100);
  }

  while(page == 2) {

    display.clearDisplay();
    display.setCursor(10, 0);     // Start at top-left corner
    display.setTextSize(0.5);
    display.print("Group 16");  // to show string
    display.setCursor(10, 10);  
    display.println(F("RESISTANCE MEASURE"));
    display.setCursor(10, 20);  
    display.println(F("SHORT CIRCUIT"));
    display.setCursor(0, i); //A function of y-position of ">"
    display.println(">");
    _scroll(); 
    selectmenu();
    display.display();
    delay(100);
  }

  if(page == 3) {

    while(1){
    
    float voltage_test_result = voltage_startup*10; // Equal to voltage test value * 10 since 1 to 10 ratio (9k <in serires> 1k resistor) so the actual voltage is the voltage * 10
    voltage_startup = 5.0 * analogRead(A0)  / 1023;
    display.clearDisplay();
    display.setCursor(0, 0);     // Start at top-left corner
    display.setTextSize(0.5);
    display.print("Voltage Test");  // to show b status
    display.setCursor(0, 10);  
    display.print("Voltage: "); //voltage value*10
    
    //display.println(F("Called volatge function ()")); //You can call the voltage value here i.e. display.println(voltage); The voltage function can be put at the top. i.e. Before: //Ended adding by Cisco//

    if((voltage_test_result < 1 && voltage_test_result > 0) || (voltage_test_result > -1 && voltage_test_result < 0)) //When the value is small
    {
    voltage_test_result = voltage_test_result*1000; //Auto-range
    display.print(voltage_test_result); //voltage value*10 accroding to voltage divider ratio of 9:1
    display.print("mV"); //Change unit
    display.setCursor(0, 20); 
    display.print("Connect B and D");
    }
    else
    {
    display.print(voltage_test_result); //voltage value*10
    display.print("V"); //Keep default unit
    display.setCursor(0, 20); 
    display.print("Connect B and D");
    }
    display.display();
    
    returnmenu(); //means back

    if(returnmenu() == 1)
    {
    delay(100);
    break;
    }
    delay(100);
    }
}

if(page == 4) {

    while(1)
    {
    const int averageValue = 500;
    long int sensorValue = 0;  // variable to store the sensor value read
    float voltage = 0;
    float current = 0;

    for (int j = 0; j < averageValue; j++)
  {
    sensorValue += analogRead(analogPin); //analogPin = A2

    // wait 2 milliseconds before the next loop
    delay(2);
  }
  

  sensorValue = sensorValue / averageValue; //ADC value without offset
  voltage = sensorValue * 5.0 / 1023.0; //Voltage value without offset
  current = (voltage - 2.52) / 0.185; //The sensitivity is 0.185 according to our current sensor model
  //current = (voltage - 2.5) / _sensitivity; 


    
    display.clearDisplay();
    //display.setCursor(0, 0);     // Start at top-left corner
    //display.setTextSize(0.5);
    //display.print(b);  // to show B status
    display.setCursor(0, 0);  
    //display.println(F("Called current function ()")); 
    display.print("ADC Value/Voltage:"); //Raw data without offset
    display.setCursor(0,10);  
    display.print(sensorValue); //no offset
    display.print(" / ");
    display.print(voltage);  //no offset
    display.print("V");

    display.setCursor(0, 20);  
    display.print("Current: ");
    if ((current < 1 && current > 0) || (current > -1 && current < 0)) //When the value is small
    {
    current = current*1000; //Auto-range
    display.print(current); //Offset
    display.print("mA"); //Change unit
    }
    else
    {
    display.print(current);
    display.print("A"); //Keep unit by default
    }
    
    returnmenu(); //means back
    display.display();

    if(returnmenu()==1)
    {
    delay(100);
    break;
    }
    
    delay(100);
    }
}


  if(page == 5) {

    while(1)
    {

    int k;
    int pos_val;
    display.clearDisplay();
    display.setCursor(0, 0);     // Start at top-left corner
    display.setTextSize(0.5);
    display.print(b);  // to show string
    for(k=0;k<=7; k++)
  {
    pos_val = pot_SW[k];
    //pot.set(99);
    //delay(500);
    pot.set(pos_val);
  //  Serial.print("pos_val="); Serial.print(pos_val); Serial.print(" ");
    //delay(30);
    unknown_voltage = 5.0 * analogRead(A1)  / 1023;     // read the corresponding voltage level of unknown resistor
    if ( unknown_voltage > 2)
    {
    display.clearDisplay();
    display.setCursor(0, 0);     // Start at top-left corner
    display.setTextSize(0.5);
    display.print("unknown voltage > 2");  // to show string
    delay(100);
    }
    //else
    //{
      // Serial.print("("); Serial.print(unknown_voltage); Serial.print(") ");
    //}
  }
  if(k>7)
    k=7;
  if(unknown_voltage > 4.9)
  {
    display.clearDisplay();
    display.setCursor(0, 10);     // Start at top-left corner
    display.setTextSize(0.5);
    display.print("Over Load");  // to show string
    display.setCursor(0, 20);
    display.print("Connect C and D");
    display.display();
    delay(50);
    //break;
  }
  if(unknown_voltage < 4.9) 
  {
    cal_res(unknown_voltage, res_val[k]); 
    display.setCursor(0,10); 
    display.setTextSize(0.5);
    display.print("R:"); 
    display.print(Rx); 
    display.print(pPUnit);  //print out pPunit (Prefix)
    //display.print(" Ohm"); //Ohm (work?)
    display.setCursor(0,20); 
    display.print("Connect C and D");
    delay(50);
  }

    //display.setCursor(0, 10);  
    //display.println(F("Called resistance function ()")); 
    
    display.display();

    returnmenu(); //means back
    if(returnmenu()==1)
    {
    delay(100);
    break;
    }
    
    delay(100);
    //Don't know why. When returnmenu() is invoked in this page, the main menu loops -> press red button to restart Adrinuo is OK.
  }
}

if(page == 6) 
{

    while(1)
    {
  
    int shortport = digitalRead(7);
    display.clearDisplay();
    display.setCursor(0, 0);     // Start at top-left corner
    display.setTextSize(0.5);
    display.print("SC Test");  // to show string
    display.setCursor(0, 10);  
    display.println("Connect A and D"); //Short Circuit Function
      
    if(shortport == 1) //Short circuit -> current detected? -> digital port reads
    {
    display.setCursor(0, 20);  
    display.println("Status: Not Detected"); //Short Circuit Function
    }

    if(shortport == 0)
    {
    display.setCursor(0, 20);  
    display.println("Status: Short Circuit"); //Short Circuit Function
    }
    
    
    returnmenu(); //Return menu
    display.display();
    
    if(returnmenu()==1)
    {
    delay(100);
    break;
    }

    delay(100);
    }
}
}
