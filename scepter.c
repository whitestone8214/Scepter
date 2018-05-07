/*
	Copyright (C) 2015-2018 Minho Jo <whitestone8214@gmail.com>
	
	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.
	
	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.
	
	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

// gcc -O3 -std=gnu99 $(pkg-config --cflags --libs no8177) scepter.c


#include <no8177.h>

#include <pwd.h>


#define VERSION_SCEPTER 1


int main(int nElements, char **elements, char **conditions);
void action_green_dispose(void **data);

int main(int nElements, char **elements, char **conditions) {
	/* Check for commandline arguments */
	if (nElements < 2) return 0;
	if ((strcmp(elements[1], "help") == 0) || (strcmp(elements[1], "--help") == 0)) {printf("Usage: scepter [command]\n"); return 0;}
	
	/* Load /etc/scepter.ptn */
	byte *_textSettings = no8177_file_load("/etc/scepter.ptn", NULL);
	pair *_databaseSettings = no8177_database_from_string(_textSettings);
	action_green_dispose((void **)&_textSettings);
	
	/* Load mandatory setting(s) */
	byte *_valueAccountsAllowed = no8177_database_get_value(_databaseSettings, "accounts-allowed");
	if (_databaseSettings != NULL) no8177_database_dispose(_databaseSettings);
	element *_listAccountsAllowed = no8177_string_split(_valueAccountsAllowed, " ", 0);
	action_green_dispose((void **)&_valueAccountsAllowed);
	
	/* What's your name? */
	struct passwd *_treeYourName = getpwuid(getuid());
	int _nth;
	int _nAccountsAllowed = (_listAccountsAllowed != NULL) ? (no8177_element_length(_listAccountsAllowed) - 1) : -1;
	for (_nth = 0; _nth <= _nAccountsAllowed; _nth++) {
		byte *_name = (byte *)no8177_element_first(no8177_element_get(_listAccountsAllowed, _nth))->data;
		if (_name == NULL) continue;
		if (strcmp(_treeYourName->pw_name, _name) == 0) break;
	}
	if (_listAccountsAllowed != NULL) no8177_element_dispose(_listAccountsAllowed);
	if (_nth > _nAccountsAllowed) {printf("ERROR: You are not allowed to use this program.\n"); return 1;}
	
	/* Read and launch the command */
	byte *_command[nElements + 1]; _command[0] = "/bin/env"; _command[nElements] = NULL;
	for (_nth = 1; _nth < nElements; _nth++) _command[_nth] = elements[_nth];
	//for (_nth = 0; _nth < nElements; _nth++) printf("ARGUMENT %s\n", _command[_nth]);
	setuid(0);
	return execv("/bin/env", (char **)_command);
}
void action_green_dispose(void **data) {
	if (data == NULL) return;
	
	if (*data != NULL) free(*data);
}
