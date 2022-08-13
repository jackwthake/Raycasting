void setup() {
    pinMode(A0, OUTPUT);
}

void loop() {
    digitalWrite(A0, HIGH);
    delay(200);
    digitalWrite(A0, LOW);
    delay(200);
    digitalWrite(A0, HIGH);
    delay(200);
    digitalWrite(A0, LOW);
    delay(1000);
}