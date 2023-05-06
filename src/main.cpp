/** @file main.cpp
 * Main driver file for the Operating systems project.
 */
// The previous block is needed in every file for which you want to generate documentation

#include <vector>
#include <queue>
#include <instruction.h>
#include <input.h>

using namespace std;

struct Process
{
    int id;
    int burstTimeRemaining;
    int neededMemory;
    int devicesHeld;
};

void handleJobArrival()
{
    // TODO
    cout << "Handling job arrival" << endl;
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

int main(int argc, char *argv[])
{
    vector<Instruction> instructions;
    System system;
    readInput("../input/i0.txt", instructions, &system);

    // Populate ready queue with dummy variables for testing
    queue<Process> readyQueue;
    for (int i = 0; i < 3; i++)
    {
        Process tmp;
        tmp.id = i;
        tmp.burstTimeRemaining = i * 5;
        tmp.neededMemory = 3;
        tmp.devicesHeld = 0;
        readyQueue.push(tmp);
    }
    Process *CPU = nullptr;

    // Main driver.
    int timeOfNextInput;
    int timeOfNextInternalEvent = system.quantum;

    int roundRobinIdx = 0;
    long unsigned int instructionIdx = 0;

    CPU = &readyQueue.front();
    readyQueue.pop();
    while (readyQueue.size())
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
        };

        // The time of next internal event will normally be the time quantum
        // unless the quantum will finish the burst time of the process, then it is the remaining time.
        if (CPU->burstTimeRemaining < system.quantum)
        {
            timeOfNextInternalEvent = CPU->burstTimeRemaining;
        }
        else
        {
            timeOfNextInternalEvent = system.quantum;
        }

        if (timeOfNextInput < timeOfNextInternalEvent)
        {
            // We will handle the instruction first
            // Jump the current time to the next input.
            system.currTime += timeOfNextInput;
            CPU->burstTimeRemaining -= timeOfNextInput;
            switch (instructions[instructionIdx].type)
            {
            case JA:
                handleJobArrival();
                break;
            case DRel:
                handleDeviceRelease();
                break;
            case DReq:
                handleDeviceRequest();
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

            CPU->burstTimeRemaining -= timeOfNextInternalEvent;
            // Remove the process from the queue.
            if (CPU->burstTimeRemaining == 0)
            {
                        }
            handleProcessTermination();
            roundRobinIdx = (roundRobinIdx + 1) % readyQueue.size();
        }
    }

    // print(system, instructions);
    return 0;
}