#!/bin/bash

# OBJECTIF :
#
# Créer un fichier contenant une nouvelle instance du problème
# d'ordonnancement générée aléatoirement.

# PARAMÈTRES DU SCRIPT :
#
# $1 nom du fichier à créer
# $2 nombre de tâches à créer
# $3 paramètre utilisé dans la génération de dates de libération


# On modifie la valeur de la variable d'environnement LC_NUMERIC du shell
# afin d'éviter le problèmes éventuels dans les calculs de nombres flottants.
LC_NUMERIC=en_US.UTF-8


# FONCTION getNormalDistributionSample
#
# Génère de façon aléatoire un entier dans l'intervalle [min,max]
# selon la distribution normale.
#
# PARAMÈTRES : deux entiers correspondants aux limites de l'intervalle.

getNormalDistributionSample () {
	min=$1
	max=$2
	echo $(( $min + ($RANDOM % ($max-$min+1)) ))
}


# FONCTION getParetoDistributionSample
#
# Génère de façon aléatoire un entier selon la distribution de Pareto.

getParetoDistributionSample () {
	alpha=1.1
	lambda=100

	u=$(echo "($RANDOM+1)/32768" | bc -l)

	# x = lambda * (1-u)^(-1/alpha)
	x=$(echo "$lambda * e((-1/$alpha)*l(1-$u))" | bc -l)
	echo $x | awk '{printf("%.0f\n",$1)}'
}


# GESTION DE PARAMÈTRES
#
# Testez si le nombre de paramètres est correct et affichez un message dans
# le cas contraire.
# Affectez les paramètres aux variables.

if [ "$#" -ne 3 ]; then
	echo "Usage: $0 <fichier_de_sortie> <nombres_de_taches> <date_de_libération>"
	exit 1
fi

# verifiy if $2 and $3 are numbers
if ! [[ $2 =~ ^[0-9]+$ ]]; then
	echo "'$2' is not a number!"
	exit 1
fi	
if ! [[ $3 =~ ^[0-9]*\.?[0-9]+$ ]]; then
	echo "'$3' is not a number!"
	exit 1
fi

# Verify if $2 and $3 are positive
if [ $2 -le 0 ]; then
	echo "'$2' must be a positive integer!"
	exit 1
fi
if [ $(echo "$3 <= 0" | bc -l) -eq 1 ]; then
	echo "'$3' must be a positive number!"
	exit 1
fi

output_file=$1
number_of_tasks=$2
release_date=$3

# GÉNÉRATION DE DURÉES OPÉRATOIRES ALÉATOIRES
#
# Dans la suite, cherchez et utilisez la commande "seq".
# La valeur de la durée opératoire de chaque tâche est basée sur la
# distribution Pareto. Pour la générer aléatoirement, il faut appeler la
# fonction getParetoDistributionSample.

processing_times=() # liste vide

for i in $(seq 1 $number_of_tasks); do
	processing_times+=($(getParetoDistributionSample))
done

# GÉNÉRATION DE DATES DE LIBÉRATION ALÉATOIRES
#
# La valeur de la date de libération de chaque tâche est basée sur la
# distribution normale. Une fois que les durées opératoires sont générées,
# il faut calculer leur somme. En suite la fonction
# getNormalDistributionSample doit être appelée afin de générer
# aléatoirement une valeur dans l'intervalle [min,max]=[0, t*somme], ou t
# correspond au troisième paramètre du script.

sum_of_processing_time=0

for processing_time in ${processing_times[@]};
do
	sum_of_processing_time=$(($sum_of_processing_time + $processing_time))
done

release_dates=()

for i in $(seq 1 $number_of_tasks)
do
	gen_release_date=$(getNormalDistributionSample 0 $(echo "($release_date * $sum_of_processing_time)/1" | bc))
	release_dates+=($gen_release_date)
done

# ÉCRITURE DES DONNÉES GÉNÉRÉES AU FICHIER
#
# Format du fichier à créer (chaque ligne) :
# 		job_id processing_time release_date

echo -n "" > $output_file # vider le fichier

for i in $(seq 1 $number_of_tasks); do
	job_id=$(printf "%02d" $i)
	processing_time=${processing_times[$(($i-1))]}
	release_date=${release_dates[$(($i-1))]}
    if [ $i -lt $number_of_tasks ]; then
        echo "$job_id $processing_time $release_date" >> $output_file
    else
        # If this is the last line, do not add a newline character
        echo -n "$job_id $processing_time $release_date" >> $output_file
    fi
done

echo "Le fichier '$output_file' a été créé avec succès!"