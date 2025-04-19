function [selectedTime] = Timeline (freshData)
% Declare the global variable at the top level
global selectedTime;

% Plot the timeline
figure;
plot(freshData.Var2, zeros(height(freshData), 1), '-', 'LineWidth', 5, 'Color', "#FF9D8C");
xlim([freshData.Var2(1,1) freshData.Var2(end,end)])
xlabel('Time (ms)');
title('Click on the timeline to get value');
ylim([-0.5 0.5]);
grid on;
set(gcf, 'Position', [2440,40,1000,100]);

% Set up the click callback
set(gcf, 'WindowButtonDownFcn', @(~, ~) onClick(freshData));

function onClick(freshData)
    % Declare the global variable inside the function
    %global selectedTime;

    % Get click location
    cp = get(gca, 'CurrentPoint');
    clickTime = cp(1,1);  % X-coordinate

    % Find the closest time
    [~, idx] = min(abs(freshData.Var2 - clickTime));

    % Assign to global variable
    selectedTime = freshData.Var2(idx);
    %selectedValue = freshData.Value(idx);

    % Display result
    disp(['You clicked near time (ms): ', num2str(selectedTime)]);
    %disp(['Corresponding value: ', num2str(selectedValue)]);
    selectedTime
end
end