#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include <boost/mpi.hpp>
#include <boost/mpi/timer.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/program_options.hpp>

#include "launch.h"

using namespace std;

namespace mpi = boost::mpi;
namespace po = boost::program_options;


std::string
launch::parse_launch_options(int argc, char **argv)
{
    string taskfile;

    po::options_description options("Options");
    options.add_options()
            ("help,h", "show this help message");

    po::options_description args("Arguments");
    args.add_options()
            ("taskfile", po::value<string>(&taskfile), "tasks file");

    po::options_description desc;
    desc.add(args).add(options);

    po::positional_options_description p;
    p.add("taskfile", 1);

    po::variables_map vm;

    try {
        po::store(po::command_line_parser(argc, argv)
                          .options(desc)
                          .positional(p).run(), vm);

        if (vm.count("help") or !vm.count("taskfile")) {
            cout << "Usage: dextr [options] launch taskfile" << "\n";
            cout << "Arguments:" << "\n";
            cout << "  taskfile         file containing tasks to launch" << "\n";
            cout << options;
            exit(EXIT_FAILURE);
        }

        po::notify(vm);
    } catch (const po::error &e) {
        cerr << e.what() << "\n";
        exit(EXIT_FAILURE);
    }

    if (vm.count("taskfile")) {
        taskfile = vm["taskfile"].as<string>();
    }

    return taskfile;
}


std::vector<std::string>
launch::read_tasks(std::string taskfile)
{
    std::ifstream in(taskfile.c_str());
    if (!in) {
        cerr << "Cannot open " << taskfile << "\n";
        exit(EXIT_FAILURE);
    }

    vector<string> tasks;

    std::string line;
    while (std::getline(in, line)) {
        if (line.size() > 0)
            tasks.push_back(line);
    }

    in.close();

    return tasks;
}


void
launch::exec_task(std::string task, int rank)
{
    mpi::timer timer;
    int status = system(task.c_str());
    int elapsed_time = timer.elapsed();

    if (WIFEXITED(status)) {
        status = WEXITSTATUS(status);
        cout << "#executed by process " << rank
             << " in " << elapsed_time << " s"
             << " with status " << status
             << " : " << task << "\n";
    } else {
        if (WIFSIGNALED(status)) {
            status = WTERMSIG(status);
            cout << "#executed by process " << rank
                 << " in " << elapsed_time << " s"
                 << " killed by signal " << status
                 << " : " << task << "\n";
        }
    }
}


void
launch::run_cmd(std::string taskfile)
{
    mpi::environment env;
    mpi::communicator world;

    vector<vector<string>> chunks(world.size(), vector<string>(0));

    if (world.rank() == 0) {
        vector<string> tasks = read_tasks(taskfile);
        for (int i = 0; i < tasks.size(); ++i) {
            chunks.at(i % world.size()).push_back(tasks[i]);
        }
    }

    vector<string> received_tasks;
    mpi::scatter(world, chunks, received_tasks, 0);

    for (auto &task : received_tasks) {
        exec_task(task, world.rank());
    }
}
