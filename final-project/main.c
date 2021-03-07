#include <stdio.h>
#include <Windows.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "hmm.h"

/*	
	f 0, k 1, n 2, r 3, s 4, t 5, 
	v 6, w 7, z 8, ah 9, ao 10, ay 11, eh 12, 
	ey 13, ih 14, iy 15, ow 16, th 17, uw 18, sil 19, sp 20  total 21   
*/

typedef struct {
	int lines;
	int dimensions;
	double** sequence_data;
}Sequences;

// Files IO
Sequences* read_one_file(char* file_path);
void* print_given_data(Sequences* sequence);
double char_to_double(char* list);
double prob_of_phone
(hmmType* phones, char* word, int state, Sequences* input, int start);

// List of the Basic functions controlling the hmm data
int change_word_to_int(char* word);
char* change_int_to_word(int number);

int print_name_List(hmmType* phones);
int print_tp(hmmType* phones, char* word);
int print_pdf(hmmType* phones, char* word, int state, int pdf);

// About Interpretation of Phonym
double prob_of_given_data_dimension(hmmType* phones, double* data, char* word, int number);

char* viterbi_algorithm(hmmType* phones, Sequences* input);
double calculated_sequence(hmmType* phones, Sequences* input, int i);

int main(void) {
	// Local Variables Declaration
	hmmType* hmm = phones;
	double probability;

	// Given Data
	char test_path[] = "./test_data.txt";

	Sequences* files = read_one_file(test_path);

	//print_given_data(files);
	
	//double temp_list[] = {-1.391818e+001, 9.337726e-001,-8.008668e-001,-2.249418e+000,3.061608e-001,-1.821656e+000,-4.268857e+000,-1.597530e+000,-9.981467e-001,-8.185837e-001,-1.302086e+000, 1.086287e-001, 3.095545e+001,-1.509245e+000, 7.043715e-001, 7.474790e-001,  3.141684e+000, -4.023337e+000 ,-5.991039e-001,  2.184176e+000,  3.418445e+000,  2.143303e+000, -4.632097e-001,  1.130623e+000,  4.160827e-001, -1.395662e+000,  1.700599e+000, -6.367068e-001, -2.060893e+000, -8.313527e+000,  1.161764e+000,  4.755530e-001, -5.936308e-001, -9.339354e+000, -6.520984e+000, -6.333081e-001,  2.233563e+000,  1.056194e+000,  2.155071e+000 };
	//double temp_prob = prob_of_given_data_dimension(hmm, temp_list, "f", 0);

	//probability = prob_of_phone(phones, "f", 1, files,0);

	//printf("%1.6e \n", probability);

	viterbi_algorithm(phones, files);
	
	//Body of the Main
	//print_name_List(hmm);
	//print_tp(hmm, "sp");
	//print_pdf(hmm, "sp", 2, 3);

	//Terminate
	system("pause");
	return 0;

}// main


char* viterbi_algorithm(hmmType* phones, Sequences* input) {
	double probability;
	double compare = 0;

	for (int i = 0; i < 21; i++) {
		
		probability = calculated_sequence(phones, input, i);

		if (compare < probability) {
			compare = probability;
		}
	}

	return 0;
}

double calculated_sequence(hmmType* phones, Sequences* input, int i){

	double before = 0;
	double after = 0;
	double compare = 0;
	double inway = 0;
	double outway = 0;

	int word_num = 0;
	int startpoint = 0;
	int state_num = 0;

	if (i == 20) {
		for ( state_num = 0; state_num < 2; state_num++) {
			if (state_num != 0) {
				state_num = 0;
				
				before = prob_of_phone(phones, change_int_to_word(i), state_num, input, startpoint);
				before *= phones[i].tp[state_num + 1][state_num + 1];

				after = prob_of_phone(phones, change_int_to_word(i), state_num, input, startpoint);
				after *= phones[i].tp[state_num + 1][state_num + 2];
								
				if (after > before) {
					compare = prob_of_phone(phones, change_int_to_word(word_num), state_num, input, startpoint);
					break;
				}
				else {
					startpoint++;
					continue;
				}

			}// �ٽõ��ƿ� �༮
			state_num = 0;

			inway = prob_of_phone(phones, change_int_to_word(i), state_num, input, startpoint);
			inway *= phones[i].tp[state_num][state_num + 1];

			outway = prob_of_phone(phones, change_int_to_word(i), state_num, input, startpoint);
			outway *= phones[i].tp[state_num][state_num + 2];

			if (outway > inway) {
				compare = prob_of_phone(phones, change_int_to_word(word_num), state_num, input, startpoint);
				break;
			}
			startpoint++;

		}//for
	}
	else {
		for ( state_num = 0; state_num < 3; state_num++) {

			while (1) {
				
				before = prob_of_phone(phones, change_int_to_word(i), state_num, input, startpoint);
				printf("before : %1.6f \n", before);
				before *= phones[i].tp[state_num + 1][state_num + 1];

				after = prob_of_phone(phones, change_int_to_word(i), state_num, input, startpoint);
				after *= phones[i].tp[state_num + 1][state_num + 2];

				
				printf("after : %1.6f \n", after);
				getchar();
				if (after > before) {
					if (state_num == 2) {
						compare = prob_of_phone(phones, change_int_to_word(word_num), state_num, input, startpoint);

					}
					
					startpoint++;
					break;
				}
				startpoint++;
			}//while
		}//for
	}

	printf("%d %d %d %1.6e \n", state_num ,startpoint ,i, compare);
	return 0;
}

double prob_of_phone
(hmmType* phones, char* word, int state, Sequences* input, int start) {
	int endpoint = 10;
	if (start > input->lines){
		return -1;
	}

	int order = change_word_to_int(word);
	if (order == -1) {
		printf("there is no words\n");
		return -1;
	}//if

	double* probability = (double*)malloc(sizeof(double)*N_DIMENSION);

	double* result = probability;
	double x = 0;
	double prob = 0;
	double normal_value = 0;
	
	double mean = 0;
	double var = 0;
	double weight = 0;
	double prob_of_probs = 1;
	
	for (int dimension = 0; dimension < N_DIMENSION; dimension++){
		for (int pdf = 0; pdf < 10; pdf++) {
			x = input->sequence_data[start][dimension];

			mean = phones[order].state[state].pdf[pdf].mean[dimension];
			var = phones[order].state[state].pdf[pdf].var[dimension];
			weight = phones[order].state[state].pdf[pdf].weight;

			normal_value = (1 / sqrt(var)*sqrt(2*3.14)) * exp(-(pow((x-mean),2)/2*var));
			
			prob += normal_value*weight;// �� pdf �� ��
		}
		result[dimension] = prob;
		prob = 0; //�ʱ�ȭ
	}

	for (int i = 0; i < N_DIMENSION; i++) {
		if (result[i] == 0) {
			continue;
		}//
		prob_of_probs += log(result[i]);
	}	

	free(probability);
	return prob_of_probs;
}

void* print_given_data(Sequences* sequence) {
	
	Sequences* temp = sequence;

	printf("lines : %d dimensions :%d \n", temp->lines, temp->dimensions);
	for (int i = 0; i < temp->lines; i++) {
		for (int a = 0; a < temp->dimensions; a++) {
			printf("%1.6e ", temp->sequence_data[i][a]);
		}
		printf("\n");
	}
	return 0;
}

double char_to_double(char* list) {
	
	double result = 0;
	double exponential = 0;

	result = list[1] - 48;
	for (int i = 0; i < 6; i++) {
		result += (list[i + 3] - 48) / pow(10, i+1);
	}
	if (list[0] == '-') {
		result *= (-1);
	}
	
	for (int i = 0; i < 3;  i++) {
		exponential += (list[i + 11]-48) * pow(10,2-i);
	}

	if (list[10] == '-') {
		exponential *= (-1);
	}
	result *= pow(10,exponential);
	
	return result;
}

Sequences* read_one_file(char* file_path) {
	
	FILE* fp;

	char buf[16];
	char bufnum[585];

	char number[15];
	
	number[14] = '\0';
	double temp;
	double** sequence;

	int lines = 0;
	int dimensions = 0;
	int cnt = 0;
	int a = 0;
	int b = 0;
	int process_lines = 0;

	int sequence_x = 0;
	int sequence_y = 0;

	fopen_s(&fp, file_path, "rt");
	
	// read the numbers of sequence
	fgets(buf, 4, fp);
	for (int i = 0; i < 3; i++) {
		lines += (buf[i]-48) * pow(10,2-i);
	}

	// read the numbers of dimensions
	fgets(buf, 4, fp);
	for (int i = 0; i < 3; i++) {
		if (buf[i] != ' ') {
			dimensions += (buf[i] - 48) * pow(10, 2 - i);
		}
	}
	Sequences* result = (Sequences*)malloc(sizeof(Sequences));
	result->dimensions = dimensions;
	result->lines = lines;

	sequence = (double**)malloc(sizeof(double*)*lines);
	for (int i = 0; i < lines; i++) {
		sequence[i] = (double*)malloc(sizeof(double)*dimensions);
	}
	while (fgets(buf, 2, fp) != EOF)
	{
		cnt = 0;
		if (process_lines > lines) {
			break;
		}
		while (buf[0] != '\n') {
			bufnum[cnt] = buf[0];
			//printf("%c", bufnum[cnt]);
			fgets(buf, 2, fp);
			cnt++;
		}
		bufnum[584] ='\0';
		sequence_y = 0;
		for (int a = 0; a < 584; a++) {
			if (cnt == 0) {
				break;
			}
			if (a % 15 == 0) {
				for (b = 0; b < 15; b++) {
					number[b] = bufnum[a+b];
				}
				temp = char_to_double(number);
				sequence[sequence_x-1][sequence_y] = temp;
				sequence_y++;
			}// �Ѻ�����
		}
		sequence_x++;
		process_lines++;
	}
	result->sequence_data = sequence;
	fclose(fp);
	return result;
}// read_one_files

char* change_int_to_word(int number) {
	char result = 0;
	char* wordlist[] = {"f","k","n","r","s","t","v","w","z","ah","ao","ay","eh",
		"ey","ih","iy","ow","th","uw","sil","sp"};

	for (int i = 0; i < 21; i++) {
		if (strcmp(wordlist[i], wordlist[number]) == 0) {
			return wordlist[i];
		}
	}
	return -1;
}

int change_word_to_int(char* word) {
	
	// f 0, k 1, n 2, r 3, s 4, t 5, 
	// v 6, w 7, z 8, ah 9, ao 10, ay 11, eh 12, 
	// ey 13, ih 14, iy 15, ow 16, th 17, uw 18, sil 19, sp 20  total 21
	
	char* wordlist[] = {"f","k","n","r","s","t","v","w","z","ah","ao","ay","eh",
		"ey","ih","iy","ow","th","uw","sil","sp" };
	
	for (int i = 0; i < 21; i++) {
		if (strcmp(word, wordlist[i]) == 0) {
			return i; //corresponding number to input word
		}//if
	}//for

	return -1; // there is no corresponding word
}//change_word_to_int

int print_name_List(hmmType* phones) {

	printf("The name list\n");

	for (int i = 0; i < 21; i++){
		printf("%s ",phones[i].name);
		if (i % 11 == 0 && i !=0) {
			printf("\n");
		}//if
	}//for

	printf("\n");
	return 0;
}//print_name_List

int print_tp(hmmType* phones, char* word){
	int number = 0;
	int order = change_word_to_int(word);
	if (order == -1) {
		printf("there is no words\n");
		return -1;
	}//if
	if (order == 20) {
		number = 3;
	}
	else {
		number = 5;
	}
	printf("\nTransition Probability of '%s' is.. \n",  phones[order].name);
	for (int i = 0; i < number; i++) {
		for (int j = 0; j < number; j++) {
			printf("%f ", phones[order].tp[i][j]);
		}//for
		printf("\n");
	}//for
	printf("\n");
	return 0;

}//print_tp

int print_pdf(hmmType* phones, char* word, int state, int pdf) {
	
	if (state > 3 || state < 1) {
		return -1;
	}//if
	if (pdf > 10 || pdf < 1) {
		return -1;
	}//if

	int order = change_word_to_int(word);
	if (order == -1) {
		printf("there is no words\n");
		return -1;
	}//if

	printf("Info of %d pdf of %s in %d state ..\n\n", pdf, word, state);
	printf("Weight is ...%f\n", phones[order].state[state].pdf[pdf].weight);
	
	printf("Means are ..\n");
	for (int i = 0; i < N_DIMENSION; i++) {
		printf("%f ", phones[order].state[state].pdf[pdf].mean[i]);
		if(i%10 == 0 && i != 0){
			printf("\n");
		}//if
	}//for
	
	printf("\n\n");

	printf("Variances are ..\n");
	for (int i = 0; i < N_DIMENSION; i++) {
		printf("%f ", phones[order].state[state].pdf[pdf].var[i]);
		if (i % 10 == 0 && i != 0 ) {
			printf("\n");
		}//if
	}//for

	printf("\n\n");
	return 0;
}//print_pdf

double prob_of_given_data_dimension(hmmType* phones, double* data, char* word,int number) {

	int order = change_word_to_int(word);
	if (order == -1) {
		printf("there is no words\n");
		return -1;
	}//if
	return 0;
}
