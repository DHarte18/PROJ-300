function Timeline(selLapTable)
% setupTimelineClickPlot - Interactive timeline that triggers a callback
%                          on each user click with the selected time.
%
% INPUT:
%   selLapTable - table with 'Var2' column containing times in milliseconds

    % Create figure and plot
    figureHandle = figure;
    subplot(2,1,1)
    e = plot(selLapTable.Var2, zeros(height(selLapTable), 1), '-', 'LineWidth', 5, 'Color', '#FF9D8C');
    xlabel('Time (ms)');
    xlim([selLapTable.Var2(1,1) selLapTable.Var2(end,end)])
    title('Click on the timeline to select a time');
    ylim([-0.5 0.5]);
    grid on;
    set(gcf, 'Position', [200,0,2560,1440]);
    set(gca, 'Units', 'pixels', 'Position', [100, 500, 1000, 500]);

    % Store selLapTable in the figure for access in callbacks
    figureHandle.UserData.selLapTable = selLapTable;
    figureHandle.UserData.selectedTime = NaN;  % Initialize

    % Set click callback
    set(figureHandle, 'WindowButtonDownFcn', @(src, ~) onClick(src));
end

function onClick(figHandle)
    % Retrieve table from figure's UserData
    selLapTable = figHandle.UserData.selLapTable;

    % Get clicked x-position
    cp = get(gca, 'CurrentPoint');
    clickTime = cp(1,1);

    % Find the closest point
    [~, idx] = min(abs(selLapTable.Var2 - clickTime));
    selectedTime = selLapTable.Var2(idx);

    % Store selected time
    figHandle.UserData.selectedTime = selectedTime;

    % --- CALL YOUR CALLBACK FUNCTION ---
    ThreeDPlot(selectedTime, selLapTable);
end

function ThreeDPlot(selectedTime, selLapTable)
    x = [240;240;240;0;0;0];
    y = [210;105;0;210;105;0];
    ThreeDTable = table(x,y);
    FRz = 0; FMz = 0; FLz = 0; BRz = 0; BMz = 0; BLz = 0;
    %Z = [FRz;FMz;FLz;BRz;BMz;BLz];
    b = 1;
    while b < height(selLapTable)
        timeOfReading = selLapTable{b,1};
        if timeOfReading == selectedTime
            FRz = -1*selLapTable{b,2};    %FR
            FMz = -1*selLapTable{b,3};    %FM
            FLz = -1*selLapTable{b,4};    %FL
            BRz = -1*selLapTable{b,5};    %BR
            BMz = -1*selLapTable{b,6};    %BM
            BLz = -1*selLapTable{b,7};    %BL
            z = [FRz;FMz;FLz;BRz;BMz;BLz];
            Z = array2table(z);
            ThreeDTable = [ThreeDTable Z];
            b = b + 1;
        else
            b = b + 1;
        end
    end
    x;
    y;
    z;
    subplot(2,1,2)
    plot3(x,y,z,'o','Color','#FF9D8C')
    xlabel('X');
    xlim([-10 250]);
    ylabel('Y');
    ylim([-50 250]);
    zlabel('Z');
    title('3D Plot of X, Y, Z');
    grid on;
    axis equal;
    view(3);
    %set(gca, 'Units', 'pixels', 'Position', [])
end
