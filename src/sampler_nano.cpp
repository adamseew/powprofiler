
#include "../include/sampler_nano.hpp"

#include <sys/fcntl.h>
#include <stdexcept>
#include <unistd.h>

using namespace plnr;

using std::runtime_error;

sampler_nano::sampler_nano() {
    file_descriptors[0] = open("/sys/bus/i2c/drivers/ina3221x/6-0040/iio_device/in_power0_input", O_RDONLY | O_NONBLOCK); // total power
    file_descriptors[1] = open("/sys/bus/i2c/drivers/ina3221x/6-0040/iio_device/in_power2_input", O_RDONLY | O_NONBLOCK); // cpu power
    file_descriptors[2] = open("/sys/bus/i2c/drivers/ina3221x/6-0040/iio_device/in_power1_input", O_RDONLY | O_NONBLOCK); // gpu power
}

sampler_nano::~sampler_nano() {
    close(file_descriptors[0]);
    close(file_descriptors[1]);
    close(file_descriptors[2]);
}

vectorn sampler_nano::get_sample() {
    int             i,
                    bytes_read;
    char            buffer[BUFFER_SIZE];
    vectorn         power_sample(SAMPLES_COUNT);
    vectorn_flags   flags[SAMPLES_COUNT] =              { vectorn_flags::power,
                                                          vectorn_flags::power_cpu,
                                                          vectorn_flags::power_gpu };

    if (file_descriptors[0] * file_descriptors[1] * file_descriptors[2] < 0)
        throw runtime_error("unable to open file descriptor");

    for (i = 0; i < SAMPLES_COUNT; i++) {
        lseek(file_descriptors[i], 0, 0);
        if ((bytes_read = read(file_descriptors[i], buffer, BUFFER_SIZE + 1)) > 0) {
            buffer[bytes_read] = 0;
            power_sample.set(i, strtod(buffer, NULL) / 1000, flags[i]);
        } else
            throw runtime_error("unable to get data from file descriptor");
    } 

    return power_sample;
}

bool sampler_nano::dryrun() {
    return (file_descriptors[0] * file_descriptors[1] * file_descriptors[2] >= 0);
}
