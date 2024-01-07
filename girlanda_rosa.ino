#include <EEPROM.h>
int num = 0;

bool flag = false;
uint32_t btnTimer = 0;

const int ledCount = 6;                                   // Количество светодиодов
const int ledPins[ledCount] = { 3, 5, 6, 9, 10, 11 };  // Пины светодиодов
unsigned long previousMillis1 = 0;                        // Время последнего переключения
const unsigned long interval1 = 500;                      // Интервал в мс между переключениями
int currentLed = 0;

unsigned long previousMillis2 = 0;    // Время последнего переключения
const unsigned long interval3 = 500;  // Интервал в мс между переключениями
int currentLed1 = 0;
bool allOn1 = true;

bool allOn = true;
const unsigned long interval2 = 100;

int brightness = 0;                // начальное значение яркости
int fadeAmount = 1;                // величина изменения яркости за один шаг
unsigned long previousMillis = 0;  // хранит последнее время обновления яркости
const long interval = 10;
int button;

unsigned long time = 0;

int j = 0;
int jj = 255;
unsigned long timer = 0;

int i1 = 0;
int j1 = 255;

const long updateInterval = 10;  // Интервал обновления яркости в миллисекундах
unsigned long lastUpdate = 0;    // Время последнего обновления

// Переменные состояния и яркости для каждого пина
int brightness1[6] = {  0, 0, 0, 0, 0, 0 };
int increment[6] = {  1, 1, 1, 1, 1, 1 };
int pinIndices[6] = { 3, 5, 6, 9, 10, 11 };                      // Пины, к которым подключены светодиоды
bool increasing[6] = {  true, true, true, true, true, true };  // Состояние увеличения яркости светодиода
int activePin = 0;                                                  // Текущий активный пин


#include <NecDecoder.h>
NecDecoder ir;

void setup() {
  Serial.begin(9600);
  // подключил на D2, прерывание 0
  attachInterrupt(0, irIsr, FALLING);

  pinMode(8, INPUT_PULLUP);
  for (int i = 3; i <= 11; i++) {
    pinMode(i, OUTPUT);
  }
  num = EEPROM.read(0);
}
// в прерывании вызываем tick()
void irIsr() {
  ir.tick();
}
void ef0() {  //плавное затухание и включение

  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    analogWrite(3, brightness);
    analogWrite(5, brightness);
    analogWrite(6, brightness);
    analogWrite(9, brightness);
    analogWrite(10, brightness);
    analogWrite(11, brightness);
    
    brightness = brightness + fadeAmount;
    if (brightness <= 0 || brightness >= 255) {
      fadeAmount = -fadeAmount;
    }
  }
}

void ef1() {  //постоянное горение
  digitalWrite(5, HIGH);
  digitalWrite(6, HIGH);
  digitalWrite(3, HIGH);
  digitalWrite(9, HIGH);
  digitalWrite(10, HIGH);
  digitalWrite(11, HIGH);
}
void ef2() {  //поочередное включение
  unsigned long currentMillis1 = millis();

  if (currentMillis1 - previousMillis1 >= interval1) {
    previousMillis1 = currentMillis1;

    // Выключаем предыдущий светодиод
    digitalWrite(ledPins[currentLed], LOW);

    // Переходим к следующему светодиоду
    currentLed = (currentLed + 1) % ledCount;

    // Включаем текущий светодиод
    digitalWrite(ledPins[currentLed], HIGH);
  }
}
void ef3() {  //круговая
  unsigned long currentMillis2 = millis();

  if (currentMillis2 - previousMillis1 >= interval2) {
    previousMillis1 = currentMillis2;

    if (allOn) {
      // Зажигаем следующий светодиод
      digitalWrite(ledPins[currentLed], HIGH);

      currentLed = (currentLed + 1) % ledCount;

      // Проверяем, завершилось ли зажигание всех светодиодов
      if (currentLed == 0) {
        allOn = false;
      }
    } else {
      // Тушим текущий светодиод
      digitalWrite(ledPins[currentLed], LOW);

      currentLed = (currentLed + 1) % ledCount;

      // Проверяем, завершилось ли тушение всех светодиодов
      if (currentLed == 0) {
        allOn = true;
      }
    }
  }
}
void ef4() {  //мигалка
  unsigned long now = millis();

  // Восходящий PWM от 0 до 255
  if (now - time >= 1) {
    time = now;

    static int i = 0;
    static int dir = 1;
    i += dir;
    analogWrite(3, i);
    analogWrite(5, 255 - i);
    analogWrite(6, i);
    analogWrite(9, 255 - i);
    analogWrite(10, i);
    analogWrite(11, 255 - i);

    if (i <= 0 || i >= 255) {
      dir *= -1;
    }
  }
}
void ef5() {  //круговая медленная
  unsigned long currentMillis = millis();

  // Проверяем, пришло ли время обновления
  if (currentMillis - lastUpdate >= updateInterval) {
    lastUpdate = currentMillis;  // Сброс таймера

    // Обновление яркости для текущего активного пина
    analogWrite(pinIndices[activePin], brightness1[activePin]);

    // Изменяем яркость для следующего шага
    if (increasing[activePin]) {
      // Если яркость увеличивается
      brightness1[activePin] += increment[activePin];
      if (brightness1[activePin] >= 255) {
        // Достижение максимума, начинаем убавление
        increasing[activePin] = false;
        activePin = (activePin + 1) % 6;  // Переход к следующему пину
      }
    } else {
      // Если яркость уменьшается
      brightness1[activePin] -= increment[activePin];
      if (brightness1[activePin] <= 0) {
        // Достижение минимума, начинаем увеличение
        increasing[activePin] = true;
        activePin = (activePin + 1) % 6;  // Переход к следующему пину
      }
    }
  }
}

void loop() {
  // если пакет успешно принят
  if (num>=6 || num<=0) num=0;

  if (ir.available()) { //обработка кнопок пульта 
    switch(ir.readCommand()){
      case 0x30: num=0;break; //1
      case 0x18: num=1;break; //2
      case 0x7A: num=2;break; //3
      case 0x10: num=3;break; //4
      case 0x38: num=4;break; //5
      case 0x5A: num=5;break; //6
      case 0xA8: num++;break; //+
      case 0xE0: num--;break; //-
    }
    Serial.println(num);
  }
  /*-----------------------------------кнопка------------------------------------*/
   bool btnState = !digitalRead(8); 
  if (btnState && !flag && millis() - btnTimer > 100) {
    flag = true;
    btnTimer = millis();
    num++;
    if (num >= 6) num = 0;
    Serial.print("press");
    Serial.println(num);
    EEPROM.update(0, num);
    digitalWrite(5, LOW);
    digitalWrite(6, LOW);
    digitalWrite(3, LOW);
    digitalWrite(9, LOW);
    digitalWrite(10, LOW);
    digitalWrite(11, LOW);
  }
  if (!btnState && flag && millis() - btnTimer > 100) {
    flag = false;
    btnTimer = millis();
    //Serial.println("release");
  }
  /*-----------------------------------кнопка------------------------------------*/
  /*-----------------------------------------------------------------------------*/
  if (num == 0) ef0();
  /*-----------------------------------------------------------------------------*/
  if (num == 1) ef1();
  /*-----------------------------------------------------------------------------*/
  if (num == 2) ef2();
  /*-----------------------------------------------------------------------------*/
  if (num == 3) ef3();
  /*-----------------------------------------------------------------------------*/
  if (num == 4) ef4();
  /*-----------------------------------------------------------------------------*/
  if (num == 5) ef5();
  /*-----------------------------------------------------------------------------*/

  
}
