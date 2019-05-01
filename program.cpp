
#include "include/integrator_rk4.hpp"
#include "include/soc_1resistor.hpp"
#include "include/sampler_tx2.hpp"
#include "include/profiler.hpp"

#include <iostream>
#include <fstream>
#include <unistd.h>

#include <sys/time.h>
#include <sys/resource.h>

using namespace plnr;
using namespace std;

int main(int argc, char** argv) {

     /// setting priority to highest to cause more favorable scheduling

     setpriority(PRIO_PROCESS, getpid(), -20);
    
     int                 i;
     double              t0 =                0.0,
                         t =                 1.0,
                         h =                 0.001;
     string              file;
     vectorn             sample,
                         start_soc,
                         start_dsoc;
     vectorn*            soc;
     vectorn*            dsoc;
    
     pathn*              profile;
     pathn*              profile_soc;

     sampler*            _sampler =          new sampler_tx2();
     profiler*           _profiler =         new profiler(10, _sampler);
     first_derivative*   _first_derivative;
     integrator_rk4*     _integrator_rk4;      
    
     if (argc > 1) {
          // testing if power sampling is working

          // just testing what happens before lunching the profiler

          sample = _sampler->get_sample();

          cout << "general test on tx2 (1)"                              << endl
               << "power:\t\t"   << sample.get(vectorn_flags::power)     << endl
               << "power cpu:\t" << sample.get(vectorn_flags::power_cpu) << endl
               << "power gpu:\t" << sample.get(vectorn_flags::power_gpu) << endl;
        
          profile = new pathn(_profiler->profile(argv[1], 120000));

          cout << "power profile on tx2"                                             << endl
               << "avg power:\t\t"   << profile->avg().get(vectorn_flags::power)     << endl
               << "avg power cpu:\t" << profile->avg().get(vectorn_flags::power_cpu) << endl
               << "avg power gpu:\t" << profile->avg().get(vectorn_flags::power_gpu) << endl;

          sample = _sampler->get_sample();

          // and ahead: the values shuld be somehow similar

          cout << "general test on tx2 (2)"                              << endl
               << "power:\t\t"   << sample.get(vectorn_flags::power)     << endl
               << "power cpu:\t" << sample.get(vectorn_flags::power_cpu) << endl
               << "power gpu:\t" << sample.get(vectorn_flags::power_gpu) << endl;

          file = "profile_" + string(argv[1]) + ".csv";
          profile->save(file);

          cout << "power profile saved in " << file << endl;
     } else {
          // testing if the battery soc model is working

          profile = new pathn("/home/user/pplanner/profiles/profile_nvx_sample_sfm.csv");

          _first_derivative = new soc_1resistor(*profile / 12.0, 14.8, 0.0012, 12, 5);
          
          // starting for every column at 100 % of state of the charge and integrating downwards

          start_soc = *new vectorn(profile->rows(), new double[profile->rows()] {0.0});
          start_soc.inherit_flags(profile->get(0));
          start_soc = start_soc + 100.0;
          start_dsoc = _first_derivative->get_value(t0, start_soc);

          soc = new vectorn(start_soc.length());
          dsoc = new vectorn(start_soc.length());

          profile_soc = new pathn(start_soc);

          _integrator_rk4 = new integrator_rk4(_first_derivative, t0, start_soc, h, start_dsoc);

          while (t < profile->columns() - 1) {
               _integrator_rk4->step(&t, soc, dsoc);
               profile_soc->add(*soc);
          }

          profile_soc->save();
     }

     delete profile;
     delete _sampler;
     delete _profiler;

     exit(0);
}
