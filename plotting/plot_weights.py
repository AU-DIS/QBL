import sys
import matplotlib.pyplot as plt
import numpy as np
from matplotlib import colors
import itertools

regrets = {}
styles = ['darkorange', 'khaki', 'darkolivegreen', 'firebrick',
          'springgreen', 'turquoise', 'darkorchid', 'royalblue', 'steelblue' ]
import numpy as np


plt.figure(0, figsize=(15, 7))
with open(sys.argv[1], 'r') as f:
    blank_line_lol = f.readline()
    lines = f.readlines()
    lines = [
        [float(y) for y in x.replace('\n', '').split(',')]
        for x in lines
    ]


max_element = np.max([np.max(x) for x in lines])

index = np.arange(len(lines[0])) + 0.3
for i, line in enumerate(lines[::-1]):
    plt.bar(index, [np.log(x) for x in line], width=1)
   # plt.plot(index, line, color=styles[i % len(styles)])


colormap = {
    "Exp3": "blue",
    "Exp3.1": "green",
    "UCB1 (Exp3)": "orange",
    "FPL": "royalblue",
    "UCB (FPL)": "blueviolet",
    "Tsallis-INF (IW)": "blue",
    "Tsallis-INF (RV)": "green",
    "Uniform": "red"
}

plt.yscale('log')

plt.xticks(np.arange(0, len(lines[0]), step=1))
plt.grid()
plt.legend([f"{i}th interval" for i in range(len(lines))][::-1], fancybox=True, loc='upper left', bbox_to_anchor=(1, 1))
plt.tight_layout()
if len(sys.argv) >= 3:
    plt.savefig(sys.argv[2])
else:
    plt.show()


