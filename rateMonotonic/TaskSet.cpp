//
//  taskSet.cpp
//  rateMonotonic
//
//  Created by Cyril Trosset on 21/09/2013.
//  Copyright (c) 2013 Cyril Trosset. All rights reserved.
//

#include "TaskSet.h"

using namespace std;

TaskSet::TaskSet(int numberOfTasks,int utilization):_numberOftasks(numberOfTasks),_utilization(utilization),_t(0),_noNull(true)
{
    //Generate Task
    _tasks.resize(numberOfTasks);
    
    int attempt=0;
    
    _noNull = true;
    
    while(1)
    {
        int sumU = utilization;
        for(int i=0;i<numberOfTasks-1;i++)
        {
            double r = ((double) rand() / (RAND_MAX));
            double powVar = pow(r,1/(numberOfTasks-(double)i+1));
            int nextSumU = (sumU * powVar);
            if(sumU - nextSumU<=100)
            {
                _tasks[i].setUtilization(sumU - nextSumU);
                sumU = nextSumU;
            }
            else
                i--;
        }
        
        _tasks[numberOfTasks-1].setUtilization(sumU);
        if(sumU<=100 && sumU>0)
            break;
        
        if(attempt++>=100)
        {
            _noNull = false;
            break;
        }
        
    }
    
    
    //Order Priorities
    int *periodsCopy = new int[numberOfTasks];
    
    for(int i=0;i<numberOfTasks;i++)
        periodsCopy[i] = _tasks[i].getPeriod();
    
    for(int i=0;i<numberOfTasks;i++)
    {
        int min=9999999;
        int taskI = 0;
        for(int j=0;j<numberOfTasks;j++)
        {
            if(periodsCopy[j]!=0 && periodsCopy[j]<min)
            {
                min = periodsCopy[j];
                taskI = j;
            }
        }
        
        swap(_tasks[i], _tasks[taskI]);
        swap(periodsCopy[i],periodsCopy[taskI]);
        
        _tasks[i].setPriority(i);
        periodsCopy[i] = 0;
        
        
    }
    
    delete periodsCopy;
    
    //Determine observation period
    long double result = 1;
    for(int i=0;i<numberOfTasks;i++)
    {
        result = ppcm(result,_tasks[i].getPeriod());
    }
    
    _ppcm = result;
    
    //cout << endl << "PPCM : " << _ppcm << endl;
}
TaskSet::TaskSet()
{
    string input;
    cout << "Enter your set of task with the following form : [(E1,T1),(E2,T2),...] : ";
    cin >> input;
    
    vector<string> splitStrings;
    istringstream mainString(input);
    string string;
    
    
    while(getline( mainString , string , ',' ) )
    {
        //push split strings into vector.
        splitStrings.push_back(string);
    }
    
    _numberOftasks = (int)splitStrings.size()/2;
    _tasks.resize(_numberOftasks);
    
    int execution=0;
    int task = 0;
    for(int i=0;i<splitStrings.size();i++)
    {
        splitStrings[i].erase(remove(splitStrings[i].begin(), splitStrings[i].end(), '[' ), splitStrings[i].end() );
        splitStrings[i].erase(remove(splitStrings[i].begin(), splitStrings[i].end(), ']' ), splitStrings[i].end() );
        splitStrings[i].erase(remove(splitStrings[i].begin(), splitStrings[i].end(), ',' ), splitStrings[i].end() );
        splitStrings[i].erase(remove(splitStrings[i].begin(), splitStrings[i].end(), '(' ), splitStrings[i].end() );
        splitStrings[i].erase(remove(splitStrings[i].begin(), splitStrings[i].end(), ')' ), splitStrings[i].end() );
        
        if(execution==0)
        {
            execution = atoi(splitStrings[i].c_str());
        }
        else
        {
            _tasks[task].setExecTimeAndPeriod(execution*100, atoi(splitStrings[i].c_str()));
            execution=0;
            task++;
        }

    }
    
    int *periodsCopy = new int[_numberOftasks];
    
    for(int i=0;i<_numberOftasks;i++)
        periodsCopy[i] = _tasks[i].getPeriod();
    
    for(int i=0;i<_numberOftasks;i++)
    {
        int min=9999999;
        int taskI = 0;
        for(int j=0;j<_numberOftasks;j++)
        {
            if(periodsCopy[j]!=0 && periodsCopy[j]<min)
            {
                min = periodsCopy[j];
                taskI = j;
            }
        }
        
        swap(_tasks[i], _tasks[taskI]);
        swap(periodsCopy[i],periodsCopy[taskI]);
        
        _tasks[i].setPriority(i);
        periodsCopy[i] = 0;
    }
    
    delete periodsCopy;
    
    //Determine observation period
    long double result = 1;
    for(int i=0;i<_numberOftasks;i++)
    {
        result = ppcm(result,_tasks[i].getPeriod());
    }
    
    _ppcm = result;
    
    cout << endl << "PPCM : " << _ppcm << endl;
}
TaskSet::TaskSet(bool empty):_numberOftasks(0),_utilization(0),_ppcm(1),_t(0)
{
    
}
void TaskSet::addTask(Task *task)
{
    
    _numberOftasks++;
    _utilization+=task->getUtilization();
    _ppcm = ppcm(_ppcm, task->getPeriod());
    _tasks.push_back(*task);
    
    //Order Priorities
    int *periodsCopy = new int[_numberOftasks];
    
    for(int i=0;i<_numberOftasks;i++)
        periodsCopy[i] = _tasks[i].getPeriod();
    
    for(int i=0;i<_numberOftasks;i++)
    {
        int min=9999999;
        int taskI = 0;
        for(int j=0;j<_numberOftasks;j++)
        {
            if(periodsCopy[j]!=0 && periodsCopy[j]<min)
            {
                min = periodsCopy[j];
                taskI = j;
            }
        }
        
        _tasks[taskI].setPriority(i);
        periodsCopy[taskI] = 0;
    }
    
    delete periodsCopy;
    
}
int TaskSet::getUtilization()
{
    return _utilization;
}
int TaskSet::getNumberOfTasks()
{
    return _numberOftasks;
}

TaskSet::~TaskSet()
{
   
}

//Public
void TaskSet::print()
{
    for(int i=0;i<_numberOftasks;i++)
    {
        cout << i << " : Utilization : " << _tasks[i].getUtilization() << " % | exec time : " << _tasks[i].getExecTime()*10 << " ms | period : " << _tasks[i].getPeriod() << "s | Priority : " << _tasks[i].getPriority() << endl;
    }
}
int TaskSet::priorityTaskForT(int t)
{
        int iExectued = -1;
        for(int i=0;i<_numberOftasks;i++)
        {
            if(_tasks[i].executable(t))
            {
                if(iExectued==-1 || ((_tasks[i].getPriority()<_tasks[iExectued].getPriority())&&(!_tasks[iExectued].promotedAt(t))) || _tasks[i].promotedAt(t))
                {
                    iExectued = i;
                }
            }
            
        }
    
    return iExectued;
}
int TaskSet::rmSchedule(bool print, bool promotions)
{
    if(print)
    {
        cout << endl << endl;
    }
    
    int t = 0;
    int nextExec = -1;
    int setTested = 1;
    
    timestamp_t t0 = get_timestamp();
    
    int numberOfPromotions = 0;
    
    if(_ppcm>=999999999)
    {
        return -1;
    }
    
    while(t<=_ppcm*100)
    {
        
        timestamp_t t1 = get_timestamp();
        double secs = (t1 - t0) / 1000000.0L;
        
        if(secs>180)
        {
            return -1;
        }
        
        int thisExec = -1;
        
        nextExec = -1;
        
        if(nextExec == -1)
            thisExec = this->priorityTaskForT(t);
        else
        {
            thisExec = nextExec;
            nextExec = -1;
        }
        
        //How long for the execution ?
        if(thisExec!=-1)
        {
            int execTime = _tasks[thisExec].getExecTime()-_tasks[thisExec].getExecuted();
            
            if((_tasks[thisExec].getInstance()*_tasks[thisExec].getPeriod()*100-t)<execTime)
                execTime = _tasks[thisExec].getInstance()*_tasks[thisExec].getPeriod()*100-t;
            
            bool preempt = false;
            bool promoted = false;
            for(int i=0;i<_numberOftasks;i++)
            {
                int tPromotion = -1;
                if(promotions)
                {
                    int indexNotNeeded = 0;
                    tPromotion = _tasks[i].promotionBetween(t, t+execTime, indexNotNeeded);
                }
                //Check for preemption
                if((_tasks[i].getInstanceForT(t) != _tasks[i].getInstanceForT(t+execTime-1)) || tPromotion!=-1)
                {
                    if((_tasks[i].getPriority()<_tasks[thisExec].getPriority()) || tPromotion!=-1)
                    {
                        int possibleExecTime = _tasks[i].getInstanceForT(t)*_tasks[i].getPeriod()*100 - t;
                        if(tPromotion!=-1)
                            possibleExecTime = tPromotion - t;
                        if(!preempt || possibleExecTime<execTime || ((possibleExecTime==execTime)&&(_tasks[nextExec].getPriority()>_tasks[i].getPriority())))
                        {
                            preempt = true;
                            nextExec = i;
                            execTime = possibleExecTime;
                            
                            if(tPromotion!=-1)
                                promoted = true;
                            else
                                promoted = false;
                        }
                    }
                }
            }
            
            if(!preempt)
                nextExec = -1;
            
            //Execute
            if(print)
            {
                cout << "t : "<< t*10 <<"ms (task"<< thisExec <<",instance"<< _tasks[thisExec].getInstance() <<"),duration : "<< execTime*10<< "ms";
                if(_tasks[thisExec].getExecuted()+execTime==_tasks[thisExec].getExecTime())
                {
                    cout << " finish (release:" << (_tasks[thisExec].getInstance()-1)*_tasks[thisExec].getPeriod() << "s , deadline : " << (_tasks[thisExec].getInstance())*_tasks[thisExec].getPeriod() << "s)";
                }
                
                if(_tasks[thisExec].promotedAt(t))
                {
                    cout << " PROMOTED";
                    numberOfPromotions++;
                }
                cout << endl;
            }
            else
            {
                if(_tasks[thisExec].promotedAt(t))
                {
                    numberOfPromotions++;
                }
            }
            
            _tasks[thisExec].executeFor(execTime);
            t+=execTime;
            
        }
        else //IDLE
        {
            int execTime = 999999999;
            bool preempt = false;
            
            for(int i=0;i<_numberOftasks;i++)
            {
                    int possibleExecTime = _tasks[i].getInstanceForT(t)*_tasks[i].getPeriod()*100 - t;
                    if(!preempt || (preempt && possibleExecTime<execTime) || ((possibleExecTime==execTime)&&(_tasks[nextExec].getPriority()>_tasks[i].getPriority())))
                    {
                        preempt = true;
                        nextExec = i;
                        execTime = possibleExecTime;
                    }
            }
            
            if(!preempt)
            {
                //PAS POSSIBLE !
            }
            
            //Stay idle
            if(print)
            {
            cout << "t : "<< t*10 << "ms IDLE For " << execTime*10 << "ms" << endl;
            }
            t+=execTime;
            nextExec=-1;
        }
        
        //Check Deadlines
        for(int i=0;i<_numberOftasks;i++)
        {
            if(_tasks[i].deadlineMissed(t))
            {
                if(promotions)
                {
                    int deadline = (_tasks[i].getInstance())*_tasks[i].getPeriod()*100;
                    int deltaMissed = _tasks[i].getExecTime()-_tasks[i].getExecuted();
                    
                    int indexNotNeeded = 0;
                    int currentPromotion = _tasks[i].promotionBetween(deadline-(_tasks[i].getPeriod()*100), deadline, indexNotNeeded);
                    if(currentPromotion==-1)
                        currentPromotion = deadline;
                    
                    if((currentPromotion-deltaMissed)<(deadline-(_tasks[i].getPeriod()*100)))
                    {
                        if(print)
                            cout << " NOT SCHEDULABLE  !!!!! ";
                        return 0;
                    }
                    else if((currentPromotion-deltaMissed)==(deadline-(_tasks[i].getPeriod()*100)))
                    {
                        return -1;
                    }
                    
                    _tasks[i].addPromotion(currentPromotion-deltaMissed);
                    
                    if(print)
                    {
                        cout << "t : " << t*10 << "ms DEADLINE MISSED for task : " << i << " , instance : " << _tasks[i].getInstance();
                        cout << " , PROMOTED at : " << (currentPromotion-deltaMissed)*10;
                        cout << " executed : " << _tasks[i].getExecuted()*10 << "ms" << endl;
                        cout << endl;
                    }
                    
                    t=0;
                    this->resetTask();
                    nextExec = -1;
                    setTested++;
                    numberOfPromotions = 0;
                }
                else
                {
                    if(print)
                    {
                        cout << "t : " << t*10 << "ms DEADLINE MISSED for task : " << i << " , instance : " << _tasks[i].getInstance();
                        cout << " executed : " << _tasks[i].getExecuted()*10 << "ms" << endl;
                        cout << endl;
                    }
                    return 0;
                }
            }
        }
    }
    
    if(setTested==1)
        return 2;
    else
    {
        //Calculer le nbre d'instances total.
        int nbreInstances = 0;
        for(int i=0;i<_numberOftasks;i++)
        {
            nbreInstances+= _tasks[i].getInstance()-1;
        }
        
        _promotions = ((long double)numberOfPromotions*100)/((long double)nbreInstances-1);
        
        return 1;
    }
    
    return 0;
}

bool TaskSet::getNoNull()
{
    return _noNull;
}
long double TaskSet::getPromotions()
{
    return _promotions;
}

//Private
long double TaskSet::ppcm(long double a, long double b)
{
    long double p=a*b;
    while (a!=b) if (a<b) b-=a; else a-=b;
    return p/a;
}
void TaskSet::resetTask()
{
    for(int i=0;i<_numberOftasks;i++)
    {
        _tasks[i].setInstance(1);
        _tasks[i].setExecuted(0);
    }
}