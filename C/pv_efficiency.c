// pv_efficiency.c (version MEX pour MATLAB)
// Compilation dans MATLAB :
//    mex pv_efficiency.c
//
// Utilisation dans MATLAB :
//    [eta, P, Tcell, station] = pv_efficiency();
//
// - eta      : vecteur colonne des rendements (fraction, ex: 0.22 = 22%)
// - P        : vecteur colonne des puissances (W)
// - Tcell    : vecteur colonne des températures de cellule (°C)
// - station  : cell array de chaînes (abréviations des stations)
//
// Le code écrit aussi un fichier "results.csv" dans le dossier courant MATLAB,
// identique à celui généré par ta version C d’origine.

#include "mex.h"
#include <stdio.h>   // fopen, fprintf, fclose, printf...
#include <stdlib.h>  // qsort, malloc, free

// Structure décrivant un type de cellule PV
typedef struct {
    const char *name; // nom de la technologie
    double eta_min;   // rendement min en %
    double eta_max;   // rendement max en %
} CellType;

// Structure pour stocker les résultats
typedef struct {
    const char *station; // nom court de la station
    double Tcell;        // température de cellule (°C)
    double eta;          // rendement (fraction, ex: 0.22 = 22%)
    double P;            // puissance (W)
} Result;

// Fonction de comparaison pour qsort() (ordre décroissant d'efficacité)
int compare_desc(const void *a, const void *b) {
    double diff = ((Result*)b)->eta - ((Result*)a)->eta;
    return (diff > 0) - (diff < 0); // +1, 0 ou -1
}

// Point d'entrée MEX
void mexFunction(int nlhs, mxArray *plhs[],
                 int nrhs, const mxArray *prhs[])
{
    (void)nrhs;   // on n'utilise pas d'arguments d'entrée
    (void)prhs;

    // --- Données "de base" (comme dans ton main) ---

    CellType type = {"Monocrystalline silicon (PERC, TOPCon)", 18.0, 24.0};
    double eta_ref = ((type.eta_min + type.eta_max) / 2.0) / 100.0; // moyenne
    double Tref = 25.0;   // température de référence (°C)
    double beta = 0.0045; // coefficient thermique (1/°C)
    double G = 1000.0;    // irradiance (W/m²)
    double area = 1.0;    // surface du module (m²)

    mexPrintf("Automatic PV simulation with CSV export\n");
    mexPrintf("Technology: %s\n", type.name);
    mexPrintf("eta_ref = %.2f%%, Tref = %.1f°C, beta = %.4f 1/°C, G = %.0f W/m², A = %.1f m²\n\n",
              eta_ref * 100, Tref, beta, G, area);

    // Tableau des températures moyennes annuelles de cellule
    double Tcell_avg_annual[] = {
        12.0160, 9.3685, 12.7916, 12.3780, 6.8380, 6.9369, 13.0152, 13.7139, 12.4287, 13.1569,
        6.9884, 12.6867, 10.6504, 9.2900, 6.7853, 12.1096, 10.2407, 11.2238, 10.5449, 9.8330,
        10.2743, 8.5559, 12.1233, 12.3748, 11.7560, 12.9408, 12.3778, 13.1684, 12.1612, 10.0195,
        11.8449, 12.4067, 8.3441, 10.6015, 9.2738, 11.0877, 13.4392, 15.6150, 15.6525, 12.8992,
        14.4268, 11.2347, 10.0868, 13.1254, 8.3358, 13.3880, 13.4881, 12.9741, 12.0255, 10.4576,
        9.8090, 12.5788, 6.6210, 5.7742, 12.7303, 9.4811, 5.2899, 14.5829, 10.6758, 9.0264,
        14.2138, 6.8102, 13.4517, 13.0105, 12.9773, 12.9241, 12.9606, 12.6483
    };
    int nT = (int)(sizeof(Tcell_avg_annual) / sizeof(Tcell_avg_annual[0]));

    // Noms courts des stations
    const char* Station[] = {
        "COM", "ABO", "AIG", "ALT", "ANT", "ARO", "RAG", "BAS", "BER", "BEZ", "BLA", "BUS",
        "CHD", "CHM", "DAV", "DEM", "DIS", "EBK", "EIN", "ELM", "ENG", "EVO", "FAH", "GRA",
        "GLA", "GOE", "GRO", "GUT", "HLL", "HOE", "INT", "KOP", "BRL", "CDF", "FRE", "LAG",
        "LEI", "OTL", "LUG", "LUZ", "MAG", "MER", "MVE", "MUB", "NAP", "NEU", "CGI", "PAY",
        "PFA", "PLF", "ROB", "RUE", "SBE", "SAM", "SHA", "SCU", "SIA", "SIO", "STG", "SMM",
        "SBO", "ULR", "VAD", "WAE", "WYN", "REH", "SMA", "KLO"
    };
    int nStation = (int)(sizeof(Station) / sizeof(Station[0]));

    if (nStation != nT) {
        mexPrintf("Warning: number of stations (%d) does not match number of Tcell values (%d).\n",
                  nStation, nT);
        mexPrintf("Only the first %d entries will be paired.\n",
                  (nStation < nT) ? nStation : nT);
    }

    int nPairs = (nStation < nT) ? nStation : nT;

    // IMPORTANT pour compatibilité MEX/Windows : pas de VLA -> malloc
    Result *results = (Result *)mxCalloc(nPairs, sizeof(Result));
    if (!results) {
        mexErrMsgIdAndTxt("pv_efficiency:allocationFailed",
                          "Memory allocation failed for results.");
    }

    // Calculs
    for (int i = 0; i < nPairs; ++i) {
        double Tcell = Tcell_avg_annual[i];
        double eta = eta_ref * (1.0 - beta * (Tcell - Tref));
        if (eta < 0.0) eta = 0.0; // rendement non négatif
        double P = eta * G * area;

        results[i].station = Station[i];
        results[i].Tcell   = Tcell;
        results[i].eta     = eta;
        results[i].P       = P;
    }

    // Tri décroissant par rendement
    qsort(results, nPairs, sizeof(Result), compare_desc);

    // Export CSV
    FILE *f = fopen("results.csv", "w");
    if (!f) {
        mxFree(results);
        mexErrMsgIdAndTxt("pv_efficiency:fileError",
                          "Error while creating 'results.csv'.");
    }

    fprintf(f, "Index;Station;Tcell(°C);Efficiency(%%);Power(W)\n");
    for (int i = 0; i < nPairs; ++i) {
        fprintf(f, "%d;%s;%.4f;%.4f;%.4f\n",
                i+1, results[i].station,
                results[i].Tcell,
                results[i].eta * 100.0,
                results[i].P);
    }
    fclose(f);

    mexPrintf("\nResults saved in 'results.csv'\n");

    // --- Sorties MATLAB (optionnelles) ---
    // plhs[0] : eta (nPairs x 1)
    // plhs[1] : P   (nPairs x 1)
    // plhs[2] : Tcell (nPairs x 1)
    // plhs[3] : station (nPairs x 1, cell array de strings)

    if (nlhs > 0) {
        // eta
        plhs[0] = mxCreateDoubleMatrix(nPairs, 1, mxREAL);
        double *eta_out = mxGetPr(plhs[0]);
        for (int i = 0; i < nPairs; ++i) {
            eta_out[i] = results[i].eta;
        }
    }

    if (nlhs > 1) {
        // P
        plhs[1] = mxCreateDoubleMatrix(nPairs, 1, mxREAL);
        double *P_out = mxGetPr(plhs[1]);
        for (int i = 0; i < nPairs; ++i) {
            P_out[i] = results[i].P;
        }
    }

    if (nlhs > 2) {
        // Tcell
        plhs[2] = mxCreateDoubleMatrix(nPairs, 1, mxREAL);
        double *T_out = mxGetPr(plhs[2]);
        for (int i = 0; i < nPairs; ++i) {
            T_out[i] = results[i].Tcell;
        }
    }

    if (nlhs > 3) {
        // station (cell array de strings)
        plhs[3] = mxCreateCellMatrix(nPairs, 1);
        for (int i = 0; i < nPairs; ++i) {
            mxArray *str = mxCreateString(results[i].station);
            mxSetCell(plhs[3], i, str);
        }
    }

    // Libération de la mémoire
    mxFree(results);
}

