#include <iostream>
#include <queue>
#include <vector>

struct Process {
    int id;
    int numOfInstructions;
    int type;
};

const int I0_DURATION = 4; // Determines how long it takes a IO process to run

class ProcessScheduler {

    private:

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

        void run() {
            Process currentProcess = readyProcesses.front();
            readyProcesses.pop();
        }
};

int main() {
    ProcessScheduler scheduler;
    scheduler.run();
    return 0;
}