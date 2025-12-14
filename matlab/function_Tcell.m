function Tcell_table = compute_Tcell(EPOA_table, cleaned_data, U0, U1)

% compute_Tcell : Calculates the monthly PV cell temperature Tcell [°C]
%   using the empirical relation by Faiman:
%       Tcell = Ta + EPOA / (U0 + U1 * WS)
%
% Inputs :
%   EPOA_table : table [N x 14] with columns :
%                Abbreviation_station | Jan | Feb | ... | Dec | EPOA_annual_mean
%                where Jan..Dec are monthly plane-of-array irradiation values
%                expressed in [kWh/m^2/month].
%
%   cleaned_data : cell (N x 1), each cell = monthly table (12x7) with columns :
%               station_abbr | Date | tre005m0 | fkl010m0 | gre000m0 | sre000m0 | sremaxmv
%               where:
%                 - tre005m0 : air temperature at 5 cm [°C]
%                 - fkl010m0 : wind speed at 10 m [m/s]
%                 - gre000m0 : monthly mean Global Horizontal Irradiance (GHI) [W/m^2]
%                 - sre000m0 : sunshine duration [min]
%                 - sremaxmv : relative sunshine [%]
%
%   U0, U1 : Heat transfer parameters of the PV module thermal model:
%                 - U0 [W/(m^2·K)]
%                 - U1 [W·s/(m^3·K)] 
%                 Default values: U0 = 25 ; U1 = 6.84
%
% Output :
%   Tcell_table : table [N x 14] with columns :
%               Abbreviation_station | Jan | Feb | ... | Dec | Tcell_annual_mean
%               where Jan..Dec are monthly PV cell temperatures Tcell [°C].

% Default settings 
if nargin < 3, U0 = 25; end       
if nargin < 4, U1 = 6.84; end     

% Preparation 
month_names = EPOA_table.Properties.VariableNames(2:13); 
stations_epoa = EPOA_table.Abbreviation_station;   

% Pre-allocation of results
Tcell_matrix = NaN(height(EPOA_table), 12);

% Loop through the stations present in EPOA_table 
for i = 1:height(EPOA_table)

    abbr = stations_epoa{i};

    % Find the corresponding table in cleaned_data
    idx = find(cellfun(@(t) string(t.station_abbr{1}) == abbr, cleaned_data), 1, 'first');
    data = cleaned_data{idx};

    % Monthly weather variables
    Ta = data.tre005m0;         % °C
    WS = data.fkl010m0;         % m/s

    % EPOA conversion: kWh/m²/month - W/m² (monthly average)
    Date = datetime(data.Date,'InputFormat','dd.MM.yyyy HH:mm');
    ndays = eomday(year(Date), month(Date));  % Number of days in each month (accounts for leap years)       
    hours_in_month = 24 .* ndays;                   

    EPOA_kWh = EPOA_table{i, month_names};           
    EPOA_kWh = EPOA_kWh(:);                          
    EPOA_Wm2 = (EPOA_kWh * 1000) ./ hours_in_month;  % Convert monthly EPOA (kWh/m^2/month) to mean irradiance (W/m^2)

    % T_cell formula 
    Tcell = Ta + EPOA_Wm2 ./ (U0 + U1 .* WS);        

    % Stockage
    Tcell_matrix(i, :) = Tcell.';
end

% Create the final table
Tcell_table = array2table(Tcell_matrix, 'VariableNames', month_names);  % Table 
Tcell_table.Abbreviation_station = EPOA_table.Abbreviation_station;  
Tcell_table = movevars(Tcell_table, 'Abbreviation_station', 'Before', 'Jan'); % Move 'Abbreviation_station' to the first column 

% Annual average
Tcell_table.Tcell_annual_mean = mean(Tcell_matrix, 2);

end

