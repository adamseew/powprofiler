
#include "include/integrator_rk4.hpp"
#include "include/sampler_odroid.hpp"
#include "include/soc_1resistor.hpp"
#include "include/model_battery.hpp"
#include "include/model_1layer.hpp"
#include "include/model_2layer.hpp"
#include "include/sampler_nano.hpp"
#include "include/sampler_tx2.hpp"
#include "include/profiler.hpp"

#include <sys/resource.h>
#include <sys/time.h>
#include <iostream>
#include <unistd.h>
#include <fstream>

using namespace plnr;
using namespace std;

using std::runtime_error;

int main(int argc, char** argv) {

    /// setting priority to highest to cause more favorable scheduling

    setpriority(PRIO_PROCESS, getpid(), -20);

    config*     _config;
    sampler*    _sampler;
    profiler*   _profiler;
    pathn*      _model;

    model*      _model_2layer;

    _sampler = new sampler_tx2();
    if (!_sampler->dryrun()) {
        delete _sampler;

        _sampler = new sampler_odroid();
        if (!_sampler->dryrun()) {
            delete _sampler;

            _sampler = new sampler_nano();
            if (!_sampler->dryrun()) {
                delete _sampler;

                throw runtime_error("unsupported architecture");
            }
        }
    }

    if (argc >= 2) {

        _config = new config(argv[1]);

        _config->load();
        _config->configure();

        _profiler = new profiler(_config, _sampler);

        for (auto _component : _config->components()) {

            _model_2layer = new model_2layer(_config, _profiler, _component);

            // use following for approximation method
            // model.second->approximate(degree);

            _model = _model_2layer->get_model();

            _model->save(_config, _component);
            
            if (_component.size == 2) { } 
            
            else if (_component.size == 3) { }

        }

        // system(("../splot.sh " + string(argv[2]) + " " + string(argv[2]) + "_1layer_.csv " + string(argv[2]) + "_battery.csv").c_str());
    } else
        throw runtime_error("missing configuration file");
    
    delete _model_2layer;
    delete _profiler;
    delete _sampler;
    delete _config;

    exit(0);
}
