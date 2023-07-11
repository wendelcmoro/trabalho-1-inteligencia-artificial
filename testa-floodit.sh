#!/bin/bash
rm out/*.txt > /dev/null

#Tamanho maximo/minímo de linhas e colunas do tabuleiro
MAX=100;
MIN=2;

#Número maximo/minímo de cores do tabuleiro
MIN_COLORS=2
MAX_COLORS=100

# 0 - redireciona saida no terminal
# 1 - redireciona saida para arquivos correspondentes
toFile=0;

# Número de vezes para repetir o teste(serve para por exemplo executar 20 vezes um teste com 100 100 20)
repeat=1;

for k in $(seq 1 $repeat)
    do
        for i in $(seq $MIN $MAX)
            do
                for j in $(seq $MIN_COLORS $MAX_COLORS)
                    do
                    if [[ $j -le $i ]]
                        then
                            if [[ $i -eq 100 && $j -ge 21 ]]
                                then
                                    break
                                else
                                    # echo $i $i $j
                                    ./geramapa $i $i $j > input.txt;
                                    if [[ $toFile -eq 1 ]]
                                        then
                                            echo -n "flood_it: " >> out/solucao-$i-$i-$j.txt && ./flood_it < input.txt | head -n 1 | tr '\n' ' ' >> out/solucao-$i-$i-$j.txt && echo ""  >> out/solucao-$i-$i-$j.txt;
                                            echo -n "flood_it(Greedy): " >> out/solucao-$i-$i-$j.txt && ./flood_it_greedy < input.txt | head -n 1 | tr '\n' ' ' >> out/solucao-$i-$i-$j.txt && echo ""  >> out/solucao-$i-$i-$j.txt;
                                            echo -n "h1: " >> out/solucao-$i-$i-$j.txt && ./floodit_h1 < input.txt | head -n 1 | tr '\n' ' ' >> out/solucao-$i-$i-$j.txt && echo ""  >> out/solucao-$i-$i-$j.txt;
                                            echo -n "h2: " >> out/solucao-$i-$i-$j.txt && ./floodit_h2 < input.txt | head -n 1 | tr '\n' ' ' >> out/solucao-$i-$i-$j.txt && echo ""  >> out/solucao-$i-$i-$j.txt;
                                            echo -n "h4: " >> out/solucao-$i-$i-$j.txt && ./floodit_h4 < input.txt | head -n 1 | tr '\n' ' ' >> out/solucao-$i-$i-$j.txt && echo ""  >> out/solucao-$i-$i-$j.txt;
                                        else
                                            # echo "Size: $i $i $j: "
                                            # echo -n "   flood_it: " && ./flood_it < input.txt | head -n 1 | tr '\n' ' ' && echo "";
                                            # echo -n "   flood_it(Greedy): " && ./flood_it_greedy < input.txt | head -n 1 | tr '\n' ' ' && echo "";
                                            # echo -n "   h1: " && ./floodit_h1 < input.txt | head -n 1 | tr '\n' ' ' && echo "";
                                            # echo -n "   h2: " && ./floodit_h2 < input.txt | head -n 1 | tr '\n' ' ' && echo "";
                                            # echo -n "   h4: " && ./floodit_h4 < input.txt | head -n 1 | tr '\n' ' ' && echo "";

                                            echo "Size: $i $i $j: "
                                            echo -n "   flood_it: " && ./flood_it < input.txt | head -n 1 | tr '\n' ' ' && echo "";
                                            echo -n "   flood_it(Greedy): " && ./flood_it_greedy < input.txt | head -n 1 | tr '\n' ' ' && echo "";
                                            echo -n "   h1: " && ./floodit_h1 < input.txt | head -n 1 | tr '\n' ' ' && echo "";
                                            echo -n "   h2: " && ./floodit_h2 < input.txt | head -n 1 | tr '\n' ' ' && echo "";
                                            echo -n "   h4: " && ./floodit_h4 < input.txt | head -n 1 | tr '\n' ' ' && echo "";
                                    fi
                                    echo ""
                                fi
                        else
                            break
                        fi
                    done
            done
    done