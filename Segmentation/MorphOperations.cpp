#include "MorphOperations.h"

void ErodeMask(Mask& mask, const std::vector<std::vector<int>>& structuringElement, 
               int chosenLayerColorIdx, int fillLayerColorIdx)
{
    ALLEGRO_COLOR chosenLayerColor = mask.maskColors[chosenLayerColorIdx];
    ALLEGRO_COLOR fillLayerColor = fillLayerColorIdx >= 0 ? mask.maskColors[fillLayerColorIdx] : chosenLayerColor;

    // Create a copy of the original mask
    ALLEGRO_BITMAP* originalMask = al_clone_bitmap(mask.bmp.get());

    for (int y = 0; y < mask.height; y++)
    {
        for (int x = 0; x < mask.width; x++)
        {
            bool erodePixel = false;
            for (int i = 0; i < structuringElement.size(); i++)
            {
                for (int j = 0; j < structuringElement[i].size(); j++)
                {
                    int nx = x + i - structuringElement.size() / 2;
                    int ny = y + j - structuringElement[i].size() / 2;

                    if (nx >= 0 && nx < mask.width && ny >= 0 && ny < mask.height)
                    {
                        if (structuringElement[i][j] == 1)
                        {
                            ALLEGRO_COLOR neighborColor = al_get_pixel(originalMask, nx, ny);
                            if (!areColorsEqual(neighborColor, chosenLayerColor))
                            {
                                erodePixel = true;
                                i = structuringElement.size(); // Break out of both loops
                                break;
                            }
                        }
                    }
                }
                if (!erodePixel) break; // Break out of outer loop if erodePixel is false
            }

            if (erodePixel) {
                al_put_pixel(x, y, fillLayerColor);
            }
        }
    }
    al_destroy_bitmap(originalMask);
}


void DilateMask(Mask& mask, const std::vector<std::vector<int>>& structuringElement, 
                int chosenLayerColorIdx, int fillLayerColorIdx)
{
    ALLEGRO_COLOR chosenLayerColor = mask.maskColors[chosenLayerColorIdx];
    ALLEGRO_COLOR fillLayerColor = fillLayerColorIdx >= 0 ? mask.maskColors[fillLayerColorIdx] : chosenLayerColor;

    // Create a copy of the original mask
    ALLEGRO_BITMAP* originalMask = al_clone_bitmap(mask.bmp.get());

    for (int y = 0; y < mask.height; y++)
    {
        for (int x = 0; x < mask.width; x++)
        {
            bool updatePixel = false;
            for (int i = 0; i < structuringElement.size(); i++)
            {
                for (int j = 0; j < structuringElement[i].size(); j++)
                {
                    int nx = x + i - structuringElement.size() / 2;
                    int ny = y + j - structuringElement[i].size() / 2;

                    if (nx >= 0 && nx < mask.width && ny >= 0 && ny < mask.height) {
                        if (structuringElement[i][j] == 1) {
                            ALLEGRO_COLOR neighborColor = al_get_pixel(originalMask, nx, ny);
                            if (areColorsEqual(neighborColor, chosenLayerColor)) {
                                updatePixel = true;
                                break;
                            }
                        }
                    }
                }
                if (updatePixel) break;
            }

            if (updatePixel) {
                al_put_pixel(x, y, fillLayerColor);
            }
        }
    }
    al_destroy_bitmap(originalMask);
}

// void DilateClustersBasedOnIntensity(Mask& mask, int structuringElementSize, std::vector<Cluster> clusters)
// {
//     // Iterate over each pixel in the mask
//     for (int y = 0; y < mask.height; y++)
//     {
//         for (int x = 0; x < mask.width; x++)
//         {
//             // Determine the cluster of the current pixel
//             int clusterIdx = findClusterOfPixel(mask, x, y, clusters);
//             float clusterMean = clusters[clusterIdx].meanIntensity;

//             bool dilatePixel = false;
//             for (int i = -structuringElementSize/2; i <= structuringElementSize/2; i++)
//             {
//                 for (int j = -structuringElementSize/2; j <= structuringElementSize/2; j++)
//                 {
//                     int nx = x + i;
//                     int ny = y + j;

//                     // Check bounds
//                     if (nx >= 0 && nx < mask.width && ny >= 0 && ny < mask.height)
//                     {
//                         float neighborIntensity = getPixelIntensity(mask, nx, ny);

//                         // Check if the intensity is within the threshold
//                         if (std::abs(neighborIntensity - clusterMean) <= threshold)
//                         {
//                             dilatePixel = true;
//                             break;
//                         }
//                     }
//                 }
//                 if (dilatePixel) break;
//             }

//             if (dilatePixel)
//             {
//                 // Dilate: Modify the pixel as needed (e.g., include it in the cluster)
//             }
//         }
//     }
// }
