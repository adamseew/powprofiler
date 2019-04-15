
#include "../include/profiler.hpp"

#include <thread>
#include <chrono> 
#include <limits>
#include <pthread.h>

using namespace plnr;
using namespace std::chrono;

profiler::profiler(int _frequency, sampler* __sampler) {
    frequency = _frequency;
    _sampler = __sampler;
}

pathn profiler::profile(std::string component, int _milliseconds) {
    
    double          timespan =          1000 / frequency,
                    sampled_timespan =  0,
                    sampled_frequency = std::numeric_limits<double>::max(),
                    _sampled_frequency,
                    avg_frequency =     0,
                    overall_frequency;
    int             samples_count =     0,
                    _samples_count =    1,
                    needed_samples =    _milliseconds > 0 ?
                                        _milliseconds / timespan :
                                        std::numeric_limits<int>::max();
    pathn           _profile;
    vectorn         sample(_sampler->get_sample());

    milliseconds    last_result;
    milliseconds    _last_result;
    milliseconds    start_time;

    std::string     log_data;

    std::thread     sampler_thread([&](){ 
        start_time = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
        last_result = start_time;
        while (1) {
            sample = sample + _sampler->get_sample();
            ++_samples_count;

            if ((sampled_timespan = duration_cast<milliseconds>(
                (_last_result = duration_cast<milliseconds>(system_clock::now().time_since_epoch())) - 
                last_result).count()) >= timespan
            ) {
                if (sampled_frequency > (_sampled_frequency = (1000.0 / (sampled_timespan / _samples_count))))
                    sampled_frequency = _sampled_frequency;

                avg_frequency += _sampled_frequency;
                last_result = _last_result;

                _profile.add(sample / _samples_count);
                sample = *(new vectorn(_sampler->get_sample()));

                _samples_count = 1;
                ++samples_count;
            }

            if (samples_count == needed_samples)
                return;
        }
    });

    std::thread     benchmark_thread([&](){
        system(component.c_str());
    });
    
    if (_milliseconds > 0) {
        sampler_thread.join();
        pthread_cancel(benchmark_thread.native_handle());
        benchmark_thread.join();
    } else {
        benchmark_thread.join();
        pthread_cancel(sampler_thread.native_handle());
        sampler_thread.join();
    }

    overall_frequency = (1000.0 / (duration_cast<milliseconds>(
        system_clock::now().time_since_epoch() - start_time).count() / samples_count));
    avg_frequency /= samples_count;

    /// a simple sanity check if the sampled frequeny i.e., the actual frequency of getting and storing the sample and the overall frequence i.e., the number of samples against inital and final interval is equal to the expected value

    if ((-1) * (overall_frequency + sampled_frequency) + 2 * frequency > 0) {
        log_data = "Frequency overall="         + std::to_string(overall_frequency) +
                           ", sampled(lowest)=" + std::to_string(sampled_frequency) +
                           ", avg="             + std::to_string(avg_frequency) +
                           ", but expected="    + std::to_string(frequency);
        throw std::out_of_range(
            "sampled frequency is different from the expected. Consider lowering the expected frequency. " + log_data
        );
    }

    return _profile;
}

pathn profiler::profile(std::string component) {
    return profiler::profile(component, 0);
}
