clearvars
clear;
close all
clc;
warning off;


muestras = 5000;
t = zeros(1, muestras); 
pwm = zeros(1, muestras);
vel = zeros(1, muestras);


% Configuración de comunicación con Arduino serial
arduinoSerial = serialport('COM5', 1000000); 

% Iniciar la comunicación con Arduino
fopen(arduinoSerial);
tic
% Leer y escribir datos en el bucle
for k = 1:muestras
    t(k)=toc;
    pwm(k) = .5*(t(k)>1); 
    %pwm(k) = 5*sin(2*pi*t(k)/7.0); 

    %write(arduinoSerial,num2str(pwm(k)),"char")
    fprintf(arduinoSerial, '%f\n', pwm(k)); % Enviar datos

    % Pausa para dar tiempo al Arduino a procesar y enviar la respuesta
    pause(0.001); % Ajusta este tiempo si es necesario

    if arduinoSerial.NumBytesAvailable > 0
        vel(k) = fscanf(arduinoSerial, '%f\n'); % Leer datos
        %vel(k) = str2double(readline(arduinoSerial));
    else
        vel(k) = NaN; % Si no hay datos disponibles, asignar NaN
    end
end

samp_time = mean(diff(t));
disp(samp_time)

samp_frecc = 1/samp_time;
disp(samp_frecc)

qw = 55*(1/22)*(1/45);
disp(qw)
% Graficar resultados
plot(t, vel, 'b', 'LineWidth', .5);
hold on;
plot(t, pwm, 'r', 'LineWidth', .5);
legend('Velocidad', 'Señal');
xlabel('Tiempo');

% Cerrar y eliminar la conexión serial
fclose(arduinoSerial);
delete(arduinoSerial);
clear arduinoSerial;
