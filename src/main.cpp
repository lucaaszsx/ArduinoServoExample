#include <Arduino.h>
#include <Servo.h>

#define BUTTON_PIN 8
#define BUTTON_CLICK_THRESHOLD 350

#define SERVO_THRESHOLD 15

void blink(uint8_t pin, unsigned long ms, size_t steps);
unsigned long elapsed();

bool active = false;
bool is_pressed = false;
size_t button_clicks = 0;
unsigned long last_click = 0;

Servo servo;
int servo_pos = 0;
bool servo_reverse = false;
unsigned long last_servo_tick = 0;

void setup() {
    pinMode(LED_BUILTIN, OUTPUT);
    pinMode(BUTTON_PIN, INPUT);

    servo.attach(9);
    servo.write(0);
}

void loop() {
    bool clicked = digitalRead(BUTTON_PIN) == HIGH;

    if (!is_pressed && clicked) {
        if (button_clicks > 0 && elapsed() <= BUTTON_CLICK_THRESHOLD) {
            button_clicks++;
        } else {
            button_clicks = 1;
        }
        
        is_pressed = true;
        last_click = millis();
    } else if (is_pressed && !clicked) {
        is_pressed = false;
    }

    if (button_clicks >= 2) {
        button_clicks = 0;
        last_click = 0;

        servo.write(servo_pos = 0);

        blink(LED_BUILTIN, 1000, 4);
    } else if (elapsed() > BUTTON_CLICK_THRESHOLD && button_clicks == 1) {
        active = !active;
        button_clicks = 0;
        last_click = 0;
    }

    digitalWrite(LED_BUILTIN, active);

    if (!active) return;
    if (millis() - last_servo_tick >= SERVO_THRESHOLD) {
        last_servo_tick = millis();

        servo_reverse = (!servo_reverse && servo_pos >= 180)
            || (servo_reverse && servo_pos > 0);
        servo_pos = servo_reverse ? servo_pos - 1 : servo_pos + 1;

        servo.write(servo_pos);
    }
}

bool read_button(uint8_t pin) {
    
}

void blink(uint8_t pin, unsigned long ms, size_t cycles) {
    unsigned long ms_per_step = ms / (cycles * 2);
    size_t step_counter = 0;

    while (true) {
        if (step_counter >= cycles) break;

        digitalWrite(pin, HIGH);
        delay(ms_per_step);
        digitalWrite(pin, LOW);
        delay(ms_per_step);

        step_counter++;
    }
}

unsigned long elapsed() {
    return millis() - last_click;
}
