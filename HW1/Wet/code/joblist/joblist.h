#ifndef JOBSLIST_H_
#define JOBSLIST_H_

#include <vector>
#include "external/external_command.h"

class JobsList {
public:
    class JobEntry {
        ExternalCommand* m_cmd;
        int m_jobId;
        int m_processId;
        public:
            JobEntry(ExternalCommand *cmd, int jobId, int processId) {
                m_cmd = cmd;
                m_jobId = jobId;
                m_processId = processId;
            }

            int getPID() {
                return m_processId;
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

    void addJob(ExternalCommand *cmd, bool isStopped = false) {
        removeFinishedJobs(); // removing finished jobs before adding

        int jobId = m_maxJobId+1;
        m_maxJobId++;

        JobEntry job(cmd, jobId, 0);
        m_listOfJobs.push_back(job);
    }

    void printJobsList() {
        removeFinishedJobs(); // removing finished jobs before printing
    }

    void killAllJobs();

    void removeFinishedJobs();

    JobEntry *getJobById(int jobId);

    void removeJobById(int jobId);

    JobEntry *getLastJob(int *lastJobId);

    JobEntry *getLastStoppedJob(int *jobId);

private:
    std::vector<JobEntry> m_listOfJobs;
    int m_numberOfJobsInList=0;
    int m_maxJobId = 0;
};

#endif //JOBSLIST_H_