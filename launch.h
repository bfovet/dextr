#ifndef DEXTR_VERSION_H
#define DEXTR_VERSION_H


namespace launch {
    std::string parse_launch_options(int, char **);
    std::vector<std::string> read_tasks(std::string);
    void exec_task(std::string, int);
    void run_cmd(std::string);
}

#endif //DEXTR_VERSION_H
