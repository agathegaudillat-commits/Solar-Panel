// pv_efficiency.c
// Compilation: gcc -std=c99 -O2 -o pv_efficiency pv_efficiency.c
// Execution: ./pv_efficiency

// This code calculates the efficiency of a solar panel based on cell temperature and other parameters

#include <stdio.h>  // for printf(), fprintf(), fopen(), fclose(),...
#include <stdlib.h> // for qsort(), malloc(),...

// definition of data structures

// Structure describing a type of PV cell
typedef struct {
    const char *name; // cell technology name
    double eta_min;   // minimum efficiency in %
    double eta_max;   // maximum efficiency in %
} CellType;

// Structure used to store calculation results and associated station name
typedef struct {
    const char *station; // station short name (e.g., "COM")
    double Tcell; // cell temperature (°C)
    double eta;   // efficiency (as a fraction, e.g., 0.22 = 22%)
    double P;     // power output (W)
} Result;

// Comparison function for qsort() to sort by descending efficiency
int compare_desc(const void *a, const void *b) {
    double diff = ((Result*)b)->eta - ((Result*)a)->eta;
    return (diff > 0) - (diff < 0); // returns +1, 0, or -1 for descending order
}

// Main program
int main(void) {
    // basic input data
    // Cell technology: Monocrystalline silicon (typical commercial module)
    CellType type = {"Monocrystalline silicon (PERC, TOPCon)", 18.0, 24.0};
    double eta_ref = ((type.eta_min + type.eta_max) / 2.0) / 100.0; // average
    double Tref = 25.0;   // reference temperature (°C)
    double beta = 0.0045; // temperature coefficient (1/°C)
    double G = 1000.0;    // irradiance (W/m²)
    double area = 1.0;    // module area (m²)

    printf("Automatic PV simulation with CSV export \n");
    printf("Technology: %s\n", type.name);
    printf("eta_ref = %.2f%%, Tref = %.1f°C, beta = %.4f 1/°C, G = %.0f W/m², A = %.1f m²\n\n",
           eta_ref * 100, Tref, beta, G, area);

    // Array of annual average cell temperatures (example data set)
    // Each value represents a measured or simulated cell temperature (°C)
    double Tcell_avg_annual[] = {
        12.0160, 9.3685, 12.7916, 12.3780, 6.8380, 6.9369, 13.0152, 13.7139, 12.4287, 13.1569,
        6.9884, 12.6867, 10.6504, 9.2900, 6.7853, 12.1096, 10.2407, 11.2238, 10.5449, 9.8330,
        10.2743, 8.5559, 12.1233, 12.3748, 11.7560, 12.9408, 12.3778, 13.1684, 12.1612, 10.0195,
        11.8449, 12.4067, 8.3441, 10.6015, 9.2738, 11.0877, 13.4392, 15.6150, 15.6525, 12.8992,
        14.4268, 11.2347, 10.0868, 13.1254, 8.3358, 13.3880, 13.4881, 12.9741, 12.0255, 10.4576,
        9.8090, 12.5788, 6.6210, 5.7742, 12.7303, 9.4811, 5.2899, 14.5829, 10.6758, 9.0264,
        14.2138, 6.8102, 13.4517, 13.0105, 12.9773, 12.9241, 12.9606, 12.6483
    };
    int nT = sizeof(Tcell_avg_annual) / sizeof(Tcell_avg_annual[0]);

    // Short station names (should match the order of the Tcell values above)
    const char* Station[] = {
        "COM", "ABO", "AIG", "ALT", "ANT", "ARO", "RAG", "BAS", "BER", "BEZ", "BLA", "BUS",
        "CHD", "CHM", "DAV", "DEM", "DIS", "EBK", "EIN", "ELM", "ENG", "EVO", "FAH", "GRA",
        "GLA", "GOE", "GRO", "GUT", "HLL", "HOE", "INT", "KOP", "BRL", "CDF", "FRE", "LAG",
        "LEI", "OTL", "LUG", "LUZ", "MAG", "MER", "MVE", "MUB", "NAP", "NEU", "CGI", "PAY",
        "PFA", "PLF", "ROB", "RUE", "SBE", "SAM", "SHA", "SCU", "SIA", "SIO", "STG", "SMM",
        "SBO", "ULR", "VAD", "WAE", "WYN", "REH", "SMA", "KLO"
    };
    int nStation = sizeof(Station) / sizeof(Station[0]);

    if (nStation != nT) {
        fprintf(stderr, "Warning: number of stations (%d) does not match number of Tcell values (%d).\n", nStation, nT);
        fprintf(stderr, "Only the first %d entries will be paired.\n", (nStation < nT) ? nStation : nT);
    }

    // Calculations for each temperature value
    // We'll create results for the number of valid pairs we can form
    int nPairs = (nStation < nT) ? nStation : nT;
    Result results[nPairs]; // array of results associated with station names
    for (int i = 0; i < nPairs; ++i) {
        double Tcell = Tcell_avg_annual[i];
        // Efficiency model:
        // eta(T) = eta_ref * [1 - beta * (Tcell - Tref)]
        double eta = eta_ref * (1.0 - beta * (Tcell - Tref));
        if (eta < 0.0) eta = 0.0; // Ensure efficiency cannot be negative
        double P = eta * G * area; // Power output: P = eta * G * area
        results[i].station = Station[i];
        results[i].Tcell = Tcell; // Store results
        results[i].eta = eta;
        results[i].P = P;
    }

    // Sort results in descending order of efficiency
    qsort(results, nPairs, sizeof(Result), compare_desc);

    // Export CSV
    FILE *f = fopen("results.csv", "w");
    if (!f) {
        perror("Error while creating the CSV file");
        return 1;
    }

    fprintf(f, "Index;Station;Tcell(°C);Efficiency(%%);Power(W)\n");
    for (int i = 0; i < nPairs; ++i) {
        fprintf(f, "%d;%s;%.4f;%.4f;%.4f\n",
                i+1, results[i].station, results[i].Tcell, results[i].eta * 100.0, results[i].P);
    }
    fclose(f);

    printf("\n Results saved in 'results.csv'\n");

    return 0;
}
