
#include "../include/sampler_tx2.hpp"
#include <stdexcept>
#include <unistd.h>
#include <sys/fcntl.h>

using namespace plnr;

sampler_tx2::sampler_tx2() {
    file_descriptors[0] = open("/sys/bus/i2c/drivers/ina3221x/0-0041/iio_device/in_power0_input", O_RDONLY | O_NONBLOCK); // total power
    file_descriptors[1] = open("/sys/bus/i2c/drivers/ina3221x/0-0041/iio_device/in_power1_input", O_RDONLY | O_NONBLOCK); // cpu power
    file_descriptors[2] = open("/sys/bus/i2c/drivers/ina3221x/0-0040/iio_device/in_power0_input", O_RDONLY | O_NONBLOCK); // gpu power
}

sampler_tx2::~sampler_tx2() {
    close(file_descriptors[0]);
    close(file_descriptors[1]);
    close(file_descriptors[2]);
}

vectorn sampler_tx2::get_sample() {
    int             i,
                    bytes_read;
    char            buffer[BUFFER_SIZE];
    vectorn         power_sample(SAMPLES_COUNT);
    vectorn_flags   flags[SAMPLES_COUNT] =              { vectorn_flags::power,
                                                          vectorn_flags::power_cpu,
                                                          vectorn_flags::power_gpu };

    if (file_descriptors[0] * file_descriptors[1] * file_descriptors[2] < 0)
        throw std::runtime_error("unable to open file descriptor");

    for (i = 0; i < SAMPLES_COUNT; i++) {
        lseek(file_descriptors[i], 0, 0);
        if ((bytes_read = read(file_descriptors[i], buffer, BUFFER_SIZE + 1)) > 0) {
            buffer[bytes_read] = 0;
            power_sample.set(i, strtod(buffer, NULL) / 1000, flags[i]);
        } else
            throw std::runtime_error("unable to get data from file descriptor");
    } 

    return power_sample;
}
