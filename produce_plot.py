# read_records.py
import sys
import math 
import matplotlib.pyplot as plt
import numpy as np 

def gcd(a, b):
    while b:
        a, b = b, a % b
    return a

def read_records(filename):
    records = {}
    with open(filename, 'r') as infile:
        for line in infile:
            line = line.strip()
            if line:
                modulus_str, values_str = line.split(':')
                modulus = int(modulus_str)
                value = int(values_str)
                records[modulus] = value
    return records

filename = 'records.txt'
records = read_records(filename)

# Python3 program  to calculate Euler's  Totient Function
 
# Function to return
# gcd of a and b
def gcd(a, b):
 
    if (a == 0):
        return b
    return gcd(b % a, a)
 



##example code of logic in python


primes = [2,3,5,7]

for x in range(primes[-1] +1 , len(records)+10):
    for p in primes:
        if p > x**0.5:
            primes.append(x)
            break
        if x % p == 0:
            break

set_of_primes = set(primes)


euler_totients = {p : p - 1 for p in primes}
def euler_totient(n):
    # Initialize the count of coprime numbers to 0
    if n == 1:
        return 1
    if n in euler_totients:
        return euler_totients[n]
    for p in primes:
        if n % p == 0:
            #get largest power of p that divides n
            k = 1
            while n % p**k == 0:
                k += 1
            p_k = p**(k-1)
            break
    euler_totients[p_k] = (p-1) * p**(k-1 - 1)
    euler_totients[n//p_k] = euler_totient(n//p_k)
    euler_totients[n] = euler_totients[p_k] * euler_totients[n//p_k]
    return euler_totients[n]





plotting_list = []

for j,(modulus, value) in enumerate(records.items()):
    denom = euler_totient(modulus) * math.log(value)
    num = value - euler_totient(modulus) * math.log(euler_totient(modulus)) * math.log(value)
    plotting_list.append( num / denom)
 
b = 1.45
c = -13.6/math.e

x = np.linspace(-5,5,1000)
cdf = np.exp(-np.exp(c - b*x))
pdf = np.array([cdf[i+1] - cdf[i] for i in range(len(cdf)-1)])
pdf = pdf / (x[1]-x[0])
#density of y

plt.plot(x[:-1],pdf)


plt.hist(plotting_list,bins = 1000,density = True)
plt.xlim(-5,5)
plt.title('Scaled least prime vs Gumbel distribution')
plt.savefig('histogram.png')
