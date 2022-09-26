#include <iostream>
#include <vector>
#include <fstream> 
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <ctime>
#include <thread>
#include <mutex>
# include <unistd.h>
# include <sys/shm.h>
# include <sys/wait.h>
# include <sys/mman.h>
#include <sstream>
using namespace std ;

mutex mu ;

vector<int> vec ; // 一維 
vector<vector<int>> vec2 ; // 二維 
vector<thread> threads ;
time_t start, end ;
int gnum = 0 ;

// ============= case 1 ========================

string intToString( int num ){
    stringstream ss ;
    ss << num ;
    string str = ss.str() ;
    return str ;
    
}

void readFile(string filename){ // case1
	fstream file ;
    char ss[5] = ".txt" ;
    char ff[20] = "\0" ;
    int number = 0 ;
    strcpy(ff,filename.c_str()) ;
    strcat(ff,ss) ;
	file.open (ff,ios::in) ;
	while(!file) { // file not found
		cout << "file not found!" << endl ;
		cout << "Input a file number: " ;
		cin >> ff ;
        strcat(ff,ss) ;
		file.open(ff,ios::in) ;
	}
	file >> number ; 
	while( !file.eof() ){
		vec.push_back(number) ;
		file >> number ;
	}
	file.close() ;
 
}

void bubbleSort(){
	for(int i=vec.size()-1; i > 0; i--){
	    for(int j = 0; j <= i-1; j++){
	        if( vec.at(j) > vec.at(j+1) ){
	            swap(vec.at(j), vec.at(j+1)) ;
	        }
	    }
    }
    
}

void writeFile( string filename, int num, long ttime ) {
    fstream file ;
    string taskNum = intToString(num) ;
    string str = filename + "_output" + taskNum + ".txt" ;
    file.open(str.c_str(),ios::out) ;
    file << "Sort : " << endl ; 
	for( int i = 0; i < vec.size(); i++ ){
		file << vec.at(i)<< endl ;
	}
	
	file << "CPU Time : " << ttime << endl ;

    time_t now = time(NULL) ;
    char temp[80];
    strftime( temp, sizeof(temp), "%Y-%m-%d %X.%S %Z",localtime(&now) );
    file << "Output Time : " << temp << endl ;

    file.close() ;
} 

// ================= case 1 END ======================

void bubbleSort2(int k){

	for(int i=vec2.at(k).size()-1; i > 0; i--){
	    for(int j = 0; j <= i-1; j++){
	        if( vec2.at(k).at(j) > vec2.at(k).at(j+1) ){
	            swap(vec2.at(k).at(j), vec2.at(k).at(j+1)) ;
	        }
	    }
    }
    
}

void readFile_cut(string filename, int cut){ // NO.4
	fstream file ;
	vector<int> temp ;
    char ss[5] = ".txt" ;
    char ff[20] = "\0" ;
    int number = 0 ;
    strcpy(ff,filename.c_str()) ;
    strcat(ff,ss) ;
    // ====建立二維vector====
	for( int i = 0; i < cut; i++ ){
		temp.clear() ;
		vec2.push_back(temp) ;
	} 
    // ======================
	file.open (ff,ios::in) ;
	while(!file) { // file not found
		cout << "file not found!" << endl ;
		cout << "Input a file name: " ;
		cin >> ff ;
        strcat(ff,ss) ;
		file.open(ff,ios::in) ;
	}
	file >> number ; 
	while( !file.eof() ){
		for( int j = 0; !file.eof() && j < cut; j++ ){
			vec2.at(j).push_back(number) ;
			file >> number ;
		}
	}
	file.close() ;
 
}

void merge(int k){ // k列 、k+1列 case2+case4+case3
	vector<int> temp ;
	int  i = 0, j = 0 ;
	if( k+1 >= vec2.size() ){ //剩一個 
	    ;
	}
	else{
	    while (i < vec2.at(k).size() && j < vec2.at(k+1).size()) {
	        if (vec2.at(k).at(i) <= vec2.at(k+1).at(j)){
	        	temp.push_back(vec2.at(k).at(i)) ;
	        	i++ ;
			} 
	        else{
	        	temp.push_back(vec2.at(k+1).at(j)) ;
	        	j++ ;
			}
	           
	    }

	    while (i < vec2.at(k).size()) {
	       	temp.push_back(vec2.at(k).at(i)) ;
	       	i++ ;
	    }
	 
	    while (j < vec2.at(k+1).size()) {
	       	temp.push_back(vec2.at(k+1).at(j)) ;
	       	j++ ;
	    }	
		
		//====================
		vec2.at(k).assign(temp.begin(), temp.end()) ;
		
		if( gnum == 2 || gnum == 3 ){ // case2 + case3
		    vec2.erase(vec2.begin()+k+1) ; // delete k+1
		}
	}

}

void bubbleMerge(int cut){ // case2
    // ===== bubble sort ======
    for( int i = 0; i < cut; i++ ){
        bubbleSort2(i) ;
    } 

	// =======merge ======
	while( vec2.size() != 1 ){
		for( int k = 0; k < vec2.size(); k+=2 ){
			merge(k) ;
		}

	}	
}


void multithread(int cut){
    time_t start2, end2 ;    
    // ===== bubble sort ======
    start2 = time(NULL) ;
    for( int i = 0; i < cut; i++ ){
        threads.push_back(thread(bubbleSort2, i));
    } 
	for( int i = 0; i < threads.size(); i++ ) {
        threads.at(i).join();
    
    }
    threads.clear() ;
    end2 = time(NULL) ;
	double diff = difftime(end2, start2) ;
//    printf("TTTT = %f\n", diff) ;

	// =======merge ======
	while( vec2.size() != 1 ){
		
		for( int k = 0; k < vec2.size(); k+=2 ){
			threads.push_back(thread(merge, k)) ;
		}
		
		for( int i = 0; i < threads.size(); i+=1 ) {
	        threads.at(i).join();
	    }
	    threads.clear() ;
	    
	    mu.lock() ;
		for( int k = 0; k+1 < vec2.size(); k+=1 ){
			vec2.erase(vec2.begin()+k+1) ; // delete k+1
		}	    

        mu.unlock() ;
		
	}							

}
//================= case4 =========================
	
void* create_shared_memory(size_t size) {
	int protection = PROT_READ | PROT_WRITE;
	int visibility = MAP_SHARED | MAP_ANONYMOUS;
	return mmap(NULL, size, protection, visibility, -1, 0);
}


void multiprocess(int cut){
//	int *i = (int*)create_shared_memory( sizeof( int ) * 4 ) ;
	int *k = (int*)create_shared_memory( sizeof( int ) * 4 ) ;
    int *temp = (int*)create_shared_memory( (vec2.at(0).size()+1)*4 ) ;
    
    while((*k) < cut){
    	pid_t pid = fork() ;
    	if( pid > 0 ){
    		wait(0) ;
			for( int j = 0; j < temp[0]; j++ ){
				vec2.at((*k)-1).at(j) = temp[j+1] ;
			}    		
		}
		else if( pid == 0 ){
			bubbleSort2((*k)) ;
			temp[0] = vec2.at((*k)).size() ;
			
			for( int j = 0; j < temp[0]; j++ ){
				temp[j+1] = vec2.at((*k)).at(j) ;
			}
			(*k) = (*k) + 1 ;
			exit(0) ;
		}
		else{
			cout << "Error!" << endl ;
			exit(0) ;
		}
	}
		
}

void merge_multiprocess(){
	while( vec2.size() != 1 ){
		for( int k = 0; k < vec2.size(); k+=2 ){
			merge(k) ;
		}

	}	
}


void writeFile2( string filename, int num, long ttime ) {
    fstream file ;
    string taskNum = intToString(num) ;
    string str = filename + "_output" + taskNum + ".txt" ;
    file.open(str.c_str(),ios::out) ;
    file << "Sort : " << endl ; 
	for( int i = 0; i < vec2.size(); i++ ){
		for( int j = 0; j < vec2.at(i).size(); j++ ){
			file << vec2.at(i).at(j) << endl ;
		}
	}
	
	file << "CPU Time : " << ttime << endl ;

    time_t now = time(NULL) ;
    char temp[80];
    strftime( temp, sizeof(temp), "%Y-%m-%d %X.%S %Z",localtime(&now) );
    file << "Output Time : " << temp << endl ;

    file.close() ;
} 

int main(void) {
	string filename = "\0" ;
	int cut = 0, num = 0 ;
	double diff = 0 ;
	time_t start, end ;
	do {
	    cout << endl << "Input a file name [0: Quit]: " ;
	    cin >> filename ;
	    if( filename == "0" ){
	    	break ;
		}
		else{

		    cout << endl << "Input method number: " ;		
			cin >> num ;
			if( num == 1 ){
				readFile(filename) ;
				start = time(NULL) ;
				bubbleSort() ;
				end = time(NULL) ;
				diff = difftime(end, start) ;
				writeFile(filename,num,diff) ;				
			}
			else if( num == 2 ){
		    	cout << endl << "Input cuts of file: " ;
		    	cin >> cut ; // 幾份 
				gnum = 2 ;
				readFile_cut(filename, cut) ;
				start = time(NULL) ;
				bubbleMerge(cut) ;
				end = time(NULL) ;
				diff = difftime(end, start) ;
				writeFile2(filename,num,diff) ;
			}
			else if( num == 3 ){
		    	cout << endl << "Input cuts of file: " ;
		    	cin >> cut ; // 幾份 
				gnum = 3 ;
				readFile_cut(filename, cut) ;
				start = time(NULL) ;
				multiprocess(cut) ;
				merge_multiprocess() ;
				end = time(NULL) ;
				diff = difftime(end, start) ;
				writeFile2(filename,num,diff) ;
				
			}
			else if( num == 4 ){
		    	cout << endl << "Input cuts of file: " ;
		    	cin >> cut ; // 幾份 
				readFile_cut(filename, cut) ;
				start = time(NULL) ;
				multithread(cut) ;
				end = time(NULL) ;
				diff = difftime(end, start) ;
				writeFile2(filename,num,diff) ;
			}
			else{
				cout << "ERROR METHOD!" << endl ;
			}
			
			
             printf("Time = %f\n", diff) ;
			threads.clear() ;
			vec.clear() ;
			vec2.clear() ;
			start = 0 ;
			end = 0 ;
			gnum = 0 ;

		}

	    cout << endl << "=============================" << endl ;
	} while (filename != "0"); // '0': stop the program

	system("pause"); // pause the display
	return 0;
} // end main


