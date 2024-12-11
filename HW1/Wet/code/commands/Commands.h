#ifndef SMASH_COMMAND_H_
#define SMASH_COMMAND_H_

#define COMMAND_MAX_LENGTH (200)
#define COMMAND_MAX_ARGS (20)

class Command {
public:
    Command() : m_isRedirection(false) {}
    ~Command()=default;

    virtual void execute() = 0;

    //virtual void prepare();
    //virtual void cleanup();

    void setRedirectionFile(const std::string& file, bool overideContent) {
        // init file according to m_redirectionFile and m_overrideRedirection
        m_redirectionFile = file;
        m_overrideRedirection = overideContent;
    }

    void printOut(const std::string& str) {
        if (m_isRedirection) {
            // do print to file
        }
        else {
            std::cout << str;
        }
    }

private:
    bool m_isRedirection;
    std::string m_redirectionFile;
    bool m_overrideRedirection; // symbols of redirection (> or >>)
};

#endif //SMASH_COMMAND_H_
