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
	int done_time ; // �����ɶ� 
};

struct Node{
	queue<Process> q ; // �ۦPpriority�b�P�@��Queue�� 
	int priority ;
};


vector<Process> input_list ;
vector<Process> sorted_list ; // �H�ƧǹL��list
vector<Node> multi_queue ; // PPRR�� 

// �p�⧹���ε��ݮɶ� 
vector<Process> fcfs_list ;
vector<Process> rr_list ;
vector<Process> srtf_list ;
vector<Process> pprr_list ;
vector<Process> hrrn_list ;

// �̯S�� 
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
		cout << "�ɮפ��s�b\n" ;
		return false ;
	} // if 
	else { // �}�lŪ�� 
		fscanf( pfile, "%d", &method_num ) ; 
		fscanf( pfile, "%d", &time_slice ) ; 
		char ch = '\0' ;
		fscanf( pfile, "%c", &ch ) ; // Ū���� 
		fscanf( pfile, "%c", &ch ) ;
		while ( ch != '\n')   //Ū���ĤG��r (ID  CPU Burst  Arrival Time   Priority) 
			ch = getc( pfile ) ;
		
		int temp = -1 ; // Ū�J���F��Ȧs
		Process p ; // Ū�J���F��Ȧs�o�̡A������Jvector�� 
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


void Sort_By_Arrival_Time() { // �Ninput_list�̷�Arrival_time�Ƨ� 
	int i = 0 ;
	int j = 0 ;
	vector<Process> input_list_temp = input_list;
	Process ptemp = input_list.at(0) ; // �Ȧs�ثe�̤p�� 
	int temp_min_index ;

	
	while( i < input_list_temp.size() ) {
		ptemp = input_list_temp.at(0) ;
		j = 0 ;
		temp_min_index = j ;
		while ( j < input_list_temp.size() ) { // �q�Y�]��� 
			if ( input_list_temp.at(j).arrival_time < ptemp.arrival_time ) { // ���p �s�Jptemp�� 
				ptemp = input_list_temp.at(j) ;
				temp_min_index = j ;
			} // if 
			else if ( input_list_temp.at(j).arrival_time == ptemp.arrival_time ) { // �@�ˤj -> ��PID 
				if ( input_list_temp.at(j).pid < ptemp.pid ) { // ���p �s�Jptemp��  
					ptemp = input_list_temp.at(j) ;
					temp_min_index = j ;
				} // if 
			} // else if
			
			j++ ; 
		} // while 
		sorted_list.push_back(ptemp) ; // �T�{�n�̤p���N���Jlist�� 
		input_list_temp.erase(input_list_temp.begin()+temp_min_index) ;
		
	} // while 
} // Sort_By_Arrival_Time()

void Calc_time( int num ) {
	// ����PID�Ƨ� 
	vector<Process> temp_list = sorted_list ; // ����PID�Ƨ� �Ȧs��list 
	int i = 0 ;
	int j = 0 ;
	int temp_min_index ;
	Process temp_to_swap ; // �洫�Ϊ�temp 
	
	while( i < temp_list.size() ) {
		j = i ;
		temp_min_index = j ;
		while ( j < temp_list.size() ) { // �q�Y�]��� 
			if ( temp_list.at(j).pid < temp_list.at(temp_min_index).pid ) { // �ѤU���̤p�� 
				temp_min_index = j ;
			} // if 
			
			j++ ; 
		} // while 
		
		// SWAP 
		temp_to_swap = temp_list.at(i) ;
		temp_list.at(i) = temp_list.at(temp_min_index) ;
		temp_list.at(temp_min_index) = temp_to_swap ;
		
		// �p��Turnaround time
		temp_list.at(i).turnaround = temp_list.at(i).done_time - temp_list.at(i).arrival_time ;
		
		// �p��Waiting time 
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
	Sort_By_Arrival_Time() ; // �Ninput_list�̷�Arrival_time�Ƨ�
	// Debug() ; 
	vector<Process> fcfs = sorted_list ; // copy�@���w�Ʀn��list 
	int timer = 0 ;
	char ch = '\0' ;
	int i = 0 ; // sorted_list��index 
	while ( !fcfs.empty() ) {
		if ( fcfs.at(0).arrival_time > timer ) { // �H�٨S�� 
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
			if ( fcfs.at(0).cpu_burst == 0 ) { // �����N���ںu  
				sorted_list.at(i).done_time = timer + 1; // �����ɶ�  (+1: �]���ثe�����ɶ��٨S��)
				fcfs.erase( fcfs.begin() ) ;
				i++ ; // ���U�@�ӤH�� 
			} // if 
		} // else 
		
		timer++ ;
	} // while 
	
} // FCFS()
// ------------------------------------------------------------------------------------ RR
void RR( string fileName, int time_slice, bool is_method_six) {
	if ( is_method_six == false )
		Sort_By_Arrival_Time() ; // �Ninput_list�̷�Arrival_time�Ƨ�
	vector<Process> rr = sorted_list ; // copy�@���w�Ʀn��list 
	Process temp ; // �Ȧs���brun��process 
	queue<Process> readyQ ; // Ready Queue
	int timer = 0 ;
	int countdown = time_slice ; // �˼�timeSlice
	bool cpu_running = false ;
	char ch = '\0' ;
	bool done = false ;
	
	while( done == false ) {

		if ( rr.empty() && readyQ.empty() && temp.cpu_burst == 0 ) { // �����������F  
			done = true ;
		} // if 
		else {
					
			if ( rr.empty() == false && rr.at(0).arrival_time > timer && cpu_running == false && readyQ.empty() ) { // �H�٨S�� 
				list2 = list2 + "-" ;
			} // if 
			else {
				while ( rr.empty() == false && rr.at(0).arrival_time == timer ) {
					readyQ.push(rr.at(0)) ; // �s�Ӫ���ready queue�ƶ�
					rr.erase(rr.begin()) ;
					
				} // while 
				
				if ( countdown == 0 ) { // time Slice�Χ� 
					if ( temp.cpu_burst == 0 ) { // process���� 
						;				 
					} // if 
					else {
						readyQ.push(temp) ; // �٨S�����N���^�h 
						cpu_running = false ; // �{�bcpu�ťX���o  
					} // else 
					
					countdown = time_slice ; // ���mtime Slice
				} // if 
								
				if ( cpu_running == false && readyQ.empty() == false  ) {
					temp= readyQ.front() ; // ���Ĥ@�ӥX�Ӥu�@ 
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
				
				if ( temp.cpu_burst == 0 ) { // process���� 
					temp.done_time = timer + 1 ;
					
					int index = 0 ;
					while ( index < sorted_list.size() ) { // ����process�bsorted_list����m�A�ç�done_time��J 
						if ( temp.pid == sorted_list.at(index).pid ) {
							break ;
						} // if 
						else {
							index++ ;
						} // else 				
					} // while 
					
					sorted_list.at(index).done_time = temp.done_time ;
					cpu_running = false ; // �{�bcpu�ťX���o 
					countdown = time_slice ; // ���mtime Slice
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
		if ( list.at(i).cpu_burst < min_process.cpu_burst ) { // �Ѿl�ɶ��p�A��! 
			min_index = i ;
			min_process = list.at(i) ;
		} // if 
		else if ( list.at(i).cpu_burst == min_process.cpu_burst ) { // �Ѿl�ɶ��@�ˡA��Arrival time 
			if ( list.at(i).arrival_time < min_process.arrival_time ) { // Arrival�p�A��!  
				min_index = i ;
				min_process = list.at(i) ;
			} // if 
			else if ( list.at(i).arrival_time == min_process.arrival_time ) { // �Ѿl�ɶ� Arrival�ɶ��@�ˡA��PID 
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

void SRTF(string fileName, bool is_method_six) { // �̵u�Ѿl�ɶ� 
	if ( is_method_six == false )
		Sort_By_Arrival_Time() ; // �Ninput_list�̷�Arrival_time�Ƨ�	

	vector<Process> srtf = sorted_list ; // copy�@���w�Ʀn��list 
	Process temp ; // �Ȧs���brun��process 
	vector<Process> ready_list ; // �Ҧ����b���ݤu�@��Process���s�b�o��
	char ch = '\0' ;
	int timer = 0 ;
	bool done = false ;
	
	while ( done == false ) {
		if ( srtf.empty() && ready_list.empty() && temp.cpu_burst == 0 ) { // �����������F  
			done = true ;
		} // if 
		else {
			if ( srtf.empty() == false && srtf.at(0).arrival_time > timer && ready_list.empty() ) { // �H�٨S�� 
				list3 = list3 + "-" ;
			} // if	
			else { // ���H 
				while ( srtf.empty() == false && srtf.at(0).arrival_time == timer ) {
					ready_list.push_back( srtf.at(0) ) ; // �s�Ӫ���ready list�ƶ�
					srtf.erase( srtf.begin() ) ;				
				} // while 
				
				if ( ready_list.empty() == false ) {
					// min_index�Oready_list�����̤pprocess�Ҧb��m 
					int min_index = CountRemaining( ready_list ) ; // ��Xready_list���ѾlCPU_burst�̤p��process(��index)
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
									
					if ( temp.cpu_burst == 0 ) { // process���� 
						temp.done_time = timer + 1 ;
					
						int index = 0 ; // sorted_list��index 
						while ( index < sorted_list.size() ) { // ����process�bsorted_list����m�A�ç�done_time��J 
							if ( temp.pid == sorted_list.at(index).pid ) {
								break ;
							} // if 
							else {
								index++ ;
							} // else 				
						} // while 
						
						sorted_list.at(index).done_time = temp.done_time ;
						
						ready_list.erase(ready_list.begin()+min_index) ; // �����N���}ready_list 
					} // if 
					else {
						ready_list.at(min_index) = temp ; // �S�����N��^�h 
					} // else 					
				} // if 						 
			} // else 
		} // else 
		
		timer++ ;
	} // while done == false 
		
	
} // SRTF() 

void Sort_Multi_Q( Process p, bool &isPreempted ) { // �N Process P �ƤJ Multi_Queue�� 
	int i = 0 ; // vector��index 
	Node temp_Q ; // ���U�nPush�ivector
	Node cur_Q ; // �ثe�̤p��Queue(���b���檺) 
	
	if ( multi_queue.empty() ) { // multi_queue�O�Ū� 
		temp_Q.q.push(p) ;
		temp_Q.priority = p.priority ;		
		multi_queue.push_back(temp_Q) ; // �������Jvector 
	} // if 
	else { // multi_queue���F�� �ݭn��������QUEUE �� ����Priority�B�~���J�s��QUEUE 
		cur_Q = multi_queue.at(0) ;
		i= 0 ;
		while ( i < multi_queue.size() ) {
			if ( p.priority == multi_queue.at(i).priority ) { // priority�ۦP �������JQueue�Y�i 
				multi_queue.at(i).q.push(p) ;
				break ;
			} // if 
			else {
				i++ ;
			} // else 
		} // while
		
		if ( i == multi_queue.size() ) { // �n���J�s��QUEUE 
			i = 0 ;
			while ( i < multi_queue.size() ) {
				if ( p.priority < multi_queue.at(i).priority ) { // �N���b(i)�o��!!! 
					temp_Q.q.push(p) ;
					temp_Q.priority = p.priority ;
					
					multi_queue.insert(multi_queue.begin()+i, temp_Q ) ; // �bi�o�Ӧ�m���Jp 
					break ;	
				} // if
				 
				i++ ;
			} // while 
			
			if (  i == multi_queue.size() ) {
				temp_Q.q.push(p) ;
				temp_Q.priority = p.priority ;
					
				multi_queue.insert(multi_queue.begin()+i , temp_Q ) ; // �b�̫᭱���Jp 
			} // if 
		} // if �n���J�s��QUEUE
		
		if ( cur_Q.priority > p.priority ) { // �|�Q�ܨ�! 
			isPreempted = true ; 
		} // if 
		 
	} // else 
	

} // Sort_Multi_Q()

void Push_Back_Process( Process p ) { // �NProcess p��^��Queue �̫᭱ 

	int i = 0 ;
	while ( i < multi_queue.size() ) {
		if ( p.priority == multi_queue.at(i).priority ) { // ����Queue���� 
			multi_queue.at(i).q.pop();
			multi_queue.at(i).q.push(p) ; // push�^�h 
		} // if 
		i++ ;
	} // while 

} // Push_Back_Process()

void Push_Front_Process( Process p ) { // �NProcess p��^��Queue�̫e�� 
	queue<Process> temp_Q ;
	int i = 0 ;
	while ( i < multi_queue.size() ) {
		if ( p.priority == multi_queue.at(i).priority ) { // ����Queue����
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
		Sort_By_Arrival_Time() ; // �Ninput_list�̷�Arrival_time�Ƨ�
	vector<Process> pprr = sorted_list ; // copy�@���w�Ʀn��list 
	Process temp ; // �Ȧs���brun��process 
	char ch = '\0' ;
	int timer = 0 ;
	bool done = false ;
	int countdown = time_slice ;
	bool isPreempted = false ; // �O�_�Q�ܨ� 
	while ( done == false ) {
		if ( pprr.empty() && multi_queue.empty() && temp.cpu_burst == 0 ) { // �����������F  
			done = true ;
		} // if 
		else { // �|������ 
					
			if ( pprr.empty() == false && pprr.at(0).arrival_time > timer && multi_queue.empty() ) { // �H�٨S�� 
				list4 = list4 + "-" ;
			} // if
			else { // ���H 
				isPreempted = false ;
				// ��process��A��multi_Q�ƶ� ���K�P�_�o���|���|�Q�ܨ� 
				while ( pprr.empty() == false && pprr.at(0).arrival_time == timer ) { 
					Sort_Multi_Q( pprr.at(0), isPreempted ) ; // �N�� Process �ƤJ Multi_Queue�� 
					pprr.erase(pprr.begin()) ;				
				} // while
				
				if ( multi_queue.empty() == false ) {
					
					// ��Time out�γQ�ܨ�  ���U�@��process �í��mcountdown 
					if ( countdown == 0 || isPreempted ) { 
					
						if ( temp.cpu_burst != 0 ) {
							// �NProcess temp��^��Queue�̫᭱
							Push_Back_Process( temp ) ;  
												
							// ���m�ɶ� 
							countdown = time_slice ;
						} // if 
							
					} // if 
					else { //��L���p�N�������queue���Y 
						
						if ( temp.cpu_burst != 0 ) {
							// ��^�hQueue�� (����^queue���̫e��)
							Push_Front_Process( temp ) ; 
						} // if 
											
					} // else 
					
					// �C������ �Ĥ@��Queue �� �Ĥ@��process ���X���� 
					Node first_Q = multi_queue.at(0) ; 
					temp = first_Q.q.front() ; 
					multi_queue.at(0).q.size() ;			
										
					// ���� 
					temp.cpu_burst = temp.cpu_burst - 1 ;
					if ( temp.pid > 9 ) {
						ch = temp.pid + 55 ; // 10 = A, 11 = B...
						list4 = list4 + ch ;
					} // if 
					else {
						ch = temp.pid + '0' ;
						list4 = list4 + ch ;
					} // else 
					
					
					// Process���� 
					if ( temp.cpu_burst == 0 ) { 	
						multi_queue.at(0).q.pop() ;	// �����N�u!		
						temp.done_time = timer + 1 ;
							
						int index = 0 ; // sorted_list��index 
						
						// ����process�bsorted_list����m�A�ç�done_time��J 
						while ( index < sorted_list.size() ) { 
							if ( temp.pid == sorted_list.at(index).pid ) {
								break ;
							} // if 
							else {
								index++ ;
							} // else 				
						} // while 
						
						 
						sorted_list.at(index).done_time = temp.done_time ;
						
						// �Y�o��Queue�w�g�O�Ū� �N�������Queue 
						if ( multi_queue.at(0).q.empty() ) {
							multi_queue.erase( multi_queue.begin() ) ; 
						} // if 			
						
						countdown = time_slice ; // ���m 
					} // if  Process���� 
					else { // Proess�٨S����
												
						countdown-- ;
						 
					} // else Proess�٨S����
			
				} // if 
				
			} // else ���H
		} // else �|������ 
		
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
		Sort_By_Arrival_Time() ; // �Ninput_list�̷�Arrival_time�Ƨ�
		
	vector<Process> hrrn = sorted_list ; // copy�@���w�Ʀn��list
	vector<Process> ready_list ; // ���ݤu�@��list 
	Process temp ; // �Ȧs���brun��process 
	char ch = '\0' ;
	double timer = 0 ;
	bool done = false ;
	int max_index = 0 ;
	temp.cpu_burst = 0 ;
	while ( done == false ) {
		if ( hrrn.empty() && ready_list.empty() && temp.cpu_burst == 0 ) { // �����������F  
			done = true ;
		} // if 
		else { // �|������ 
					
			if ( hrrn.empty() == false && hrrn.at(0).arrival_time > timer && ready_list.empty() ) { // �H�٨S�� 
				list5 = list5 + "-" ;
			} // if
			else { // ���H 
				while ( hrrn.empty() == false && hrrn.at(0).arrival_time == timer ) {
					ready_list.push_back( hrrn.at(0) ) ; // �s�Ӫ���ready list�ƶ�
					hrrn.erase( hrrn.begin() ) ;				
				} // while 
				
				if ( ready_list.empty() == false ) {
					if ( temp.cpu_burst == 0 ) { // ���H��			
						max_index = Ratio_Calc( ready_list, timer ) ; // ��X�nready_list���n���檺process
						
					} // if
					
					temp = ready_list.at(max_index) ;
					// ���� 
					temp.cpu_burst = temp.cpu_burst - 1 ;
					if ( temp.pid > 9 ) {
						ch = temp.pid + 55 ; // 10 = A, 11 = B...
						list5 = list5 + ch ;
					} // if 
					else {
						ch = temp.pid + '0' ;
						list5 = list5 + ch ;
					} // else 
					
					// Process���� 
					if ( temp.cpu_burst == 0 ) {
						temp.done_time = timer + 1 ;
						
						int index = 0 ; // sorted_list��index 
						
						// ����process�bsorted_list����m�A�ç�done_time��J 
						while ( index < sorted_list.size() ) { 
							if ( temp.pid == sorted_list.at(index).pid ) {
								break ;
							} // if 
							else {
								index++ ;
							} // else 				
						} // while 
						
						sorted_list.at(index).done_time = temp.done_time ;
						ready_list.erase(ready_list.begin()+max_index) ; // �����N���}ready_list 
					} // if 
					else {
						ready_list.at(max_index) = temp ; // �S�����N��^�h 
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
	cout << "�п�J�nŪ�J���ɮצW��: " ;
	cout << "\n" ;
	cin >> input_file_name ;
	bool is_method_six = false ;
	int method_num, time_slice ;
	while ( !ReadFile(input_file_name, method_num, time_slice) ) cin >> input_file_name ;

    
	if ( method_num == 1 ) {
		FCFS( input_file_name ) ;
		Calc_time( 1 ) ; // �p��Turnaround time �� waiting time 	
		WriteOutput( 1, input_file_name ) ; 
	} // if 
	else if ( method_num == 2 ) {
		RR( input_file_name, time_slice, is_method_six ) ; // round robin
		Calc_time( 2 ) ; // �p��Turnaround time �� waiting time 	
		WriteOutput( 2, input_file_name ) ; 
	} // else if 
	else if ( method_num == 3 ) {
		SRTF(input_file_name, is_method_six ) ;
		Calc_time( 3 ) ; // �p��Turnaround time �� waiting time 	
		WriteOutput( 3, input_file_name ) ; 
	} // else if
	else if ( method_num == 4 ) {
		PPRR( input_file_name, time_slice, is_method_six ) ;
		Calc_time( 4 ) ; // �p��Turnaround time �� waiting time 	
		WriteOutput( 4, input_file_name ) ;
	} // else if
	else if ( method_num == 5 ) {
		HRRN(input_file_name, is_method_six ) ;
		Calc_time( 5 ) ; // �p��Turnaround time �� waiting time 
		WriteOutput( 5, input_file_name ) ; 
	} // else if 
	else if ( method_num == 6 ) {
		is_method_six = true ;
		FCFS( input_file_name) ;
		Calc_time( 1 ) ; // �p��Turnaround time �� waiting time 
		RR( input_file_name, time_slice, is_method_six ) ;
		Calc_time( 2 ) ; // �p��Turnaround time �� waiting time 
		SRTF( input_file_name, is_method_six ) ;
		Calc_time( 3 ) ; // �p��Turnaround time �� waiting time 
		PPRR( input_file_name, time_slice, is_method_six ) ;
		Calc_time( 4 ) ; // �p��Turnaround time �� waiting time 
		HRRN( input_file_name, is_method_six ) ;
		Calc_time( 5 ) ; // �p��Turnaround time �� waiting time 
		WriteOutput( 6, input_file_name ) ; 
	} // else if 
//	Sort() ; 
//	Debug() ;
	return 0;
}
