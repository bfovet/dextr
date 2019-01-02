#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include <boost/mpi.hpp>
#include <boost/mpi/timer.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/program_options.hpp>

#include "launch.h"
#include "version.h"

using namespace std;

namespace mpi = boost::mpi;
namespace po = boost::program_options;


void print_global_help(po::options_description options)
{
    cout << "Usage: " << "\n";
    cout << "  dextr [options] <command>" << "\n";
    cout << "Commands:" << "\n";
    cout << "  launch   execute tasks from taskfile" << "\n";
    cout << "  filter   filter tasks logs from logfile" << "\n";
    cout << options;
}


int main(int argc, char **argv)
{
    // Parse commands and global options
    po::options_description options("Options");
    options.add_options()
            ("version,v", "print version string")
            ("help,h", "show this help message");

    if (argc == 1) {
        print_global_help(options);
        return 1;
    }

    po::options_description args("Arguments");
    args.add_options()
            ("command", po::value<string>(), "command")
            ("subargs", po::value<vector<string> >(), "arguments for command");

    po::options_description desc;
    desc.add(args).add(options);

    po::positional_options_description pos;
    pos.add("command", 1);
    pos.add("subargs", -1);

    po::variables_map vm;

    po::parsed_options parsed = po::command_line_parser(argc, argv).
            options(desc).
            positional(pos).
            allow_unregistered().
            run();

    try {
        po::store(parsed, vm);

        if (vm.count("help") and !vm.count("command")) {
            print_global_help(options);
            return 1;
        }

        if (vm.count("version")) {
            cout << version << "\n";
            return 1;
        }

        po::notify(vm);
    } catch (const po::error &e) {
        cerr << e.what() << "\n";
        return 1;
    }

    string cmd = vm["command"].as<string>();
    if (cmd == "launch") {
        string taskfile = launch::parse_launch_options(argc-1, &argv[1]);
        launch::run_cmd(taskfile);
    } else if (cmd == "filter") {
        // TODO:
        //parse_filter_options();
        //filter();
        cout << "filter command not implemented" << "\n";
        return 1;
    } else {
        print_global_help(options);
        return 1;
    }

    return 0;
}
