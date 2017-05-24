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
       Serial.print(refSum);
       Serial.print(", ");
       Serial.println(measSum);
       Serial.flush();
       sendMeasurement = false;
   }
}
