

#include <SD.h>
#include <SPI.h>
File data; 

void setup() {
  Serial.begin(1000000);
  SD.begin(10, SPI_FULL_SPEED);

  unsigned long time1;
  unsigned long time2;
  unsigned long current;
  unsigned long set;
  unsigned long count = 0;
  int sps; 
  int x = 1023;
  
  SD.remove("speed.txt");
  
  data = SD.open("speed.txt", FILE_WRITE);
  //O_CREAT | O_WRITE
  time1 = micros(); 
  set = micros() + 500000;
  while(micros() < set){
    data.println(x);
    count = count + 1; 
   }
  time2 = micros();

  data.flush();
  data.close();

  sps = (count * 1000000)/(time2 - time1);
  Serial.println(sps); 
  

  
}

void loop() {
  // put your main code here, to run repeatedly:

}
