#include <PS2x_lib.h>

#include "ps2x_control.h"
#include "motor_control.h"

int error = 0;
int type = 0;
int vibrate = 0;

PS2X ps2x;
MotorControl motor_control;

/* ps手柄引脚初始化 */
void PS2xControl::init() {
    error = ps2x.config_gamepad(PS2_CLK_PIN, PS2_CMD_PIN, PS2_SEL_PIN, PS2_DAT_PIN, pressures, rumble);
    if (error == 0) {
        Serial.print("Found Controller, configured successful ");
        Serial.print("pressures = ");
        if (pressures) {
            Serial.println("true ");
        } else {
            Serial.println("false");
            Serial.print("rumble = ");
        }
        if (rumble) {
            Serial.println("true)");
        } else {
            Serial.println("false");
            Serial.println("Try out all the buttons, X will vibrate the controller, faster as you press harder;");
            Serial.println("holding L1 or R1 will print out the analog stick values.");
            Serial.println("Note: Go to www.billporter.info for updates and to report bugs.");
        }
    } else if (error == 1) {
        Serial.println(
            "No controller found, check wiring, see readme.txt to enable debug. visit www.billporter.info for troubleshooting tips");
    } else if (error == 2) {
        Serial.println(
            "Controller found but not accepting commands. see readme.txt to enable debug. Visit www.billporter.info for troubleshooting tips");
    } else if (error == 3) {
        Serial.println("Controller refusing to enter Pressures mode, may not support it. ");
    }
    type = ps2x.readType();
    if (type == 1) {
        Serial.println("DualShock Controller found.");
    } else if (type == 3) {
        Serial.println("Wireless Sony DualShock Controller found.");
    } else {
        Serial.println("Unknown controller type.");
    }
}

/* PS2控制器主逻辑 */
void PS2xControl::controller() {
    if (type == 2) {
        Serial.println("Guitar Hero Controller");
    } else {
        ps2x.read_gamepad(false, vibrate);
        /* 优先使用按钮控制小车移动 */
        if (ps2x.Button(PSB_PAD_UP) || ps2x.Button(PSB_PAD_DOWN) || ps2x.Button(PSB_PAD_LEFT) || ps2x.Button(
                PSB_PAD_RIGHT)) {
            move_car(); /* 按钮控制 */
        } else {
            left_rocker(); /* 左摇杆控制 */
        }

        /* L2和R2调整速度 */
        if (ps2x.Button(PSB_L2)) {
            motor_control.adjust_speed(speedStep);
        } else if (ps2x.Button(PSB_R2)) {
            motor_control.adjust_speed(-speedStep);
        }
        /* 调用震动功能，力度由X按键模拟量控制 */
        vibrate = ps2x.Analog(PSAB_CROSS);
    }
    delay(50); /* 稍作延迟，防止过多指令发送 */
}

/* 控制车移动 */
void PS2xControl::move_car() {
    if (ps2x.Button(PSB_PAD_UP)) {
        motor_control.move(CarCommand::FORWARD);
    } else if (ps2x.Button(PSB_PAD_DOWN)) {
        motor_control.move(CarCommand::BACKWARD);
    } else if (ps2x.Button(PSB_PAD_LEFT)) {
        motor_control.move(CarCommand::TRANSLATION_LEFT);
    } else if (ps2x.Button(PSB_PAD_RIGHT)) {
        motor_control.move(CarCommand::TRANSLATION_RIGHT);
    } else {
        motor_control.move(CarCommand::STOP);
    }
}

/* 优化左摇杆控制：根据摇杆位置调整速度 */
void PS2xControl::left_rocker() {
    const int Y1 = ps2x.Analog(PSS_LY); /* 获取左摇杆Y轴 */
    const int X1 = ps2x.Analog(PSS_LX); /* 获取左摇杆X轴 */
    /* 根据摇杆的倾斜程度动态调整车速 */
    const int dynamicSpeed = static_cast<int>(
        map(abs(Y1 - 128), 0, 128, minSpeed, maxSpeed));
    if (Y1 < 5 && X1 > 80 && X1 < 180) {
        motor_control.move(CarCommand::FORWARD);
        MotorControl::motor_speed(dynamicSpeed);
    } else if (Y1 > 230 && X1 > 80 && X1 < 180) {
        motor_control.move(CarCommand::BACKWARD);
        MotorControl::motor_speed(dynamicSpeed);
    } else if (X1 < 5) {
        motor_control.move(CarCommand::TRANSLATION_LEFT);
        MotorControl::motor_speed(dynamicSpeed);
    } else if (X1 > 230) {
        motor_control.move(CarCommand::TRANSLATION_RIGHT);
        MotorControl::motor_speed(dynamicSpeed);
    } else {
        motor_control.move(CarCommand::STOP); /* 摇杆未操作时停止 */
    }
}

/* 右摇杆控制 (未实现复杂功能，可以根据需求扩展) */
void PS2xControl::right_rocker() {
    const int Y2 = ps2x.Analog(PSS_RY);
    const int X2 = ps2x.Analog(PSS_RX);

    // 设置摇杆的中立范围，以避免传感器的噪声影响
    constexpr int centerMin = 110;
    constexpr int centerMax = 140;

    // 判断摇杆是否在中立位置
    if (Y2 > centerMin && Y2 < centerMax && X2 > centerMin && X2 < centerMax) {
        Serial.println("RightRocker in neutral position - Stop");
        // 添加停止设备的逻辑
    } else if (Y2 < centerMin) {
        Serial.println("RightRocker moving up");
        // 添加上移逻辑，例如：控制设备向前移动
    } else if (Y2 > centerMax) {
        Serial.println("RightRocker moving down");
        // 添加下移逻辑，例如：控制设备向后移动
    } else if (X2 < centerMin) {
        Serial.println("RightRocker moving left");
        // 添加左移逻辑，例如：控制设备左转
    } else if (X2 > centerMax) {
        Serial.println("RightRocker moving right");
        // 添加右移逻辑，例如：控制设备右转
    }
}
