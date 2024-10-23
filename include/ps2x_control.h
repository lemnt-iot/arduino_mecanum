#ifndef PS2X_CONTROL_H
#define PS2X_CONTROL_H

#define PS2_DAT_PIN A3
#define PS2_CMD_PIN A2
#define PS2_SEL_PIN A1
#define PS2_CLK_PIN A0

#define pressures true
#define rumble true

extern int error;
extern int type;
extern int vibrate;

class PS2xControl {
public:
    static void init();

    static void controller();

    static void move_car();

    static void left_rocker();

    static void right_rocker();

private:
    static void process_rocker_input(int X, int Y);
};

#endif //PS2X_CONTROL_H
