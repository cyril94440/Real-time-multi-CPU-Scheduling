//
//  main.cpp
//  rateMonotonic
//
//  Created by Cyril Trosset on 21/09/2013.
//  Copyright (c) 2013 Cyril Trosset. All rights reserved.
//

#include <iostream>
#include <stdlib.h>
#include <vector>

#include "TaskSetMP.h"

using namespace std;


int main(int argc, const char * argv[])
{
    srand((unsigned int)time(0));
    
    for(int purcent=70;purcent<401;purcent++)
    {
        double first = 0,next = 0,best = 0;
        double firstTime = 0,nextTime=0,bestTime=0;
        
        for(int i=1;i<10000;i++)
        {
            
            TaskSetMP *set = new TaskSetMP(15,purcent);
            if(set->getNoNull())
            {
                set->firstFitRepartition(0);
                set->firstFitRepartition(0);
                first = ((double)set->getM() * 1/(double)i) + ((double)first * (1-(1/(double)i)));
                firstTime = ((double)set->getFirstFitTime() * 1/(double)i) + ((double)firstTime * (1-(1/(double)i)));
                
                set->nextFitRepartition(0);
                next = ((double)set->getM() * 1/(double)i) + ((double)next * (1-(1/(double)i)));
                nextTime = ((double)set->getNextFitTime() * 1/(double)i) + ((double)nextTime * (1-(1/(double)i)));
                
                set->bestFitRepartition(0);
                best = ((double)set->getM() * 1/(double)i) + ((double)best * (1-(1/(double)i)));
                bestTime = ((double)set->getBestFitTime() * 1/(double)i) + ((double)bestTime * (1-(1/(double)i)));
            }
            else
                i--;
            
        }
        
        //cout << nTask << "\t" << first << "\t" << next << "\t" << best << endl;
        //cout << nTask << "\t" << firstTime << "\t" << nextTime << "\t" << bestTime << endl;
        cout << purcent << "% ExecTime \t" << firstTime << "\t" << nextTime << "\t" << bestTime << endl;
        cout << purcent << "% Processors\t" << first << "\t" << next << "\t" << best << endl;
        
    }
    
    
    
    
    
    return 0;
}

