#!/bin/bash
for d in ../temi/20* ; do
    folder=$(basename $d)
    if [ ! -d "$folder" ]; then
        mkdir $folder
        ln -t $folder $d/*.pdf ;
    fi    
done;
for f in 20*/*tema.pdf ; do
    ln -s -r -t temi_all $f ;
done;
for f in 20*/*corr.pdf ; do
    ln -s -r -t corr_all $f ;
done;
git add .
msg="automatic commit from script update_archive.sh $(date +%Y-%m-%d:%Hh:%Mm:%Ss)"
git commit -m "$msg"
git push
