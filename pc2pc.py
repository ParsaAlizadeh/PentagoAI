from enum import Enum
from os import system
from threading import Thread
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

SOLVER = "solvers/hashsolver.o"

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
    np.savetxt("log/in.txt", cp, fmt = '%s', delimiter=' ')

    cmd = "./" + SOLVER + "< ./log/in.txt > ./log/out.txt"
    system(cmd)

    file = open("log/out.txt", "r")
    data = file.readlines()
    points = list(map(int, data[0].split()))
    for i in range(1, 7):
        cp = np.array(data[i].split(), dtype=str)
        board[i-1, cp=='_'] = User.Null
        board[i-1, cp=='X'] = User.PC
        board[i-1, cp=='O'] = User.Human

    return

def wait():
    pygame.draw.circle(disp, (255,0,0), (W//2, W//2), 50)
    pygame.display.update()
    time.sleep(.5)

cmap = {User.Null : (0,0,200),
        User.PC   : (255,255,255),
        User.Human: (0,0,0)}

W = 600
r = 30
rimg = pygame.image.load("img/iright.png")
limg = pygame.image.load("img/ileft.png")
disp = pygame.display.set_mode((W, W))
font = pygame.font.Font(None, 128)
clock = pygame.time.Clock()

board = np.zeros((6,6), dtype=User)
board[:][:] = User.Null
last = None
ax = np.linspace(0, W, 13, dtype=np.int32)[1:-1:2]
ay = np.linspace(0, W, 13, dtype=np.int32)[1:-1:2]
rx = np.linspace(0, W, 7, dtype=np.int32)[[1,2,4,5]]
ry = np.linspace(0, W, 5, dtype=np.int32)[[1,3]]

draw()
pygame.display.update()

now = Mode.Select
current = User.Human
points = [0, 0, 0]
running = True

while running:
    if sum(points) > 0:
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
                last = board.copy()
                i, j = ind
                board[i, j] = User.Human
                now = Mode.Rotate
                rotdraw()
                break

        if e.type == MOUSEBUTTONDOWN and now == Mode.Rotate:
            xm, ym = e.pos
            ind = getrot(xm, ym)
            if ind is not None:
                i, j, k = ind
                pl = i + j*2
                q = board[i*3:(i+1)*3, j*3:(j+1)*3]
                q[:] = np.rot90(q, -k)
                now = Mode.Solve
                current = User.PC
                draw()

        if e.type == KEYDOWN:
            if e.key == K_z and last is not None:
                board[:] = last

    pygame.display.update()

    if current == User.PC and now == Mode.Solve:
        thr = Thread(target=solve)
        thr.start()
        wait()
        thr.join()
        current = User.Human
        now = Mode.Select
        draw()

    pygame.display.update()

    clock.tick(60)

text = 'null'
print(points)
if (points[0] == points[1] and points[0] != 0) or points[2] == 1:
    text = "draw"
elif points[0] > points[1]:
    text = "PC wins"
elif points[1] > points[0]:
    text = "You win"
else:
    text = "Press again"

box = font.render(text, True, (255,128,0))
wb, hb = box.get_size()
disp.blit(box, ((W-wb)//2, (W-hb)//2))
pygame.display.update()
print("End of Game, Press Quit Button!")

while True:
    for e in pygame.event.get():
        if e.type == QUIT:
            pygame.quit()
