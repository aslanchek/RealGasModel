# Molecular Dynamic simulation of Lennard-Jones gas engine.

Visualization for 961 particles distibuted on the plane:
<p>
    <img src="https://github.com/aslanchek/RealGasModel/blob/master/animation.gif">
</p>
Red particle represents Brownian motion.

3D visualization using Ovito:
<p>
    <img src="https://github.com/aslanchek/RealGasModel/blob/master/3d.gif">
</p>

## Build

Requirements:

* C++17 compatible compiler
* cmake 3.1+
* OpenMP
* Eigen
* nlohmann/json
* fmt
* rapigcsv
 
## Building:

* Create and change working directory to build directory: `mkdir build && cd build`.
* Build on Linux: `cmake .. && cmake --build .`. It will build all required dependencies and the project itself.

Default simulation parameters, Lennard-Jones properties, etc. can be modified in data/conf.json.
All executable files will be placed in `build/` directory.

## Running and Usage:
* Run simulation: `./calc.exe`. It will save all simulation output data in `data/` directory.
* Run output formatter: `./output_format.exe`. You will be asked to specify filenames to format (e.g. `position_data.csv`, `absolute_position_data.csv`). It will format all output data in `data/` directory and save it with `.lammps` extension.
* Use OVITO to get 3D visualization: `File -> Load File -> <Auto-detect file format>(*)` and select formatted output file with extension `.lammps`.

## Configuration:

You can specify simulation (e.g. gas density, number of threads, Lennard-Jones constants, etc.) parameters in `data/conf.json` file.

## Acknowledgements:

This project contains 
* [Lennard-Jones potential](https://en.wikipedia.org/wiki/Lennard-Jones_potential)
* [Velocity Verlet algorithm](https://en.wikipedia.org/wiki/Verlet_integration#Velocity_Verlet)
* [OpenMP](https://www.openmp.org/)

## Results

See report_RU.pdf for more details.

## References:
* [Transport coefficients of the Lennard-Jones model fluid. II Self-diffusion](https://aip.scitation.org/doi/pdf/10.1063/1.1786579)
* [Self‐Diffusion in Argon](https://aip.scitation.org/doi/pdf/10.1063/1.1747116)
* [Молекулярно-динамическое
  моделирование процесса установления
  термодинамического равновесия
  нагретого никеля (RU)](https://keldysh.ru/papers/2014/prep2014_41.pdf)