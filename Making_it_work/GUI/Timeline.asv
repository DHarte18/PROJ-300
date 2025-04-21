function Timeline(selLapTable)
% setupTimelineClickPlot - Interactive timeline that triggers a callback
%                          on each user click with the selected time.
%
% INPUT:
%   selLapTable - table with 'Var2' column containing times in milliseconds

    % Create figure and plot
    figureHandle = figure;
    plot(selLapTable.Var2, zeros(height(selLapTable), 1), 'o', 'MarkerSize', 8);
    xlabel('Time (ms)');
    ylabel('Event');
    title('Click on the timeline to select a time');
    ylim([-1 1]);
    grid on;

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
    ThreeDPlot(selectedTime);
end

function ThreeDPlot(selectedTime, selLapTable)
    
end
