#include <utils.h>

using namespace std;

void handleJobArrival(struct Job job, priority_queue<struct Job, vector<struct Job>, cmpQ1>& holdQueue1, priority_queue<struct Job, vector<struct Job>, cmpQ2>& holdQueue2, queue<Process>& readyQueue,System* system)
{
    if(system->memory<job.memoryRequirement){
        if(job.priority ==  1){
            holdQueue1.push(job);
        }
        else{
            holdQueue2.push(job);
        }
    }
    else{
        struct Process tmp;
        tmp.id = job.jobNumber;
        tmp.burstTimeRemaining = job.burstTime;
        tmp.neededMemory = job.memoryRequirement;
        tmp.devicesHeld = 0;
        system->memory -= tmp.neededMemory;
        readyQueue.push(tmp);
    }
    return;
}

void handleDeviceRequest()
{
    // TODO
    // Run banker's algo -> it will return a boolean value
    // If true:
    // update the number of devices that the job currently holds
    // reduce the number of devices that the system has to give
    // If false: move job to end of the waiting queue

    cout << "handling device request" << endl;
    return;
}

void handleDeviceRelease()
{
    // TODO
    // Update the number of devices the job currently holds
    // increase the number of devices the system has to give
    // Loop through the waiting queue
    // And run bankers algorithm on for each job that banker's algo returns true for
    // move back to the ready queue. 
    // If a job can be moved from the waiting to the ready queue, then you need to have similar logic to the device request.
    // Still need to increase the number of devices that the job holds
    // Reduce the number of devices system has to give. 
    
    cout << "handling device release" << endl;
    return;
}

void handleDisplay()
{
    // TODO
    cout << "handling display" << endl;
    return;
}

void handleProcessTermination()
{
    // TODO
    cout << "terminating process" << endl;
    return;
}