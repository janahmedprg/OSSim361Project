#include <utils.h>
#include <algorithm>

using namespace std;

bool cmpJobArr(pair<Job,int> a, pair<Job,int> b){
            return (a.first.arrival < b.first.arrival);
}

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
        bool flag = true;
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
        safe = safe && finish[i]; 
    }
    return safe;
}

void handleDeviceRequest(DeviceRequest req, queue<Job>& waitQueue, queue<Job>& readyQueue,Job*& CPU, System* system)
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
    return;
}

void handleDeviceRelease(DeviceRelease req, queue<Job>& waitQueue, queue<Job>& readyQueue, Job*& CPU, System* system)
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

    while(!tmpWait.empty()){
        int work = system->devices - tmpWait.front().devicesHeld;
        int k = readyQueue.size()+1;
        queue<Job> tmpCopy = readyQueue;
        vector<int> alloc;
        vector<int> maxReq;
        vector<int> need;
        // Populating the bankers algorithm helper vectors.
        for (int i = 0; i<k-1; ++i){
            alloc.push_back(tmpCopy.front().devicesHeld);
            maxReq.push_back(tmpCopy.front().devicesRequirement);
            need.push_back(maxReq[i] - alloc[i]);
            tmpCopy.pop(); 
        }
        // Add the process also currently on the CPU
        alloc.push_back(CPU->devicesHeld);
        maxReq.push_back(CPU->devicesRequirement);
        need.push_back(CPU->devicesRequirement - CPU->devicesHeld);

        // Assume we were to add the device in the wait queue
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

void handleDisplay(queue<Job>& waitQueue,priority_queue<struct Job, vector<struct Job>, cmpQ1>& holdQueue1,
                   priority_queue<struct Job, vector<struct Job>, cmpQ2>& holdQueue2,
                   queue<Job>& readyQueue, Job*& CPU, System* system, vector<pair<Job,int>>& doneArr)
{
    double systemTurn = 0;
    sort(doneArr.begin(),doneArr.end(),cmpJobArr);
    cout<<"At time "<<system->currTime<<":\n";
    cout<<"Current Available Main Memory="<<system->memory<<"\n";
    cout<<"Current Devices="<<system->devices<<"\nCompleted Jobs:\n";
    cout<<"--------------------------------------------------------\n";
    cout<<"Job ID    Arrival Time    Finish Time    Turnaround Time\n";
    cout<<"========================================================\n";
    for(auto x:doneArr){
        systemTurn += (double)(x.second - x.first.arrival);
        cout<<"   "<<x.first.jobNumber<<"           "<<x.first.arrival<<"               "<<x.second<<"              "<<x.second - x.first.arrival<<"\n";
    }
    systemTurn = systemTurn/(double)doneArr.size();
    cout<<"\n";
    cout<<"Hold Queue 1:\n-------------------------\nJob ID    Run Time\n=========================\n";
    priority_queue<struct Job, vector<struct Job>, cmpQ1> tmpHQ1 = holdQueue1;
    while(!tmpHQ1.empty()){
        cout<<"  "<<tmpHQ1.top().jobNumber<<"         "<<tmpHQ1.top().burstTime<<"\n";
        tmpHQ1.pop();
    }

    cout<<"\n";
    cout<<"Hold Queue 2:\n-------------------------\nJob ID    Run Time\n=========================\n";
    priority_queue<struct Job, vector<struct Job>, cmpQ2> tmpHQ2 = holdQueue2;
    while(!tmpHQ2.empty()){
        cout<<"  "<<tmpHQ2.top().jobNumber<<"         "<<tmpHQ2.top().burstTime<<"\n";
        tmpHQ2.pop();
    }

    cout<<"\n";
    cout<<"Ready Queue:\n----------------------------------\nJob ID    Run Time    Time Accrued\n==================================\n";
    queue<Job> tmpRQ = readyQueue;
    while(!tmpRQ.empty()){
        cout<<"  "<<tmpRQ.front().jobNumber<<"         "<<tmpRQ.front().burstTime<<"             "<<tmpRQ.front().origBTime - tmpRQ.front().burstTime<<"\n";
        tmpRQ.pop();
    }
    
    cout<<"\n";
    cout<<"Process running on CPU:\n-----------------------------------\nJob ID    Time Accrued    Time Left\n===================================\n";
    if(CPU != nullptr){
        cout<<"   "<<CPU->jobNumber<<"           "<<CPU->origBTime-CPU->burstTime<<"             "<<CPU->burstTime<<"\n";
    }
    cout<<"\n";

    cout<<"Wait Queue:\n----------------------------------\nJob ID    Run Time    Time Accrued\n==================================\n";
    queue<Job> tmpWQ = waitQueue;
    while(!tmpWQ.empty()){
        cout<<"  "<<tmpWQ.front().jobNumber<<"         "<<tmpWQ.front().burstTime<<"             "<<tmpWQ.front().origBTime - tmpWQ.front().burstTime<<"\n";
        tmpWQ.pop();
    }
    cout<<"\n";

    if(system->currTime == 9999){
        cout<<"System Turnaround Time: "<<systemTurn<<"\n";
    }
    return;
}

void handleProcessTermination(queue<Job>& waitQueue,priority_queue<struct Job, vector<struct Job>, cmpQ1>& holdQueue1,
                              priority_queue<struct Job, vector<struct Job>, cmpQ2>& holdQueue2,
                              queue<Job>& readyQueue, Job*& CPU, System* system, vector<pair<Job,int>>& doneArr)
{
    DeviceRelease dRelease;
    dRelease.jobNumber = CPU->jobNumber;
    dRelease.deviceNumber = CPU->devicesHeld;
    handleDeviceRelease(dRelease, waitQueue, readyQueue, CPU, system);
    system->memory += CPU->memoryRequirement;
    if(!holdQueue1.empty()){
        if(system->memory >= holdQueue1.top().memoryRequirement){
            readyQueue.push(holdQueue1.top());
            system->memory -= holdQueue1.top().memoryRequirement;
            holdQueue1.pop();
        }
    }
    else if (!holdQueue2.empty()){
        if (system->memory >= holdQueue2.top().memoryRequirement){
            readyQueue.push(holdQueue2.top());
            system->memory -= holdQueue2.top().memoryRequirement;
            holdQueue2.pop();
        }
    }
    doneArr.push_back({*CPU, system->currTime});
    return;
}