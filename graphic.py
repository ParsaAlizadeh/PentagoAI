from enum import Enum
import numpy as np
import pygame
from pygame.locals import *
pygame.init()

class Mode(Enum):
    Select = 1
    Rotate = 2
    Solve = 3

class User(Enum):
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

def getcell(xm, ym):
    dx = (ax - xm)**2
    dy = (ay - ym)**2
    for i in range(6):
        for j in range(6):
            if dx[i] + dy[j] <= r**2:
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
                    return i,j,-1
                if x2-w//2 <= xm <= x2+w//2:
                    return i,j,1
    return None

cmap = ((0,0,200),
        (255,255,255),
        (0,0,0))
W = 600
r = 30
rimg = pygame.image.load("iright.png")
limg = pygame.image.load("ileft.png")
disp = pygame.display.set_mode((W, W))
clock = pygame.time.Clock()

board = np.zeros((6,6), dtype=np.int16)
ax = np.linspace(0, W, 13, dtype=np.int32)[1:-1:2]
ay = np.linspace(0, W, 13, dtype=np.int32)[1:-1:2]
rx = np.linspace(0, W, 7, dtype=np.int32)[[1,2,4,5]]
ry = np.linspace(0, W, 5, dtype=np.int32)[[1,3]]

draw()
pygame.display.update()

now = Mode.Select
current = User.Human
running = True

while running:
    pass
