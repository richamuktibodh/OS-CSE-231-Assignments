import matplotlib.pyplot as plt
import numpy as np

RR = []
FIFO = []
OTHER = []
PRIOS = []  # x values

f = open("vals.txt", "r")
for row in f:
    row = row.split()
    if (row[0] == "RR"):
        num = float(row[1]+ "."+ row[2])
        RR.append(num)
    elif (row[0] == "FIFO"):
        num = float(row[1]+ "."+ row[2])
        FIFO.append(num)
    elif (row[0] == "OTHER"):
        num = float(row[1]+ "."+ row[2])
        OTHER.append(num)
    else:
        PRIOS.append(str(row[1]) + "(" + str(row[0] + ")") )


X_axis = np.arange(len(PRIOS))
  
plt.bar(X_axis - 0.1, OTHER, 0.4, label = 'OTHER')
plt.bar(X_axis + 0.1, FIFO, 0.4, label = 'FIFO')
plt.bar(X_axis + 0.3, RR, 0.4, label = 'RR')

plt.xticks(X_axis, PRIOS)
plt.xlabel("Priority values")
plt.ylabel("Times")
plt.title("Priority values v/s time")
plt.legend()
plt.show()


