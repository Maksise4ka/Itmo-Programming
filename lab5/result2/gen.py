import imageio
images = []
for i in range(1, 509):
    images.append(imageio.imread("result" + str(i) + ".bmp"))
imageio.mimsave('res.gif', images)