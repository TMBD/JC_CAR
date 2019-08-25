#include <ServoTimer2.h>
#include <VirtualWire.h>


#define ULTRASON_TRIG_PIN  7
#define ULTRASON_ECHO_PIN  8
#define SERVO_PIN          12
#define RF_RECEIVER_PIN    13

#define MOTOR_RIGHT_FRONT_SPEED_PIN    5  //couleur verte 9
#define MOTOR_RIGHT_FRONT_GOAHEAD_PIN  9  //couleur jaune 5
#define MOTOR_RIGHT_FRONT_GOBACK_PIN   4  //couleur orange

#define MOTOR_LEFT_FRONT_SPEED_PIN    11  //couleur verte 11
#define MOTOR_LEFT_FRONT_GOAHEAD_PIN  A3  //couleur bleu
#define MOTOR_LEFT_FRONT_GOBACK_PIN   A2  //couleur verte

#define MOTOR_RIGHT_BACK_SPEED_PIN     6  //couleur verte
#define MOTOR_RIGHT_BACK_GOAHEAD_PIN   2  //couleur verte
#define MOTOR_RIGHT_BACK_GOBACK_PIN    10  //couleur jaune 3

#define MOTOR_LEFT_BACK_SPEED_PIN     3  //couleur jaune 10
#define MOTOR_LEFT_BACK_GOAHEAD_PIN   A4  //coul-ur verte
#define MOTOR_LEFT_BACK_GOBACK_PIN    A5  //couleur verte

#define fromLow 0
#define fromHigh 512
#define toLow 0
#define toHigh 512

///////////////////////////////////
#define TX_BAUDE 2000
#define TAILLE_MESSAGE 18

#define MIN_SPEED_WHEEL 40
#define MAX_SPEED_WHEEL 255
#define LEAST_SPEED_FOR_RUN 80
#define SPEED_STEP 25

#define MIN_ANGLE 0
#define MAX_ANGLE 180
#define SERVO_0_DEGRE 750
#define SERVO_90_DEGRE 1500
#define SERVO_180_DEGRE 2250

///////////ULTRASON////////////////////////////
#define SECURITY_DISTANCE_MM 200
/* Constantes pour le timeout */
const unsigned long MEASURE_TIMEOUT = 25000UL; // 25ms = ~08m à 340m/s

/* Vitesse du son dans l'air en mm/us */
const float SOUND_SPEED = 340.0 / 1000;

///////////SERVO///////////////////

ServoTimer2 myServo;

////////////////////////////////////////

char end_char[2];

bool runed = false;
int LF_previousSpeed = 0;
int LB_previousSpeed = 0;
int RF_previousSpeed = 0;
int RB_previousSpeed = 0;

//short notWorking = 0;

void setup() {
 
  pinMode(MOTOR_RIGHT_FRONT_SPEED_PIN, OUTPUT);
  pinMode(MOTOR_RIGHT_FRONT_GOAHEAD_PIN, OUTPUT);
  pinMode(MOTOR_RIGHT_FRONT_GOBACK_PIN, OUTPUT);

  pinMode(MOTOR_LEFT_FRONT_SPEED_PIN, OUTPUT);
  pinMode(MOTOR_LEFT_FRONT_GOAHEAD_PIN, OUTPUT);
  pinMode(MOTOR_LEFT_FRONT_GOBACK_PIN, OUTPUT);

  pinMode(MOTOR_RIGHT_BACK_SPEED_PIN, OUTPUT);
  pinMode(MOTOR_RIGHT_BACK_GOAHEAD_PIN, OUTPUT);
  pinMode(MOTOR_RIGHT_BACK_GOBACK_PIN, OUTPUT);

  pinMode(MOTOR_LEFT_BACK_SPEED_PIN, OUTPUT);
  pinMode(MOTOR_LEFT_BACK_GOAHEAD_PIN, OUTPUT);
  pinMode(MOTOR_LEFT_BACK_GOBACK_PIN, OUTPUT);

////////////ULTRASON////////////////////
  pinMode(ULTRASON_TRIG_PIN, OUTPUT);
  digitalWrite(ULTRASON_TRIG_PIN, LOW); // La broche TRIGGER doit être à LOW au repos
  pinMode(ULTRASON_ECHO_PIN, INPUT);
  
//////////////SERVO//////////////////////////
  //myServo.attach(SERVO_PIN);


/*
  digitalWrite(MOTOR_LEFT_BACK_GOAHEAD_PIN, HIGH);
  digitalWrite(MOTOR_LEFT_BACK_GOBACK_PIN, LOW);

  digitalWrite(MOTOR_LEFT_FRONT_GOAHEAD_PIN, HIGH);
  digitalWrite(MOTOR_LEFT_FRONT_GOBACK_PIN, LOW);

  digitalWrite(MOTOR_RIGHT_FRONT_GOAHEAD_PIN, HIGH);
  digitalWrite(MOTOR_RIGHT_FRONT_GOBACK_PIN, LOW);

  digitalWrite(MOTOR_RIGHT_BACK_GOAHEAD_PIN, HIGH);
  digitalWrite(MOTOR_RIGHT_BACK_GOBACK_PIN, LOW);
*/

  //////////////////////////////////////////////

  Serial.begin(9600); // Debugging only
  Serial.println("receiver");
  vw_set_rx_pin(RF_RECEIVER_PIN);

  vw_set_ptt_inverted(true); // Required for DR3100
  vw_setup(TX_BAUDE);      // Bits per sec
  vw_rx_start();       // Start the receiver PLL running

}

void loop() {


  /*
 //delay(5000);
  analogWrite(MOTOR_LEFT_FRONT_SPEED_PIN, 250);  
  analogWrite(MOTOR_LEFT_BACK_SPEED_PIN, 250);
  
  analogWrite(MOTOR_RIGHT_FRONT_SPEED_PIN, 250);
  analogWrite(MOTOR_RIGHT_BACK_SPEED_PIN, 250);

  delay(1000);
  digitalWrite(MOTOR_LEFT_FRONT_SPEED_PIN, LOW);
  digitalWrite(MOTOR_LEFT_BACK_SPEED_PIN, LOW);
  digitalWrite(MOTOR_RIGHT_FRONT_SPEED_PIN, LOW);
  digitalWrite(MOTOR_RIGHT_BACK_SPEED_PIN, LOW);
  

  
  digitalWrite(MOTOR_RIGHT_FRONT_GOAHEAD_PIN, LOW);
  digitalWrite(MOTOR_RIGHT_FRONT_GOBACK_PIN, HIGH);
 
  digitalWrite(MOTOR_LEFT_FRONT_GOAHEAD_PIN, LOW);
  digitalWrite(MOTOR_LEFT_FRONT_GOBACK_PIN, HIGH);

  digitalWrite(MOTOR_RIGHT_BACK_GOAHEAD_PIN, LOW);
  digitalWrite(MOTOR_RIGHT_BACK_GOBACK_PIN, HIGH);

  digitalWrite(MOTOR_LEFT_BACK_GOAHEAD_PIN, LOW);
  digitalWrite(MOTOR_LEFT_BACK_GOBACK_PIN, HIGH);
  
  delay(2000);

*/
  /*
  uint8_t buf[VW_MAX_MESSAGE_LEN];
  uint8_t buflen = VW_MAX_MESSAGE_LEN;
  if (vw_get_message(buf, &buflen)) // Non-blocking
  {
      int i;
      digitalWrite(13, true); // Flash a light to show received good message
      // Message with a good checksum received, dump it.
      Serial.print("Got: ");
      
      for (i = 0; i < buflen; i++)
      {
          Serial.print(buf[i], HEX);
          Serial.print(" ");
      }
      Serial.println("");
      digitalWrite(13, false);
  }

*/
  
  char message[TAILLE_MESSAGE];
  //char *message; 
  //message = (char*)message_recu;
  //byte taille = TAILLE_MESSAGE;
  //Serial.println(".");
  Serial.println("*************************");

  uint8_t buflen = TAILLE_MESSAGE;
  vw_wait_rx();

  
    
  if(vw_get_message(message, &buflen)){
    
    //delay(50);
    digitalWrite(13, true); 
    Serial.println(message);
    //String str(chArray);
    String str_message = String(message);
    int sens = getSpeedValue(str_message, '_', 0).toInt(); //sens = 0 la voiture roule en avant
    int LF_WheelSpeed = getSpeedValue(str_message, '_', 1).toInt();
    int LB_WheelSpeed = getSpeedValue(str_message, '_', 2).toInt();
    int RF_WheelSpeed = getSpeedValue(str_message, '_', 3).toInt();
    int RB_WheelSpeed = getSpeedValue(str_message, '_', 4).toInt();
    Serial.println(sens);
    Serial.println(LF_WheelSpeed);
    Serial.println(LB_WheelSpeed);
    Serial.println(RF_WheelSpeed);
    Serial.println(RB_WheelSpeed);
    
    //setServoAngle(LF_WheelSpeed, LB_WheelSpeed, RF_WheelSpeed, RB_WheelSpeed);
    //myServo.write(750);
    //delay(1000);
    if(obstacle() and sens == 0){
      analogWrite(MOTOR_LEFT_FRONT_SPEED_PIN, 0); 
      analogWrite(MOTOR_LEFT_BACK_SPEED_PIN, 0); 
      analogWrite(MOTOR_RIGHT_FRONT_SPEED_PIN, 0); 
      analogWrite(MOTOR_RIGHT_BACK_SPEED_PIN, 0); 
      LF_previousSpeed = 0;
      LB_previousSpeed = 0;
      RF_previousSpeed = 0;
      RB_previousSpeed = 0;
    }
    else{
      if(LF_WheelSpeed > LF_previousSpeed + SPEED_STEP) LF_WheelSpeed = LF_previousSpeed = LF_previousSpeed + SPEED_STEP;
      else LF_previousSpeed = LF_WheelSpeed;
  
      if(LB_WheelSpeed > LB_previousSpeed + SPEED_STEP) LB_WheelSpeed = LB_previousSpeed = LB_previousSpeed + SPEED_STEP;
      else LB_previousSpeed = LB_WheelSpeed;
  
      if(RF_WheelSpeed > RF_previousSpeed + SPEED_STEP) RF_WheelSpeed = RF_previousSpeed = RF_previousSpeed + SPEED_STEP;
      else RF_previousSpeed = RF_WheelSpeed;
  
      if(RB_WheelSpeed > RB_previousSpeed + SPEED_STEP) RB_WheelSpeed = RB_previousSpeed = RB_previousSpeed + SPEED_STEP;
      else RB_previousSpeed = RB_WheelSpeed;
  
  
  
      if((LF_WheelSpeed < MIN_SPEED_WHEEL) and 
         (LB_WheelSpeed < MIN_SPEED_WHEEL) and 
         (RF_WheelSpeed < MIN_SPEED_WHEEL) and 
         (RB_WheelSpeed < MIN_SPEED_WHEEL) )
      {
        runed = false;
        analogWrite(MOTOR_LEFT_FRONT_SPEED_PIN, 0); 
        analogWrite(MOTOR_LEFT_BACK_SPEED_PIN, 0); 
        analogWrite(MOTOR_RIGHT_FRONT_SPEED_PIN, 0); 
        analogWrite(MOTOR_RIGHT_BACK_SPEED_PIN, 0); 
      }
      else{
          if(sens == 0){
          digitalWrite(MOTOR_LEFT_FRONT_GOAHEAD_PIN, HIGH);
          digitalWrite(MOTOR_LEFT_FRONT_GOBACK_PIN, LOW);
    
          digitalWrite(MOTOR_LEFT_BACK_GOAHEAD_PIN, HIGH);
          digitalWrite(MOTOR_LEFT_BACK_GOBACK_PIN, LOW);
          
          digitalWrite(MOTOR_RIGHT_FRONT_GOAHEAD_PIN, HIGH);
          digitalWrite(MOTOR_RIGHT_FRONT_GOBACK_PIN, LOW);
         
          digitalWrite(MOTOR_RIGHT_BACK_GOAHEAD_PIN, HIGH);
          digitalWrite(MOTOR_RIGHT_BACK_GOBACK_PIN, LOW);
          
        }else{
          digitalWrite(MOTOR_LEFT_FRONT_GOAHEAD_PIN, LOW);
          digitalWrite(MOTOR_LEFT_FRONT_GOBACK_PIN, HIGH);
    
          digitalWrite(MOTOR_LEFT_BACK_GOAHEAD_PIN, LOW);
          digitalWrite(MOTOR_LEFT_BACK_GOBACK_PIN, HIGH);
          
          digitalWrite(MOTOR_RIGHT_FRONT_GOAHEAD_PIN, LOW);
          digitalWrite(MOTOR_RIGHT_FRONT_GOBACK_PIN, HIGH);
         
          digitalWrite(MOTOR_RIGHT_BACK_GOAHEAD_PIN, LOW);
          digitalWrite(MOTOR_RIGHT_BACK_GOBACK_PIN, HIGH);  
        }
    
        if(!runed){
          runed = true;
          analogWrite(MOTOR_LEFT_FRONT_SPEED_PIN, LEAST_SPEED_FOR_RUN); 
          analogWrite(MOTOR_LEFT_BACK_SPEED_PIN, LEAST_SPEED_FOR_RUN); 
          analogWrite(MOTOR_RIGHT_FRONT_SPEED_PIN, LEAST_SPEED_FOR_RUN); 
          analogWrite(MOTOR_RIGHT_BACK_SPEED_PIN, LEAST_SPEED_FOR_RUN);
          delay(50);
        }
        
        analogWrite(MOTOR_LEFT_FRONT_SPEED_PIN, LF_WheelSpeed); 
        analogWrite(MOTOR_LEFT_BACK_SPEED_PIN, LB_WheelSpeed); 
        analogWrite(MOTOR_RIGHT_FRONT_SPEED_PIN, RF_WheelSpeed); 
        analogWrite(MOTOR_RIGHT_BACK_SPEED_PIN, RB_WheelSpeed); 
        
      }
      
  
  
      /*analogWrite(MOTOR_LEFT_FRONT_SPEED_PIN, 240); 
      analogWrite(MOTOR_LEFT_BACK_SPEED_PIN, 140); 
      analogWrite(MOTOR_RIGHT_FRONT_SPEED_PIN, 210); 
      analogWrite(MOTOR_RIGHT_BACK_SPEED_PIN, 250); 
      */
      //Serial.println("---------------------");
      digitalWrite(13, false);
    }

  }

  
  
  
}


////////////END LOOP////////////////////

String getSpeedValue(String data, char separator, int index)
{
  int found = 0;
  int strIndex[] = {0, -1};
  int maxIndex = data.length()-1;

  for(int i=0; i<=maxIndex && found<=index; i++){
    if(data.charAt(i)==separator || i==maxIndex){
        found++;
        strIndex[0] = strIndex[1]+1;
        strIndex[1] = (i == maxIndex) ? i+1 : i;
    }
  }

  return found>index ? data.substring(strIndex[0], strIndex[1]) : "";
}

void setServoAngle(int LF_WheelSpeed, int LB_WheelSpeed, int RF_WheelSpeed, int RB_WheelSpeed){
  int meanWheelLeft = (LF_WheelSpeed + LB_WheelSpeed)/2;
  int meanWheelRight = (RF_WheelSpeed + RB_WheelSpeed)/2;
  if(meanWheelLeft == meanWheelRight) myServo.write(SERVO_90_DEGRE);
  else if(meanWheelLeft > meanWheelRight){
    int angle = map(meanWheelRight, MIN_SPEED_WHEEL, MAX_SPEED_WHEEL, MIN_ANGLE, MAX_ANGLE);
    int angleServo = map(angle, MIN_ANGLE, MAX_ANGLE, SERVO_90_DEGRE, SERVO_180_DEGRE);
    myServo.write(angleServo);
  }
  else{
    int angle = map(meanWheelLeft, MIN_SPEED_WHEEL, MAX_SPEED_WHEEL, MIN_ANGLE, MAX_ANGLE);
    int angleServo = map(angle, MIN_ANGLE, MAX_ANGLE, SERVO_0_DEGRE, SERVO_90_DEGRE);
    myServo.write(angleServo);
  }
}

bool obstacle(){
  digitalWrite(ULTRASON_TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(ULTRASON_TRIG_PIN, LOW);

  /* 2. Mesure le temps entre l'envoi de l'impulsion ultrasonique et son écho (si il existe) */
  long measure = pulseIn(ULTRASON_ECHO_PIN, HIGH, MEASURE_TIMEOUT);
   
  /* 3. Calcul la distance à partir du temps mesuré */
  int distance_mm = measure / 2.0 * SOUND_SPEED;
  Serial.println("-----------obstacle----------");
  Serial.println(distance_mm);
  if(distance_mm < SECURITY_DISTANCE_MM) return true;
  else return false;
}
