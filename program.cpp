
#include "include/config.hpp"
#include "include/integrator_rk4.hpp"
#include "include/model_1layer.hpp"
#include "include/model_2layer.hpp"
#include "include/model_battery.hpp"
#include "include/pathn.hpp"
#include "include/profiler.hpp"
#include "include/sampler_nano.hpp"
#include "include/sampler_odroid.hpp"
#include "include/sampler_tx2.hpp"
#include "include/sampler.hpp"
#include "include/soc_1resistor.hpp"
#include "include/utility.hpp"
#include "include/vectorn.hpp"

#include <sys/resource.h>
#include <sys/time.h>
#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <fstream>
#include <sstream>
#include <string>

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

                printf("unsupported architecture\n");
            }
        }
    }

    if (argc >= 2) {
        ifstream file(argv[1]);
        
        if (file.is_open()) {
            
            string line;
            getline(file, line);

            if(utility_trim(line).compare("[combinations]") == 0) {

                pathn* m_2l = NULL;

                while (getline(file, line)) {
                    vector<string> line_data = utility_split(line, '=');

                    if (line_data.at(0).compare("name") == 0) {
                        m_2l->save(line_data.at(1));
                        exit(0);
                    }


                    pathn* m_1l = new pathn(line_data.at(0));
                    line_data = utility_split(line_data.at(1), ',');
                    
                    int i = line_data.size();

                    vectorn v(i + 2);

                    for (i = 0; i < line_data.size(); i++) {
                        v.set(i, atof(line_data.at(i).c_str()), static_cast<vectorn_flags>(static_cast<int>(vectorn_flags::problem_dimension) + i));
                    }

                    v.set(i, m_1l->avg().get(0), vectorn_flags::power);
                    v.set(i + 1, m_1l->sum().get(0), vectorn_flags::energy);
                        

                    if (m_2l == NULL) {                        
                        m_2l = new pathn(v);
                    } else {
                        m_2l->add(v);
                    }
                }

                m_2l->save();
                exit(0);
            }
            
            file.close();
        }

        _config = new config(argv[1]);

        _config->load();
        _config->configure();

        _profiler = new profiler(_config, _sampler);

        for (auto _component : _config->components()) {

            _model_2layer = new model_2layer(_config, _profiler, _component);

            // use following for approximation method
            // model.second->approximate(degree);

            _model = _model_2layer->get_model();

            // function save return the name of the file with the results

            _model->save(_config, _component);
    
            // this condition means that a 2D model has been generated

            if (_component.size == 2) { } 

            // this that a 3D one has been generated
            
            else if (_component.size == 3) { }

        }

    } else
        throw runtime_error("missing configuration file");
    
    delete _model_2layer;
    delete _profiler;
    delete _sampler;
    delete _config;

    exit(0);
}
