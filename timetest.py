import time
pairs = [(x, x) for x in range(1000)]
start = time.time()
for _ in range(10000):
    time_v = [p[0] for p in pairs]
    value = [p[1] for p in pairs]
end = time.time()
print(end - start)

start = time.time()
for _ in range(10000):
    time_v = []
    value = []
    for p in pairs:
        time_v.append(p[0])
        value.append(p[1])
        
end = time.time()

print(end - start)
start = time.time()
for _ in range(10000):
    lst1, lst2 = map(list, zip(*pairs))            
end = time.time()
print(end - start)
