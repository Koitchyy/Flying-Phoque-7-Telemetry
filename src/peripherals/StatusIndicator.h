#ifndef STATUS_INDICATOR_H
#define STATUS_INDICATOR_H

class StatusIndicator {
public:
    StatusIndicator(int redPin, int greenPin, int bluePin);

    enum Color {
        RED,
        GREEN,
        BLUE,
        ORANGE,
        WHITE,
        OFF
    };

    Color currentColor;

    void flash(Color color, int durationMs);
    void solid(Color color);
    void off();

private:
    int redPin;
    int greenPin;
    int bluePin;

    int durationMs;
};

#endif