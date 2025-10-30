import sys
import matplotlib.pyplot as plt
import pandas as pd
from colors import colormap


styles = ['-', '--', '-.', ':', ',']
colours = ['b', 'g', 'r', 'c', 'm', 'y', 'k']

df = pd.read_csv(sys.argv[1])
algorithms = sorted(set(df["name"]))
print(algorithms)
ks = sorted(set(df['k']))
ks = [int(x) for x in ks]
plt.figure(0, figsize=(4, 4))

for nr, algo in enumerate(algorithms):
    rows = df.loc[df["name"] == algo]
    runtimes = [x for i, x in enumerate(rows["cpu_time"])]
    plt.loglog(ks, runtimes, color=colormap[algo])


plt.xlabel('k')
plt.ylabel('cputime')
plt.legend(algorithms, fontsize=15)
plt.grid(True)
if len(sys.argv) >= 3:
    plt.savefig(sys.argv[2])
else:
    plt.show()
