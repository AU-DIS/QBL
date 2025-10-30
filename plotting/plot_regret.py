import sys
import matplotlib.pyplot as plt
import numpy as np
import numpy as np
from sklearn.linear_model import LinearRegression
from matplotlib import colors
model = LinearRegression()
import itertools

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


algorithms = sorted(algorithms)


plt.figure(0, figsize=(5, 5))
for nr, algorithm in enumerate(algorithms):
    lines = np.array([float(x) for x in regrets[algorithm]])
    max_val = np.max(lines)
    lines = np.cumsum(lines)
    x = range(len(lines))
    y = lines
    plt.loglog(x, y, marker=styles[nr % len(styles)], markersize=0.5)


gap_xs = []
gap_iter = 0
while int(gap**gap_iter) <= int(rounds):
    gap_xs.append(int(gap**gap_iter))
    gap_iter += 1
plt.vlines(gap_xs, ymin=0, ymax=int(rounds), linestyle=(0, (5, 1)), color="darkgrey")

plt.xlabel('Rounds')
plt.ylabel('Cumulative regret')

plt.title(f"{description} k = {k}")
plt.ylim(0, int(rounds))

plt.legend(algorithms, fontsize=15)

if len(sys.argv) >= 3:
    plt.savefig(sys.argv[2])
else:
    plt.show()




