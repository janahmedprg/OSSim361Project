#include <utils.h>

using namespace std;

void handleJobArrival(struct Job job, priority_queue<struct Job, vector<struct Job>, cmpQ1>& holdQueue1, priority_queue<struct Job, vector<struct Job>, cmpQ2>& holdQueue2, queue<Process>& readyQueue,System* system)
{
    if(system->memory<job.memoryRequirement){
        if(job.priority ==  1){
            holdQueue1.push(job);
            cout<<"job"<<job.jobNumber<<"placed in q 1\n";
        }
        else{
            holdQueue2.push(job);
            cout<<"job"<< job.jobNumber<<"placed in q 2\n";
        }
    }
    else{
        struct Process tmp;
        tmp.id = job.jobNumber;
        tmp.burstTimeRemaining = job.burstTime;
        tmp.neededMemory = job.memoryRequirement;
        tmp.devicesHeld = job.devicesRequirement;
        system->memory -= tmp.neededMemory;
        readyQueue.push(tmp);
    }
    return;
}

void handleDeviceRequest()
{
    // TODO
    cout << "handling device request" << endl;
    return;
}

void handleDeviceRelease()
{
    // TODO
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