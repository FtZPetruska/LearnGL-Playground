#include "Utils.hpp"

#include <fmt/core.h>
#include <stb_image.h>

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

Image::Image(const std::filesystem::path &img_path) noexcept
{
    stbi_set_flip_vertically_on_load(true);
    data = {0, 0, 0, nullptr};
    if (!std::filesystem::is_regular_file(img_path)) {
        fmt::print(stderr, "Image: Path '{}' is not a file.\n", img_path.string());
        return;
    }
    data.pixels = stbi_load(img_path.c_str(), &data.width, &data.height, &data.channels, 0);
}

Image::~Image() noexcept
{
    if (nullptr != data.pixels) {
        stbi_image_free(data.pixels);
    }
}

const ImageData &
Image::getImageData(void) const noexcept
{
    return data;
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