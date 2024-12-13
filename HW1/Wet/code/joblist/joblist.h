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
        JobEntry(const std::string& cmd_line)
            : m_cmd_line(cmd_line), m_jobId(-1), m_processId(-1) {}
        void setPID(int pid) {
            m_processId = pid;
        }

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

    JobsList() {
        m_listOfJobs = std::vector<JobEntry*>();
        m_maxJobId = 0;
    }
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
        int jobId = ++m_maxJobId;
        JobEntry* job = new JobEntry(cmd, jobId, pid);
        m_listOfJobs.push_back(job);
    }

    void printJobsList(Command* command) {
        removeFinishedJobs(); // removing finished jobs before printing
        for (JobEntry* jobEntry : m_listOfJobs) {
            jobEntry->printJob(command);
        }
    }

    void killAllJobs(Command* command) {
        for (JobEntry* jobEntry : m_listOfJobs) {
            int pid = jobEntry->getPID();
            kill(pid, SIGKILL);
            command->printOut(std::to_string(pid));
            command->printOut(": ");
            command->printOut(jobEntry->getCmdLine());
            command->printOut("\n");
        }
    }

    void removeFinishedJobs() {
        for (auto it = m_listOfJobs.begin(); it != m_listOfJobs.end();) {
            int status;
            JobEntry* job = *it;
            if (waitpid(job->getPID(), &status, WNOHANG) > 0) {
                delete job;
                it = m_listOfJobs.erase(it);
            } else {
                ++it;
            }
        }
        updateMaxJobId();
    }

    void updateMaxJobId() {
        m_maxJobId = 0;
        for (auto it = m_listOfJobs.begin(); it != m_listOfJobs.end(); ++it) {
            JobEntry* job = *it;
            int jobId = job->getId();
            m_maxJobId = std::max(m_maxJobId, jobId);
        }
    }

    JobEntry* getJobById(int jobId) {
        for (JobEntry* jobEntry : m_listOfJobs) {
            if (jobEntry->getId() == jobId) {
                return jobEntry;
            }
        }
        return nullptr;
    }

    JobEntry* removeJobById(int jobId) {
        for (auto it = m_listOfJobs.begin(); it != m_listOfJobs.end();) {
            JobEntry* job = *it;
            if (job->getId() == jobId) {
                m_listOfJobs.erase(it);
                updateMaxJobId();
                return job;
            }
            ++it;
        }
        return nullptr;
    }
private:
    std::vector<JobEntry*> m_listOfJobs;
    int m_maxJobId;
};

#endif //JOBSLIST_H_