from enum import Enum
from os import system
import numpy as np
import pygame, time
from pygame.locals import *
pygame.init()

class Mode(Enum):
    Select = 1
    Rotate = 2
    Solve = 3

class User(Enum):
    Null = 0
    Human = 1
    PC = 2

def draw():
    disp.fill((0,150,255))
    pygame.draw.line(disp, (0,0,0), (W//2,0), (W//2,W), 5)
    pygame.draw.line(disp, (0,0,0), (0,W//2), (W,W//2), 5)
    for i in range(6):
        for j in range(6):
            q = board[i,j]
            pygame.draw.circle(disp, cmap[q], (ax[i],ay[j]), r)

def rotdraw():
    w, h = rimg.get_size()
    for i in range(2):
        for j in range(2):
            x1 = rx[i*2]
            x2 = rx[i*2+1]
            y = ry[j]
            disp.blit(limg, (x1-w//2,y-h//2))
            disp.blit(rimg, (x2-w//2,y-h//2))

def getcell(xm, ym):
    dx = (ax - xm)**2
    dy = (ay - ym)**2
    for i in range(6):
        for j in range(6):
            if dx[i] + dy[j] <= r**2 and board[i,j] == User.Null:
                return i,j
    return None

def getrot(xm, ym):
    w, h = rimg.get_size()
    for i in range(2):
        for j in range(2):
            x1 = rx[i*2]
            x2 = rx[i*2+1]
            y = ry[j]
            if y-h//2 <= ym <= y+h//2:
                if x1-w//2 <= xm <= x1+w//2:
                    return i,j,+1
                if x2-w//2 <= xm <= x2+w//2:
                    return i,j,-1
    return None

def solve():
    global board, points, last

    cp = np.zeros((6, 6), dtype=str)
    cp[board==User.Null] = '_'
    cp[board==User.PC] = 'X'
    cp[board==User.Human] = 'O'
    np.savetxt("in.txt", cp, fmt = '%s', delimiter=' ')
    file = open("in.txt", "a")
    file.write(" ".join(map(str, last)))
    file.close()

    system("./solver.o")
    cp = np.loadtxt("out.txt", dtype=str, delimiter=" ")

    if len(cp) <= 2:
        points = list(map(int, cp[0:2]))
        print(points)

    else:
        cp = cp[:6, :6]
        board[cp=='_'] = User.Null
        board[cp=='X'] = User.PC
        board[cp=='O'] = User.Human

    return

def wait():
    pygame.draw.circle(disp, (255,0,0), (W//2, W//2), 100)
    pygame.display.update()
    time.sleep(.5)

cmap = {User.Null : (0,0,200),
        User.PC   : (255,255,255),
        User.Human: (0,0,0)}

W = 600
r = 30
rimg = pygame.image.load("iright.png")
limg = pygame.image.load("ileft.png")
disp = pygame.display.set_mode((W, W))
clock = pygame.time.Clock()

board = np.zeros((6,6), dtype=User)
board[:][:] = User.Null
ax = np.linspace(0, W, 13, dtype=np.int32)[1:-1:2]
ay = np.linspace(0, W, 13, dtype=np.int32)[1:-1:2]
rx = np.linspace(0, W, 7, dtype=np.int32)[[1,2,4,5]]
ry = np.linspace(0, W, 5, dtype=np.int32)[[1,3]]


#board[:4, 0] = User.Human

draw()
pygame.display.update()

now = Mode.Select
current = User.Human
last = [-1, -1, -1, -1]
points = [-1, -1]
running = True

while running:
    if sum(points) >= 0:
        if points[0] == points[1]:
            print("draw")
        elif points[0] > points[1]:
            print("PC wins")
        else:
            print("You win")
        running = False
        break

    for e in pygame.event.get():
        if e.type == QUIT:
            running = False

        if e.type == MOUSEMOTION and now == Mode.Select:
            xm, ym = e.pos
            ind = getcell(xm, ym)
            if ind is not None:
                i, j = ind
                pygame.draw.circle(disp, cmap[current], (ax[i], ay[j]), r)
            else:
                draw()

        if e.type == MOUSEBUTTONDOWN and now == Mode.Select:
            xm, ym = e.pos
            ind = getcell(xm, ym)
            if ind is not None:
                i, j = ind
                board[i, j] = User.Human
                last[0] = i; last[1] = j
                now = Mode.Rotate
                rotdraw()
                break

        if e.type == MOUSEBUTTONDOWN and now == Mode.Rotate:
            xm, ym = e.pos
            ind = getrot(xm, ym)
            if ind is not None:
                i, j, k = ind
                pl = i + j*2
                last[2] = pl; last[3] = k
                now = Mode.Solve
                current = User.PC
                draw()

    if current == User.PC and now == Mode.Solve:
        solve()
        current = User.Human
        now = Mode.Select
        draw()

    #draw()
    pygame.display.update()

    clock.tick(60)

print("End of Game, Press Quit Button!")
while True:
    for e in pygame.event.get():
        if e.type == QUIT:
            pygame.quit()
