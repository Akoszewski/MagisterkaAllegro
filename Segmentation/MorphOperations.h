#ifndef MORPH_OPERATIONS_H
#define MORPH_OPERATIONS_H

#include "Mask.h"
#include "Utils.h"

void ErodeMask(const Mask& mask, std::vector<std::vector<int>> structuringElement, int chosenLayerColorIdx);
void DilateMask(const Mask& mask, std::vector<std::vector<int>> structuringElement, int chosenLayerColorIdx);

#endif
