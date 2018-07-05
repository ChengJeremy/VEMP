

#include <SD.h>
#include <SPI.h>
File data; 

void setup() {
  Serial.begin(115200);
  SD.begin(10, SPI_FULL_SPEED);

  unsigned long time1;
  unsigned long time2;
  unsigned long current;
  unsigned long set;
  unsigned int count = 0;
  unsigned long sps; 
  int x;
  int sample[16000];
 // x = &sample[0];
  int sample2[16000];
  int sample3[16000];
  int sample4[16000];  
  int Yellow = A5;
  SD.remove("speed.txt");
  
  data = SD.open("speed.txt", FILE_WRITE);
  time1 = micros(); 
  set = micros() + 500000;
  while(micros() < set){
    //data.println(x);
    sample[1] = {analogRead(Yellow)};
    //sample2[count] = {analogRead(Yellow)};
    //sample3[count] = 1023;
    //sample4[count] = 1023;
    //Serial.println(count);
    x = x + 1; 
    count = count + 1; 
   }
  time2 = micros();

  data.flush();
  data.close();

  
  sps = (count * 1000000)/(time2 - time1);
  int y = sample[1];
  Serial.println(sps); 

  for(int i = 0; i < 6000; i++)
    {
    Serial.println(sample[i]);  
    }
  //Serial.println(count-1);
  //Serial.println(y);
  //Serial.println(sample[10]);
  //Serial.println(sample[10000]);
  

  
}

void loop() {
  // put your main code here, to run repeatedly:

}
