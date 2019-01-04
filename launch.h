#ifndef DEXTR_LAUNCH_H
#define DEXTR_LAUNCH_H


namespace launch {
    std::string parse_launch_options(int, char **);
    std::vector<std::string> read_tasks(std::string);
    void exec_task(std::string, int);
    void run_cmd(std::string);
}

#endif //DEXTR_LAUNCH_H
