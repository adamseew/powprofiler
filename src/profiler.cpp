
#include "../include/profiler.hpp"

#include <chrono> 
#include <limits>

using namespace plnr;
using namespace std::chrono;

using std::string;
using std::thread;
using std::to_string;
using std::numeric_limits;

profiler::profiler(config* _config, sampler* __sampler):stop_sampler_atomic(false) {

    frequency = _config->get_frequency();
    timespan =  1000 / frequency;
    _sampler = __sampler;
    _profile = nullptr;
}

profiler::profiler(int _frequency, sampler* __sampler):stop_sampler_atomic(false) {

    frequency = _frequency;
    timespan =  1000 / frequency;
    _sampler = __sampler;
    _profile = nullptr;
}

profiler::~profiler() {

    pthread_cancel(sampler_thread.native_handle());
    pthread_cancel(benchmark_thread.native_handle());

    sampler_start_mutex.unlock();
    sampler_thread.join();

    benchmark_start_mutex.unlock();
    benchmark_thread.join();
}

void profiler::init_threads() {

    if (!first_execution)
        return;

    first_execution = false;

    sampler_thread = thread([&](){

        while(1) { 

            // starting the sampling thread that collects samples from the boards (supported) through sampler class

            sampler_start_mutex.lock();

            vectorn sample(_sampler->get_sample());

            needed_samples =    duration > 0 ?
                                duration / timespan :
                                numeric_limits<int>::max();

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

                    _profile->add(sample / _samples_count);

                    at_least_one_sample_mutex.unlock();
                    sample = vectorn(_sampler->get_sample());

                    _samples_count = 1;
                    ++samples_count;
                }

                if (samples_count >= needed_samples || stop_sampler_atomic)
                    break;
            }

            // sampler thread execution done, advising whoever is waiting, and preventing from running again

            sampler_done_mutex.unlock();
        }
    });

    benchmark_thread = thread([&](){

        while (1) {
        
            // starting the benchmark thread that calls a component under specific configuration

            benchmark_start_mutex.lock();

            if (!process_path.empty())
                system(process_path.c_str());
            else

                // this is the "library version execution"

                async_call_mutex.lock();

            /// it can happen that the component under analysis is too fast and no sample has been collected. So let's wait for at least one sample

            at_least_one_sample_mutex.lock();

            // benchmark thread execution completed, advising whoever is waiting, and preventing from running again

            benchmark_done_mutex.unlock();
        }
    });
}

void profiler::check_frequency() {

    double  overall_frequency;

    string  log_data;

    overall_frequency = (1000.0 / (duration_cast<milliseconds>(
        system_clock::now().time_since_epoch() - start_time).count() / samples_count));
    avg_frequency /= samples_count;

    /// a simple sanity check if the sampled frequeny i.e., the actual frequency of getting and storing the sample and the overall frequence i.e., the number of samples against inital and final interval is equal to the expected value

    if ((-1) * (overall_frequency + sampled_frequency) + 2 * frequency > 0) {
        log_data = "Frequency overall="         + to_string(overall_frequency) +
                           ", sampled(lowest)=" + to_string(sampled_frequency) +
                           ", avg="             + to_string(avg_frequency) +
                           ", but expected="    + to_string(frequency);
        throw std::out_of_range(
            "sampled frequency is different from the expected. Consider lowering the expected frequency. " + log_data
        );
    }
}

void profiler::wait_sampler_terminate_benchmark(const string& _process_path) {

    sampler_done_mutex.lock(); // ~= sampler_thread.join();

    system((
        "kill $(ps aux | grep '" + _process_path + "' | awk '{print $2}')"
    ).c_str()); // ~= pthread_cancel(benchmark_thread.native_handle());
                //    benchmark_thread.join();
    benchmark_done_mutex.lock();

    // leaving ready for next invocation

    at_least_one_sample_mutex.unlock();
    sampler_done_mutex.unlock();
    benchmark_done_mutex.unlock();
}

void profiler::wait_benchmark_terminate_sampler() {

    benchmark_done_mutex.lock(); // ~= benchmark_thread.join();

    stop_sampler_atomic = true;  // ~= pthread_cancel(sampler_thread.native_handle());
                                 //    sampler_thread.join();
    sampler_done_mutex.lock();

    // leaving ready for next invocation

    at_least_one_sample_mutex.unlock();
    sampler_done_mutex.unlock();
    benchmark_done_mutex.unlock();
}

pathn* profiler::profile(const string& _process_path, int _duration) {
    
    at_least_one_sample_mutex.lock();
    sampler_done_mutex.lock();
    benchmark_done_mutex.lock();
    
    stop_sampler_atomic = false;
    process_path = _process_path;
    duration = _duration;
    _profile = new pathn();

    sampler_start_mutex.unlock();
    benchmark_start_mutex.unlock();

    init_threads();

    // first modality of profiling:
    //   1st =>> given a path of an executable and the duration in ms, profile for that amount. Then terminate profiling and return model

    if (_duration > 0) {
        
        wait_sampler_terminate_benchmark(_process_path);

    // second modality of profiling
    //   2nd =>> given a path of an executable, profile until the executable is terminated. Then return the model

    } else if (!_process_path.empty()) {

        wait_benchmark_terminate_sampler();

    // third modality of profiling
    //   3rd =>> (library version) start profiling with call start. Stop profiling with call stop, then return model

    } else {

        return nullptr;
    }

    check_frequency();

    return _profile;
}

pathn* profiler::profile(const string& _process_path) {

    return profiler::profile(_process_path, 0);
}

pathn* profiler::profile() {

    if (!started) {

        started = true;

        async_call_mutex.lock();

        return profiler::profile("", 0);
    } else {

        async_call_mutex.unlock();

        // this is the same code as as the 2nd modality of profiling

        wait_benchmark_terminate_sampler();

        // making sure that on the next invocation it will not stuck on lock (the above call will lock it, since it was waiting on a lock)

        async_call_mutex.unlock();

        check_frequency();

        started = false;

        return _profile;
    }
}
