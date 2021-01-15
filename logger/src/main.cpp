#include <iostream>
#include <string>
#include <pthread.h>
#include "yaml-cpp/yaml.h"
#include "ThreadableMsgQueue.hpp"
#include "monitor.hpp"

using namespace::std;

/// The default name of the logger configuration file
const string DEFAULT_CONFIG_FILE = "logger.conf";

/// setting this boolean as true will output debugging statements to
/// the standard output stream
const bool LOGGER_DEBUG = true;

/// Global queue to be accessed by socket monitoring threads and
/// decoder thread
CThreadableMsgQueue decoderQueue;

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


    // loop through the specified interfaces to grab the number of required monitoring threads
    int socketCount = 0;
    if (config["interfaces"]) {

        for (YAML::const_iterator iface = config["interfaces"].begin(); iface != config["interfaces"].end(); ++iface) {

            if (LOGGER_DEBUG) std::cout << "Found " << iface->as<string>() << " in config." << endl;
            socketCount++;
        }

    } else {
        cerr << "ERROR: You must configure at least one interface to log!" << endl;
        exit( 1 );
    }


    // loop through the configured CAN interfaces and create threads for their monotors
    pthread_t monitorThreads[socketCount];
    MonitorParams paramsArray[socketCount];
    int currentThreadIndex = 0;
    for (YAML::const_iterator iface = config["interfaces"].begin(); iface != config["interfaces"].end(); ++iface) {

        // populate the parameters for the current interface's monitoring thread
        paramsArray[currentThreadIndex].iface_name = iface->as<string>();
        paramsArray[currentThreadIndex].queue      = &decoderQueue;
        
        // create the thread
        if (pthread_create(&monitorThreads[currentThreadIndex], NULL, monitor, (void *)(&paramsArray[currentThreadIndex]))) {
            cerr << "ERROR: Failed to create " <<  iface->as<string>() << " monitoring thread!" << endl;
            exit( 1 );
        }

    }


    // join the producer threads
    for (long n = 0; n < socketCount; n++) {
        if (pthread_join(monitorThreads[n], NULL)) {
            cerr << "ERROR: Failed to join thread #" <<  n << "!" << endl;
            exit( 2 );
        } 
    }


    return 0;
}