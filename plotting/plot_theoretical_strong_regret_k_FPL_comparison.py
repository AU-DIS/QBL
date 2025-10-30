import pandas as pd
import matplotlib.pyplot as plt
import sys

plt.rcParams["figure.figsize"] = [4, 4]
plt.rcParams["figure.autolayout"] = True

df_original = pd.read_csv(sys.argv[1], skiprows=1)
df_QBL = pd.read_csv(sys.argv[2], skiprows=1)

# plot lines
fig1 = plt.figure()
ax1 = fig1.add_subplot()

ax1.plot(df_original['k'], df_original['average_divided_by_promise'], marker='o', color='green')
ax1.plot(df_QBL['k'], df_QBL['average_divided_by_promise'], marker='o', color='darkorange')

ax1.set_xlabel('k')
ax1.set_xscale('log')
ax1.set_ylabel('strong regret / regret upper bound')
ax1.set_title(f"FPL and QBL regret plot for T=10^6")

plt.legend(["FPL", "QBL"])
plt.show()