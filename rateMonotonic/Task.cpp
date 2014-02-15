//
//  task.cpp
//  rateMonotonic
//
//  Created by Cyril Trosset on 21/09/2013.
//  Copyright (c) 2013 Cyril Trosset. All rights reserved.
//

#include "Task.h"

Task::Task():_executed(0),_execTime(0),_instance(1),_utilization(0),_period(0),_priority(0){};
Task::Task(int execTime, int period):_executed(0),_execTime(execTime),_instance(1),_utilization(0),_period(period),_priority(0){};
Task::~Task(){};

int Task::getUtilization()
{
    return _utilization;
}
void Task::setUtilization(int utilization)
{
    _utilization = utilization;
    
    _period = rand()%7+1;
    _execTime = round(_period*utilization);
}
int Task::getPeriod()
{
    return _period;
}

int Task::getPriority()
{
    return _priority;
}
void Task::setPriority(int priority)
{
    _priority = priority;
}
void Task::setExecTimeAndPeriod(int execTime, int period)
{
    _execTime = execTime;
    _period = period;
    
    _utilization = _execTime/(_period);
}

long double Task::getExecTime()
{
    return _execTime;
}

int Task::getInstance()
{
    return _instance;
}
int Task::getInstanceForT(int t)
{
    return floor(t/(_period*100))+1;
}
void Task::setInstance(int instance)
{
    _instance = instance;
}
int Task::getExecuted()
{
    return _executed;
}
void Task::setExecuted(int executed)
{
    _executed = executed;
}
void Task::executeFor(int t)
{
    _executed+=t;
    if(_executed==_execTime)
    {
        _instance++;
        _executed = 0;
    }
    else if(_executed>_execTime)
    {
//Temps d'exec depasse le max
    }
}
bool Task::executable(int t)
{
    int instance = floor(t/(_period*100))+1;
    if(instance<_instance)
        return false;
    else
    {
        if(instance>_instance)
        {
            //Deadline missed
            return false;
        }
        else if(_executed<_execTime)
                return true;
    }
    
    return false;
}

bool Task::deadlineMissed(int t)
{
    int instanceShouldBe = floor(t/(_period*100))+1;
    if(instanceShouldBe>_instance)
        return  true;
    
    return false;
}

int Task::promotionBetween(int t1, int t2, int &index)
{
    for(int i=0;i<_promotions.size();i++)
    {
        if(_promotions[i]>t1 && _promotions[i]<t2)
        {
            index = i;
            return _promotions[i];
        }
    }
    
    return -1;
}
void Task::addPromotion(int t)
{
    int instance = this->getInstanceForT(t);
    int t1 = (instance-1)*(_period*100);
    
    int index = 0;
    
    int existingPromotion = this->promotionBetween(t1, t1+(_period*100),index);
    
    if(existingPromotion==-1)
    {
        _promotions.push_back(t);
    }
    else
    {
        if(t<existingPromotion)
        {
            _promotions[index] = t;
        }
    }
    
}
bool Task::promotedAt(int t)
{
    for(int i=0;i<_promotions.size();i++)
    {
        if(_promotions[i]==t)
            return true;
    }
    
    return false;
}