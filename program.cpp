
#include "include/integrator_rk4.hpp"
#include "include/sampler_odroid.hpp"
#include "include/soc_1resistor.hpp"
#include "include/model_battery.hpp"
#include "include/model_1layer.hpp"
#include "include/model_2layer.hpp"
#include "include/sampler_nano.hpp"
#include "include/sampler_tx2.hpp"
#include "include/profiler.hpp"

#include <iostream>
#include <fstream>
#include <unistd.h>

#include <sys/time.h>
#include <sys/resource.h>

using std::runtime_error;

using namespace plnr;
using namespace std;

int main(int argc, char** argv) {

    /// setting priority to highest to cause more favorable scheduling

    setpriority(PRIO_PROCESS, getpid(), -20);

    // here

    // pathn* _model_1layer = new pathn("/home/user/Dropbox/acad/papers/ijpp-hlpgpu-special-issue/figures/source/matrix-gpu/matrix-exponentiation_layer2-power.csv");
    // _model_1layer->approximate(4);
    // _model_1layer->save("/home/user/Dropbox/acad/papers/ijpp-hlpgpu-special-issue/figures/source/matrix-gpu/matrix-exponentiation_layer2-power_approx.csv");

    // here

    config*             _config;
    sampler*            _sampler;
    profiler*           _profiler;

    model_2layer*       _model_2layer;

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
        _sampler = new sampler_tx2();
        _profiler = new profiler(_config, _sampler);

        _model_2layer = new model_2layer(_config, _profiler);

        for (auto model : _model_2layer->models()) {
            model.second->save(model.first.name + ".csv");
            if (model.first.size < 3) {
                // todo: plot a 2D plot with gnuplot here
            } else if (model.first.size < 4) {
                // todo: plot a 3D plot with gnuplot here
            }
        }

        // system(("../splot.sh " + string(argv[2]) + " " + string(argv[2]) + "_1layer_.csv " + string(argv[2]) + "_battery.csv").c_str());
    }

    delete _model_2layer;
    delete _profiler;
    delete _sampler;
    delete _config;

    exit(0);
}
