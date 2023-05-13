/** @file main.cpp
 * Main driver file for the Operating systems project.
 */
// The previous block is needed in every file for which you want to generate documentation

#include <vector>
#include <queue>
#include <instruction.h>
#include <input.h>
#include <utils.h>

using namespace std;


int main(int argc, char *argv[])
{
    vector<Instruction> instructions;
    System system;
    readInput("../input/i0.txt", instructions, &system);
    int totalMemory = system.memory;
    int totalDevices = system.devices;

    queue<Job> readyQueue;
    queue<Job> waitQueue;
    priority_queue<struct Job, vector<struct Job>, cmpQ1> holdQueue1;
    priority_queue<struct Job, vector<struct Job>, cmpQ2> holdQueue2;

    Job *CPU = nullptr;

    // Main driver.
    int timeOfNextInput;
    int timeOfNextInternalEvent = system.quantum;
    long unsigned int instructionIdx = 0;
    long currQuantum = system.quantum;

    bool running = true;


    while (running)
    {
        // The time of next input should be infinite if there are none left,
        // otherwise it is the time of next instruction - current time.
        if (instructionIdx < instructions.size())
        {
            timeOfNextInput = instructions[instructionIdx].time - system.currTime;
        }
        else
        {
            timeOfNextInput = INT16_MAX;
        }

        // The time of next internal event will normally be the time quantum
        // unless the quantum will finish the burst time of the process, then it is the remaining time.
        if (CPU != nullptr && CPU->burstTime < currQuantum)
        {
            timeOfNextInternalEvent = CPU->burstTime;
        }
        else
        {
            timeOfNextInternalEvent = currQuantum;
        }

        if (timeOfNextInput < timeOfNextInternalEvent)
        {
            // We will handle the instruction first
            // Jump the current time to the next input.
            system.currTime += timeOfNextInput;
            currQuantum -= timeOfNextInput;
            if(CPU != nullptr){
                CPU->burstTime -= timeOfNextInput;
            }
            switch (instructions[instructionIdx].type)
            {
            case JA:
                Job tmp;
                tmp.jobNumber = instructions[instructionIdx].data.jobArrival.jobNumber;
                tmp.arrival = instructions[instructionIdx].time;
                tmp.memoryRequirement = instructions[instructionIdx].data.jobArrival.memoryRequirement;
                tmp.devicesRequirement = instructions[instructionIdx].data.jobArrival.devicesRequirement;
                tmp.burstTime = instructions[instructionIdx].data.jobArrival.burstTime;
                tmp.priority = instructions[instructionIdx].data.jobArrival.priority;
                if(totalDevices>=tmp.devicesRequirement && totalMemory>= tmp.memoryRequirement){
                    handleJobArrival(tmp, holdQueue1, holdQueue2, readyQueue, &system);
                }
                break;
            case DRel:
            if (instructions[instructionIdx].data.deviceRelease.jobNumber == CPU->jobNumber && 
                (instructions[instructionIdx].data.deviceRelease.deviceNumber)<=CPU->devicesHeld){
                    handleDeviceRelease(instructions[instructionIdx].data.deviceRelease, waitQueue, readyQueue, CPU, &system);
                    currQuantum = system.quantum;
                }
                break;
            case DReq:
                if (instructions[instructionIdx].data.deviceRequest.jobNumber == CPU->jobNumber && 
                (instructions[instructionIdx].data.deviceRequest.deviceNumber+CPU->devicesHeld)<=CPU->devicesRequirement){
                    bool safe = handleDeviceRequest(instructions[instructionIdx].data.deviceRequest, waitQueue, readyQueue, CPU, &system);
                    currQuantum = system.quantum;
                }
                break;
            case Display:
                handleDisplay();
                break;
            default:
                cout << "Erroneous type value of event!" << endl;
            }
            // Update the instructionIdx, we are now looking forward to the next instruction.
            instructionIdx += 1;
        }
        else
        {
            // We are handling the internal event first (this includes the case that they are at the same time).
            // Jump forward to the end of the quantum.
            system.currTime += timeOfNextInternalEvent;
            if (CPU != nullptr){
                CPU->burstTime -= timeOfNextInternalEvent;
            }
            // Remove the process from the queue.
            if (CPU != nullptr && CPU->burstTime == 0)
            {
                readyQueue.pop();
                if(readyQueue.size()>0){
                    CPU = &readyQueue.front();
                }
                else{
                    CPU = nullptr;
                }
                handleProcessTermination();
            }
            else
            {
                if(CPU != nullptr){
                    readyQueue.push(*CPU);
                }
                if(readyQueue.size() != 0){
                    CPU = &readyQueue.front();
                    readyQueue.pop();
                }
            }
            currQuantum = system.quantum;
        }
    }

    // print(system, instructions);
    return 0;
}