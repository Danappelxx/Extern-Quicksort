FROM gcc:latest

COPY . /usr/src/isg
WORKDIR /usr/src/isg

RUN g++ -std=c++11 -O -o main *.cpp -pthread

# run with thread count = number of logical cores
CMD ./main input.csv output_isg.csv 1,2,0 $(lscpu -p | egrep -v '^#' | wc -l)\
  && sort -t\, -k 2,2n -k 3,3n -k 1,1d input.csv > output_sort.csv\
  && echo "outputs: " $(wc -l output_isg.csv output_sort.csv)\
  && echo "diff: " $(diff output_isg.csv output_sort.csv)
