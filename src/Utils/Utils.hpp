#ifndef UTILS_HPP
#define UTILS_HPP

#include <array>

namespace Utils
{

struct RGBColour
{
    float r, g, b;
};

class ScrollingColour
{
  public:
    RGBColour getNext(void) noexcept;
    RGBColour getCurrent(void) const noexcept;

  private:
    void UpdateColours(void) noexcept;
    enum class ScrollingState
    {
        RED,
        GREEN,
        BLUE
    };
    ScrollingState state = ScrollingState::RED;
    RGBColour colour = {1.0f, 0.0f, 0.0f};
};

template <class T, std::size_t N>
inline constexpr std::size_t
arrayDataSize(std::array<T, N> array)
{
    if (N == 0) {
        return 0;
    }
    return array.size() * sizeof array[0];
}

template <class T>

inline constexpr T
clamp(T value, T lower_bound, T higher_bound)
{
    if (value < lower_bound) {
        return lower_bound;
    }
    if (value > higher_bound) {
        return higher_bound;
    }
    return value;
}

}; // namespace Utils
#endif
