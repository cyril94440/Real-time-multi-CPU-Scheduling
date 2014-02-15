//
//  TaskSetMP.cpp
//  rateMonotonic
//
//  Created by Cyril Trosset on 18/11/2013.
//  Copyright (c) 2013 Cyril Trosset. All rights reserved.
//

#include "TaskSetMP.h"

TaskSetMP::TaskSetMP():TaskSet(),_m(0){};
TaskSetMP::TaskSetMP(int numberOfTasks,int utilization):TaskSet(numberOfTasks,utilization),_m(0){};
TaskSetMP::~TaskSetMP(){};

using namespace std;

void TaskSetMP::firstFitRepartition(int delta)
{

    _m = 1;
    _taskSets.clear();
    TaskSet firstTaskSet = TaskSet(true);
    
    _taskSets.push_back(firstTaskSet);
    
    timestamp_t t1 = this->get_timestamp();
    
    for(int i=0;i<_numberOftasks;i++)
    {
        
        bool assigned = false;
        
        int j=0;
        while(!assigned && j<_m)
        {
            int n = _taskSets[j].getNumberOfTasks() + 1;
            double powValue = powf(2, 1/(double)n);
            double bound = (double)n*(powValue-1);
            if((_taskSets[j].getUtilization()+_tasks[i].getUtilization())<=(bound*100)+delta)
            {
                assigned=true;
                _taskSets[j].addTask(&_tasks[i]);
            }
            j++;
        }
        
        if(!assigned)
        {
            _m++;
            TaskSet newTaskSet = TaskSet(true);
            _taskSets.push_back(newTaskSet);
            _taskSets[_m-1].addTask(&_tasks[i]);
        }
        
    }
    
    firstFitTime = this->get_timestamp()-t1;
    
    //cout << "Number of processor : " << _m << endl;
}
void TaskSetMP::nextFitRepartition(int delta)
{
    
    _taskSets.clear();
    _m = 1;
    
    TaskSet firstTaskSet = TaskSet(true);
    
    _taskSets.push_back(firstTaskSet);
    
    timestamp_t t1 = this->get_timestamp();
    
    for(int i=0;i<_numberOftasks;i++)
    {
        
        int n = _taskSets[_m-1].getNumberOfTasks() + 1;
        double powValue = powf(2, 1/(double)n);
        double bound = (double)n*(powValue-1);
        if((_taskSets[_m-1].getUtilization()+_tasks[i].getUtilization())<=(bound*100)+delta)
        {
            _taskSets[_m-1].addTask(&_tasks[i]);
        }
        else
        {
            _m++;
            TaskSet newTaskSet = TaskSet(true);
            _taskSets.push_back(newTaskSet);
            _taskSets[_m-1].addTask(&_tasks[i]);
        }
        
    }
    
    nextFitTime = this->get_timestamp()-t1;
    
    //cout << "Number of processor : " << _m << endl;
}
void TaskSetMP::bestFitRepartition(int delta)
{
    
    _taskSets.clear();
    _m = 1;
    
    TaskSet firstTaskSet = TaskSet(true);
    
    _taskSets.push_back(firstTaskSet);
    
    timestamp_t t1 = this->get_timestamp();
    
    for(int i=0;i<_numberOftasks;i++)
    {
        float minDelta = 999999;
        int iProcessor = -1;
        for(int j=0;j<_m;j++)
        {
            int n = _taskSets[j].getNumberOfTasks() + 1;
            double powValue = powf(2, 1/(double)n);
            double bound = (double)n*(powValue-1);
            
            float deltaCPU = ((bound*100)+delta)-(_taskSets[j].getUtilization()+_tasks[i].getUtilization());
            if(deltaCPU>0 && deltaCPU<minDelta)
            {
                minDelta = deltaCPU;
                iProcessor = j;
            }
        }
        if(iProcessor != -1)
        {
            _taskSets[_m-1].addTask(&_tasks[i]);
        }
        else
        {
            _m++;
            TaskSet newTaskSet = TaskSet(true);
            _taskSets.push_back(newTaskSet);
            _taskSets[_m-1].addTask(&_tasks[i]);
        }
        
    }
    
    bestFitTime = this->get_timestamp()-t1;
    
    //cout << "Number of processor : " << _m << endl;
}
bool TaskSetMP::testSchedulability()
{
    bool success = true;
    for(int i=0;i<_m;i++)
    {
        //cout << endl << "Processor " << i << " :  " << endl ;
        TaskSet set = _taskSets[i];
        //set.print();
        if(set.rmSchedule(false, false)==0)
        {
            success = false;
            //cout << "NOT SCHEDULABLE" << endl;
        }
        
    }
    
    return success;
}
int TaskSetMP::getM()
{
    return _m;
}
double TaskSetMP::getFirstFitTime()
{
    return firstFitTime;
}
double TaskSetMP::getNextFitTime()
{
    return nextFitTime;
}
double TaskSetMP::getBestFitTime()
{
    return bestFitTime;
}