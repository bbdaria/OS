#ifndef JOBSLIST_H_
#define JOBSLIST_H_

#include <vector>
#include <sys/wait.h>

#include "smash/smash.h"
#include "commands/Commands.h"

class JobsList {
public:
    class JobEntry {
        std::string m_cmd_line;
        int m_jobId;
        int m_processId;
    public:
        JobEntry(const std::string& cmd_line, int jobId, int processId)
            : m_cmd_line(cmd_line), m_jobId(jobId), m_processId(processId) {}

        int getPID() {
            return m_processId;
        }

        int getId() {
            return m_jobId;
        }

        std::string& getCmdLine() {
            return m_cmd_line;
        }

        void printJob(Command* command) {
            command->printOut("[");
            command->printOut(std::to_string(m_jobId));
            command->printOut("] ");
            command->printOut(m_cmd_line);
            command->printOut("\n");
        }
    };

    JobsList()=default;
    ~JobsList()=default;

    bool contains(int jobId) {
        return getJobById(jobId) != nullptr;
    }

    bool empty() {
        return m_listOfJobs.empty();
    }

    int getMaxId() {
        return m_maxJobId;
    }

    int size() {
        return m_listOfJobs.size();
    }

    void addJob(const char* cmd, int pid, bool isStopped = false) {
        removeFinishedJobs(); // removing finished jobs before adding

        int jobId = m_maxJobId+1;
        m_maxJobId++;
        JobEntry job(cmd, jobId, pid);
        m_listOfJobs.push_back(job);
    }

    void printJobsList(Command* command) {
        removeFinishedJobs(); // removing finished jobs before printing
        for (JobEntry& jobEntry : m_listOfJobs) {
            jobEntry.printJob(command);
        }
    }

    void killAllJobs(Command* command) {
        for (JobEntry& jobEntry : m_listOfJobs) {
            int pid = jobEntry.getPID();
            kill(pid, SIGKILL);
            command->printOut(std::to_string(pid));
            command->printOut(": ");
            command->printOut(jobEntry.getCmdLine());
            command->printOut("\n");
        }
    }

    void removeFinishedJobs() {
        for (auto it = m_listOfJobs.begin(); it != m_listOfJobs.end();) {
            int status;
            if (waitpid(it->getPID(), &status, WNOHANG) > 0) {
                it = m_listOfJobs.erase(it);
            } else {
                ++it;
            }
        }
    }

    JobEntry* getJobById(int jobId) {
        for (JobEntry& jobEntry : m_listOfJobs) {
            if (jobEntry.getId() == jobId) {
                return &jobEntry;
            }
        }
        return nullptr;
    }

    void removeJobById(int jobId) {
        for (auto it = m_listOfJobs.begin(); it != m_listOfJobs.end(); ++it) {
            if (it->getId() == jobId) {
                m_listOfJobs.erase(it);
                return;
            }
        }
    }
    
    JobEntry *getLastJob(int *lastJobId);

    JobEntry *getLastStoppedJob(int *jobId);

private:
    std::vector<JobEntry> m_listOfJobs;
    int m_maxJobId = 0;
};

#endif //JOBSLIST_H_