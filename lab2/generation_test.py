import random as r

#эта штука генерирует число длиной от 20 до 300 так,
#что каждая цифра генерируется отдельно друг от друга(хз зачем может пригодиться)
def num_generation():
    randlen = r.randint(20, 300)
    ans = r.randint(1, 9)
    for i in range(randlen - 1):
        ans = ans * 10 + r.randint(0, 9)
    return ans

def test_generation(f):
    a = r.randint(10, 2 ** 600)
    b = r.randint(10, 2 ** 600)
    a, b = max(a,b), min(a, b)
    arg = 0
    
    if a * b <= 2 ** 1024:
        arg = 5
        print(arg, a, b, a + b, a - b, a * b, file = f)
    else:
        arg = 4
        print(arg, a, b, a + b, a - b, file = f)

with open("input.txt", "w") as f:
    for i in range(100):
        test_generation(f)
