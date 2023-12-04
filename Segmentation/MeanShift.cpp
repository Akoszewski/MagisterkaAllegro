#include "MeanShift.h"

MeanShift::MeanShift(DataPoint3D maxDataPoint, DimensionWeights dimentionWeights)
  : maxDataPoint(maxDataPoint), dimensionWeights(dimentionWeights)
{
    
}


void MeanShift::RunStep(const Image& orygImage, const Mask& mask)
{

}
