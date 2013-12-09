def createColorCube(file):
    for i in range(11):
        for j in range(11):
            for k in range(11):
                if i == 10 or j == 10 or k == 10:
                    file.write("diffuse " + str(float(i) / 10) + " " + str(float(j)/ 10) + " " + str(float(k) / 10) + "\n")
                    file.write("pushTransform\n")
                    file.write("translate ")
                    file.write(str(i) + " " + str(j) + " " + str(k) + "\n")
                    file.write("sphere 0 0 0 .3\n")
                    file.write("popTransform\n")
                    file.write("\n")

def main():
    with open('scenefile.txt', 'w') as file:
        file.write("size 1000 1000\n")
        file.write("camera 25 25 25 0 0 0 0 0 1 30\n")
        file.write("ambient .1 .1 .1\n")
        file.write("directional 1 1 1 .5 .5 .5\n")
        file.write("specular .5 .5 .5\n")
        file.write("shininess 20\n\n")
 #       file.write("pushTransform\n")
 #       file.write("translate 0 -5 -20\n\n")
        createColorCube(file)
 #       file.write("\n\npopTransform")

main()
