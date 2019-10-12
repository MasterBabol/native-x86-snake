#include "../sysinfo.h"

void init_keyboard( struct KEYBOARD_MASTER *km )
{
init_queue (&km->key_queue, km->key_queue_data, 256);
km->isinput = 0;
km->keyinput_enabled = 1;
}

static char keytable[0x80] = {
	0,   0,   '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', 0x08,   0,
	'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '[', ']', '\n',   0,   'A', 'S',
	'D', 'F', 'G', 'H', 'J', 'K', 'L', ';', '\'', '`',   0,   '\\', 'Z', 'X', 'C', 'V',
	'B', 'N', 'M', ',', '.', '/', 0,   '*', 0,   ' ', 0,   0,   0,   0,   0,   0,
	0,   0,   0,   0,   0,   0,   0,   '7', '8', '9', '-', '4', '5', '6', '+', '1',
	'2', '3', '0', '.', 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
	0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
	0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0
};

char last_key = 0;
void key_int()
{
struct SYSINFO *si = (struct SYSINFO *) SYSINFO_OFFSET;
struct KEYBOARD_MASTER *km = (struct KEYBOARD_MASTER *) &(si->keymaster);
char data = io_in8(PORT_KEYDAT);
waitforme();
km->isinput = 1;
if (data > 0xFF) { km->isinput = 0; data -= 0x80; }
goahead();
if (keytable[data] == 0) { io_out8(PIC0_OCW2, 0x61); return; }
dataput (&km->key_queue, keytable[data]);
last_key = keytable[data];
io_out8(PIC0_OCW2, 0x61);
}

char getch ()
{
 return last_key;
}

int kbhit ()
{
struct SYSINFO *si = (struct SYSINFO *) SYSINFO_OFFSET;
struct KEYBOARD_MASTER *km = (struct KEYBOARD_MASTER *) &(si->keymaster);
waitforme();
 if (km->isinput != 0)
 {
  km->isinput = 0;
  goahead();
  return 1;
 }
}