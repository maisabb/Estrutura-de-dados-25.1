dados = readtable('resultados.csv');

dados_ordenados = sortrows(dados, 'numero_sorteado');

limite_y = max([dados_ordenados.comparacoes_abb; dados_ordenados.comparacoes_lista]);

figure;
plot(dados_ordenados.numero_sorteado, dados_ordenados.comparacoes_abb, ...
    'b-', 'LineWidth', 2); 
hold on;
plot(dados_ordenados.numero_sorteado, dados_ordenados.comparacoes_lista, ...
    'r-', 'LineWidth', 2); 
hold off;

xlabel('Número Sorteado');
ylabel('Número de Comparações');
title('Comparação: ABB vs Lista');
ylim([0 limite_y]);

legend('ABB', 'Lista', 'Location', 'northeast');

