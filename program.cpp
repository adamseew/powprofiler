
#include "include/sampler_tx2.hpp"
#include "include/profiler.hpp"

#include <iostream>
#include <fstream>

using namespace plnr;
using namespace std;

int main(int argc, char** argv) {
    
    // testing if power sampling is working

    sampler*    _sampler =  new sampler_tx2();
    profiler*   _profiler = new profiler(10, _sampler);
    vectorn     sample;
    pathn       profile;
    int         i, j;

    sample = _sampler->get_sample();

    cout << "general test on tx2 (1)"                              << endl
         << "power:\t\t"   << sample.get(vectorn_flags::power)     << endl
         << "power cpu:\t" << sample.get(vectorn_flags::power_cpu) << endl
         << "power gpu:\t" << sample.get(vectorn_flags::power_gpu) << endl;

    // so it is!

    // getting a power profile from an application (from the arguments)

    if (argc > 1) {

        profile = _profiler->profile(argv[1], 10000);

        cout << "power profile on tx2"                                            << endl
             << "avg power:\t\t"   << profile.avg().get(vectorn_flags::power)     << endl
             << "avg power cpu:\t" << profile.avg().get(vectorn_flags::power_cpu) << endl
             << "avg power gpu:\t" << profile.avg().get(vectorn_flags::power_gpu) << endl;

        sample = _sampler->get_sample();

        cout << "general test on tx2 (2)"                              << endl
             << "power:\t\t"   << sample.get(vectorn_flags::power)     << endl
             << "power cpu:\t" << sample.get(vectorn_flags::power_cpu) << endl
             << "power gpu:\t" << sample.get(vectorn_flags::power_gpu) << endl;

        ofstream output_csv;
        output_csv.open(string(argv[1]).append("_profile.csv"));

        for (i = 0; i < profile.length(); i++) {
            for (j = 0; j < profile.get(0).length() - 1; j++)
                output_csv << profile.get(i).get(j) << "\t";
            output_csv << profile.get(i).get(j) << endl;
        }

        output_csv.close();

        cout << "power profile saved in " << string(argv[1]).append("_profile.csv") << endl;
    }

    delete _sampler;
    delete _profiler;

    return 0;
}
