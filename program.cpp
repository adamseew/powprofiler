
#include "include/integrator_rk4.hpp"
#include "include/soc_1resistor.hpp"
#include "include/model_3layer.hpp"
#include "include/model_1layer.hpp"
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
     pathn*              _model_1layer;

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
          _model_1layer = (new model_1layer())->get_model();

          _first_derivative = new soc_1resistor(*_model_1layer / 12.0, 14.8, 0.0012, 12, 5);

          (new model_3layer(_model_1layer, _first_derivative, h))->get_model()->save();
     }

     delete profile;
     delete _model_1layer;
     delete _sampler;
     delete _profiler;

     exit(0);
}
