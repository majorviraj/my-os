/*
 * file: command_proc.c
 */


struct command_db {
	char command[20];
	int number_of_args;
	char args;
};

/*
 * command_proc() - runs commands according to the input
 * 
 * @input_string: This is a null terminated string, containing space seperated
 * 			 the name of command and all the arguments.
 * 
 */
int command_proc(char* input_string)
{
	int args = -1;
	
	{
		int i = 0;
		while(*(input_string + i) != ' '){
		}
		*(input_string + i) == '\n';

	}
	
	for (int i = 0; *(input_string + i) != '\n' ;i++) {
		if (*(input_string + i) == ' '){
			args ++;
		}
	}

	// for(int i = 0; i < no_of_cmds; i++)
	// {
	// 	if(string_compare(input_string, command_db->command) == 0){
	// 	}
	// }

}

int string_compare(char* string1, char* string2)
{
	for (int i=0; (*(string1 + i) == '\n' || *(string2 + i) == '\n');i++){
		if (*(string1 + i) != *(string2 + i)) {
			return -1;
		}
	}
	return 0;
}
