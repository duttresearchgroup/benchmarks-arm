
read -p "HPC user: " USER
#read -s -p "Password: " PASSWORD

TGTDIR="/dfs1/drg/tmuck/shared_data/benchmarks_arm/benchmarks"

#get a list of files to be updated
#  call rsynch
#  filter-out new files
#  get file name of modified files

LIST=$(rsync --dry-run -ai $USER@hpc.oit.uci.edu:$TGTDIR . | grep -v "+++++++++" | awk '{print $2}')

if [ "$LIST" ] ; then
    
    echo "The following files will be overwriten:"
    for file in $LIST
    do
        echo $file
    done
  
    read -r -p "Continue? [y/N] " response
    if [[ $response =~ ^([nN][oO]|[nN])$ ]]
    then
        exit
    fi  
fi

#do the thing
rsync -av $USER@hpc.oit.uci.edu:$TGTDIR .


