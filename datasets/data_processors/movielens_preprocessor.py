import csv
import sys

# This uses the ratings file with headers "userId","movieId","rating","timestamp"
# Get it from here https://www.kaggle.com/grouplens/movielens-20m-dataset?select=rating.csv
# Pass it source file and destination file (data_directory/movielens.csv)
with open(sys.argv[1], 'r') as csvfile:
    csvreader = csv.reader(csvfile, delimiter=',')
    # Skip headers
    csvreader.__next__()
    id_map = {}
    i = 0
    with open(sys.argv[2], 'w+') as of:
        of.write("movieId,rating\n")
        for line in csvreader:
            if line[1] not in id_map:
                id_map[line[1]] = i
                i = i + 1
            of.write(f"{id_map[line[1]]},{line[2]}\n")


