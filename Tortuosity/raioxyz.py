import numpy as np
import random
import math

# Global variables
Epz, E1pz, Enz, Ez = 0.0, 0.0, 0.0, 0.0
Epx, E1px, Enx, Ex = 0.0, 0.0, 0.0, 0.0
Epy, E1py, Eny, Ey = 0.0, 0.0, 0.0, 0.0
Et, jx, jy, jz, jt = 0.0, 0.0, 0.0, 0.0, 0.0
sigx, sigy, sigz, sigo = 0.0, 0.0, 0.0, 1.0
x, y, z, cp, minnz, unitx, unity, unitz = 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0
xEx, yEy, zEz, unitxEx, unityEy, unitzEz = 0.0, 0.0, 0.0, 0.0, 0.0, 0.0
Hp, Hn, H1p, Qp, Qn, Q1p, xo, yo, zo = 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0
nx, ny, nz, sx, sy, sz, i, j, m1, m2, m3, g = 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0

def entradas():
    global x, y, z, Hp, Qp, H1p, Q1p, Hn, Qn, xo, yo, zo, minnz, nx, ny, nz
    print("raio3d1.py Aplica o metodo Monte Carlo para gerar canais tortuosos")
    print("Calcula o modulo do campo eletrico Et(x,y,z)=Ex(x,y,z)+Ey(x,y,z)+Ez(x,y,z)")
    print("em forma de um tripolo com cargas Qp, Qn, Q1p em Hp, Hn e H1p imagens")
    print("Calcula a orientacao do vetor densidade de correntes (j) versao beta mar 2000")
    print("Autor modulo 2D: MOACIR LACERDA; Inclusao Modulo 3D: EDUARDO AUGUSTO CURVO\n")

    x = float(input("Entre x(m): "))
    xo = x
    y = float(input("Entre y(m): "))
    yo = y
    z = float(input("Entre z(m): "))
    zo = z
    xEx, yEy, zEz = x, y, z

    cp = float(input("Entre cp(m): "))
    Hp = float(input("Entre Hp(m): "))
    Qp = float(input("Entre Qp(C): "))
    H1p = float(input("Entre H1p(m): "))
    Q1p = float(input("Entre Q1p(C): "))
    Hn = float(input("Entre Hn(m): "))
    Qn = float(input("Entre Qn(C): "))

    nx = int(input("Entre nx (largura do randomico sx 0-nx): "))
    ny = int(input("Entre ny (largura do randomico sy 0-ny): "))
    nz = int(input("Entre nz (largura do randomico sz 0-nz): "))
    minnz = float(input("Entre minnz - minimo valor percentual p/ sz=minnz+(0-nz): "))

    with open("raioen3d1.dat", "a") as coord2:
        coord2.write(f"{x} {y} {z} {Hp} {H1p} {Hn} {Qp} {Q1p} {Qn} {nx} {ny} {nz}\n")

def calcepz():
    global Epz, Epx, Epy, x, y, z, Hp, Qp
    Ka = -9e9
    Qnim = -Qp

    Epz = Ka * (Qp * (z - Hp)) / (math.pow(math.pow(Hp - z, 2) + math.pow(x, 2), 1.5)) + \
          Ka * (Qnim * (z + Hp)) / (math.pow(math.pow(Hp + z, 2) + math.pow(x, 2), 1.5))

    Epx = Ka * (Qp * x) / (math.pow(math.pow(Hp - z, 2) + math.pow(x, 2), 1.5)) + \
          Ka * (Qnim * x) / (math.pow(math.pow(Hp + z, 2) + math.pow(x, 2), 1.5))

    Epy = Ka * (Qp * y) / (math.pow(math.pow(Hp - z, 2) + math.pow(y, 2), 1.5)) + \
          Ka * (Qnim * y) / (math.pow(math.pow(Hp + z, 2) + math.pow(y, 2), 1.5))

def calce1pz():
    global E1pz, E1px, E1py, x, y, z, H1p, Q1p
    Ka = -9e9
    Q1nim = -Q1p

    E1pz = Ka * (Q1p * (z - H1p)) / (math.pow(math.pow(H1p - z, 2) + math.pow(x, 2), 1.5)) + \
           Ka * (Q1nim * (z + H1p)) / (math.pow(math.pow(H1p + z, 2) + math.pow(x, 2), 1.5))

    E1px = Ka * (Q1p * x) / (math.pow(math.pow(H1p - z, 2) + math.pow(x, 2), 1.5)) + \
           Ka * (Q1nim * x) / (math.pow(math.pow(H1p + z, 2) + math.pow(x, 2), 1.5))

    E1py = Ka * (Q1p * y) / (math.pow(math.pow(H1p - z, 2) + math.pow(y, 2), 1.5)) + \
           Ka * (Q1nim * y) / (math.pow(math.pow(H1p + z, 2) + math.pow(y, 2), 1.5))

def calcenz():
    global Enz, Enx, Eny, x, y, z, Hn, Qn
    Ka = -9e9
    Qpim = -Qn

    Enz = Ka * (Qn * (z - Hn)) / (math.pow(math.pow(Hn - z, 2) + math.pow(x, 2), 1.5)) + \
          Ka * (Qpim * (z + Hn)) / (math.pow(math.pow(Hn + z, 2) + math.pow(x, 2), 1.5))

    Enx = Ka * (Qn * x) / (math.pow(math.pow(Hn - z, 2) + math.pow(x, 2), 1.5)) + \
          Ka * (Qpim * x) / (math.pow(math.pow(Hn + z, 2) + math.pow(x, 2), 1.5))

    Eny = Ka * (Qn * y) / (math.pow(math.pow(Hn - z, 2) + math.pow(y, 2), 1.5)) + \
          Ka * (Qpim * y) / (math.pow(math.pow(Hn + z, 2) + math.pow(y, 2), 1.5))

def calcet():
    global Ez, Ex, Ey, Et
    Ez = Epz + E1pz + Enz
    Ex = Epx + E1px + Enx
    Ey = Epy + E1py + Eny
    Et = math.sqrt(Ex**2 + Ey**2 + Ez**2)

def calcjt():
    global sigx, sigy, sigz, sigo, jx, jy, jz, jt, unitx, unity, unitz, unitxEx, unityEy, unitzEz
    sigx = (sigo * sx) / 100
    sigy = (sigo * sy) / 100
    sigz = (sigo * (minnz + sz)) / 100

    jx = sigx * Ex
    jy = sigy * Ey
    jz = sigz * Ez
    jt = math.sqrt(jx**2 + jy**2 + jz**2)

    unitx = jx / jt
    unity = jy / jt
    unitz = jz / jt
    unitxEx = Ex / Et
    unityEy = Ey / Et
    unitzEz = Ez / Et

def saidas():
    print(f"\nO valor de Enz e': {Enz}")
    print(f"O valor de Enx e': {Enx}")
    print(f"O valor de Eny e': {Eny}")
    print(f"\nO valor de E1pz e': {E1pz}")
    print(f"O valor de E1px e': {E1px}")
    print(f"O valor de E1py e': {E1py}")
    print(f"\nO valor de Epz e': {Epz}")
    print(f"O valor de Epx e': {Epx}")
    print(f"O valor de Epy e': {Epy}")
    print(f"\nO valor de sx (numero randomico) e': {sx}")
    print(f"O valor de sy (numero randomico) e': {sy}")
    print(f"O valor de sz (numero randomico) e': {sz}")
    print(f"\nminnz - minimo valor percentual p/ sz=minnz+(0-nz): {minnz}")
    print(f"\nO valor de Ez (total) e': {Ez}")
    print(f"O valor de Ex (total) e': {Ex}")
    print(f"O valor de Ey (total) e': {Ey}")
    print(f"O valor de Et (total) e': {Et}")
    print(f"O valor de jz e': {jz}")
    print(f"O valor de jx e': {jx}")
    print(f"O valor de jy e': {jy}")
    print(f"O valor de jt (total) e': {jt}")
    print(f"O valor de unitx e': {unitx}")
    print(f"O valor de unity e': {unity}")
    print(f"O valor de unitz (total) e': {unitz}")

def main():
    global x, y, z, xEx, yEy, zEz, sx, sy, sz, i, j, m1, m2, m3, g
    entradas()

    for g in range(30):
        with open("raio3d1.dat", "a") as coord1:
            print(f"\nINICIANDO O PROGRAMA, rodada {g + 1}\n")
            x, y, z = xo, yo, zo
            xEx, yEy, zEz = xo, yo, zo
            i = 0

            while z > 0:
                calcepz()
                calce1pz()
                calcenz()
                calcet()

                m1, m2, m3 = 0, 0, 0
                while m1 < 6000:
                    sx = random.randint(0, nx - 1)
                    m1 += 1

                while m2 < 6000:
                    sy = random.randint(0, ny - 1)
                    m2 += 1

                while m3 < 6000:
                    sz = random.randint(0, nz - 1)
                    m3 += 1

                calcjt()

                x += cp * unitx
                y += cp * unity
                z += cp * unitz
                xEx += cp * unitxEx
                yEy += cp * unityEy
                zEz += cp * unitzEz

                coord1.write(f"{x} {y} {z} {xEx} {yEy} {zEz} {sx} {sy} {sz} {unitz} {unitzEz} {Ex} {Ey} {Ez}\n")
                i += 1

        saidas()

if __name__ == "__main__":
    main()