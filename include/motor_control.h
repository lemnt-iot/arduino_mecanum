#ifndef MOTOR_CONTROL_H
#define MOTOR_CONTROL_H

#define MOTOR_EN_A_PIN 10
#define MOTOR_EN_B_PIN 11

#define MOTOR_IN1_LA_PIN 2
#define MOTOR_IN2_LA_PIN 3

#define MOTOR_IN1_RA_PIN 4
#define MOTOR_IN2_RA_PIN 5

#define MOTOR_IN1_LB_PIN 6
#define MOTOR_IN2_LB_PIN 7

#define MOTOR_IN1_RB_PIN 8
#define MOTOR_IN2_RB_PIN 9

/* 定义五种运动状态 */
enum class CarCommand {
    STOP,
    FORWARD,
    BACKWARD,
    ROTATE_CLOCKWISE,
    ROTATE_COUNTERCLOCKWISE,
    TRANSLATION_LEFT,
    TRANSLATION_RIGHT
};

constexpr int maxSpeed = 255; // 定义最大速度
constexpr int minSpeed = 0; // 定义最小速度
constexpr int speedStep = 10; // 定义速度步长

class MotorControl {
public:
    static void init();

    void move(CarCommand cmd) const;

    void adjust_speed(int adjustment);

    static void motor_speed(int speed);

private:
    static void motor_pins(int in1_LA, int in2_LA, int in1_LB, int in2_LB, int in1_RA, int in2_RA, int in1_RB,
                           int in2_RB);

    int carSpeed = 128; // 默认速度为 128，可动态调整
};

#endif //MOTOR_CONTROL_H
