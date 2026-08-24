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
    double pixelsPerUnit = 100.0,
    double lineWidth = 0.02
) {
    if (circles.empty())
        return;

    // Center of mass, weighting each circle by its area.
    double totalMass = 0.0;
    double comX = 0.0;
    double comY = 0.0;

    for (const Circle& c : circles) {
        double mass = c.r * c.r;

        totalMass += mass;
        comX += c.x * mass;
        comY += c.y * mass;
    }

    comX /= totalMass;
    comY /= totalMass;

    // Find bounds relative to COM.
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

    const double padding = 1.0;

    minX -= padding;
    maxX += padding;
    minY -= padding;
    maxY += padding;

    const int width =
        static_cast<int>(std::ceil((maxX - minX) * pixelsPerUnit));

    const int height =
        static_cast<int>(std::ceil((maxY - minY) * pixelsPerUnit));

    const int rowStride = (width * 3 + 3) & ~3;

    // White background.
    std::vector<uint8_t> pixels(
        static_cast<size_t>(rowStride) * height,
        255
    );

    // Half the border thickness.
    const double halfWidth = lineWidth * 0.5;

    for (const Circle& c : circles) {
        const double cx = c.x - comX;
        const double cy = c.y - comY;

        const int x0 = std::max(
            0,
            static_cast<int>(
                std::floor((cx - c.r - halfWidth - minX) * pixelsPerUnit)
            )
        );

        const int x1 = std::min(
            width - 1,
            static_cast<int>(
                std::ceil((cx + c.r + halfWidth - minX) * pixelsPerUnit)
            )
        );

        const int y0 = std::max(
            0,
            static_cast<int>(
                std::floor(
                    (maxY - (cy + c.r + halfWidth)) * pixelsPerUnit
                )
            )
        );

        const int y1 = std::min(
            height - 1,
            static_cast<int>(
                std::ceil(
                    (maxY - (cy - c.r - halfWidth)) * pixelsPerUnit
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

                // Only draw the circumference.
                if (std::abs(distance - c.r) <= halfWidth) {
                    const size_t index =
                        static_cast<size_t>(py) * rowStride +
                        static_cast<size_t>(px) * 3;

                    // Black border, white interior.
                    pixels[index + 0] = 0; // B
                    pixels[index + 1] = 0; // G
                    pixels[index + 2] = 0; // R
                }
            }
        }
    }

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
#endif //PRIMECIRCLEPACKING_VIEW_H
