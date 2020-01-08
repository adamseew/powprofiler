
# PowProfiler

Or simply powprof is a profiling tool to build computation energy models for mobile robots, and more generally for power critical heterogeneous embedded devices. Born as a simple profiling tool for the companion computers of drones, evolved to a quite complex program that models energy, average power, and drain from the battery through state-of-charge. It helps you understand where the energy of your computation goes, and thus to adjust your code and consume less.

## Getting Started

It does not require much, as it's intended for power critical devices; so it compiles on the majority of available Linux based embedded hardware that supports the standard compilation flow (`cmake` and `g++`).

To run it, you will need one of the following embedded devices: NVIDIA Jetson Nano or TX2, or ODROID XU3. Nevertheless extending it to another one or using an external multimeter is rather simple (a tutorial will be ready soon).

### Setup
Just run the following steps and you should be ok. They generate the executable along with the static and dynamic libraries to include powprof in your code. 

* First, clone the repo (you will find the same repo mirrored on [bitbucket](https://bitbucket.org/adamseew/powprofiler.git) and [github](https://github.com/adamseew/powprofiler.git))
```bash
git clone https://bitbucket.org/adamseew/powprofiler.git
cd powprofiler
```
* Then, continue with the typical `cmake` compilation flow. Here is a quick reminder
```bash
mkdir build
cd build
cmake ..
make
```
* You can include powprof in the environment variable so you can run it directly from bash by calling `powprofile`
```bash
echo 'export PATH=${PATH}:'$PWD >> ~.bashrc
source ~/.bashrc
```
* In case you wish to use powprof in its asynchronous library version, just build the shared library with
```bash
sudo make install
```
* Make sure that the cache with your shared libraries is up-to-date before including powprof in your project with `#include <powprof/async.h>`
```bash
sudo ldconfig
```

That's all. Now you can use powprof command to build your computational energy models.

### Model build

To build a model you need to use powprof along with a configuration file. Please refer to [config3.cfg](config3.cfg) to see how a simple configuration file looks like (a tutorial is on the way). You need to set the following properties:

* __frequency__ in Hz. Yes, more frequency means a better model, but keep on mind that it might be too high
* __h__ is the granularity of the integration for the battery model, i.e., RK4 step
* __directory__ refers to the directory where the model will be stored
* __component name__ goes after the dot per each component, e.g., you can have
```conf
[components]
    [component.first-component]
    ...
    [component.second-component]
    ...
```

Instead of the dots in the above slice of code, you have to specify:

* __src__ the source of the executable. Note that the executable must accept some arguments that change its state. In the example in [config3.cfg](config3.cfg) those are matrix size and exponent
* __range__ (1 or more) uses the pattern min, max, step (with `pow(n)` you allow only values that are power of `n`). For the example above, powprof generates configurations 20, 30, 40, 50, and 60 with the following line. If you have more range entries, powprof combines them automatically 
```conf
range = 20, 60, 10
```
* __fixed__ (0 or more) imagine you need to pass the location of the data as an argument. You can just do it with (powprof uses the order of your entries to understand the order of the arguments)
```conf
fixed = /dat/matrix1.csv
```
* __runtime__ (optional) the profiling time in ms (important for components running in a loop)

Written your configuration file, you can build the model that will be stored in a csv file by just running powprof along with the configuration file, e.g.,
```bash
powprof config3.cfg
```
this generates an energy model stored in the directory that you specified.

## Getting Help

Happy to help you anytime, visit [my webpage](https://adamseew.bitbucket.io) for contacts or write [a github issue](https://github.com/adamseew/powprofiler/issues) (so you will help also the others).

## License

It is distributed under [MIT license](LICENSE.md) for your convenience, so use it freely in your project as soon as you are aware that it was built for research purposes and we don't support any unethical use of it (i.e., for military involved projects).
