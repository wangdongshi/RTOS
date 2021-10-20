int var;

void func1(void)
{
	var = 1;
}

void func2(void)
{
	var = 2;
}

void libEntry(void)
{
	register char *got asm ("r9");
	char* gotBackup = got;
	got = (char*)0x08080000;

	var = 0;

	func1();
	func2();

	got = gotBackup;
}
