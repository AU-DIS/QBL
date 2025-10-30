import pandas as pd
import matplotlib.pyplot as plt
import sys

plt.rcParams["figure.figsize"] = [4, 4]
plt.rcParams["figure.autolayout"] = True

df_original = pd.read_csv(sys.argv[1], skiprows=1)
df_1 = pd.read_csv(sys.argv[2], skiprows=1)
df_2 = pd.read_csv(sys.argv[3], skiprows=1)
df_3 = pd.read_csv(sys.argv[4], skiprows=1)
df_4 = pd.read_csv(sys.argv[5], skiprows=1)
df_5 = pd.read_csv(sys.argv[6], skiprows=1)

# plot lines
fig1 = plt.figure()
ax1 = fig1.add_subplot()

ax1.plot(df_original['T'], df_original['average_divided_by_promise'], marker='o', color='magenta')
ax1.plot(df_1['T'], df_1['average_divided_by_promise'], marker='o', color='#007')
ax1.plot(df_2['T'], df_2['average_divided_by_promise'], marker='o', color='#229')
ax1.plot(df_3['T'], df_3['average_divided_by_promise'], marker='o', color='#44B')
ax1.plot(df_4['T'], df_4['average_divided_by_promise'], marker='o', color='#66D')
ax1.plot(df_5['T'], df_5['average_divided_by_promise'], marker='o', color='#88F')

ax1.set_xlabel('T')
ax1.set_xscale('log')
ax1.set_ylabel('weak regret / expected regret bound')
ax1.set_title(f"LAD regret plot for k=10")

plt.legend(["Exp3 Base", "LAD (θ=0.6)", "LAD (θ=0.7)", "LAD (θ=0.8)", "LAD (θ=0.9)", "LAD (θ=1.0)"])
plt.show()