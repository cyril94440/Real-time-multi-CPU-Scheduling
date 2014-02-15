//
//  taskSet.h
//  rateMonotonic
//
//  Created by Cyril Trosset on 21/09/2013.
//  Copyright (c) 2013 Cyril Trosset. All rights reserved.
//

#include "Task.h"
#include <math.h>
#include <stdlib.h>
#include <iostream>
#include <sys/time.h>
#include <string.h>
#include <sstream>
#include <vector>

#ifndef rateMonotonic_taskSet_h
#define rateMonotonic_taskSet_h

class TaskSet{
public:
    TaskSet(int numberOfTasks,int utilization);
    TaskSet();
    TaskSet(bool empty);
    
    ~TaskSet();
    
    void print();
    
    void addTask(Task *task);
    
    int priorityTaskForT(int t);
    int rmSchedule(bool print, bool promotions); //0 not schedulable, 1 rm PP schedulable only, 2 rm schedulable
    
    bool getNoNull();
    
    int getUtilization();
    int getNumberOfTasks();
    
    long double getPromotions();
    
protected:
    std::vector<Task> _tasks;
    
    int _numberOftasks;
    int _utilization;
    
    long double _ppcm;
    
    int _t;
    
    bool _noNull;
    
    long double _promotions;
    
    long double ppcm(long double a,long double b);
    void resetTask();
    
    typedef unsigned long long timestamp_t;
    
    static timestamp_t get_timestamp ()
    {
        struct timeval now;
        gettimeofday (&now, NULL);
        return  now.tv_usec + (timestamp_t)now.tv_sec * 1000000;
    }

};


#endif
