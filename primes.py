from collections import defaultdict
import sys 

##example code of logic in python


primes = [2,3,5,7]

for x in range(primes[-1] +1 , 100_00):
    for p in primes:
        if p > x**0.5:
            primes.append(x)
            break
        if x % p == 0:
            break

set_of_primes = set(primes)
def gcd(a,b):
    while b:
        a,b = b, a%b
    return a


records = defaultdict(list)

for modulus in range(2,100):
    for ap in range(1,modulus):
        if gcd(ap,modulus) == 1:
            searching =True
            ell = 0 
            while searching:
                tmp = ap + ell*modulus
                if tmp in set_of_primes:
                    records[modulus].append(tmp)
                    searching = False
                ell += 1
                assert tmp < primes[-1] , "We need more primes"

print(f"Size of records: {sys.getsizeof(records)/10e6} MB")

for j in range(2,100):
    print(f"Modulus {j} has value: {max(records[j])}")