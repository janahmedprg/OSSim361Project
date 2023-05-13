#include <utils.h>

using namespace std;

void handleJobArrival(struct Job job, priority_queue<struct Job, vector<struct Job>, cmpQ1>& holdQueue1, priority_queue<struct Job, 
                      vector<struct Job>, cmpQ2>& holdQueue2, queue<Job>& readyQueue,System* system)
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
        system->memory -= job.memoryRequirement;
        readyQueue.push(job);
    }
    return;
}

bool bankersAlgo(int k, vector<int> alloc, vector<int> maxReq, vector<int> need, int work){
    
    vector<bool> finish(k,false);
    while(true){
        bool flag = false;
        for(int i = 0; i<k+1; ++i){
            if(finish[i] == false && need[i]<=work){
                work += alloc[i];
                finish[i] = true;
                flag = false;
            }
        }
        if (flag){
            break;
        }
    }
    bool safe = true;
    for (int i = 0; i<k; ++i){
        safe = safe & finish[i]; 
    }
    return safe;
}

bool handleDeviceRequest(DeviceRequest req, queue<Job>& waitQueue, queue<Job>& readyQueue,Job* CPU, System* system)
{
    // Run banker's algo -> it will return a boolean value
    // If true:
    // update the number of devices that the job currently holds
    // reduce the number of devices that the system has to give
    // If false: move job to end of the waiting queue
    int work = system->devices - req.deviceNumber;
    int k = readyQueue.size()+1;
    queue<Job> tmpCopy = readyQueue;
    vector<int> alloc;
    vector<int> maxReq;
    vector<int> need;
    for (int i = 0; i<k-1; ++i){
        alloc.push_back(tmpCopy.front().devicesHeld);
        maxReq.push_back(tmpCopy.front().devicesRequirement);
        need.push_back(maxReq[i] - alloc[i]);
        tmpCopy.pop(); 
    }
    alloc.push_back(CPU->devicesHeld + req.deviceNumber);
    maxReq.push_back(CPU->devicesRequirement);
    need.push_back(CPU->devicesRequirement - (CPU->devicesHeld + req.deviceNumber));

    bool safe = bankersAlgo(k, alloc, maxReq, need, work);

    CPU->devicesHeld += req.deviceNumber;
    if(safe){
        system->devices -= req.deviceNumber;
        readyQueue.push(*CPU);
    }
    else{
        waitQueue.push(*CPU);
    }
    CPU = nullptr; 
    return safe;
}

void handleDeviceRelease(DeviceRelease req, queue<Job>& waitQueue, queue<Job>& readyQueue, Job* CPU, System* system)
{
    // Update the number of devices the job currently holds
    // increase the number of devices the system has to give
    // Loop through the waiting queue
    // And run bankers algorithm on for each job that banker's algo returns true for
    // move back to the ready queue. 
    // If a job can be moved from the waiting to the ready queue, then you need to have similar logic to the device request.
    // Still need to increase the number of devices that the job holds
    // Reduce the number of devices system has to give. 
    system->devices += req.deviceNumber;
    CPU->devicesHeld -= req.deviceNumber;

    queue<Job> tmpWait = waitQueue;
    queue<Job> newWaitQueue;
    int n = tmpWait.size();

    while(!tmpWait.empty()){
        int work = system->devices - tmpWait.front().devicesHeld;
        int k = readyQueue.size()+1;
        queue<Job> tmpCopy = readyQueue;
        vector<int> alloc;
        vector<int> maxReq;
        vector<int> need;
        for (int i = 0; i<k-1; ++i){
            alloc.push_back(tmpCopy.front().devicesHeld);
            maxReq.push_back(tmpCopy.front().devicesRequirement);
            need.push_back(maxReq[i] - alloc[i]);
            tmpCopy.pop(); 
        }
        alloc.push_back(CPU->devicesHeld);
        maxReq.push_back(CPU->devicesRequirement);
        need.push_back(CPU->devicesRequirement - CPU->devicesHeld);

        alloc.push_back(tmpWait.front().devicesHeld);
        maxReq.push_back(tmpWait.front().devicesRequirement);
        need.push_back(tmpWait.front().devicesRequirement - tmpWait.front().devicesHeld);

        bool safe = bankersAlgo(k+1, alloc, maxReq, need, work);

        if(safe){
            readyQueue.push(tmpWait.front());
            system->devices -= tmpWait.front().devicesHeld;
        }
        else{
            newWaitQueue.push(tmpWait.front());
        }

        tmpWait.pop();
    }
    waitQueue = newWaitQueue;
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