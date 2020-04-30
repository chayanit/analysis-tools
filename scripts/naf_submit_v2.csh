#!/bin/csh -f

if ( $#argv < 3 ) then
   echo 'Need to give the parameters in this order:'
   echo 'Macro name, sample file name (e.g.rootFileList.txt), number of files per job'
   echo 'Optionally: configuration file name and json file name'
   exit
endif

set macro = $1
set rootfilelist = $2
set nsplit = $3
set config = ""
set json = ""


if ( $#argv == 4 ) then
   set config = $4
endif

if ( $#argv == 5 ) then
   set config = $4
   set json = $5
endif

set maindir = "Condor_"$macro
if ( $config != "" ) then
   set maindir = "Condor_"$macro"_"`basename $config .cfg`
endif


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
   mv ../../$tmpdir/$file ./$rootfilelist
   if ( $json != "" ) then
      cp -p ../../$json .
   endif
   if ( $config == "" ) then
      condor_submit.csh "job_"$counter $macro
   else
      cp -p ../../$config .
      condor_submit.csh "job_"$counter $macro `basename $config`
   endif
   
   sleep 0.2
   cd -
end

rm -fR $tmpdir

exit




#./qsub.sh $exeName $sampleName
