f = open("run.info")
lines = f.readlines()
for line in lines:
	print(line.split("."))
