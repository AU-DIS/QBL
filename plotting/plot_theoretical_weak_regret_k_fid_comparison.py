import pandas as pd
import matplotlib.pyplot as plt
import sys

plt.rcParams["figure.figsize"] = [4, 4]
plt.rcParams["figure.autolayout"] = True

df_original = pd.read_csv(sys.argv[1], skiprows=1)
df_1000 = pd.read_csv(sys.argv[2], skiprows=1)
df_2000 = pd.read_csv(sys.argv[3], skiprows=1)
df_3000 = pd.read_csv(sys.argv[4], skiprows=1)
df_4000 = pd.read_csv(sys.argv[5], skiprows=1)
df_5000 = pd.read_csv(sys.argv[6], skiprows=1)

# plot lines
fig1 = plt.figure()
ax1 = fig1.add_subplot()

ax1.plot(df_original['k'], df_original['average_divided_by_promise'], marker='o', color='magenta')
ax1.plot(df_1000['k'], df_1000['average_divided_by_promise'], marker='o', color='#070')
ax1.plot(df_2000['k'], df_2000['average_divided_by_promise'], marker='o', color='#292')
ax1.plot(df_3000['k'], df_3000['average_divided_by_promise'], marker='o', color='#4B4')
ax1.plot(df_4000['k'], df_4000['average_divided_by_promise'], marker='o', color='#6D6')
ax1.plot(df_5000['k'], df_5000['average_divided_by_promise'], marker='o', color='#8F8')

ax1.set_xlabel('k')
ax1.set_xscale('log')
ax1.set_ylabel('weak regret / expected regret bound')
ax1.set_title(f"FID regret plot for T=10^4")

plt.legend(["Exp3 Base", "FID (λ=1000)", "FID (λ=2000)", "FID (λ=3000)", "FID (λ=4000)", "FID (λ=5000)"])
plt.show()