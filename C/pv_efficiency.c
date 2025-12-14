// Compile in MATLAB:
//   mex pv_efficiency.c
//
// Usage in MATLAB:
//   [eta, P, Tcell, station] = pv_efficiency(Tmean, stations, filename);
//
// Inputs:
// - Tmean    : double array (°C), mean temperature for each station
// - stations : cell array of strings, name of each station
// - filename : char, name of the CSV file to write results
//
// Outputs:
// - eta      : efficiency of the PV cell
// - P        : power produced (W)
// - Tcell    : cell temperature (°C)
// - station  : station names

#include "mex.h"
#include <stdio.h>
#include <stdlib.h>

// Function used by qsort to sort data
// We sort from the higher efficiency to the lower one
int compare(const void *a, const void *b)
{
    double ea = ((double*)a)[2]; // efficiency of element a
    double eb = ((double*)b)[2]; // efficiency of element b
    return (eb > ea) - (eb < ea);
}

// Main entry point for the MEX function
void mexFunction(int nlhs, mxArray *plhs[],
                 int nrhs, const mxArray *prhs[])
{
    //Inputs from MATLAB
    double *Tmean = mxGetPr(prhs[0]);  // Mean temperatures
    size_t n = mxGetNumberOfElements(prhs[0]);  // Number of stations
    char *filename = mxArrayToString(prhs[2]);  // Name of the CSV file

    //Constants of the PV model 
    double eta_ref = 0.21;   // Reference efficiency
    double Tref    = 25.0;   // Reference temperature (°C)
    double beta    = 0.0045; // Temperature coefficient (1/°C)
    double G       = 1000.0; // Solar irradiance (W/m^2)
    double area    = 1.0;    // Cell area (m^2)

    // Memory allocation 
    double *data = mxCalloc(n * 3, sizeof(double));  // Table to store data: [Tcell, Power, Efficiency]
    char **names = mxCalloc(n, sizeof(char*));  // Array to store station names
 
    //Compute efficiency and power
    for (size_t i = 0; i < n; i++) {

        double T = Tmean[i];
        double eta = eta_ref * (1.0 - beta * (T - Tref));  // Efficiency formula depending on temperature
        if (eta < 0)   // Efficiency cannot be negative
            eta = 0;

        // Store results
        data[3*i + 0] = T;                // Tcell
        data[3*i + 1] = eta * G * area;   // Power
        data[3*i + 2] = eta;              // Efficiency

        // Get station name from MATLAB cell array
        names[i] = mxArrayToString(mxGetCell(prhs[1], (mwIndex)i));
    }

    // Sort data by efficiency (descending order)
    qsort(data, n, 3 * sizeof(double), compare);

    // Write results into CSV file
    FILE *f = fopen(filename, "w");

    fprintf(f, "Index;Station;Tcell;Efficiency;Power\n");

    for (size_t i = 0; i < n; i++) {
        fprintf(f, "%zu;%s;%.3f;%.3f;%.3f\n",
                i + 1,
                names[i],
                data[3*i + 0],
                data[3*i + 2] * 100.0, // efficiency in %
                data[3*i + 1]);
    }

    fclose(f);

    // Create MATLAB outputs 
    // Note: MATLAB memory for outputs is already allocated

    double *eta_out = mxGetPr(plhs[0]);
    double *P_out   = mxGetPr(plhs[1]);
    double *T_out   = mxGetPr(plhs[2]);

    for (size_t i = 0; i < n; i++) {

        T_out[i]   = data[3*i + 0];
        P_out[i]   = data[3*i + 1];
        eta_out[i] = data[3*i + 2];

        // Copy station name into MATLAB cell
        mxSetCell(plhs[3], (mwIndex)i, mxCreateString(names[i]));
    }

    // Free allocated memory

    for (size_t i = 0; i < n; i++)
        mxFree(names[i]);

    mxFree(names);
    mxFree(data);
    mxFree(filename);
}
