function Tcell_table = compute_Tcell(EPOA_table, cleaned_data, U0, U1)
%--------------------------------------------------------------------------
% compute_Tcell : Calculates the monthly PV cell temperature (°C)
%   T_cell = T_a + EPOA / (U0 + U1 * WS)
%
% Inputs :
%   EPOA_table : table [N x 14] with columns :
%                Abbreviation_station | Jan | Feb | Mar | ... | Dec | EPOA_moy_annuelle 
%
%   cleaned_data : cell (N x 1), each cell = monthly table (12x7) with columns:
%               station_abbr | Date | tre005m0 | ... | sremaxmv
%
%   U0, U1 : Heat transfer parameters (W/m²K and W/m³sK).
%                 Default values if not provided : U0 = 25 ; U1 = 6.84
%
% Output :
%   Tcell_table : table [N x 14] with columns : 
%               Abbreviation_station | Jan | Feb | Mar | ... | Dec | Tcell_moy_annuelle 

% Default settings (if not provided)
if nargin < 3, U0 = 25; end       
if nargin < 4, U1 = 6.84; end     

% Preparation 
month_names = EPOA_table.Properties.VariableNames(2:13); 
stations_epoa = EPOA_table.Abbreviation_station;   % <-- changé ici

% Pre-allocation of results
Tcell_matrix = NaN(height(EPOA_table), 12);

% Loop through the stations present in EPOA_table 
for i = 1:height(EPOA_table)

    abbr = stations_epoa{i};

    % Find the corresponding table in cleaned_data
    idx = find(cellfun(@(t) strcmp(strrep(t.station_abbr{1},'''',''), abbr), cleaned_data), 1, 'first');
    data = cleaned_data{idx};

    % Monthly weather variables
    Ta = data.tre005m0;         % °C
    WS = data.fkl010m0;         % m/s

    % EPOA conversion: kWh/m²/month - W/m² (monthly average)
    Date = datetime(data.Date,'InputFormat','dd.MM.yyyy HH:mm');
    ndays = eomday(year(Date), month(Date));         
    hours_in_month = 24 .* ndays;                   

    EPOA_kWh = EPOA_table{i, month_names};           
    EPOA_kWh = EPOA_kWh(:);                          
    EPOA_Wm2 = (EPOA_kWh * 1000) ./ hours_in_month;  

    % T_cell formula (element by element)
    Tcell = Ta + EPOA_Wm2 ./ (U0 + U1 .* WS);        

    % Stockage
    Tcell_matrix(i, :) = Tcell.';
end

% Create the final table
Tcell_table = array2table(Tcell_matrix, 'VariableNames', month_names);
Tcell_table.Abbreviation_station = EPOA_table.Abbreviation_station;   % <-- changé ici
Tcell_table = movevars(Tcell_table, 'Abbreviation_station', 'Before', 'Jan');

% Annual average
Tcell_table.Tcell_annual_mean = mean(Tcell_matrix, 2);

fprintf('Tcell calculation completed for %d stations.\n', numel(cleaned_data));
end

