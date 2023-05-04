/** @file main.cpp
 * Main driver file for the Operating systems project.
 */
// The previous block is needed in every file for which you want to generate documentation

#include <vector>
#include <instruction.h>
#include <input.h>

using namespace std;

int main(int argc, char* argv[])
{
    vector<Instruction> instructions;
    System system;
    readInput("../input/i0.txt", instructions, &system);
    print(system, instructions);
    return 0;
}