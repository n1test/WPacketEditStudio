//Injects a dll into the specified process
//
// returns whether the injection succeeded
//
bool InjectDll(char *exe, char *dll);

//Ejects a dynamically loaded dll from the specified process
//
// returns whether the ejection succeeded
//
bool EjectDll(char *exe, char *dll);

//Injects a dll into every process
//
// returns the number of processes hooked
//
int HookAllProcesses(char *dll);

//unloads a dynamically loaded dll from every process (if possible)
//
// returns the number of processes unhooked
//
int UnhookAllProcesses(char *dll);
