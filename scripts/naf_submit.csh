#!/bin/csh -f

if ( $#argv < 4 ) then
   echo Need to give the sample name, the macro name, the config file name, and the number of files per job, optional json
   exit
endif

set rootfilelist = $1
set macro = $2
set config = $3
set nsplit = $4
set json = ""

if ( $#argv == 5 ) then
   set json = $5
endif

set json = $5

set maindir = "Condor_"$macro"_"`basename $config .cfg`

if ( -d $maindir ) then
   echo "$maindir exist, rename or remove it and then resubmit" 
   exit
endif

mkdir $maindir

# split the ntuples in a temporary file
set tmpdir = ".splitdir_"$$
mkdir $tmpdir
cp -p $rootfilelist $tmpdir
cd $tmpdir
split.csh $nsplit $rootfilelist
set files = `/bin/ls "."*_x????.txt`
cd -

foreach file ( $files )

   set counter = `basename $file .txt | awk -F "x" '{print $2}'`
   set exedir = $maindir"/job_"$counter
   if ( -d $exedir ) then
      echo "Similar jobs were already submitted. Move or remove directories and resubmit"
      exit
   endif
   mkdir -p $exedir
   cd $exedir
   mv ../../$tmpdir/$file ./rootFileList.txt
   cp -p ../../$config .
   if ( $json != "" ) then
      cp -p ../../$json .
   endif
   condor_submit.csh "job_"$counter $macro `basename $config`
   sleep 0.2
   cd -
end

rm -fR $tmpdir

exit




#./qsub.sh $exeName $sampleName
