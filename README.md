# Solar-Panel

Our client, a Swiss energy company, asked us to identify the best locations for photovoltaic (PV) panel installations. Using long-term sunshine data from weather stations across Switzerland, we will analyze solar exposure and other parameters to rank potential sites and deliver data-driven recommendations for efficient and sustainable investments.

## Description
This project has the scope of find the best location in swiss to build a solar panel central. We analyse sunshine rate and efficiency.

### Input files
The input data comes from MeteoSwiss and is provided as CSV tables. Each row corresponds to a measurement station, while each column contains specific characteristics such as sunshine duration, temperature, and other relevant environmental variables.

### Output fils
The project outputs include cleaned datasets containing only the essential variables, as well as various graphs, images, and tables summarizing the calculations performed to determine the most suitable location.

### Report
The final report, including all methodologies and conclusions, can be found in the docs folder under the name finalreport.pdf.

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
## 📁 Folder Structure

To run the project, you must place all files in a single folder on your computer. The required structure is:
Your_folder/
```bash
├── Donnees_mensuelles/         # Monthly sunshine data (raw input) 
├── pv_efficiency.c             # C code for performance-critical calculations 
├── main_code.m                 # Main MATLAB script 
├── function_clean_data.m       # Cleans and formats raw data 
├── function_EPOA.m             # Computes Effective Plane of Array irradiance 
├── function_Tcell.m            # Estimates cell temperature /n
```

## Running the program

### Run the MATLAB Scripts

To launch the analysis, open MATLAB and run the main script:

```matlab
>> main_code
```
If the code runs correctly, it will produce:
- A clean table with all relevant data for the project
- Graphs showing which stations have the highest sunshine rates
- A table of estimated cell temperatures for each station


### Compile and Run the C Code

This part is handled automatically by the MATLAB code.
If the code runs correctly, it will generate three CSV files containing the cell efficiency data for each year (2022, 2023, and 2024). 

## Project Structure

- `/matlab` – MATLAB scripts for data analysis
- `/c` – C source files for computation
- `/data` – Sample and processed data sets
- `/docs` – Documentation and report files

---

## Contributors
Perle Boucard and Agathe Gaudillat

## Licence

MIT
