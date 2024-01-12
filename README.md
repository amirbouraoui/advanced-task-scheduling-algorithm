![gaze](https://cdn.discordapp.com/attachments/485141053473685504/1092788691551932536/image.png)

# Projet De Synthèse [![Awesome](https://cdn.jsdelivr.net/gh/sindresorhus/awesome@d7305f38d29fed78fa85652e3a63e154dd8e8829/media/badge.svg)](https://gitlab.univ-lorraine.fr/bouraoui9u/projet-de-synthese/-/blob/main/README.md)
> Ce projet implémente un algorithme d'ordonnancement pour un ensemble de tâches à exécuter sur plusieurs machines. L'algorithme d'ordonnancement peut gérer l'ordonnancement non préemptif et préemptif avec un équilibrage facultatif de l'arbre de recherche binaire utilisé pour stocker les tâches.

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
- [Documentation (in French)](#-documentation-in-french)
   - [Commandes de compilation](#commandes-de-compilation)
   - [Commandes de nettoyage](#commandes-de-nettoyage)
   - [Commandes de débogage](#commandes-de-débogage)
- [Desciption des options](#desciption-des-options)
- [Structures du projet](#-structures-du-projet)
- [Auteurs](#-auteurs)


# 🚀  Installation and Documentation
## ✅ Prerequisites
- [x] A C compiler (for exemple, [GCC](https://www.gnu.org/software/gcc/))
- [x] [GNU Make](https://www.gnu.org/software/make/) (préférence sous Linux)
- [x] [JavaFX SDK](https://gluonhq.com/products/javafx/) 

## ⏳ Quick  installation setup
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


## 📚 Documentation (in French)
### Commandes de compilation
Commande| Explication
:-----:|:-----:
`all`|Construit l'exécutable `PROG` (`scheduling`).
`run`|Construit l'exécutable `PROG` et le lance avec des arguments spécifiés.
`test`|Construit l'exécutable `TEST` (`runtest`) et le lance.
`jni`|Construit la bibliothèque d'interface Java `LIB` (`java/libscheduling.so`).
`expe`|Construit l'exécutable `EXPE` (`script/expe`).

### Commandes de nettoyage
Commande| Explication
:-----:|:-----:
`cleanall`|Nettoye tous les fichiers (obj, exécutables et les bibliothèques).
`clean`|Nettoye les fichiers objets dans le répertoire `obj`.
`delete`|Supprime l'exécutable `PROG`.
`deletetest`|Supprime l'exécutable `TEST`.
`deletelib`|Supprime la bibliothèque `LIB`.
`deleteexpe`|Supprime l'exécutable `EXPE`.


### Commandes de débogage
Commande| Explication
:-----:|:-----:
`memorycheck`|Construit `PROG` et lance `valgrind` pour vérifier les erreurs de mémoire.
`testmemorycheck`|Construit `TEST` et le lance avec `valgrind` pour vérifier les erreurs de mémoire.


## Desciption des options
Commande/Option| Description
:-----:|:-----:
`make run`|Exécutez le programme avec les options spécifiées
`in=fichierentrée`|Nom du fichier d'entrée
`out=fichiersortie`|Nom du fichier de sortie
`m=machines`|Nombre de machines
`pre=préemption`|Option de préemption
`bal=équilibré`|Option d'arbre équilibré

Options d'arbre équilibré| Description
:-----:|:-----:
`0`|Utiliser un arbre de recherche binaire
`1`|Utiliser un arbre de recherche binaire équilibré

Options de préemption| Description
:-----:|:-----:
`0`|Sans préemption
`1`|Avec préemption

## 🛠 Structures du projet
`struct task_t` Représente une tâche dans le problème d'ordonnancement.
* `int id` L'identifiant de la tâche.
* `unsigned long release_time` L'heure de lancement de la tâche (rj).
* `unsigned long processing_time` Le temps de traitement de la tâche (pj).

`struct ready_task_key_t` Représente une clé de tâche prête utilisée dans la file d'attente des tâches prêtes (un arbre de recherche binaire).
* `unsigned long remaining_processing_time` Le temps de traitement restant de la tâche.
* `char *task_id` L'identifiant de la tâche associée à la tâche prête.
***
`struct schedule_t` Représente le calendrier global des tâches sur les machines parallèles.
* `struct list_t **schedule` Un tableau de pointeurs vers la structure list_t, où chaque élément représente la programmation d'une machine.
* `int num_machines` Le nombre de machines disponibles.

`struct schedule_node_t` Représente un nœud dans le planning d'une machine.
* `struct task_t *task` Pointeur sur la tâche assignée à ce nœud.
* `unsigned long begin_time` L'heure à laquelle la tâche commence à être exécutée.
* `unsigned long end_time` Heure de fin d'exécution de la tâche.
***
`struct list_t` Structure de liste doublement chainées utilisée pour représenter une instance du problème d'ordonnancement et pour stocker les horaires des machines.
* `struct list_node_t * head, * tail` Pointeurs vers la tête et la queue de la liste.
* `int numelm` Le nombre d'éléments de la liste.
* `void (*viewData)(const void * data)` Un pointeur de fonction vers une fonction qui imprime les données stockées dans les nœuds de la liste.
* `void (*freeData)(void * data)` Un pointeur de fonction vers une fonction qui libère les données stockées dans les nœuds de la liste.

`struct list_node_t` Représente un nœud dans une liste doublement chainées. Il est utilisé dans le cadre de la structure de données struct list_t.
* `void *data` Un pointeur sur les données stockées dans le nœud de liste. Il peut s'agir d'un pointeur sur l'une des structures mentionnées précédemment, comme task_t ou schedule_node_t.
* `struct list_node_t *successor` Pointeur sur le nœud suivant de la liste.
* `struct list_node_t *predecessor` Pointeur sur le nœud précédent de la liste.
***
`struct tree_t` Un arbre de recherche binaire (équilibré ou déséquilibré) utilisé pour représenter la file d'attente des événements et la file d'attente des tâches prêtes.
* `struct tree_node_t * root` Pointeur sur la racine de l'arbre.
* `int numelm` Le nombre d'éléments dans l'arbre.
* `int (*preceed)(const void * a, const void * b)` Un pointeur de fonction vers une fonction qui compare deux éléments de l'arbre. Elle renvoie 1 si le premier élément précède le second, 0 sinon.
* `void (*viewKey)(const void * key)` Un pointeur de fonction vers une fonction qui imprime la clé stockée dans les nœuds de l'arbre.
* `void (*viewData)(const void * data)` Un pointeur de fonction vers une fonction qui imprime les données stockées dans les nœuds de l'arbre.
* `void (*freeKey)(void * key)` Un pointeur de fonction vers une fonction qui libère la clé stockée dans les nœuds de l'arbre.
* `void (*freeData)(void * data)` Un pointeur de fonction vers une fonction qui libère les données stockées dans les nœuds de l'arbre.
* `int balanced` Un drapeau indiquant si l'arbre est équilibré ou non.

`struct tree_node_t` Représente un nœud dans un arbre de recherche binaire. Il est utilisé dans le cadre de la structure de données struct tree_t.
* `void *key` Pointeur sur la clé utilisée pour trier les nœuds de l'arbre. Il peut s'agir d'un pointeur sur n'importe quelle structure de clé, telle que event_key_t ou ready_task_key_t.
* `void *data` Un pointeur sur les données stockées dans le nœud de l'arbre. Il peut s'agir d'un pointeur sur l'une des structures mentionnées précédemment, comme task_t ou schedule_node_t.
* `int bfactor` Le facteur d'équilibre du nœud. Il est utilisé dans les arbres de recherche binaires équilibrés (arbres AVL) pour maintenir l'équilibre pendant les opérations d'insertion et de suppression.
* `struct tree_node_t *left` Un pointeur sur le nœud gauche de l'arbre.
* `struct tree_node_t *right` Un pointeur sur le nœud droit de l'arbre.
***
`struct event_key_t` Représente une clé d'événement utilisée dans la file d'attente des événements (un arbre de recherche binaire).
* `int event_type` Le type d'événement (0 pour le lancement d'une tâche, 1 pour la fin d'une tâche).
* `unsigned long event_time` L'heure de l'événement.
* `unsigned long processing_time` Le temps de traitement de la tâche associée à l'événement.
* `char *task_id` L'identifiant de la tâche associée à l'événement.
* `int machine` La machine associée à l'événement (pour les événements de fin de tâche).

## 📘 License
Université de Lorraine - UFR Mathématiques, informatique, mécanique (MIM) - 2023
