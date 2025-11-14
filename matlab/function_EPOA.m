function EPOA_table = compute_EPOA(cleaned_data, T)

% compute_EPOA : Calculates the Plane-of-Array Irradiance (EPOA)
% for each station contained in cleaned_data, based on the GHI (gre000m0).
%
% Inputs :
%   cleaned_data : cell (N x 1), each cell = monthly table (12x7) with columns :
%               station_abbr | Date | tre005m0 | fkl010m0 | ... | sremaxmv
%               
%   T : table [N x 6] of coordinates with columns :
%            Station | Abr | Hauteur de mesure m s. mer | Latitude | Longitude | Canton
%
% Output :
%   EPOA_table : table [N x 14] with columns:
%                Abbreviation_station | Jan | Feb | Mar | ... | Dec | EPOA_moy_annuelle
%
% Requires MATLAB PVlib : https://github.com/sandialabs/MATLAB_PV_LIB


% Module settings 
tilt = 30;          % [°]
azimuth = 180;      % [°] (180° = south-facing)
rho = 0.2;          % mean albedo

% Extract abbreviations
abbr_clean = cellfun(@(tbl) tbl.station_abbr{1}, cleaned_data, 'UniformOutput', false);
abbr_T = T.("Abr.");

% Pre-allocation of results
month_names = {'Jan','Feb','Mar','Apr','May','Jun','Jul','Aug','Sep','Oct','Nov','Dec'};
EPOA_matrix = NaN(numel(cleaned_data), 12);  % [stations x mois]

% Main loop 
for k = 1:numel(cleaned_data)
    
    data = cleaned_data{k};
    abbr = abbr_clean{k};
    
    lat = T.Latitude(k);
    lon = T.Longitude(k);
    alt = T.('Hauteur de mesure m s. mer')(k);
    
    % Extract data
    GHI = data.gre000m0;
    Date = datetime(data.Date, 'InputFormat', 'dd.MM.yyyy HH:mm');
    
    % Create the PVlib time structure
    time = pvl_maketimestruct(datenum(Date), lon);

    % Create the localisation structure
    location.latitude = lat;
    location.longitude = lon;
    location.altitude = alt;

    % Calculation of solar position (elevation via SPA)
    solpos_el = pvl_spa(time, location);   % return elevation [°]
    SunZen = 90 - solpos_el;               % conversion elevation/zenith
    SunZen = max(0, SunZen);               % avoid negative values (night)

    % Azimuth calculation with pvl_ephemeris 
    SunAz = pvl_ephemeris(time, location); % solar azimuth [°]

    % Approximation of DHI/DNI from GHI
    [DNI, DHI] = pvl_disc(GHI, SunZen, day(Date));

    % Calculation of the angle of incidence 
    AOI = pvl_getaoi(tilt, azimuth, SunZen, SunAz);
    
    % Components
    Gbeam = DNI .* max(0, cosd(AOI));                 
    Gsky_diffuse = DHI .* (1 + cosd(tilt)) / 2;
    Gground = rho .* GHI .* (1 - cosd(tilt)) / 2;
    
    % Total irradiance 
    EPOA = Gbeam + Gsky_diffuse + Gground;

    % Conversion from W/m² to kWh/m²/month 
    hours_in_month = hours(diff([datetime(Date(1)), datetime(Date(end) + calmonths(1))]));
    EPOA = EPOA * hours_in_month * 1e-3;
    
    % Saving in the matrix 
    EPOA_matrix(k, :) = EPOA(:)';  % one line per station
end

% Create the final table
EPOA_table = array2table(EPOA_matrix, 'VariableNames', month_names);
EPOA_table.Abbreviation_station = abbr_clean(:);
EPOA_table = movevars(EPOA_table, 'Abbreviation_station', 'Before', 'Jan');

EPOA_table.EPOA_annual_mean = mean(EPOA_matrix, 2);

fprintf('EPOA calculation completed for %d stations.\n', numel(cleaned_data));

end
