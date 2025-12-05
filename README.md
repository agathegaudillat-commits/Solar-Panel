# Solar-Panel

## Description

The goal of this project is to determine the most suitable locations in Switzerland for building solar power plants. We evaluate sunshine duration and panel efficiency to guide investment decisions.

1. Ouvrez MATLAB.

2. Téléchargez PVlib depuis [MATLAB File Exchange](https://fr.mathworks.com/matlabcentral/fileexchange/87774-pvlib-matlab) ou utilisez la commande suivante :
   ```matlab
   matlab.addons.install('chemin/vers/le/fichier/PVlib.mltbx')
   ```
3. Suivez les instructions sur la [documentation officielle PVlib](https://pvlib-python.readthedocs.io/en/latest/index.html) si vous avez besoin d'aide.

### Input files

The input data comes from MeteoSwiss and is provided as CSV tables. Each row corresponds to a measurement station, while each column contains specific characteristics such as sunshine duration, temperature, and other relevant environmental variables.

### Output files

The project outputs include cleaned datasets containing only the essential variables, as well as various graphs and tables summarizing the calculations performed to determine the most suitable location.

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

  - MATLAB 2025
  - GCC (for compiling the C code) — MATLAB alone is not sufficient
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
Make sure that the file **pv_efficiency.c** is located in the same folder.  
If the code runs correctly, it will generate three CSV files containing the cell efficiency data for the years **2022, 2023, and 2024**.  
You do not need to open VS Code or any other application to execute this step.

## Project Structure

- `/matlab` – MATLAB scripts for data analysis


- `/c` – C source files for computation


- `/data` – Sample and processed data sets


- `/docs` – Documentation and report files

---
Assurez-vous que la librairie est bien installée avant d'exécuter les scripts du projet.



## Contributors


Perle Boucard and Agathe Gaudillat





## Licence





MIT
