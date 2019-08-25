#include <VirtualWire.h>
#include<Wire.h>
//POUR LA TRANSMISSION DES DONNEES
//#define X_AXIS_PIN A0
//#define Y_AXIS_PIN A1
#define TX_DATA 3
#define TX_BAUDE 2000
#define TAILLE_MESSAGE 18

//############## ROTATION (X AXIS)###################
//---------- LEFT WHEEL -------------------
#define STOP_LEFT_FRONT_WHEEL_MIN_LIMIT 10
#define STOP_LEFT_FRONT_WHEEL_MAX_LIMIT 40

#define STOP_LEFT_BACK_WHEEL_MIN_LIMIT 40
#define STOP_LEFT_BACK_WHEEL_MAX_LIMIT 90

#define STOP_LEFT_FRONT_BACK_WHEEL_MIN_LIMIT 90
#define STOP_LEFT_FRONT_BACK_WHEEL_MAX_LIMIT 170

//-------------- STOP --------------------
#define STOP_CAR_MIN_LIMIT 170
#define STOP_CAR_MAX_LIMIT 190

//-------------- RIGHT WHEEL --------------

#define STOP_RIGHT_FRONT_BACK_WHEEL_MIN_LIMIT 190
#define STOP_RIGHT_FRONT_BACK_WHEEL_MAX_LIMIT 270

#define STOP_RIGHT_BACK_WHEEL_MIN_LIMIT 270
#define STOP_RIGHT_BACK_WHEEL_MAX_LIMIT 310

#define STOP_RIGHT_FRONT_WHEEL_MIN_LIMIT 310
#define STOP_RIGHT_FRONT_WHEEL_MAX_LIMIT 350


//------------- STRAIGHT -----------------
#define STRIGHT_MIN_LIMIT_LEFT 0
#define STRIGHT_MAX_LIMIT_LEFT 10

#define STRIGHT_MIN_LIMIT_RIGHT 350
#define STRIGHT_MAX_LIMIT_RIGHT 360

//############### SENS (Y AXIS) #################

#define STOP_CAR_MVT_LEFT_MIN_LIMIT_1 0
#define STOP_CAR_MVT_LEFT_MAX_LIMIT_1 10

#define STOP_CAR_MVT_RIGHT_MIN_LIMIT_1 350
#define STOP_CAR_MVT_RIGHT_MAX_LIMIT_1 360

#define STOP_CAR_MVT_MIN_LIMIT_2 170
#define STOP_CAR_MVT_MAX_LIMIT_2 190

#define MOVE_TO_FORWARD_MIN_LIMIT 10
#define MOVE_TO_FORWARD_MAX_LIMIT 90
#define MOVE_TO_FORWARD_CONST_SPEED_MIN_LIMIT 90
#define MOVE_TO_FORWARD_CONST_SPEED_MAX_LIMIT 170

#define MOVE_TO_BACKWARD_MIN_LIMIT 270
#define MOVE_TO_BACKWARD_MAX_LIMIT 350
#define MOVE_TO_BACKWARD_CONST_SPEED_MIN_LIMIT 190
#define MOVE_TO_BACKWARD_CONST_SPEED_MAX_LIMIT 270

#define MIN_SPEED_WHEEL 30 //0
#define MAX_SPEED_WHEEL 255
#define MOVE_TO_FORWARD 0
#define MOVE_TO_BACKWARD 1


const int MPU_addr=0x68;
int16_t axis_X,axis_Y,axis_Z;
int minVal=265;
int maxVal=402;
short x;
short y;
short z;

//variables pour la transmission des données
char end_char[2];


void setup(){
  //initialisation pour la transmission
  end_char[0] = '_';
  vw_setup(TX_BAUDE);
  vw_set_tx_pin(TX_DATA);
  
  Wire.begin();
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x6B);
  Wire.write(0);
  Wire.endTransmission(true);
  Serial.begin(9600);
  
  
  //vw_rx_start();
}


void loop(){
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x3B); 
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_addr,14,true);
  axis_X = Wire.read()<<8|Wire.read();
  axis_Y = Wire.read()<<8|Wire.read();
  axis_Z = Wire.read()<<8|Wire.read();

  int xAng = map(axis_X,minVal,maxVal,-90,90);
  int yAng = map(axis_Y,minVal,maxVal,-90,90);
  int zAng = map(axis_Z,minVal,maxVal,-90,90);

  x = RAD_TO_DEG * (atan2(-yAng, -zAng) + PI);
  y = RAD_TO_DEG * (atan2(-xAng, -zAng) + PI);
  z = RAD_TO_DEG * (atan2(-yAng, -xAng) + PI);

  /*
  Serial.print("Angle of inclination in X axis = ");
  Serial.print(x);
  Serial.println((char)176);
  Serial.print("Angle of inclination in Y axis= ");
  Serial.print(y);
  Serial.println((char)176);
  Serial.print("Angle of inclination in Z axis= ");
  Serial.print(z);
  Serial.println((char)176);
  Serial.println("-------------------------------------------");
  */

  short initialSpeedWheels;
  short sens;
  getSpeedAndSens(y, &initialSpeedWheels, &sens);
  Serial.println("-------------------------------------------");
  Serial.println(initialSpeedWheels);
  Serial.println(sens);
  short LF_WheelSpeed, LB_WheelSpeed, RF_WheelSpeed, RB_WheelSpeed;
  getDirection(x, initialSpeedWheels, &LF_WheelSpeed, &LB_WheelSpeed, &RF_WheelSpeed, &RB_WheelSpeed);
  /*Serial.println("-------------------------------------------");
  Serial.println(LF_WheelSpeed);
  Serial.println(LB_WheelSpeed);
  Serial.println(RF_WheelSpeed);
  Serial.println(RB_WheelSpeed);
  */

  char chrLF_WheelSpeed[6];
  char chrLB_WheelSpeed[6];
  char chrRF_WheelSpeed[6];
  char chrRB_WheelSpeed[6];
  char chrSens[2];
  /*chrSens[0] = (char)sens;
  chrLF_WheelSpeed = (char*)LF_WheelSpeed;
  chrLB_WheelSpeed = (char*)LB_WheelSpeed;
  chrRF_WheelSpeed = (char*)RF_WheelSpeed;
  chrRB_WheelSpeed = (char*)RB_WheelSpeed;
  */

  itoa(sens, chrSens, 10);
  itoa(LF_WheelSpeed, chrLF_WheelSpeed, 10);
  itoa(LB_WheelSpeed, chrLB_WheelSpeed, 10);
  itoa(RF_WheelSpeed, chrRF_WheelSpeed, 10);
  itoa(RB_WheelSpeed, chrRB_WheelSpeed, 10);

  /*dtostrf(LF_WheelSpeed, 5, 1, chrLF_WheelSpeed);
  dtostrf(LB_WheelSpeed, 5, 1, chrLB_WheelSpeed);
  dtostrf(RF_WheelSpeed, 5, 1, chrRF_WheelSpeed);
  dtostrf(RB_WheelSpeed, 5, 1, chrRB_WheelSpeed);
  */


  char message[TAILLE_MESSAGE];
  strcpy(message, chrSens);
  strcat(message, end_char);
  strcat(message, chrLF_WheelSpeed);
  strcat(message, end_char);
  strcat(message, chrLB_WheelSpeed);
  strcat(message, end_char);
  strcat(message, chrRF_WheelSpeed);
  strcat(message, end_char);
  strcat(message, chrRB_WheelSpeed);

  digitalWrite(13, true);
  vw_send(message, TAILLE_MESSAGE);
  vw_wait_tx();
  digitalWrite(13, false);
  Serial.println("---------------------------------- ");
  Serial.println(message);
  Serial.println(chrSens);
  Serial.println(chrLF_WheelSpeed);
  Serial.println(chrLB_WheelSpeed);
  Serial.println(chrRF_WheelSpeed);
  Serial.println(chrRB_WheelSpeed);
  
  Serial.println("#############################################");
  
  delay(200);
}








void getSpeedAndSens(short angle, short *speedWheel, short *orientation){
  
  //intervalle [0; 10[, [350, 360] et [170, 190[ -> pour arreter la voiture
  if(    ((angle>=STOP_CAR_MVT_LEFT_MIN_LIMIT_1)&&(angle<STOP_CAR_MVT_LEFT_MAX_LIMIT_1))
      || ((angle>=STOP_CAR_MVT_RIGHT_MIN_LIMIT_1)&&(angle<=STOP_CAR_MVT_RIGHT_MAX_LIMIT_1))
      || ((angle>=STOP_CAR_MVT_MIN_LIMIT_2)&&(angle<STOP_CAR_MVT_MAX_LIMIT_2)) ){
    *speedWheel = MIN_SPEED_WHEEL;
    *orientation = MOVE_TO_FORWARD;
    
  } 
  
  //intervalle [10; 90[ pour faire avance la voiture avec une vitesse variable
  else if((angle>=MOVE_TO_FORWARD_MIN_LIMIT)&&(angle<MOVE_TO_FORWARD_MAX_LIMIT)){
    *speedWheel = map(angle, MOVE_TO_FORWARD_MIN_LIMIT, MOVE_TO_FORWARD_MAX_LIMIT, MIN_SPEED_WHEEL, MAX_SPEED_WHEEL);
    *orientation = MOVE_TO_FORWARD;
  }
  //intervalle [90; 170[ pour faire avance la voiture avec une vitesse maximale et constante
  else if((angle>=MOVE_TO_FORWARD_CONST_SPEED_MIN_LIMIT)&&(angle<MOVE_TO_FORWARD_CONST_SPEED_MAX_LIMIT)){
    *speedWheel = MAX_SPEED_WHEEL;
    *orientation = MOVE_TO_FORWARD;
  }

  //intervalle [190; 270[ pour faire marche arriere avec une vitesse maximale et constante
  else if((angle>=MOVE_TO_BACKWARD_CONST_SPEED_MIN_LIMIT)&&(angle<MOVE_TO_BACKWARD_CONST_SPEED_MAX_LIMIT)){
    *speedWheel = MAX_SPEED_WHEEL;
    *orientation = MOVE_TO_BACKWARD;
  }
  //intervalle [270; 350[ pour faire marche arriere avec une vitesse variable
  else if((angle>=MOVE_TO_BACKWARD_MIN_LIMIT)&&(angle<MOVE_TO_BACKWARD_MAX_LIMIT)){
    *speedWheel = MAX_SPEED_WHEEL - map(angle, MOVE_TO_BACKWARD_MIN_LIMIT, MOVE_TO_BACKWARD_MAX_LIMIT, MIN_SPEED_WHEEL, MAX_SPEED_WHEEL);
    *orientation = MOVE_TO_BACKWARD;
  }
}




void getDirection(short angle, short instantSpeed, short *LF_WheelSpeed, short *LB_WheelSpeed, short *RF_WheelSpeed, short *RB_WheelSpeed){

  //PARTIE DOSAGE DE LA VITESSE DES ROUES GAUCHE POUR LA ROTATION 
  
  //intervalle [0; 10[ et [350; 360] : la voiture avance tout droit
  if(    ((angle >= STRIGHT_MIN_LIMIT_LEFT)&&(angle < STRIGHT_MAX_LIMIT_LEFT))
      || ((angle >= STRIGHT_MIN_LIMIT_RIGHT)&&(angle <= STRIGHT_MAX_LIMIT_RIGHT)) ){
    *LF_WheelSpeed = instantSpeed;
    *LB_WheelSpeed = instantSpeed;
    *RF_WheelSpeed = instantSpeed;
    *RB_WheelSpeed = instantSpeed;
  }
  //intervalle [10; 40[ : la voiture tourne doucement vers la gauche
  //On diminue la vitesse de la roue avant gauche en fonction de la vitesse instantané actuelle
  else if((angle >=STOP_LEFT_FRONT_WHEEL_MIN_LIMIT) && (angle <STOP_LEFT_FRONT_WHEEL_MAX_LIMIT)){
    *LF_WheelSpeed = instantSpeed - map(angle, STOP_LEFT_FRONT_WHEEL_MIN_LIMIT, STOP_LEFT_FRONT_WHEEL_MAX_LIMIT, MIN_SPEED_WHEEL, instantSpeed);
    *LB_WheelSpeed = instantSpeed;
    *RF_WheelSpeed = instantSpeed;
    *RB_WheelSpeed = instantSpeed;
  }
  //intervalle [40; 90[ : la voiture tourne plus vite vers la gauche 
  //On arret la rotation de la roue avant gauche et on diminue la vitesse de la roue arrière gauche en fonction de la vitesse instantané actuelle
  else if((angle >=STOP_LEFT_BACK_WHEEL_MIN_LIMIT) && (angle <STOP_LEFT_BACK_WHEEL_MAX_LIMIT)){
    *LF_WheelSpeed = MIN_SPEED_WHEEL;
    *LB_WheelSpeed = instantSpeed - map(angle, STOP_LEFT_BACK_WHEEL_MIN_LIMIT, STOP_LEFT_BACK_WHEEL_MAX_LIMIT, MIN_SPEED_WHEEL, instantSpeed);
    *RF_WheelSpeed = instantSpeed;
    *RB_WheelSpeed = instantSpeed;
  }
  //intervalle [90; 170[ : la voiture tourne aux max vers la gauche
  //On arret la rotation des deux roues gauches
  else if((angle >=STOP_LEFT_FRONT_BACK_WHEEL_MIN_LIMIT) && (angle <STOP_LEFT_FRONT_BACK_WHEEL_MAX_LIMIT)){
    *LF_WheelSpeed = MIN_SPEED_WHEEL;
    *LB_WheelSpeed = MIN_SPEED_WHEEL;
    *RF_WheelSpeed = instantSpeed;
    *RB_WheelSpeed = instantSpeed;
  }

  //PARTIE ARRET DE LA VOITURE

  //intervalle [170; 190[ : la voiture sarret
  //On arret la rotation de tous les roues
  else if((angle >=STOP_CAR_MIN_LIMIT) && (angle <STOP_CAR_MAX_LIMIT)){
    *LF_WheelSpeed = MIN_SPEED_WHEEL;
    *LB_WheelSpeed = MIN_SPEED_WHEEL;
    *RF_WheelSpeed = MIN_SPEED_WHEEL;
    *RB_WheelSpeed = MIN_SPEED_WHEEL;
  }
  
  //PARTIE DOSAGE DE LA VITESSE DES ROUES DROITE POUR LA ROTATION 

  //intervalle [190; 270[ : la voiture tourne aux max vers la droite 
  //On arret la rotation des deux roues droites
  else if((angle >= STOP_RIGHT_FRONT_BACK_WHEEL_MIN_LIMIT) && (angle < STOP_RIGHT_FRONT_BACK_WHEEL_MAX_LIMIT)){
    *LF_WheelSpeed = instantSpeed;
    *LB_WheelSpeed = instantSpeed;
    *RF_WheelSpeed = MIN_SPEED_WHEEL;
    *RB_WheelSpeed = MIN_SPEED_WHEEL;
  }
  //intervalle [270; 310[ : la voiture tourne plus vite vers la droite 
  //On arret la rotation de la roue avant droite et on diminue la vitesse de la roue arrière droit en fonction de la vitesse instantané actuelle
  else if((angle >= STOP_RIGHT_BACK_WHEEL_MIN_LIMIT) && (angle < STOP_RIGHT_BACK_WHEEL_MAX_LIMIT)){
    *LF_WheelSpeed = instantSpeed;
    *LB_WheelSpeed = instantSpeed ;
    *RF_WheelSpeed = MIN_SPEED_WHEEL;
    *RB_WheelSpeed = map(angle, STOP_RIGHT_BACK_WHEEL_MIN_LIMIT, STOP_RIGHT_BACK_WHEEL_MAX_LIMIT, MIN_SPEED_WHEEL, instantSpeed);
  }
  //intervalle [310; 350[ : la voiture tourne doucement vers la droite
  //On diminue la vitesse de la roue avant droite en fonction de la vitesse instantané actuelle
  else if((angle >= STOP_RIGHT_FRONT_WHEEL_MIN_LIMIT) && (angle < STOP_RIGHT_FRONT_WHEEL_MAX_LIMIT)){
    *LF_WheelSpeed = instantSpeed;
    *LB_WheelSpeed = instantSpeed ;
    *RF_WheelSpeed = map(angle, STOP_RIGHT_FRONT_WHEEL_MIN_LIMIT, STOP_RIGHT_FRONT_WHEEL_MAX_LIMIT, MIN_SPEED_WHEEL, instantSpeed);
    *RB_WheelSpeed = instantSpeed;
  }
}
