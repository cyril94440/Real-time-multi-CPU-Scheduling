//
//  task.h
//  rateMonotonic
//
//  Created by Cyril Trosset on 21/09/2013.
//  Copyright (c) 2013 Cyril Trosset. All rights reserved.
//

#include <stdlib.h>
#include <math.h>
#include <vector>


#ifndef rateMonotonic_task_h
#define rateMonotonic_task_h

class Task
{
public:
    
    Task();
    Task(int execTime, int period);
    ~Task();
    
    int getUtilization();
    void setUtilization(int utilization);
    
    int getPeriod();
    
    int getPriority();
    void setPriority(int priority);
    
    void setExecTimeAndPeriod(int execTime, int period);
    
    long double getExecTime();
    
    int getInstance();
    int getInstanceForT(int t);
    void setInstance(int instance);
    
    int getExecuted();
    void setExecuted(int executed);
    void executeFor(int time);
    
    bool executable(int t);
    
    bool deadlineMissed(int t);
    
    int promotionBetween(int t1, int t2,int &index);
    void addPromotion(int t);
    bool promotedAt(int t);
    
private:
    int _utilization;
    int _execTime;
    int _period;
    int _priority;
    
    int _instance;
    int _executed;
    
    std::vector<int> _promotions;
    
    
    
};

#endif
