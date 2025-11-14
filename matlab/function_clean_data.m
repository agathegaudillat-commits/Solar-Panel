function [cleaned_data_2024, cleaned_data_2023, cleaned_data_2022] = function_clean_data()
% Ouvre tous les CSV du dossier 'donnees_mensuelles' et renvoie
% trois cell arrays avec uniquement les stations complètes (sans NaN)
% pour les années 2024, 2023 et 2022.

    folder = fullfile(pwd,'donnees_mensuelles');
    files  = dir(fullfile(folder, '*.csv'));

    % Variables minimales exigées (si l'une manque/NaN -> on supprime la station)
    vars_to_keep = {'tre005m0','fkl010m0','gre000m0','sre000m0','sremaxmv'};

    % Pré-allocation des sorties
    out2024 = cell(numel(files), 1);
    out2023 = cell(numel(files), 1);
    out2022 = cell(numel(files), 1);
    k2024 = 0; k2023 = 0; k2022 = 0;

    for i = 1:numel(files)
        fname = fullfile(folder, files(i).name);
        T = readtable(fname);

        % Datation
        date_col = datetime(T{:,2}, 'InputFormat','dd.MM.yyyy HH:mm');

        for target_year = [2024, 2023, 2022]

            is_year = year(date_col) == target_year;

            if any(is_year)
                % Colonnes
                first_col  = T(is_year, 1);                         % abréviation
                date_table = table(date_col(is_year), 'VariableNames', {'Date'});
                kept_cols  = T(is_year, vars_to_keep);

                % Table nettoyée
                Tclean = [first_col, date_table, kept_cols];

                % Renommer la première colonne si besoin ('station_abbr')
                if ~strcmp(Tclean.Properties.VariableNames{1}, 'station_abbr')
                    Tclean.Properties.VariableNames{1} = 'station_abbr';
                end

                % === FILTRE : supprimer la station si AU MOINS UNE valeur manque ===
                has_missing = any(ismissing(Tclean(:, vars_to_keep)), 'all');

                if ~has_missing
                    switch target_year
                        case 2024
                            k2024 = k2024 + 1;
                            out2024{k2024} = Tclean;
                        case 2023
                            k2023 = k2023 + 1;
                            out2023{k2023} = Tclean;
                        case 2022
                            k2022 = k2022 + 1;
                            out2022{k2022} = Tclean;
                    end
                end
            end
        end
    end

    % Compacte les cellules (enlève les vides)
    cleaned_data_2024 = out2024(1:k2024);
    cleaned_data_2023 = out2023(1:k2023);
    cleaned_data_2022 = out2022(1:k2022);
end
