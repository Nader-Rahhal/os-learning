#include <iostream>
#include <queue>
#include <vector>

struct Process {
    int id;
    int numOfInstructions;
    int type; // 100 = CPU, 0 IO
    int startTime = 0;
    
    Process(int _id, int _instructions, int _type, int _startTime = 0)  : id(_id), numOfInstructions(_instructions), type(_type), startTime(_startTime){}

};

class ProcessScheduler {

    private:

        const int IO_DURATION = 4;
        std::vector<Process> blockedProcesses;
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

        void rebalance() {

            if (blockedProcesses.empty()) {
                return;
            }

            auto it = blockedProcesses.begin();
            
            while (it != blockedProcesses.end()) {

                if (it->startTime + IO_DURATION == timeElapsed) {
                    Process completedProcess = *it;
                    completedProcess.type = 100;
                    readyProcesses.push(completedProcess);
                    it = blockedProcesses.erase(it);
                } 
                else {
                    ++it;
                }
            }
        
            if (!blockedProcesses.empty()) {
                io_cycles++;
            }

        }

        void run() {

            while (!readyProcesses.empty() || !blockedProcesses.empty()) {

                if (readyProcesses.empty()) {
                
                    timeElapsed++;
                    io_cycles++;
                    rebalance();
                    continue;
                }

                Process currentProcess = readyProcesses.front();
                readyProcesses.pop();

                if (currentProcess.type == 100) {
               
                    currentProcess.numOfInstructions--;
                    timeElapsed++;
                    cpu_cycles++;
                
                    if (currentProcess.numOfInstructions > 0) {
                        readyProcesses.push(currentProcess);
                    }
                }
                
                else {
                    timeElapsed++;
                    cpu_cycles++;
                    currentProcess.startTime = timeElapsed;
                    blockedProcesses.push_back(currentProcess);
                }
            
                rebalance();
            }

            std::cout << "Processes Finished!" << std::endl;
            std::cout << "Stats: " << std::endl;
            std::cout << "CPU Cycles: " << cpu_cycles << std::endl;
            std::cout << "IO Cycles: " << io_cycles << std::endl;
            std::cout << "Total Cycles: " << timeElapsed << std::endl;

        }
};

int main() {

    ProcessScheduler scheduler;

    std::vector<Process> processes = {
        Process(1, 3, 0),    // IO process
        Process(2, 5, 100),  // CPU process
        Process(3, 2, 0)     // Another IO process
    };
    
    scheduler.load(processes);
    scheduler.run();
    
    return 0;
}