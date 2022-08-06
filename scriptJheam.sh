srcFile=main.c

gcc --std=gnu89 $srcFile -o main -lm

for subfolder in ./Testes/*
do
    for testFolder in $subfolder/*
    do
        rm -r $testFolder/saida/
        mkdir $testFolder/saida
        ./main $testFolder < $testFolder/movimentos.txt > $testFolder/saida/saida.txt
    done
done

diff -r Testes/ Gabarito/