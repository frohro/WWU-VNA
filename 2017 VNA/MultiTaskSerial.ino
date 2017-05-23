volatile extern bool sendMeasurement;
volatile extern int numberFrequenciestoMeasure, frequencyIndex;
volatile extern float  refSum, measSum;

void setupSerial()
{
  Serial.begin(115200);
}

void loopSerial()
{
   static int indexSending = -1;
   if(sendMeasurement&&(indexSending < frequencyIndex))
   {
       indexSending = frequencyIndex;
       Serial.print(refSum,0); // 0 means this is an integer.
       Serial.print(", ");
       Serial.println(measSum,0);
       Serial.flush();
       sendMeasurement = false;
   }
/*  For debugging  *
  Serial.print("Task 1 millis: ");
  Serial.println(millis());
  delay(1000);*/
}
