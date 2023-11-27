#ifndef MORPH_OPERATIONS_H
#define MORPH_OPERATIONS_H

#include "Mask.h"
#include "Utils.h"

void ErodeMask(Mask& mask, const std::vector<std::vector<int>>& structuringElement, int chosenLayerColorIdx, int fillLayerColorIdx = -1);
void DilateMask(Mask& mask, const std::vector<std::vector<int>>& structuringElement, int chosenLayerColorIdx, int fillLayerColorIdx = -1);

#endif
