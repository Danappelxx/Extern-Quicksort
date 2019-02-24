//
//  Quicksort.h
//  ISGAssignment
//
//  Created by Dan Appel on 2/22/19.
//  Copyright © 2019 Dan Appel. All rights reserved.
//

#pragma once

#include "ThreadPool.h"
#include <vector>
#include <memory>

template <class T>
class Quicksort {
    using IT = typename std::vector<T>::iterator;

public:
    // quicksort only makes sense as a one-off call, so this
    // helper function is the only public entrypoint
    static void sort(IT begin, IT end, int threadCount, std::function<bool(T, T)> lessThanComparator) {
        Quicksort qs(begin, end, threadCount, lessThanComparator);
        qs.sort();
    };

private:
    Quicksort(IT begin, IT end, int threadCount, std::function<bool(T, T)> lessThanComparator)
    : begin(begin), end(end), lessThanComparator(lessThanComparator) {
        threadPool = std::make_shared<progschj::ThreadPool>(threadCount);
    }

    void sort() {
        // kick-off parallel-recursive sort
        parallelQuicksort(begin, end - 1);
        // wait until all jobs are done
        threadPool->wait_until_nothing_in_flight();
    }

    IT begin;
    IT end;
    std::shared_ptr<progschj::ThreadPool> threadPool;
    std::function<bool(T, T)> lessThanComparator;

    IT partition(IT low, IT high) {
        T pivot = *high;
        IT i = low;
        for (IT j = low; j < high; j++) {
            if (lessThanComparator(*j, pivot)) {
                std::iter_swap(i, j);
                i++;
            }
        }
        std::iter_swap(i, high);
        return i;
    }

    void parallelQuicksort(IT low, IT high) {
        if (low < high) {
            IT pivot = partition(low, high);
            if (std::distance(low, high) > 100) {
                threadPool->enqueue([=]() { return parallelQuicksort(low, pivot - 1); });
                threadPool->enqueue([=]() { return parallelQuicksort(pivot + 1, high); });
            } else {
                // fallback to sequential sort once we get close
                sequentialQuicksort(low, pivot - 1);
                sequentialQuicksort(pivot + 1, high);
            }
        }
    }

    void sequentialQuicksort(IT low, IT high) {
        if (low < high) {
            IT pivot = partition(low, high);
            sequentialQuicksort(low, pivot - 1);
            sequentialQuicksort(pivot + 1, high);
        }
    }
};
