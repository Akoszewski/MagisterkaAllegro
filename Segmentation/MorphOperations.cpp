#include "MorphOperations.h"

void ErodeMask(const Mask& mask, std::vector<std::vector<int>> structuringElement, int chosenLayerColorIdx)
{
    ALLEGRO_COLOR chosenLayerColor = mask.maskColors[chosenLayerColorIdx];
    for (int y = 0; y < mask.height; y++)
    {
        for (int x = 0; x < mask.width; x++)
        {
            ALLEGRO_COLOR readMaskColor = al_get_pixel(mask.bmp.get(), x, y);
            if (areColorsEqual(readMaskColor, chosenLayerColor)) {
                bool erodePixel = false;
                for (int i = 0; i < structuringElement.size(); i++)
                {
                    for (int j = 0; j < structuringElement[i].size(); j++)
                    {
                        int nx = x + i - structuringElement.size() / 2;
                        int ny = y + j - structuringElement[i].size() / 2;
                        if (nx >= 0 && nx < mask.width && ny >= 0 && ny < mask.height) {
                            ALLEGRO_COLOR maskPixel = al_get_pixel(mask.bmp.get(), nx, ny);

                            if (structuringElement[i][j] == 1 && !areColorsEqual(maskPixel, chosenLayerColor)) {
                                erodePixel = true;
                            }
                        }
                    }
                }
                if (erodePixel) {
                    al_put_pixel(x, y, mask.maskColors[chosenLayerColorIdx+1]);
                }
            }
        }
    }
}

void DilateMask(const Mask& mask, std::vector<std::vector<int>> structuringElement, int chosenLayerColorIdx)
{
    ALLEGRO_COLOR chosenLayerColor = mask.maskColors[chosenLayerColorIdx];
    for (int y = 0; y < mask.height; y++)
    {
        for (int x = 0; x < mask.width; x++)
        {
            ALLEGRO_COLOR readMaskColor = al_get_pixel(mask.bmp.get(), x, y);
            if (areColorsEqual(readMaskColor, chosenLayerColor)) {
                bool dilatePixel = false;
                for (int i = 0; i < structuringElement.size(); i++)
                {
                    for (int j = 0; j < structuringElement[i].size(); j++)
                    {
                        int nx = x + i - structuringElement.size() / 2;
                        int ny = y + j - structuringElement[i].size() / 2;
                        if (nx >= 0 && nx < mask.width && ny >= 0 && ny < mask.height) {
                            ALLEGRO_COLOR maskPixel = al_get_pixel(mask.bmp.get(), nx, ny);

                            if (structuringElement[i][j] == 1 && areColorsEqual(maskPixel, chosenLayerColor)) {
                                dilatePixel = true;
                            }
                        }
                    }
                }
                if (!dilatePixel) {
                    al_put_pixel(x, y, mask.maskColors[chosenLayerColorIdx+1]);
                }
            }
        }
    }
}

// std::unique_ptr<Image> erode(const Image& image, const std::vector<std::vector<int>>& structuringElement) {
//     std::unique_ptr<Image> erodedImage = std::make_unique<Image>(image.width, image.height);

//     for (int x = 0; x < image.width; x++) {
//         for (int y = 0; y < image.height; y++) {
//             bool erodePixel = false;
//             for (int i = 0; i < structuringElement.size(); i++) {
//                 for (int j = 0; j < structuringElement[i].size(); j++) {
//                     int nx = x + i - structuringElement.size() / 2;
//                     int ny = y + j - structuringElement[i].size() / 2;
//                     if (nx >= 0 && nx < image.width && ny >= 0 && ny < image.height) {
//                         if (structuringElement[i][j] == 1 && image.getPixel(nx, ny) == 0) {
//                             erodePixel = true;
//                         }
//                     }
//                 }
//             }
//             erodedImage->setPixel(x, y, erodePixel ? 0 : 1);
//         }
//     }
//     return erodedImage;
// }

// std::unique_ptr<Image> dilate(const Image& image, const std::vector<std::vector<int>>& structuringElement) {
//     std::unique_ptr<Image> dilatedImage = std::make_unique<Image>(image.width, image.height);

//     for (int x = 0; x < image.width; x++) {
//         for (int y = 0; y < image.height; y++) {
//             bool dilatePixel = false;
//             for (int i = 0; i < structuringElement.size(); i++) {
//                 for (int j = 0; j < structuringElement[i].size(); j++) {
//                     int nx = x + i - structuringElement.size() / 2;
//                     int ny = y + j - structuringElement[i].size() / 2;
//                     if (nx >= 0 && nx < image.width && ny >= 0 && ny < image.height) {
//                         if (structuringElement[i][j] == 1 && image.getPixel(nx, ny) == 1) {
//                             dilatePixel = true;
//                         }
//                     }
//                 }
//             }
//             dilatedImage->setPixel(x, y, dilatePixel ? 1 : 0);
//         }
//     }
//     return dilatedImage;
// }

// std::unique_ptr<Image> open(const Image& image, const std::vector<std::vector<int>>& structuringElement) {
//     return dilate(*erode(image, structuringElement), structuringElement);
// }
