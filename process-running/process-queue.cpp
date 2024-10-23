#include <iostream>
#include <queue>
#include <vector>

struct Process {
    int id;
    int numOfInstructions;
    int type; // 100 = CPU, 0 IO
    int startTime = 0;
};

const int I0_DURATION = 4; // Determines how long it takes a IO process to run

class ProcessScheduler {

    private:

        const int I0_DURATION = 4;

        std::queue<Process> blockedProcesses;
        std::queue<Process> readyProcesses;
        int timeElapsed = 0;
        int io_cycles = 0;
        int cpu_cycles = 0;


    public:

        void load(std::vector<Process>& processes) {

            std::cout << "Queuing " << processes.size() << " Processes!" << std::endl;
            for (const auto& process: processes){
                readyProcesses.push(process);
            }

            return;
        }

        void rebalance(){

            if (blockedProcesses.size() == 0){
                return;
            }

        }

        void run() {

            if (readyProcesses.size() == 0 && blockedProcesses.size() == 0){
                std::cout << "Processes Finished!" << std::endl;
                std::cout << "Stats: " << std::endl;
                std::cout << "CPU Cycles: " << cpu_cycles << std::endl;
                std::cout << "IO Cycles: " << io_cycles << std::endl;
                std::cout << "Total Cycles: " << timeElapsed << std::endl;
            }

            if (readyProcesses.size() == 0 && blockedProcesses.size() != 0){
                timeElapsed++;
                io_cycles++;
            }

            Process currentProcess = readyProcesses.front();
            readyProcesses.pop();

            if (currentProcess.type == 100){
                while (currentProcess.numOfInstructions > 0){
                    timeElapsed++;
                    cpu_cycles++;
                    currentProcess.numOfInstructions--;
                }
            }

            else {
                currentProcess.startTime = timeElapsed;
                blockedProcesses.push(currentProcess);
            }

            rebalance();
            run();

        }
};

int main() {
    ProcessScheduler scheduler;
    scheduler.run();
    return 0;
}