#include <iostream>
#include <iomanip>
#include <string>
#include <vector>

using namespace std;

//global vectors
vector<string>jobName;
vector<int>partitionArray;
vector<int>jobSizeArray;
vector<int>sizeHolder;
vector<int>sequenceHolder;


int getPartitionNumber(){
	int tempPartitionNumbers;
	int partitionCheck = 0;
	do{
    	cout<<"Enter Number of partition(s): ";
    	cin>>tempPartitionNumbers;
     	if(tempPartitionNumbers <= 5 && tempPartitionNumbers>0){
        	partitionCheck = 1;
     	}else
        	cout<<"Range for number of partition is 1 to 5."<<endl;
    	 
	} while(partitionCheck == 0);
	return tempPartitionNumbers;
}

void getPartitionSize(int partitionNum, int memSize){
	bool condition = true;
	
	int temp;
	
    
	for(int i = 0; i<partitionNum;i++){
    	cout<<"Enter Partition size for P"<<i+1<<":";
        cin>>temp;
        if (temp>memSize || temp<0){
            	cout<<"Size must be between 0 to "<<memSize<<endl;
            	i--;
        }else{
            partitionArray.push_back(temp);
            memSize -= temp;
            cout<<setw(50)<<"Remaining memory: "<<memSize<<endl;
            temp =0;
            if (memSize == 0){
                for (int index = i+1; index<partitionNum-1; index++){
                	partitionArray.push_back(0);
        	    }
            i=partitionNum;
            }
        }
	}
}

void getJobSize(int jobCount){
    
    cout<<"\n\n******** Enter Job Details below ********\n"<<endl;
    
    for(int i =0; i<jobCount;i++){
		string temp; int tempSize = 0;
        cout<<"Enter name for job"<<i+1<<": ";
		cin>>temp;
		jobName.push_back(temp);
		
		cout<<"Enter job size: ";
        cin>>tempSize;
        jobSizeArray.push_back(tempSize);
        
        if(tempSize == 0){
            cout<<"Job size should to be at least 1"<<endl;
            i--;
        }
    }
}


int addData(int memorySize, int partitionSize){
    
    memorySize = 1000;
    partitionSize = 4;
    string jobName1[]={"1","2","3","4"};
    //int partitionArray1[] = {100,200,300,400};
    //int jobSizeArray1[]= {1000,200,300,100};
    int partitionArray1[] = {300,200,400,100};
    int jobSizeArray1[]= {200,300,100,200};
	//copying arrays into global variable
    copy(begin(jobName1), end(jobName1), back_inserter(jobName));
    copy(begin(partitionArray1), end(partitionArray1), back_inserter(partitionArray));
    copy(begin(jobSizeArray1), end(jobSizeArray1), back_inserter(jobSizeArray));

    cout<<"Memory Size: "<<memorySize<<endl;
    cout<<"Number of Partition: "<<partitionSize<<endl;
    
    int index[]={0,1,2,3};
    for(int i:index){
        cout<<"Partition "<<i+1<<": "<<partitionArray[i]<<"   "<<"Job "
        <<i+1<<": "<<jobSizeArray[i]<<endl;
    }
    
    return partitionSize;
}

bool findElement(int element, int partSize){
    
    for(int i = 0; i<sequenceHolder.size(); i++){

        if(sequenceHolder[i] == element){
            return true;
        }
    }
    return false;
}


void printData(int partSize){
    int waste = 0;
    int totalWaste=0;

    for (int i = 0; i<partSize; i++){
        if((!(findElement(i+1, partSize)))){
		       totalWaste +=partitionArray[i];
		   }
		   
        if (sequenceHolder[i] > 0){
            //cout<<"\nwaste for i: "<<i<<"  "<<partitionArray[sequenceHolder[i]-1]<<" sequenceHolder  "<<sequenceHolder[i]-1<<"  "<< sizeHolder[i];
            waste = partitionArray[sequenceHolder[i]-1] - sizeHolder[i];
            totalWaste += waste;
			cout<<"\n"<<setw(7)<<"Job "<<jobName[i]<<"  -> "<<setw(3)<<"P"<<sequenceHolder[i]<<setw(10)<<waste<<setw(10)<<"  Run";
        }
        else{
			cout<<"\n"<<setw(7)<<"Job "<<jobName[i]<<"  ->   --" <<setw(10)<<"---"<<setw(11)<<"Wait";
		}
    }
    cout<<"\n\nTotal Waste: "<<setw(11)<<totalWaste<<endl;
}


void assignJob(int &jobIndex, int &partIndex, int partSize, bool condition, bool firstOrNext_fit){
    //cout<<"Job  " <<jobIndex<<" partIndex "<<partIndex<<" condition "<<condition<<endl;
    if(condition){
        //cout<<"jobIndex:"<<jobIndex<<" partition index "<<partIndex<<"  jobSizeArray[jobIndex]: "<<jobSizeArray[jobIndex]<<endl;
        sizeHolder.push_back(jobSizeArray[jobIndex]);
        sequenceHolder.push_back(partIndex+1);
        if(!firstOrNext_fit && partIndex < partSize-1){
            jobIndex++;
        }
    }else{
        //if job can't fit in any partition, adding index*(-1)
        sequenceHolder.push_back(0);
        sizeHolder.push_back(partitionArray[partIndex]);
        if(!firstOrNext_fit && partIndex < partSize-1){
            jobIndex++;
        }
    }
    firstOrNext_fit ? partIndex = partSize : (NULL);
}


void first_fit(int partSize, bool firstFit){
    
    sequenceHolder.clear();
    sizeHolder.clear();
    
	firstFit ? cout<<"\n\n********** This is First Fit **********"<<endl : cout<<"\n\n********** This is Next Fit **********"<<endl;
	//variables to keep track of indexes
    int empty_index = 0; int partIndex = 0; int jobIndex = 0; 
    int counter = 0;
    
    for(jobIndex = 0; jobIndex<partSize; jobIndex++){
        //for loop to keep track of partIndex 
        for(partIndex = 0; partIndex<partSize; partIndex++){
            
            //finds if the partition in use or not, if not in use returns true
            if((!(findElement(partIndex+1, partSize))) && (partitionArray[partIndex] >= jobSizeArray[jobIndex])){
               
               assignJob(jobIndex, partIndex, partSize, true, firstFit);
               counter = 0;
               
           }else{
			   if(counter<partSize-1){
			       counter++;
			   }else{
			       counter = 0;
			       assignJob(jobIndex, partIndex, partSize, false, firstFit);
               }
            }
        }
    }
    
	cout<<setw(10)<<"Job Names | "<<setw(10)<<"Partition | "<<setw(6)<<"Waste |"<<setw(7)<<"  Status";

    printData(partSize);
    
}

void best_fit(int partSize){
    
    sequenceHolder.clear();
    sizeHolder.clear();
    
    cout<<"\n\n********** This is Best Fit **********"<<endl;
    
    //variables to keep track of indexes
    int empty_index = 0; int partIndex = 0; int jobIndex = 0; 
    
    //outer for loop to keep track of jobIndex
    for(jobIndex = 0; jobIndex<partSize; jobIndex++){
        //for loop to keep track of partIndex 
        for(partIndex = 0; partIndex<partSize; partIndex++){
            
            //checks the size of partition with job size, and cheks if the partition has been allocated or not
            if((partitionArray[partIndex] >= jobSizeArray[jobIndex]) && (!(findElement(partIndex+1, partSize)))){
            
                if(partitionArray[partIndex] == jobSizeArray[jobIndex]){
                    
                    assignJob(jobIndex, partIndex, partSize, true, true);

                }else{
                    //storing memory waste to find smallest memory waste
                    int tempSize = partitionArray[partIndex] - jobSizeArray[jobIndex];
                    int tempIndex = -1;
                    //if it is the last job 
                    if (jobIndex == partSize-1){
                        tempIndex = partIndex;
                    }
                    
                    //this for loop will find smallest waste
                    for(int i = 0; i < partSize; i++){
                        int curSize = partitionArray[i] - jobSizeArray[jobIndex];

                        if((partitionArray[i] > jobSizeArray[jobIndex]) && (!(findElement(i+1, partSize)))&&(tempSize>curSize)){
                            tempSize = curSize;
                            tempIndex = i;
                        }
                    }
                    //if 
                    if(tempIndex != -1){
                        assignJob(jobIndex, partIndex, partSize, true, true);
                    }
                    else if(partIndex == partSize-1){
                        assignJob(jobIndex, partIndex, partSize, false, true);
                    }
                }
            }
        }
    }
	cout<<setw(10)<<"Job Names | "<<setw(10)<<"Partition | "<<setw(6)<<"Waste |"<<setw(7)<<"  Status";
    printData(partSize);
}


void next_fit(int partIndex){
    first_fit(partIndex, false);
}

int main(){
	int memorySize;
	int partitionSize;
	int partitionCheck = 0;
	int userInput = 0;
	
	cout<<"Enter 0 to menually enter data or press any number and ENTER."<<endl;
	cin>>userInput;
	
	//giving option to do quik check or by menual data entry
	if (userInput == 0){
    	cout<<"Enter Memory Size: ";
    	cin>> memorySize;
    	
    	partitionSize = getPartitionNumber();
    	
    	getPartitionSize(partitionSize, memorySize);
    	
    	getJobSize(partitionSize);
	}
	
	//to do quik check user can input any number (except 0)
	else{
	    partitionSize = addData(memorySize, partitionSize);
    }
    
    //first_fit(partitionSize, true);
    //best_fit(partitionSize);
    next_fit(partitionSize);

	return 0;
}

