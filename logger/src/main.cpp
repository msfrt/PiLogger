#include <iostream>
#include <string>
#include "yaml-cpp/yaml.h"
#include "ThreadableMsgQueue.hpp"

using namespace::std;

/// The default name of the logger configuration file
const string DEFAULT_CONFIG_FILE = "logger.conf";

CThreadableMsgQueue queue;

int main(int argc, char *argv[])
{

    string config_file = DEFAULT_CONFIG_FILE;
    for (int i = 1; i < argc; i++){

        if (string(argv[i]) == "-f" && i < argc - 1) {
            config_file = argv[i + 1];

        } else if (string(argv[i]) == "-f") {
            cerr << "ERROR: No config filename was included!" << endl;
            exit ( 1 );
        }

    }

    // load config file into a yaml node
    YAML::Node config;
    try {
        config = YAML::LoadFile(config_file);
    } catch (YAML::BadFile) {
        cerr << "ERROR: Bad configuration file: " << config_file << endl;
        exit ( 1 );
    }

    if (config["busses"]) {
        for (YAML::const_iterator bus = config["busses"].begin(); bus != config["busses"].end(); ++bus) {
            std::cout << bus->as<string>() << "\n";
        }
    } else {
        cerr << "ERROR: You must configure at least one bus to log!" << endl;
        exit( 1 );
    }

    return 0;
}