#pragma once

#include <ranges>
#include "canvas.hpp"
#include "CIEcolor.hpp"
#include "renderPipeline.hpp"

void spectrum(Canvas& canvas) {
    const int width = canvas.width();
    const int height = canvas.height();

    for (int col = 0; col < width; ++col) {
        // Normalize column index to range [0,1]
        double normalizedValue = static_cast<double>(col) / (width - 1);

        // Convert normalized value to RGB color
        color v = spectralToRGB(normalizedValue);

        // Fill the entire column with the color
        for (int row = 0; row < height; ++row) {
            canvas.set_pixel(col, row, v);
        }
    }
}