#!/usr/bin/python3
import csv
import matplotlib.pyplot as plt

userscount=[]
testduration=[]
requestscount=[]
rtt=[]
with open("values1.csv","r") as file:
	csvfile=csv.reader(file)
	for lines in csvfile:
		userscount.append(int(lines[0]))
		testduration.append(float(lines[2]))
		requestscount.append(int(lines[1]))

print("No.of users:",userscount)
print("testduration:",testduration)
print("Requests count:",requestscount)
for i in range(len(userscount)):
	rtt.append(requestscount[i]/testduration[i])
print("RTT values are:",rtt)
plt.plot(userscount,rtt)
plt.title("usercount vs through_put")
plt.xlabel("number of users")
plt.ylabel("Throughput")
plt.savefig("Throughput")
plt.show()
