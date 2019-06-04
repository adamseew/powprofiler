
#include "include/integrator_rk4.hpp"
#include "include/soc_1resistor.hpp"
#include "include/model_battery.hpp"
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
                        sum_1layer,
                        last_battery;

    pathn*              _model_1layer;
    pathn*              _model_battery;

    first_derivative*   _first_derivative;

    std::ofstream       output_csv1,
                        output_csv2,
                        output_csv3;

    config*             _config =           new config("/home/user/pplanner/config2.cfg");
    sampler*            _sampler;
    profiler*           _profiler;

    _config->load();
    _sampler =                              new sampler_tx2();
    _profiler =                             new profiler(_config, _sampler);

    // first parameter is the sample to profile
    // second parameter is the name of the sample, i.e., matrix-multiplication

    if (argc >= 3) {

        output_csv1.open(string(argv[2]) + "_1layer_.csv");
        output_csv2.open(string(argv[2]) + "_1layer2.csv");
        output_csv3.open(string(argv[2]) + "_battery.csv");

        // change i to fit the x axis in the surface, i.e., matrix size

        for (i = 256; i <= 4096; i *= 2) {

            // change j to fit the y axis in the surface, i.e., exponent

            for (j = 20; j <= 60; j += 10) {

                // sampling data about power evolution and fitting model

                _model_1layer = (new model_1layer(argv[1], to_string(i) + " " + to_string(j), _profiler))->get_model();

                _model_1layer = new pathn(string(argv[2]) + "_" + to_string(i) + "-" + to_string(j) + "_1layer.csv");

                // evaluating average for the surface

                avg_1layer = _model_1layer->avg().get(vectorn_flags::power);
                sum_1layer = _model_1layer->sum().get(vectorn_flags::power);

                // saving the data

                _model_1layer->save(string(argv[2]) + "_" + to_string(i) + "-" + to_string(j) + "_1layer_.csv");

                // battery model, change if needed

                _first_derivative = new soc_1resistor(*_model_1layer / 12.0, 14.8, 0.0012, 12, 5);

                // getting battery model evolution in time (SoC) by integrating the dynamics with obtained model

                _model_battery = (new model_battery(_config, _model_1layer, _first_derivative))->get_model();

                delete _model_1layer;

                // getting the remaining battery status and saving all the data

                last_battery = _model_battery->get(_model_battery->columns() - 1).get(vectorn_flags::power);
                _model_battery->save(string(argv[2]) + "_" + to_string(i) + "-" + to_string(j) + "_battery.csv");

                delete _model_battery;
                delete _first_derivative;

                // saving data for the surface
                // last_3layer / 10 => gnuplot need to build two plots some normalized values (quick fix)

                output_csv1 << i << " " << j << " " << avg_1layer << endl;
                output_csv2 << i << " " << j << " " << avg_1layer << endl;
                output_csv3 << i << " " << j << " " << last_battery / 10 << endl;

            }
        }

        output_csv1.close();
        output_csv2.close();
        output_csv3.close();

        // plotting the surface

        system(("../splot.sh " + string(argv[2]) + " " + string(argv[2]) + "_1layer_.csv " + string(argv[2]) + "_battery.csv").c_str());
    }

    delete _sampler;
    delete _profiler;

    exit(0);
}
