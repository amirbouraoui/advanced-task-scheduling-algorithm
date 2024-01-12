#!/bin/bash

# OBJECTIF :
#
# Générer une collection d'instances pour un problème d'ordonnancement.

# PARAMÈTRES DU SCRIPT (optionnels) :
#
# -d Donner le nom du répertoire sur lequel les nouvelles instances vont
#	 être enregistrées.
# -j Les différentes instances générées avec ce script contiennent un nombre
#	 de tâches différent. Ce paramètre précise le nombre de tâches à
#	 utiliser dans un format MIN:MAX:PAS.
#	 Exemple : 20:40:10 va produire des instances avec 20, 30 et 40 tâches.
# -t Il s'agit d'un paramètre lié à la génération aléatoire des dates de
#	 libération de tâches. La valeur de ce paramètre varie pour les
#	 différentes instances et est donnée dans un format MIN:MAX:PAS.
#	 Exemple : 0.1:0.2:0.1 va générer des instances avec t=0.1 et 0.2.
# -i Donner le nombre de différentes instances qui vont être générées
#	 pour chaque combinaison de paramètres j et t.


# On modifie la valeur de la variable d'environnement LC_NUMERIC du shell
# afin d'éviter le problèmes éventuels dans les calculs de nombres flottants.
LC_NUMERIC=en_US.UTF-8


# FONCTION USAGE
#
# Affiche une aide sur l'utilisation du script et termine son exécution.

usage () {
	echo ""
	echo "NAME"
	echo -e "\t generate_collection\n"
	echo "DESCRIPTION"
	echo -e "\t Generates a collection of instances for a scheduling problem.\n"
	echo "USAGE"
	echo -e "\t bash generate_collection.sh [OPTIONS]\n"
	echo "OPTIONS"
	echo -e "\t -d DIR_NAME"
	echo -e "\t\t set the name of the created directory which will contain the collection;"
	echo -e "\t\t default value: collection\n"
	echo -e "\t -j MIN:MAX:STEP"
	echo -e "\t\t set the limites and the step concerning the number of jobs that will be"
	echo -e "\t\t created in the generated instances;"
	echo -e "\t\t default values: 10:100:10\n"
	echo -e "\t -t MIN:MAX:STEP"
	echo -e "\t\t set the limites and the step concerning the parameter t that will be"
	echo -e "\t\t used in the generation of the release dates;"
	echo -e "\t\t default values: 0.1:0.5:0.1\n"
	echo -e "\t -i NUM"
	echo -e "\t\t create NUM instances for each combination of the parameters j and t;"
	echo -e "\t\t default value: 10\n"
	echo -e "\t -h"
	echo -e "\t\t display this help and exit\n"
	echo "EXAMPLE"
	echo -e "\t bash generate_collection.sh -j 50:200:50 -d boo\n"
	echo -e "\t Creates in the directory boo 10 instances (default value) for each"
	echo -e "\t combination of parameters t={0.1,0.2,0.3,0.4,0.5} (default value)"
	echo -e "\t and j={50,100,150,200}\n"
	exit 1
}

# DÉFINITION DE PARAMÈTRES PAR DÉFAUT
#
# On définit d'abord les valeurs par défaut des paramètres qui vont être
# utilisées dans le cas où l'utilisateur ne donne pas la valeur de certains
# ou de tous les paramètres.

dir="collection"
nbjobsmin=10
nbjobsmax=100
nbjobsstep=10
tmin=0.1
tmax=0.5
tstep=0.1
nbinstances=10


# GESTION DE PARAMÈTRES DONNÉS PAR L'UTILISATEUR
#
# Cherchez et utilisez la commande "getopts" (et pas "getopt").
# L'objectif est de mettre éventuellement à jour les paramètres par défaut,
# si l'utilisateur a choisi d'utiliser des valeurs différentes.
#
# Supprimez le # dans les deux lignes suivantes :
#		usage
#		exit 0
#
# enregistrez et exécutez le script avec la commande :
# 	bash generate_collection.sh
#
# afin d'avoir une idée des différents cas à traiter.
# N'oubliez pas à remettre les # dans les deux lignes !

while getopts "d:j:t:i:h" option; do
	case $option in
		d) dir="$OPTARG" ;;
		j)
			IFS=":" read -ra nbjobs_values <<< "$OPTARG"
			nbjobsmin="${nbjobs_values[0]}"
			nbjobsmax="${nbjobs_values[1]}"
			nbjobsstep="${nbjobs_values[2]}"
			;;
		t)
			IFS=":" read -ra t_values <<< "$OPTARG"
			tmin="${t_values[0]}"
			tmax="${t_values[1]}"
			tstep="${t_values[2]}"
			;;
		i) nbinstances="$OPTARG" ;;
		h|*) usage ;;
	esac
done

# GÉNÉRATION DES INSTANCES ALÉATOIRES
#
# Créez dans le répertoire dir des instances aléatoires avec les paramètres
# déjà définis : pour chaque couple de valeurs ($nbjobs, $t), il faut créer
# $nbinstances instances en appelant le script generate_instance.sh.
#
# ATTENTION : NOM DU FICHIER DE CHAQUE INSTANCE
#
# Il faut obligatoirement respecter le format suivant :
#		nbjobs_t_instance
#
# Exemple : l'instance numéro 4 avec 20 tâches et t=0.3 sera enregistrer
# au fichier (sans suffixe, des espaces ou d'autres caractères) :
#		20_0.3_4

mkdir -p $dir

generate_instance_file=""

# Verifier si le fichier generate_instance.sh existe dans le repertoire courant
if [ -f ./generate_instance.sh ]; then
	generate_instance_file="./generate_instance.sh"
elif [ -f ./script/generate_instance.sh ]; then
	generate_instance_file="./script/generate_instance.sh"
else
	echo "Error: 'generate_instance.sh' not found!"
	exit 1
fi

# Default output: 500 Le fichier 'data/collection/100_0.5_10' a été créé avec succès.
for nbjobs in $(seq $nbjobsmin $nbjobsstep $nbjobsmax); do
	for t in $(seq -w $tmin $tstep $tmax); do
		for instance in $(seq 1 $nbinstances); do
			filename="${dir}/${nbjobs}_${t}_${instance}"
			bash $generate_instance_file "$filename" "$nbjobs" "$t"
		done
	done
done
