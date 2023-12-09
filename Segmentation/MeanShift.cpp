#include "MeanShift.h"
#include "Utils.h"

#include <math.h>

MeanShift::MeanShift(int radius, int epsilon, DataPoint3D maxDataPoint, DimensionWeights dimentionWeights)
  : radius(radius), epsilon(epsilon), maxDataPoint(maxDataPoint), dimensionWeights(dimentionWeights)
{
    
}

double MeanShift::calculateSquaredDistance(const DataPoint3D& point, const DataPoint3D& centroid) const
{
    int normalizedPointXDistance = (point.x - centroid.x) * MAX_NORMALIZED_VALUE / maxDataPoint.x;
    int normalizedPointYDistance = (point.y - centroid.y) * MAX_NORMALIZED_VALUE / maxDataPoint.y;
    int normalizedPointIntensityDistance = (point.intensity - centroid.intensity) * MAX_NORMALIZED_VALUE / maxDataPoint.intensity;

    return (
                dimensionWeights.xWeight * normalizedPointXDistance * normalizedPointXDistance +
                dimensionWeights.yWeight * normalizedPointYDistance * normalizedPointYDistance +
                dimensionWeights.intensityWeight * normalizedPointIntensityDistance * normalizedPointIntensityDistance
    );
}

bool MeanShift::addCentroidIfUnique(const DataPoint3D& newPoint, double epsilon)
{
    for (const auto& centroid : centroids)
    {
        if (calculateSquaredDistance(centroid, newPoint) < epsilon*epsilon) {
            return false;
        }
    }
    centroids.push_back(newPoint);
    printf("Added centroid. Centroids count: %d\n", centroids.size());
    return true;
}

int MeanShift::getMostSimilarCentroidIdx(DataPoint3D point, const std::vector<DataPoint3D>& centroids) const
{
    int minDiff = sqrt(3 * MAX_NORMALIZED_VALUE * MAX_NORMALIZED_VALUE);
    int mostSimilarCentroidIdx = 0;
    for (int k = 0; k < centroids.size(); k++) {
        int diff = sqrt(calculateSquaredDistance(point, centroids[k]));
        if (diff < minDiff) {
            minDiff = diff;
            mostSimilarCentroidIdx = k;
        }
    }
    return mostSimilarCentroidIdx;
}

// General algorithm outline –

// for p in copied_points:
// while not at_kde_peak:
// p = shift(p, original_points)
// Shift function looks like this –

// def shift(p, original_points):
// shift_x = float(0)
// shift_y = float(0)
// scale_factor = float(0)

// for p_temp in original_points:
// # numerator
// dist = euclidean_dist(p, p_temp)
// weight = kernel(dist, kernel_bandwidth)
// shift_x += p_temp[0] * weight
// shift_y += p_temp[1] * weight
// # denominator
// scale_factor += weight

// shift_x = shift_x / scale_factor
// shift_y = shift_y / scale_factor
// return [shift_x, shift_y]

// DataPoint3D initialPoint = {mask.width/2, mask.height/2, getGray(al_get_pixel(orygImage.bmp.get(), 0 + mask.x - orygImage.x, 0 + mask.y - orygImage.y))};

void MeanShift::RunStep(const Image& orygImage, const Mask& mask)
{
    const double radiusSquared = radius * radius;

    std::vector<DataPoint3D> copiedPoints;
    copiedPoints.reserve(mask.width * mask.height);
    for (int y = 0; y < mask.height; y++)
    {
        if ((y % 5) == 0) {
            for (int x = 0; x < mask.width; x++)
            {
                if (x % 5 == 0) {
                int pxColorGray = getGray(al_get_pixel(orygImage.bmp.get(), x + mask.x - orygImage.x, y + mask.y - orygImage.y));
                copiedPoints.push_back(DataPoint3D{x, y, pxColorGray});
                }
            }
        }
    }


    for (auto& point : copiedPoints)
    {
        // printf("Point initially: %f %f %f\n", point.x, point.y, point.intensity);
        bool atPeak = false;
        while (!atPeak)
        {
            DataPoint3D newPoint{0, 0, 0};
            double totalWeight = 0;

            for (int y = 0; y < mask.height; y++)
            {
                for (int x = 0; x < mask.width; x++)
                {
                    int pxColorGray = getGray(al_get_pixel(orygImage.bmp.get(), x + mask.x - orygImage.x, y + mask.y - orygImage.y));
                    double distanceSquared = calculateSquaredDistance(DataPoint3D{x, y, pxColorGray}, point);

                    // TODO: separate distance for every field of Data
                    if (distanceSquared < radiusSquared) {
                        double weight = exp(-distanceSquared / (2 * radiusSquared));
                        newPoint.x += x * weight;
                        newPoint.y += y * weight;
                        newPoint.intensity += pxColorGray * weight;
                        totalWeight += weight;
                    }
                }
            }

            newPoint.x /= totalWeight;
            newPoint.y /= totalWeight;
            newPoint.intensity /= totalWeight;

            // Check for convergence
            double shiftDistance = sqrt(calculateSquaredDistance(newPoint, point));
            if (shiftDistance < epsilon) {
                atPeak = true;
            }
            point = newPoint;
        }
        addCentroidIfUnique(point, epsilon);
    }

    for (const auto& centroid : centroids)
    {
        printf("Centroid: %f %f %f\n", centroid.x, centroid.y, centroid.intensity);
        for (int y = 0; y < mask.height; y++)
        {
            for (int x = 0; x < mask.width; x++)
            {
                int pxColorGray = getGray(al_get_pixel(orygImage.bmp.get(), x + mask.x - orygImage.x, y + mask.y - orygImage.y));
                double distanceSquared = calculateSquaredDistance(DataPoint3D{x, y, pxColorGray}, centroid);
                int mostSimilarCentroidIdx = getMostSimilarCentroidIdx(DataPoint3D{x, y, pxColorGray}, centroids);
                if (distanceSquared < radiusSquared) {
                    // printf("Point: %f %f %f\n", point.x, point.y, point.intensity);
                    al_put_pixel(x, y, maskColors[mostSimilarCentroidIdx]);
                }
            }
        }
    }
}

// void DBSCAN(const Image& orygImage, const Mask& mask, double eps, int minPts) {
//     const double epsSquared = eps * eps;
//     std::vector<DataPoint3D> points;
//     std::vector<int> clusterLabels;
//     points.reserve(mask.width * mask.height);
//     clusterLabels.resize(mask.width * mask.height, -1); // -1 for unclassified

//     // Load points from the image
//     for (int y = 0; y < mask.height; y++) {
//         for (int x = 0; x < mask.width; x++) {
//             int pxColorGray = getGray(al_get_pixel(orygImage.bmp.get(), x + mask.x - orygImage.x, y + mask.y - orygImage.y));
//             points.push_back(DataPoint3D{x, y, pxColorGray});
//         }
//     }

//     int clusterId = 0;
//     for (int i = 0; i < points.size(); ++i) {
//         if (clusterLabels[i] != -1) continue; // Already classified

//         std::vector<int> neighbors = regionQuery(points, i, epsSquared);
//         if (neighbors.size() < minPts) {
//             clusterLabels[i] = 0; // Mark as noise
//             continue;
//         }

//         // Expand cluster
//         clusterLabels[i] = ++clusterId;
//         for (int j = 0; j < neighbors.size(); ++j) {
//             if (clusterLabels[neighbors[j]] == 0) {
//                 clusterLabels[neighbors[j]] = clusterId; // Change noise to border point
//             }
//             if (clusterLabels[neighbors[j]] != -1) continue; // Already processed

//             clusterLabels[neighbors[j]] = clusterId;
//             std::vector<int> neighbors2 = regionQuery(points, neighbors[j], epsSquared);
//             if (neighbors2.size() >= minPts) {
//                 neighbors.insert(neighbors.end(), neighbors2.begin(), neighbors2.end()); // Add new neighbors
//             }
//         }
//     }

//     // Post-processing: Assign pixel colors based on cluster ID
//     for (int i = 0; i < points.size(); ++i) {
//         const auto& p = points[i];
//         int label = clusterLabels[i];
//         al_put_pixel(p.x, p.y, clusterColors[label]); // Define clusterColors for each clusterId
//     }
// }

// std::vector<int> regionQuery(const std::vector<DataPoint3D>& points, int idx, double epsSquared) {
//     std::vector<int> neighbors;
//     for (int i = 0; i < points.size(); ++i) {
//         if (calculateSquaredDistance(points[i], points[idx]) <= epsSquared) {
//             neighbors.push_back(i);
//         }
//     }
//     return neighbors;
// }


