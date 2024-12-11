#ifndef JOBSLIST_H_
#define JOBSLIST_H_

#include <vector>
#include "external/external_command.h"

class JobsList {
public:
    class JobEntry {
        std::string m_cmd;
        int m_jobId;
        int m_processId;
    public:
        JobEntry(const std::string& cmd, int jobId, int processId)
            : m_cmd(cmd), m_jobId(jobId), m_processId(processId) {}

        int getPID() {
            return m_processId;
        }

        int getId() {
            return m_jobId;
        }

        std::string& getCmd() {
            return m_cmd;
        }

        void printJob() {
            std::cout << "[" << m_jobId << "] " << m_cmd << std::endl;
        }

        void printForegroundJob() {
            std::cout << m_cmd << m_processId;
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

    void addJob(const char* cmd, int pid, bool isStopped = false) {
        removeFinishedJobs(); // removing finished jobs before adding

        int jobId = m_maxJobId+1;
        m_maxJobId++;
        JobEntry job(cmd, jobId, pid);
        m_listOfJobs.push_back(job);
    }

    void printJobsList() {
        removeFinishedJobs(); // removing finished jobs before printing
        for (JobEntry& jobEntry : m_listOfJobs) {
            jobEntry.printJob();
        }
    }

    void killAllJobs() {
        std::cout << SmallShell::getInstance().getStartPrompt() << ": sending SIGKILL";
        std::cout << " signal to " << m_listOfJobs.size() << " jobs:" << std::endl;
        for (JobEntry& jobEntry : m_listOfJobs) {
            int pid = jobEntry.getPID();
            kill(pid, SIGKILL);
            std::cout << pid << ": " << jobEntry.getCmd() << std::endl;
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

    void JobsList::removeJobById(int jobId) {
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