/*Le code est sale parcequ'il a été prévu
 * à la base pour une autre application
 * (un drone), raison pour laquelle il y a
 * les variables spin, etc...
 * 
 * Mais la fonction decodeSerial() reste utile 
 * pour récupérer le X et le Y (la flemme de la refaire).
 * 
 * La fonction laBite() permet de calculer les 
 * PWM a envoyer de chaque côté.
 * 
 * Les roues de gauche et de droite sont commandées
 * de la même manière. Ainsi, les pins de direction
 * vers le L298 sont mutualisées, mais les PWM sont 
 * indépendantes. Flemme strikes again.
 * 
 * La variable sensitivity sert à rien. tkt.
 * Le softwareSerial non plus. 
 * Du coup il faut débrancher le BT pour programmer, 
 * Sinon le Serial "part" sur le BT au lieu d'âtre lu
 * par la nano. 
 * 
 * Attention aux poulpes.
 */


#include <SoftwareSerial.h>

#define SENSITIVITY 1 //0 = immobile, 2 = très instable.

boolean On = false;
int input;
char work;
char work2;
char work3;
int X = 50;
int Y = 50;
int Pow = -30;
int offFront = 0;
int offRight = 0;
int spinRight = 0;
int mot1 = 3;
int mot2 = 5;
int mot3 = 6;
int mot4 = 9;
int intValueG = 0;
int intValueD = 0;
int calculated = 0;
int PWMG = 0;
int PWMD = 0;


void setup() {
  Serial.begin(9600);

  
  pinMode(2, OUTPUT); //Sens Gauche
  pinMode(3, OUTPUT); //Sens Gauche
  pinMode(7, OUTPUT); //Sens Droit
  pinMode(8, OUTPUT); //Sens Droit
  pinMode(5, OUTPUT); //PWM Gauche
  pinMode(6, OUTPUT); //PWM Gauche
  pinMode(9, OUTPUT); //PWM Droite
  pinMode(10, OUTPUT); //PWM Droite

  digitalWrite(2, HIGH);
  digitalWrite(3, LOW);
  digitalWrite(7, HIGH);
  digitalWrite(8, LOW);


}

void loop() {
  getSerial();
  laBite();
}


void getSerial() {
  if (Serial.available()) {
    input = Serial.read();
    decodeSerial();
  }
}

void decodeSerial() {
  work = (input >> 7) && 1;
  if ((work) != 0) {
    Y = (input - 128);
  }
  else {
    work2 = (input >> 4);
    if (work2 == 7) {
      work3 = input - 112;
      if (work3 == 1) {
        Pow++;
        //Serial.println("En haut !");
      }
      else if (work3 == 2) {
        Pow--;
        //Serial.println("En bas !");
      }
      else if (work3 == 3) {
        offFront--;
        //Serial.println("Derriere !");
      }
      else if (work3 == 4) {
        offFront++;
        //Serial.println("Devant !");
      }
      else if (work3 == 5) {
        offRight--;
        //Serial.println("A gauche !");
      }
      else if (work3 == 6) {
        offRight++;
        //Serial.println("A droite !");
      }
      else if (work3 == 7) {
        spinRight--;
        //Serial.println("Tourne a gauche !");
      }
      else if (work3 == 8) {
        spinRight++;
        //Serial.println("Tourne a droite !");
      }
      else if (work3 == 9) {
        On = 1 - On;
        //Serial.print("On = ");
        //Serial.println(On);
      }
      else Serial.println("WTF Bro ?");
    }
    else {
      X = (input);
    }
  }
}

void laBite(void)
{
  PWMD = constrain(map(2*(X-50)+2*(Y-50), 0, 100, 0, 254), -253, 254);
  PWMG = constrain(map(-2*(X-50)+2*(Y-50), 0, 100, 0, 254), -253, 254);
  
  analogWrite(5, abs(PWMG));
  analogWrite(6, abs(PWMG));
  analogWrite(9, abs(PWMD));
  analogWrite(10, abs(PWMD));

  if(PWMG < 0)
  {
  digitalWrite(2, LOW);
  digitalWrite(3, HIGH);
  }
  else
  {
  digitalWrite(2, HIGH);
  digitalWrite(3, LOW);
  }
  
  if(PWMD < 0)
  {
  digitalWrite(7, LOW);
  digitalWrite(8, HIGH);
  }
  else
  {
  digitalWrite(7, HIGH);
  digitalWrite(8, LOW);
  }
  
  
}

