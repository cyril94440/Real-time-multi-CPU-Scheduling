//
//  TaskSetMP.h
//  rateMonotonic
//
//  Created by Cyril Trosset on 18/11/2013.
//  Copyright (c) 2013 Cyril Trosset. All rights reserved.
//

#ifndef __rateMonotonic__TaskSetMP__
#define __rateMonotonic__TaskSetMP__

#include <iostream>
#include "TaskSet.h"

#endif /* defined(__rateMonotonic__TaskSetMP__) */


class TaskSetMP : public TaskSet
{
public:
    TaskSetMP();
    TaskSetMP(int numberOfTasks,int utilization);
    ~TaskSetMP();
    
    void firstFitRepartition(int delta);
    void nextFitRepartition(int delta);
    void bestFitRepartition(int delta);
    
    double getFirstFitTime();
    double getNextFitTime();
    double getBestFitTime();
    
    
    bool testSchedulability();
    
    int getM();
    
protected:
    std::vector<TaskSet> _taskSets;
    int _m;
    double firstFitTime;
    double nextFitTime;
    double bestFitTime;
    
};