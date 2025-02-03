function [xyz, db] = pmatch(P1,PR1,P2,PR2,INFO)
% Esse algoritmo calcula as possíveis coordenadas x,y,z considerando que os dois pontos de
entrada P1 e P2 são correspondentes. Também retorna o valor db, que calcula a correspondência
(quanto menor o db, maior a chance de P1 e P2 serem o mesmo ponto no espaço).
% xpr e ypr correspondem à localização do ponto de referência da imagem de que o ponto de
match foi retirado. xci e yci são a localização do centro da imagem.
format compact
xpr1 = PR1(1) ; ypr1 = PR1(2);
xpr2 = PR2(1) ; ypr2 = PR2(2);
xci1 = PR1(3) ; yci1 = PR1(4);
xci2 = PR2(3) ; yci2 = PR2(4);
% As coordenadas de P1 e P2 são traduzidas para ângulos de inclinação com a horizontal
(ANG(1)) e ângulo azimutal (ANG(2)) em radiano.
ANG1d(1) = INFO(1) - 180 * atan((P1(2) - yci1) * INFO(7)/INFO(6))/pi;
ANG1d(2) = INFO(2) - 180 * (atan((P1(1) - xci1) * INFO(7)/INFO(6)) - atan((xpr1 - xci1) *
INFO(7)/INFO(6)))/pi;
ANG2d(1) = INFO(8) - 180 * atan((P2(2) - yci2) * INFO(14)/INFO(13))/pi;
ANG2d(2) = INFO(9) - 180 * (atan((P2(1)-xci2) * INFO(14)/INFO(13)) - atan((xpr2 - xci2) *
INFO(14)/INFO(13)))/pi;
ANG1 = ANG1d * pi/180;
ANG2 = ANG2d * pi/180;
% encontrando a projeção (xp,yp) no plano horizontal para esse par.
xp = -(INFO(11) - INFO(4) - INFO(10) * tan(ANG2(2) - INFO(3) * tan(ANG1(2)))) /
(tan(ANG2(2)) - tan(ANG1(2)));
yp = (xp - INFO(3)) * tan(ANG1(2)) + INFO(4);
% calculando as estimativas de altura para o suposto ponto 3D (x,y,z)
d1 = sqrt((INFO(3) - xp)∧2 + (INF O(4) − yp) ∧ 2);
d2 = sqrt((INFO(10) - xp)∧2 + (INF O(11) − yp) ∧ 2);
zp1 = d1 * tan(ANG1(1)) + INFO(5);
zp2 = d2 * tan(ANG2(1)) + INFO(13);
109
% calculando db de acordo com a diferença entre os valores de zp1 e zp2
db = atan(abs(zp1-zp2)/min(d1,d2));
% retornando a altura estimada como a média dos valores de altura encontrados para cada ponto
z = zp2 + (d2/cos(ANG2(1))) * (zp1-zp2)/(d2/cos(ANG2(1)) + d1/cos(ANG1(1)));
xyz = [xp,yp,z];
end
