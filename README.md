# Solar-Panel

## Description

This project aims to identify the most suitable locations for building solar power plants in Switzerland.
We analyze sunshine duration, environmental conditions, and expected PV panel performance using real meteorological data.
The goal is to compare weather stations across Switzerland to determine which locations offer the best potential for photovoltaic installations. The workflow combines:
MeteoSwiss datasets (sunshine duration, temperature, etc.)
MATLAB scripts for data cleaning, analysis, and visualization
C code for performance-critical PV efficiency calculations
The complete methodology and conclusions can be found in
Project_Proposal_PV.pdf.

### Input files

Raw data is provided by MeteoSwiss as CSV files:
- Each row represents a measurement station
- Each column corresponds to an environmental variable

### Output files

The project produces:
- Cleaned datasets containing only relevant variables
- Graphs comparing sunshine duration and PV performance across stations
- CSV files with calculated photovoltaic efficiencies for 2022, 2023, and 2024

## Project Proposal
[Voir le projet en PDF](https://github.com/agathegaudillat-commits/Solar-Panel/raw/main/Project_Proposal_PV.pdf)

---
## Requirements & Dependencies

To ensure reproducibility, all dependencies (languages, compilers, libraries, versions) must be declared.

- **Languages used**:

  - High-level: MATLAB
  - Low-level: C

- **Dependencies**:

  - MATLAB 2025
  - GCC (for compiling the C code) — MATLAB alone is not sufficient
  - PVlib for matlab
---

## Installing PVlib for MATLAB

Follow these steps to install PVlib:

1. **Access the Repository**  
   Visit the official MATLAB PVlib repository on GitHub:  
   [https://github.com/sandialabs/MATLAB_PV_LIB](https://github.com/sandialabs/MATLAB_PV_LIB)

2. **Download the Library**  
   - Click on the green **Code** button.  
   - Select **Download ZIP** to download the entire library to your computer.

3. **Extract the Files**  
   - Extract the ZIP file to a folder of your choice.  
   - Example paths:  
     - Windows: `C:\MATLAB\PVlib`  
     - macOS/Linux: `/Users/YourName/MATLAB/PVlib`

4. **Add PVlib to MATLAB Path**  
   - Open MATLAB.  
   - Go to **Home > Set Path > Add Folder**, then select the folder where PVlib was extracted.  
   - Click **Save** to ensure MATLAB can access the library in future sessions.

5. **Verify the Installation**  
   In the MATLAB Command Window, type:  
   ```matlab
   help pvl_version


## Installation of the repository

Clone the repository and set up the environment:
```bash
git clone https://github.com/agathegaudillat-commits/Solar-Panel.git
cd Solar-Panel
# Configure MATLAB as needed
```
---
## 📁 Folder Structure
To run the project, you must place all files in a single folder on your computer. The required structure is:

Your_folder/
```bash
├── donnees_mensuelles/         # Monthly sunshine data (raw input)
├── coordonnees_stations/       # Coordinates of each station for the map
├── pv_efficiency.c             # C code for performance-critical calculations 
├── main_code.m                 # Main MATLAB script 
├── function_clean_data.m       # Cleans and formats raw data 
├── function_EPOA.m             # Computes Effective Plane of Array irradiance 
├── function_Tcell.m            # Estimates cell temperature 
```
## Running the program

### Run the MATLAB Scripts

To launch the analysis, open MATLAB and run the main script:
```matlab
>> main_code
```
The script generates:
- A cleaned and formatted table of essential variables
- Graphs showing station-level sunshine comparisons
- A table of estimated PV cell temperatures

### Compile and Run the C Code

No manual action is required.
The C code is compiled and executed automatically via MATLAB using a MEX interface or system call.
If successful, you obtain:
- `efficiency_2022.csv`
- `efficiency_2023.csv`
- `efficiency_2024.csv`

## Workflow summary

```text
Raw Data (MeteoSwiss)
        ↓
Data Cleaning (function_clean_data.m)
        ↓
Calculation of the average sunrate (MeteoSwiss)
        ↓
EPOA Calculation — Effective Plane of Array (function_EPOA.m)
        ↓
PV Cell Temperature Estimation (function_Tcell.m)
        ↓
PV Efficiency Computation in C (pv_efficiency.c)
        ↓
Final Outputs:
    • Clean CSV files (2022, 2023, 2024)
    • Graphs & visualizations
```
## Contributors

Perle Boucard and Agathe Gaudillat





## Licence





MIT
