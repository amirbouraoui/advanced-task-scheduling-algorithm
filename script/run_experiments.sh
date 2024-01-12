#!/bin/bash

# OBJECTIF :
#
# Réaliser une campagne expérimentale sur une collection des instances.

# PARAMÈTRES DU SCRIPT :
#
# $1 nom du répertoire contenant la collection des instances
# $2 nom du fichier avec les résultats
# $3 nombre minimum des machines à utiliser
# $4 nombre maximum des machines à utiliser
# $5 pas du nombre des machines à utiliser


# On modifie la valeur de la variable d'environnement LC_NUMERIC du shell
# afin d'éviter le problèmes éventuels dans les calculs de nombres flottants.
LC_NUMERIC=en_US.UTF-8


# GESTION DE PARAMÈTRES
#
# Testez si le nombre de paramètres est correct et affichez un message dans
# le cas contraire.
# Assurez vous que le premier paramètre correspond bien à un répertoire.

if [ "$#" -ne 5 ]; then
	echo "Usage: $0 <dir> <filename> <machine_min> <machine_max> <machine_step>"
	exit 1
fi

if [ ! -d "$1" ]; then
	echo "Error: $1 n'est pas un répertoire!"
	exit 1
fi

if [ ! -e "expe" ]; then
    echo "Error: 'expe' n'existe pas! Utilisez 'make expe' pour le compiler."
    exit 1
fi

# Affectation des paramètres aux variables.

# Note: realpath is not available on Mac OS by default (brew install coreutils)
dir=`realpath $1`	# nom du répertoire contenant la collection des instances 
fname=$2
m_min=$3
m_max=$4
m_step=$5

# EXÉCUTION DE L'EXPÉRIENCE
#
# Pour toutes les instances de la collection et pour chaque nombre de machines
# de $m_min à $m_max avec un pas de $m_step, exécutez le programme "./expe".
# Pour chaque exécution, vous devez écrire une ligne dans le fichier $fname
# avec 12 colonnes :
# Colonne 1 : nombre des tâches de l'instance
# Colonne 2 : paramètre t de l'instance
# Colonne 3 : numéro de l'instance avec ces paramètres
# Colonne 4 : nombre des machines de l'exécution
# Colonnes 5-12 : résultats obtenus par l'exécution du programme "./expe"
# Les colonnes sont séparées par une espace.

echo -n "" > $fname # vider le fichier

for instance_file in $dir/*; do
    file_name=$(basename $instance_file)
    IFS='_' read -ra file_parts <<< "$file_name"
    task_count="${file_parts[1]}"
    t_param="${file_parts[2]}"
    instance_num="${file_parts[3]}"

    for m in $(seq $m_min $m_step $m_max); do
        result=$(./expe "$instance_file" $m)
        echo "$task_count $t_param $instance_num $m $result" >> $fname
    done
done

echo "Experimental campaign complete. Results are in '$fname'."