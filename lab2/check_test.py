inp = []
out = []
err = []
k_err = 0
with open("input.txt", "r") as f:
    for line in f:
        s = line.split()
        if s[0] == "5":
            inp.append((s[1], s[2], s[3], s[4], s[5], s[0]))
        else:
            inp.append((s[1], s[2], s[3], s[4], 0, s[0]))


with open("output.txt", "r") as f:
    i = 0
    for line in f:
        s = line.split()
        if inp[i][5] == "5":
            out.append((s[0], s[1], s[2], s[3], s[4]))
        else:
            out.append((s[0], s[1], s[2], s[3]))
        i += 1



with open("testresult.txt" ,"w") as f:
    for i in range(100):
        arg = inp[i][5]
        if arg == "5":
            print("Expected", inp[i][0], inp[i][1], inp[i][2], inp[i][3], inp[i][4], file = f)
            print("Result  ", out[i][0], out[i][1], out[i][2], out[i][3], out[i][4], file = f)
            if (inp[i][0] == out[i][0] and inp[i][1] == out[i][1] and inp[i][2] == out[i][2] and inp[i][3] == out[i][3] and inp[i][4] == out[i][4]):
                print("Test", i + 1, "Passed", file = f)
            else:
                print("Test", i + 1, "Not Passed", file = f)
        else:
            print("Expected", inp[i][0], inp[i][1], inp[i][2], inp[i][3], file = f)
            print("Result  ", out[i][0], out[i][1], out[i][2], out[i][3], file = f)
            if (inp[i][0] == out[i][0] and inp[i][1] == out[i][1] and inp[i][2] == out[i][2] and inp[i][3] == out[i][3]):
                print("Test", i + 1, "Passed", file = f)
            else:
                print("Test", i + 1, "Not Passed", file = f)
                err.append(i + 1)
                k_err += 1
print(k_err)
            