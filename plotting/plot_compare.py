import sys
import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
from matplotlib import colors
import itertools
from colors import colormap

regrets = {}
styles = ['o', 'v', '^', '<', '>', 's', '8', 'p']


with open(sys.argv[1], 'r') as f:
    metadata = f.readline().replace('"', '').replace('\n', '').split(',')
    algorithms = metadata[4:]
    algorithms = [x for x in algorithms if x != '']
    for algorithm in algorithms:
        if algorithm != '':
            regrets[algorithm] = f.readline().replace('\n', '').split(',')

description = metadata[0]
k = metadata[1]
rounds = metadata[2]
gap = float(metadata[3])


ys = []
algorithms = sorted(algorithms)

plt.figure(0, figsize=(4, 4))
reducer = 1000
d = {}
x = []
for nr, algorithm in enumerate(algorithms):
    lines = np.array([float(x) for x in regrets[algorithm]])

    max_val = np.max(lines)
    lines = np.cumsum(lines)
    print(algorithm)
    print(lines)
    ys.append(lines)
    x = range(len(lines))
    y = lines
    d[algorithm] = y[::reducer]
    plt.loglog(x, y, color=colormap[algorithm])
    #plt.plot(x, y, color=colormap[algorithm])
d["step"] = x[::reducer]

gap_xs = []
gap_ys = []
gap_iter = 0

df = pd.DataFrame(d)
df.to_csv(sys.argv[1]+"reduced.txt", sep=" ", index=False)
while int(gap**gap_iter) <= int(rounds):
    gap_xs.append(int(gap**gap_iter))
    gap_iter += 1
plt.vlines(gap_xs, ymin=np.min([np.min(obs) for obs in ys]), ymax=np.max(
    [np.max(obs) for obs in ys]), linestyle='dotted', color='silver', alpha=0.4)

# plt.grid(True)
plt.legend(algorithms, fontsize=15)
plt.tight_layout()

if len(sys.argv) >= 3:
    plt.savefig(sys.argv[2])
else:
    plt.show()
