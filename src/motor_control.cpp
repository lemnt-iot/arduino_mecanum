#include <Arduino.h>

#include "motor_control.h"

/* 电机引脚初始化 */
void MotorControl::init() {
    pinMode(MOTOR_EN_A_PIN, OUTPUT);
    pinMode(MOTOR_EN_B_PIN, OUTPUT);

    pinMode(MOTOR_IN1_LA_PIN, OUTPUT);
    pinMode(MOTOR_IN2_LA_PIN, OUTPUT);

    pinMode(MOTOR_IN1_LB_PIN, OUTPUT);
    pinMode(MOTOR_IN2_LB_PIN, OUTPUT);

    pinMode(MOTOR_IN1_RA_PIN, OUTPUT);
    pinMode(MOTOR_IN2_RA_PIN, OUTPUT);

    pinMode(MOTOR_IN1_RB_PIN, OUTPUT);
    pinMode(MOTOR_IN2_RB_PIN, OUTPUT);

    // 停止电机，确保所有引脚初始为 LOW
    motor_pins(LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW);
}

/* 设置电机引脚状态 */
void MotorControl::motor_pins(const int in1_LA, const int in2_LA, const int in1_LB, const int in2_LB, const int in1_RA,
                              const int in2_RA, const int in1_RB, const int in2_RB) {
    digitalWrite(MOTOR_IN1_LA_PIN, in1_LA);
    digitalWrite(MOTOR_IN2_LA_PIN, in2_LA);
    digitalWrite(MOTOR_IN1_LB_PIN, in1_LB);
    digitalWrite(MOTOR_IN2_LB_PIN, in2_LB);
    digitalWrite(MOTOR_IN1_RA_PIN, in1_RA);
    digitalWrite(MOTOR_IN2_RA_PIN, in2_RA);
    digitalWrite(MOTOR_IN1_RB_PIN, in1_RB);
    digitalWrite(MOTOR_IN2_RB_PIN, in2_RB);
}

/* 电机控制 */
void MotorControl::move(const CarCommand cmd) const {
    switch (cmd) {
        case CarCommand::FORWARD:
            motor_pins(HIGH, LOW, HIGH, LOW,HIGH, LOW,HIGH, LOW);
            Serial.println("FORWARD");
            break;
        case CarCommand::BACKWARD:
            motor_pins(LOW, HIGH, LOW, HIGH,LOW, HIGH, LOW, HIGH);
            Serial.println("BACKWARD");
            break;
        case CarCommand::ROTATE_CLOCKWISE:
            /*顺时针原地旋转*/
            motor_pins(HIGH, LOW, HIGH, LOW,LOW, HIGH, LOW, HIGH);
            Serial.println("ROTATE_CLOCKWISE");
            break;
        case CarCommand::ROTATE_COUNTERCLOCKWISE:
            /*逆时针原地旋转*/
            motor_pins(LOW, HIGH, LOW, HIGH,HIGH, LOW, HIGH, LOW);
            Serial.println("ROTATE_COUNTERCLOCKWISE");
            break;
        case CarCommand::TRANSLATION_LEFT:
            /*左边平移*/
            motor_pins(LOW, HIGH, HIGH, LOW, HIGH, LOW, LOW, HIGH);
            Serial.println("TURN_LEFT");
            break;
        case CarCommand::TRANSLATION_RIGHT:
            /*右边平移*/
            motor_pins(HIGH, LOW, LOW, HIGH, LOW, HIGH, HIGH, LOW);
            Serial.println("TURN_RIGHT");
            break;
        default:
            motor_pins(LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW);
            Serial.println("STOP");
    }
    motor_speed(carSpeed);
}

void MotorControl::motor_speed(const int speed) {
    analogWrite(MOTOR_EN_A_PIN, speed);
    analogWrite(MOTOR_EN_B_PIN, speed);
}

/* 调整速度 */
void MotorControl::adjust_speed(const int adjustment) {
    carSpeed += adjustment;
    if (carSpeed > maxSpeed) carSpeed = maxSpeed;
    if (carSpeed < minSpeed) carSpeed = minSpeed;
    Serial.print("Adjusted carSpeed: ");
    Serial.println(carSpeed);
}
