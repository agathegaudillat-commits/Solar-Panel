# Solar-Panel

Our client, a Swiss energy company, asked us to identify the best locations for photovoltaic (PV) panel installations. Using long-term sunshine data from weather stations across Switzerland, we will analyze solar exposure and other parameters to rank potential sites and deliver data-driven recommendations for efficient and sustainable investments.

## Description

- **Motivation**: Renewable energy will play an increasingly central role in reducing dependence on fossil fuels. Solar energy, in particular, is fundamental to the energy transition. We are all concerned by this challenge and aim to better understand how to optimize the development of solar parks in the future. Selecting optimal locations for solar panels is not straightforward, as multiple factors must be considered. These include sunshine duration (which is central to our project), topography, accessibility, and urbanization, which can act as a constraint.

Meanwhile, numerous studies are currently underway to improve the efficiency and durability of solar panels. These efforts focus on developing new cell technologies, optimizing module angle and orientation, and integrating cooling systems. These advancements underscore the importance of rigorous site selection to maximize the benefits of emerging technologies under real environmental conditions.

- **Scope**: Our objective is to compare the amount of sunshine at different sites and the influence of certain local parameters on the yield of solar panels in order to identify the most favorable location for installing a photovoltaic park. The photovoltaic technology selected for this project is monocrystalline silicon, as it is the most widely used type of PV cell, accounting for approximately 80–90% of global installations [8]. Its high efficiency, durability, and low cost make it the most suitable choice. The project requires a minimum PV surface area of 2 hectares. The photovoltaic park will be installed with a fixed tilt angle of 30° relative to the horizontal plane, facing due south. This configuration is chosen to optimize the angle of incidence of solar radiation throughout the year, thereby maximizing energy yield. The 30° inclination is consistent with best practices for fixed installations at Swiss latitudes and ensures a balanced capture of solar irradiance across seasons [4].

We will use data from MeteoSuisse [1], such as solar radiation, irradiance and temperature, to analyses the duration of sunshine over several years and to estimate the potential output of the park. Where possible, we will also take the terrain into account in order to assess any potential difficulties associated with the installation. The analysis will cover two or three years of data to ensure more reliable results. Our study focuses on data analysis rather than spatial visualization. While mapping can complement the results, our objective is to derive quantitative indicators of solar potential.

---

## Project Proposal

[Voir le projet en PDF](https://github.com/agathegaudillat-commits/Solar-Panel/raw/main/Project_Proposal_PV.pdf)

---

## Requirements & Dependencies

To ensure reproducibility, all dependencies (languages, compilers, libraries, versions) must be declared.

- **Languages used**:
  - High-level: MATLAB
  - Low-level: C

- **Dependencies**:
  - MatLab 2025
  - GCC 12.2.0 (for compiling C code)

---

## Installation

Clone the repository and set up the environment:

```bash
git clone https://github.com/agathegaudillat-commits/Solar-Panel.git
cd Solar-Panel
# Configure MATLAB and GCC as needed
```

---

## Usage

1. Prepare your data according to the format described in the documentation.
2. Run the MATLAB scripts for analysis.
3. (Optional) Compile and run C code for performance-critical computations:
   ```bash
   gcc -o analysis analysis.c
   ./analysis
   ```
4. Review the output and data-driven recommendations.

---

## Project Structure

- `/matlab` – MATLAB scripts for data analysis
- `/c` – C source files for computation
- `/data` – Sample and processed data sets
- `/docs` – Documentation and report files

---

## Contributing

Contributions are welcome! Please open an issue or submit a pull request.

---

## Licence

MIT
