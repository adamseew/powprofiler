
# PowProfiler 

[![DOI](https://zenodo.org/badge/DOI/10.5281/zenodo.5562457.svg)](https://doi.org/10.5281/zenodo.5562457)

Or powprof is a profiling tool to build computation energy models for mobile robots, and generally, for power critical heterogeneous embedded devices. Born as a simple profiling tool for the companion computers of drones, it evolved to model energy, average power, and battery state of charge. It helps to understand the implications of the computations energy (i.e., the energy consumed by some energy-expensive computational tasks on the device) and optimize the software components in an energy-/battery- aware fashion, perhaps along with [the motion of a mobile robot](https://github.com/adamseew/energy-planning-paper).

## Getting Started

It does not require much, as it's intended for power-critical devices; so it compiles on the major available Linux-based embedded hardware that supports the standard compilation flow (`cmake` and `g++`).

It explicitly supports the following embedded devices: NVIDIA Jetson Nano or TX2, or ODROID XU3. Nevertheless, extending it to another one or using an external multimeter is very simple.

### Setup
The following steps generate the executable with the static and dynamic libraries that allow including powprof in your code. 

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
echo 'export PATH=${PATH}:'$PWD >> ~/.bashrc
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

That's all. Now powprof is ready to build computations energy models.

### Model build

To build a model you need to use powprof along with a configuration file. There is an example in [config_matrixexp.cfg](config_matrixexp.cfg) to see how a simple configuration file looks like. You need to set the following properties:

* __frequency__ in Hz. More frequency means a better model, but it might be too high time-wise. A typical value is then 10 Hz
* __h__ is the granularity of the integration for the battery model, i.e., RK4 step
* __directory__ refers to the directory where the models will be stored
* __component name__ goes after the dot per each component, e.g., you can have
```conf
[components]
    [component.first-component]
    ...
    [component.second-component]
    ...
```

Instead of the dots in the above slice of code, you have to specify:

* __src__ is the source of the executable. Note that the executable must accept some arguments that change its state. In the example in [config_matrixexp.cfg](config_matrixexp.cfg) those are matrix size and exponent
* __range__ (1 or more) uses the pattern min, max, step (with `pow(n)` you allow only values that are a power of `n`). For the example above, powprof generates configurations 20, 30, 40, 50, and 60 with the following line. If you have more range entries, powprof combines them automatically 
```conf
range = 20, 60, 10
```
* __fixed__ (0 or more) imagine you need to pass the location of the data as an argument. You can just do it with (powprof uses the order of your entries to understand the order of the arguments)
```conf
fixed = /dat/matrix1.csv
```
* __runtime__ (optional) the profiling time in ms (important for components running in a loop)

Written your configuration file, you can build the model that will be stored in a CSV file by just running powprof along with the configuration file, e.g.,
```bash
powprof config_matrixexp.cfg
```
this generates an energy model stored in the directory that you specified.

## Publications

The powprof tool appeared in a number of research peer-reviewed publications. To cite powprof, it is sufficient to use the following BibTeX entry:
```bibtex
@article{seewald2019coarse,
  title={Coarse-Grained Computation-Oriented Energy Modeling for Heterogeneous Parallel Embedded Systems},
  author={Seewald, Adam and Schultz, Ulrik Pagh and Ebeid, Emad and Midtiby, Henrik Skov},
  journal={International Journal of Parallel Programming},
  year={2021},
  number={2},
  volume={49},
  pages={136--157},
  issn={0885-7458},
  publisher={Springer},
  DOI={10.1007/s10766-019-00645-y},
  url={https://adamseewald.cc/short/coarse2019}
}
```

Other publications involving powprof are:

* [Component-Based Computation-Energy Modeling for Embedded Systems](https://adamseewald.cc/publications/component-based-computation-energy-modeling-for-embedded-systems/preprint/)
* [Mechanical and Computational Energy Estimation of a Fixed-Wing Drone](https://adamseewald.cc/publications/mechanical-and-computational-energy-estimation-of-a-fixed-wing-drone/preprint/)
* [Energy-Aware Design of Vision-Based Autonomous Tracking and Landing of a UAV](https://adamseewald.cc/publications/energy-aware-design-of-vision-based-autonomous-tracking-and-landing-of-a-uav/preprint/)


## Getting Help

We are happy to help you anytime, visit the main contributor's [webpage](https://adamseew.bitbucket.io) for contacts, or write [a github issue](https://github.com/adamseew/powprofiler/issues) directly.

## License

The powprof tool is distributed under the [MIT license](LICENSE.md). Its development was made possible by the TeamPlay research project thanks to the funding by the European Union's Horizon 2020 research and innovation program (grant agreement 779882).
