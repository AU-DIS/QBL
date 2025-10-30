import pandas as pd
import matplotlib.pyplot as plt
import sys

plt.rcParams["figure.figsize"] = [7.50, 3.50]
plt.rcParams["figure.autolayout"] = True

df = pd.read_csv(sys.argv[1], skiprows=1)

# plot lines
fig1 = plt.figure()
ax1 = fig1.add_subplot()
ax1.errorbar(df.loc[:,'eta'], df.loc[:,'mean_regret_exp3_T_half'], df.loc[:,'std_regret_exp3_T_half'], label = "mean regret Exp3")
ax1.errorbar(df.loc[:,'eta'], df.loc[:,'mean_regret_exp3IX_T_half'], df.loc[:,'std_regret_exp3IX_T_half'], label = "mean regret Exp3IX")
ax1.set_xscale('log')
ax1.set_xlabel('ŋ')
ax1.set_ylabel('Regret T/2')

fig2 = plt.figure()
ax2 = fig2.add_subplot()
ax2.errorbar(df.loc[:,'eta'], df.loc[:,'mean_regret_exp3_T'], df.loc[:,'std_regret_exp3_T'], label = "mean regret Exp3")
ax2.errorbar(df.loc[:,'eta'], df.loc[:,'mean_regret_exp3IX_T'], df.loc[:,'std_regret_exp3IX_T'], label = "mean regret Exp3IX")
ax2.set_xscale('log')
ax2.set_xlabel('ŋ')
ax2.set_ylabel('Regret T')
plt.show()