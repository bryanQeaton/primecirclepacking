//vibecoded
#ifndef PRIMECIRCLEPACKING_VIEW_H
#define PRIMECIRCLEPACKING_VIEW_H

#include <vector>
#include <cmath>
#include <cstdint>
#include <fstream>
#include <algorithm>
#include "circles.h"

#pragma pack(push, 1)
struct BMPFileHeader {
    uint16_t type{0x4D42};
    uint32_t size{};
    uint16_t reserved1{};
    uint16_t reserved2{};
    uint32_t offset{54};
};

struct BMPInfoHeader {
    uint32_t size{40};
    int32_t  width{};
    int32_t  height{};
    uint16_t planes{1};
    uint16_t bitsPerPixel{24};
    uint32_t compression{};
    uint32_t imageSize{};
    int32_t  xPixelsPerMeter{};
    int32_t  yPixelsPerMeter{};
    uint32_t colorsUsed{};
    uint32_t colorsImportant{};
};
#pragma pack(pop)

void rasterizeCirclesBMP(
    const std::vector<Circle>& circles,
    const char* filename,
    int resolution,
    double lineThickness
) {
    if (circles.empty() || resolution <= 0)
        return;

    // ------------------------------------------------------------
    // Center the image around the area-weighted center of mass.
    // ------------------------------------------------------------

    double totalMass = 0.0;
    double comX = 0.0;
    double comY = 0.0;

    for (const Circle& c : circles) {
        const double mass = c.r * c.r;

        totalMass += mass;
        comX += c.x * mass;
        comY += c.y * mass;
    }

    comX /= totalMass;
    comY /= totalMass;

    // ------------------------------------------------------------
    // Find the bounding box relative to the center of mass.
    // ------------------------------------------------------------

    double minX = 0.0;
    double maxX = 0.0;
    double minY = 0.0;
    double maxY = 0.0;

    for (const Circle& c : circles) {
        minX = std::min(minX, c.x - c.r - comX);
        maxX = std::max(maxX, c.x + c.r - comX);
        minY = std::min(minY, c.y - c.r - comY);
        maxY = std::max(maxY, c.y + c.r - comY);
    }

    // ------------------------------------------------------------
    // Make the world bounds square.
    // ------------------------------------------------------------

    const double padding = 1.0;

    minX -= padding;
    maxX += padding;
    minY -= padding;
    maxY += padding;

    const double worldWidth  = maxX - minX;
    const double worldHeight = maxY - minY;

    const double worldSize =
        std::max(worldWidth, worldHeight);

    // Center the shorter dimension inside the square.
    const double centerX = (minX + maxX) * 0.5;
    const double centerY = (minY + maxY) * 0.5;

    minX = centerX - worldSize * 0.5;
    maxX = centerX + worldSize * 0.5;
    minY = centerY - worldSize * 0.5;
    maxY = centerY + worldSize * 0.5;

    // ------------------------------------------------------------
    // Pixel scale.
    // ------------------------------------------------------------

    const double pixelsPerUnit =
        static_cast<double>(resolution) / worldSize;

    const int width = resolution;
    const int height = resolution;

    const int rowStride = (width * 3 + 3) & ~3;

    // White background.
    std::vector<uint8_t> pixels(
        static_cast<size_t>(rowStride) * height,
        255
    );

    const double halfWidth = lineThickness * 0.5;

    // ------------------------------------------------------------
    // Rasterize circle circumferences.
    // ------------------------------------------------------------

    for (const Circle& c : circles) {
        const double cx = c.x - comX;
        const double cy = c.y - comY;

        const int x0 = std::max(
            0,
            static_cast<int>(
                std::floor(
                    (cx - c.r - halfWidth - minX) *
                    pixelsPerUnit
                )
            )
        );

        const int x1 = std::min(
            width - 1,
            static_cast<int>(
                std::ceil(
                    (cx + c.r + halfWidth - minX) *
                    pixelsPerUnit
                )
            )
        );

        const int y0 = std::max(
            0,
            static_cast<int>(
                std::floor(
                    (maxY - (cy + c.r + halfWidth)) *
                    pixelsPerUnit
                )
            )
        );

        const int y1 = std::min(
            height - 1,
            static_cast<int>(
                std::ceil(
                    (maxY - (cy - c.r - halfWidth)) *
                    pixelsPerUnit
                )
            )
        );

        for (int py = y0; py <= y1; ++py) {
            const double worldY =
                maxY - (py + 0.5) / pixelsPerUnit;

            for (int px = x0; px <= x1; ++px) {
                const double worldX =
                    minX + (px + 0.5) / pixelsPerUnit;

                const double dx = worldX - cx;
                const double dy = worldY - cy;

                const double distance =
                    std::sqrt(dx * dx + dy * dy);

                if (std::abs(distance - c.r) <= halfWidth) {
                    const size_t index =
                        static_cast<size_t>(py) * rowStride +
                        static_cast<size_t>(px) * 3;

                    // BMP stores pixels as BGR.
                    pixels[index + 0] = 0;
                    pixels[index + 1] = 0;
                    pixels[index + 2] = 0;
                }
            }
        }
    }

    // ------------------------------------------------------------
    // Write BMP.
    // ------------------------------------------------------------

    BMPFileHeader fileHeader;
    BMPInfoHeader infoHeader;

    infoHeader.width = width;
    infoHeader.height = height;
    infoHeader.imageSize =
        static_cast<uint32_t>(pixels.size());

    fileHeader.size =
        sizeof(BMPFileHeader) +
        sizeof(BMPInfoHeader) +
        static_cast<uint32_t>(pixels.size());

    std::ofstream file(filename, std::ios::binary);

    if (!file)
        return;

    file.write(
        reinterpret_cast<const char*>(&fileHeader),
        sizeof(fileHeader)
    );

    file.write(
        reinterpret_cast<const char*>(&infoHeader),
        sizeof(infoHeader)
    );

    file.write(
        reinterpret_cast<const char*>(pixels.data()),
        static_cast<std::streamsize>(pixels.size())
    );
}

#endif // PRIMECIRCLEPACKING_VIEW_H