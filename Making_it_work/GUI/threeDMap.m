function plot3DOnClick()
    % Create the figure
    f = figure('Name', '3D Plot on Mouse Click', ...
               'WindowButtonDownFcn', @refreshPlot);

    % Initial plot
    refreshPlot();

    % Callback function to refresh the 3D plot
    function refreshPlot(~, ~)
        % Access the BigTable
        global BigTable;

        % Validate existence of required columns
        if ~all(ismember({'X', 'Y', 'Z'}, BigTable.Properties.VariableNames))
            error('BigTable must contain columns: X, Y, and Z');
        end

        % Clear current axes
        clf;

        % 3D plot
        plot3(BigTable.X, BigTable.Y, BigTable.Z, 'o-', 'LineWidth', 1.5);
        xlabel('X');
        ylabel('Y');
        zlabel('Z');
        title('3D Plot of BigTable (Click to Refresh)');
        grid on;
        rotate3d on;
    end
end
