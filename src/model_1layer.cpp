
#include "../include/model_1layer.hpp"

using namespace plnr;

model_1layer::model_1layer() { }

model_1layer::~model_1layer() { }

pathn* model_1layer::get_model() {
    // todo

    // this functions returns just a test profile and has to be implemented properly i.e., has to call the profiler, get the power profile from specifc config and return it

    return new pathn("/home/user/pplanner/profiles/profile_nvx_sample_sfm.csv");
}
