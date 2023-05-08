#ifndef UTILS_H
#define UTILS_H
#include <fstream>
#include <iostream>
#include <vector>
#include <instruction.h>

using namespace std;

struct Process
{
    int id;
    int burstTimeRemaining;
    int neededMemory;
    int devicesHeld;
};

struct Job
{
    int jobNumber;
    int arrival;
    int memoryRequirement;
    int devicesRequirement;
    int burstTime;
    int priority;
};

struct cmpQ1{
        bool operator()(struct Job a, struct Job b){
            return (a.burstTime < b.burstTime);
        }
    };

struct cmpQ2{
        bool operator()(struct Job a, struct Job b){
            return (a.arrival < b.arrival);
        }
    };


void handleJobArrival(struct Job job, priority_queue<struct Job, vector<struct Job>, cmpQ1> holdQueue1, priority_queue<struct Job, vector<struct Job>, cmpQ2> holdQueue2, queue<Process> readyQueue,System* system);

void handleDeviceRequest();

void handleDeviceRelease();
void handleDisplay();

void handleProcessTermination();

#endif