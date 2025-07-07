int redLed = 6;
int greenLed = 7;
int pButton1 = 8;
int pButton2 = 9;
int pButton3 = 10;
int pButton4 = 11;

const int correctSequence[] = {1, 3, 4, 2};
const int sequenceLength = 4;
int step = 0;
unsigned long startTime = 0;
bool sequenceStarted = false;
bool sequenceFailed = false;

void setup() {
  pinMode(redLed, OUTPUT);
  pinMode(greenLed, OUTPUT);
  pinMode(pButton1, INPUT);
  pinMode(pButton2, INPUT);
  pinMode(pButton3, INPUT);
  pinMode(pButton4, INPUT);
  
  Serial.begin(9600);
}

void readInput() {
  if (!sequenceStarted) {
    startTime = millis(); 
    sequenceStarted = true;
    sequenceFailed = false;
    Serial.println("Sequence started! You have 10 seconds.");
  }
  
  unsigned long currentTime = millis();
  
  if (currentTime - startTime > 10000) {
    if (sequenceFailed || step < sequenceLength) {
      Serial.println("Timeout or incorrect sequence!");
      handleFailure();
    }
    return;
  }

  if (sequenceFailed) {
    return;
  }

  switch(step) {
    case 0:
      if (digitalRead(pButton1)) {
        step++;
        Serial.println("Step 1 correct!");
        delay(200);
      } 
      else if (digitalRead(pButton2) || digitalRead(pButton3) || digitalRead(pButton4)) {
        Serial.println("Wrong button at step 1!");
        sequenceFailed = true;
      }
      break;
      
    case 1:
      if (digitalRead(pButton3)) {
        step++;
        Serial.println("Step 2 correct!");
        delay(200);
      } 
      else if (digitalRead(pButton1) || digitalRead(pButton2) || digitalRead(pButton4)) {
        Serial.println("Wrong button at step 2!");
        sequenceFailed = true;
      }
      break;
      
    case 2:
      if (digitalRead(pButton4)) {
        step++;
        Serial.println("Step 3 correct!");
        delay(200);
      } 
      else if (digitalRead(pButton1) || digitalRead(pButton2) || digitalRead(pButton3)) {
        Serial.println("Wrong button at step 3!");
        sequenceFailed = true;
      }
      break;
      
    case 3:
      if (digitalRead(pButton2)) {
        step++;
        Serial.println("Step 4 correct!");
        handleSuccess();
        delay(200);
      } 
      else if (digitalRead(pButton1) || digitalRead(pButton3) || digitalRead(pButton4)) {
        Serial.println("Wrong button at step 4!");
        sequenceFailed = true;
      }
      break;
  }
}

void handleSuccess() {
  digitalWrite(greenLed, HIGH);
  Serial.println("SUCCESS! Access granted.");
  delay(3000);
  digitalWrite(greenLed, LOW);
  resetLock();
}

void handleFailure() {
  Serial.println("FAILURE! Access denied.");
  for (int i = 0; i < 3; i++) {
    digitalWrite(redLed, HIGH);
    delay(500);
    digitalWrite(redLed, LOW);
    delay(500);
  }
  resetLock();
}

void resetLock() {
  step = 0;
  sequenceStarted = false;
  sequenceFailed = false;
  Serial.println("System reset. Ready for new input.");
  Serial.println();
}

void loop() {
  readInput();
}