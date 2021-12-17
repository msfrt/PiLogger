#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>

#include <pthread.h>
#include <signal.h>
#include <semaphore.h>

#include <linux/can.h>
#include <linux/can/raw.h>

#include <memory>

#include "yaml-cpp/yaml.h"
#include "Message.hpp"
#include "ThreadableQueue.hpp"
#include "monitor.hpp"
#include "consumer.hpp"
#include "interfaces.hpp"


using namespace::std;

/// The default name of the logger configuration file
const string DEFAULT_CONFIG_FILE = "logger.conf";

/// The default max write size for batch write metrics. Configurable in the config file
const int DEFAULT_MAX_WRITE_SIZE = 5000;

/// The default write max write delay in milliseconds
const int DEFAULT_MAX_WRITE_DELAY = 5000;

/// setting this boolean as true will output debugging statements to
/// the standard output stream
extern const bool LOGGER_DEBUG = true;

/// This number probably shouldn't be messed with. It's just so that we don't have
/// to deal with dynamic array allocation and whatnot
const int MAX_THREADS = 100;

/// Global queue to be accessed by socket monitoring threads and
/// decoder thread
CThreadableMsgQueue<shared_ptr<CMessage>> decoder_queue;

/// global vector to store the pthread objects for monitoring threads
pthread_t monitoring_threads[MAX_THREADS];

/// A count for the number of monitoring threads contained in the monitoring
/// thread array
int monitor_count = 0;

/// global for threads to monitor and see if they need to exit
bool stop_logging = false;

/// A semaphore that will be used to restrict printing to the standard output
/// stream. This is useful when debugging many threads. Doesn't do
/// much if you're not debugging.
sem_t stdout_sem;


/**
 * A simple function that joins threads when given an array of thread IDs
 * \param threads The vector of thread IDs
 */
int join_threads(pthread_t threads[], int count){
    for (long n = 0; n < count; n++) {
        if (pthread_join(threads[n], NULL)) {
            sem_wait(&stdout_sem);
            cerr << "ERROR: Failed to join thread #" <<  n << "!" << endl;
            sem_post(&stdout_sem);
            return 2;
        } 
    }
    return 0;
}


/**
 * Signal callback handler to catch runtime signals such as CTRL-C to quit.
 * https://www.tutorialspoint.com/how-do-i-catch-a-ctrlplusc-event-in-cplusplus
 * \param signum The number that corresponds to the signal (2 is CTRL-C)
 */
void signal_callback_handler(int signum) {
   if (LOGGER_DEBUG) {
       sem_wait(&stdout_sem);
       cout << "Caught signal " << signum << endl;
       sem_post(&stdout_sem);
   }

   switch (signum)
   {
   case 2:
        stop_logging = true;
        join_threads(monitoring_threads, monitor_count);

        // Terminate program
        exit(signum);
   
   default:
       break;
   }
   
}





int main(int argc, char *argv[])
{

    // register the handler function for user signals (such as CTRL-C)
    // https://www.tutorialspoint.com/how-do-i-catch-a-ctrlplusc-event-in-cplusplus
    signal(SIGINT, signal_callback_handler);


    // initialize stdout semaphore to 1, since we only want one thread printing at a time
    sem_init(&stdout_sem, 0, 1);


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


    // Get the database information
    DBInfo dbinfo;

    if (config["database"]) {

        auto dbinfo_node = config["database"];

        try {
            dbinfo.host = dbinfo_node["host"].as<std::string>();
            dbinfo.port = dbinfo_node["port"].as<int>();
            dbinfo.org = dbinfo_node["org"].as<std::string>();
            dbinfo.bucket = dbinfo_node["bucket"].as<std::string>();
            dbinfo.token = dbinfo_node["token"].as<std::string>();

        } catch (const std::exception& ex) {

            cerr << "ERROR: The database configuration parameters were either missing or incorrectly formatted!" << endl;
            exit( 1 );

        }

        

    } else {
        cerr << "ERROR: You must configure the database!" << endl;
        exit( 1 );
    }


    // optional configuration options in the config file
    int max_write_size = DEFAULT_MAX_WRITE_SIZE;
    try {
        max_write_size  = config["max_write_size"].as<int>();
        if (LOGGER_DEBUG) std::cout << "Max write size found in config: " << max_write_size << endl;
    } catch (const std::exception& ex) {}

    int max_write_delay = DEFAULT_MAX_WRITE_DELAY;
    try {
        max_write_delay  = config["max_write_delay"].as<int>();
        if (LOGGER_DEBUG) std::cout << "Max write delay found in config: " << max_write_delay << endl;
    } catch (const std::exception& ex) {}


    // loop through the specified interfaces to grab the number of required monitoring threads and DBC info
    unordered_map<Interface, string> bus_dbc_name_map;  /// will hold the interface names and the respective dbcs
    unordered_map<Interface, string> bus_name_map;  /// will hold the interface names and their given names (ex. can0 -> "CAN1")
    if (config["interfaces"]) {

        auto ifaces_node = config["interfaces"];

        for (YAML::const_iterator it=ifaces_node.begin(); it!=ifaces_node.end();++it) {
            monitor_count++;
            
            // first is the interface name for socketcan
            string iface = it->first.as<std::string>();

            // info node contains bus name and dbc path
            auto iface_info = it->second;

            // extract info from the socketcan YAML node
            string name = iface_info["name"].as<std::string>();
            string dbc = iface_info["dbc"].as<std::string>();

            if (LOGGER_DEBUG) std::cout << "Found " << iface << " named " << name << " with dbc " << dbc << " in config." << endl;

            // add the interface name and the dbc name to the map
            Interface current_iface = string_to_iface(iface);
            bus_name_map[current_iface] = name;
            bus_dbc_name_map[current_iface] = dbc;
            
        }


    } else {
        cerr << "ERROR: You must configure at least one interface to log!" << endl;
        exit( 1 );
    }



    // read in the queue configuration and initialize it
    if (config["queue_size"]){
        decoder_queue.SetBufferSize(config["queue_size"].as<int>());
        if (LOGGER_DEBUG) cout << "Queue buffer size set to " << config["queue_size"].as<int>() << endl;
    } else {
        if (LOGGER_DEBUG) cout << "Using default queue buffer size of " << decoder_queue.GetBufferSize() << endl;
    }
    decoder_queue.Initialize();



    // loop through the configured CAN interfaces and create threads for their monotors
    MonitorParams mon_params_ary[monitor_count];
    int current_thread_idx = 0;
    for (const auto & [ iface, dbc_path ] : bus_dbc_name_map) {

        // populate the parameters for the current interface's monitoring thread
        mon_params_ary[current_thread_idx].iface_name = iface;
        mon_params_ary[current_thread_idx].queue      = &decoder_queue;
        
        // create the thread
        if (pthread_create(&monitoring_threads[current_thread_idx], NULL, monitor, (void *)(&mon_params_ary[current_thread_idx]))) {
            sem_wait(&stdout_sem);
            cerr << "ERROR: Failed to create " <<  iface_to_string(iface) << " monitoring thread!" << endl;
            sem_post(&stdout_sem);
            exit( 1 );
        }

        current_thread_idx++;

    }



    // create the consumer thread
    ConsumerParams con_params;
    con_params.bus_name_map = bus_name_map;
    con_params.bus_dbc_file_map = bus_dbc_name_map;
    con_params.queue = &decoder_queue;
    con_params.dbinfo = dbinfo;
    con_params.max_write_size = max_write_size;
    con_params.max_write_delay = max_write_delay;
    pthread_t consumer_thread;
    if (pthread_create(&consumer_thread, NULL, consumer, (void *)(&con_params))) {
        sem_wait(&stdout_sem);
        cerr << "ERROR: Failed to create the consumer thread!" << endl;
        sem_post(&stdout_sem);
        exit( 1 );
    }




    // at this point, we're done, join the producer threads after they've exited here
    join_threads(monitoring_threads, monitor_count);

    // join the consumer
    if (pthread_join(consumer_thread, NULL)) {
        sem_wait(&stdout_sem);
        cerr << "ERROR: Failed to join the consumer!" << endl;
        sem_post(&stdout_sem);
        return 2;
    } 


    return 0;
}




