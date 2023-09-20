#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h> 
#include "my_string.h"
#include "generic.h"
#include "generic_vector.h"

Status get_word_key_value(MY_STRING current_word_family, MY_STRING new_key, MY_STRING word, char guess);
void start(int* word_length, int* number_of_guesses, Boolean* remaining_words, GENERIC_VECTOR* words);
void clearKeyboardBuffer(void);
void evil_hangman_turn(GENERIC_VECTOR current_family, MY_STRING current_key, MY_STRING guesses, Boolean remaining_words);
char user_input(MY_STRING guesses);
void evil_hangman_game(void);


int main(int argc, char* argv[]) {
	Boolean cont;
	char c;

	do {
		evil_hangman_game();

		printf("*************************\n");

		printf("Would you like to play again (y/n): ");
		scanf("%c", &c);
		clearKeyboardBuffer();
	} while (c == 'y');
}

void evil_hangman_game(void) {
	FILE* dictionary = fopen("dictionary.txt", "r");
	GENERIC_VECTOR words[30];
	GENERIC_VECTOR current_family;
	MY_STRING current_key;
	MY_STRING guesses;
	MY_STRING temp = my_string_init_default();
	int i;
	int size;
	int word_length;
	int win = 0;
	int number_of_guesses;
	Boolean remaining_words;

	for (i = 0; i < 30; i++) {
		words[i] = generic_vector_init_default(my_string_assignment, my_string_destroy);
	}
	while (my_string_extraction(temp, dictionary)) {
		size = my_string_get_size(temp) - 1;

		if (size < 0) {
			break;
		}

		generic_vector_push_back(words[size], temp);
	}

	start(&word_length, &number_of_guesses, &remaining_words, words);
	srand(time(0));

	guesses = my_string_init_default();
	current_family = generic_vector_init_default(my_string_assignment, my_string_destroy);
	current_key = my_string_init_default();
	generic_vector_copy(current_family, words[word_length - 1]);

	for (i = 0; i < word_length; i++) {
		my_string_push_back(current_key, '-');
	}

	for (i = number_of_guesses; i > 0; i--) {
		printf("You have %d guesses left.\n", i);
		printf("Used letters: "); my_string_print(guesses); printf("\n");
		printf("Word: "); my_string_print(current_key); printf("\n");

		evil_hangman_turn(current_family, current_key, guesses, remaining_words);

		win = 1;
		for (int j = 0; j < my_string_get_size(current_key); j++) {
			if (*my_string_at(current_key, j) == '-') {
				win = 0;
				break;
			}
		}
		printf("*************************\n");
		if (win) {
			break;
		}
	}

	if (win) {
		printf("Congradulations you correctly guessed the word \n");
		my_string_print_s(generic_vector_at(current_family, 1));
	}
	else {
		printf("You ran out of guesses you lose\n");
		printf("The word was: "); my_string_print_s(generic_vector_at(current_family, (rand() % generic_vector_get_size(current_family))));
	}

	for (i = 0; i < 30; i++) {
		generic_vector_destroy(&words[i]);
	}
	my_string_destroy((ITEM*)&temp);
	my_string_destroy((ITEM*)&current_key);
	my_string_destroy((ITEM*)&guesses);
	generic_vector_destroy(&current_family);

	fclose(dictionary);
}


Status get_word_key_value(MY_STRING current_word_family, MY_STRING new_key, MY_STRING word, char guess) {
	
	while (my_string_pop_back(new_key));

	for (int i = 0; i < my_string_get_size(word); i++) {
		if (*my_string_at(word, i) == guess) {
			if (!my_string_push_back(new_key, guess)) {
				return Failure;
			}
		}
		else {
			if (!my_string_push_back(new_key, *my_string_at(current_word_family, i))) {
				return Failure;
			}
		}
	}

	return Success;
}

void start(int* word_length, int* number_of_guesses, Boolean* remaining_words, GENERIC_VECTOR* words) {
	int noc;
	char c;
	do {
		printf("Enter a word length: ");
		noc = scanf("%d", word_length);
		clearKeyboardBuffer();
	} while ((*word_length <= 0 || *word_length > 30) || noc != 1 || *word_length == 1 || *word_length == 23 || *word_length == 30 || *word_length == 25 || *word_length == 26 || *word_length == 27);

	do {
		printf("Enter how many guessed you want: ");
		noc = scanf("%d", number_of_guesses);
		clearKeyboardBuffer();
	} while (*number_of_guesses <= 0 || noc != 1);

	do {
		printf("Do you want to see the total remaning words (y/n): ");
		noc = scanf("%c", &c);
		clearKeyboardBuffer();
		*remaining_words = (c == 'y') ? TRUE : FALSE;

	} while (c != 'y' && c != 'n');


}

void clearKeyboardBuffer(void) {
	char c;
	scanf("%c", &c);
	while (c != '\n') {
		scanf("%c", &c);
	}
}

char user_input(MY_STRING guesses) {
	int noc;
	char c;
	Boolean char_used; 

	do {
		printf("Enter Guess: ");
		noc = scanf("%c", &c);
		clearKeyboardBuffer();

		if (my_string_get_size(guesses) == 0) {
			char_used = FALSE; 
		}
		else {
			for (int i = 0; i < my_string_get_size(guesses); i++) {
				if (*my_string_at(guesses, i) == c) {
					char_used = TRUE;
					break;
				}
				
				char_used = FALSE;
			}
		}
	} while (noc != 1 || char_used || !isalpha(c));

	my_string_push_back(guesses, c);
	return c;
}

void evil_hangman_turn(GENERIC_VECTOR current_family, MY_STRING current_key, MY_STRING guesses, Boolean remaining_words) {
	char guess = user_input(guesses);
	GENERIC_VECTOR generated_keys[500] = { NULL };
	MY_STRING temp = my_string_init_default();
	int x;
	int j;
	int index_max = 0;


	for (int i = 1; i < generic_vector_get_size(current_family); i++) {
		get_word_key_value(current_key, temp, (MY_STRING)generic_vector_at(current_family, i), guess);

		x = 1;
		for (j = 0; generated_keys[j]; j++) {
			if (!my_string_compare(temp, (MY_STRING)generic_vector_at(generated_keys[j], 0))) {
				x = 0;
				generic_vector_push_back(generated_keys[j], generic_vector_at(current_family, i));
			}
		}
		if (x) {
			generated_keys[j] = generic_vector_init_default(my_string_assignment, my_string_destroy);
			generic_vector_push_back(generated_keys[j], (ITEM)temp);
			generic_vector_push_back(generated_keys[j], generic_vector_at(current_family, i));
		}
	}

	if (remaining_words == TRUE) {
		for (j = 0; generated_keys[j]; j++) {
			printf("%d, ", generic_vector_get_size(generated_keys[j]) - 1);
			my_string_print_s(generic_vector_at(generated_keys[j], 0));
		}
	}

	for (j = 0; generated_keys[j]; j++) {
		if (generic_vector_get_size(generated_keys[index_max]) <= generic_vector_get_size(generated_keys[j])) {
			index_max = j;
		}
	}
	
	generic_vector_copy(current_family, generated_keys[index_max]);
	my_string_assignment(&current_key, generic_vector_at(generated_keys[index_max], 0));

	my_string_destroy(&temp);
	for (j = 0; generated_keys[j]; j++) {
		generic_vector_destroy(&generated_keys[j]);
	}
}