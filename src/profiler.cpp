
#include "../include/profiler.hpp"

#include <thread>
#include <chrono> 
#include <limits>

using namespace plnr;
using namespace std::chrono;

profiler::profiler(int _frequency, sampler* __sampler) {
    frequency = _frequency;
    _sampler = __sampler;
}

pathn profiler::profile(std::string component, int _milliseconds) {
    
    int             timespan =          1000 / frequency,
                    samples_count =     0,
                    _samples_count =    0,
                    needed_samples =    _milliseconds > 0 ?
                                        _milliseconds / frequency :
                                        std::numeric_limits<int>::max(),
                    sampled_timespan =  0,
                    sampled_frequency = std::numeric_limits<int>::max(),
                    overall_frequency;
    pathn           _profile;
    vectorn         sample(_sampler->get_sample());

    milliseconds    max_sampling_time(_milliseconds);
    milliseconds    last_result;
    milliseconds    _last_result;
    milliseconds    start_time;

    std::thread     sampler_thread([&](){ 
        start_time = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
        while (1) {
            sample = sample + _sampler->get_sample();
            ++_samples_count;

            if ((sampled_timespan = duration_cast<milliseconds>(
                (_last_result = duration_cast<milliseconds>(system_clock::now().time_since_epoch())) - 
                last_result).count()) >= timespan
            ) {
                _profile.add(sample / _samples_count); 
                last_result = _last_result;
                ++samples_count;
                _samples_count = 0;
                if (sampled_frequency > sampled_timespan / _samples_count)
                    sampled_frequency = sampled_timespan / _samples_count;
            }

            if (samples_count == needed_samples)
                return;
        }
    });
    std::thread     benchmark_thread([&](){ system(component.c_str()); });
    
    if (_milliseconds > 0)
        sampler_thread.join();
    else
        benchmark_thread.join();

    if ((overall_frequency = duration_cast<milliseconds>(
        system_clock::now().time_since_epoch() - 
        start_time).count() / frequency) != samples_count
    )
        throw std::out_of_range(
            "sampled frequency f=" + std::to_string(overall_frequency) + 
            " is different from the expected. Consider lowering the expected frequency"
        );

    return _profile;
}

pathn profiler::profile(std::string component) {
    return profiler::profile(component, 0);
}
