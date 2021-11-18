# testing the stupid numToStr function...'
temp = ""
a = 1234

i = 1
while(a - i >= 0):
    temp = temp + f"{round(a / i) % 10}"
    i = i * 10
print(temp)