#define COLOR_RED "\033[0;31m"
#define COLOR_RESET "\033[0m"
#define COLOR_GREEN "\033[0;32m"
#define COLOR_CYAN "\033[0;36m"

#define NAME Martin
#define SEPARATOR_1 ":"
#define SEPARATOR_2 "$"
#define EMPTY " "


#define xstr(s) str(s)
#define str(s) #s
#define PROMPT COLOR_RED xstr(NAME) COLOR_GREEN xstr(DIR) xstr(SEPARATOR) COLOR_RESET EMPTY



typedef void (*fnct_ptr_type)(char**);

char* exec_commands[] = {
	"nano",
	"cat"
};
char* allowed_commands[] = {
	"ls",
	"rm",
	"cd",
	"pwd",
	"clear",
	"nano",
	"cat"
};




