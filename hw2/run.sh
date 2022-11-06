#!/bin/bash
inputs=("1.in" "2.in" "3.in")
outputs=("1.7z" "2.7z" "3.7z")
TIMEFORMAT="%R sec"
make

# for i in ${!inputs[@]}; do
#     echo "input: ${inputs[$i]}, output: ${outputs[$i]}"
#     echo -n "    encoding: "
#     time ./main 1 ${inputs[$i]} ${outputs[$i]}
#     echo -n "    decoding: "
#     time ./main 2 ${outputs[$i]} raw.txt
#     stat --printf="\n    ${inputs[$i]}: %s bytes" ${inputs[$i]}
#     stat --printf="\n    ${outputs[$i]}: %s bytes\n\n" ${outputs[$i]}
#     echo "    print diff"
#     diff ${inputs[$i]} raw.txt
#     echo
# done

# methods=("LZMA" "LZMA2" "PPMd" "BZip2" "Deflate")
method="Deflate"

# for method in $methods; do
echo $method
rm -rf temp
mkdir temp
for i in ${!inputs[@]}; do
    echo "input: ${inputs[$i]}, output: ${outputs[$i]}"
    # echo -n "    encoding: "
    # time 7z a -m0="$method" ${outputs[$i]} ${inputs[$i]}
    echo -n "    decoding: "
    time 7z x ${outputs[$i]} -otemp
    # stat --printf="\n    ${inputs[$i]}: %s bytes" ${inputs[$i]}
    # stat --printf="\n    ${outputs[$i]}: %s bytes\n\n" ${outputs[$i]}
    # echo "    print diff"
    # diff ${inputs[$i]} raw.txt
    echo
done
# done
