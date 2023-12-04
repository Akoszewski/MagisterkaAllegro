#include "MeanShift.h"
#include "Utils.h"

#include <math.h>

MeanShift::MeanShift(int radius, DataPoint3D maxDataPoint, DimensionWeights dimentionWeights)
  : radius(radius), maxDataPoint(maxDataPoint), dimensionWeights(dimentionWeights)
{
    
}


void MeanShift::RunStep(const Image& orygImage, const Mask& mask)
{
    DataPoint3D initialPoint = {mask.width/2, mask.height/2, getGray(al_get_pixel(orygImage.bmp.get(), 0 + mask.x - orygImage.x, 0 + mask.y - orygImage.y))};
    DataPoint3D currentPoint = initialPoint;
    const double radiusSquared = radius * radius;
    const double epsilon = 1; // Convergence threshold

    while (true)
    {
        DataPoint3D newPoint{0, 0, 0};
        double totalWeight = 0;

        // for (const auto& point : points) {
        //     double distanceSquared = (point.x - currentPoint.x) * (point.x - currentPoint.x) +
        //                              (point.y - currentPoint.y) * (point.y - currentPoint.y);

        //     if (distanceSquared < radiusSquared) {
        //         double weight = exp(-distanceSquared / (2 * radiusSquared));
        //         newPoint.x += point.x * weight;
        //         newPoint.y += point.y * weight;
        //         totalWeight += weight;
        //     }
        // }

        newPoint.x /= totalWeight;
        newPoint.y /= totalWeight;
        newPoint.intensity /= totalWeight;

        // Check for convergence
        double shiftDistance = sqrt((newPoint.x - currentPoint.x) * (newPoint.x - currentPoint.x) +
                                    (newPoint.y - currentPoint.y) * (newPoint.y - currentPoint.y));
        if (shiftDistance < epsilon) {
            break;
        }
        currentPoint = newPoint;
    }
}


// Point meanShift(const std::vector<Point>& points, Point initialPoint, double radius) {
//     Point currentPoint = initialPoint;
//     const double radiusSquared = radius * radius;
//     const double epsilon = 1; // Convergence threshold

//     while (true) {
//         Point newPoint(0, 0);
//         double totalWeight = 0;

//         for (const auto& point : points) {
//             double distanceSquared = (point.x - currentPoint.x) * (point.x - currentPoint.x) +
//                                      (point.y - currentPoint.y) * (point.y - currentPoint.y);

//             if (distanceSquared < radiusSquared) {
//                 double weight = exp(-distanceSquared / (2 * radiusSquared));
//                 newPoint.x += point.x * weight;
//                 newPoint.y += point.y * weight;
//                 totalWeight += weight;
//             }
//         }

//         newPoint.x /= totalWeight;
//         newPoint.y /= totalWeight;

//         // Check for convergence
//         double shiftDistance = sqrt((newPoint.x - currentPoint.x) * (newPoint.x - currentPoint.x) +
//                                     (newPoint.y - currentPoint.y) * (newPoint.y - currentPoint.y));
//         if (shiftDistance < epsilon) {
//             break;
//         }

//         currentPoint = newPoint;
//     }

//     return currentPoint;
// }
