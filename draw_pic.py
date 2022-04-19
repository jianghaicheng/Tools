import matplotlib.pyplot as plt
from scipy import interpolate
import numpy as np

X,Y = [], []
file = '/localdata/cn-customer-engineering/haichengj/auto_partition/public_examples/applications/popart/bert/timer_logs/getLeafControlProgramSize.txt@tid:22527'
with open(file,'r') as f:
    lines = f.readlines()
    for line in lines:
        value = float(line.split('@')[0].split("totalTime:")[1].split('s')[0])
        Y.append(value)
X=[x for x in range(0,len(Y))]
print(X)
print(Y)

plt.plot(X,Y,X,Y)
plt.legend(['first','second'])
plt.xlabel('NamedOp')
plt.ylabel('Time(s)')
plt.title('time cost per Op')  
# save
plt.savefig('out.jpg')
plt.show()
