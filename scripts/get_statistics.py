import sys
import os
import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt

def get_dict(file_name, n_group) :
	file = open(file_name, "r")
	diction = {}
	line = file.readline()
	while line:
		group_name = line.strip()
		line_start = file.tell()
		try:
			for i in range(n_group - 1):
				next_line = next(file)
				next_line = next_line.strip()
				group_name += " " + next_line
		except StopIteration as e:
			break
		if group_name not in diction:
			diction[group_name] = 0
		diction[group_name] += 1
		file.seek(line_start)
		line = file.readline()
	file.close()
	return diction

def plot(diction, n_group, path_to_save) :
	diction_sorted = sorted(diction.items(), key=lambda x:x[1], reverse=True)
	dict_to_plot = diction_sorted[:10]
	values = [tup[1] for tup in dict_to_plot]
	names = [tup[0] for tup in dict_to_plot]
	total_amount = sum(values)
	values = [value/total_amount for value in values]
	names = [name.replace(' ', '\n') for name in names]
	fig = plt.figure(figsize=(16, 10))
	plt.bar(names, values)
	plt.title("Frequency of llvm instructions (by groups of %d) in app" %n_group)
	plt.xlabel("Names")
	plt.ylabel("Frequency, %")
	fig.savefig(path_to_save + "freq_by_group_of_" + str(n_group))

def get_statistic_by_groups(file_name, n_group, path_to_save) :
	diction = get_dict(file_name, n_group)
	plot(diction, n_group, path_to_save)

if __name__ == '__main__' :
	get_statistic_by_groups(sys.argv[1], 1, sys.argv[2])
	get_statistic_by_groups(sys.argv[1], 2, sys.argv[2])
	get_statistic_by_groups(sys.argv[1], 3, sys.argv[2])
	get_statistic_by_groups(sys.argv[1], 4, sys.argv[2])
	get_statistic_by_groups(sys.argv[1], 5, sys.argv[2])