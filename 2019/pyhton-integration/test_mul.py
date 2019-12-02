import swigdemo

v = swigdemo.mul(3)
print v

# s1 = std::string('hello')
s = swigdemo.concat('hello ', 'Maxim')
print s

try:
    s2 = swigdemo.concat('hi', '')
except:
    print "Error occured"

a = [1, 2, 3, 4]
# b = swigdemo.reverse(a)
b = swigdemo.generate(10)

print(b)
