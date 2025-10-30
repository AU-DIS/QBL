import matplotlib.pyplot as plt

with open('/tmp/helikoptere2', 'r') as f:
    text = f.readline()
    n = text.replace('[', '').replace(']', ''.replace('\n', '')).split(',')


n = [float(x) for x in n]

counters = {}

for x in n:
    if x not in counters:
        counters[x] = 1
    else:
        counters[x] += 1
x = []
y = []

for k, v in counters.items():
    x.append(v)
    y.append(k)

max_x = max(x)
x = [x/max_x for x in x]
print(max_x)
#print(counters)

plt.hist(x, bins=100)
plt.show()


