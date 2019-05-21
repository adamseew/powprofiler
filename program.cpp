
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
    
     int                 i,
                         j;
     double              h =                 0.01,
                         avg_1layer,
                         last_3layer;

     pathn*              _model_1layer;
     pathn*              _model_3layer;

     sampler*            _sampler =          new sampler_tx2();
     profiler*           _profiler =         new profiler(10, _sampler);
     first_derivative*   _first_derivative;

     std::ofstream       output_csv1,
                         output_csv2;

     // first parameter is the sample to profile
     // second parameter is the name of the sample, i.e., matrix-multiplication

     if (argc >= 3) {

          output_csv1.open(string(argv[2]) + "_1layer.csv");
          output_csv2.open(string(argv[2]) + "_3layer.csv");

          // change i to fit the x axis in the surface

          for (i = 256; i <= 4096; i *= 2) {

              // change j to fit the y axis in the surface

              for (j = 0; j <= 10; j += 2) {

                  // sampling data about power evolution and fitting model

                  _model_1layer = (new model_1layer(argv[1], to_string(i) + " " + to_string(j), _profiler))->get_model();

                  // evaluating average for the surface

                  avg_1layer = _model_1layer->avg().get(vectorn_flags::power);

                  // saving the data

                  _model_1layer->save(string(argv[2]) + "_" + to_string(i) + "-" + to_string(j) + "_1layer.csv");

                  // battery model, change if needed

                  _first_derivative = new soc_1resistor(*_model_1layer / 12.0, 14.8, 0.0012, 12, 5);

                  // getting battery model evolution in time (SoC) by integrating the dynamics with obtained model

                  _model_3layer = (new model_3layer(_model_1layer, _first_derivative, h))->get_model();

                  delete _model_1layer;

                  // getting the remaining battery status and saving all the data

                  last_3layer = _model_3layer->get(_model_3layer->columns() - 1).get(vectorn_flags::power);
                  _model_3layer->save(string(argv[2]) + "_" + to_string(i) + "-" + to_string(j) + "_3layer.csv");

                  delete _model_3layer;
                  delete _first_derivative;

                  // saving data for the surface

                  output_csv1 << i << " " << j << " " << avg_1layer << endl;
                  output_csv2 << i << " " << j << " " << last_3layer << endl;

              }
          }

          output_csv1.close();
          output_csv2.close();

          // plotting the surface

          system(("../splot.sh " + string(argv[2]) + " " + string(argv[2]) + "_1layer.csv " + string(argv[2]) + "_3layer.csv").c_str());
     }

     delete _sampler;
     delete _profiler;
     delete _first_derivative;

     exit(0);
}
