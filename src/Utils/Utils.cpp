#include "Utils.hpp"

namespace Utils
{

RGBColour
ScrollingColour::getNext(void) noexcept
{
    UpdateColours();
    return getCurrent();
}

RGBColour
ScrollingColour::getCurrent(void) const noexcept
{
    return colour;
}

void
ScrollingColour::UpdateColours(void) noexcept
{
    static constexpr float STEP = 0.01f;
    switch (state) {
    case ScrollingState::RED:
        colour.r -= STEP;
        colour.g += STEP;
        if (colour.r <= 0.0f) {
            state = ScrollingState::GREEN;
        }
        break;
    case ScrollingState::GREEN:
        colour.g -= STEP;
        colour.b += STEP;
        if (colour.g <= 0.0f) {
            state = ScrollingState::BLUE;
        }
        break;
    case ScrollingState::BLUE:
        colour.b -= STEP;
        colour.r += STEP;
        if (colour.b <= 0.0f) {
            state = ScrollingState::RED;
        }
        break;
    }
}

}; // namespace Utils