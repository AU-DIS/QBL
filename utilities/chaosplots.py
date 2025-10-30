from random import random
import matplotlib.pyplot as plt
import functools
import math
import random
import time

from torch import randint

random.seed(time.time())


def tent_map(x, mu=2):
    if x < 1/2:
        return mu * x
    else:
        return mu * (1 - x)


def standard_map(theta, p, K=2):
    p = p + K * math.sin(theta)
    theta = theta + p
    return theta, p


arms = 64
k = 100
avg = 10000
func = tent_map
for i in range(10):
    xs = [x for x in range(k)]
    ys = [0 for x in range(k)]

    for j in range(avg):
        v = ((time.time_ns()/10000) % 0.8)
        #p = (time.time() % 1)
        for a in range(len(xs)):
            #v, p = func(v, p)
            if a % 10 == 0:
                v = ((time.time_ns()/10000) % 0.8)
            v = func(v)
            #print(v)
            #c = ((v+p) % (2 * math.pi)) % 1
            c = v
            # print(v)
            ys[a] += (math.floor(c * arms))
            #ys[a] += random.randint(0, arms)
    ys = [x/avg for x in ys]

    plt.figure(i, figsize=(15, 10))
    plt.plot(xs, ys, c='blue')
    plt.scatter(xs, ys, c='red')
    plt.ylim((0, arms))
    plt.savefig(f"/mnt/ssd/bandits/chaos{i}.png")
