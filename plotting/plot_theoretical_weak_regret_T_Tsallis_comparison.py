import pandas as pd
import matplotlib.pyplot as plt
import sys

plt.rcParams["figure.figsize"] = [4, 4]
plt.rcParams["figure.autolayout"] = True

df_original = pd.read_csv(sys.argv[1], skiprows=1)
df_Approx = pd.read_csv(sys.argv[2], skiprows=1)

# plot lines
fig1 = plt.figure()
ax1 = fig1.add_subplot()

ax1.plot(df_original['T'], df_original['average_divided_by_promise'], marker='o', color='mediumblue')
ax1.plot(df_Approx['T'], df_Approx['average_divided_by_promise'], marker='o', color='mediumpurple')

ax1.set_xlabel('T')
ax1.set_xscale('log')
ax1.set_ylabel('weak regret / expected regret bound')
ax1.set_title(f"Tsallis (RV) regret plot for k=10")

plt.legend(["Tsallis (RV)", "Tsallis (RV approx)"])
plt.show()