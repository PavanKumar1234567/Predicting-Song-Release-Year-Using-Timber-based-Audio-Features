#!/usr/bin/python3
import csv
import matplotlib.pyplot as plt

userscount=[]
responsetime=[]
requestscount=[]
rtt=[]
with open("values.csv","r") as file:
	csvfile=csv.reader(file)
	for lines in csvfile:
		userscount.append(int(lines[0]))
		responsetime.append(float(lines[2]))
		requestscount.append(int(lines[1]))

print("No.of users:",userscount)
print("Response time:",responsetime)
print("Requests count:",requestscount)
for i in range(len(userscount)):
	rtt.append(responsetime[i]/requestscount[i])
print("RTT values are:",rtt)
plt.plot(userscount,rtt)
plt.title("usercount vs responsetime")
plt.xlabel("number of users")
plt.ylabel("average response time")
plt.savefig("responsetime")
plt.show()
