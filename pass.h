/********************************************************************************
* File Name : pass.h
* Title     :
* Programmer: Ron Hashimoto
* Belonging :
* Date      : 2014.11.1
* Language  : C++
*********************************************************************************
* class to calculate next AOS, LOS and TCA
*
********************************************************************************/
#ifndef PASS_H_
#define PASS_H_

#include "tracker.h"
#include <vector>

struct Pass {
    double AOS, LOS, TCA;
};

class PassFinder {
public:
  PassFinder (void);
  Pass findFirst(SpacecraftTracker* tracker, double begin_unix, double end_unix, double interval) const;
  std::vector<Pass> findAll(SpacecraftTracker* tracker, double begin_unix, double end_unix, double interval) const;
};

#endif
