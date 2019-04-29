from enum import Enum
from os import system
from threading import Thread
import numpy as np
import pygame, time
from pygame.locals import *
from welcome import getName

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
Name = getName()


def draw():
    disp.fill((0, 0, 0))
    pygame.draw.rect(disp, (0, 150, 255), (0, 0, W, W))
    pygame.draw.line(disp, (0, 0, 0), (W // 2, 0), (W // 2, W), 5)
    pygame.draw.line(disp, (0, 0, 0), (0, W // 2), (W, W // 2), 5)
    for i in range(6):
        for j in range(6):
            q = board[i, j]
            pygame.draw.circle(disp, cmap[q], (ax[i], ay[j]), r)

    (bestname, best), avg = getSummary()
    t1 = getFont(64).render("Best Score:", True, (255, 0, 0))
    t2 = getFont(54).render(bestname + " -> " + str(best), True, (255, 0, 0))
    w1, h1 = t1.get_size()
    w2, h2 = t2.get_size()
    disp.blit(t1, (W + (W_score - w1) // 2, sy[0] - (h1 + h2) // 2))
    disp.blit(t2, (W + (W_score - w2) // 2, sy[0] - (h1 + h2) // 2 + h1))

    t1 = getFont(64).render("Average:", True, (0, 255, 0))
    t2 = getFont(54).render(str(avg), True, (0, 255, 0))
    w1, h1 = t1.get_size()
    w2, h2 = t2.get_size()
    disp.blit(t1, (W + (W_score - w1) // 2, sy[1] - (h1 + h2) // 2))
    disp.blit(t2, (W + (W_score - w2) // 2, sy[1] - (h1 + h2) // 2 + h1))

    t1 = getFont(64).render("Your Score:", True, (0, 0, 255))
    t2 = getFont(54).render(str(score), True, (0, 0, 255))
    w1, h1 = t1.get_size()
    w2, h2 = t2.get_size()
    disp.blit(t1, (W + (W_score - w1) // 2, sy[2] - (h1 + h2) // 2))
    disp.blit(t2, (W + (W_score - w2) // 2, sy[2] - (h1 + h2) // 2 + h1))


def rotdraw():
    w, h = rimg.get_size()
    for i in range(2):
        for j in range(2):
            x1 = rx[i * 2]
            x2 = rx[i * 2 + 1]
            y = ry[j]
            disp.blit(limg, (x1 - w // 2, y - h // 2))
            disp.blit(rimg, (x2 - w // 2, y - h // 2))


def getcell(xm, ym):
    dx = (ax - xm) ** 2
    dy = (ay - ym) ** 2
    for i in range(6):
        for j in range(6):
            if dx[i] + dy[j] <= r ** 2 and board[i, j] == User.Null:
                return i, j
    return None


def getrot(xm, ym):
    w, h = rimg.get_size()
    for i in range(2):
        for j in range(2):
            x1 = rx[i * 2]
            x2 = rx[i * 2 + 1]
            y = ry[j]
            if y - h // 2 <= ym <= y + h // 2:
                if x1 - w // 2 <= xm <= x1 + w // 2:
                    return i, j, +1
                if x2 - w // 2 <= xm <= x2 + w // 2:
                    return i, j, -1
    return None


def solve():
    global board, points, last

    cp = np.zeros((6, 6), dtype=str)
    cp[board == User.Null] = '_'
    cp[board == User.PC] = 'X'
    cp[board == User.Human] = 'O'
    np.savetxt("log/in.txt", cp, fmt='%s', delimiter=' ')

    cmd = "./" + SOLVER + "< ./log/in.txt > ./log/out.txt"
    system(cmd)

    file = open("log/out.txt", "r")
    data = file.readlines()
    points = list(map(int, data[0].split()))
    for i in range(1, 7):
        cp = np.array(data[i].split(), dtype=str)
        board[i - 1, cp == '_'] = User.Null
        board[i - 1, cp == 'X'] = User.PC
        board[i - 1, cp == 'O'] = User.Human

    return


def wait():
    pygame.draw.circle(disp, (255, 0, 0), (W // 2, W // 2), 50)
    pygame.display.update()
    time.sleep(.5)


def getFont(size):
    return pygame.font.Font(None, size)


def getSummary():
    ind = np.argmax(scoreboard[:, 1])
    avg = np.mean(scoreboard[:, 1])
    return scoreboard[ind], round(avg, 2)


cmap = {User.Null: (0, 0, 200),
        User.PC: (255, 255, 255),
        User.Human: (0, 0, 0)}

W = 600
W_score = 300
r = 30
rimg = pygame.image.load("img/iright.png")
limg = pygame.image.load("img/ileft.png")
disp = pygame.display.set_mode((W + W_score, W))
font = pygame.font.Font(None, 128)
clock = pygame.time.Clock()

board = np.zeros((6, 6), dtype=User)
board[:][:] = User.Null
last = None
ax = np.linspace(0, W, 13, dtype=np.int32)[1:-1:2]
ay = np.linspace(0, W, 13, dtype=np.int32)[1:-1:2]
rx = np.linspace(0, W, 7, dtype=np.int32)[[1, 2, 4, 5]]
ry = np.linspace(0, W, 5, dtype=np.int32)[[1, 3]]
sy = np.linspace(0, W, 7, dtype=np.int32)[1:-1:2]

scoreboard = np.loadtxt("./log/score.txt",
                        dtype=object,
                        delimiter=",", )
scoreboard[:, 1] = np.int64(scoreboard[:, 1])
score = 0

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
                pl = i + j * 2
                q = board[i * 3:(i + 1) * 3, j * 3:(j + 1) * 3]
                q[:] = np.rot90(q, -k)

                now = Mode.Solve
                current = User.PC
                score += 1
                draw()

        if e.type == KEYDOWN:
            if e.key == K_z and last is not None:
                board[:] = last

            if e.key == K_r:
                board[:] = User.Null
                current = User.Human
                now = Mode.Select

    pygame.display.update()

    if current == User.PC and now == Mode.Solve:
        thr = Thread(target=solve)
        thr.start()
        wait()
        thr.join()

        if not points[1] > points[0]:
            score += 1

        current = User.Human
        now = Mode.Select
        draw()

    pygame.display.update()
    clock.tick(60)

if points[0] == points[1] and points[0] == 0 and points[2] == 0:
    newscore = np.array([[Name, score]], dtype=object)
    scoreboard = np.vstack((scoreboard,newscore))
    np.savetxt("./log/score.txt", scoreboard, delimiter=",")

text = "null"
if points[0] > points[1]:
    text = "PC wins"
elif points[1] > points[0]:
    text = "You win"
elif (points[0] == points[1] and points[0] != 0) or points[2] == 1:
    text = "draw"
else:
    text = "Press again"

box = font.render(text, True, (255, 128, 0))
wb, hb = box.get_size()
disp.blit(box, ((W - wb) // 2, (W - hb) // 2))
pygame.display.update()
print("End of Game, Press Quit Button!")

while True:
    for e in pygame.event.get():
        if e.type == QUIT:
            pygame.quit()
