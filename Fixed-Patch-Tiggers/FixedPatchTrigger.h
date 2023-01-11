#pragma once

#include <stddef.h>

#define FIXED_POINT_TRIGGERS 1 // we have only one firmware function in this program, i.e., "add4"

void fixedPointPatchInstaller();
void fixedPointTrigger(size_t functionID);
void fixedPointPatchRemover(size_t functionID);