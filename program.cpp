
#include "include/integrator_rk4.hpp"
#include "include/soc_1resistor.hpp"
#include "include/model_battery.hpp"
#include "include/model_1layer.hpp"
#include "include/model_2layer.hpp"
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

    pathn*              _model_1layer;

    first_derivative*   _first_derivative;

    config*             _config;
    sampler*            _sampler;
    profiler*           _profiler;

    model_2layer*       _model_2layer;

    _config->load();
    _config->configure();
    _sampler =                              new sampler_tx2();
    _profiler =                             new profiler(_config, _sampler);

    // todo: this (_first_derivative) must be integrated into model_2layer, it uses model_1layer

    _first_derivative = new soc_1resistor(*_model_1layer / 12.0, 14.8, 0.0012, 12, 5);

    if (argc >= 2) {

        _config = new config(argv[2]);

        _config->load();
        _config->configure();
        _sampler = new sampler_tx2();
        _profiler = new profiler(_config, _sampler);

        _model_2layer = new model_2layer(_config, _first_derivative, _profiler);
        _model_2layer->models();

        // todo: well ... that
        // _model_2layer->save();

        system(("../splot.sh " + string(argv[2]) + " " + string(argv[2]) + "_1layer_.csv " + string(argv[2]) + "_battery.csv").c_str());
    }

    delete _profiler;
    delete _config;

    exit(0);
}
