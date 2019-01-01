#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include <boost/mpi.hpp>
#include <boost/mpi/timer.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/program_options.hpp>

using namespace std;

namespace mpi = boost::mpi;
namespace po = boost::program_options;


int main(int argc, char **argv) {

    std::string taskfile;

    po::options_description options("Options");
    options.add_options()
            ("version,v", "print version string")
            ("help,h", "show this help message");

    po::options_description args("Arguments");
    args.add_options()
            ("taskfile", po::value<string>(&taskfile)->required(), "tasks file");

    po::options_description desc;
    desc.add(args).add(options);

    po::positional_options_description p;
    p.add("taskfile", 1);

    po::variables_map vm;

    try {
        po::store(po::command_line_parser(argc, argv)
                    .options(desc)
                    .positional(p).run(), vm);

        if (vm.count("help") or argc == 1) {
            cout << "Usage: " << argv[0] << " [options] taskfile" << "\n";

            cout << "Arguments:" << "\n";
            cout << "  taskfile         file containing tasks to launch" << "\n";

            cout << options;
            return 1;
        }

        po::notify(vm);
    } catch (const po::error &e) {
        cerr << e.what() << "\n";
        return 1;
    }

    if (vm.count("taskfile")) {
        taskfile = vm["taskfile"].as<string>();
    }

    mpi::environment env;
    mpi::communicator world;

    // Read tasks
    std::ifstream in(taskfile.c_str());
    if (!in) {
        cerr << "Cannot open " << taskfile << "\n";
        return 1;
    }

    vector<string> tasks;

    std::string line;
    while (std::getline(in, line)) {
        if (line.size() > 0)
            tasks.push_back(line);
    }

    in.close();

    // Generate world.size() chunks of tasks
    vector<vector<string>> chunks(world.size(), vector<string>(0));
    for (int i = 0; i < tasks.size(); ++i) {
        chunks.at(i % world.size()).push_back(tasks[i]);
    }

    vector<string> received_tasks;
    mpi::scatter(world, chunks, received_tasks, 0);

    // Run tasks : launch command
    for (auto &task : received_tasks) {
        mpi::timer *time = new mpi::timer();
        int status = system(task.c_str());
        int elapsed = time->elapsed();

        if (WIFEXITED(status)) {
            status = WEXITSTATUS(status);
            cout << "#executed by process " << world.rank()
                 << " in " << elapsed << " s"
                 << " with status " << status
                 << " : " << task << "\n";
        } else {
            if (WIFSIGNALED(status)) {
                status = WTERMSIG(status);
                cout << "#executed by process " << world.rank()
                     << " in " << elapsed << " s"
                     << " killed by signal " << status
                     << " : " << task << "\n";
            }
        }
    }

    // TODO : filter command

    return 0;
}
