import sys


def get_data():
    """get data from the input.txt file
    and put it in a multidimensional list namd
    dataList = [[process name/number, arrivalTime, requiredCPUtime],
                [same for next line in input.txt]] """
    try:

        dataFile = open("data_priority.txt", "r")  # or "a+", whatever you need
        tempList = dataFile.readlines()
        totalProcess = len(tempList)-1
        dataFile.seek(0)
        dataFile.readline()
        dataList = [None]*(totalProcess)

        for index in range(0, totalProcess):
            #get elements from txt file as string, removes '\n' and split by '/'
            #int(x)... will change each elements from string to int

            tempHolder = dataFile.readline().replace('\n', '').split('/')
            tempHolder[1] = int(tempHolder[1])
            tempHolder[2] = int(tempHolder[2])
            tempHolder[3] = int(tempHolder[3])
            dataList[index] = tempHolder

        dataFile.close()
        return(dataList)
    except IOError:
        print("Could not open file!")
        sys.exit()


def get_avg_tt(dataList, list_len):
    temp = 0
    for index in range(0,list_len):
        temp += dataList[index][6]

    return (temp/list_len)


#adding Completetion time and Turnaround Time in this function
def add_ct_tt(dataList):

    list_len = len(dataList)

    for index in range(0, list_len):

        if(index>0):
            dataList[index].append(dataList[index-1][5])
        else:
            dataList[index].append(dataList[index][1])

        # add 5th element = completion time
        if index != 0:
            #checking if the current process arrived before
            #last process finished
            if dataList[index][1]<dataList[index-1][5]:
                dataList[index].append(dataList[index][2]+dataList[index-1][5])
            else:
                dataList[index].append(dataList[index][2]+dataList[index-1][5]+(dataList[index][1] - dataList[index-1][5]))
        else:
            dataList[index].append(dataList[index][2])

        #add 5th element =  turnaround time(completion time - arrival time)
        dataList[index].append(dataList[index][5]-dataList[index][1])

    avg_tt = get_avg_tt(dataList, list_len)

    printData(dataList, list_len, avg_tt)

#first come first serve
def fcfs(dataList):

    print("\n\n         This is First-Come First-Serve")

    #sort the array by arrival time which is at index 1 of each inner list
    dataList = sorted(dataList, key=lambda arrivalTime:arrivalTime[1], reverse = False)

    add_ct_tt(dataList)


#shortest job next
def sjn_primaryQueue(dataList, sjn):
    print("\n\n         This is Shortest Job Next")  \
        if sjn else print("\n\n         This is Priority Queue")
    in_index = 0
    tempList = []
    tempHolder = []
    time = 0
    condition = True

    #sorting the dataList by arrival time
    dataList = sorted(dataList, key=lambda arrivalTime:arrivalTime[1], reverse = False)

    for out_index in range(0, len(dataList)):

        if (out_index == 0):
            tempList.append(dataList[0])
            time = dataList[0][2]
            dataList.remove(tempList[len(tempList) - 1])
        else:
            #loop runs until we find process that arrives
            #before last process finished
            while(condition and len(dataList) > 0):
                in_index +=1
                #checking completion is bigger than CPU time of first process
                if(time >= dataList[0][1]):

                    tempHolder.append(dataList[0])
                    #we remove the process, so next time we check the first process only
                    #this way first process will be the next one in line
                    dataList.remove(tempHolder[len(tempHolder)-1])
                else:
                    #to stop while loop we change the condition
                    #as arrival time of next process is > completion time of last process
                    condition = False

            #list was sorted by arrival time, here it will be
            # sorted by CPU time to get the sortest process first in list
            if(sjn):
                tempHolder = sorted(tempHolder,
                                key=lambda CPUTime: CPUTime[2],
                                reverse=False)
            else:
                tempHolder = sorted(tempHolder,
                                    key=lambda PQ: PQ[3],
                                    reverse=False)

            if(len(tempHolder) != 0):
                #First element of tempHolder is shortest available process
                tempList.append(tempHolder[0])
                time += tempHolder[0][2]
                #increased time by last added process, now removing
                #it from tempHolder
                tempHolder.remove(tempList[len(tempList) - 1])
                condition = True

    add_ct_tt(tempList)


def printData(dataList, list_len, avg_tt):
    print("_" * 78)

    print("| Process | Arrival | CPU  | Priority |   Start    | Completion | Turnaround |")
    print("|___Name__|__Time___|_Time_|__________|____Time____|____Time____|____Time____|")
    for index in range(0, list_len):
        print(("|"),dataList[index][0].rjust(5),("|").rjust(3),
              repr(dataList[index][1]).rjust(4),("|").rjust(4),
              repr(dataList[index][2]).rjust(3),("|").rjust(2),
              repr(dataList[index][3]).rjust(5), ("|").rjust(4),
              repr(dataList[index][4]).rjust(5),("|").rjust(6),
              repr(dataList[index][5]).rjust(5), ("|").rjust(6),
              repr(dataList[index][6]).rjust(6),("|").rjust(5))

    print("_"*78)

    print("\nAverage Turnaround Time is: ", avg_tt)


def main():
    dataList = get_data()
    #fcfs(dataList)
    #sjn_primaryQueue(dataList, True)
    sjn_primaryQueue(dataList, False)


if __name__ == '__main__':
    main()

"""
OUTPUT:------------------------------------------------------------------------------------------------

         This is Priority Queue
______________________________________________________________________________
| Process | Arrival | CPU  | Priority |   Start    | Completion | Turnaround |
|___Name__|__Time___|_Time_|__________|____Time____|____Time____|____Time____|
|    P1   |    0    |  10  |     4    |     0      |    10      |     10     |
|    P3   |   10    |   5  |     1    |    10      |    15      |      5     |
|    P2   |    5    |   5  |     2    |    15      |    20      |     15     |
|    P4   |   15    |   2  |     3    |    20      |    22      |      7     |
______________________________________________________________________________

Average Turnaround Time is:  9.25
"""
