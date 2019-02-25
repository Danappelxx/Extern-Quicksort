# usage:
#  build the container
#   $ docker build -t isg .
#  run it
#   $ docker run isg
#  run it with 50mb of ram
#   $ docker run --memory 50m isg

FROM gcc:latest

RUN apt-get update && apt-get install time -y

COPY . /usr/src/isg
WORKDIR /usr/src/isg

RUN g++ -std=c++11 -O -o main *.cpp -pthread

# run with thread count = number of logical cores
CMD echo "quicksort: " && time -p ./main input.csv output_isg.csv 1,2,0 $(lscpu -p | egrep -v '^#' | wc -l)\
  && echo "sort: " && time -p sort -t\, -k 2,2n -k 3,3n -k 1,1d input.csv > output_sort.csv\
  && echo "outputs: " $(wc -l output_isg.csv output_sort.csv)\
  && echo "diff: " $(diff output_isg.csv output_sort.csv)
