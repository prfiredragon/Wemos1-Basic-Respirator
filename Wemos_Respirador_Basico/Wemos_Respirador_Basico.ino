
const int segundos_encendido = 4;
const int segundos_apagado = 4;

void setup() {

  pinMode(5, OUTPUT);
  digitalWrite(5, LOW);

}

void loop() {

  digitalWrite(5, LOW);
  delay(segundos_apagado*1000);                       // wait for a second
  digitalWrite(5, HIGH);
  delay(segundos_encendido*1000); 
}
