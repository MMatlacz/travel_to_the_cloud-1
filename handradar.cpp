#define trigPin 8
#define echoPin 9
#define ledPin 7
#define vibPin 10
#define buzzPin 11

#define vibPower 225
#define buzzPower 50
#define buzzTime 50

int T[10];
int next_buzz = 0;
int buzz_end = 0;
int last_end = 0;

void setup() {
  pinMode(trigPin, OUTPUT); //Pin, do którego podłączymy trig jako wyjście
  pinMode(echoPin, INPUT); //a echo, jako wejście
  pinMode(ledPin, OUTPUT); //Pin silnika wibracyjnego
  pinMode(vibPin, OUTPUT); //Pin silnika wibracyjnego
  pinMode(buzzPin, OUTPUT); //Pin buzzera
  for(int i = 0; i < 10; ++i)
    T[i] = 0;
}

void on() {
  analogWrite(vibPin, vibPower);
  analogWrite(buzzPin, buzzPower);    
  digitalWrite(ledPin, HIGH);
}

void off() {
  analogWrite(vibPin, 0);
  analogWrite(buzzPin, 0);  
  digitalWrite(ledPin, LOW);
}

void buzz(int dist) {
  if(dist < 20)
    dist = 20;
  dist *= 2;
  int now = millis();
  if(buzz_end != 0) {
    if(now > buzz_end) {
      buzz_end = 0;
      off();           
    }
  } else if(now > next_buzz) {
    last_end = buzz_end = now + buzzTime;
    next_buzz = buzz_end + dist;
    on();
  } else if(last_end != 0 && last_end + dist < now) {
    last_end = buzz_end = now + buzzTime;
    next_buzz = buzz_end + dist;
    on();    
  }
}
 
void loop() {
  long czas, dystans;
 
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
 
  czas = pulseIn(echoPin, HIGH, 15000);
  dystans = czas / 58;
  if(dystans > 300 || dystans == 0)
    dystans = 300;
  

  for(int i = 9; i > 0; --i)
    T[i] = T[i - 1];
  T[0] = dystans;
  int mdist = 300;
  for(int i = 0; i < 10; ++i)
    if(T[i] < mdist && T[i] != 0)
      mdist = T[i];
  buzz(mdist);  
}    
