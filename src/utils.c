#include "utils.h"

void
updateColours(Colour *colour)
{
    enum states_e
    {
        RED,
        GREEN,
        BLUE
    };
    static const float step = 0.01f;
    static enum states_e current_state = RED;
    switch (current_state) {
    case RED:
        colour->r -= step;
        colour->g += step;
        if (colour->r <= 0.0f) {
            current_state = GREEN;
        }
        break;
    case GREEN:
        colour->g -= step;
        colour->b += step;
        if (colour->g <= 0.0f) {
            current_state = BLUE;
        }
        break;
    case BLUE:
        colour->b -= step;
        colour->r += step;
        if (colour->b <= 0.0f) {
            current_state = RED;
        }
        break;
    }
}
