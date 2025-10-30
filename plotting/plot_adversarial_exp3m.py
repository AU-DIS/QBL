import sys
import matplotlib.pyplot as plt
import numpy as np
from matplotlib import colors
import itertools
from colors import colormap

regrets = {}
styles = ['o', 'v', '^', '<', '>', 's', '8', 'p']


with open(sys.argv[1], 'r') as f:
    metadata = f.readline().replace('"', '').replace('\n', '').split(',')
    algorithms = metadata[5:]
    algorithms = [x for x in algorithms if x != '']
    for algorithm in algorithms:
        if algorithm != '':
            regrets[algorithm] = f.readline().replace('\n', '').split(',')

description = metadata[0]
k = int(metadata[1])
rounds = int(metadata[2])
gap = float(metadata[3])
K = int(metadata[4])



ys = []
algorithms = sorted(algorithms)

plt.figure(0, figsize=(5, 5))
for nr, algorithm in enumerate(algorithms):
    lines = np.array([float(x) for x in regrets[algorithm]])
    max_val = np.max(lines)
    lines = np.cumsum(lines)
    ys.append(lines)
    x = range(len(lines))
    y = lines
    plt.loglog(x, y, color=colormap[algorithm])

gap_xs = []
gap_ys = []
gap_iter = 0
while int(gap**gap_iter) <= int(rounds):
    gap_xs.append(int(gap**gap_iter))
    gap_iter += 1
print(gap_xs)
plt.vlines(gap_xs, ymin=np.min([np.min(obs) for obs in ys]), ymax=np.max([np.max(obs) for obs in ys]), linestyle=(0, (5, 1)), color="darkgrey")
plt.grid(True)
plt.legend(algorithms, fontsize=15)
plt.tight_layout()

if len(sys.argv) >= 3:
    plt.savefig(sys.argv[2])
else:
    plt.show()

