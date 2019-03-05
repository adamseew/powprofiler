
#include "include/sampler_tx2.hpp"
#include <iostream>

using namespace plnr;
using namespace std;

int main(int argc, char** argv) {
    
    sampler*    _sampler =  new sampler_tx2();
    vectorn     sample;

    sample = _sampler->get_sample();

    cout << "power:\t\t"  << sample.get(vectorn_flags::power) << "\t"
         << "power cpu:\t" << sample.get(vectorn_flags::power_cpu) << "\t"
         << "power gpu:\t" << sample.get(vectorn_flags::power_gpu) << endl;

    return 0;
}