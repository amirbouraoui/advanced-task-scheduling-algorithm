![img](https://cdn.discordapp.com/attachments/485141053473685504/1224401999400009769/image.png?ex=66bf84b8&is=66be3338&hm=c6ff84ddf4cc232c7e7fae7bb59509cfecbb1e4f242374fc40a6a12eeeb946a1&)

# Synthesis Project [![Awesome](https://cdn.jsdelivr.net/gh/sindresorhus/awesome@d7305f38d29fed78fa85652e3a63e154dd8e8829/media/badge.svg)](https://github.com/amirbouraoui/advanced-task-scheduling-algorithm)
> This project implements a scheduling algorithm for a set of tasks to be executed on several machines. The scheduling algorithm can handle non-preemptive and preemptive scheduling with an optional balancing of the binary search tree used to store the tasks.

## Table of Contents
- [Installation and Documentation](#-installation-and-documentation)
   - [Prerequisites](#-prerequisites)
   - [Quick Installation Setup](#-quick-installation-setup)
   - [Getting Started](#-getting-started)
     - [Compiling and Testing the Application (Only C)](#compiling-and-testing-the-application-only-c)
     - [Compiling for JNI Modules](#compiling-for-jni-modules)
     - [Creating the libscheduling File for JNI](#creating-the-libscheduling-file-for-jni)
     - [Compiling Java](#compiling-java)
     - [Launching the JavaFX Interface](#launching-the-javafx-interface)
- [Scripts](#-scripts)
   - [generate_instances.sh](#generate_instancessh)
   - [generate_collection.sh](#generate_collectionsh)
   - [run_experiments.sh](#run_experimentssh)
- [Compile expe](#compile-expe)
- [Documentation](#-documentation)
   - [Compilation commands](#compile-commands)
   - [Cleaning controls](#cleaning-controls)
   - [Debugging commands](#debugging-commands)
- [Desciption of options](#desciption-of-options)
- [Project structures](#-project-structures)


# 🚀 Installation and Documentation
## ✅ Prerequisites
- [x] A C compiler (for exemple, [GCC](https://www.gnu.org/software/gcc/))
- [x] [GNU Make](https://www.gnu.org/software/make/) (preferred under Linux)
- [x] [JavaFX SDK](https://gluonhq.com/products/javafx/) 

## ⏳ Quick installation setup
For a fast setup, follow these steps:
- Replace <path-to-javafx-sdk\> with the actual path to your JavaFX SDK.
- Replace <os-name\> with your OS name/abbreviation. Accepted values are: **linux**, **mac**, **win**.
```bash
JAVAFX_SDK="<path-to-javafx-sdk>/lib"
OS="-<os-name>"
```
Then, copy and paste the following block into your terminal.
```bash
git clone https://gitlab.univ-lorraine.fr/bouraoui9u/projet-de-synthese.git
cd projet-de-synthese
mkdir -p obj
if [ "$OS" = "-linux" ]; then OS=""; fi
make -f "Makefile$OS" all
make -f "Makefile$OS" expe
make -f "Makefile$OS" jni
javac -h ./include java/src/application/Scheduling.java
cd java
javac --module-path lib:$JAVAFX_SDK -d bin src/**/*.java
java --module-path $JAVAFX_SDK --add-modules javafx.controls,javafx.fxml -cp "bin/:lib/*" application.Main
```
After running the above commands, the application should be running. If you encounter any issues, please refer to the [Getting Started](#-getting-started) section, where you will find more detailed instructions.

## 💾 Getting Started
Clone the project repository and navigate to the project directory with the following commands:
```bash
git clone https://gitlab.univ-lorraine.fr/bouraoui9u/projet-de-synthese.git
cd projet-de-synthese
```
### Compiling and testing the application (Only C)
You can run the application with the following command:
```bash
make run in=data/instance out=data/instance_out m=2 pre=1 bal=1
```

### Compiling for JNI Modules
To compile for JNI modules and create ./include/application_Scheduling.h, use the following command:
```bash
javac -h ./include java/src/application/Scheduling.java
```
### Creating the libscheduling File for JNI
Depending on your operating system, run the corresponding command to create the libscheduling file:
For Linux
```bash
make jni
```
For Mac
```bash
make -f Makefile-mac jni
```
For Windows
```bash
make -f Makefile-win jni
```

### Compiling Java
To compile Java with the JavaFX interface, first replace <path-to-javafx-sdk> with the actual path to your JavaFX SDK:
```bash
cd java
javac --module-path lib:<path-to-javafx-sdk>/lib -d bin src/**/*.java
```

### Launching the JavaFX Interface
To open the JavaFX interface via Terminal, first replace <path-to-javafx-sdk> with the actual path to your JavaFX SDK:
```bash
cd java
java --module-path <path-to-javafx-sdk>/lib --add-modules javafx.controls,javafx.fxml -cp "bin/:lib/*" application.Main
```
***
## 💫 Scripts
### generate_instances.sh
**generate_instances.sh** is a Bash script designed to generate an instance of the scheduling problem.

### Usage
You can execute the script by running the following command in the terminal:
```bash
bash generate_instance.sh [output_file] [number_of_tasks] [release_date]
```

#### Parameters
- **output_file**: The name of the file to be created.
- **number_of_tasks**: The number of tasks to be created.
- **release_date**: The parameter used in the generation of release dates.

#### Example
Here is an example command:
```bash
bash generate_instance.sh output.txt 100 0.2
```
This will create a new instance of a scheduling problem with 100 tasks. The generated instance will be written to a file named output.txt. The parameter used in generating the task release dates is 0.2.

#### Output
The generated instance will be written to a file with the following format:
```
job_id processing_time release_date
```
***
### generate_collection.sh
This script generates a collection of instances for a scheduling problem.

### Usage
You can execute the script by running the following command in the terminal:
```bash
bash generate_collection.sh [OPTIONS]
```

#### Parameters
The script accepts the following ***optional*** parameters:

**-d DIR_NAME**: The name of the directory where the new instances will be saved. Default is collection.
**-j MIN:MAX:STEP**: Defines the number of tasks to be used in the instances. The script generates instances with numbers of tasks ranging from MIN to MAX, in steps of STEP. For example, "20:40:10" will generate instances with 20, 30, and 40 tasks.
**-t MIN:MAX:STEP**: Defines the parameter related to the random generation of task release dates. It varies for different instances and is given in a MIN:MAX:STEP format. For example, "0.1:0.2:0.1" will generate instances with t=0.1 and 0.2.
**-i NUM**: The number of different instances to be generated for each combination of j and t parameters.

#### Example
Here is an example command:
```bash
bash generate_collection.sh -d myCollection -j 50:200:50 -t 0.1:0.5:0.1 -i 5
```
This command will create a collection of instances of a scheduling problem. The instances will be stored in a directory named myCollection. The script will generate instances with 50, 100, 150, and 200 tasks (as specified by -j 50:200:50). The parameter used in generating the task release dates will vary between 0.1 and 0.5 with a step of 0.1 (as specified by -t 0.1:0.5:0.1). For each combination of the number of tasks and release date parameter, 5 instances will be generated (as specified by -i 5).

All the parameters provided in the command are optional. If not provided, the script will use the default values.
***

### run_experiments.sh
**run_experiments.sh** is a Bash script designed to execute an experimental campaign over a collection of instances.

### Usage
```bash
bash run_experiments.sh [directory] [results_file] [min_machines] [max_machines] [step_machines]
```
#### Parameters
- **directory**: The name of the directory containing the instances on which the experiments will be run. The directory should be located in the same folder as the script, or a relative/absolute path to it should be provided.
- **results_file**: The name of the file where the results of the experiments will be written. If the file doesn't exist, the script will create it. If it does exist, the script will overwrite it.
- **min_machines**: The minimum number of machines to use in the experiments. This should be a positive integer.
- **max_machines**: The maximum number of machines to use in the experiments. This should be a positive integer greater than or equal to min_machines.
- **step_machines**: The step size for incrementing the number of machines to use. This means that the experiments will be run with min_machines, then min_machines + step_machines, and so on, up to max_machines.

#### Example
To run an experimental campaign on instances in the 'collection' directory with 1 to 100 machines in steps of 10, storing the results in 'results.txt', you would use:
```bash
bash run_experiments.sh collection results.txt 1 100 10
```

#### Output
The results of the experiments will be written to a file with the following format:
```
[T Value] [Instance Number] [Number of Machines] [Non-preemptive Non-balanced Makespan] [Non-preemptive Non-balanced CPU Time] [Non-preemptive Balanced Makespan] [Non-preemptive Balanced CPU Time] [Preemptive Non-balanced Makespan] [Preemptive Non-balanced CPU Time] [Preemptive Balanced Makespan] [Preemptive Balanced CPU Time]
```

###### Here is the output explaination:
- **T Value**: This is the ratio of the sum of all task times to the total available machine time.

- **Instance Number**: This number uniquely identifies the instance within the collection.

- **Number of Machines**: This is the count of machines used in each instance.

- **Non-preemptive Non-balanced Makespan**: This represents the total time from the start to the completion of all tasks for the non-preemptive, non-balanced tree configuration of the algorithm.

- **Non-preemptive Non-balanced CPU Time**: This is the time taken by the CPU to compute the schedule for the non-preemptive, non-balanced tree configuration of the algorithm.

- **Non-preemptive Balanced Makespan**: This represents the total time from the start to the completion of all tasks for the non-preemptive, balanced tree configuration of the algorithm.

- **Non-preemptive Balanced CPU Time**: This is the time taken by the CPU to compute the schedule for the non-preemptive, balanced tree configuration of the algorithm.

- **Preemptive Non-balanced Makespan**: This represents the total time from the start to the completion of all tasks for the preemptive, non-balanced tree configuration of the algorithm.

- **Preemptive Non-balanced CPU Time**: This is the time taken by the CPU to compute the schedule for the preemptive, non-balanced tree configuration of the algorithm.

- **Preemptive Balanced Makespan**: This represents the total time from the start to the completion of all tasks for the preemptive, balanced tree configuration of the algorithm.

- **Preemptive Balanced CPU Time**: This is the time taken by the CPU to compute the schedule for the preemptive, balanced tree configuration of the algorithm.
***
# Compile expe
```bash
make expe
```

The script `script/expe` can be used to run a series of tests on the scheduling algorithm. The script takes the following arguments:
```bash
./script/expe <num_machines> <num_instances> <num_tasks> <num_releases> <num_preemptions> <num_balanced> <num_runs>
```
The script will run the algorithm on a series of randomly generated instances and print the results to the file `data/expe.csv`. The script will also print the average execution time of the algorithm to the terminal.


## 📚 Documentation
### Compile commands
Commands| Explaination
:-----:|:-----:
`all`|Builds the executable `PROG` (`scheduling`).
`run`|Builds the executable `PROG` and launches it with specified arguments.
`test`|Builds the executable `TEST` (`runtest`) and launches it.
`jni`|Builds the Java interface library `LIB` (`java/libscheduling.so`).
`expe`|Builds the executable `EXPE` (`script/expe`).

### Cleaning controls
Commands| Explaination
:-----:|:-----:
`cleanall`|Cleans all files (obj, executables, and libraries).
`clean`|Cleans object files in the `obj` directory.
`delete`|Deletes the executable `PROG`.
`deletetest`|Deletes the executable `TEST`.
`deletelib`|Deletes the library `LIB`.
`deleteexpe`|Deletes the executable `EXPE`.


### Debugging commands
Commands| Explaination
:-----:|:-----:
`memorycheck`|Builds `PROG` and launches `valgrind` to check for memory errors.
`testmemorycheck`|Builds `TEST` and launches it with `valgrind` to check for memory errors.


## Desciption of options
Commands/Option| Description
:-----:|:-----:
`make run`|Run the program with the specified options
`in=inputfile`|Name of the input file
`out=outputfile`|Name of the output file
`m=machines`|Number of machines
`pre=preemption`|Preemption option
`bal=balanced`|Balanced tree option

Balanced Tree Options| Description
:-----:|:-----:
`0`|Use a binary search tree
`1`|Use a balanced binary search tree

Preemption Options| Description
:-----:|:-----:
`0`|Without preemption
`1`|With preemption

## 🛠 Project structures
`struct task_t` Represents a task in the scheduling problem.
* `int id` The task identifier.
* `unsigned long release_time` The task's release time (rj).
* `unsigned long processing_time` The task's processing time (pj).

`struct ready_task_key_t` Represents a ready task key used in the ready task queue (a binary search tree).
* `unsigned long remaining_processing_time` The remaining processing time of the task.
* `char *task_id` The identifier of the task associated with the ready task.
***
`struct schedule_t` Represents the global schedule of tasks on parallel machines.
* `struct list_t **schedule` An array of pointers to the list_t structure, where each element represents the schedule of a machine.
* `int num_machines` The number of available machines.

`struct schedule_node_t` Represents a node in a machine's schedule.
* `struct task_t *task` Pointer to the task assigned to this node.
* `unsigned long begin_time` The time at which the task starts execution.
* `unsigned long end_time` Time when the task finishes execution.
***
`struct list_t` Doubly linked list structure used to represent an instance of the scheduling problem and to store machine schedules.
* `struct list_node_t * head, * tail` Pointers to the head and tail of the list.
* `int numelm` The number of elements in the list.
* `void (*viewData)(const void * data)` A function pointer to a function that prints the data stored in the list nodes.
* `void (*freeData)(void * data)` A function pointer to a function that frees the data stored in the list nodes.

`struct list_node_t` Represents a node in a doubly linked list. It is used as part of the struct list_t data structure.
* `void *data` A pointer to the data stored in the list node. It can be a pointer to any of the previously mentioned structures, like task_t or schedule_node_t.
* `struct list_node_t *successor` Pointer to the next node in the list.
* `struct list_node_t *predecessor` Pointer to the previous node in the list.
***
`struct tree_t` A binary search tree (balanced or unbalanced) used to represent the event queue and the ready task queue.
* `struct tree_node_t * root` Pointer to the root of the tree.
* `int numelm` The number of elements in the tree.
* `int (*preceed)(const void * a, const void * b)` A function pointer to a function that compares two elements of the tree. It returns 1 if the first element precedes the second, 0 otherwise.
* `void (*viewKey)(const void * key)` A function pointer to a function that prints the key stored in the tree nodes.
* `void (*viewData)(const void * data)` A function pointer to a function that prints the data stored in the tree nodes.
* `void (*freeKey)(void * key)` A function pointer to a function that frees the key stored in the tree nodes.
* `void (*freeData)(void * data)`  A function pointer to a function that frees the data stored in the tree nodes.
* `int balanced` A flag indicating whether the tree is balanced or not.

`struct tree_node_t` Represents a node in a binary search tree. It is used as part of the struct tree_t data structure.
* `void *key` Pointer to the key used to sort the tree nodes. It can be a pointer to any key structure, such as event_key_t or ready_task_key_t.
* `void *data` A pointer to the data stored in the tree node. It can be a pointer to any of the previously mentioned structures, like task_t or schedule_node_t.
* `int bfactor` The balance factor of the node. It is used in balanced binary search trees (AVL trees) to maintain balance during insert and delete operations.
* `struct tree_node_t *left` A pointer to the left node of the tree.
* `struct tree_node_t *right` A pointer to the right node of the tree.
***
`struct event_key_t` Represents an event key used in the event queue (a binary search tree).
* `int event_type` The type of event (0 for task launch, 1 for task completion).
* `unsigned long event_time` The time of the event.
* `unsigned long processing_time` The processing time of the task associated with the event.
* `char *task_id` The identifier of the task associated with the event.
* `int machine` The machine associated with the event (for task completion events).

## 📘 License
University of Lorraine - Faculty of Mathematics, Computer Science, Mechanics (MIM) - 2023
