#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LINES 10000
#define CHAR 200

struct instruction_s {
	int step;
	int index;
};

void read_book(FILE* ptr, char** lines) {
	for (int i = 0; i < LINES; i++) {
		fgets(lines[i], CHAR * sizeof(char), ptr);
	}
}

int number_of_lines(FILE* ptr) {
	int count = 0;
	char x;
	while(!feof(ptr)) {
		x = getc(ptr);
		if (x == '\n') {
			count++;
		}
	}
	return count;
}

void read_instructions(FILE* ptr, struct instruction_s* instructions, int size_instructions) {
	rewind(ptr);
	for (int i = 0; i < size_instructions; i++) {
		fscanf(ptr,"%d %d", &instructions[i].step, &instructions[i].index);
	}
}

char* get_word(char* s, int n) {
	char* word =(char*) malloc(CHAR * sizeof(char));
	int count = 1;
	int i = 0;
	
	while (s[i] != '\0') {
		if (s[i] == ' ') {
			count++;
		}
		if (n == count) {
			if (i != 0) {
				i++;
			}
			int x = 0;
			while(s[i] != ' ' && s[i] != '\0' && s[i] != '\n') {
				word[x] = s[i];
				x++;
				i++;
			}
			word[x] = '\0';
			break;
		}
		i++;
	}
	return word;
}

void get_sentence(char** lines, struct instruction_s* instructions, int n_instructions, char* sentence) {
	int total = -1;
	for (int i = 0; i < n_instructions; i++) {
		total += instructions[i].step;
	}
	strcpy(sentence, lines[total]);
}

void no_punctuation(char* word) {
	int i = 0;
	while(word[i] != '\0') {
		int x = word[i];
		if ((x >= 33 && x <= 47) || (x >= 58 && x <= 64) || (x >= 91 && x <= 96) || (x >= 123 && x <= 126) ) {
			for (int j = i; word[j - 1] != '\0'; j++) {
				word[j] = word[j + 1];
			}
		}
		i++;
	}
}

void str_lower(char* word) {
	int i = 0;
	while(word[i] != '\0') {
		int x = word[i];
		if(x >= 65 && x <= 90) {
			x += 32;
			word[i] = x;
		}
		i++;
	}
}

int main(int argc, char* argv[]) {
	if (argc != 3) {
		return 1;
	}
	
	char* book_text = argv[1];
	char* instructions_text = argv[2];
	
	char** lines = (char**) malloc(LINES * sizeof(char*));
	
	for (int i = 0; i < LINES; i++) {
		lines[i] = (char*) malloc(CHAR * sizeof(char));
	}
	
	char* sentence = (char*) malloc(CHAR * sizeof(char));
	char* word;
	struct instruction_s* instructions;
	int size_instructions;
	
	FILE *alicePtr;
	FILE *instructionsPtr;
	
	if((alicePtr = fopen(book_text, "r")) == NULL) {
		puts("File could not be opened.");
	} else {
		read_book(alicePtr, lines);
		fclose(alicePtr);
	}
	
	if((instructionsPtr = fopen(instructions_text, "r")) == NULL) {
		puts("File could not be opened.");
	} else {
		size_instructions = number_of_lines(instructionsPtr);
		instructions = (struct instruction_s*) malloc(size_instructions * sizeof(struct instruction_s));
		read_instructions(instructionsPtr, instructions, size_instructions);
		fclose(instructionsPtr);
	}
	
	printf("\n");
	for (int i = 1; i <= size_instructions; i++) {
		get_sentence(lines, instructions, i, sentence);
		word = get_word(sentence, instructions[i - 1].index);
		str_lower(word);
		no_punctuation(word);
		printf(i == size_instructions ? "%s\n" : "%s ", word);
	}
	
	for (int i = 0; i < LINES; i++) {
		free(lines[i]);
	}
	free(lines);
	free(sentence);
	free(instructions);
	free(word);
	
	return 0;
}
