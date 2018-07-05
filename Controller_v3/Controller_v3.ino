//Controller for Vestibular Evoked MyoPotentials 
//Arudino Pro Mini/Dynabrush Board Code
//Shreyank Desai
//MS Thesis Project
//Rutgers Univserity
//Spring 2018 
//Version 3

#include <SPI.h>
#include <SD.h>
File data;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  
  int CardDetect = 9; //Card Detect Pin
  SD.begin(10); //SD chip select Pin 10
    while (!Serial) {
      ; // Wait for serial port to connect. Needed for native USB port only
    }

  //SD card detection - Halts program, Reset after mounting
  //If SD card not deteted LED D2 will light up
  if (digitalRead(CardDetect) == HIGH) {  
    digitalWrite(4, HIGH);
    while (1);
  }

  data = SD.open("/"); //Lists directort files
  printDirectory(data, 0);
  data.close();

  //Pin Designations 
  pinMode(A4,INPUT); //Button
  pinMode(A3,OUTPUT); //Blue
  pinMode(A2,INPUT); //Yellow
  pinMode(A1,INPUT); //FSR1
  pinMode(A0,INPUT); //FSR2
  pinMode(5, OUTPUT); //PWM for Brown
}

void loop() {
  // put your main code here, to run repeatedly:
  int Brown = 5; //Base on transistor, controls solenoid firing
  int Button = A4; //Data acquistion initiaion button
  int Blue = A3; //Emitter on transistor, set to zero/GND
  int Yellow = A2; //Striker internal sensor
  int FSR1 = A1; //FSR1 - LEFT 
  int FSR2 = A0; //FSR2 - Right 

  if(analogRead(Button) > 0){ // && analogRead(Yellow) > 1000){  //*****
    //Checks Button press and Striker internal sensor above threshold 
    NewFile(); //Creates and opens new file
    StrikeRecord(); //Fires striker and records data 
    Serial.println("TEST COMPLETED");
    }
  //END OF PROGRAM - Standby for next button press
}

void NewFile(){
    //Creates new file each call
    //Checks to see if file exists first before creating
    
  char filename[] = "RUN000.TXT";
  for (int i = 0; i <= 999; i++) {
    filename[3] = floor(i/100) + '0';
    filename[4] = (i % 100)/ 10 + '0';
    filename[5] = i%10 + '0';
      if (! SD.exists(filename)) {
        // only open a new file if it doesn't exist
        Serial.println(filename);
        data = SD.open(filename, FILE_WRITE);
        break;  // leave the loop!
      }
    }
    Serial.println("Newfile Function - DONE");
}

void StrikeRecord(){
  //This function sends 50% Duty Cycle square wave
  //to the base of the transistor (brown wire) in the striker
  //allowing the solenoid to fire
    int F = 2; //Frequency in Hz
    int T = 500/F; // half* of period in milliseconds
    int S = 10; //number of strikes requiredq
    int Brown = 5; //Base on transistor, controls solenoid firing
    int set1; //Limit for data acquisition loop first half of period
    int set2; //Limit for data acquisition loop second half of period
    int sig;
    int FSR1 = A1; //FSR1 - LEFT 
    int FSR2 = A0; //FSR2 - Right
    int val1; //FSR1 value
    int val2; //FSR2 value

    digitalWrite(7, HIGH); //D1 LED ON
    for(int i=0; i < S; i++){
      digitalWrite(Brown, HIGH);  
      set1 = millis() + T; //set time limit
      while(millis() <= set1) //Read signal for T ms after strike
      {
        sig = 1023; 
        val1 = analogRead(FSR1);  //Read FSR1 Signal
        val2 = analogRead(FSR2);  //Read FSR2 Signal                          
        Serial.print(val1);  //Printing to serial port
       Serial.print(", "); //Comma separation
       Serial.println(sig);
        data.print(val1);  //Send data to patient file
        data.print(", ");  //Comma separation
        data.println(sig);      
      }
      digitalWrite(Brown, LOW);
      set2 = millis() + T; //set time limit
      while(millis() <= set2) //Read signal for T ms after strike
      {
        sig = 0;
        val1 = analogRead(FSR1);  //Read FSR1 Signal
        val2 = analogRead(FSR2);  //Read FSR2 Signal                          
        Serial.print(val1); //Printing to serial port
        Serial.print(", "); //Comma separation
        Serial.println(sig);
        data.print(val1);  //Send data to patient file
        data.print(", ");  //Comma separation
        data.println(sig);      
      }
    //  for(int t=0; t <= 20; t++){ //GAP
    //    Serial.print("3000");  //Printing to serial port
    //    Serial.print(", "); //Comma separation
    //    Serial.println("3000");
    //    data.print("3000");  //Send data to patient file
    //    data.print(", ");  //Comma separation
    //    data.println("3000");     
    //  }
    }
    digitalWrite(7, LOW);  //D1 LED OFF
    data.close(); //*NOTE: file does save unless closed or flushed
    Serial.println("Striker Function - DONE");
}

void printDirectory(File dir, int numTabs) {
  //When connected to serial port 
  //displays all files in directory 
  
  while (true) {
    File entry =  dir.openNextFile();
    if (! entry) {
      // no more files
      break;
    }
    for (uint8_t i = 0; i < numTabs; i++) {
      Serial.print('\t');
    }
    Serial.print(entry.name());
    if (entry.isDirectory()) {
      Serial.println("/");
      printDirectory(entry, numTabs + 1);
    } else {
      // files have sizes, directories do not
      Serial.print("\t\t");
      Serial.println(entry.size(), DEC);
    }
    entry.close();
  }
}




