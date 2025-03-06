#!/bin/bash
#!/bin/bash
set -e
echo
echo "Sono lo script che pubblica sul repo temiRO_publish gli esiti di un singolo appello precisato in input (argomento obbligatorio: yyyy-mm-dd)."
echo
echo "Usage: $0 DATA_APPELLO"
echo "Usage: dove DATA_APPELLO è nel formato 20??-??-?? ossia yyyy-mm-dd"
if [ "$#" -gt 1 ]; then
    echo "Esco senza fare nulla perchè mi hai chiamato con più di un argomento"		    
    exit 1
fi
if [ "$#" -lt 1 ]; then
    echo "Esco senza fare nulla perchè mi hai chiamato senza argomenti"		    
    exit 1
fi
DATA_APPELLO="$1"
if [ -d "$DATA_APPELLO" ]; then
    echo "questo appello è già presente nel repo temiRO_publish!"
    echo "se vuoi ripetere il suo inserimento devi prima cancella"
        mkdir $DATA_APPELLO
        ln -t $DATA_APPELLO $d/*.pdf ;
    exit 1
fi    
mkdir $DATA_APPELLO
ln -t $DATA_APPELLO ../temi/$DATA_APPELLO/*.pdf
ln -s -r -t temi_all ../temi/$DATA_APPELLO/$DATA_APPELLO-ro-tema.pdf
ln -s -r -t corr_all ../temi/$DATA_APPELLO/$DATA_APPELLO-ro-corr.pdf
git add .
msg="automatic commit from script add_one_single.sh $DATA_APPELLO ($(date +%Y-%m-%d:%Hh:%Mm:%Ss))"
git commit -m $msg
git push
