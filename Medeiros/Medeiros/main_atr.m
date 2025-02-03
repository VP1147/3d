function [xyz, db] = main_atr(ENTR,PRR,ENTA,PRA,INFO)
nR = size(ENTR);
nA = size(ENTA);
% calcular pontos e produzir visualização da matriz db.
for i = 1:nR(1)
for j = 1:nA(1)
[xyz, db(i,j)] = pmatch(ENTR(i,:),PRR,ENTA(j,:),PRA,INFO);
x(i,j) = xyz(1);
y(i,j) = xyz(2);
z(i,j) = xyz(3);
end
end
figure
mesh(db.∧(1/4))
% escolher os pontos para construção do raio.
kmin=1;
for j = 1:nA(1)
dbarrayj = db(:,j).∧(1/4);
[min_db,k] = min(dbarrayj);
% k(i) = no do ponto da CA que corresponde ao ponto i da CR
if(k < kmin-1)
404 % mandar erro. Significa que os pontos estão "voltando".
end
if k == kmin-1
k=kmin;
end
% atualizat kmin para que os pontos estejam em ordem descendente
if k < nA
if k > kmin
110
kmin = k;
end
end
% caso o ponto não encontre correspondência:
if k == 1
k=0;
end
if k == nR(1)
k=0;
end
K(j) = k;
j = j+1;
end
p=1;
for j = 1:length(K)
if(K(j) > 0)
xyzt(p,1) = x(K(j),j);
xyzt(p,2) = y(K(j),j);
xyzt(p,3) = z(K(j),j);
p = p+1;
end
end
% plot
res(:,1) = xyzt(:,1)
res(:,2) = xyzt(:,2)
res(:,3) = xyzt(:,3)
% plot tridimensional do raio
figure
plot3(xyzt(:,1),xyzt(:,2),xyzt(:,3));
xlabel(’eixo x (m)’);
ylabel(’eixo y (m)’);
zlabel(’eixo z (m)’);
title(’Representação tridimensional dos pontos adquiridos’);
grid;
axis equal;
% plot tridimensional do raio da visão da câmera auxiliar
figure
plot3(xyzt(:,1),xyzt(:,2),xyzt(:,3));
xlabel(’eixo x (m)’);
ylabel(’eixo y (m)’);
zlabel(’eixo z (m)’);
111
title(’Visão da câmera rápida’);
grid;
axis equal;
view(INFO(2) - 90, -INFO(1));
% plot tridimensional do raio da visão da câmera rápida
figure
plot3(xyzt(:,1),xyzt(:,2),xyzt(:,3));
xlabel(’eixo x (m)’);
ylabel(’eixo y (m)’);
zlabel(’eixo z (m)’);
title(’Visão da câmera auxiliar’);
grid;
axis equal;
view(INFO(9) - 90, -INFO(8));
end
