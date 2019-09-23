
#include <atomic>
#include <thread>
#include <mutex>

#include "pathn.hpp"
#include "sampler.hpp"
#include "config.hpp"

#ifndef PLNR_PROFILER_H
#define PLNR_PROFILER_H

namespace plnr
{
    class profiler {
    
    private:
        bool                first_execution = true,
                            started = false;

        int                 _samples_count =    1,
                            needed_samples,
                            samples_count =     0,

                            frequency,
                            duration;

        double              _sampled_frequency,
                            sampled_frequency = std::numeric_limits<double>::max(),
                            sampled_timespan =  0,
                            avg_frequency =     0,
                            timespan;

        std::string         process_path;

        sampler*            _sampler;

        pathn*              _profile;

        std::chrono::
            milliseconds    _last_result,
                            last_result,
                            start_time;

        std::mutex          at_least_one_sample_mutex,
                            benchmark_start_mutex,
                            benchmark_done_mutex,
                            sampler_start_mutex,
                            sampler_done_mutex,
                            async_call_mutex;

        std::atomic_bool    stop_sampler_atomic;

        std::thread         benchmark_thread,
                            sampler_thread;

        void init_threads();

        void check_frequency();

        void wait_sampler_terminate_benchmark(const std::string& _process_path);

        void wait_benchmark_terminate_sampler();

    public:
        profiler(config* _config, sampler* __sampler);

        profiler(int _frequency, sampler* __sampler);
        
        ~profiler();

        pathn* profile(const std::string& _process_path, int duration);

        pathn* profile(const std::string& _process_path);

        pathn* profile();
    };
}

#endif
