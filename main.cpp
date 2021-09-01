# include <iostream>
# include <stdio.h>
# include <vector>
# include <cstdio> // fscanf
# include <fstream> // writeOutput
# include <stdlib.h> // itoa
# include <queue>
using namespace std ;


struct Process {
	int pid ;
	int cpu_burst ;
	int arrival_time ;
	int priority ;
	int waiting ; // waiting time
	int turnaround ; // turnaround time
	int done_time ; // 完成時間 
};

struct Node{
	queue<Process> q ; // 相同priority在同一個Queue中 
	int priority ;
};


vector<Process> input_list ;
vector<Process> sorted_list ; // 以排序過的list
vector<Node> multi_queue ; // PPRR用 

// 計算完成及等待時間 
vector<Process> fcfs_list ;
vector<Process> rr_list ;
vector<Process> srtf_list ;
vector<Process> pprr_list ;
vector<Process> hrrn_list ;

// 甘特圖 
static string list1 ; // FCFS
static string list2 ; // RR
static string list3 ; // SRTF
static string list4 ; // PPRR 
static string list5 ; // HRRN

bool ReadFile( string fileName, int &method_num, int &time_slice ) {
	FILE *pfile = NULL ;
	fileName = fileName + ".txt" ;
	pfile = fopen( fileName.c_str(), "r" ) ;
	if ( pfile == NULL ) {
		cout << "檔案不存在\n" ;
		return false ;
	} // if 
	else { // 開始讀檔 
		fscanf( pfile, "%d", &method_num ) ; 
		fscanf( pfile, "%d", &time_slice ) ; 
		char ch = '\0' ;
		fscanf( pfile, "%c", &ch ) ; // 讀換行 
		fscanf( pfile, "%c", &ch ) ;
		while ( ch != '\n')   //讀掉第二行字 (ID  CPU Burst  Arrival Time   Priority) 
			ch = getc( pfile ) ;
		
		int temp = -1 ; // 讀入的東西暫存
		Process p ; // 讀入的東西暫存這裡，之後推入vector中 
		while ( fscanf( pfile, "%d", &temp ) != EOF ) {
			p.pid = temp ;
			fscanf( pfile, "%d", &temp ) ;
			p.cpu_burst = temp ;
			fscanf( pfile, "%d", &temp ) ;
			p.arrival_time = temp ;
			fscanf( pfile, "%d", &temp ) ;
			p.priority = temp ;
			input_list.push_back(p) ;
		} // while  
		return true ;
	} // else 
} // ReadFile()

void Debug() {
	int i = 0 ;
	while ( i < sorted_list.size() ) {
		cout << sorted_list.at(i).pid << " " << sorted_list.at(i).cpu_burst 
			 << " " << sorted_list.at(i).arrival_time << " " << sorted_list.at(i).priority << "\n" ;
		i++ ;
	} // while 
}


void Sort_By_Arrival_Time() { // 將input_list依照Arrival_time排序 
	int i = 0 ;
	int j = 0 ;
	vector<Process> input_list_temp = input_list;
	Process ptemp = input_list.at(0) ; // 暫存目前最小的 
	int temp_min_index ;

	
	while( i < input_list_temp.size() ) {
		ptemp = input_list_temp.at(0) ;
		j = 0 ;
		temp_min_index = j ;
		while ( j < input_list_temp.size() ) { // 從頭跑到尾 
			if ( input_list_temp.at(j).arrival_time < ptemp.arrival_time ) { // 較小 存入ptemp中 
				ptemp = input_list_temp.at(j) ;
				temp_min_index = j ;
			} // if 
			else if ( input_list_temp.at(j).arrival_time == ptemp.arrival_time ) { // 一樣大 -> 比PID 
				if ( input_list_temp.at(j).pid < ptemp.pid ) { // 較小 存入ptemp中  
					ptemp = input_list_temp.at(j) ;
					temp_min_index = j ;
				} // if 
			} // else if
			
			j++ ; 
		} // while 
		sorted_list.push_back(ptemp) ; // 確認好最小的就推入list中 
		input_list_temp.erase(input_list_temp.begin()+temp_min_index) ;
		
	} // while 
} // Sort_By_Arrival_Time()

void Calc_time( int num ) {
	// 先照PID排序 
	vector<Process> temp_list = sorted_list ; // 按照PID排序 暫存的list 
	int i = 0 ;
	int j = 0 ;
	int temp_min_index ;
	Process temp_to_swap ; // 交換用的temp 
	
	while( i < temp_list.size() ) {
		j = i ;
		temp_min_index = j ;
		while ( j < temp_list.size() ) { // 從頭跑到尾 
			if ( temp_list.at(j).pid < temp_list.at(temp_min_index).pid ) { // 剩下中最小的 
				temp_min_index = j ;
			} // if 
			
			j++ ; 
		} // while 
		
		// SWAP 
		temp_to_swap = temp_list.at(i) ;
		temp_list.at(i) = temp_list.at(temp_min_index) ;
		temp_list.at(temp_min_index) = temp_to_swap ;
		
		// 計算Turnaround time
		temp_list.at(i).turnaround = temp_list.at(i).done_time - temp_list.at(i).arrival_time ;
		
		// 計算Waiting time 
		temp_list.at(i).waiting = temp_list.at(i).turnaround - temp_list.at(i).cpu_burst ;
	    i++ ;	
    } // while 
	

	
	if ( num == 1 ) { // fcfs_list
		fcfs_list = temp_list ;
	} // if 
	else if ( num == 2 ) { // rr_list
		rr_list = temp_list ;
	} // else if 
	else if ( num == 3 ) { // srtf_list
		srtf_list = temp_list ;
	} // else if
	else if ( num == 4 ) { // pprr_list
		pprr_list = temp_list ;
	} // else if
	else if ( num == 5 ) { // hrrn_list
		hrrn_list = temp_list ;
	} // else if

} // Calc_time()

void Output_Waiting( string &output, string str, vector<Process> list ){

	output = output + "waiting\n" ;
	output = output + "ID	" + str + "\n" ;
	output = output + "===========================================================\n" ;
	char ch_pid[100] ;
	char ch_waiting[100] ;
	int i = 0 ;
	while ( i < list.size() ) {
		itoa(list.at(i).pid, ch_pid, 10 ) ;
		itoa(list.at(i).waiting, ch_waiting, 10 ) ;
		output = output + ch_pid + "	" + ch_waiting + "\n" ;
		i++ ;
	} // while 
	output = output + "===========================================================\n\n" ;
} // Output_Waiting()

void Output_Turnaround( string &output, string str, vector<Process> list ){

	output = output + "Turnaround Time\n" ;
	output = output + "ID	" + str + "\n" ;
	output = output + "===========================================================\n" ;
	char ch_pid[100] ;
	char ch_tt[100] ; 
	int i = 0 ;
	while ( i < list.size() ) {
		itoa(list.at(i).pid, ch_pid, 10 ) ;
		itoa(list.at(i).turnaround, ch_tt, 10 ) ;
		output = output + ch_pid + "	" + ch_tt + "\n" ;
		i++ ;
	} // while 

} // Output_Turnaround()




void Output_Waiting_ALL( string &output, string str ){

	output = output + "waiting\n" ;
	output = output + "ID	" + str + "\n" ;
	output = output + "===========================================================\n" ;
	char ch_pid[100] ;
	char ch_waiting1[100] ;
	char ch_waiting2[100] ;
	char ch_waiting3[100] ;
	char ch_waiting4[100] ;
	char ch_waiting5[100] ;
	int i = 0 ;
	while ( i < fcfs_list.size() ) {
		itoa(fcfs_list.at(i).pid, ch_pid, 10 ) ;
		itoa(fcfs_list.at(i).waiting, ch_waiting1, 10 ) ;
		itoa(rr_list.at(i).waiting, ch_waiting2, 10 ) ;
		itoa(srtf_list.at(i).waiting, ch_waiting3, 10 ) ;
		itoa(pprr_list.at(i).waiting, ch_waiting4, 10 ) ;
		itoa(hrrn_list.at(i).waiting, ch_waiting5, 10 ) ;
		output = output + ch_pid + "	" + ch_waiting1 + "	" + ch_waiting2+ "	" + ch_waiting3
						+ "	" + ch_waiting4 + "	" + ch_waiting5 + "\n" ;
		i++ ;
	} // while 
	output = output + "===========================================================\n\n" ;
} // Output_Waiting_ALL()

void Output_Turnaround_ALL( string &output, string str ){

	output = output + "Turnaround Time\n" ;
	output = output + "ID	" + str + "\n" ;
	output = output + "===========================================================\n" ;
	char ch_pid[100] ;
	char ch_tt1[100] ; 
	char ch_tt2[100] ; 
	char ch_tt3[100] ; 
	char ch_tt4[100] ; 
	char ch_tt5[100] ; 
	int i = 0 ;
	while ( i < fcfs_list.size() ) {
		itoa(fcfs_list.at(i).pid, ch_pid, 10 ) ;
		itoa(fcfs_list.at(i).turnaround, ch_tt1, 10 ) ;
		itoa(rr_list.at(i).turnaround, ch_tt2, 10 ) ;
		itoa(srtf_list.at(i).turnaround, ch_tt3, 10 ) ;
		itoa(pprr_list.at(i).turnaround, ch_tt4, 10 ) ;
		itoa(hrrn_list.at(i).turnaround, ch_tt5, 10 ) ;
		output = output + ch_pid + "	" + ch_tt1 + "	" + ch_tt2 + "	" + ch_tt3 
		                + "	" + ch_tt4 + "	" + ch_tt5 + "\n" ;
		i++ ;
	} // while 
	
	output = output + "===========================================================\n\n" ;
} // Output_Turnaround_ALL()

void WriteOutput( int num, string fileName ) {
	fstream file ;

    string str = "out_" + fileName  ;
    str = str + ".txt"  ;
	file.open( str.c_str(), ios::out ) ;
	if( file ) {

		if ( num == 1 ) {
			file << "FCFS\n" ;
			file << list1 << "\n" ;
			file << "===========================================================\n\n" ; 
			string str = "FCFS" ;
			string output = "" ;
			Output_Waiting( output, str, fcfs_list ) ;
			file << output ;
			output = "" ;
			Output_Turnaround( output, str, fcfs_list ) ;
			file << output ;
		} // if 
		else if ( num == 2 ) {
			file << "RR\n" ;
			file << list2 << "\n" ;
			file << "===========================================================\n\n" ; 
			string str = "RR" ;
			string output = "" ;
			Output_Waiting( output, str, rr_list ) ;
			file << output ;
			output = "" ;
			Output_Turnaround( output, str, rr_list ) ;
			file << output ;
		} // else if
		else if ( num == 3 ) {
			file << "SRTF\n" ;
			file << list3 << "\n" ;
			file << "===========================================================\n\n" ; 
			string str = "SRTF" ;
			string output = "" ;
			Output_Waiting( output, str, srtf_list ) ;
			file << output ;
			output = "" ;
			Output_Turnaround( output, str, srtf_list ) ;
			file << output ;
		} // else if
		else if ( num == 4 ) {
			file << "PPRR\n" ;
			file << list4 << "\n" ;
			file << "===========================================================\n\n" ; 
			string str = "PPRR" ;
			string output = "" ;
			Output_Waiting( output, str, pprr_list ) ;
			file << output ;
			output = "" ;
			Output_Turnaround( output, str, pprr_list ) ;
			file << output ;
		} // else if
		else if ( num == 5 ) {
			file << "HRRN\n" ;
			file << list5 << "\n" ;
			file << "===========================================================\n\n" ; 
			string str = "HRRN" ;
			string output = "" ;
			Output_Waiting( output, str, hrrn_list ) ;
			file << output ;
			output = "" ;
			Output_Turnaround( output, str, hrrn_list ) ;
			file << output ;
		} // else if
		else if ( num == 6 ) {
			file << "All\n" ;
			file << "==        FCFS==\n" << list1 << "\n" ; 
			file << "==          RR==\n" << list2 << "\n" ;
			file << "==        SRTF==\n" << list3 << "\n" ;
			file << "==        PPRR==\n" << list4 << "\n" ;
			file << "==        HRRN==\n" << list5 << "\n" ;
			file << "===========================================================\n\n" ; 
			string str = "FCFS	RR	SRTF	PPRR	HRRN" ;
			string output = "" ;
			Output_Waiting_ALL( output, str ) ;
			file << output ;
			output = "" ;
			Output_Turnaround_ALL( output, str ) ;
			file << output ;
		} // else if
		
		
		file.close() ;		
	}// if
} // writeOutput

// ------------------------------------------------------------------------------- FCFS

void FCFS( string fileName ) {
	Sort_By_Arrival_Time() ; // 將input_list依照Arrival_time排序
	// Debug() ; 
	vector<Process> fcfs = sorted_list ; // copy一份已排好的list 
	int timer = 0 ;
	char ch = '\0' ;
	int i = 0 ; // sorted_list的index 
	while ( !fcfs.empty() ) {
		if ( fcfs.at(0).arrival_time > timer ) { // 人還沒到 
			list1 = list1 + "-" ;
		} // if 
		else {
			if ( fcfs.at(0).pid > 9 ) {
				ch = fcfs.at(0).pid + 55 ; // 10 = A, 11 = B...
				list1 = list1 + ch ;
			} // if 
			else {
				ch = fcfs.at(0).pid + '0' ;
				list1 = list1 + ch ;
			} // else 
			
			fcfs.at(0).cpu_burst = fcfs.at(0).cpu_burst - 1 ;
			if ( fcfs.at(0).cpu_burst == 0 ) { // 做完就給我滾  
				sorted_list.at(i).done_time = timer + 1; // 完成時間  (+1: 因為目前做的時間還沒算)
				fcfs.erase( fcfs.begin() ) ;
				i++ ; // 換下一個人做 
			} // if 
		} // else 
		
		timer++ ;
	} // while 
	
} // FCFS()
// ------------------------------------------------------------------------------------ RR
void RR( string fileName, int time_slice, bool is_method_six) {
	if ( is_method_six == false )
		Sort_By_Arrival_Time() ; // 將input_list依照Arrival_time排序
	vector<Process> rr = sorted_list ; // copy一份已排好的list 
	Process temp ; // 暫存正在run的process 
	queue<Process> readyQ ; // Ready Queue
	int timer = 0 ;
	int countdown = time_slice ; // 倒數timeSlice
	bool cpu_running = false ;
	char ch = '\0' ;
	bool done = false ;
	
	while( done == false ) {

		if ( rr.empty() && readyQ.empty() && temp.cpu_burst == 0 ) { // 全部都做完了  
			done = true ;
		} // if 
		else {
					
			if ( rr.empty() == false && rr.at(0).arrival_time > timer && cpu_running == false && readyQ.empty() ) { // 人還沒到 
				list2 = list2 + "-" ;
			} // if 
			else {
				while ( rr.empty() == false && rr.at(0).arrival_time == timer ) {
					readyQ.push(rr.at(0)) ; // 新來的到ready queue排隊
					rr.erase(rr.begin()) ;
					
				} // while 
				
				if ( countdown == 0 ) { // time Slice用完 
					if ( temp.cpu_burst == 0 ) { // process完成 
						;				 
					} // if 
					else {
						readyQ.push(temp) ; // 還沒做完就推回去 
						cpu_running = false ; // 現在cpu空出來囉  
					} // else 
					
					countdown = time_slice ; // 重置time Slice
				} // if 
								
				if ( cpu_running == false && readyQ.empty() == false  ) {
					temp= readyQ.front() ; // 拿第一個出來工作 
					readyQ.pop() ;
					cpu_running = true ; 
					temp.cpu_burst = temp.cpu_burst - 1 ;
					if ( temp.pid > 9 ) {
						ch = temp.pid + 55 ; // 10 = A, 11 = B...
						list2 = list2 + ch ;
					} // if 
					else {
						ch = temp.pid + '0' ;
						list2 = list2 + ch ;
					} // else
					
					countdown-- ; 
				} // if 
				
				else if ( cpu_running == true ) {
					temp.cpu_burst = temp.cpu_burst - 1 ;
					if ( temp.pid > 9 ) {
						ch = temp.pid + 55 ; // 10 = A, 11 = B...
						list2 = list2 + ch ;
					} // if 
					else {
						ch = temp.pid + '0' ;
						list2 = list2 + ch ;
					} // else 
					
					countdown-- ;
				} // else if 
				
				if ( temp.cpu_burst == 0 ) { // process完成 
					temp.done_time = timer + 1 ;
					
					int index = 0 ;
					while ( index < sorted_list.size() ) { // 找到該process在sorted_list的位置，並把done_time放入 
						if ( temp.pid == sorted_list.at(index).pid ) {
							break ;
						} // if 
						else {
							index++ ;
						} // else 				
					} // while 
					
					sorted_list.at(index).done_time = temp.done_time ;
					cpu_running = false ; // 現在cpu空出來囉 
					countdown = time_slice ; // 重置time Slice
				} // if 			
			} // else 
		} // else 
		
		timer++ ;
	} // while 
	
 
} // RR() 

int CountRemaining( vector<Process> list ) {
	int i = 0 ;
	int min_index = 0 ;
	Process min_process = list.at(0) ;
	i++ ; 
	while ( i < list.size() ) {
		if ( list.at(i).cpu_burst < min_process.cpu_burst ) { // 剩餘時間小，換! 
			min_index = i ;
			min_process = list.at(i) ;
		} // if 
		else if ( list.at(i).cpu_burst == min_process.cpu_burst ) { // 剩餘時間一樣，比Arrival time 
			if ( list.at(i).arrival_time < min_process.arrival_time ) { // Arrival小，換!  
				min_index = i ;
				min_process = list.at(i) ;
			} // if 
			else if ( list.at(i).arrival_time == min_process.arrival_time ) { // 剩餘時間 Arrival時間一樣，比PID 
     			if ( list.at(i).pid < min_process.pid ) {
     				
     				min_index = i ;
					min_process = list.at(i) ;
				} // if 
			} // else if  
		} // else 
		i++ ;
	} // while 
	
	return min_index ;
} // CountRemaining()

void SRTF(string fileName, bool is_method_six) { // 最短剩餘時間 
	if ( is_method_six == false )
		Sort_By_Arrival_Time() ; // 將input_list依照Arrival_time排序	

	vector<Process> srtf = sorted_list ; // copy一份已排好的list 
	Process temp ; // 暫存正在run的process 
	vector<Process> ready_list ; // 所有正在等待工作的Process都存在這裡
	char ch = '\0' ;
	int timer = 0 ;
	bool done = false ;
	
	while ( done == false ) {
		if ( srtf.empty() && ready_list.empty() && temp.cpu_burst == 0 ) { // 全部都做完了  
			done = true ;
		} // if 
		else {
			if ( srtf.empty() == false && srtf.at(0).arrival_time > timer && ready_list.empty() ) { // 人還沒到 
				list3 = list3 + "-" ;
			} // if	
			else { // 有人 
				while ( srtf.empty() == false && srtf.at(0).arrival_time == timer ) {
					ready_list.push_back( srtf.at(0) ) ; // 新來的到ready list排隊
					srtf.erase( srtf.begin() ) ;				
				} // while 
				
				if ( ready_list.empty() == false ) {
					// min_index是ready_list中的最小process所在位置 
					int min_index = CountRemaining( ready_list ) ; // 找出ready_list中剩餘CPU_burst最小的process(的index)
					temp = ready_list.at(min_index) ; 
					
					temp.cpu_burst = temp.cpu_burst - 1 ;
					if ( temp.pid > 9 ) {
						ch = temp.pid + 55 ; // 10 = A, 11 = B...
						list3 = list3 + ch ;
					} // if 
					else {
						ch = temp.pid + '0' ;
						list3 = list3 + ch ;
					} // else 
									
					if ( temp.cpu_burst == 0 ) { // process完成 
						temp.done_time = timer + 1 ;
					
						int index = 0 ; // sorted_list的index 
						while ( index < sorted_list.size() ) { // 找到該process在sorted_list的位置，並把done_time放入 
							if ( temp.pid == sorted_list.at(index).pid ) {
								break ;
							} // if 
							else {
								index++ ;
							} // else 				
						} // while 
						
						sorted_list.at(index).done_time = temp.done_time ;
						
						ready_list.erase(ready_list.begin()+min_index) ; // 做完就離開ready_list 
					} // if 
					else {
						ready_list.at(min_index) = temp ; // 沒做完就放回去 
					} // else 					
				} // if 						 
			} // else 
		} // else 
		
		timer++ ;
	} // while done == false 
		
	
} // SRTF() 

void Sort_Multi_Q( Process p, bool &isPreempted ) { // 將 Process P 排入 Multi_Queue中 
	int i = 0 ; // vector的index 
	Node temp_Q ; // 等下要Push進vector
	Node cur_Q ; // 目前最小的Queue(正在執行的) 
	
	if ( multi_queue.empty() ) { // multi_queue是空的 
		temp_Q.q.push(p) ;
		temp_Q.priority = p.priority ;		
		multi_queue.push_back(temp_Q) ; // 直接推入vector 
	} // if 
	else { // multi_queue有東西 需要找到對應的QUEUE 或 按照Priority額外插入新的QUEUE 
		cur_Q = multi_queue.at(0) ;
		i= 0 ;
		while ( i < multi_queue.size() ) {
			if ( p.priority == multi_queue.at(i).priority ) { // priority相同 直接推入Queue即可 
				multi_queue.at(i).q.push(p) ;
				break ;
			} // if 
			else {
				i++ ;
			} // else 
		} // while
		
		if ( i == multi_queue.size() ) { // 要插入新的QUEUE 
			i = 0 ;
			while ( i < multi_queue.size() ) {
				if ( p.priority < multi_queue.at(i).priority ) { // 就插在(i)這裡!!! 
					temp_Q.q.push(p) ;
					temp_Q.priority = p.priority ;
					
					multi_queue.insert(multi_queue.begin()+i, temp_Q ) ; // 在i這個位置插入p 
					break ;	
				} // if
				 
				i++ ;
			} // while 
			
			if (  i == multi_queue.size() ) {
				temp_Q.q.push(p) ;
				temp_Q.priority = p.priority ;
					
				multi_queue.insert(multi_queue.begin()+i , temp_Q ) ; // 在最後面插入p 
			} // if 
		} // if 要插入新的QUEUE
		
		if ( cur_Q.priority > p.priority ) { // 會被奪取! 
			isPreempted = true ; 
		} // if 
		 
	} // else 
	

} // Sort_Multi_Q()

void Push_Back_Process( Process p ) { // 將Process p放回原Queue 最後面 

	int i = 0 ;
	while ( i < multi_queue.size() ) {
		if ( p.priority == multi_queue.at(i).priority ) { // 找到原Queue之後 
			multi_queue.at(i).q.pop();
			multi_queue.at(i).q.push(p) ; // push回去 
		} // if 
		i++ ;
	} // while 

} // Push_Back_Process()

void Push_Front_Process( Process p ) { // 將Process p放回原Queue最前面 
	queue<Process> temp_Q ;
	int i = 0 ;
	while ( i < multi_queue.size() ) {
		if ( p.priority == multi_queue.at(i).priority ) { // 找到原Queue之後
			int j = 0 ;
			temp_Q.push(p) ;
			multi_queue.at(i).q.pop() ;
			
			while ( multi_queue.at(i).q.size() > 0 ) {
				temp_Q.push(multi_queue.at(i).q.front() ) ;
				multi_queue.at(i).q.pop() ;
				
			} // while 

			break ;
		} // if 
		
		i++ ;
	} // while 

	multi_queue.at(i).q = temp_Q ;
} // Push_Front_Process()

void PPRR(string fileName, int time_slice, bool is_method_six ) {
	if ( is_method_six == false )
		Sort_By_Arrival_Time() ; // 將input_list依照Arrival_time排序
	vector<Process> pprr = sorted_list ; // copy一份已排好的list 
	Process temp ; // 暫存正在run的process 
	char ch = '\0' ;
	int timer = 0 ;
	bool done = false ;
	int countdown = time_slice ;
	bool isPreempted = false ; // 是否被奪取 
	while ( done == false ) {
		if ( pprr.empty() && multi_queue.empty() && temp.cpu_burst == 0 ) { // 全部都做完了  
			done = true ;
		} // if 
		else { // 尚未做完 
					
			if ( pprr.empty() == false && pprr.at(0).arrival_time > timer && multi_queue.empty() ) { // 人還沒到 
				list4 = list4 + "-" ;
			} // if
			else { // 有人 
				isPreempted = false ;
				// 有process到，到multi_Q排隊 順便判斷這次會不會被奪取 
				while ( pprr.empty() == false && pprr.at(0).arrival_time == timer ) { 
					Sort_Multi_Q( pprr.at(0), isPreempted ) ; // 將該 Process 排入 Multi_Queue中 
					pprr.erase(pprr.begin()) ;				
				} // while
				
				if ( multi_queue.empty() == false ) {
					
					// 當Time out或被奪取  換下一個process 並重置countdown 
					if ( countdown == 0 || isPreempted ) { 
					
						if ( temp.cpu_burst != 0 ) {
							// 將Process temp放回原Queue最後面
							Push_Back_Process( temp ) ;  
												
							// 重置時間 
							countdown = time_slice ;
						} // if 
							
					} // if 
					else { //其他狀況就直接放到queue的頭 
						
						if ( temp.cpu_burst != 0 ) {
							// 放回去Queue中 (先放回queue的最前面)
							Push_Front_Process( temp ) ; 
						} // if 
											
					} // else 
					
					// 每次都取 第一條Queue 的 第一個process 取出執行 
					Node first_Q = multi_queue.at(0) ; 
					temp = first_Q.q.front() ; 
					multi_queue.at(0).q.size() ;			
										
					// 執行 
					temp.cpu_burst = temp.cpu_burst - 1 ;
					if ( temp.pid > 9 ) {
						ch = temp.pid + 55 ; // 10 = A, 11 = B...
						list4 = list4 + ch ;
					} // if 
					else {
						ch = temp.pid + '0' ;
						list4 = list4 + ch ;
					} // else 
					
					
					// Process完成 
					if ( temp.cpu_burst == 0 ) { 	
						multi_queue.at(0).q.pop() ;	// 做完就滾!		
						temp.done_time = timer + 1 ;
							
						int index = 0 ; // sorted_list的index 
						
						// 找到該process在sorted_list的位置，並把done_time放入 
						while ( index < sorted_list.size() ) { 
							if ( temp.pid == sorted_list.at(index).pid ) {
								break ;
							} // if 
							else {
								index++ ;
							} // else 				
						} // while 
						
						 
						sorted_list.at(index).done_time = temp.done_time ;
						
						// 若這條Queue已經是空的 就移除整條Queue 
						if ( multi_queue.at(0).q.empty() ) {
							multi_queue.erase( multi_queue.begin() ) ; 
						} // if 			
						
						countdown = time_slice ; // 重置 
					} // if  Process完成 
					else { // Proess還沒做完
												
						countdown-- ;
						 
					} // else Proess還沒做完
			
				} // if 
				
			} // else 有人
		} // else 尚未做完 
		
		timer++ ;
	} // while 
	
	 
} // PPRR()

int Ratio_Calc( vector<Process> list, double timer ) {
	
	double max_ratio = 0.0 ;
	double temp_ratio = 0.0 ;
	int max_index = 0 ;
	int i = 0 ; // index of list 
	while ( i < list.size() ) {
		temp_ratio = ( timer - list.at(i).arrival_time ) / list.at(i).cpu_burst + 1 ;
		if ( temp_ratio > max_ratio ) {
			max_index = i ;
			max_ratio = temp_ratio ;
		} // if 
		
		i++ ;
	} // while 
	
	return max_index ;
} // Ratio_Calc()


void HRRN(string fileName, bool is_method_six) {
	if ( is_method_six == false )
		Sort_By_Arrival_Time() ; // 將input_list依照Arrival_time排序
		
	vector<Process> hrrn = sorted_list ; // copy一份已排好的list
	vector<Process> ready_list ; // 等待工作的list 
	Process temp ; // 暫存正在run的process 
	char ch = '\0' ;
	double timer = 0 ;
	bool done = false ;
	int max_index = 0 ;
	temp.cpu_burst = 0 ;
	while ( done == false ) {
		if ( hrrn.empty() && ready_list.empty() && temp.cpu_burst == 0 ) { // 全部都做完了  
			done = true ;
		} // if 
		else { // 尚未做完 
					
			if ( hrrn.empty() == false && hrrn.at(0).arrival_time > timer && ready_list.empty() ) { // 人還沒到 
				list5 = list5 + "-" ;
			} // if
			else { // 有人 
				while ( hrrn.empty() == false && hrrn.at(0).arrival_time == timer ) {
					ready_list.push_back( hrrn.at(0) ) ; // 新來的到ready list排隊
					hrrn.erase( hrrn.begin() ) ;				
				} // while 
				
				if ( ready_list.empty() == false ) {
					if ( temp.cpu_burst == 0 ) { // 換人做			
						max_index = Ratio_Calc( ready_list, timer ) ; // 找出要ready_list中要執行的process
						
					} // if
					
					temp = ready_list.at(max_index) ;
					// 執行 
					temp.cpu_burst = temp.cpu_burst - 1 ;
					if ( temp.pid > 9 ) {
						ch = temp.pid + 55 ; // 10 = A, 11 = B...
						list5 = list5 + ch ;
					} // if 
					else {
						ch = temp.pid + '0' ;
						list5 = list5 + ch ;
					} // else 
					
					// Process完成 
					if ( temp.cpu_burst == 0 ) {
						temp.done_time = timer + 1 ;
						
						int index = 0 ; // sorted_list的index 
						
						// 找到該process在sorted_list的位置，並把done_time放入 
						while ( index < sorted_list.size() ) { 
							if ( temp.pid == sorted_list.at(index).pid ) {
								break ;
							} // if 
							else {
								index++ ;
							} // else 				
						} // while 
						
						sorted_list.at(index).done_time = temp.done_time ;
						ready_list.erase(ready_list.begin()+max_index) ; // 做完就離開ready_list 
					} // if 
					else {
						ready_list.at(max_index) = temp ; // 沒做完就放回去 
					} // else 			
				} // if
			} // else 
		} // else 
		timer++ ;
	} // while 
	
	
	

} // HRRN()
// ----------------------------------------------------------------------------------------
int main() {
	string input_file_name ;
	cout << "請輸入要讀入的檔案名稱: " ;
	cout << "\n" ;
	cin >> input_file_name ;
	bool is_method_six = false ;
	int method_num, time_slice ;
	while ( !ReadFile(input_file_name, method_num, time_slice) ) cin >> input_file_name ;

    
	if ( method_num == 1 ) {
		FCFS( input_file_name ) ;
		Calc_time( 1 ) ; // 計算Turnaround time 及 waiting time 	
		WriteOutput( 1, input_file_name ) ; 
	} // if 
	else if ( method_num == 2 ) {
		RR( input_file_name, time_slice, is_method_six ) ; // round robin
		Calc_time( 2 ) ; // 計算Turnaround time 及 waiting time 	
		WriteOutput( 2, input_file_name ) ; 
	} // else if 
	else if ( method_num == 3 ) {
		SRTF(input_file_name, is_method_six ) ;
		Calc_time( 3 ) ; // 計算Turnaround time 及 waiting time 	
		WriteOutput( 3, input_file_name ) ; 
	} // else if
	else if ( method_num == 4 ) {
		PPRR( input_file_name, time_slice, is_method_six ) ;
		Calc_time( 4 ) ; // 計算Turnaround time 及 waiting time 	
		WriteOutput( 4, input_file_name ) ;
	} // else if
	else if ( method_num == 5 ) {
		HRRN(input_file_name, is_method_six ) ;
		Calc_time( 5 ) ; // 計算Turnaround time 及 waiting time 
		WriteOutput( 5, input_file_name ) ; 
	} // else if 
	else if ( method_num == 6 ) {
		is_method_six = true ;
		FCFS( input_file_name) ;
		Calc_time( 1 ) ; // 計算Turnaround time 及 waiting time 
		RR( input_file_name, time_slice, is_method_six ) ;
		Calc_time( 2 ) ; // 計算Turnaround time 及 waiting time 
		SRTF( input_file_name, is_method_six ) ;
		Calc_time( 3 ) ; // 計算Turnaround time 及 waiting time 
		PPRR( input_file_name, time_slice, is_method_six ) ;
		Calc_time( 4 ) ; // 計算Turnaround time 及 waiting time 
		HRRN( input_file_name, is_method_six ) ;
		Calc_time( 5 ) ; // 計算Turnaround time 及 waiting time 
		WriteOutput( 6, input_file_name ) ; 
	} // else if 
//	Sort() ; 
//	Debug() ;
	return 0;
}
